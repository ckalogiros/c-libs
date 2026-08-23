/*
 * Created by ckalo on 1/6/26.
 */

#include "sys_utils.h"
#include <stdio.h>
#include "file_sys_interface.h"
#include "logger.h"

void
my_exit(const char* msg, const char* file, const uint32 line)
{
    print("%s####################################################\n"
           "%s%s\nEXIT AT %s\nline:%u\n", RED, msg, CLEAR, file, line);
    print("%s####################################################%s\n", RED, CLEAR);
    
    const char* filename = "/crash_debug.txt";
    struct fs_t* fs = fs_create(filename);
    fs_fprint_all_allocated_mem(filename);
    fs_append(fs, "CRASH EXIT\nmsg:%s", msg);

    exit(1);
}



/***************************************************************************************************
 * SOME INTERNET CODE, NEEDS TO BE IMPLEMENTED
 **************************************************************************************************/
// #include <xmmintrin.h>
// #include <emmintrin.h>
#include <immintrin.h>

// Check for AVX support
#if defined(__AVX__)
#   define AVX1
#elif defined(__AVX2__)
#   define AVX2
#else
#   warning AVX is not available
#endif

void
memset_128bit(void* dest, const uint64 val, const uint64 size)
{
    const uint32_t val_size = sizeof(__m128i);
    __m128i vector_val = _mm_set1_epi64x(val);  // Replicate val to both 64-bit lanes
    
    // Check if pointer is 16-byte aligned
    if (((uintptr_t)dest & 15) == 0) {
        // Aligned case
        __m128i* ptr = (__m128i*)dest;
        for (uint64 i = 0; i < size / val_size; i++) {
            _mm_store_si128(ptr + i, vector_val);
        }
    } else {
        // Unaligned case
        char* byte_ptr = (char*)dest;
        for (uint64 i = 0; i < size; i += val_size) {
            _mm_storeu_si128((__m128i*)(byte_ptr + i), vector_val);
        }
    }
    
    // Handle remaining bytes if size is not multiple of 16
    uint64 remaining = size % val_size;
    if (remaining > 0) {
        char* byte_ptr = (char*)dest + (size - remaining);
        char* val_bytes = (char*)&val;
        for (uint64 i = 0; i < remaining; i++) {
            byte_ptr[i] = val_bytes[i % sizeof(uint64)];
        }
    }
}

void
memset_64bit(const char* src, const uint64 val, const uint64 size)
{
    const uint32 val_size = sizeof(uint64);
    uint64* ptr = (uint64*)src;

    for (size_t i = 0; i < size; i+= val_size) {

        *(ptr) = val;
        ptr++;
    }
}

#if defined(AVX1) || defined(AVX2)

void 
memset_256bit_optimized(void* dest, const uint64 val, const uint64 size)
{
    const uint32_t val_size = sizeof(__m256i);
    __m256i vector_val = _mm256_set1_epi64x(val);

    uint64 num_vectors = size / val_size;
    __m256i* ptr = (__m256i*)dest;
    uint64 i = 0;

    // Process 4 vectors at a time for better performance
    for (; i + 3 < num_vectors; i += 4) {
        _mm256_store_si256(ptr + i, vector_val);
        _mm256_store_si256(ptr + i + 1, vector_val);
        _mm256_store_si256(ptr + i + 2, vector_val);
        _mm256_store_si256(ptr + i + 3, vector_val);

        // Prefetch next cache line (optional)
        _mm_prefetch((const char*)(ptr + i + 8), _MM_HINT_T0);
    }

    // Handle remaining vectors
    for (; i < num_vectors; i++) {
        _mm256_store_si256(ptr + i, vector_val);
    }

    // Handle remaining bytes
    uint64 remaining = size % val_size;
    if (remaining > 0) {
        uint8* byte_ptr = (uint8*)(ptr + num_vectors);
        uint8* val_bytes = (uint8*)&val;
        for (uint64 j = 0; j < remaining; j++) {
            byte_ptr[j] = val_bytes[j % sizeof(uint64)];
        }
    }
}


void memset_256bit_streaming(void* dest, const uint64 val, const uint64 size)
{
    const uint32_t val_size = sizeof(__m256i);
    __m256i vector_val = _mm256_set1_epi64x(val);
    
    // Ensure 32-byte alignment for streaming stores
    if (((uintptr_t)dest & 31) != 0) {
        // Fall back to regular memset_256bit for unaligned
        memset_256bit_optimized(dest, val, size);
        return;
    }
    
    uint64 num_vectors = size / val_size;
    __m256i* ptr = (__m256i*)dest;
    
    // Use non-temporal stores to avoid cache pollution
    for (uint64 i = 0; i < num_vectors; i++) {
        _mm256_stream_si256(ptr + i, vector_val);
    }
    
    // Need sfence to ensure streaming stores are visible
    _mm_sfence();
    
    // Handle remaining bytes
    uint64 remaining = size % val_size;
    if (remaining > 0) {
        uint8* byte_ptr = (uint8*)(ptr + num_vectors);
        uint8* val_bytes = (uint8*)&val;
        for (uint64 j = 0; j < remaining; j++) {
            byte_ptr[j] = val_bytes[j % sizeof(uint64)];
        }
    }
}
#endif


/***************************************************************************************************
 * SOME INTERNET CODE, NEEDS TO BE IMPLEMENTED
 **************************************************************************************************/
// If c is a hex digit returns its numeric value, otherwise -1.
int32 digit_to_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}


/***************************************************************************************************
 * OLD CODE, NEEDS TO BE IMPLEMENTED
 **************************************************************************************************/

uint32 reverse_bytes_uint32(uint32 value)
{
    // 31 32 33 34
    //  - -- -- - 6 hex = 4*6 = 24
    //     - -    2 hex = 4*2 = 8
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) << 8)  |
            ((value & 0x00FF0000) >> 8)  |
            ((value & 0xFF000000) >> 24));
}
uint64 reverse_bytes_uint64(uint64 value)
{
    // 31 32 33 34 35 36 37 38
    //  - -- -- -- -- -- -- - 16 hex = 4*16 = 56
    //     - -- -- -- -- -    10 hex = 4*10 = 40
    //        - -- -- -        6 hex = 4*6  = 24
    //           - -           2 hex = 4*2  =  8
    return (
        ((value & 0x00000000000000ff) << 56) |
        ((value & 0x000000000000FF00) << 40) |
        ((value & 0x0000000000FF0000) << 24) |
        ((value & 0x00000000FF000000) << 8)  |
        ((value & 0x000000FF00000000) >> 8) |
        ((value & 0x0000FF0000000000) >> 24) |
        ((value & 0x00FF000000000000) >> 40) |
        ((value & 0xff00000000000000) >> 56)
        );
}

uint16 copy_bytes_uint16(uint16 from, uint16 to)
{ // ISSUES:IMPLEMENT
    to |= (from & MASK_WORD1);
    to |= (from & MASK_WORD2) << 8;
    return to;
}
uint32 copy_bytes_uint32(uint32 from, uint32 to)
{ // ISSUES:IMPLEMENT
    to |= (from & MASK_WORD4);
    to |= (from & MASK_WORD3) | (to & MASK_WORD3) << 8;
    to |= (from & MASK_WORD2) | (to & MASK_WORD2) << 16;
    to |= (from & MASK_WORD1) | (to & MASK_WORD1) << 24;
    return to;
}
uint64 copy_bytes_uint64(uint64 from, uint64 to)
{ // ISSUES:IMPLEMENT
    to |= (from & MASK_WORD4);
    to |= (from & MASK_WORD3) | (to & MASK_WORD3) << 8;
    to |= (from & MASK_WORD2) | (to & MASK_WORD2) << 16;
    to |= (from & MASK_WORD1) | (to & MASK_WORD1) << 24;
    return to;
}


uint64 make_uint64_from_string_max_8chars(const char string[8], const uint8 num_chars) {
    uint64 result = 0;
    // if(num_chars <= 2) {
    //     result |= ((uint64)(uint8)string[0]) << 0;
    //     result |= ((uint64)(uint8)string[1]) << 8;
    // }
    // if(num_chars <= 2) result = copy_bytes_uint16(result, *(uint16*)string);

    uint16  mask = 0;
    uint64 raw64 = *(const uint64_t*)string;
    for (int i = 0; i < num_chars; i++) {
        result |= ((raw64 >> mask) & 0xFF) << mask;
        mask += BYTE_SIZE;
    }
    return result;
}

/**************************************************************************************************/

/*
	The only reason using this function is that it does not need a buffer,
	it returns an integer representation of the string number

	Max number of string digits: 7 (8 bytes in uint64 -1byte for the '\0'.)
*/
// uint64 NumberToStringUpTo7Digits(uint64 number) // uint16 Max Value = 65,535
// {
// 	if (number == 0)
// 		return 0;
//
// 	struct Digits_Base db = CalcDigits_Base(number);
//
// 	uint64 num = number;
// 	char digitsbuff[32] = { '\0' };
// 	uint32 i = 0;
//
// 	// Divide the number from base. Each iteration reduse the base by 1
// 	while (db.digits != 0)
// 	{
// 		digitsbuff[i] = (number / db.base) + ASCII_0;
// 		number %= db.base;
// 		db.base /= 10;
// 		db.digits--;
//
// 		i++;
// 	}
//
// 	digitsbuff[i] = '\0'; // Add the end of string
//
// 	return GET_UINT64_FROM_STR(digitsbuff);
// }
//
// // Max number of digits: 15 (16 bytes in uint64 -1byte for the '\0.)
// UINT128 NumberToStringUint128(uint64 number)
// {
// 	uint64 result = 0;
//
// 	// Calculate the number of digits of the "number" var
// 	uint64 numdigits = CalcNumDigitsInNumber(number);
// 	// Get the decimal base in which we will start to devide the "number"
// 	uint64 base = GetBaseOfNumDigits(numdigits);
//
// 	uint64 num = number;
// 	char digitsbuff[32] = {'\0'};
// 	uint32 i = 0;
// 	// Divide the number from base. Each iteration reduse the base by 1
// 	while (numdigits != 0)
// 	{
// 		digitsbuff[i] = (num / base) + ASCII_0;
// 		result += digitsbuff[i];
// 		num %= base; //
// 		base /= 10;
// 		numdigits--;
//
// 		i++;
// 	}
//
// 	digitsbuff[i] = '\0'; // Add the end of string
//
// 	UINT128 res = CreateUINT128(digitsbuff);
// 	return res;
// }
