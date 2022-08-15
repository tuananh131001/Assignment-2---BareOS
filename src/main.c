// -----------------------------------main.c
// -------------------------------------
#include "mbox.h"
#include "uart.h"
// include welcome
#include "quest_a.h"
#include "quest_b.h"

void main() {
    // set up serial console
    uart_init();

    // welcome
    print_welcome();
    printf("Characters: %c %c \n", 'a', 65);
    printf("Decimals: %d %ld\n", 1977, 650000L);
    printf("Preceding with blanks: %10d \n", 1977);
    printf("Preceding with zeros: %010d \n", 1977);
    printf("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
    printf("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
    printf("Width trick: %*d \n", 5, 10);
    printf("%s \n", "A string");

    uart_puts("\n\n");
    // echo everything back
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