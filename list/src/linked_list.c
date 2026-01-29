//
// Created by ckalo on 12/30/25.
//

#include "linked_list.h"
#include "linked_list_private.h"
#include <stddef.h>

/*
 * A uint32 implementation
 * todo: lists should be an interface and let the client define the type of the
 * "data"
 *
 * todo: find by data (???) what if data is a struct
 * todo: find by id
 * todo: push at start
 * todo: push at end
 * todo: delete by data (???) what if data is a struct
 * todo: delete by id
 * todo: sort by list's links
 * todo: sort by data
 * todo: sort by id
 * todo: consolidate - in case the buffer is fragmented
 *
 * todo: create buffer structures to store all lists created. dont forget the
 * todo: decide if the client gets a handle or the pointer
 * todo: implement node metadata/flags in a bitmap
 */



sll_node_t* sll_create_list(void* const data)
{
    sll_node_t* head = node_alloc();
    head->data       = data;
    head->id         = 0;
    head->next       = NULL;
    return head;
}

list_node_id sll_insert(sll_node_t* const head, void* const data)
{
    if (head->next == NULL) {
        head->next = node_alloc_renameme(head);
        head->next->id = head->id + 1;
        head->next->data = data;
        return head->next->id;
    }

    sll_node_t* curr = head->next;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = node_alloc_renameme(head);
    curr->next->id = curr->id + 1;
    curr->next->data = data;

    return curr->next->id;
}

void sll_delete_byid(sll_node_t** head, const list_node_id id)
{
    if ((*head)->id == id) {              // case head is to be deleted
        sll_node_t* curr = (*head);
        (*head)          = (*head)->next; // set next as head
        curr->next       = NULL;          // remove heads reference
        sll_release(curr);               // delete head
        curr = NULL;
        return;
    }

    sll_node_t* prev_node = find_byid_ret_prev(*head, id);
    if (prev_node == NULL) {
        handle_error(OP_DELETE|NOT_FOUND);
        return;
    }

    sll_node_t* prev = prev_node;
    sll_node_t* curr = prev_node->next;
    // check if prev is the last node
    if (curr != NULL) {
        sll_node_t* next = prev_node->next->next;
        prev->next = next;
        curr->next = NULL;
        sll_release(curr);
    }
    else {
        prev->next = NULL;
    }

}
