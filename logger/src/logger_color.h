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
#define br1 (FG_RGB(120,50, 255))
#define br2 (FG_RGB(200,50, 255))
#define br3 (FG_RGB(240, 50, 100))

#endif // LOGGER_COLORS_H

