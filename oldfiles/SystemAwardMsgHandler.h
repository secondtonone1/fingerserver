#ifndef __LYNX_GAME_SERVER_SYSTEM_AWARD_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_SYSTEM_AWARD_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct SystemAwardMsgHandler
    {
        static void onGetAccumulateLoginAwardReq(const ConnId& connId, GetAccumulateLoginAwardReq& msg);
        static void onGetContinousLoginAwardReq(const ConnId& connId, GetContinousLoginAwardReq& msg);
        static void onGetLevelAwardReq(const ConnId& connId, GetLevelAwardReq& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_SYSTEM_AWARD_MSG_HANDLER_H__