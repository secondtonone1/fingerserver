#ifndef __LYNX_GAME_SERVER_RANK_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_RANK_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct RankMsgHandler
    {

        static void onGetRankReq(const ConnId& connId, GetRankReq& msg);
        static void onGetRankTopReq(const ConnId& connId, GetRankTopReq& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RANK_MSG_HANDLER_H__

