/*
 * Created by ckalo on 1/6/26.
 */
#ifndef BUFFER_BUFFER_LOG_H
#define BUFFER_BUFFER_LOG_H

#include "buffer.h"
#include "designated_initializers.h"


void
buffer_print_uint32(buffer_print_params);

void
buffer_print_free_list(const buffer_t* const buffer);

void
buffer_print_char(const buffer_t* const buffer);


// ERROR handling
void
buffer_print_error(const enum BUFFER_ERROR_CODE code);

#endif //BUFFER_BUFFER_LOG_H
