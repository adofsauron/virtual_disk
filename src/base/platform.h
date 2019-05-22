
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <limits.h>

#include <cstdlib>

typedef signed char			int8;
typedef signed short		int16;
typedef signed int			int32;
typedef long long			int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

typedef float				float32;
typedef double				float64;

typedef unsigned int		uint;
typedef uint8				byte;


#ifdef __WIN32__

#pragma warning(disable:4819)
#pragma warning(disable:4244)
#pragma warning(disable:4103) 

#define snprintf _snprintf
#define access _access

#endif//__WIN32__



#endif//__PLATFORM_H__
