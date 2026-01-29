///*
// * Created by ckalo on 12/31/25.
// */
//#include "tests.h"
//#include <stdio.h>
//
//#include "logger.h"
//
//static uint32 incrementor = 11;
///*
// * private functions
// */
//void test_linked_list_fill(const uint32 num_elems, struct list_node_t *head)
//{
//    uint32 some_num = incrementor;
//    for (uint32 i = 0; i < num_elems; i++)
//    {
//        linked_list_insert(some_num += incrementor, head);
//    }
//}
//
///*
// * interface functions
// */
//void test_list_insertion(struct list_node_t *head)
//{
//    print("%s-- TEST: --\n", ry, CLEAR);
//    print("%s list initialization and fillup %s\n", ry, CLEAR);
//
//
//    uint32 some_num        = 0;
//    const uint32 num_elems = 10;
//    test_linked_list_fill(num_elems, head);
//    // linked_list_print(head);
//    uint32 data              = some_num + incrementor;
//    struct list_node_t *node = head;
//    for (uint32 i = 0; i < num_elems; i++)
//    {
//        if (node->data != data)
//        {
//            print("%sFAILED%s", r, CLEAR);
//            printf("nodedata:%d data:%d\n", node->data, data);
//        }
//        node = node->next;
//        data += incrementor;
//    }
//    print("%sPASSED%s\n", g, CLEAR);
//
//    print("%s-- TEST: --%s\n", ry, CLEAR);
//}
//
//void test_list_deletion(struct list_node_t *head, const uint32 data)
//{
//    print("%s-- TEST: --\n", ry, CLEAR);
//    print("%s list search and delete by data %s\n", ry, CLEAR);
//    print("%s delete node with data = %u%s\n", ry, data, CLEAR);
//
//    uint32 some_num        = 0;
//    const uint32 num_elems = 10;
//    linked_list_delete_by_data(data, head);
//
//    struct list_node_t *node = head;
//    for (uint32 i = 0; i < num_elems; i++)
//    {
//        if (node->data == data)
//        {
//            print("%sFAILED%s ", r, CLEAR);
//            printf("nodedata:%d data:%d was not deleted\n", node->data, data);
//            return;
//        }
//        node = node->next;
//    }
//
//    print("%sPASSED%s\n", g, CLEAR);
//    print("%s-- TEST: --%s\n", ry, CLEAR);
//}
