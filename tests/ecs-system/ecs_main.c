/*
 * Created by ckalo on 1/27/26.
 */

#include "ecs.h"
#include "mem_man.h"

int main(void)
{


  memory_manager_init(10);

  test_ecs();

  memory_manager_terminate();

  return 0;
}