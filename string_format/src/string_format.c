/*
 * Created by ckalo on 3/9/26.
 */

#include "string_format.h"
#include <assert.h>
#include "ascii_characters.h"
#include "logger.h"
#include "str_utils.h"
#include <string.h>


/* stigma
32 	     59  ;
33 	!    60  <
34 	"    61  =
35 	#    62  >
36 	$    63  ?
37 	%    64  @
38 	&    91  [
39 	'    92  \
40 	(    93  ]
41 	)    94  ^
42 	*    95  _
43 	+    96  `
44 	,    123 {
45 	-    124 |
46 	.    125 }
47 	/    126 ~
127 DEL
*/
/* numeric
_______
48 	0
49 	1
50 	2
51 	3
52 	4
53 	5
54 	6
55 	7
56 	8
57 	9
*/
/* alphabet
upper    lower
_____    _____
65  A    97  a
66 	B    98  b
67 	C    99  c
68 	D    100 d
69 	E    101 e
70 	F    102 f
71 	G    103 g
72 	H    104 h
73 	I    105 i
74 	J    106 j
75 	K    107 k
76 	L    108 l
77 	M    109 m
78 	N    110 n
79 	O    111 o
80 	P    112 p
81 	Q    113 q
82 	R    114 r
83 	S    115 s
84 	T    116 t
85 	U    117 u
86 	V    118 v
87 	W    119 w
88 	X    120 x
89 	Y    121 y
90 	Z    122 z
*/
/*
0 	NUL 	26 	SUB 	52 	4 		78 	N 		104 	h
1 	SOH 	27 	ESC 	53 	5 		79 	O 		105 	i
2 	STX 	28 	FS 		54 	6 		80 	P 		106 	j
3 	ETX 	29 	GS 		55 	7 		81 	Q 		107 	k
4 	EOT 	30 	RS 		56 	8 		82 	R 		108 	l
5 	ENQ 	31 	US 		57 	9 		83 	S 		109 	m
6 	ACK 	32 			58 	: 		84 	T 		110 	n
7 	BEL 	33 	! 		59 	; 		85 	U 		111 	o
8 	BS 		34 	" 		60 	< 		86 	V 		112 	p
9 	HT 		35 	# 		61 	= 		87 	W 		113 	q
10 	LF 		36 	$ 		62 	> 		88 	X 		114 	r
11 	VT 		37 	% 		63 	? 		89 	Y 		115 	s
12 	FF 		38 	& 		64 	@ 		90 	Z 		116 	t
13 	CR 		39 	' 		65 	A 		91 	[ 		117 	u
14 	SO 		40 	( 		66 	B 		92 	\ 		118 	v
15 	SI 		41 	) 		67 	C 		93 	] 		119 	w
16 	DLE 	42 	* 		68 	D 		94 	^ 		120 	x
17 	DC1 	43 	+ 		69 	E 		95 	_ 		121 	y
18 	DC2 	44 	, 		70 	F 		96 	` 		122 	z
19 	DC3 	45 	- 		71 	G 		97 	a 		123 	{
20 	DC4 	46 	. 		72 	H 		98 	b 		124 	|
21 	NAK 	47 	/ 		73 	I 		99 	c 		125 	}
22 	SYN 	48 	0 		74 	J 		100 d 		126 	~
23 	ETB 	49 	1 		75 	K 		101 e 		127 	DEL
24 	CAN 	50 	2 		76 	L 		102 f
25 	EM 		51 	3 		77 	M 		103 g
*/

/* helper: add to a string buffer, the int and fract parts of a floating point value */
uint32
floating_point_format(char** const buf, double num, char* formated, bool is_formated, uint32 k)
{
    /*
        * Format	Output for -1.2345	Description
        %f	        -1.234500	        Default 6 decimal places
        %.2f	    -1.23	            2 decimal places
        %4.2f	    -1.23	            Min width 4, 2 decimals
        %8.2f	    -1.23	            Right-aligned, width 8
        %-8.2f	    -1.23 	            Left-aligned, width 8
        %+f	        -1.234500	        Always show sign
        %e	        -1.234500e+00	    Scientific notation
        %g	        -1.2345	Shortest    representation

        so, in the format we have:
        1. no format
        2. left format: number
        3. right format: a , with following number
     */

    struct float_number f = str_conv_double_to_int_parts(num);
    char ipstr[8] = {0};
    char fpstr[8] = {0};
    uint32 ilen = str_conv_int32_to_str(ipstr, f.int_part);
    uint32 flen = str_conv_uint32_to_str(fpstr, f.fract_part);
    uint16 ii = 0, fi = 0, j = 0;
    uint8 n = 0;

    /// integer part format
    if( is_formated && is_numeric(formated[j]) ) { /// check integer part format
        n = (int)formated[j++] - ASCII_0; /// get the number from char
        if(ilen > n) n = 0;
        else n-=ilen;
        while(n > 0) {
            (*buf)[k++] = ' ';
            n--;
        }
        /// TODO: implement case: format value > 9 (e.x. %44.2f, we need to consume both numbers before the '.')
    }

    /// integer part value
    while(ilen > 0) {
        (*buf)[k++] = ipstr[ii++];
        ilen--;
    }


    (*buf)[k++] = '.'; /// add floating point '.'

    // fract part format
    n = flen;
    if( is_formated && formated[j] == '.' ) {
        n = formated[++j] - ASCII_0;
    }
    /// fractional part value
    while(n > 0) {
        (*buf)[k++] = fpstr[fi++];
        n--;
    }
    k-=1;

    return k;

}

uint32 /* helper: add space characters to a string buffer */
add_leading_spaces(char** buf, uint32 k, uint32 count, char snum)
{
    uint32 n = (int)snum - ASCII_0; /// char to integer

    if(count > n) return k; /// no leading spaces
    else n -= count; /// how many leading spaces

    while(n > 0) {
        (*buf)[k++] = ' ';
        n--;
    }
    /// TODO: implement case: format value > 9 (e.x. %44.2f, we need to consume both numbers before the '.')
    return k;
}

/* add to a string buffer, the formated input from variable arguments */
uint32
string_format(char** const buf, const uint32 size, const char* format, ...)
{
    uint32 i, k;

    va_list args;
    va_start(args, format);


    i = 0, k = 0;
    while(format[i]) {

        char c = format[i];

        char formated[8] = {0};
        bool is_formated = FALSE;

        if(c == '%') {
            c = format[++i];

            if(c < ASCII_A) {      /// accumulate format of type: %num OR %num.num
                uint8 j = 0;
                while(format[i] < ASCII_A)
                    formated[j++] = format[i++];
                is_formated = TRUE;
                c = format[i];
            }
            switch(c) {
                case 'u': {
                    uint64 num = va_arg(args, uint64);
                    uint8 digits = calculate_num_digits((uint64)num);
                    if(is_formated) k = add_leading_spaces(buf, k, digits, formated[0]);
                    uint32 len = str_conv_uint32_to_str(&(*buf)[k], num);
                    k += len; assert(k < size);
                    break;
                }
                case 'd': {
                    int64 num = va_arg(args, int64);
                    uint8 digits = calculate_num_digits((uint64)num);
                    if(is_formated) k = add_leading_spaces(buf, k, digits, formated[0]);
                    uint32 len = str_conv_int32_to_str(&(*buf)[k], num);
                    k += len; assert(k < size);
                    break;
                }
                case 'x': {
                    assert(0 > 0);
                }
                case 'f': {
                    double num = (double) va_arg(args, double);
                    k = floating_point_format(buf, num, formated, is_formated, k);
                    break;
                }
                case 'c': {
                    if(is_formated) k = add_leading_spaces(buf, k, 1, formated[0]);
                    (*buf)[k++] = (char) va_arg(args, int);
                    assert(k < size);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if(is_formated) k = add_leading_spaces(buf, k, strlen(str), formated[0]);
                    if(str != NULL) {
                        uint32 j = 0;
                        while(str[j]) (*buf)[k++] = str[j++];
                        assert(k < size);
                    }
                    break;
                }
                default: { assert(0 > 0); }
            }
        } else {
            (*buf)[k++] = c;
            assert(k < size);
        }

        i++;
    }

    va_end(args);
    assert(k < size);
    return k;
}

/* add to a string buffer, the formated input from va_list arguments */
uint32
string_format_va_list_call(char** const buf, const uint32 size, const char* format, va_list args)
{
    uint32 i, k;

    i = 0, k = 0;
    while(format[i]) {

        char c = format[i];

        char formated[8] = {0};
        bool is_formated = FALSE;

        if(c == '%') {
            c = format[++i];

            if(c < ASCII_A) {      /// accumulate format of type: %num OR %num.num
                uint8 j = 0;
                while(format[i] < ASCII_A)
                    formated[j++] = format[i++];
                is_formated = TRUE;
                c = format[i];
            }
            switch(c) {
                case 'u': {
                    uint64 num = va_arg(args, uint64);
                    uint8 digits = calculate_num_digits((uint64)num);
                    if(is_formated) k = add_leading_spaces(buf, k, digits, formated[0]);
                    uint32 len = str_conv_uint32_to_str(&(*buf)[k], num);
                    k += len; assert(k < size);
                    break;
                }
                case 'd': {
                    int64 num = va_arg(args, int64);
                    uint8 digits = calculate_num_digits((uint64)num);
                    if(is_formated) k = add_leading_spaces(buf, k, digits, formated[0]);
                    uint32 len = str_conv_int32_to_str(&(*buf)[k], num);
                    k += len; assert(k < size);
                    break;
                }
                case 'x': {
                    assert(0 > 0);
                }
                case 'f': {
                    double num = (double) va_arg(args, double);
                    k = floating_point_format(buf, num, formated, is_formated, k);
                    break;
                }
                case 'c': {
                    if(is_formated) k = add_leading_spaces(buf, k, 1, formated[0]);
                    (*buf)[k++] = (char) va_arg(args, int);
                    assert(k < size);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if(is_formated) k = add_leading_spaces(buf, k, strlen(str), formated[0]);
                    if(str != NULL) {
                        uint32 j = 0;
                        while(str[j]) (*buf)[k++] = str[j++];
                        assert(k < size);
                    }
                    break;
                }
                default: { assert(0 > 0); }
            }
        } else {
            (*buf)[k++] = c;
            assert(k < size);
        }

        i++;
    }

    assert(k < size);

    return k;
}

/* push characters to a string buffer. @return: the index of the last pushed character */
uint32
string_buffer_push(char** const buf, const uint32 size, const char* str)
{
    uint32 i = 0, k = 0;
    while(str[i]) {

        (*buf)[k++] = str[i];
        assert(k < size);
        i++;
    }

    return k;
}



/* TEST-DEBUG */
void string_format_test()
{
#   define TEMP_BUF_LEN 16

    print("- Test formated: float -\n", ry, CLEAR);
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%f";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%f", 10.981f);  print("%s:%s\n", str, buf);
    }
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%4f";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%4f", 10.981f); print("%s:%s\n", str, buf);
    }
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%.4f";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%.4f", 10.981f); print("%s:%s\n", str, buf);
    }
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%4.4f";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%4.4f", 10.981f); print("%s:%s\n", str, buf);
    }

    print("- Test formated: integers -\n", ry, CLEAR);
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%u";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%u", 981);  print("%s:%s\n", str, buf);
    }
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%4u";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%4u", 981);  print("%s:|%s|\n", str, buf); print("%s:|    |\n", str);
    }

    print("- Test formated: character -\n", ry, CLEAR);
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%c";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%c", 'A');  print("%s:%s\n", str, buf);
    }
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%4c";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%4c", 'A');  print("%s:|%s|\n", str, buf); print("%s:|    |\n", str);
    }

    print("- Test formated: string -\n", ry, CLEAR);
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%s";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%s", "abcd");  print("%s:%s\n", str, buf);
    }
    {
        char buf[TEMP_BUF_LEN] = {0}; char* ptr = &buf[0];
        char* str = "%6s";
        uint32 size = string_format(&ptr, TEMP_BUF_LEN, "%6s", "abcd");  print("%s:|%s|\n", str, buf); print("%s:|      |\n", str);
    }
#   undef TEMP_BUF_LEN
}
