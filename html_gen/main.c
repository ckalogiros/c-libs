
#include "buffer_internals.h"
#include "logger.h"
#include "mem_man.h"
#include "html_structs.h"


struct html_node* html_add_child_node(buffer_t* node_buf, struct html_node* parent,
                                    enum TAG_IDS tag_id, const char* name)
{
    struct html_node node = {
        .uid = uuid++,
        .child = NULL, .sibling = NULL,
        .group_id = GROUP_NULL, .tag_id = tag_id,
        .class_bitfield = CLASS_NULL,
        .name = name
    };
    uint16 idx = buffer_set_for_push(node_buf);
    struct html_node* data = node_buf->data;
    data[idx] = node;
    parent->child = &data[idx];

    return &data[idx];
}
struct html_node* html_add_sibling_node(buffer_t* node_buf, struct html_node* prev_sibling,
                                        enum TAG_IDS tag_id, const char* name)
{
    struct html_node node = {
        .uid = uuid++,
        .child = NULL, .sibling = NULL,
        .group_id = GROUP_NULL, .tag_id = tag_id,
        .class_bitfield = CLASS_NULL,
        .name = name
    };
    uint16 idx = buffer_set_for_push(node_buf);
    struct html_node* data = node_buf->data;
    data[idx] = node;
    prev_sibling->sibling = &data[idx];

    return &data[idx];
}

void print_html_tree_style(buffer_t* node_buf);
void print_html_buffer_style(buffer_t* node_buf);

void html_build(buffer_t* node_buf)
{

}

int
main(void)
{
    memory_manager_init(10);
    buffer_init_table();

    buffer_t* node_buf = buffer_create_gentype(16, sizeof(struct html_node));

    struct html_node html = {
        .uid = uuid++,
        .child = NULL, .sibling = NULL,
        .group_id = 0, .tag_id = HTML_TAG,
        .class_bitfield = CLASS_NULL,
        .name = "html"
    };
    uint16 html_idx = buffer_set_for_push(node_buf);
    struct html_node* data = node_buf->data;
    data[html_idx] = html;


    struct html_node* head = html_add_child_node(node_buf, &data[html_idx], HEAD_TAG, "head");
    struct html_node* style = html_add_child_node(node_buf, head, STYLE_TAG, "style");
    struct html_node* body = html_add_child_node(node_buf, head, BODY_TAG, "body");
    struct html_node* group1 = html_add_child_node(node_buf, body, DIV_TAG, "group1");
    struct html_node* p1 = html_add_child_node(node_buf, group1, P_TAG, "p1");
    struct html_node* span1 = html_add_child_node(node_buf, p1, P_TAG, "span1");

    {struct html_node* test = html_add_child_node(node_buf, p1, P_TAG, "test1");}

    // struct html_node* head = html_add_child_node(node_buf, &data[html_idx], HEAD_TAG, "head");
    // struct html_node* style = html_add_child_node(node_buf, head, STYLE_TAG, "style");
    // struct html_node* body = html_add_sibling_node(node_buf, head, BODY_TAG, "body");
    // struct html_node* group1 = html_add_child_node(node_buf, body, DIV_TAG, "group1");
    // struct html_node* p1 = html_add_child_node(node_buf, group1, P_TAG, "p1");
    // struct html_node* span1 = html_add_child_node(node_buf, p1, P_TAG, "span1");
    //
    // {struct html_node* test = html_add_child_node(node_buf, p1, P_TAG, "span1");}



    print_html_tree_style(node_buf);
    print_html_buffer_style(node_buf);
    // print("Hello, World!\n");

    memory_manager_terminate();
    return 0;
}

void print_html_buffer_style(buffer_t* node_buf)
{
    for(uint32 i = 0; i <= node_buf->tail; i++) {
        struct html_node* node = node_buf->data;

        print("[%u]: %s", i, node[i].name);
        if(node[i].sibling) {
            print(" sibling:%s", node[i].sibling->name);
        }
        if(node[i].child) {
            print(" child:%s", node[i].child->name);
        }

        print("\n");

    }
}

void print_html_tree_style(buffer_t* node_buf)
{
    struct html_node* node = (struct html_node*)node_buf->data;
    while(node != NULL) {

        print("%s",  node->name);

        if(node->sibling) {

            struct html_node* sib = (struct html_node*)node->sibling;
            while(sib != NULL) {
                print(" %s ",  sib->name);
                sib = sib->sibling;
                // TODO HERE: must check for children and call recursive.
            }
            print("\n");
        }
        else print("\n");

        node = node->child;
    }
}
