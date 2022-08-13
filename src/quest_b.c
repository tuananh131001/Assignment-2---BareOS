#include "quest_b.h"

#include "mbox.h"
#include "uart.h"

void get_command() {
    static int CHAR_LIMIT = 40;
    char temp_str[CHAR_LIMIT];  // temp string to store the entire command
    char split_color[CHAR_LIMIT]
                    [CHAR_LIMIT];  // char array for storing color commands
    char split_screen[CHAR_LIMIT]
                     [CHAR_LIMIT];  // char array for storing color commands
    // Set Color Variables
    char set_color[CHAR_LIMIT];
    char set_type1[CHAR_LIMIT];
    char color1[CHAR_LIMIT];
    char set_type2[CHAR_LIMIT];
    char color2[CHAR_LIMIT];

    // Set screen Variables
    char set_screen[CHAR_LIMIT];
    char width[CHAR_LIMIT];
    char height[CHAR_LIMIT];
    char screen_type1[CHAR_LIMIT];

    get_input(temp_str);

    // Split the command into color  commands
    int y = 0;
    int z = 0;
    for (int x = 0; x <= sizeof(temp_str); x++) {
        // if space or NULL found, assign NULL into split[z]
        if (temp_str[x] == ' ' || temp_str[x] == '\0') {
            split_color[z][y] = '\0';
            z++;    // for next word
            y = 0;  // for next word, init index to 0
        } else {
            split_color[z][y] = temp_str[x];
            y++;
        }
    }

    strcpy(split_color[0], set_color);
    strcpy(split_color[1], set_type1);
    strcpy(split_color[2], color1);
    strcpy(split_color[3], set_type2);
    strcpy(split_color[4], color2);

    // Split the command into screen commands
    int y1 = 0;
    int z1 = 0;
    for (int x1 = 0; x1 <= sizeof(temp_str); x1++) {
        // if space or NULL found, assign NULL into split[z]
        if (temp_str[x1] == ' ' || temp_str[x1] == '\0') {
            split_screen[z1][y1] = '\0';
            z1++;    // for next word
            y1 = 0;  // for next word, init index to 0
        } else {
            split_screen[z1][y1] = temp_str[x1];
            y1++;
        }
    }
    strcpy(split_screen[0], set_screen);
    strcpy(split_screen[1], screen_type1);
    strcpy(split_screen[2], width);
    strcpy(split_screen[3], height);
    // End split

    int cmd = -1;
    static int TOTAL_CMD = 5;
    static char cmds[][20] = {"brdrev",   "cls",     "scrsize", "setcolor",
                              "help",     "clockrate", "macadr",  "draw",
                              "brdmodel", "pxlclk"};
    // compare input
    for (int i = 0; i <= TOTAL_CMD; i++) {
        // set_color reuse only take the first argument
        if (str_check(set_color, cmds[i])) {
            cmd = i;
            break;
        }
    }
    // check arguments counts
    switch (cmd) {
        case 0:
            get_brdrev();
            break;
        // clear screen
        case 1:
            cls();
            break;
        // Set Screen
        case 2:
            // command 5: scrsize <options>
            if (strcmp(screen_type1, "-p") == 0 && strcmp(width, '\0') != 0 &&
                strcmp(height, '\0') != 0) {
                int w = convert(width);
                int h = convert(height);
                physical_framebf_init(w, h);
            }

            else if (strcmp(screen_type1, "-v") == 0 &&
                     strcmp(width, '\0') != 0 && strcmp(height, '\0') != 0) {
                int w = convert(width);
                int h = convert(height);
                virtual_framebf_init(w, h);
            }

            else if (strcmp(screen_type1, "-b") == 0 &&
                     strcmp(width, '\0') != 0 && strcmp(height, '\0') != 0) {
                int w = convert(width);
                int h = convert(height);
                default_framebf_init(w, h);
            }
            break;
        // Setcolor
        case 3:
            if (strcmp(set_type1, "-t") == 0 && strcmp(color1, '\0') != 0) {
                uart_puts(color1);
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
        // Help
        case 4:
            help_function(temp_str);
            break;
            // clockrate
        case 5:
            get_clock_rate();
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

        // Reset strings for command setcolor
        split_color[k][k] = '\0';
        set_color[k] = '\0';
        set_type1[k] = '\0';
        color1[k] = '\0';
        set_type2[k] = '\0';
        color2[k] = '\0';

        // Reset strings for command scrsize
        split_screen[k][k] = '\0';
        set_screen[k] = '\0';
        screen_type1[k] = '\0';
        width[k] = '\0';
        height[k] = '\0';
    }
}
void get_input(char *temp_str) {
    int i = 0;
    int total_char = 0;  // total number of char that was input

    uart_puts("\nTuanAnhOS> ");
    while (1) {
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
            temp_str[total_char] = 0;
        }
    }
    uart_puts("Execute command: ");
    uart_puts(temp_str);
    uart_puts("\n");
}

// Utility
void help_function(char *temp_str) {
    if (strcmp(temp_str, "help setcolor") == 0) {
        uart_puts(
            "SETCOLOR\tSet text color, and/or background color of the "
            "	\n"
            "\t\tconsole to one of the following color: BLACK, RED, "
            "	\n"
            "\t\tGREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE 	"
            "		\n"
            "\t\tExample: setcolor -b yellow");

    } else if (strcmp(temp_str, "help cls") == 0) {
        uart_puts(
            "CLS\t\tClear screen (the terminal will scroll down to "
            "current position of the cursor). \n"
            "\t\tExample: cls");

    } else if (strcmp(temp_str, "help brdrev") == 0) {
        uart_puts(
            "BRDREV\t\tShow board revision \n"
            "\t\tExample: brdrev \n");

    } else if (strcmp(temp_str, "help scrsize") == 0) {
        uart_puts(
            "SCRSIZE\t\tSet screen size 			"
            "	  \n"
            "\t\tMust have options to set physical screen size (-p)\n"
            "\t\tor virtual screen size (-v), or both (by default) \n"
            "\t\tExample: scrsize -p 1024 768 (set physical screen) \n"
            "\t\tscrsize -v 1024 768 (set virtual screen)\n"
            "\t\tscrsize -b 1024 768 (set both physical and virtual "
            "screens)\n");

    } else if (strcmp(temp_str, "help clockrate") == 0) {
        uart_puts(
            "CLOCKRATE\t\tDisplay information of clock rate \n"
            "\t\tExample: armfreq \n");

    } else if (strcmp(temp_str, "help uartfreq") == 0) {
        uart_puts(
            "UARTFREQ\tDisplay information of UART frequency \n"
            "\t\tExample: uartfreq \n");

    } else if (strcmp(temp_str, "help brdmodel") == 0) {
        uart_puts(
            "BRDMODEL\tDisplay board model \n"
            "\t\tExample: brdmodel \n");

    } else if (strcmp(temp_str, "help firmware") == 0) {
        uart_puts(
            "FIRMWARE\tDisplay a version of current firmware \n"
            "\t\tExample: firmware \n");

    } else {
        uart_puts(
            "For more information on a specific command, type HELP "
            "command-name \n"
            "HELP\t\tShow brief information of all commands 	"
            "			  \n"
            "SETCOLOR\tSet text color, and/or background color 	 "
            "		  \n"
            "CLS\t\tClear screen				"
            "	"
            "					  \n"
            "BRDREV\t\tShow board revision			"
            "	"
            "					  \n"
            "SCRSIZE\t\tSet screen size 			"
            "	"
            "					  \n"
            //"brdrev", "cls", "scrsize", "setcolor",
            //"help","armfreq","macadr","draw","brdmodel","pxlclk"
            "ARMFREQ\t\tDisplay ARM frequency 			"
            "				  \n"
            "MACADR\t\tShow MAC address			"
            "				  \n"
            "DRAW\t\tDraw an circle on the screen 			"
            "	"
            "			  \n"
            "BRDMODEL\tShow board model		"
            "	"
            "			  \n"
            "PXLCLK\t\tShow clock frequency of pixel clock	 	"
            "			"
            "					  \n");
    }
}
void get_brdrev() {
    mBuf[0] =
        8 *
        4;  // Message Buffer Size in bytes (9 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST;  // Message Request Code (this is a request message)

    mBuf[2] = 0x00010002;  // TAG Identifier: Get clock rate
    mBuf[3] =
        4;  // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0;  // REQUEST CODE = 0
    mBuf[5] = 0;  // clear output buffer
    mBuf[6] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("Board revision: ");
        uart_hex(mBuf[5]);
    } else {
        uart_puts("Unable to query!\n");
    }
}
void get_clock_rate() {
    // mailbox data buffer: Read ARM frequency
    mBuf[0] =
        8 *
        4;  // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST;  // Message Request Code (this is a request message)
    mBuf[2] = 0x00030002;    // TAG Identifier: Get clock rate
    mBuf[3] = 8;             // Reponse length
    mBuf[4] = 0;             // REQUEST CODE = 0
    mBuf[5] = 0x000000003;   // clock id
    mBuf[6] = 0;             // clear output buffer for board revision
    mBuf[7] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("\nARM clock rate = ");
        uart_dec(mBuf[6]);
        uart_puts("\n");
    } else {
        uart_puts("\nUnable to get ARM clock rate!\n");
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