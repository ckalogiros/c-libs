/*
 * Created by ckalo on 6/15/26.
 */
#ifndef GRAPHICS_TEST_COMMAND_H
#define GRAPHICS_TEST_COMMAND_H
#include "my_types.h"

#define CMD_CREATE_SHADER_PROGRAM BIT_MASK_FROM_UINT(1)
#define CMD_CREATE_VAO            BIT_MASK_FROM_UINT(2)
#define CMD_CREATE_UNIF_MESH      BIT_MASK_FROM_UINT(3)
#define CMD_SET_CAMERA_PROJ       BIT_MASK_FROM_UINT(4)
#define CMD_ADD_RQ_DRAW_COUNT     BIT_MASK_FROM_UINT(5)

struct command {
    void (*fn)(void*);
    void* params;
};

void cmd_add_command(void* fn, void* params, const uint32 params_size);
void cmd_run(void);

#endif //GRAPHICS_TEST_COMMAND_H