#include <stdio.h>
#include "src/logger.h"


int
main(void)
{
    // Combined styles
    // printf("%s%sBold red text%s\n", get_color("bold"), get_color("red"), get_color("reset"));

    // logger_print_sample1();
    logger_test_variadic(3, 10, 'a', "abcde");

    return 0;
}
