#include "linked_list.h"
#include "linked_list_private.h"
#include "list_log.h"
#include "logger.h"
#include "mem_man.h"
#include "tests.h"


void test_linked_list_create_insert_delete();

int main(void)
{
    memory_manager_init(4);

//    print("create head\n");
//    uint32 data = 11;
//    sll_node_t* head  = sll_create(&data);
//    list_print_list(head);

//    uint32 data2 = 22;
//    print("insert: %u\n", data2);
//    list_node_id data2_id = sll_insert(head, &data2);
//    list_print_list(head);


    test_linked_list_create_insert_delete();

    memory_manager_terminate();
    return 0;
}


void test_linked_list_create_insert_delete()
{
    print("create head\n");
    uint32 data = 11;
    sll_node_t* head  = sll_create_list(&data);
    list_print_list(head);


    // list insert
    uint32 data2 = 22;
    print("insert: %u\n", data2);
    list_node_id data2_id = sll_insert(head, &data2);
    uint32 data3          = 33;
    print("insert: %u\n", data3);
    list_node_id data3_id = sll_insert(head, &data3);
    uint32 data4          = 44;
    print("insert: %u\n", data4);
    list_node_id data4_id = sll_insert(head, &data4);
    list_print_list(head);

    // list remove
    print("remove data_id= %u\n", data2_id);
    sll_delete_byid(&head, data2_id);
    list_print_list(head);
    print("remove data_id= %u\n", data4_id);
    sll_delete_byid(&head, data4_id);
    list_print_list(head);
    print("remove data_id= %u\n", data4_id);
    sll_delete_byid(&head, data3_id);
    list_print_list(head);


    // list insert
    print("insert: %u\n", data2);
    data2_id = sll_insert(head, &data2);
    print("insert: %u\n", data3);
    data3_id = sll_insert(head, &data3);
    print("insert: %u\n", data4);
    data4_id = sll_insert(head, &data4);
    list_print_list(head);

    print("remove all list items from head\n", data2_id);
    print("remove data_id= %u\n", head->id);
    sll_delete_byid(&head, head->id);
    list_print_list(head);
    print("remove data_id= %u\n", head->id);
    sll_delete_byid(&head, head->id);
    list_print_list(head);
    print("remove data_id= %u\n", head->id);
    sll_delete_byid(&head, head->id);
    list_print_list(head);
    print("remove data_id= %u\n", head->id);
    sll_delete_byid(&head, head->id);
    list_print_list(head);

}