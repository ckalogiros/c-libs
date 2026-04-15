#include <stdio.h>
#include "logger.h"
// #include "logger_to_html.h"
// #include "logger_to_html_css.h"


#define DEBUG_HTML_DIR "/data/projects/clion/logger/logger_debug.html"

void dummy_write_fn1(const uint32 class)
{
    // printhtml_group_append_code(HTML_GROUP_1, class, "<p>This is dummy_write_fn1</p>\n");
}
void dummy_write_fn2()
{
    // printhtml_group_append_code(HTML_GROUP_1, 0, "<p>This is dummy_write_fn2</p>\n");
}


int
main(void)
{

    // log_to_html_init();
    //
    // // dummy_write_fn1(0);
    //
    //
    // printhtml_group_append_code(HTML_GROUP_1, 0, "<p class=\"blue3\";>This is dummy_write_fn1</p>\n");
    //
    //
    // printhtml_close(DEBUG_HTML_DIR);
    //
    // printhtml_destroy();

    print("This is %syellow%s color.", YELLOW, CLEAR);

    return 0;
}
