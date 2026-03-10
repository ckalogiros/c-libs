/*
 * Created by ckalo on 1/6/26.
 */

#include "sys_utils.h"
#include "logger_color.h"
#include <stdio.h>
#include "file_sys_interface.h"

void
my_exit(const char* msg, const char* file, const uint32 line)
{
    printf("%s####################################################\n"
           "%s%s\nEXIT AT %s\nline:%u\n", RED, msg, CLEAR, file, line);
    printf("%s####################################################%s\n", RED, CLEAR);

    const char* filename = "/crash_debug.txt";
    struct fs_t* fs = fs_create(filename);
    fs_fprint_all_allocated_mem(filename);
    fs_append(fs, "CRASH EXIT\nmsg:%s", msg);

    exit(1);
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

/***************************************************************************************************
 * SOME INTERNET CODE, NEEDS TO BE IMPLEMENTED
/**************************************************************************************************/
bool is_whitespace(char c) { return c == ' ' || c == '\n'; }

// If c is a hex digit returns its numeric value, otherwise -1.
int32 to_hex_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
/***************************************************************************************************
 * OLD CODE, NEEDS TO BE IMPLEMENTED
/**************************************************************************************************/

uint32 ReverseBits32(uint32 value)
{
    // 31 32 33 34
    //  - -- -- - 6 hex = 4*6 = 24
    //     - -    2 hex = 4*2 = 8
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) << 8)  |
            ((value & 0x00FF0000) >> 8)  |
            ((value & 0xFF000000) >> 24));
}
uint64 ReverseBits64(uint64 value)
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

uint16 CopyBytesUint16(uint16 from, uint16 to)
{
    to |= (from & MASK_WORD4);
    to |= (from & MASK_WORD3) | (to & MASK_WORD3) << 8;
    to |= (from & MASK_WORD2) | (to & MASK_WORD2) << 16;
    to |= (from & MASK_WORD1) | (to & MASK_WORD1) << 24;
    return to;
}
uint32 CopyBytesUint32(uint32 from, uint32 to)
{
    to |= (from & MASK_WORD4);
    to |= (from & MASK_WORD3) | (to & MASK_WORD3) << 8;
    to |= (from & MASK_WORD2) | (to & MASK_WORD2) << 16;
    to |= (from & MASK_WORD1) | (to & MASK_WORD1) << 24;
    return to;
}
uint64 CopyBytesUint64(uint64 from, uint64 to)
{
    to |= (from & MASK_WORD4);
    to |= (from & MASK_WORD3) | (to & MASK_WORD3) << 8;
    to |= (from & MASK_WORD2) | (to & MASK_WORD2) << 16;
    to |= (from & MASK_WORD1) | (to & MASK_WORD1) << 24;
    return to;
}

/**************************************************************************************************/


static uint64 CalcNumDigitsInNumber(uint64 number)
{
	// Case the number is 0
	if (number == 0) return 1;

	uint64 numdigits = 0;

	// Find how many digits are in val
	while (number)
	{
		number /= 10;
		numdigits++;
	}

	return numdigits;
}


static uint64 GetBaseOfNumDigits(uint8 val)
{
	if (val == 1) return 1;
	else if (val == 2)  return 10;
	else if (val == 3)  return 100U;
	else if (val == 4)  return 1000U;
	else if (val == 5)  return 10000U;
	else if (val == 6)  return 100000U;
	else if (val == 7)  return 1000000U;
	else if (val == 8)  return 10000000U;
	else if (val == 9)  return 100000000U;
	else if (val == 10) return 1000000000U;
	else if (val == 11) return 10000000000U;
	else if (val == 12) return 100000000000U;
	else if (val == 13) return 1000000000000U;
	else if (val == 14) return 10000000000000U;
	else if (val == 15) return 100000000000000U;
	else if (val == 16) return 1000000000000000U;
	else if (val == 17) return 10000000000000000U;
	else if (val == 18) return 100000000000000000U;
	else if (val == 19) return 1000000000000000000U;
	else if (val == 20) return 10000000000000000000U;
	else {
		return 0; // Just for the warning..
	}
	// Timing: -{  100.000000 nanosec |  0.100000 microsec |  0.000100 ms |  0.000000 sec }-

}


struct Digits_Base
{
	uint8 digits;
	uint64 base;
};

// static
// struct Digits_Base CalcDigits_Base(const uint64 number)
// {
// 	if (number == 0)
// 	{
// 		printf("");
// 		ASSERT(TRUE);
// 	}
//
// 	struct Digits_Base db = {.base=0, .digits=0};
// 	if (number < 10)						{db.digits = 1;	db.base = 1;						return db; }
// 	if (number < 100)						{db.digits = 2;	db.base = 10U;						return db; }
// 	if (number < 1000)					{db.digits = 3;	db.base = 100U;					return db; }
// 	if (number < 10000)					{db.digits = 4;	db.base = 1000U;					return db; }
// 	if (number < 100000)					{db.digits = 5;	db.base = 10000U;					return db; }
// 	if (number < 1000000)				{db.digits = 6;	db.base = 100000U;				return db; }
// 	if (number < 10000000)				{db.digits = 7;	db.base = 1000000U;				return db; }
// 	if (number < 100000000)				{db.digits = 9;	db.base = 10000000U;				return db; }
// 	if (number < 1000000000)			{db.digits = 9;	db.base = 100000000U;			return db; }
// 	if (number < 10000000000)			{db.digits = 10; 	db.base = 1000000000U;			return db; }
// 	if (number < 100000000000)			{db.digits = 11; 	db.base = 10000000000U;			return db; }
// 	if (number < 1000000000000)		{db.digits = 12; 	db.base = 100000000000U;		return db; }
// 	if (number < 10000000000000)		{db.digits = 13; 	db.base = 1000000000000U;		return db; }
// 	if (number < 100000000000000)		{db.digits = 14; 	db.base = 10000000000000U;		return db; }
// 	if (number < 1000000000000000)	{db.digits = 15; 	db.base = 100000000000000U;	return db; }
// 	if (number < 10000000000000000)	{db.digits = 16; 	db.base = 1000000000000000U;	return db; }
// 	if (number < 100000000000000000)	{db.digits = 17; 	db.base = 10000000000000000U;	return db; }
// 	if (number < 1000000000000000000){db.digits = 18; 	db.base = 100000000000000000U;return db; }
//
// 	LogError2(TRUE, __LINE__, __FILE__, "No Implementation For Such A High Number. Number Is:%llu, ", number);
// 	Terminate(__LINE__, __FILE__, "NUMBER IS TOO BIG.\n");
// 	return db; // So the compiler does not complain about control path
// }
//
// // Use this Number to String for numbers larger than 7 digits
// // Returns number of digits
// int16 NumberToString(int64 number, char* buffer, const int16 bufferlen)
// {
// 	if (number < 10 && number >= 0)
// 	{
// 		buffer[0] = '0' + number;
// 		buffer[1] = '\0';
// 		return 1;
// 	}
//
// 	if (number > 0x8FFFFF)
// 	{
// 		buffer[0] = 'B'; buffer[1] = 'I';buffer[2] = 'G'; buffer[3] = '\0';
// 		return 0;
// 	}
//
// 	uint32 i = 0;
// 	uint16 len = bufferlen;
//
// 	if (number < 0)
// 	{
// 		buffer[i++] = '-';
// 		number *= -1;
// 	}
// 	ASSERT(number);
// 	struct Digits_Base db = CalcDigits_Base(number);
//
// 	// Divide the number from base. Each iteration reduse the base by 1
// 	// Write until bufferlen -1, trancate the rest
// 	while (db.digits != 0 && len)
// 	{
// 		if (db.base <= 0)
// 			printf("");
// 		buffer[i] = (number / db.base) + ASCII_0;
// 		number %= db.base;
// 		db.base /= 10;
// 		db.digits--;
//
// 		i++;
// 		len--;
// 	}
//
// 	buffer[i] = '\0'; // Add the end of string
//
// 	// Print if trancation hapened
// 	if (i > bufferlen) LogError(TRUE, "Trancation of number: %llu\nNumber of trancated digits: %d\n", number, db.digits);
//
// 	return i;
// }

/*
	The only reason using this function is that it does not need a buffer,
	it returns an integer representation of the string number

	Max number of string digits: 7 (8 bytes in uint64 -1byte for the '\0.)
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
