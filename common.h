
/*
 * common.h --
 *
 * Common tools.
 */

#ifndef __COMMON_H
#define __COMMON_H

char roll_dice();

#ifndef DEBUG
#   define DEBUG 0
#endif

#ifndef TRACE
#   define TRACE 0
#endif

#if DEBUG
#  define DEBUG_MSG(x) x
# else
#  define DEBUG_MSG(x)
#endif

#if TRACE
#  define TRACE_MSG(x) x
# else
#  define TRACE_MSG(x)
#endif

#endif
