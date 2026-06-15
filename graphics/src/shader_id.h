/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_SHADER_ID_H
#define GRAPHICS_TEST_SHADER_ID_H
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

enum SID_ID {
    // SID_NULL_=0,
    SID_OPACIT_IDI = 0,
    SID_VPOSI2_IDI,
    SID_WPOSI2_IDI,
    SID_SCALE2_IDI,
    SID_ROTAT2_IDI,
    SID_PIVOT2_IDI,
    SID_UVCOO2_IDI,
    SID_VPOSI3_IDI,
    SID_WPOSI3_IDI,
    SID_SCALE3_IDI,
    SID_ROTAT3_IDI,
    SID_PIVOT3_IDI,
    SID_COLOR3_IDI,
    SID_NORMA3_IDI,
    SID_COLOR4_IDI,
    SID_PROJMAT4X4_IDI,

    SID_IDI_COUNT,
};



typedef uint64 sid_t;

/// TODO: unite some flags into one
struct gfx_sid {
    sid_t attr;
    sid_t unif;
    sid_t out;
    sid_t pass;
    sid_t oper;
};

static bool
sid_check_match(struct gfx_sid sid1, struct gfx_sid sid2)
{
    if(sid1.attr == sid2.attr &&
       sid1.unif == sid2.unif &&
       sid1.pass == sid2.pass) {
        return TRUE;
    }

    return FALSE;
}


static bool
sid_copy(struct gfx_sid* dst, struct gfx_sid src)
{
    dst->attr = src.attr;
    dst->unif = src.unif;
    dst->out  = src.out;
    dst->pass = src.pass;
    dst->oper = src.oper;
}

struct gfx {
    // TODO: sid is same for shader build or object group, so move it to object_group_descriptor and use a getter function
    struct gfx_sid sid;
};

#endif //GRAPHICS_TEST_SHADER_ID_H
