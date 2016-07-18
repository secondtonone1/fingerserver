#ifndef __LYNX_GAME_SERVER_ACTIVITY_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_ACTIVITY_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct ActivityMsgHandler
    {
        static void onActivityRuneBattleReq(const ConnId& connId, ActivityRuneBattleReq& msg);
        static void onActivityRuneResultReq(const ConnId& connId, ActivityRuneResultReq& msg);

        static void onActivityGoldBattleReq(const ConnId& connId, ActivityGoldBattleReq& msg);
        static void onActivityGoldResultReq(const ConnId& connId, ActivityGoldResultReq& msg);

        static void onActivityPetBattleReq(const ConnId& connId, ActivityPetBattleReq& msg);
        static void onActivityPetResultReq(const ConnId& connId, ActivityPetResultReq& msg);

        static void onActivityEnchantBattleReq(const ConnId& connId, ActivityEnchantBattleReq& msg);
        static void onActivityEnchantResultReq(const ConnId& connId, ActivityEnchantResultReq& msg);

        static void onActivityRuneNumberReq(const ConnId& connId, ActivityRuneNumberReq& msg);

        static void onActivityRefreshSettingReq(const ConnId& connId, ActivityRefreshSettingReq& msg);
    };

} // namespace Lynx


#endif // __LYNX_GAME_SERVER_ACTIVITY_MSG_HANDLER_H__

