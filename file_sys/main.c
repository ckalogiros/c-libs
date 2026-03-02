#include <stdio.h>
#include "file_sys_interface.h"
#include "my_macros.h"

// /home/ckalo/data/projects/clion/file_sys
#define HOME_DIR "/home/ckalo/data/projects/clion/file_sys/debug.txt"
#define LEN1 128

int
main(void)
{
    // struct fs_t* fs = fs_create(TO_STRING(HOME_DIR/file_sys/debug.txt));
    struct fs_t* fs = fs_create("/file_sys/debug.txt");

    char buf1[LEN1] = "This is a test buffer";
    fs_append(fs, "buffer1:%x\n");
    fs_append_mem(fs, buf1, LEN1, "");

    char buf2[LEN1] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    fs_append(fs, "buffer2:%x\n");
    fs_append_mem(fs, buf2, LEN1, "");

    return 0;
}

