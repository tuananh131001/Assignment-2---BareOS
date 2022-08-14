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
    // mailbox data buffer: Read ARM frequency
    mBuf[0] =
        8 *
        4;  // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST;  // Message Request Code (this is a request message)
    mBuf[2] = 0x00030002;    // TAG Identifier: Get clock rate
    mBuf[3] =
        8;  // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0;  // REQUEST CODE = 0
    mBuf[5] = 3;  // clock id: ARM system clock
    mBuf[6] = 0;  // clear output buffer (response data are mBuf[5] & mBuf[6])
    mBuf[7] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("Response Code for whole message: ");
        uart_hex(mBuf[1]);
        uart_puts("\n");

        uart_puts("Response Code in Message TAG: ");
        uart_hex(mBuf[4]);
        uart_puts("\n");

        uart_puts("DATA: ARM clock rate = ");
        uart_dec(mBuf[6]);
        uart_puts("\n");

    } else {
        uart_puts("Unable to query!\n");
    }
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