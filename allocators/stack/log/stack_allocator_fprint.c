/*
 * Created by ckalo on 8/27/26.
 */

#include "stack_allocator_fprint.h"
#include "my_macros.h"
#include "file_sys_interface.h"
#include "stack_allocator_struct.h"
#include "logger.h"


/***************************************************************************************************
 * FOR DEBUG */
/* output one debug file and register all created mcaches  */
static struct fs_t* fs_created_instances  = NULL;
static struct fs_t* fs_released_instances = NULL;
static struct fs_t* fs_all_instances_mem  = NULL;
/* FOR DEBUG
 **************************************************************************************************/


void fprint_stack_mem_data(struct stack_allocator_instance* stack) {

    if(fs_all_instances_mem == NULL) {
        fs_all_instances_mem = fs_create("Debug/allocators/stack/stack_all_mem.txt");
        fs_append(fs_all_instances_mem,
            "All allocated memory of stack allocator instance data\n"
            "________________________________________________________"
        );
    }

    fs_append(fs_all_instances_mem,
        "######################"
        "stack allocator:\n"
        "# instance_id         %d\n"
        "# offsets_table_start %p\n"
        "# offsets_table_size  %u\n"
        "# offsets_table_count %u\n"
        "# sp_offset           %p\n"
        "# data.start          %p\n"
        "# data.size           %u\n"
        "# data.used_size      %u\n"
        "# data.instance_id    %d\n"
        "######################",
        stack->instance_id,
        stack->offsets_table_start,
        stack->offsets_table_size,
        stack->offsets_table_count,
        stack->sp_offset,

        stack->data.start,
        stack->data.size,
        stack->data.used_size,
        stack->data.instance_id
        );

    fs_append_mem(fs_all_instances_mem, (char*)stack, sizeof(struct stack_allocator_instance), "fprint stack struct");
    fs_append_mem(fs_all_instances_mem, stack->data.start, stack->data.size, "fprint_stack_mem_data");
}