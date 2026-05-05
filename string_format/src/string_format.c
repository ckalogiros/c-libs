/*
 * Created by ckalo on 3/9/26.
 */

#include "string_format.h"

#include <assert.h>

#include "ascii_characters.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"
#include <stdlib.h>


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

#define alphabet_lower 97
#define alphabet_upper 65
#define alphabet_len 26
#define numeric 48
#define numeric_len 10


uint8
is_newline(const char c1, const char c2)
{
    if(c1 == '\\' && c2 == 'n')
        return c2;
    return 0;
}

uint8
is_numeric(char c)
{
    if(c >= numeric && c < numeric + numeric_len)
        return c;
    return 0;
}

uint8
is_alphabet(char c)
{
    if(c >= ASCII_A && c < ASCII_Z)
        return c;
    if(c >= ASCII_a && c < ASCII_z)
        return c;
    return 0;
}

uint32
string_format(char** const buf, const uint32 size, const char* format, ...)
{
    uint32 i, k;

    va_list args;
    va_start(args, format);

    i = 0, k = 0;
    while(format[i]) {

        char c = format[i];

        if(c == '%') {
            c = format[++i];
            switch(c) {
                case 'u': {
                    uint64 num = va_arg(args, uint64);
                    uint32 len = str_conv_unt32_to_str(&(*buf)[k], num);
                    k += len; assert(k < size);
                    break;
                }
                case 'd': {
                    int64 num = va_arg(args, int64);
                    uint32 len = str_conv_int32_to_str(&(*buf)[k], num);
                    k += len; assert(k < size);
                    break;
                }
                case 'x': { break; }
                case 'f': {
                    // long double num = (long double)va_arg(args, long double);
                    double num = (double) va_arg(args, double);
                    uint32 len = snprintf(buf[k], 7, "%.3f", num);
                    k += len;
                    break;
                }
                case '.': { /// for notation: "%.2f, %02f, etc". TODO: implement for integers too.
                    double num = (double) va_arg(args, double);

                    char a[16] = {'\0'}; uint32 m = i-1, n = 0;
                    a[n++] = format[m++];
                    while(format[m] != '%' && format[m] != ',' && format[m] != '\0') {
                        a[n++] = format[m];
                        m++;
                    }

                    /// we do not use "snprintf" cause it returns len=4 when it optimizes a "1.00" to a truncated "1.0"
                    uint32 len = sprintf(buf[k], a, num);
                    k += len;
                    /// IMPORTANT COMMENT!!!
                    /// n-1: because we want i to point at ',' in "%.2f,"
                    /// and a second n-1: because later in the loop we increment i again (i++)
                    i += n-2;
                    break;
                }
                case 'c': {
                    (*buf)[k++] = (char) va_arg(args, int);
                    assert(k < size);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if(str != NULL) {
                        uint32 j = 0;
                        while(str[j]) (*buf)[k++] = str[j++];
                        assert(k < size);
                    }
                    break;
                }
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

uint32 string_buffer_push(char** const buf, const uint32 size, const char* str)
{
    uint32 i = 0, k = 0;
    while(str[i]) {

        (*buf)[k++] = str[i];
        assert(k < size);
        i++;
    }

    return k;
}

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
