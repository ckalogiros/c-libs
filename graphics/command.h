/*
 * Created by ckalo on 6/15/26.
 */
#ifndef GRAPHICS_TEST_COMMAND_H
#define GRAPHICS_TEST_COMMAND_H
#include "my_types.h"

struct command {
    void (*fn)(void*);
    void* params;
};

void cmd_add_command(void* fn, void* params, const uint32 params_size);
void cmd_run(void);

#endif //GRAPHICS_TEST_COMMAND_H