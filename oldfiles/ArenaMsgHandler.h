#ifndef __LYNX_GAME_SERVER_ARENA_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_ARENA_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct ArenaMsgHandler
    {
        static void onOpenArenaReq(const ConnId& connId, OpenArenaReq& msg);
        static void onStartArenaBattleReq(const ConnId& connId, StartArenaBattleReq& msg);
        static void onRefreshArenaOppReq(const ConnId& connId, RefreshArenaOppReq& msg);
        static void onOpenArenaRankReq(const ConnId& connId, OpenArenaRankReq& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_ARENA_MSG_HANDLER_H__