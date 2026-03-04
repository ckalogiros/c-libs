/*
 * Created by ckalo on 12/31/25.
 */

#ifndef MY_MACROS_H
#define MY_MACROS_H


#include <assert.h> // todo: (???)move to another include file
#include <stddef.h> // todo: (???)move to another include file

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))


/**
 * @brief Cast pointer to specified type
 * @param TYPE Destination pointer type
 * @param PTR Source pointer
 * @return Pointer cast to TYPE
 */
#define CAST_PTR(TYPE, PTR) ((TYPE) (PTR))

/**
 * @brief Cast to void* (useful for generic containers)
 */
#define TO_VOID(ptr) ((void *) (ptr))

/**
 * @brief Cast from void* to specific pointer type
 */
#define FROM_VOID(type, ptr) ((type *) (ptr))

/**
 * @brief Cast and dereference in one operation
 * Example: DEREF_CAST(int*, some_void_ptr) = 42;
 */
#define DEREF_CAST(type, ptr) (*(type *) (ptr))

/**
 * @brief Cast pointer to integer (for pointer arithmetic)
 * Useful for offset calculations
 */
#define PTR_TO_INT(ptr) ((uintptr_t) (ptr))
#define INT_TO_PTR(type, val) ((type *) (uintptr_t) (val))

/**
 * @brief Container-of macro (like Linux kernel's container_of)
 * Get containing structure from member pointer
 */
#define CONTAINER_OF(ptr, type, member) ((type *) ((char *) (ptr) - offsetof(type, member)))


#define ROUND_UP(val, limit) (((val) + limit - 1) & ~((limit - 1)))
#define ROUND_DOWN(val, limit) (((val) & ~((limit - 1))))

#endif // MY_MACROS_H
