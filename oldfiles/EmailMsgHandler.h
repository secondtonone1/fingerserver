#ifndef __LYNX_GAME_SERVER_EMAIL_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_EMAIL_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct EmailMsgHandler
    {
        static void onDelEmailReq(const ConnId& connId, DelEmailReq& msg);
        static void onTakeEmailItemReq(const ConnId& connId, TakeEmailItemReq& msg);
        static void onOpenEmailNotify(const ConnId& connId, OpenEmailNotify& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_EMAIL_MSG_HANDLER_H__

