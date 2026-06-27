/*
 * Created by ckalo on 2/25/26.
 */
#ifndef FILE_SYS_FILE_SYS_INTERFACE_H
#define FILE_SYS_FILE_SYS_INTERFACE_H

#define HOME_DIR "/home/ckalo/data/projects/clion/"
// #define HOME_DIR "/media/ckalo2/data/projects/clion/Debug/"
#include "my_types.h"

struct fs_t;

void fs_init_table();

struct fs_t* fs_create(const char* filename);

void fs_write_list(struct fs_t* fs, struct flist* head, const char* format, ...);

void fs_write(const char* filename, const char* data);

void fs_close(struct fs_t*, const char* caller_function);

void fs_change_filename(struct fs_t* fs, const char* new_filename);

void fs_read(const char* filename);

void fs_append(struct fs_t* fs, const char* format, ...);

void fs_reset(struct fs_t* fs);

void fs_append_mem(struct fs_t* fs, const char* ptr, const uint64 size, const char* msg);

void fs_fprint_all_allocated_mem(const char* filename);

void fs_fprint_all_used_mem(const char* filename);

void fs_append_mem_with_msg(struct fs_t* fs, const char* ptr, const uint64 size, const char* msg);

#endif //FILE_SYS_FILE_SYS_INTERFACE_H
