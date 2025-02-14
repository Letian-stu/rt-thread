#ifndef _BLIB_H_
#define _BLIB_H_

#include <rtthread.h>

#define GET_TICK()                  rt_tick_get_millisecond()
#define RUN_IF_ELAPSED(param, ms)   if (GET_TICK() - (self->param) >= (ms))
#define SYNC_TIME(param)            (self->param) = GET_TICK()

#endif
