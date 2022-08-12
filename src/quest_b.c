#include "quest_b.h"

#include "mbox.h"
#include "uart.h"
void help_command() {
    uart_puts(
        "For more information on specific command type 'help <command>'\n");
    uart_puts("\n");
    uart_puts(
        "setcolor                                             Set  text  "
        "color,  "
        "and/or  background  color  of  the console\n");
    uart_puts("\n");
    uart_puts(
        "cls                                                  Clear  the  "
        "console.\n");
    uart_puts("\n");
    uart_puts(
        "brdrev                                               Show board "
        "revision.\n");
    uart_puts("\n");
    uart_puts(
        "scrsize <options>                                    Set Screen "
        "Size\n");
}
void split_function(char *user_input, char **split) {
    int y = 0;
    int z = 0;

    for (int x = 0; x <= sizeof(user_input); x++) {
        // if space or NULL found, assign NULL into split[z]
        if (user_input[x] == ' ' || user_input[x] == '\0') {
            split[z][y] = '\0';
            z++;    // for next word
            y = 0;  // for next word, init index to 0
        } else {
            split[z][y] = user_input[x];
            y++;
        }
    }
}

void get_command(int m, int n, char args[][n]) {
    char user_input[100];  // temp string to store the entire command
    char temp_str[100];
    char split_color[40][40];   // char array for storing color commands
    char split_screen[40][40];  // char array for storing color commands

    // Set Color Variables
    char set_color[40];
    char set_type1[40];
    char color1[40];
    char set_type2[40];
    char color2[40];

    // Set screen Variables
    char set_screen[40];
    char width[40];
    char height[40];
    char screen_type1[40];
    int y = 0;
    int z = 0;

    get_input(m, n, args, temp_str);
    // split_function(temp_str, split_color);

    for (int x = 0; x <= sizeof(temp_str); x++) {
        // if space or NULL found, assign NULL into split[z]
        if (temp_str[x] == ' ' || temp_str[x] == '\0') {
            split_color[z][y] = '\0';
            z++;    // for next word
            y = 0;  // for next word, init index to 0
        } else {
            user_input[x] = temp_str[x];
            split_color[z][y] = temp_str[x];
            y++;
        }
    }

    strcpy(split_color[0], set_color);
    strcpy(split_color[1], set_type1);
    strcpy(split_color[2], color1);
    strcpy(split_color[3], set_type2);
    strcpy(split_color[4], color2);
    // // split_function(temp_str, split_screen);
    // int y1 = 0;
    // int z1 = 0;
    // for (int x1 = 0; x1 <= sizeof(temp_str); x1++) {
    //     // if space or NULL found, assign NULL into split[z]
    //     if (str[x1] == ' ' || str[x1] == '\0') {
    //         split_screen[z1][y1] = '\0';
    //         z1++;    // for next word
    //         y1 = 0;  // for next word, init index to 0
    //     } else {
    //         split_screen[z1][y1] = str[x1];
    //         y1++;
    //     }
    // }
    strcpy(split_screen[0], set_screen);
    strcpy(split_screen[1], screen_type1);
    strcpy(split_screen[2], width);
    strcpy(split_screen[3], height);
    // ----- COMMANDS -----//
    if (user_input[0] == '\0') {
        uart_puts("");
    }
    int cmd = -1;
    static char cmds[][20] = {"brdrev", "cls", "scrsize", "setcolor"};
    // compare input
    uart_puts(split_screen);
    for (int i = 0; i <= 3; i++) {
        if (str_check(user_input, cmds[i])) {
            cmd = i;
            break;
        }
    }
    // check arguments counts
    switch (cmd) {
        case 0:
            get_brdrev();
            break;
        case 1:
            cls();
            break;
        case 2:

            if (strcmp(screen_type1, "-p") == 0 && strcmp(width, '\0') != 0 &&
                strcmp(height, '\0') != 0) {
                int w = convert(width);
                int h = convert(height);
                physical_framebf_init(w, h);
            } else if (strcmp(screen_type1, "-v") == 0 &&
                       strcmp(width, '\0') != 0 && strcmp(height, '\0') != 0) {
                int w = convert(width);
                int h = convert(height);
                virtual_framebf_init(w, h);
            } else if (strcmp(screen_type1, "-b") == 0 &&
                       strcmp(width, '\0') != 0 && strcmp(height, '\0') != 0) {
                int w = convert(width);
                int h = convert(height);
                virtual_framebf_init(w, h);
            }
        case 3:

            if (strcmp(set_type1, "-t") == 0 && strcmp(color1, '\0') != 0) {
                if (strcmp(set_type2, "-b") == 0 && strcmp(color2, '\0') != 0) {
                    set_text_color(color1);
                    set_background_color(color2);
                }
                set_text_color(color1);
            } else if (strcmp(set_type1, "-b") == 0 &&
                       strcmp(color1, '\0') != 0) {
                set_background_color(color1);
            }
            break;

        default:
            // if cmd == -1 => not found
            uart_puts("Invalid command. Please enter again!\n");
            break;
    }
    // reset string arrays
    for (int k = 0; k < 40; k++) {
        // Reset string input
        temp_str[k] = '\0';
        user_input[k] = '\0';

        // Reset strings for command setcolor
        split_screen[k][k] = '\0';
        set_color[k] = '\0';
        set_type1[k] = '\0';
        color1[k] = '\0';
        set_type2[k] = '\0';
        color2[k] = '\0';

        // Reset strings for command scrsize
        split_color[k][k] = '\0';
        set_screen[k] = '\0';
        screen_type1[k] = '\0';
        width[k] = '\0';
        height[k] = '\0';
    }
}
void get_input(int m, int n, char args[][n],
               char *temp_str) {  // temp string to store the entire command) {
    // clear arguments
    clear_args(m, n, args);

    int i = 0;  // index of string
    // variables to make sure backspace wouldn't delete the enter input message
    int total_char = 0;  // total number of char that was input

    uart_puts("\nTuanAnhOS> ");
    while (1) {
        // read each char
        char c = uart_getc();

        // if user presses ENTER, break the loop and execute a command
        if (c == 10) {
            uart_puts("\n");
            break;
        }

        // add each character into the string
        else if (c != 8) {
            uart_sendc(c);
            temp_str[total_char] = c;
            total_char++;
        }

        // delete each character when user press BackSpace each time
        // Use Ctrl + H on MacOS
        else if (c == 8 && total_char > 0) {
            uart_sendc(c);
            uart_sendc(32);
            uart_sendc(8);
            total_char--;
            temp_str[total_char] = 0;  // make the element at that index in the
                                       // string to be whitespace
        }
    }
    uart_puts("Execute command: ");
    uart_puts(temp_str);
    uart_puts("\n");
}

// split input string into arguments
int get_arguments(char *str, int m, int n, char args[][n]) {
    int i = 0;              // index of arguments
    int j = 0;              // index of character in argument string
    char *input_str = str;  // pointer pointing to input string
    int space_flag = 0;  // flag to check for space at the end of input string

    // clear arguments
    clear_args(m, n, args);

    // check space at first indices
    while (*input_str == ' ') {
        input_str++;  // go to next character, continue checking if it is '
    }
    while (*input_str != '\0') {  // util end
        if (*input_str != ' ') {
            args[i][j] = *input_str;  // add character to argument string
            space_flag = 0;           // reset space flag
            j++;  // go to the next character in the current argument string
        }

        // if *p = ' ' and the last character is not space
        if (*input_str == ' ' && !space_flag) {
            space_flag = 1;  // set space flag
            i++;             // go to the next argument
            j = 0;  // reset to the first character in the next argument string
        }

        // Increment to the next character in input string
        input_str++;
    }
    // if there are spaces at the end of input string, the number of arguments
    // is
    // i
    return (space_flag == 0) ? (i + 1) : i;
}

// Utility
void get_brdrev()

{
    // mailbox data buffer: Read ARM frequency
    mBuf[0] =
        7 *
        4;  // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST;  // Message Request Code (this is a request message)
    mBuf[2] = 0x00010002;    // TAG Identifier: Get clock rate
    mBuf[3] =
        4;  // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0;  // REQUEST CODE = 0
    mBuf[5] = 0;  // clear output buffer for board revision
    mBuf[6] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("\nBoard revision = ");
        uart_hex(mBuf[5]);
        uart_puts("\n");
    } else {
        uart_puts("\nUnable to get board revision!\n");
    }
}
// Utility
void cls() { uart_puts("\033[H\033[J"); }
/**
 * Set color for text.
 */
void set_text_color(char text[]) {
    if (strcmp(text, "black") == 0) {
        uart_puts("\033[30;1m");
    } else if (strcmp(text, "red") == 0) {
        uart_puts("\033[31;1m");
    } else if (strcmp(text, "green") == 0) {
        uart_puts("\033[32;1m");
    } else if (strcmp(text, "yellow") == 0) {
        uart_puts("\033[33;1m");
    } else if (strcmp(text, "blue") == 0) {
        uart_puts("\033[34;1m");
    } else if (strcmp(text, "purple") == 0) {
        uart_puts("\033[35;1m");
    } else if (strcmp(text, "cyan") == 0) {
        uart_puts("\033[36;1m");
    } else if (strcmp(text, "white") == 0) {
        uart_puts("\033[37;1m");
    }
}

/**
 * Set color for background.
 */
void set_background_color(char background[]) {
    if (strcmp(background, "black") == 0) {
        uart_puts("\033[40;1m");
    } else if (strcmp(background, "red") == 0) {
        uart_puts("\033[41;1m");
    } else if (strcmp(background, "green") == 0) {
        uart_puts("\033[42;1m");
    } else if (strcmp(background, "yellow") == 0) {
        uart_puts("\033[43;1m");
    } else if (strcmp(background, "blue") == 0) {
        uart_puts("\033[44;1m");
    } else if (strcmp(background, "purple") == 0) {
        uart_puts("\033[45;1m");
    } else if (strcmp(background, "cyan") == 0) {
        uart_puts("\033[46;1m");
    } else if (strcmp(background, "white") == 0) {
        uart_puts("\033[47;1m");
    }
}

/*
 * return 1: identical
 * return 0: different
 */
/**
 * Comparing 2 strings.
 */
int strcmp(char *a, char *b) {
    int flag = 0;
    while (*a != '\0' && *b != '\0')  // while loop
    {
        for (int i = 0; i < sizeof(a); i++) {
            if (*a != *b) {
                flag = 1;
            }
            a++;
            b++;
        }
    }
    if (flag == 0) {
        return 0;
    } else {
        return 1;
    }
}
int str_check(char *str1, char *str2) {
    // compare length
    if (strlen(str1) != strlen(str2)) return 0;

    char *p1 = str1;  // pointer pointing to first character in str1
    char *p2 = str2;  // pointer pointing to first character in str2

    while (*p1 != '\0' && *p2 != '\0') {
        if (*p1 != *p2) return 0;

        p1++;
        p2++;
    }
    return 1;
}
void clear_args(int m, int n, char args[][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            args[i][j] = '\0';
        }
    }
}
/**
 * String copy.
 */
void strcpy(char *src, char *dest) {
    while (*src) {
        *(dest++) = *(src++);
    }
    *dest = '\0';
}
/**
 * Convert string char to int.
 */
int convert(char s[]) {
    int i, n = 0;

    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++) n = 10 * n + (s[i] - '0');

    return n;
}
