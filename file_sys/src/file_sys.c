/*
 * Created by ckalo on 2/25/26.
 */

/*
 * Created by ckalo on 2/24/26.
 */

#include <assert.h>
#include "file_sys_interface.h"
#include "my_types.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "logger.h"
#include "mem_log.h"
#include "mem_man_internals.h"
#include "sys_utils.h"
#include "mem_structs.h"
#include "file_sys_debug.h"
#include "stringc.h"
#include "string_struct.h"

#define MAX_FILES_TABLE 32
#define MAX_BUFFER 1024

#define FS_CHECK_NULL_FILE(file, filename)\
if(file == NULL) {\
    printf("Error: Could not open file '%s' for writing\n", filename);\
    my_exit("Failed to open file", __FILE__, __LINE__);\
}

// #define dprint print
#define dprint

enum FILE_FLAGS {
    IS_OPEN = 0x1,
};

struct fs_t {
    FILE* file;
    char filename[256];
    uint32 flags;
};


FILE* debug_file = NULL;
static struct fs_t files_table[MAX_FILES_TABLE];
static uint32 files_table_counter = 0;



void fs_init_table()
{
    for(uint32 i = 0; i < MAX_FILES_TABLE; i++) {
        files_table[i] = (struct fs_t){.file = NULL, .filename = {0}, .flags = 0x0};
    }

    // file_sys_debug_init();
}


struct fs_t*
fs_create(const char* filename)
{
    dprint("%s-- FILE SYS: create() --%s\n", ry, CLEAR);

    assert(files_table_counter+1 < MAX_FILES_TABLE);

    const uint32 id = files_table_counter++;
    struct fs_t* fs = &files_table[id];
    fs->flags = 0x0;
    strcat(fs->filename, HOME_DIR);
    strcat(fs->filename, filename);

    fs->file = fopen(fs->filename, "w");
    FS_CHECK_NULL_FILE(fs->file, fs->filename);
    dprint("file:%s\ncreated successfully\n", fs->filename);

    fs_close(fs, "fs_create()");

    return fs;
}

void
fs_write_list(struct fs_t* fs, struct flist* head, const char* format, ...)
{
    assert(head != NULL);
    assert(fs != NULL);
    assert(fs->filename != NULL);

    bool is_opened = FALSE;
    if(!(fs->flags & IS_OPEN)) {
        fs->file = fopen(fs->filename, "a");
        FS_CHECK_NULL_FILE(fs->file, fs->filename);
        is_opened = TRUE;
    }

    va_list args;
    va_start(args, format);
    vfprintf(fs->file, format, args);
    va_end(args);


    struct flist* node = head;
    uint32 cnt = 0;
    while(node) {
        fprintf(fs->file, "[%3u] &node:%p next:%p\n", cnt, &node, node);
        node = node->next;
        cnt++;
    }


    if(is_opened) {
        fs_close(fs, "fs_write_list()");
    }
}

// Function to write data to a file
void
fs_write(const char* filename, const char* data)
{
    debug_file = fopen(filename, "w");
    FS_CHECK_NULL_FILE(debug_file, filename);

    fprintf(debug_file, "%s", data);
    printf("Successfully wrote to '%s'\n", filename);
}

void
fs_close(struct fs_t* fs, const char* caller_function)
{
    dprint("%sclosing file:%s\n %s\n %s%s%s ... ",
           ry, CLEAR, fs->filename, YELLOW, caller_function, CLEAR);
    const int32 res = fclose(fs->file);
    fs->file = NULL;
    dprint("%sfile closed successfully%s\nresult:%u\n", GREEN, res, CLEAR);
}

// Function to read and display file contents
void
fs_read(const char* filename)
{
    // FILE* file = fopen(filename, "r");
    // FS_CHECK_NULL_FILE(file, filename);
    //
    // char buffer[MAX_BUFFER];
    // printf("\nContents of '%s':\n", filename);
    // printf("-------------------\n");
    //
    // while(fgets(buffer, sizeof(buffer), file) != NULL) {
    //     printf("%s", buffer);
    // }
    //
    // if(feof(file)) {
    //     printf("\n-------------------\n");
    //     printf("End of file reached.\n");
    // }
    //
    // fs_close(fs, "fs_read()");
}

void
fs_append(struct fs_t* fs, const char* format, ...)
{
    bool is_opened = FALSE;
    if(!(fs->flags & IS_OPEN)) {
        fs->file = fopen(fs->filename, "a");
        FS_CHECK_NULL_FILE(fs->file, fs->filename);
        is_opened = TRUE;
    }

    va_list args;
    va_start(args, format);
    vfprintf(fs->file, format, args);
    va_end(args);

    if(is_opened) {
        fs_close(fs, "fs_append()");
    }
}

void
fs_append_mem(struct fs_t* fs, const char* ptr, const uint64 size, const char* msg)
{
    fs->file = fopen(fs->filename, "a");
    FS_CHECK_NULL_FILE(fs->file, fs->filename);

    /* define memory boundaries */
    const char* mem_address = ptr;
    const char* start = ptr;
    const uint8* data = (uint8*) start;
    const uint32 word_size = 8, line_size = 32;

    /* print header, info, and draw a line ____ separator */
    char buffer[256];
    uint32 i = 0;
    uint32 sep_len = (line_size + (line_size / word_size)) * 2 + line_size + word_size;
    for(; i < sep_len; i++)
        buffer[i] = '_';
    buffer[i] = '\0';
    fprintf(fs->file, "\n%s\n"
            "message: %s\n"
            "start:%08x %08x\n"
            "end:  %08x %08x\n"
            "dif:[%llu bytes] | [%llx hex] | "
            "size:%llu\n\n",
            buffer, msg,
            (uint32) ((uint64) start >> 32), (uint32) ((uint64) start & 0xFFFFFFFF),
            (uint32) ((uint64) start + size >> 32), (uint32) ((uint64) start + size & 0xFFFFFFFF),
            (uint64) (start + size) - (uint64) start,
            (uint64) (start + size) - (uint64) start, size);


    fprintf(fs->file, "   ADDRESS        | ");
    for(i = 0; i < line_size; i += word_size / 2) {
        uint32 space = (word_size + 1) - 5;
        fprintf(fs->file, "%2u:%2u", i, i + (word_size / 2) - 1);
        while(space--) fprintf(fs->file, " ");
    }
    fprintf(fs->file, "\n");

    const int32 sidx[] = {7, 5, 3, 1, -1, -3, -5, -7};
    uint32 cnt = 0;
    for(i = 0; i < size; i++) {
        /* print memory address (per line) */
        if((i % line_size) == 0) {
            fprintf(fs->file, "%08llx %08llx |",
                    ((uint64) mem_address >> 32),
                    ((uint64) mem_address & 0xFFFFFFFF));
            mem_address += line_size;
        }

        /* print memory hex values (per line, reverse printing) */
        fprintf(fs->file, "%02x", data[i + sidx[cnt++]]);
        if(cnt == 8) cnt = 0;
        if((i + 1) % (word_size / 2) == 0)
            fprintf(fs->file, " ");

        /* print number of bytes (per line) */
        if(((i + 1) % line_size) == 0)
            fprintf(fs->file, "[%5u] ", (i + 1) - line_size);

        /* print memory ASCII representation (per line) */
        if((i + 1) % line_size == 0) {
            fprintf(fs->file, "| ");

            // Print the ASCII for this line
            const size_t line_start = i - (i % line_size);
            for(size_t j = line_start; j <= i; j++) {
                if(isprint(data[j])) fprintf(fs->file, "%c", data[j]);
                else fprintf(fs->file, ".");
                if((j + 1) % word_size == 0) fprintf(fs->file, " ");
            }

            fprintf(fs->file, "\n");
        }
    }

    fs_close(fs, "fs_mem_blocks_debug()");
}

void
fs_fprint_all_allocated_mem(const char* filename)
{
    struct fs_t* fs = fs_create(filename);
    fs->file = fopen(fs->filename, "a");
    FS_CHECK_NULL_FILE(fs->file, fs->filename);

    /* print all allocated memory from mem table */
    const struct mem* mem_table = debug_mem_get_table();
    const uint32 mem_table_count = debug_mem_get_table_count();
    for(uint32 i = 0; i < mem_table_count; i++) {
        const char* mem_address = mem_table[i].start;
        const uint64 size = mem_table[i].size;
        fs_append_mem(fs, mem_address, size, "");
    }
}

void
fs_fprint_all_used_mem(const char* filename, const memid_t memid)
{
    struct fs_t* fs = fs_create(filename);
    fs->file = fopen(fs->filename, "a");
    FS_CHECK_NULL_FILE(fs->file, fs->filename);

    /* print used pages of memory, from mem table by id */
    struct mem* mem_table = debug_mem_get_table();


    struct mem* mem = &mem_table[memid];
    for(uint32 i = 0; i < mem->size; i += PAGE_SIZE) {

        const char* address = (char*)&mem->start[i];
        const char* offset_addr = FREE_LIST_GET_PAGE_COLOR_OFFSET(address);

        if(*(uint32_t*)offset_addr == (uint32) COLOR_PAGE_FREE) {
            fs_append(fs, "[%5u] FREE PAGE | addr:%08x %08x\n", i,
                (uint64) address >> 32, (uint64) address & 0xFFFFFFFF
            );
        } else {
            mem_log_print_mem(mem);
            fs_append_mem(fs, address, PAGE_SIZE, "");
        }
    }
}



void
fs_append_mem_with_msg(struct fs_t* fs, const char* ptr, const uint64 size, const char* msg)
{
    fs->file = fopen(fs->filename, "a");
    FS_CHECK_NULL_FILE(fs->file, fs->filename);

    /* define memory boundaries */
    const char* mem_address = ptr;
    const char* start = ptr;
    const uint8* data = (uint8*) start;
    const uint32 word_size = 8, line_size = 32;

    /* print header, info, and draw a line ____ separator */
    char buffer[256];
    uint32 i = 0;
    uint32 sep_len = (line_size + (line_size / word_size)) * 2 + line_size + word_size;
    for(; i < sep_len; i++)
        buffer[i] = '_';
    buffer[i] = '\0';
    fprintf(fs->file, "\n%s\n"
            "message: %s\n"
            "start:%08x %08x\n"
            "end:  %08x %08x\n"
            "dif:[%llu bytes] | [%llx hex] | "
            "size:%llu\n\n",
            buffer, msg,
            (uint32) ((uint64) start >> 32), (uint32) ((uint64) start & 0xFFFFFFFF),
            (uint32) ((uint64) start + size >> 32), (uint32) ((uint64) start + size & 0xFFFFFFFF),
            (uint64) (start + size) - (uint64) start, (uint64) (start + size) - (uint64) start,
            size);


    fprintf(fs->file, "   ADDRESS        | ");
    for(i = 0; i < line_size; i += word_size / 2) {
        uint32 space = (word_size + 1) - 5;
        fprintf(fs->file, "%2u:%2u", i, i + (word_size / 2) - 1);
        while(space--) fprintf(fs->file, " ");
    }
    fprintf(fs->file, "\n");

    const int32 sidx[] = {7, 5, 3, 1, -1, -3, -5, -7};
    uint32 cnt = 0;
    for(i = 0; i < size; i++) {
        /* print memory address (per line) */
        if((i % line_size) == 0) {
            fprintf(fs->file, "%08llx %08llx |",
                    ((uint64) mem_address >> 32),
                    ((uint64) mem_address & 0xFFFFFFFF));
            mem_address += line_size;
        }

        /* print memory hex values (per line, reverse printing) */
        fprintf(fs->file, "%02x", data[i + sidx[cnt++]]);
        if(cnt == 8) cnt = 0;
        if((i + 1) % (word_size / 2) == 0)
            fprintf(fs->file, " ");

        /* print number of bytes (per line) */
        if(((i + 1) % line_size) == 0)
            fprintf(fs->file, "[%5u] ", (i + 1) - line_size);


        if((i + 1) % line_size == 0) {
            fprintf(fs->file, "| ");

        // Print the ASCII for this line
        const size_t line_start = i - (i % line_size);
        for(size_t j = line_start; j <= i; j++) {

            char* ptr = (char*)&data[j];
            const uint32 exists = file_sys_exists(ptr);
            if(exists) {
                char buf[128] = {0};
                file_sys_retrieve_info(buf);
                fprintf(fs->file, "%s", buf);
            }
        }

        // for(size_t j = line_start; j <= i; j++) {
        //     if(isprint(data[j])) fprintf(fs->file, "%c", data[j]);
        //     else fprintf(fs->file, ".");
        //     if((j + 1) % word_size == 0) fprintf(fs->file, " ");
        // }

        fprintf(fs->file, "\n");
        }
    }

    fs_close(fs, "fs_mem_blocks_debug()");
}