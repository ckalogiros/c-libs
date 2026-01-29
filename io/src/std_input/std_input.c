/*
 * Created by ckalo on 1/22/26.
 */

#include "std_input.h"
#include "internal.h"
#include "logger.h"
#include <string.h>



bool find_op(const char* str) {

    int16 found_index = INT_NULL;
    struct op* operations = get_operations();
    const uint32 op_count = get_operations_buffer_count();
    bool ret = TRUE;

    for (uint32 i = 0; i < op_count; i++){

        const int32 matches = strcmp(str, operations[i].str);

        if (matches == 0){
            found_index = i;
            break;
        }
    }

    if (found_index != INT_NULL){
        print("operation \"%s\" found\n", operations[found_index].str);
        void* params = operations[found_index].params;
        ret = (uint64)operations[found_index].fn(params);
    }
    else{
        print("No match found for operation: \"%s\"\n"
              "type %s\"help\"%s for help\n",
                str, YELLOW, CLEAR
              );
    }

    return ret;
}
