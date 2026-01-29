/*
 * Created by ckalo on 1/22/26.
 */
#ifndef IO_STD_INPUT_PUBLIC_H
#define IO_STD_INPUT_PUBLIC_H

typedef void* (*func_ptr)(void* params);

void op_set_callback(func_ptr fn_ptr, void* params, char* name);

#endif //IO_STD_INPUT_PUBLIC_H