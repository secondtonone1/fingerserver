#ifndef __LYNX_GAME_SERVER_DEBUG_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_DEBUG_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
#ifdef DEBUG
    struct DebugMsgHandler
    {
        static void onDebugRemoteLogNotify(const ConnId& connId, DebugRemoteLogNotify& msg);
    };
#endif

} // namespace Lynx


#endif // __LYNX_GAME_SERVER_DEBUG_MSG_HANDLER_H__