/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_SHADER_ID_H
#define GRAPHICS_TEST_SHADER_ID_H
#include <assert.h>
#include "my_types.h"


#define SHADER_MAX_LOCATION_SIZE 4
#define SHADER_DEVISOR_PER_VERTEX 0
#define SHADER_DEVISOR_INSTANCED 1


/***************************************************************************************************
 * ## SHADER STANDARD/STATIC VARIABLES {ATTRIBUTES, UNIFORMS}
 * # These flags are used to compile the appropriate shaders for mesh creation, by the user.
 * # all static and user created shader variables are stored in an array,
 *   along with other useful information.
 * ! IMPORTANT !
 *  The number of flags, the number of indexes and the number of array elements
 *  MUST MATCH and MUST BE 1 TO 1 in terms of their position in their decleration.
 *  -- So we must create the appropriate checks --
 *   TODO: continue explanation
 **************************************************************************************************/
#define SID_NULL (0x0)
#define SID_OPACIT BIT_MASK_FROM_UINT(1)
#define SID_VPOSI2 BIT_MASK_FROM_UINT(2)
#define SID_WPOSI2 BIT_MASK_FROM_UINT(3)
#define SID_SCALE2 BIT_MASK_FROM_UINT(4)
#define SID_ROTAT2 BIT_MASK_FROM_UINT(5)
#define SID_PIVOT2 BIT_MASK_FROM_UINT(6)
#define SID_UVCOO2 BIT_MASK_FROM_UINT(7)
#define SID_VPOSI3 BIT_MASK_FROM_UINT(8)
#define SID_WPOSI3 BIT_MASK_FROM_UINT(9)
#define SID_SCALE3 BIT_MASK_FROM_UINT(10)
#define SID_ROTAT3 BIT_MASK_FROM_UINT(11)
#define SID_PIVOT3 BIT_MASK_FROM_UINT(12)
#define SID_COLOR3 BIT_MASK_FROM_UINT(13)
#define SID_NORMA3 BIT_MASK_FROM_UINT(14)
#define SID_COLOR4 BIT_MASK_FROM_UINT(15)
#define SID_PROJMAT4X4 BIT_MASK_FROM_UINT(16)
#define SID_DIF BIT_MASK_FROM_UINT(17)

enum SID_ID {
    // SID_NULL_=0,
    SID_OPACIT_ID = 0,
    SID_VPOSI2_ID,
    SID_WPOSI2_ID,
    SID_SCALE2_ID,
    SID_ROTAT2_ID,
    SID_PIVOT2_ID,
    SID_UVCOO2_ID,
    SID_VPOSI3_ID,
    SID_WPOSI3_ID,
    SID_SCALE3_ID,
    SID_ROTAT3_ID,
    SID_PIVOT3_ID,
    SID_COLOR3_ID,
    SID_NORMA3_ID,
    SID_COLOR4_ID,
    SID_PROJMAT4X4_ID,

    SID_ID_COUNT,
};


typedef uint64 sid_t;
typedef uint16 sididx_t;

/// TODO: unite some flags into one
struct gfx_sid {
    sid_t attr;
    sid_t unif;
    sid_t out;
    sid_t pass;
    sid_t oper;
};

static const char*
shaderinfo_flag_to_flagname(const sid_t sid_flag)
{
    if(sid_flag & SID_OPACIT) return "OPACIT";
    if(sid_flag & SID_VPOSI2) return "VPOSI2";
    if(sid_flag & SID_WPOSI2) return "WPOSI2";
    if(sid_flag & SID_SCALE2) return "SCALE2";
    if(sid_flag & SID_ROTAT2) return "ROTAT2";
    if(sid_flag & SID_PIVOT2) return "PIVOT2";
    if(sid_flag & SID_UVCOO2) return "UVCOO2";
    if(sid_flag & SID_VPOSI3) return "VPOSI3";
    if(sid_flag & SID_WPOSI3) return "WPOSI3";
    if(sid_flag & SID_SCALE3) return "SCALE3";
    if(sid_flag & SID_ROTAT3) return "ROTAT3";
    if(sid_flag & SID_PIVOT3) return "PIVOT3";
    if(sid_flag & SID_COLOR3) return "COLOR3";
    if(sid_flag & SID_NORMA3) return "NORMA3";
    if(sid_flag & SID_COLOR4) return "COLOR4";
    if(sid_flag & SID_PROJMAT4X4) return "PROJMAT4X4";
    return "NULL";
}

static const char*
shaderinfo_id_to_flagname(const enum SID_ID type)
{
    switch(type) {
        // case SID_NULL_:   return "";
        case SID_OPACIT_ID: return "OPACIT";
        case SID_VPOSI2_ID: return "VPOSI2";
        case SID_WPOSI2_ID: return "WPOSI2";
        case SID_SCALE2_ID: return "SCALE2";
        case SID_ROTAT2_ID: return "ROTAT2";
        case SID_PIVOT2_ID: return "PIVOT2";
        case SID_UVCOO2_ID: return "UVCOO2";
        case SID_VPOSI3_ID: return "VPOSI3";
        case SID_WPOSI3_ID: return "WPOSI3";
        case SID_SCALE3_ID: return "SCALE3";
        case SID_ROTAT3_ID: return "ROTAT3";
        case SID_PIVOT3_ID: return "PIVOT3";
        case SID_COLOR3_ID: return "COLOR3";
        case SID_COLOR4_ID: return "NORMA3";
        case SID_PROJMAT4X4_ID: return "PROJMAT4X4";
        default: return "NONE";
    }
}

static sid_t
shaderinfo_id_to_flag(const enum SID_ID type)
{
    switch(type) {
        // case SID_NULL_:   return "";
        case SID_OPACIT_ID: return SID_OPACIT;
        case SID_VPOSI2_ID: return SID_VPOSI2;
        case SID_WPOSI2_ID: return SID_WPOSI2;
        case SID_SCALE2_ID: return SID_SCALE2;
        case SID_ROTAT2_ID: return SID_ROTAT2;
        case SID_PIVOT2_ID: return SID_PIVOT2;
        case SID_UVCOO2_ID: return SID_UVCOO2;
        case SID_VPOSI3_ID: return SID_VPOSI3;
        case SID_WPOSI3_ID: return SID_WPOSI3;
        case SID_SCALE3_ID: return SID_SCALE3;
        case SID_ROTAT3_ID: return SID_ROTAT3;
        case SID_PIVOT3_ID: return SID_PIVOT3;
        case SID_COLOR3_ID: return SID_COLOR3;
        case SID_NORMA3_ID: return SID_NORMA3;
        case SID_COLOR4_ID: return SID_COLOR4;
        case SID_PROJMAT4X4_ID: return SID_PROJMAT4X4;
        default: return SID_NULL;
    }
}

static enum SID_ID
shaderinfo_flag_to_id(const sid_t sid_flag)
{
    if(sid_flag & SID_OPACIT) return SID_OPACIT_ID;
    if(sid_flag & SID_VPOSI2) return SID_VPOSI2_ID;
    if(sid_flag & SID_WPOSI2) return SID_WPOSI2_ID;
    if(sid_flag & SID_SCALE2) return SID_SCALE2_ID;
    if(sid_flag & SID_ROTAT2) return SID_ROTAT2_ID;
    if(sid_flag & SID_PIVOT2) return SID_PIVOT2_ID;
    if(sid_flag & SID_UVCOO2) return SID_UVCOO2_ID;
    if(sid_flag & SID_VPOSI3) return SID_VPOSI3_ID;
    if(sid_flag & SID_WPOSI3) return SID_WPOSI3_ID;
    if(sid_flag & SID_SCALE3) return SID_SCALE3_ID;
    if(sid_flag & SID_ROTAT3) return SID_ROTAT3_ID;
    if(sid_flag & SID_PIVOT3) return SID_PIVOT3_ID;
    if(sid_flag & SID_COLOR3) return SID_COLOR3_ID;
    if(sid_flag & SID_NORMA3) return SID_NORMA3_ID;
    if(sid_flag & SID_COLOR4) return SID_COLOR4_ID;
    if(sid_flag & SID_PROJMAT4X4) return SID_PROJMAT4X4_ID;
    assert(1);
    return INT_NULL;
}

static uint8
shaderinfo_flag_to_numelems(const sid_t sid_flag)
{
    if(sid_flag & SID_OPACIT) return 1;
    if(sid_flag & SID_VPOSI2) return 2;
    if(sid_flag & SID_WPOSI2) return 2;
    if(sid_flag & SID_SCALE2) return 2;
    if(sid_flag & SID_ROTAT2) return 2;
    if(sid_flag & SID_PIVOT2) return 2;
    if(sid_flag & SID_UVCOO2) return 2;
    if(sid_flag & SID_VPOSI3) return 3;
    if(sid_flag & SID_WPOSI3) return 3;
    if(sid_flag & SID_SCALE3) return 3;
    if(sid_flag & SID_ROTAT3) return 3;
    if(sid_flag & SID_PIVOT3) return 3;
    if(sid_flag & SID_COLOR3) return 3;
    if(sid_flag & SID_NORMA3) return 3;
    if(sid_flag & SID_COLOR4) return 4;
    if(sid_flag & SID_PROJMAT4X4) return 16;
    assert(sid_flag > 0);
}

static uint8
shaderinfo_id_to_numelems(const enum SID_ID type)
{
    switch(type) {
        case SID_OPACIT_ID: return 1;
        case SID_VPOSI2_ID: return 2;
        case SID_WPOSI2_ID: return 2;
        case SID_SCALE2_ID: return 2;
        case SID_ROTAT2_ID: return 2;
        case SID_PIVOT2_ID: return 2;
        case SID_UVCOO2_ID: return 2;
        case SID_VPOSI3_ID: return 3;
        case SID_WPOSI3_ID: return 3;
        case SID_SCALE3_ID: return 3;
        case SID_ROTAT3_ID: return 3;
        case SID_PIVOT3_ID: return 3;
        case SID_COLOR3_ID: return 3;
        case SID_NORMA3_ID: return 3;
        case SID_COLOR4_ID: return 4;
        case SID_PROJMAT4X4_ID: return 16;
        default: assert(0 > 0);
    }

}

static const char*
shaderinfo_type_to_glsltype_string(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT:   return "float";
        case INT64:   return "INT64";
        case UINT64:  return "UINT64";
        case UINT32:  return "int";
        case INT32:   return "uint";
        case UINT16:  return "uint16";
        case INT16:   return "int16";
        case INT8:    return "int8";
        case UINT8:   return "uint8";
        case VEC2F:   return "vec2";
        case VEC3F:   return "vec3";
        case VEC4F:   return "vec4";
        case MAT2X2F: return "mat2";
        case MAT3X3F: return "mat3";
        case MAT4X4F: return "mat4";
        default:      return "NONE";
    }
}

static uint8
shaderinfo_type_to_numelems(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT:   return 1;
        case INT64:   return 1;
        case UINT64:  return 1;
        case UINT32:  return 1;
        case UINT16:  return 1;
        case INT8:    return 1;
        case UINT8:   return 1;
        case VEC2F:   return 2;
        case VEC3F:   return 3;
        case VEC4F:   return 4;
        case MAT2X2F: return 2 * 2;
        case MAT3X3F: return 3 * 3;
        case MAT4X4F: return 4 * 4;
        default: assert(0 > 0);
    }
}

static uint8
shaderinfo_type_to_typesize(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT:   return SIZE_F32;
        case INT64:   return SIZE_I64;
        case UINT64:  return SIZE_I64;
        case UINT32:  return SIZE_I32;
        case UINT16:  return SIZE_I16;
        case INT8:    return SIZE_I8;
        case UINT8:   return SIZE_I8;
        case VEC2F:   return SIZE_F32;
        case VEC3F:   return SIZE_F32;
        case VEC4F:   return SIZE_F32;
        case MAT2X2F: return SIZE_F32;
        case MAT3X3F: return SIZE_F32;
        case MAT4X4F: return SIZE_F32;
        default: assert(1);
    }
}

#endif //GRAPHICS_TEST_SHADER_ID_H
