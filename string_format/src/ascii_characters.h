/*
 * Created by ckalo on 3/9/26.
 */
#ifndef ASCII_CHARACTERS_H
#define ASCII_CHARACTERS_H

/* ASCII Control Characters (0x00 - 0x1F) */
typedef enum {
    ASCII_NUL = 0x00,    /* Null */
    ASCII_SOH = 0x01,    /* Start of Heading */
    ASCII_STX = 0x02,    /* Start of Text */
    ASCII_ETX = 0x03,    /* End of Text */
    ASCII_EOT = 0x04,    /* End of Transmission */
    ASCII_ENQ = 0x05,    /* Enquiry */
    ASCII_ACK = 0x06,    /* Acknowledge */
    ASCII_BEL = 0x07,    /* Bell */
    ASCII_BS  = 0x08,    /* Backspace */
    ASCII_HT  = 0x09,    /* Horizontal Tab */
    ASCII_LF  = 0x0A,    /* Line Feed */
    ASCII_VT  = 0x0B,    /* Vertical Tab */
    ASCII_FF  = 0x0C,    /* Form Feed */
    ASCII_CR  = 0x0D,    /* Carriage Return */
    ASCII_SO  = 0x0E,    /* Shift Out */
    ASCII_SI  = 0x0F,    /* Shift In */
    ASCII_DLE = 0x10,    /* Data Link Escape */
    ASCII_DC1 = 0x11,    /* Device Control 1 */
    ASCII_DC2 = 0x12,    /* Device Control 2 */
    ASCII_DC3 = 0x13,    /* Device Control 3 */
    ASCII_DC4 = 0x14,    /* Device Control 4 */
    ASCII_NAK = 0x15,    /* Negative Acknowledge */
    ASCII_SYN = 0x16,    /* Synchronous Idle */
    ASCII_ETB = 0x17,    /* End of Transmission Block */
    ASCII_CAN = 0x18,    /* Cancel */
    ASCII_EM  = 0x19,    /* End of Medium */
    ASCII_SUB = 0x1A,    /* Substitute */
    ASCII_ESC = 0x1B,    /* Escape */
    ASCII_FS  = 0x1C,    /* File Separator */
    ASCII_GS  = 0x1D,    /* Group Separator */
    ASCII_RS  = 0x1E,    /* Record Separator */
    ASCII_US  = 0x1F,    /* Unit Separator */

    /* Printable Characters (0x20 - 0x7E) */
    ASCII_SPACE   = 0x20,    /* Space */
    ASCII_EXCLAM   = 0x21,    /* ! */
    ASCII_QUOTE    = 0x22,    /* " */
    ASCII_HASH     = 0x23,    /* # */
    ASCII_DOLLAR   = 0x24,    /* $ */
    ASCII_PERCENT  = 0x25,    /* % */
    ASCII_AMP      = 0x26,    /* & */
    ASCII_APOST    = 0x27,    /* ' */
    ASCII_LPAREN   = 0x28,    /* ( */
    ASCII_RPAREN   = 0x29,    /* ) */
    ASCII_ASTERISK = 0x2A,    /* * */
    ASCII_PLUS     = 0x2B,    /* + */
    ASCII_COMMA    = 0x2C,    /* , */
    ASCII_MINUS    = 0x2D,    /* - */
    ASCII_DOT      = 0x2E,    /* . */
    ASCII_SLASH    = 0x2F,    /* / */

    /* Digits */
    ASCII_0 = 0x30,
    ASCII_1 = 0x31,
    ASCII_2 = 0x32,
    ASCII_3 = 0x33,
    ASCII_4 = 0x34,
    ASCII_5 = 0x35,
    ASCII_6 = 0x36,
    ASCII_7 = 0x37,
    ASCII_8 = 0x38,
    ASCII_9 = 0x39,

    /* Punctuation after digits */
    ASCII_COLON     = 0x3A,    /* : */
    ASCII_SEMICOLON = 0x3B,    /* ; */
    ASCII_LT        = 0x3C,    /* < */
    ASCII_EQ        = 0x3D,    /* = */
    ASCII_GT        = 0x3E,    /* > */
    ASCII_QUESTION  = 0x3F,    /* ? */
    ASCII_AT        = 0x40,    /* @ */

    /* Uppercase Letters */
    ASCII_A = 0x41,
    ASCII_B = 0x42,
    ASCII_C = 0x43,
    ASCII_D = 0x44,
    ASCII_E = 0x45,
    ASCII_F = 0x46,
    ASCII_G = 0x47,
    ASCII_H = 0x48,
    ASCII_I = 0x49,
    ASCII_J = 0x4A,
    ASCII_K = 0x4B,
    ASCII_L = 0x4C,
    ASCII_M = 0x4D,
    ASCII_N = 0x4E,
    ASCII_O = 0x4F,
    ASCII_P = 0x50,
    ASCII_Q = 0x51,
    ASCII_R = 0x52,
    ASCII_S = 0x53,
    ASCII_T = 0x54,
    ASCII_U = 0x55,
    ASCII_V = 0x56,
    ASCII_W = 0x57,
    ASCII_X = 0x58,
    ASCII_Y = 0x59,
    ASCII_Z = 0x5A,

    /* More punctuation */
    ASCII_LBRACKET  = 0x5B,    /* [ */
    ASCII_BACKSLASH = 0x5C,    /* \ */
    ASCII_RBRACKET  = 0x5D,    /* ] */
    ASCII_CARET     = 0x5E,    /* ^ */
    ASCII_UNDERSCORE = 0x5F,   /* _ */
    ASCII_BACKTICK  = 0x60,    /* ` */

    /* Lowercase Letters */
    ASCII_a = 0x61,
    ASCII_b = 0x62,
    ASCII_c = 0x63,
    ASCII_d = 0x64,
    ASCII_e = 0x65,
    ASCII_f = 0x66,
    ASCII_g = 0x67,
    ASCII_h = 0x68,
    ASCII_i = 0x69,
    ASCII_j = 0x6A,
    ASCII_k = 0x6B,
    ASCII_l = 0x6C,
    ASCII_m = 0x6D,
    ASCII_n = 0x6E,
    ASCII_o = 0x6F,
    ASCII_p = 0x70,
    ASCII_q = 0x71,
    ASCII_r = 0x72,
    ASCII_s = 0x73,
    ASCII_t = 0x74,
    ASCII_u = 0x75,
    ASCII_v = 0x76,
    ASCII_w = 0x77,
    ASCII_x = 0x78,
    ASCII_y = 0x79,
    ASCII_z = 0x7A,

    /* Final punctuation */
    ASCII_LBRACE    = 0x7B,    /* { */
    ASCII_PIPE      = 0x7C,    /* | */
    ASCII_RBRACE    = 0x7D,    /* } */
    ASCII_TILDE     = 0x7E,    /* ~ */

    /* Delete/Control */
    ASCII_DEL = 0x7F     /* Delete */
} ASCII;



#endif //ASCII_CHARACTERS_H