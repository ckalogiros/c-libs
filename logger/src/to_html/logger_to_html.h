/*
 * Created by ckalo on 2/5/26.
 */
#ifndef LOGGER_LOGGER_TO_HTML_H
#define LOGGER_LOGGER_TO_HTML_H
#include "my_types.h"


void log_to_html_init(void);

void printhtml_close(const char* dir);

void printhtml_group_append_code(uint32 group, uint32 class, const char* format, ...);

void printhtml_destroy(void);

#endif //LOGGER_LOGGER_TO_HTML_H