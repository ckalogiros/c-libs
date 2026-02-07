/*
 * Created by ckalo on 2/5/26.
 */
#ifndef LOGGER_LOGGER_TO_HTML_CSS_H
#define LOGGER_LOGGER_TO_HTML_CSS_H


enum HTML_GROUP_ID{
    HTML_GROUP_1=0,
    HTML_GROUP_2,
    HTML_GROUP_CAPACITY,
};

enum HTML_CLASS_ID{
    HTML_CLASS_1=0,
    HTML_CLASS_2,
    HTML_CLASS_CAPACITY,
};


/***************************************************************************************************
 * HTML
 */
static const char* html_header = "\
<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>Toggle Dropdown Menu</title>\n\
    <style>\n";
static const char* html_header_close = "\
    </style>\n\
</head>\n";

static const char* html_body = "<body>\n";
static const char* html_body_close = "\
</body>\n\
</html>\n";


/***************************************************************************************************
 * CSS
 */
static const char* css_all = "\
* {\n\
    margin: 0;\n\
    padding: 0;\n\
    box-sizing: border-box;\n\
    font-family: monospace;\n\
}\n";

static const char* css_tag_body = "\
body {\n\
    min-height: 100vh;\n\
    min-width: 60vh;\n\
    /*min-width: 100vh;*/\n\
    display: flex;\n\
    justify-content: center;\n\
    /*align-items: center;*/\n\
    padding: 20px;\n\
    background-color: #333;\n\
}\n";
static const char* css_tag_div = "\
div {\n\
    /*background-color: #999;*/\n\
}\n";

static const char* css_tag_p = "\
p {\n\
    color: #fff;\n\
}\n";

/*****************************
 * CLASSES
 */

static const char* css_class_container = "\
.container {\n\
    max-width: 800px;\n\
    width: 100%;\n\
    background-color: #222;\n\
    border-radius: 16px;\n\
    padding: 40px;\n\
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.1);\n\
}\n";

/* colors */
static const char* css_text_color_red1  = ".red1 {\n  color: #ef2334;\n}\n";
static const char* css_text_color_red2  = ".red2 {\n  color: #ef238d;\n}\n";
static const char* css_text_color_red3  = ".red3 {\n  color: #ef3b23;\n}\n";
static const char* css_text_color_blue1 = ".blue1 {\n    color: #232eef;\n}\n";
static const char* css_text_color_blue2 = ".blue2 {\n    color: #2367ef;\n}\n";
static const char* css_text_color_blue3 = ".blue3 {\n    color: #23a1ef;\n}\n";

#endif //LOGGER_LOGGER_TO_HTML_CSS_H
