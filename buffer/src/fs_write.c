/*
 * Created by ckalo on 2/24/26.
 */

#include "fs_write.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "my_types.h"
#include "sys_utils.h"

#define MAX_BUFFER 1024

FILE* debug_file = NULL;

void
fs_init()
{
    FILE* file = fopen(FILENAME_MEM_DEBUG, "w");
    if(file == NULL) {
        printf("Error: Could not open file '%s' for writing\n", FILENAME_MEM_DEBUG);
        my_exit("fs_init()\nFailed to open file", __FILE__, __LINE__);
    }
    fclose(file);
}

void
fs_close()
{
    fclose(debug_file);
    printf("Successfully closed file\n");
}

// Function to write data to a file
void
fs_write(const char* filename, const char* data)
{
    debug_file = fopen(filename, "w");
    if(debug_file == NULL) {
        printf("Error: Could not open file '%s' for writing\n", filename);
        return;
    }

    fprintf(debug_file, "%s", data);
    printf("Successfully wrote to '%s'\n", filename);
}

void
fs_append(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(debug_file, format, args);
    va_end(args);
}

void
fs_mem_blocks_debug(const char* filename, const char* start, const char* end, const char* msg)
{
    FILE* file = fopen(filename, "a");
    if(file == NULL) {
        printf("Error: Could not open file '%s' for writing\n", filename);
        return;
    }

    /* define memory boundaries */
    const uint64 size = (uint64)end - (uint64)start;
    const char* data = start;
    const uint32 half_word_size = 1, line_size = 8;

    /* print header, info, and draw a line ____ separator */
    char buffer[256];
    uint32 i = 0, sep_len = (line_size + (line_size/8)) *2 + line_size + 8;
    for(; i < sep_len; i++) buffer[i] = '_';
    buffer[i] = '\0';
    fprintf(file,  "\n%s\n"
                   "%s\n"
                   "size:%u\n"
                   "start:%x\n"
                   "end:%x\n"
                   "dif:%llu bytes\n",
        buffer, msg, size, start, end, (uint64)end-(uint64)start);


    for (size_t i = 0; i < size; i++) {

        if((i) % line_size == 0)
        fprintf(file, " [%5u]", (i)*line_size);
        // Print hex value
        // fprintf(file, "%02X", data[i]);
        // if((i + 1) % half_word_size == 0)
        //     fprintf(file, " ");

        // Print as 16-character hex with leading zeros
        fprintf(file, "%016llX", (unsigned long long)data[i]);
        fprintf(file, " ");


        // Print ASCII representation every 16 bytes
        if ((i + 1) % line_size == 0) {

            fprintf(file, "| ");

            // Print the ASCII for this line
            size_t line_start = i - (i % line_size);
            for (size_t j = line_start; j <= i; j++) {
                if (isprint(data[j])) {
                    fprintf(file, "%c", data[j]);
                } else {
                    fprintf(file, ".");
                }
            }

            fprintf(file, "\n");
        }
    }

    fclose(file);
}

// Function to read and display file contents
void
fs_read(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        printf("Error: Could not open file '%s' for reading\n", filename);
        return;
    }

    char buffer[MAX_BUFFER];
    printf("\nContents of '%s':\n", filename);
    printf("-------------------\n");

    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    if(feof(file)) {
        printf("\n-------------------\n");
        printf("End of file reached.\n");
    }

    fclose(file);
}