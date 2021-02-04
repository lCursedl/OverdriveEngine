#pragma once
#define OV_PLATFORM_WIN32 1
#define OV_PLATFORM_LINUX 2

#define OV_COMPILER_MSVC  1
#define OV_COMPILER_GNUC  2
#define OV_COMPILER_INTEL 3
#define OV_COMPILER_CLANG 4

#define OV_ARCHITECTURE_x86_32 1
#define OV_ARCHITECTURE_x86_64 2

#define OV_ENDIAN_LITTLE 1
#define OV_ENDIAN_BIG    2

#define OV_VERSION_MAJOR 0
#define OV_VERSION_MINOR 1
#define OV_VERSION_PATCH 0

#define OV_ENDIAN OV_ENDIAN_LITTLE

#if defined(__clang__)
#   define OV_COMPILER OV_COMPILER_CLANG
#   define OV_COMPILER_VER __clang_version__
#   define OV_THREADLOCAL __thread
#   define OV_STDCALL __attribute__((stdcall))
#   define OV_CDECL __attribute__((cdecl))
#   define OV_FALLTHROUGH [[clang::fallthrough]];
#elif defined(__GNUC__)
#   define OV_COMPILER OV_COMPILER_GNUC
#   define OV_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
#   define OV_THREADLOCAL __thread
#   define OV_STDCALL __attribute__((stdcall))
#   define OV_CDECL __attribute__((cdecl))
#   define OV_FALLTHROUGH __attribute__((fallthrough));
#elif defined(__INTEL_COMPILER)
#   define OV_COMPILER OV_COMPILER_INTEL
#   define OV_COMP_VER __INTEL_COMPILER
#   define OV_STDCALL __stdcall
#   define OV_CDECL __cdecl
#   define OV_FALLTHROUGH
#elif defined(_MSC_VER)
#   define OV_COMPILER OV_COMPILER_MSVC
#   define OV_COMP_VER _MSC_VER
#   define OV_THREADLOCAL __declspec(thread)
#   define OV_STDCALL __stdcall
#   define OV_CDECL __cdecl
#   define OV_FALLTHROUGH
#   undef __PRETTY_FUNCTION__
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/*****************************************************************************/
/*
* See if we can use __forceinline or if we need to use __inline instead
*/
/*****************************************************************************/
#if OV_COMPILER == OV_COMPILER_MSVC
# if OV_COMP_VER >= 1200
#   define FORCEINLINE __forceinline
#   ifndef RESTRICT
#     define RESTRICT __restrict
#   endif
# endif
#elif defined(__MINGW32__)
#  if !defined(FORCEINLINE)
#    define FORCEINLINE __inline
#    ifndef RESTRICT
#      define RESTRICT
#    endif
# endif
#else
# define FORCEINLINE __inline
# ifndef RESTRICT
#    define RESTRICT __restrict
# endif
#endif // OV_COMPILER == OV_COMPILER_MSVC

/*****************************************************************************/
/**
* Finds the current platform
*/
/*****************************************************************************/
#if defined(__WIN32__) || defined(_WIN32)
# define OV_PLATFORM OV_PLATFORM_WIN32
#else
# define OV_PLATFORM OV_PLATFORM_LINUX
#endif

/*****************************************************************************/
/**
* Finds the architecture type
*/
/*****************************************************************************/
#if defined (__x86_64) || defined(_M_X64)
# define OV_ARCH_TYPE OV_ARCHITECTURE_X86_64
#else
# define OV_ARCH_TYPE OV_ARCHITECTURE_X86_32
#endif

/*****************************************************************************/
/**
* Memory Alignment macros
*/
/*****************************************************************************/
#if OV_COMPILER == OV_COMPILER_MSVC
# define MS_ALIGN(n) __declspec(align(n))
# ifndef GCC_PACK
#   define GCC_PACK(n)
# endif
# ifndef GCC_ALIGN
#   define GCC_ALIGN(n)
# endif
#elif ( OV_COMPILER == OV_COMPILER_GNUC )
# define MS_ALIGN(n)
# define GCC_PACK(n)
# define GCC_ALIGN(n) __attribute__( (__aligned__(n)) )
#else
# define MS_ALIGN(n)
# define GCC_PACK(n) __attribute__( (packed, aligned(n)) )
# define GCC_ALIGN(n) __attribute__( (__aligned__(n)) )
#endif

/*****************************************************************************/
/**
* Memory Alignment macros
*/
/*****************************************************************************/
#if OV_COMPILER == OV_COMPILER_MSVC ||                                        \
    OV_COMPILER == OV_COMPILER_INTEL ||                                       \
    OV_COMPILER == OV_COMPILER_GNUC
# define _NOEXCEPT noexcept
#else
# define _NOEXCEPT
#endif

/*****************************************************************************/
/**
* Library export specifics
*/
/*****************************************************************************/
#if OV_PLATFORM == OV_PLATFORM_WIN32
# if OV_COMPILER == OV_COMPILER_MSVC
#   if defined( OV_STATIC_LIB )
#     define OV_UTILITY_EXPORT
#   else
#     if defined ( OV_UTILITY_EXPORTS )
#       define OV_UTILITY_EXPORT __declspec( dllexport )
#     else
#       define OV_UTILITY_EXPORT __declspec( dllimport )
#     endif
#   endif
# else
#   if defined( OV_STATIC_LIB )
#     define OV_UTILITY_EXPORT
#   else
#     if defined ( OV_UTILITY_EXPORTS )
#       define OV_UTILITY_EXPORT __attribute__ ((dllexport))
#     else
#       define OV_UTILITY_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define OV_UTILITY_HIDDEN
#else
# define OV_UTILITY_EXPORT __attribute__ ((visibility ("default")))
# define OV_UTILITY_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

#if OV_PLATFORM == OV_PLATFORM_WIN32
# if OV_COMPILER == OV_COMPILER_MSVC
#   define OV_PLUGIN_EXPORT __declspec(dllexport)
# endif
#else
# define OV_PLUGIN_EXPORT __attribute__ ((visibility ("default")))
#endif

/*****************************************************************************/
/**
* Windows specific Settings
*/
/*****************************************************************************/
#if OV_PLATFORM == OV_PLATFORM_WIN32
# if defined(_DEBUG) || defined(DEBUG)
#   define OV_DEBUG_MODE 1
# else
#   define OV_DEBUG_MODE 0
# endif
# if OV_COMPILER == OV_COMPILER_INTEL
#   define OV_THREADLOCAL __declspec(thread)
# endif
#endif

/*****************************************************************************/
/**
* Linux/Apple specific Settings
*/
/*****************************************************************************/
#if OV_PLATFORM == OV_PLATFORM_LINUX
# define stricmp strcasecmp
# if defined(_DEBUG) || defined(DEBUG)
#   define OV_DEBUG_MODE 1
# else
#   define OV_DEBUG_MODE 0
# endif
# if OV_COMPILER == OV_COMPILER_INTEL
#   define OV_THREADLOCAL __thread
# endif
#endif

/*****************************************************************************/
/**
* Definition of Debug macros
*/
/*****************************************************************************/
#if OV_DEBUG_MODE
# define OV_DEBUG_ONLY(x) x
# define OV_ASSERT(x) asert(x)
#else
# define OV_DEBUG_ONLY(x)
# define OV_ASSERT
#endif