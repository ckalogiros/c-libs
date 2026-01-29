/*
 * Created by ckalo on 1/13/26.
 */

#include "list_log.h"
#include "logger.h"
#include <stddef.h>


void list_print_list(sll_node_t* head)
{
    print("-- PRINT LIST\n");

    if (head == NULL) {
        print("    list is empty\n");

    }

    const sll_node_t* curr = head;
    while (curr != NULL) {
        print("    id:%u data=%u\n", curr->id, *((uint32*)curr->data));
        curr = curr->next;
    }
    print("--\n");
}
