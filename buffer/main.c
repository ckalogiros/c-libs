#include <stdio.h>

#include "logger.h"
#include "mem_man_interface.h"
#include "buffer.h"
#include "buffer_log.h"
#include "precision_timer.h"

void
keyboard_input1();

void
keyboard_input2();

int
main(void)
{
    memman_init();


    keyboard_input2();


    memman_terminate();
    return 0;
}

enum KEYS {
    X = 'x',
};

void
keyboard_input1()
{
    int charIn;

    print("Enter a character: ");

    // Collect input
    charIn = getchar();

    printf("\nYou entered: ");
    putchar(charIn);
}

void
keyboard_input2()
{
    // Create an int and a char variable
    int myNum;
    char myChar;

    // Ask the user to type a number AND a character
    printf("Type a number AND a character and press enter: \n");

    // Get and save the number AND character the user types
    scanf("%d %c", &myNum, &myChar);

    // Print the number
    printf("Your number is: %d\n", myNum);

    // Print the character
    printf("Your character is: %c\n", myChar);
}
