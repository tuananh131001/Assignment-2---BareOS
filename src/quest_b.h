void get_input(int m, int n, char args[][n], char *temp_str);
int get_command(int m, int n, char args[][n]);

void help_command();
int strcmp(char *s1, char *s2);
void clear_args(int m, int n, char args[][n]);

void loop_new_char(int *i, char *c, int *total_char, char **temp_str);
// command
void get_brdrev();
void cls();
void set_background_color(char background[]);
void set_text_color(char text[]);
// Util
int strlen(char *str);
void split_function(char *user_input, char **split);
void strcpy(char *src, char *dest);