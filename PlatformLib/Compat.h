#ifndef __LYNX_PLATFORM_LIB_COMMON_COMPAT_H__
#define __LYNX_PLATFORM_LIB_COMMON_COMPAT_H__

#ifdef __linux__
#include <stdint.h>
#endif 

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cassert>
#include <stdarg.h>
#include <time.h>
#include <climits>
#include <math.h>
#include <algorithm>
#include <limits>

#if defined (_WIN32)
//#ifdef _LYNX_PLATFORM_EXPORTS
//#  define _PlatformExport __declspec(dllexport)
//#else
//#  define _PlatformExport __declspec(dllimport)
//#endif
#define _PlatformExport
#else
#define _PlatformExport
#endif

namespace Lynx
{

#if defined (_WIN32)
#pragma warning (disable:4099)
#pragma warning (disable:4348)
#pragma warning (disable:4267)
#pragma warning (disable:4800)
#pragma warning (disable:4018)
#pragma warning (disable:4267)
#pragma warning (disable:4244)
#pragma warning (disable:6281)
#pragma warning (disable:4005)

#define strcasecmp                         _stricmp
#define strncasecmp                        _strnicmp
#define ERRNUM()                           WSAGetLastError()
#define EWOULDBLOCK_                       WSAEWOULDBLOCK
#define ECONNRESET_                        WSAECONNRESET

//#define strncpy(_Dst, _Src, _SizeInBytes) strncpy_s(_Dst, _SizeInBytes, _Src, _SizeInBytes - 1)
#define snprintf(_DstBuf, _SizeInBytes, ...)  _snprintf_s(_DstBuf, _SizeInBytes, _SizeInBytes - 1, __VA_ARGS__)



typedef char                               Int8;
typedef short                              Int16;
typedef int                                Int32;
typedef long long                          Int64;

typedef unsigned char                      UInt8;
typedef unsigned short                     UInt16;
typedef unsigned int                       UInt32;
typedef unsigned long long                 UInt64;

#elif defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
#include <stdint.h>
#include <errno.h>

#if defined(__APPLE__)
#   include <MacTypes.h>
#endif

typedef char                               Int8;
typedef short                              Int16;
typedef int                                Int32;
// typedef int64_t                            Int64;
typedef long long int Int64;


typedef unsigned char                      UInt8;
typedef unsigned short                     UInt16;
#if !defined(__APPLE__)
typedef unsigned int                       UInt32;
#endif
// typedef uint64_t                           UInt64;
typedef unsigned long long int			   UInt64;

#define closesocket(fd)                    ::close(fd)
#define ERRNUM()                           errno
#define EWOULDBLOCK_                       EWOULDBLOCK
#define ECONNRESET_                        ECONNRESET


#endif // #if defined (_WIN32)
}


#define TYPE_CONV(type, data) (*(type*)&data)
#define FLOAT_EQUAL(a, b) (fabs(a - b) <= 0.000001f)
#define KMAX(num1, num2) (num1 > num2 ? num1 : num2)
#define KMIN(num1, num2) (num1 < num2 ? num1 : num2)

//设置浮点数运算精度,参数可选_PC_24, _PC_53, _PC_64, 
//此宏可单独使用
#define SET_FLOAT_PERCISION_TO(PER)                     \
    unsigned int control_word, precision_control;       \
    do                                                  \
    {                                                   \
        _controlfp_s(&control_word, 0, 0);              \
        precision_control = control_word & MCW_PC;      \
        if (precision_control != PER)                   \
        {                                               \
            _controlfp_s(0, PER, MCW_PC);               \
        }                                               \
    } while (0);                                        \

//将浮点数运算精度设置回来,参数必须和设置浮点数的宏参数保持一致, 
//此宏必须配合设置浮点数精度宏使用,不可单独使用
#define SETBACK_FLOAT_PERCISION_FROM(PER)               \
    do                                                  \
    {                                                   \
        if (precision_control != PER)                   \
        {                                               \
            _controlfp_s(0, precision_control, MCW_PC); \
        }                                               \
    } while (0);                                        \
    

#endif // __LYNX_PLATFORM_LIB_COMMON_COMPAT_H__


