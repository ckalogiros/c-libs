/*
 * Created by ckalo on 8/10/26.
 */
#ifndef GRAPHICS_TEST_UNIFORMS_H
#define GRAPHICS_TEST_UNIFORMS_H

#include "buffer_arr_struct.h"
#include "gfx_types.h"
#include "mem_man_types.h"
#define MAX_UNIFORMS 16


union uniform_data{
    struct { /// single values, upp to vec4 type
        uint32 v0;
        uint32 v1;
        uint32 v2;
        uint32 v3;
    }value;
    struct { // array buffer (for larger than vec4 types)
        float* data;
        uint32 count;
        bool   transpose;
    }buffer;
};


struct uniform_fnptr {
    void (*fnptr)(union uniform_data*, int32);
};

struct uniform_descriptor {
    sid_t  unif_flag;               /// flag of type: SID_... VPOS2,WPOS2,COL4, etc
    sid_t  shader_input_flags;      /// flag of type: SHADER_INPUT_FLAGS aka IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, etc
    attr_t unif_offset;             /// the start offset of the attribute  in the attributes data buffer
    attr_t unif_num_elems;          /// how many: floats | ints etc, for the attribute
    int16  loc;
    // union  uniform_data unif_data;  /// the actual data of a uniform
    struct uniform_fnptr update_fn; /// the callback function for the uniform's upload to the gpu
};


struct uniforms_group {
    struct uniform_descriptor unif_descr[MAX_UNIFORMS]; /// attributes descriptor
    sid_t    added_uniforms_flag;   /// a way to know if a uniform is already added to this descriptors group
    sid_t    commands_flag;         /// a way to know any commands scheduled to be run
    sididx_t sid_idx;               /// the index to the shader id buffer
    memid_t  unif_data_id;          /// the index to the allocator's table for the uniforms data buffer.
    attr_t   unif_count;            /// the number of unique uniform
    attr_t   unif_desc_count;       /// a counter for the uniforms_desc buffer

};

void     unifGroupDescr_table_init(void);
attrid_t unifGroupDescr_create_new_unif_group(sididx_t sid_idx);
attrid_t unifGroupDescr_group_find_match(sididx_t sid_idx);
/// TODO: NOT USED?
void     unifGroupDescr_add_uniforms(const attrid_t unif_idx, const float* const src,
                                     const uint32 elems_count, const sid_t unif_flag);

struct uniforms_group* unifGroupDescr_get_unif_group(attrid_t unif_idx);
struct uniform_descriptor* unifGroupDescr_get_unif_descr(const attrid_t unif_idx);
attr_t    unifGroupDescr_get_unif_count(const attrid_t unif_idx);
float*    unifGroupDescr_get_unif_data(attrid_t unif_idx);


void gl_uniform1f             (union uniform_data* params, int32 location);         // (int location     , float  v0);
void gl_uniform1fv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , float *value);
void gl_uniform1i             (union uniform_data* params, const int32 location);   // (int location     , int    v0);
void gl_uniform1iv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , int *value);
void gl_uniform1ui            (union uniform_data* params, const int32 location);   // (int location     , uint32 v0);
void gl_uniform1uiv           (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , uint32 *value);
void gl_uniform2f             (union uniform_data* params, const int32 location);   // (int location     , float  v0     , float   v1);
void gl_uniform2fv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , float  *value);
void gl_uniform2i             (union uniform_data* params, const int32 location);   // (int location     , int    v0     , int     v1);
void gl_uniform2iv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , int    *value);
void gl_uniform2ui            (union uniform_data* params, const int32 location);   // (int location     , uint32 v0     , uint32  v1);
void gl_uniform2uiv           (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , uint32 *value);
void gl_uniform3f             (union uniform_data* params, const int32 location);   // (int location     , float  v0     , float   v1       , float  v2);
void gl_uniform3fv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , float  *value);
void gl_uniform3i             (union uniform_data* params, const int32 location);   // (int location     , int    v0     , int     v1       , int    v2);
void gl_uniform3iv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , int    *value);
void gl_uniform3ui            (union uniform_data* params, const int32 location);   // (int location     , uint32 v0     , uint32  v1       , uint32 v2);
void gl_uniform3uiv           (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , uint32 *value);
void gl_uniform4f             (union uniform_data* params, const int32 location);   // (int location     , float  v0     , float   v1       , float  v2    , float  v3);
void gl_uniform4fv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , float  *value);
void gl_uniform4i             (union uniform_data* params, const int32 location);   // (int location     , int    v0     , int     v1       , int    v2    , int    v3);
void gl_uniform4iv            (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , int    *value);
void gl_uniform4ui            (union uniform_data* params, const int32 location);   // (int location     , uint32 v0     , uint32  v1       , uint32 v2    , uint32  v3);
void gl_uniform4uiv           (union uniform_data* params, const int32 location);   // (int location     , uint32 count  , uint32 *value);
void gl_uniform_matrix2fv     (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix2x3fv   (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix2x4fv   (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix3fv     (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix3x2fv   (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix3x4fv   (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix4fv     (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix4x2fv   (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_matrix4x3fv   (union uniform_data* params, const int32 location);   // (int location, uint32 count, bool transpose, const float * value);
void gl_uniform_block_binding (uint32 program, uint32 uniformBlockIndex, uint32 uniformBlockBinding);   // (uint32 program, uint32 uniformBlockIndex, uint32 uniformBlockBinding);


/* DEBUG-PRINT */
void unifGroupDescr_group_print();


#endif //GRAPHICS_TEST_UNIFORMS_H