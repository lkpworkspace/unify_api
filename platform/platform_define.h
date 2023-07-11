/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once

#include "unify_api_platform.h"
#if defined(UNIFY_API_OS_WINDOWS)
#include "win/platform_define-win.h"
#else
#include "linux/platform_define-linux.h"
#endif
