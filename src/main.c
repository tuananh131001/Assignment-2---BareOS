// -----------------------------------main.c
// -------------------------------------
#include "mbox.h"
#include "uart.h"
// include welcome
#include "printf.h"
#include "quest_a.h"
#include "quest_b.h"

void main() {
    // set up serial console
    uart_init();

    // welcome
    print_welcome();

    uart_puts("\n\n");
    /*
    Test with repl.it:
Decimals: 1977
Characters: a A 
String: Hello 
Float: 3.142
Hex: 64
Octal: 144
Percent: %
Width:       3232
Check precious: 32.14 
*/
    printf("Decimals: %d\n", 1977);
    printf("Characters: %c %c \n", 'a', 65);
    printf("String: %s \n", "Hello");
    printf("Float: %f \n", 3.14159265);
    printf("Hex: %x\n", 100);
    printf("Octal: %o\n", 100);
    printf("Percent: %%\n");
    printf("Width: %10d ", 3232);
    printf("\nCheck precious: %.2f \n", 32.1416);


    while (1) {
        get_command();
    }
}

// #include "uart.h"
// #include "mbox.h"
// #include "framebf.h"

// void main()
// {
//     // set up serial console
//  uart_init();

//  // say hello
//  uart_puts("\n\nHello World!\n");

//  // Initialize frame buffer
//  framebf_init();

//  // Draw something on the screen
// //  drawRectARGB32(100,100,400,400,0x00AABB00,1); //RED
// //  drawRectARGB32(150,150,400,400,0x0000BB00,1); //GREEN
// //  drawRectARGB32(200,200,400,400,0x000000CC,1); //BLUE
// //  drawRectARGB32(250,250,400,400,0x00FFFF00,1); //YELLOW
// //  drawPixelARGB32(300, 300, 0x00FF0000); //RED
//  drawLineARGB32(0, 300, 0x00FF0000, 1); //RED
//  // draw circle yellow
//     drawCircleARGB32(300, 300, 100, 0x00FFFF00, 0); //YELLOW
// //  drawCircleARGB32(300, 300, 100, 0x00AABB00,0);

//  // echo everything back
//  while(1) {
//   //read each char
//   char c = uart_getc();

//   //send back twice
//   uart_sendc(c);
//   uart_sendc(c);
//  }
// }
int strlen(char *str) {
    int i = 0;

    while (str[i] != '\0') {
        i++;
    }

    return i;
}