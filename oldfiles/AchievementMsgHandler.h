#ifndef __LYNX_GAME_SERVER_ACHIEVEMENT_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_ACHIEVEMENT_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct AchievementMsgHandler
    {
        static void onGetAchievementAwardReq(const ConnId& connId, GetAchievementAwardReq& msg);
    };

} // namespace Lynx


#endif // __LYNX_GAME_SERVER_ACHIEVEMENT_MSG_HANDLER_H__

