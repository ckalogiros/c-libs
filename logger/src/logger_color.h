#ifndef LOGGER_COLORS_H
#define LOGGER_COLORS_H

// True color format: \033[38;2;R;G;Bm (foreground)
//                    \033[48;2;R;G;Bm (background)

#define FG_RGB(r, g, b) ("\033[38;2;" #r ";" #g ";" #b "m")
#define BG_RGB(r, g, b) ("\033[48;2;" #r ";" #g ";" #b "m")
#define CLEAR ("\033[0m")

#define RED   (FG_RGB(255,50,50))
#define ORANGE2  (FG_RGB(255,100,10))
#define ry  (FG_RGB(255,150,10))
#define YELLOW   (FG_RGB(240,240,10))
#define yg  (FG_RGB(150,240,50))
#define GREEN   (FG_RGB(20, 240, 20))
#define gc  (FG_RGB(50,240, 140))
#define CYAN   (FG_RGB(10,240, 255))
#define BLUE   (FG_RGB(0,100, 255))
#define br1 (FG_RGB(170, 70, 250))
#define br2 (FG_RGB(200,50, 255))
#define br3 (FG_RGB(240, 50, 100))

#define COLOR_ARRAY_COUNT 9
static char* color_array[COLOR_ARRAY_COUNT] =
    { RED, ry, YELLOW, GREEN, CYAN, BLUE, br1, br2, br3 };
#define COLOR_ARRAY_GET(col_idx) (color_array[(col_idx)])


/***************************************************************************************************
 * hex predifined colors
 */
#define hex_red1 (#ef2334)
#define hex_red2 (#ef238d)
#define hex_red3 (#ef3b23)
#define hex_purple1 (#ef23e2)
#define hex_purple2 (#e223ef)
#define hex_purple3 (#b223ef)
#define hex_purple4 (#8623ef)
#define hex_blue1 (#232eef)
#define hex_blue2 (#2367ef)
#define hex_blue3 (#23a1ef)
#define hex_cyan1 (#23dbef)
#define hex_cyan2 (#23efd7)



/***************************************************************************************************
 * HTML builder: css style colors
 */
// #define HTML_RED(s)     "<span style='color:red;font-weight:bold'>" s "</span>"
// #define HTML_GREEN(s)   "<span style='color:green'>" s "</span>"
// #define HTML_YELLOW(s)  "<span style='color:orange'>" s "</span>"
// #define HTML_BLUE(s)    "<span style='color:blue'>" s "</span>"
// #define HTML_GRAY(s)    "<span style='color:gray'>" s "</span>"

#endif // LOGGER_COLORS_H
