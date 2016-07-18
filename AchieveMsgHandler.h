#ifndef __LYNX_GAME_SERVER_ACHIEVE_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_ACHIEVE_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct AchieveMsgHandler
	{
		static void onAchieveFinishReq(const ConnId&connId, CGAchieveFinishReq &msg);
		static void onDailyTaskAddReq(const ConnId&connId, CGDailyTaskAddReq &msg);
		static void onDailyTaskFinishReq(const ConnId&connId, CGDailyTaskFinishReq &msg);
		static void onActiveAwardGetReq(const ConnId& connId, CGActiveAwardGetReq &msg);
	};
} 
#endif// namespace Lynx__LYNX_GAME_SERVER_ACHIEVE_MSG_HANDLER_H__