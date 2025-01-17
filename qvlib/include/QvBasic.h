#ifndef  _QV_BASIC_
#define  _QV_BASIC_

#include <sys/types.h>

// BSD types; needed for VC 6.0

#ifdef _WIN32
typedef struct { int r[1]; } *  physadr;
typedef unsigned char   unchar;
typedef unsigned char   u_char;
typedef unsigned short  ushort;
typedef unsigned short  u_short;
typedef unsigned int    uint;
typedef unsigned int    u_int;
typedef unsigned long   ulong;
typedef unsigned long   u_long;
typedef struct  _quad { long val[2]; } quad;
#endif

// #include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
// #include <ulimit.h>
#include <math.h>
//#include <bstring.h>


#ifndef M_PI_4
#define M_PI_4 0.78539816339744830962
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif


#include <stdio.h>

#ifndef FALSE
#   define FALSE	0
#   define TRUE		1
#endif

typedef int QvBool;

// This uses the preprocessor to quote a string
// MSVC does not define __ANSI_CPP__ but follows the same convention
#if defined(__STDC__) || defined(__ANSI_CPP__)	|| defined(_WIN32)	/* ANSI C */
#  define QV__QUOTE(str)	#str
#else							/* Non-ANSI C */
#  define QV__QUOTE(str)	"str"
#endif

// This uses the preprocessor to concatenate two strings
#if defined(__STDC__) || defined(__ANSI_CPP__)	|| defined(_WIN32)	/* ANSI C */
#   define QV__CONCAT(str1, str2)	str1##str2
#else							/* Non-ANSI C */
#   define QV__CONCAT(str1, str2)	str1/**/str2
#endif

#endif /* _QV_BASIC_ */
