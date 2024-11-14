typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
#ifndef __MWERKS__
typedef uint32_t uint32;
typedef int32_t int32;
#else
typedef unsigned int uint32;
typedef int int32;
#endif
typedef uintptr_t uintptr;
typedef intptr_t intptr;
typedef uint64_t uint64;
typedef int64_t int64;
// hardcode ucs-2
typedef uint16_t wchar;
#define ARRAY_SIZE(array)                (sizeof(array) / sizeof(array[0]))