/*
 * Created by ckalo on 1/22/26.
 */
#ifndef IO_STD_INPUT_STRUCTS_H
#define IO_STD_INPUT_STRUCTS_H
#include "my_types.h"
#include "std_input_public.h"


struct op{
    char* str;
    func_ptr fn;
    void* params;
} ;

struct op* get_operations();
uint32 get_op_struct_size();
uint32 get_operations_buffer_count();

bool quit_fn0(void* params);
bool help_fn1(void* params);

#endif //IO_STD_INPUT_STRUCTS_H