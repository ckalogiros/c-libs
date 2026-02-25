/*
 * Created by ckalo on 2/24/26.
 */
#ifndef BUFFER_FS_WRITE_H
#define BUFFER_FS_WRITE_H


#define FILENAME_MEM_DEBUG "/home/ckalo/data/projects/clion/memory/mem_debug.txt"

void fs_init();

void fs_write(const char *filename, const char *data);

void fs_close();

// Function to read and display file contents
void fs_read(const char *filename);

// Function to append data to an existing file
void fs_append(const char* format, ...);

//
void fs_mem_blocks_debug(const char* filename, const char* start, const char* end, const char* msg);

#endif //BUFFER_FS_WRITE_H