#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <stdint.h>

#define TRUE 1
#define FALSE 0
#define INT_NULL (-1)
#ifndef NULL
#define NULL ((void *) 0)
#endif

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8 bool;

typedef uint16 typesize_t;


#define F4 0xffff
#define F5 0xfffff
#define F6 0xffffff
/// reduce the hex digits of an address
#define REDUCE_ADDR(addr, hex_mask) ((uint64) (addr) & (hex_mask))


#define BITS8  8
#define BITS16 16
#define BITS32 32
#define BITS64 64

#define BYTE_SIZE 8

#define MAX_INT8   INT8_MAX
#define MAX_INT16  INT16_MAX
#define MAX_INT32  INT32_MAX
#define MAX_INT64  INT64_MAX
#define MAX_UINT8  UINT8_MAX
#define MAX_UINT16 UINT16_MAX
#define MAX_UINT32 UINT32_MAX
#define MAX_UINT64 UINT64_MAX

#define BIT_MASK_FROM_UINT(n) (1ULL << (n))

#define GET_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define GET_MIN(a, b) (((a) < (b)) ? (a) : (b))

// typedef uint16 bufidx_t; // todo remove from here



/***************************************************************************************************
 * GENERICS
 **************************************************************************************************/
/* get the full mask (hex value) from value's type */
#define types_get_fullmask_from_type(T) \
_Generic((T),                           \
uint8   : 0xff,                         \
uint16  : 0xffff,                       \
uint32  : 0xffffffff,                   \
uint64  : 0xffffffffffffffff,           \
default : 0x0)

/* get the half of the full mask (hex value) from value's type */
#define types_get_half_fullmask_from_type(T) \
_Generic((T),                                \
uint8   : 0xf,                               \
uint16  : 0xff,                              \
uint32  : 0xffff,                            \
uint64  : 0xffffffff,                        \
default : 0x0)

/* get the number of bits from value's type */
#define types_get_bitsize_from_type(T) \
_Generic((T),                          \
uint8   : 8,                           \
uint16  : 16,                          \
uint32  : 32,                          \
uint64  : 64,                          \
default : 0)

/* get half the number of bits from value's type */
#define types_get_half_bitsize_from_type(T) \
_Generic((T),                               \
uint8   : 4,                                \
uint16  : 8,                                \
uint32  : 16,                               \
uint64  : 32,                               \
default : 0)


/* force the type size to value 1 */
#define types_force_typesize(T)  \
_Generic((T),                    \
uint8   : UINT8_C(1),            \
uint16  : UINT16_C(1),           \
uint32  : UINT32_C(1),           \
uint64  : UINT64_C(1),           \
default : UINT64_C(1))

#define types_get_bytesize_from_type(T) \
_Generic((T),                           \
uint8:  1,                              \
uint16: 2,                              \
uint32: 4,                              \
uint64: 8,                              \
default: 0)

#define types_get_type_from_bytesize(size) \
_Generic(((char(*)[size])NULL),            \
char(*)[1]:   uint8,                       \
char(*)[2]:   uint16,                      \
char(*)[4]:   uint32,                      \
char(*)[8]:   uint64,                      \
default:      void* )

/* get the number of bits from type */
#define types_get_bitsize_from_val(v) (sizeof(v) * (8))

// Create a macro that expands to the type name
#define type_from_size_in_char(size)   \
_Generic(((char(*)[size])NULL), \
char(*)[1]:   uint8_t,          \
char(*)[2]:   uint16_t,         \
char(*)[4]:   uint32_t,         \
char(*)[8]:   uint64_t,         \
default:      void)

/* force the type size to value 1 */
#define max_value_from_type(T) \
_Generic((T),                  \
int8    : MAX_INT8,            \
int16   : MAX_INT16,           \
int32   : MAX_INT32,           \
int64   : MAX_INT64,           \
uint8   : MAX_UINT8,           \
uint16  : MAX_UINT16,          \
uint32  : MAX_UINT32,          \
uint64  : MAX_UINT64,          \
default : 0)


/***************************************************************************************************
 * TYPES
 **************************************************************************************************/
/* vectors */
#define SIZE_F32 4
#define SIZE_F64 8
#define SIZE_I64 8
#define SIZE_I32 4
#define SIZE_I16 2
#define SIZE_I8  1

/* vectors */
typedef struct { double  x; double  y; } vec2f64;
typedef struct { float  x; float  y; } vec2f;
typedef struct { int64  x; int64  y; } vec2i64;
typedef struct { uint64 x; uint64 y; } vec2u64;
typedef struct { int32  x; int32  y; } vec2i32;
typedef struct { uint32 x; uint32 y; } vec2u32;
typedef struct { int16  x; int16  y; } vec2i16;
typedef struct { uint16 x; uint16 y; } vec2u16;
typedef struct { int8   x; int8   y; } vec2i8;
typedef struct { uint8  x; uint8  y; } vec2u8;

typedef union { struct { float x; float y; float z; };
                struct { float r; float g; float b; }; } vec3f;
typedef struct { int64  x; int64  y; int64  z; } vec3i64;
typedef struct { uint64 x; uint64 y; uint64 z; } vec3u64;
typedef struct { int32  x; int32  y; int32  z; } vec3i32;
typedef struct { uint32 x; uint32 y; uint32 z; } vec3u32;
typedef struct { int16  x; int16  y; int16  z; } vec3i16;
typedef struct { uint16 x; uint16 y; uint16 z; } vec3u16;
typedef struct { int8   x; int8   y; int8   z ; } vec3i8;
typedef struct { uint8  x; uint8  y; uint8  z; } vec3u8;

typedef union { struct { float x; float y; float z; float w; };
                struct { float r; float g; float b; float a; }; } vec4f;
typedef struct { int64  x; int64  y; int64  z; int64  w; } vec4i64;
typedef struct { uint64 x; uint64 y; uint64 z; uint64 w; } vec4u64;
typedef struct { int32  x; int32  y; int32  z; int32  w; } vec4i32;
typedef struct { uint32 x; uint32 y; uint32 z; uint32 w; } vec4u32;
typedef struct { int16  x; int16  y; int16  z; int16  w; } vec4i16;
typedef struct { uint16 x; uint16 y; uint16 z; uint16 w; } vec4u16;
typedef struct { int8   x; int8   y; int8   z; int8   w; } vec4i8;
typedef struct { uint8  x; uint8  y; uint8  z; uint8  w; } vec4u8;

/* matrices */
typedef float  mat2x2f  [2][2]; /// example: mat2x2f mat = { {.0f, .0f}, {.0f, .0f} };
typedef int64  mat2x2i64[2][2];
typedef uint64 mat2x2u64[2][2];
typedef int32  mat2x2i32[2][2];
typedef uint32 mat2x2u32[2][2];
typedef int16  mat2x2i16[2][2];
typedef uint16 mat2x2u16[2][2];
typedef int8   mat2x2i8 [2][2];
typedef uint8  mat2x2u8 [2][2];

typedef float  mat2x3f  [2][3];
typedef int64  mat2x3i64[2][3];
typedef uint64 mat2x3u64[2][3];
typedef int32  mat2x3i32[2][3];
typedef uint32 mat2x3u32[2][3];
typedef int16  mat2x3i16[2][3];
typedef uint16 mat2x3u16[2][3];
typedef int8   mat2x3i8 [2][3];
typedef uint8  mat2x3u8 [2][3];

typedef float  mat3x3f  [3][3];
typedef int64  mat3x3i64[3][3];
typedef uint64 mat3x3u64[3][3];
typedef int32  mat3x3i32[3][3];
typedef uint32 mat3x3u32[3][3];
typedef int16  mat3x3i16[3][3];
typedef uint16 mat3x3u16[3][3];
typedef int8   mat3x3i8 [3][3];
typedef uint8  mat3x3u8 [3][3];

typedef float  mat3x4f  [3][4];
typedef int64  mat3x4i64[3][4];
typedef uint64 mat3x4u64[3][4];
typedef int32  mat3x4i32[3][4];
typedef uint32 mat3x4u32[3][4];
typedef int16  mat3x4i16[3][4];
typedef uint16 mat3x4u16[3][4];
typedef int8   mat3x4i8 [3][4];
typedef uint8  mat3x4u8 [3][4];

typedef float  mat4x4f  [4][4];
typedef int64  mat4x4i64[4][4];
typedef uint64 mat4x4u64[4][4];
typedef int32  mat4x4i32[4][4];
typedef uint32 mat4x4u32[4][4];
typedef int16  mat4x4i16[4][4];
typedef uint16 mat4x4u16[4][4];
typedef int8   mat4x4i8 [4][4];
typedef uint8  mat4x4u8 [4][4];


/// types
enum TYPES_INDEXING {
    DOUBLE = 0,
    FLOAT  , INT64  ,   UINT64 ,   INT32  ,   UINT32 ,   INT16  ,   UINT16 ,   INT8   ,   UINT8  ,
    VEC2F  , VEC2I64,   VEC2U64,   VEC2I32,   VEC2U32,   VEC2I16,   VEC2U16,   VEC2I8 ,   VEC2U8 ,
    VEC3F  , VEC3I64,   VEC3U64,   VEC3I32,   VEC3U32,   VEC3I16,   VEC3U16,   VEC3I8 ,   VEC3U8 ,
    VEC4F  , VEC4I64,   VEC4U64,   VEC4I32,   VEC4U32,   VEC4I16,   VEC4U16,   VEC4I8 ,   VEC4U8 ,
    MAT2X2F, MAT2X2I64, MAT2X2U64, MAT2X2I32, MAT2X2U32, MAT2X2I16, MAT2X2U16, MAT2X2I8 , MAT2X2U8 ,
    MAT2X3F, MAT2X3I64, MAT2X3U64, MAT2X3I32, MAT2X3U32, MAT2X3I16, MAT2X3U16, MAT2X3I8 , MAT2X3U8 ,
    MAT3X3F, MAT3X3I64, MAT3X3U64, MAT3X3I32, MAT3X3U32, MAT3X3I16, MAT3X3U16, MAT3X3I8 , MAT3X3U8 ,
    MAT3X4F, MAT3X4I64, MAT3X4U64, MAT3X4I32, MAT3X4U32, MAT3X4I16, MAT3X4U16, MAT3X4I8 , MAT3X4U8 ,
    MAT4X4F, MAT4X4I64, MAT4X4U64, MAT4X4I32, MAT4X4U32, MAT4X4I16, MAT4X4U16, MAT4X4I8 , MAT4X4U8 ,

    TYPES_COUNT,
};



/***************************************************************************************************
 * STRUCTS
 **************************************************************************************************/

struct flist {
    struct flist* next;
};


#endif // MY_TYPES_H




// // For IDs/counters (usually positive)
// #define INVALID_ID 0
// #define NO_INDEX -1
// #define UNUSED_SLOT 0xFFFFFFFF
//
// // For array indices
// #define INDEX_NONE -1
// ssize_t index = INDEX_NONE;
//
// // For handles
// #define INVALID_HANDLE 0
// #define NULL_HANDLE ((void*)0)
// 2. Use functions for validation:
// c
//
// static inline bool is_valid_id(uint32_t id) {
//     return id != USER_ID_NONE;
// }
//
// static inline bool is_index_valid(ssize_t idx) {
//     return idx >= 0;
// }
//
// static inline bool is_handle_valid(void* handle) {
//     return handle != NULL;
// }
