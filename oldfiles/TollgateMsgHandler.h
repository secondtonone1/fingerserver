#ifndef __LYNX_GAME_SERVER_TOLLGATE_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_TOLLGATE_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct TollgateMsgHandler
    {
        static void onStartTollgateBattleReq(const ConnId&, StartTollgateBattleReq&);
        static void onStartFiveTollgateSweepReq(const ConnId&, StartFiveTollgateSweepReq&);
        static void onTollgateBattleResultReq(const ConnId&, TollgateBattleResultReq&);
        static void onAgainTollgateBattleReq(const ConnId&, AgainTollgateBattleReq&);
        static void onGetScenePassAwardReq(const ConnId&, GetScenePassAwardReq&);

        static void onStartStoreHouseBattleReq(const ConnId&, StartStoreHouseBattleReq&);
        static void onStoreHouseBattleResultReq(const ConnId&, StoreHouseBattleResultReq&);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_TOLLGATE_MSG_HANDLER_H__
