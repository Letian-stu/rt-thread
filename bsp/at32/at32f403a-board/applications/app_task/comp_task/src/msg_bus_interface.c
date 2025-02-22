#include "msg_bus_interface.h"
#include "task_msg_bus.h"

#ifdef LOG_TAG 
#define LOG_TAG "msg_bus_interface"
#endif
#define LOG_LVL     LOG_LVL_INFO   

#include <ulog.h>  

enum task_msg_name g_msg_bus_event_id[] = {
    MSG_BUS_EVENT_RUN1S, 
    MSG_BUS_EVENT_TASKSUSPED, 
    MSG_BUS_EVENT_DATA_TRANS
};


static void TMsgCb(task_msg_args_t args)
{
    LOG_D("[TMsgCb]:msgidx:%d len:%d ", args->msg_name, args->msg_size, args->msg_obj);
    if(args->msg_size > 0)
    {
        LOG_HEX("[TMsgCb]:data", 16, args->msg_obj, args->msg_size);
    }
}

void TMsgBusCreate(void)
{
    for(int i = 0; i < TASK_MSG_COUNT; i++)
    {
        task_msg_subscribe((enum task_msg_name)i, TMsgCb);
    }
}
