//
// Created by ckalo on 12/28/25.
//

#ifndef PERF_TIMER_EXPERIMENT_H
#define PERF_TIMER_EXPERIMENT_H
#include <stdio.h>

/************************************************************/
// const on structs

typedef struct {
    int x;
    int y;
}my_struct;

void my_struct_print(my_struct s) {
    printf("Struct\n x=%d\n y=%d\n", s.x, s.y);
}

void test_const_for_structs() {

    // const my_struct s = {1, 2,};
    my_struct s = {1, 2,};

    s.x = 10;

    my_struct_print(s);
}

#endif //PERF_TIMER_EXPERIMENT_H