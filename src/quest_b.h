void get_input(char *temp_str);
void get_command();

void help_command();
int strcmp(char *s1, char *s2);

void loop_new_char(int *i, char *c, int *total_char, char **temp_str);
// command
void help_function(char *temp_str);
void get_brdrev();
void cls();
void set_background_color(char background[]);
void set_text_color(char text[]);
void get_clock_rate();
int compare_str(char *str, char *substr);
void get_vcmemory();
void draw();
void get_board_model();
void get_pixel_clock();
// Util
int strlen(char *str);
void split_function(char *user_input, char **split);
void strcpy(char *src, char *dest);
int str_check(char *str1, char *str2);