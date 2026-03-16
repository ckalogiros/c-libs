#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "sys_utils.h"

void
print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void
print_test(const char* name,const char* format, ...)
{
    printf(
        "%s________________________________________________\n"
        "_____________________ TEST _____________________%s\n"
        "      %s%s%s\n",
        GREEN, CLEAR,
        ry, name, CLEAR
    );
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // printf("%s------------------------------------------------%s\n",GREEN, CLEAR);
    printf("%s________________________________________________%s\n\n",GREEN, CLEAR);
}

void
print_mem_hex(const uint32 size, char* address)
{
    print("-------- -------- -------- -------- \n");
    for(uint32 i = 0; i < size; i++) {
        print("%02x", address[i]);
        if((i + 1) % 16 == 0) print("\n");
        else if((i + 1) % (8 / 2) == 0) print(" ");
    }
    print("\n-------- -------- -------- -------- \n");
}


void
logger_test_variadic(const uint32 args_count, ...)
{
    va_list args;
    va_start(args, args_count);
    int total = 0;
    for(int i = 0; i < args_count; i++) {
        // if arg is a string, then the returned value is a pointer
        const uint64 value = va_arg(args, uint64);
        total += value;
    }
    va_end(args);
}

void
print_test_result(const char* test_name, bool passed, const char* file, int line, const char* msg)
{
    // reduce the file directory
    const uint32 path_len = strlen(file);
    const uint32 keep_chars = (path_len > 20) ? 20 : path_len;
    const char* path = &(file[path_len-keep_chars]);
    if(passed) {
        printf("[%s✓%s] %s%s%s %s \"at %s:%d\"", GREEN, CLEAR, ry, test_name, CLEAR, msg, path, line);
    } else {
        printf("%s[FAIL]%s %s%s%s %s \"at %s:%d\"", RED, CLEAR, br3, test_name, CLEAR, msg, path, line);
    }
    printf("\n");
}




/***************************************************************************************************
 * write to file prints
 **************************************************************************************************/

static bool temp_file_created = FALSE;
// static uint16 indentations = 0;

void
dprint(struct _IO_FILE* file, uint16 indent, const char* format, ...)
{
    char mode[] = "a";
    if(!temp_file_created) {
        mode[0] = 'w';
        temp_file_created = TRUE;
    }


    file = fopen("../../test-debug-print.md", mode);
    if(file == NULL)
        my_exit("ERROR opening file: test-debug-print.md", __FILE__, __LINE__);


    if(indent > 0) indent--;

    while(indent--) {
        fputc(' ', file);
    }
    fputs("* ", file);

    // vfprintf(file, "%sHELLO%s", YELLOW, CLEAR);


    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
}


void
html_log_init(FILE* f)
{
    fprintf(f, "<!DOCTYPE html>\n");
    fprintf(f, "<html>\n<head>\n");
    fprintf(f, "<title>Debug Log</title>\n");
    fprintf(f, "<style>\n");
    fprintf(f, "body { font-family: monospace; background: #1e1e1e; color: #d4d4d4; }\n");
    fprintf(f, ".entry { margin: 2px 0; padding: 2px; }\n");
    fprintf(f, ".time { color: #888; }\n");
    fprintf(f, ".error { color: #f44747; font-weight: bold; }\n");
    fprintf(f, ".warn { color: #ff8800; }\n");
    fprintf(f, ".info { color: #4ec9b0; }\n");
    fprintf(f, ".debug { color: #569cd6; }\n");
    fprintf(f, "pre { margin: 0; }\n");
    fprintf(f, "</style>\n</head>\n<body>\n");
    fprintf(f, "<h2>Debug Log</h2>\n<div id='log'>\n");
}

void
html_log_close(FILE* f)
{
    fprintf(f, "</div>\n</body>\n</html>\n");
}

void
dprintmd(struct _IO_FILE* file, const char* dir, const char* filename, uint16 indent, const char* format, ...)
{
    char mode[] = "a";
    if(!temp_file_created) {
        mode[0] = 'w';
        temp_file_created = TRUE;
    }


    char path[256];
    // Construct path
    int result = snprintf(path, sizeof(path), "%s/%s", dir, filename);

    // file = fopen("../../test-debug-print.md", mode);
    file = fopen(path, mode);
    if(file == NULL)
        my_exit("ERROR opening file: ", __FILE__, __LINE__);

#define MD_COLOR_STYLE "<style>" \
    "blue { color: blue; }"      \
    "red { color: red; }"        \
    "green { color: green; }"    \
    "</style>;\n"

    fputs(MD_COLOR_STYLE, file);


    if(indent > 0) indent--;

    while(indent--) {
        fputc(' ', file);
    }
    fputs("* ", file);

    // vfprintf(file, "%sHELLO%s", YELLOW, CLEAR);


    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
}
