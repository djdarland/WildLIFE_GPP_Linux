#pragma once
/*! \file defs.h
  \brief includes

*/
#ifdef __unix__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <pwd.h>


#ifdef X11
#include <X11/Xlib.h>
#ifdef NEEDXLIBINT
#include <X11/Xlibint.h>
#endif
#include <X11/Xutil.h>
#include "life_icon"
#endif

#endif


#ifdef _WIN64
#define _CRT_SECURE_NO_WARNINGS
#include <direct.h>
#include <cstdlib>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <ctime>
#include <sys/types.h>
#endif


#include "def_config.h"
#include "def_const.h"
#include "def_macro.h"
#include "def_struct.h"
#include "wl_memory.h"
#include "def_glob.h"
#include "def_proto.h"





