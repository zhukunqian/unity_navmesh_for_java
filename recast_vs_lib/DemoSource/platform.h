#pragma once

// windows include	
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
	#pragma warning(disable:4996)
	#pragma warning(disable:4819)
	#pragma warning(disable:4049)
	#pragma warning(disable:4217)
	#include <io.h>
	#include <time.h> 
	//#define FD_SETSIZE 1024
	#ifndef WIN32_LEAN_AND_MEAN 
		#include <winsock2.h>		// 必须在windows.h之前包含， 否则网络模块编译会出错
		#include <mswsock.h> 
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h> 
	#include <unordered_map>
	#include <functional>
	#include <memory>
	#define _SCL_SECURE_NO_WARNINGS
#else
	// linux include
	#include <errno.h>
	#include <float.h>
	#include <pthread.h>	
	#include <fcntl.h>
	#include <unistd.h>
	#include <signal.h>
	#include <netdb.h>
	#include <iconv.h>
	#include <langinfo.h>   /* CODESET */
	#include <stdint.h>
	#include <signal.h>
	#include <net/if.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h> 
	#include <netinet/ip.h>
	#include <arpa/inet.h>
	#include <tr1/unordered_map>
	#include <tr1/functional>
	#include <tr1/memory>
	#include <linux/types.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/ioctl.h>
	#include <sys/socket.h>
	#include <sys/uio.h>
	#include <sys/resource.h> 
	#include <linux/errqueue.h>
#endif

// current platform and compiler
#define PLATFORM_WIN32			0
#define PLATFORM_UNIX			1
#define PLATFORM_APPLE			2

#define UNIX_FLAVOUR_LINUX		1
#define UNIX_FLAVOUR_BSD		2
#define UNIX_FLAVOUR_OTHER		3
#define UNIX_FLAVOUR_OSX		4

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define KBE_PLATFORM PLATFORM_WIN32
#elif defined( __INTEL_COMPILER )
#  define KBE_PLATFORM PLATFORM_INTEL
#elif defined( __APPLE_CC__ )
#  define KBE_PLATFORM PLATFORM_APPLE
#else
#  define KBE_PLATFORM PLATFORM_UNIX
#endif

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU	   1
#define COMPILER_BORLAND   2
#define COMPILER_INTEL     3
#define COMPILER_CLANG     4

#ifdef _MSC_VER
#  define KBE_COMPILER COMPILER_MICROSOFT
#elif defined( __INTEL_COMPILER )
#  define KBE_COMPILER COMPILER_INTEL
#elif defined( __BORLANDC__ )
#  define KBE_COMPILER COMPILER_BORLAND
#elif defined( __GNUC__ )
#  define KBE_COMPILER COMPILER_GNU
#elif defined( __clang__ )
#  define KBE_COMPILER COMPILER_CLANG

#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif

#if KBE_PLATFORM == PLATFORM_UNIX || KBE_PLATFORM == PLATFORM_APPLE
#ifdef HAVE_DARWIN
#define KBE_PLATFORM_TEXT "MacOSX"
#define UNIX_FLAVOUR UNIX_FLAVOUR_OSX
#else
#ifdef USE_KQUEUE
#define KBE_PLATFORM_TEXT "FreeBSD"
#define UNIX_FLAVOUR UNIX_FLAVOUR_BSD
#else
#ifdef USE_KQUEUE_DFLY
#define KBE_PLATFORM_TEXT "DragonFlyBSD"
#define UNIX_FLAVOUR UNIX_FLAVOUR_BSD
#else
#define KBE_PLATFORM_TEXT "Linux"
#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX
#endif
#endif
#endif
#endif

#if KBE_PLATFORM == PLATFORM_WIN32
#define KBE_PLATFORM_TEXT "Win32"
#endif

#ifndef KBE_CONFIG
#ifdef _DEBUG
#define KBE_CONFIG "Debug"
#else
#define KBE_CONFIG "Release"
#endif
#endif

#ifndef X64
#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( __amd64 ) || defined( __LP64__ )
#define X64
#endif
#endif

#ifdef X64
#define KBE_ARCH "X64"
#else
#define KBE_ARCH "X86"
#endif

/*---------------------------------------------------------------------------------
类型定义
---------------------------------------------------------------------------------*/
#ifndef TCHAR
#ifdef _UNICODE
typedef wchar_t												TCHAR;
#else
typedef char												TCHAR;
#endif
#endif

typedef unsigned char											uchar;
typedef unsigned short											ushort;
typedef unsigned int											uint;
typedef unsigned long											ulong;

#define charptr													char*
#define const_charptr											const char*
#define PyObject_ptr											PyObject*

#define KBEShared_ptr											std::tr1::shared_ptr
#define KBEUnordered_map										std::tr1::unordered_map

 

/* Use correct types for x64 platforms, too */
#if KBE_COMPILER != COMPILER_GNU
typedef signed __int64											int64;
typedef signed __int32											int32;
typedef signed __int16											int16;
typedef signed __int8											int8;
typedef unsigned __int64										uint64;
typedef unsigned __int32										uint32;
typedef unsigned __int16										uint16;
typedef unsigned __int8											uint8;
 
#define PRI64													"lld"
#define PRIu64													"llu"
#define PRIx64													"llx"
#define PRIX64													"llX"
#define PRIzu													"lu"
#define PRIzd													"ld"
#define PRTime													PRI64
#else
typedef int64_t													int64;
typedef int32_t													int32;
typedef int16_t													int16;
typedef int8_t													int8;
typedef uint64_t												uint64;
typedef uint32_t												uint32;
typedef uint16_t												uint16;
typedef uint8_t													uint8;
typedef uint16_t												WORD;
typedef uint32_t												DWORD;

#ifdef _LP64
typedef int64													intptr;
typedef uint64													uintptr;

#ifndef PRI64
#define PRI64													"ld"
#endif

#ifndef PRIu64
#define PRIu64													"lu"
#endif

#ifndef PRIx64
#define PRIx64													"lx"
#endif

#ifndef PRIX64
#define PRIX64													"lX"
#endif

#ifndef PRTime
#define PRTime													PRI64
#endif

#else
typedef int32													intptr;
typedef uint32													uintptr;

#ifndef PRI64
#define PRI64													"lld"
#endif

#ifndef PRIu64
#define PRIu64													"llu"
#endif

#ifndef PRIx64
#define PRIx64													"llx"
#endif

#ifndef PRIX64
#define PRIX64													"llX"
#endif

#ifndef PRTime
#define PRTime													"ld"
#endif

#endif

#ifndef PRIzd
#define PRIzd													"zd"
#endif

#ifndef PRIzu
#define PRIzu													"zu"
#endif

#endif