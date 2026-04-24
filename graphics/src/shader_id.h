/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_SHADER_ID_H
#define GRAPHICS_TEST_SHADER_ID_H
#include "my_types.h"


typedef uint64 sid_t;

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

struct gfx_sid {
    sid_t attr;
    sid_t unif;
    sid_t out;
    sid_t pass;
    sid_t oper;
};

static bool sid_check_match(struct gfx_sid sid1, struct gfx_sid sid2) {

    // Remove all Block Uniforms flags. Better to reuse a program than to create a duplicate only because some meshes dont use Blocked Uniforms.
    // For individual uniforms we must include for the match, since a Program's shaders may be compiled before the creation of other meshes with the same sid except an extra uniform
    // const  unif1 = sid1.unif & ~(SID.UNIF.PROJECTION_MATRIX|SID.UNIF.SCREEN_RESOLUTION);
    // const  unif2 = sid2.unif & ~(SID.UNIF.PROJECTION_MATRIX|SID.UNIF.SCREEN_RESOLUTION);

    // const  attr1 = sid1.attr & ~(SID.ATTR.TEST_MULTI_VERTEX|SID.ATTR.COL4_PER_VERTEX)
    // const  attr2 = sid2.attr & ~(SID.ATTR.TEST_MULTI_VERTEX|SID.ATTR.COL4_PER_VERTEX)
    // const sid_t attr1 = sid1.attr & ~(SID.ATTR.TEST_MULTI_VERTEX);
    // const sid_t attr2 = sid2.attr & ~(SID.ATTR.TEST_MULTI_VERTEX);

    // const shad1 = sid1.shad;
    // const shad2 = sid2.shad;
    // const shader_ops1 = sid1.shad_ops;
    // const shader_ops2 = sid2.shad_ops;
    // // const  shad1 = sid1.shad & ~(SID.SHAD.INDEXED_PATT_CONNECTED_FACES_X);
    // // const  shad2 = sid2.shad & ~(SID.SHAD.INDEXED_PATT_CONNECTED_FACES_X);
    if (
          // shad1 === shad2 &&
          sid1.attr == sid2.attr &&
          sid1.unif == sid2.unif &&
          sid1.pass == sid2.pass
          // sid1.inst === sid2.inst &&
          // shader_ops1 === shader_ops2 &&
          // sid1.fx === sid2.fx
    ) {
        return TRUE;
    }

    return FALSE;
}


#endif //GRAPHICS_TEST_SHADER_ID_H