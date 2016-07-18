#ifndef __LYNX_GAME_SERVER_TOWER_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_TOWER_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct TowerMsgHandler
    {
        static void onOpenTowerBufferReq(const ConnId& connId, OpenTowerBufferReq& msg);
        static void onSelectBufferReq(const ConnId& connId, SelectBufferReq& msg);
        static void onStartTowerBattleReq(const ConnId& connId, StartTowerBattleReq& msg);
        static void onTowerBattleResultReq(const ConnId& connId, TowerBattleResultReq& msg);
        static void onResetTowerReq(const ConnId& connId, ResetTowerReq& msg);
        static void onSweepTowerReq(const ConnId& connId, SweepTowerReq& msg);
        static void onOpenTowerRankReq(const ConnId& connId, OpenTowerRankReq& msg);
    };

} // namespace Lynx


#endif // __LYNX_GAME_SERVER_TOWER_MSG_HANDLER_H__

