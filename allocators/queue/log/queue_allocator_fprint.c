/*
 * Created by ckalo on 8/27/26.
 */

#include "queue_allocator_fprint.h"
#include "my_macros.h"
#include "file_sys_interface.h"
#include "queue_allocator_struct.h"


/***************************************************************************************************
 * FOR DEBUG */
/* output one debug file and register all created mcaches  */
static struct fs_t* fs_created_instances  = NULL;
static struct fs_t* fs_released_instances = NULL;
static struct fs_t* fs_all_instances_mem  = NULL;
/* FOR DEBUG
 **************************************************************************************************/


void fprint_queue_mem_data(struct queue_allocator_instance* queue) {

    if(fs_all_instances_mem == NULL) {
        fs_all_instances_mem = fs_create("Debug/allocators/queue/queue_all_mem.txt");
        fs_append(fs_all_instances_mem,
            "All allocated memory of queue allocator instance data\n"
            "________________________________________________________\n"
        );
    }

    fs_append(fs_all_instances_mem,
        "######################\n"
        "queue allocator struct:\n"
        "# instance_id         %d\n"
        "# offsets_table_start %p\n"
        "# offsets_table_size  %u\n"
        "# offsets_table_count %u\n"

        "# sp_back             %d\n"
        "# sp_back_idx         %d\n"
        "# sp_front            %d\n"
        "# sp_front_idx        %d\n"

        "# data.start          %p\n"
        "# data.size           %u\n"
        "# data.used_size      %u\n"
        "# data.instance_id    %d\n"
        "######################\n",
        queue->instance_id,
        queue->offsets_table_start,
        queue->offsets_table_size,
        queue->offsets_table_count,

        queue->front_ptr_offset,
        queue->front_ptr_elems_counter,
        queue->back_ptr_offset,
        queue->back_ptr_elems_counter,

        queue->data.start,
        queue->data.size,
        queue->data.used_size,
        queue->data.mem_id
        );
    fs_append_mem(fs_all_instances_mem, (char*)queue, sizeof(struct queue_allocator_instance), "fprint queue struct");

    fs_append_mem(fs_all_instances_mem, queue->data.start, queue->data.size, "queue mem data");
}


