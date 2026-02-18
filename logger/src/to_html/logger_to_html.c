/*
 * Created by ckalo on 2/5/26.
 */

#include "logger_to_html.h"

#include <stdio.h>

#include "logger.h"
#include "logger_to_html_css.h"
#include "sys_utils.h"
#include <stdarg.h>
#include <stdlib.h>
// #include <string.h>
#include "memory.h"

#define GROUP_SIZE_BYTES 4096


static void* mem = NULL;



static void mem_alloc(const uint32 size){}
static void mem_append(const uint32 size){}


static char* groups[HTML_GROUP_CAPACITY];
static uint32 pointers[HTML_GROUP_CAPACITY];


static bool temp_file_created = FALSE;

struct node {
    struct node* child;
    struct node* sibling;
    uint32 groupid;
    uint32 classid;
    char* name;
};
uint32 temp_html_node_capacity = 0; // devide allocated memory by the node struct size
struct node* html_head = NULL; // head to the allocated memory

struct node* html_create_node(void)
{
    // struct node
}

void
log_to_html_init(void)
{
    // alloc buffer for head until groups and the html close nodes and end
    // alloc buffer for groups
    // alloc buffer for groups children. These are arbitrary singlr mainly nodes

    // const uint32 size = (sizeof(char) * GROUP_SIZE_BYTES * HTML_GROUP_CAPACITY);
    // mem_block = aligned_alloc(GROUP_SIZE_BYTES, size);
    // memset(mem_block, 0, size);

    //
    // const uint32_t num_pages = 15;
    // memman_init();
    //
    // buffer_init_table();
    //
    // // allocate buffer
    // const uint32 capacity = 16;
    // print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
    // buffer_t* buffer = buffer_create_uint32(capacity);

    // set the head
    html_head = (struct node*)mem;
    html_head->child = NULL;

    // construct: <html>, <head>, <style>, ... nodes>
    struct node* head = html_create_node();

}

void printhtml_group_append_code(uint32 group, uint32 class, const char* format, ...)
{
    uint32 pointer = pointers[group];
    char* cur_address = groups[group] + (pointer * sizeof(char));
    const uint32 max_len = GROUP_SIZE_BYTES - pointer;

    va_list args;
    va_start(args, format);
    uint32 input_len = snprintf(cur_address, max_len, format, args);
    va_end(args);

    pointers[group] += input_len;

    print("pointer: %u\naddress:%p\n", pointers[group], cur_address);

}

void
printhtml_close(const char* dir)
{
    char mode[] = "a";
    if(!temp_file_created) {mode[0] = 'w'; temp_file_created = TRUE;}
    FILE* file;
    file = fopen(dir, mode);
    if (file == NULL) my_exit("ERROR opening file: ", __FILE__, __LINE__);


    fputs("<div class=\"container\";>\n", file);
    fputs(groups[0], file);
    fputs("</div>\n", file);

    print("mem_block:\n%s\n", mem);

    fputs(html_body_close, file);
    fclose(file);
}


void printhtml_destroy(void)
{
    print("destroy mem_block:\n%s\n", mem);
    if(mem != NULL) free(mem);
}

