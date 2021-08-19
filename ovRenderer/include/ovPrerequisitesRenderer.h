#pragma once

#include <ovPrerequisitesUtil.h>

/*****************************************************************************/
/**
* Library export specifics
*/
/*****************************************************************************/
#if OV_PLATFORM == OV_PLATFORM_WIN32
# if OV_COMPILER == OV_COMPILER_MSVC
#   if defined( OV_STATIC_LIB )
#     define OV_CORE_EXPORT
#   else
#     if defined ( OV_RENDERER_EXPORTS )
#       define OV_RENDERER_EXPORT __declspec( dllexport )
#     else
#       define OV_RENDERER_EXPORT __declspec( dllimport )
#     endif
#   endif
# else
#   if defined( OV_STATIC_LIB )
#     define OV_RENDERER_EXPORT
#   else
#     if defined ( OV_RENDERER_EXPORTS )
#       define OV_RENDERER_EXPORT __attribute__ ((dllexport))
#     else
#       define OV_RENDERER_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define OV_RENDERER_HIDDEN
#else
# define OV_RENDERER_EXPORT __attribute__ ((visibility ("default")))
# define OV_RENDERER_HIDDEN __attribute__ ((visibility ("hidden")))
#endif