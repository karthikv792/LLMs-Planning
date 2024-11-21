#ifndef __VALLIB_EXPORT
#define __VALLIB_EXPORT

#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_BUILD
#ifdef VAL_EXPORTS
#define VAL_EXPORT __declspec(dllexport)
#else
#define VAL_EXPORT __declspec(dllimport)
#endif /* VAL_EXPORTS */
#else
#define VAL_EXPORT
#endif
#else
typedef const char *LPCSTR;
#define VAL_EXPORT
#endif

#endif