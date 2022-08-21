#include "../gcclib/stdarg.h"
#include "../gcclib/stddef.h"
#include "../gcclib/stdint.h"

void printf(char *string, ...);
void clear(char* s);
unsigned long int power(unsigned int base, unsigned int pow);
void clear_buffer(char* str);
int atoi(char s1[]) ;
int find_length(char* s1);
int find_larger(int s1, int s2);
void sub_string(char* s, char sub[], int first_index, int last_index);
int find_dash(char* string, char delimeter[]);
int find_space(char* string);
int find_space_and_eof(char* string);