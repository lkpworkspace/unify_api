/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define UNIFY_API_OS_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__)
    #define UNIFY_API_OS_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #include <TargetConditionals.h>
    // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH.
    // see TargetConditionals.h for more details.
    #if TARGET_OS_IPHONE
        #define UNIFY_API_OS_IOS
    #elif TARGET_OS_MAC
        #define UNIFY_API_OS_MAC
    #endif
#elif defined(ANDROID) || defined(__ANDROID__)
    #define UNIFY_API_OS_ANDROID
#else
#error Platform not supported by unify_api.
#endif
