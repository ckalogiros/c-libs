1. Generic Implementation<br/>
<pre style="color:#fca905;">
<code>
<tag style="color:#fca905;">
typedef struct Node {
    void *data;
    struct Node *next;
} Node;
<br/>
typedef struct LinkedList {
    Node *head;
    size_t size;
} LinkedList;                 
</tag>
</code>
</pre>
2. Macro-Based Generic Implementation<br/>
<pre>
<code style="color:#fca905;">
<tag style="color:#fca905;">
#define DEFINE_LIST(TYPE) \
typedef struct Node_##TYPE { \
    TYPE data; \
    struct Node_##TYPE *next; \
} Node_##TYPE; \
<br/>
typedef struct List_##TYPE { \
Node_##TYPE *head; \
size_t size; \
} List_##TYPE;
<br/>
// Usage
DEFINE_LIST(int)
DEFINE_LIST(float)
DEFINE_LIST(struct MyStruct)
</tag>
</code>
</pre>
