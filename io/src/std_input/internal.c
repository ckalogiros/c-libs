/*
 * Created by ckalo on 1/22/26.
 */

#include "internal.h"
#include "logger.h"
#include "std_input_public.h"

#define MAX_OPERATIONS 32


static struct op operations[MAX_OPERATIONS] = {
    {"quit", (void*)quit_fn0, NULL},
    {"help", (void*)help_fn1, NULL},
};
static uint32 op_size = sizeof(operations) / sizeof(operations[0]);
static uint32 op_cnt = 2; // op buffer element counter

struct op* get_operations()          { return operations; }
uint32 get_op_struct_size()          { return op_size; }
uint32 get_operations_buffer_count() { return op_cnt; }

void op_set_callback(func_ptr fn_ptr, void* params, char* name) {

    operations[op_cnt].str = name;
    operations[op_cnt].fn = fn_ptr;
    operations[op_cnt++].params = params;
}


bool quit_fn0(void* params) {
    return FALSE;
}
bool help_fn1(void* params) {

    print("%s", ry);
    for (uint32 i = 0; i < op_cnt; i++){
        print(" (%u) [ %s ]\n", i, operations[i].str);
    }
    print("%s",CLEAR);
    return TRUE;
}
