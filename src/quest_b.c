#include "mbox.h"
#include "uart.h"
#include "quest_b.h"

void help_command()
{
    uart_puts("For more information on specific command type 'help <command>'\n");
    uart_puts("\n");
    uart_puts("setcolor                                             Set  text  color,  and/or  background  color  of  the console\n");
    uart_puts("\n");
    uart_puts("cls                                                  Clear  the  console.\n");
    uart_puts("\n");
    uart_puts("brdrev                                               Show board revision.\n");
    uart_puts("\n");
    uart_puts("scrsize <options>                                    Set Screen Size\n");
}
void clear_args(int m, int n, char args[][n])
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            args[i][j] = '\0';
        }
    }
}

// compare string return 0 or 1
int strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
        {
            return 0;
        }
        i++;
    }
    if (s1[i] == '\0' && s2[i] == '\0')
    {
        return 1;
    }
    return 0;
}

// split input string into arguments
int get_arguments(char *str, int m, int n, char args[][n])
{
    int i = 0;          // index of arguments
    int j = 0;          // index of character in argument string
    char *p = str;      // pointer pointing to input string
    int space_flag = 0; // flag to check for space at the end of input string

    // clear arguments
    clear_args(m, n, args);

    // check space at first indices
    while (*p == ' ')
    {
        p++; // go to next character, continue checking if it is ' '
    }

    while (*p != '\0')
    {
        // if *p != ' '
        if (*p != ' ')
        {
            args[i][j] = *p; // add character to argument string
            space_flag = 0;  // reset space flag
            j++;             // go to the next character in the current argument string
        }

        // if *p = ' ' and the last character is not space
        if (*p == ' ' && !space_flag)
        {
            space_flag = 1; // set space flag
            i++;            // go to the next argument
            j = 0;          // reset to the first character in the next argument string
        }

        // Increment to the next character in input string
        p++;
    }
    // if there are spaces at the end of input string, the number of arguments is i
    return (space_flag == 0) ? (i + 1) : i;
}

int get_input(int m, int n, char args[][n])
{
    // clear arguments
    clear_args(m, n, args);

    char c = 0;         // initialize char c
    int i = 0;          // index of string
    char temp_str[100]; // temp string to store the entire command

    // variables to make sure backspace wouldn't delete the enter input message
    int total_char = 0; // total number of char that was input

    uart_puts("\nTuanAnhOS> ");

    // get first char
    c = uart_getc();

    // if first char is not ' ' or backspace => add first char to input string
    if (c != ' ' && c != 8)
    {
        // send back to display the first character
        uart_sendc(c);
        total_char++; // increase total char displayed on the terminal

        // add first char
        temp_str[i] = c;

        // increment to the next character
        i++;
    }
    else
    { // else check first char is backspace OR ' '
        while (c == 8 || c == ' ')
        {
            // if char is ' '
            if (c == ' ')
            {
                // send back to display the first space
                uart_sendc(c);
                total_char++; // increase total char displayed on the terminal
            }
            else if (c == 8)
            { // if char is backspace
                uart_puts("\b \b"); // send backspace and two spaces
                if (total_char > 0)
                {
                    uart_sendc(c); // send back backspace
                    total_char--;
                }
            }

            // get next char
            c = uart_getc();

            // if c is normal character => add first char to input string
            if (c != ' ' && c != 8)
            {
                uart_sendc(c); // send back to display character
                total_char++;  // increase total char displayed on the terminal

                // add first char
                temp_str[i] = c;

                // increment to the next character
                i++;
            }
        }
    }
    // loop until new line character
    while (c != 10)
    {
        // read each char
        c = uart_getc();

        // if backspace will delete input message, continue
        if (c == 8 && total_char == 0)
        {
            continue;
        }

        // send back
        uart_sendc(c);

        // check if backspace is pressed, go back 1 character
        if (c == 8)
        {
            // send backspace
            uart_puts("\b\x20\b");

            // make sure i > 0, then delete
            // case f\b\bf
            if (i > 0)
            {
                // delete/clear last character in string
                temp_str[i - 1] = '\0';

                // go back 1 character (go to last character that just has been cleared)
                // if character is the first at new string
                i--;
            }

            // delete 1 char => total char has 1 less char
            total_char--;

            // continue to get the next character
            continue;
        }

        // if char is not backspace, increase total char
        total_char++; // increase total char

        // add to temp str
        temp_str[i] = c;

        // go to next element
        i++;
    }

    // add null to mark end of string
    temp_str[i - 1] = '\0';

    return get_arguments(temp_str, m, n, args);
}

int get_command(int m, int n, char args[][n])
{
    int cmd = -1;
    m = get_input(m, n, args);
    static char cmds[][20] = {"brdrev", "cls", "scrsize", "setcolor"};
    // compare input
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(args[0], cmds[i]))
        {
            cmd = i;
            break;
        }
    }
    // check arguments counts
    switch (cmd)
    {
    case 0:
        if (m != 1)
        {
            uart_puts("Invalid brdrev arguments. Please enter again!\n");
            return -1;
        }
        else
        {
            get_brdrev();
        }
        break;

        // case 1:
        // 	if (m != 1) {
        // 		uart_puts("Invalid cls arguments. Please enter again!\n");
        // 		return -1;
        // 	} else {
        // 		clear_screen();
        // 	}
        // 	break;

        // case 2:
        // 	if (m != 3 && m != 4 && m != 7) {
        // 		uart_puts("Invalid scrsize arguments. Please enter again!\n");
        // 		return -1;
        // 	} else {
        // 		set_scrsize(m, n, args);
        // 	}
        // 	break;

        // case 3:
        // 	if (m != 1 && m != 2) {
        // 		uart_puts("Invalid help arguments. Please enter again!\n");
        // 		return -1;
        // 	} else {
        // 		print_help_msg(m, args[1]);
        // 	}
        // 	break;

    default:
        // if cmd == -1 => not found
        uart_puts("Invalid command. Please enter again!\n");
        break;
    }
}
void get_brdrev()
{
    // mailbox data buffer: Read ARM frequency
    mBuf[0] = 7 * 4;        // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mBuf[2] = 0x00010002;   // TAG Identifier: Get clock rate
    mBuf[3] = 4;            // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0;            // REQUEST CODE = 0
    mBuf[5] = 0;            // clear output buffer for board revision
    mBuf[6] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("\nBoard revision = ");
        uart_hex(mBuf[5]);
        uart_puts("\n");
    }
    else
    {
        uart_puts("\nUnable to get board revision!\n");
    }
}
