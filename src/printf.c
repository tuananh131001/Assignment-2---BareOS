#define SPACE 0x20
#include "printf.h"

#include "uart.h"

#define MAX_PRINT_SIZE 256

void printf(char* string, ...) {
    va_list ap;
    va_start(ap, string);

    char buffer[MAX_PRINT_SIZE];
    int buffer_idx = 0;

    char temp_buffer[MAX_PRINT_SIZE];
    clear_buffer(buffer);
    while (1) {
        if (*string == 0) break;

        if (*string == '%') {
            string++;
            if (*string == 'd') {
                /*
                1. If the number is negative, it puts a '-' in the buffer.
                2.  It then converts the number to a string and puts it in the
                buffer.
                3.  It then prints the buffer in decimal number. */
                string++;

                int x = va_arg(ap, int);
                int temp_index = MAX_PRINT_SIZE - 1;

                if (x < 0) {
                    x *= (-1);
                    buffer[buffer_idx] = '-';
                    buffer_idx++;
                }

                do {
                    temp_buffer[temp_index] = (x % 10) + '0';
                    temp_index--;
                    x /= 10;
                } while (x != 0);

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }

            } else if (*string == 'x') {
                /*
                1. Convert the integer to a hex string.
                2. Reverse the string.
                3. Copy the string to the buffer. */
                string++;
                int x = va_arg(ap, int);
                int temp_index = MAX_PRINT_SIZE - 1;

                while (x != 0) {
                    if ((x % 16) < 10) {
                        temp_buffer[temp_index] = 48 + (x % 16);
                    } else {
                        temp_buffer[temp_index] = 55 + (x % 16);
                    }
                    temp_index--;
                    x /= 16;
                }

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }

            }

            else if (*string == 'o') {
                /*
                1. The first argument is the format string.
                2. The remaining arguments are the values to be formatted.
                3. The function returns the number of characters written to the
                buffer, not including the null character. */
                string++;

                int x = va_arg(ap, int);

                static char hex[] = "01234567";
                int temp_index = MAX_PRINT_SIZE - 1;

                do {
                    temp_buffer[temp_index] = hex[x % 8];
                    temp_index--;
                    x /= 8;
                } while (x != 0);

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
            }

            else if (*string == 'c') {
                // Print the character
                string++;

                int c = va_arg(ap, int);
                buffer[buffer_idx] = c;
                buffer_idx++;
            }

            else if (*string == 's') {
                // Print the string
                string++;
                char* s = va_arg(ap, char*);
                while (*s != '\0') {
                    buffer[buffer_idx] = *s++;
                    buffer_idx++;
                }
            }

            else if (*string == 'f') {
                /*
                1. Convert the double to an integer.
                2. Convert the integer to a string.
                3. Copy the string to the buffer.
                4. If the double has a decimal point, copy the decimal point to
                the buffer.
                5. Copy the buffer to the print buffer. */
                string++;
                double f = va_arg(ap, double);
                int int_f = (double)f;
                int dot_position = 0;
                int number_precision = 10000;
                for (; int_f != 0; dot_position++) {
                    int_f /= 10;
                }
                int x = (int)(f * number_precision + 5);
                x /= 10;
                int temp_index = MAX_PRINT_SIZE - 1;

                do {
                    temp_buffer[temp_index] = (x % 10) + 48;
                    temp_index--;
                    x /= 10;
                } while (x != 0);

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    if (i == (dot_position + temp_index + 1)) {
                        buffer[buffer_idx] = '.';
                        buffer_idx++;
                    }
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
            }

            else if (*string == '%') {
                // Print percent sign
                buffer[buffer_idx] = *(string++);
                buffer_idx++;
            }

        } else if (*string >= 49 && *string <= 57) {
            /*
            1. Reads the format string and stores it in a buffer.
            2. Reads the arguments and stores them in a buffer.
            3. Prints the buffer. */
            char before_type[50];
            sub_string(string, before_type, 0, find_space_and_eof(string) - 1);

            string++;

            char width[100];
            int i = 0;
            int precision_flag = 0;

            char* p = before_type;
            while (*(p) != '.') {
                if (*(p) == before_type[find_length(before_type) - 1]) {
                    precision_flag = 0;
                    break;
                }
                width[i++] = (*p);

                p++;
                precision_flag = 1;
                string++;
            }

            char precision[100];
            if (precision_flag == 1) {
                int j = 0;

                while (*(++p) != before_type[find_length(before_type) - 1]) {
                    precision[j++] = *(p);

                    string++;
                }
            }

            if (before_type[find_length(before_type) - 1] == 's') {
                char* x = va_arg(ap, char*);

                int string_len = find_length(x);

                int temp;

                if (atoi(precision) > string_len) {
                    temp = string_len;
                } else {
                    temp = atoi(precision);
                }

                for (int z = 0; z < atoi(width) - temp; z++) {
                    buffer[buffer_idx] = ' ';
                    buffer_idx++;
                }

                for (int i = 0; i < temp; i++) {
                    buffer[buffer_idx] = *(x++);
                    buffer_idx++;
                }
            }

            if (before_type[find_length(before_type) - 1] == 'c') {
                for (int z = 0; z < atoi(width) - 1; z++) {
                    buffer[buffer_idx] = ' ';
                    buffer_idx++;
                }

                int x = va_arg(ap, int);

                buffer[buffer_idx] = x;
                buffer_idx++;
            }

            if (before_type[find_length(before_type) - 1] == 'x') {
                int x = va_arg(ap, int);

                static char hex[] = "0123456789abcdef";
                int temp_index = MAX_PRINT_SIZE - 1;
                int buffer_temp_len = 0;

                do {
                    temp_buffer[temp_index] = hex[x % 16];
                    temp_index--;
                    x /= 16;
                    buffer_temp_len++;
                } while (x != 0);

                for (int z = 0; z < atoi(width) - find_larger(atoi(precision),
                                                              buffer_temp_len);
                     z++) {
                    buffer[buffer_idx] = ' ';
                    buffer_idx++;
                }

                for (int j = 0; j < atoi(precision) - buffer_temp_len; j++) {
                    buffer[buffer_idx] = '0';
                    buffer_idx++;
                }

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
            }

            if (before_type[find_length(before_type) - 1] == 'f') {
                float f = va_arg(ap, double);
                int multiplier = 1;

                int temp_len = -1;

                if (atoi(precision) != 0) {
                    for (int i = 0; i < atoi(precision); i++) {
                        multiplier *= 10;
                    }
                } else {
                    temp_len = 6;
                }

                long long convert_int = f * multiplier;

                int flag_negative = 0;
                if (convert_int < 0) {
                    flag_negative = 1;
                    convert_int *= (-1);
                }

                int temp_index = MAX_PRINT_SIZE - 1;
                int count = 0;
                int buffer_temp_len = 0;

                do {
                    if ((count == atoi(precision) && atoi(precision) != 0) ||
                        count == temp_len) {
                        temp_buffer[temp_index] = '.';
                        temp_index--;
                        count++;
                        continue;
                    }
                    temp_buffer[temp_index] = (convert_int % 10) + '0';
                    temp_index--;
                    count++;
                    convert_int /= 10;
                    buffer_temp_len++;
                } while (convert_int != 0);

                for (int z = 0;
                     z < atoi(width) - buffer_temp_len - 1 - flag_negative;
                     z++) {
                    buffer[buffer_idx] = ' ';
                    buffer_idx++;
                }

                if (flag_negative == 1) {
                    buffer[buffer_idx] = '-';
                    buffer_idx++;
                }

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
            }

            if (before_type[find_length(before_type) - 1] == 'd') {
                int x = va_arg(ap, int);
                int temp_index = MAX_PRINT_SIZE - 1;

                int buffer_temp_len = 0;
                do {
                    temp_buffer[temp_index] = (x % 10) + '0';
                    temp_index--;
                    buffer_temp_len++;
                    x /= 10;
                } while (x != 0);

                for (int z = 0; z < atoi(width) - find_larger(atoi(precision),
                                                              buffer_temp_len);
                     z++) {
                    buffer[buffer_idx] = ' ';
                    buffer_idx++;
                }

                for (int j = 0; j < atoi(precision) - buffer_temp_len; j++) {
                    buffer[buffer_idx] = '0';
                    buffer_idx++;
                }

                int i;
                for (i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
            }

        } else if (*string == '.') {
            /*
            1. if the format is %d, then the next argument is an integer, and
            the next argument is an integer.
            2. if the format is %x, then the next argument is an integer, and
            the next argument is an integer.
            3. if the format is %f, then the next argument is a double, and the
            next argument is a double.
            4. if the format is %s, then the next argument is a string, and the
            next argument is a string. */
            char before_type[50];
            sub_string(string, before_type, 0, find_space_and_eof(string) - 1);

            string++;
            char* p = before_type;

            char precision[100];

            int j = 0;

            while (*(++p) != before_type[find_length(before_type) - 1]) {
                precision[j++] = *(p);

                string++;
            }

            if (before_type[find_length(before_type) - 1] == 'd') {
                int x = va_arg(ap, int);
                int temp_index = MAX_PRINT_SIZE - 1;

                int buffer_temp_len = 0;
                do {
                    temp_buffer[temp_index] = (x % 10) + '0';
                    temp_index--;
                    buffer_temp_len++;
                    x /= 10;
                } while (x != 0);

                for (int j = 0; j < atoi(precision) - buffer_temp_len; j++) {
                    buffer[buffer_idx] = '0';
                    buffer_idx++;
                }

                int i;
                for (i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
                string++;
            }

            if (before_type[find_length(before_type) - 1] == 'x') {
                int x = va_arg(ap, int);

                static char hex[] = "0123456789abcdef";
                int temp_index = MAX_PRINT_SIZE - 1;
                int buffer_temp_len = 0;

                do {
                    temp_buffer[temp_index] = hex[x % 16];
                    temp_index--;
                    x /= 16;
                    buffer_temp_len++;
                } while (x != 0);

                for (int j = 0; j < atoi(precision) - buffer_temp_len; j++) {
                    buffer[buffer_idx] = '0';
                    buffer_idx++;
                }

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
                string++;
            }

            if (before_type[find_length(before_type) - 1] == 'f') {
                double f = va_arg(ap, double);

                int multiplier = 1;

                int temp_len = -1;

                if (atoi(precision) != 0) {
                    for (int i = 0; i < atoi(precision); i++) {
                        multiplier *= 10;
                    }
                } else {
                    temp_len = 6;
                }

                unsigned long convert_int = f * multiplier;

                int flag_negative = 0;
                if (convert_int < 0) {
                    flag_negative = 1;
                    convert_int *= (-1);
                }

                int temp_index = MAX_PRINT_SIZE - 1;
                int count = 0;
                int buffer_temp_len = 0;

                do {
                    if ((count == atoi(precision) && atoi(precision) != 0) ||
                        count == temp_len) {
                        temp_buffer[temp_index] = '.';
                        temp_index--;
                        count++;
                        continue;
                    }
                    temp_buffer[temp_index] = (convert_int % 10) + '0';
                    temp_index--;
                    count++;
                    convert_int /= 10;
                    buffer_temp_len++;
                } while (convert_int != 0);

                if (flag_negative == 1) {
                    buffer[buffer_idx] = '-';
                    buffer_idx++;
                }

                for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
                    buffer[buffer_idx] = temp_buffer[i];
                    buffer_idx++;
                }
                string++;
            }

            if (before_type[find_length(before_type) - 1] == 's') {
                char* x = va_arg(ap, char*);

                int string_len = find_length(x);

                int temp;

                if (atoi(precision) > string_len) {
                    temp = string_len;
                } else {
                    temp = atoi(precision);
                }

                for (int i = 0; i < temp; i++) {
                    buffer[buffer_idx] = *(x++);
                    buffer_idx++;
                }
                string++;
            }

        } else {
            buffer[buffer_idx] = *string;
            buffer_idx++;
            string++;
        }

        if (buffer_idx == MAX_PRINT_SIZE - 1) break;
    }

    va_end(ap);

    uart_puts(buffer);
}

// function ulity
// reference: stackoverflow
void clear(char* s) {
    while (*s != '\0') {
        *(s++) = '\0';
    }
}

unsigned long int power(unsigned int base, unsigned int pow) {
    if (pow != 0) {
        return (base * power(base, pow - 1));
    } else
        return 1;
}
void clear_buffer(char* str) {
    char* p = str;
    while (*p != '\0') {
        *p = '\0';
        p++;
    }
}
int atoi(char s1[]) {
    int coverted_int = 0, offset = 0, c;

    for (c = offset; s1[c] != '\0'; c++) {
        coverted_int = coverted_int * 10 + s1[c] - '0';
    }

    return coverted_int;
}

int find_length(char* s1) {
    int count = 0;
    while (*s1 != '\0') {
        count++;
        s1++;
    }
    return count;
}

int find_larger(int s1, int s2) { return s1 > s2 ? s1 : s2; }

void sub_string(char* s, char sub[], int first_index, int last_index) {
    int i = 0;

    while (first_index <= last_index) {
        sub[i] = s[first_index];
        first_index++;
        i++;
    }
    sub[i] = '\0';
}

int find_dash(char* string, char delimeter[]) {
    int count = 0;

    while (*string != '\0') {
        if (*string == delimeter[0]) {
            if (*(++string) == delimeter[1]) {
                count++;
                return count;
            }
            count++;
            continue;
        }

        string++;
        count++;
    }
    return -1;
}

int find_space(char* string) {
    int count = 0;

    while (*string != '\0') {
        if (*string == SPACE) {
            return count;
        }

        string++;
        count++;
    }
    return -1;
}

int find_space_and_eof(char* string) {
    int count = 0;

    while (*string != '\0') {
        if (*string == SPACE) {
            return count;
        }

        string++;
        count++;
    }
    return count;
}
