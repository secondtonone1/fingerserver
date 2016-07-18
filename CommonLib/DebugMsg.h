#ifndef __LYNX_COMMON_LIB_DEBUG_MSG_H__
#define __LYNX_COMMON_LIB_DEBUG_MSG_H__

#include "Data.h"

namespace Lynx
{
#ifdef DEBUG
    typedef CharArray<100> DebugLogString;
    enum
    {
        DEBUG_REMOTE_LOG_NOTIFY                    = LYNX_MAX_MSG_ID - 1,

    };

    struct DebugRemoteLogNotify 
    {
        DebugRemoteLogNotify(){}

        DebugLogString mLogString;

        LYNX_MESSAGE_1(DEBUG_REMOTE_LOG_NOTIFY, DebugRemoteLogNotify, DebugLogString, mLogString);
    };

#endif
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_DEBUG_MSG_H__