#ifndef __LYNX_GAME_SERVER_JEWELRY_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_JEWELRY_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct JewelryMsgHandler
	{
		static void onJewelryPolish(const ConnId& connId, CGJewelryPolishReq& msg);
		static void onJewelryReset(const ConnId& connId, CGJewelryResetReq & msg);
		static void onJewelrySet(const ConnId& connId, CGJewelrySetReq & msg);
		static void onJewelryUnload(const ConnId & connId, CGJewelryUnloadReq& msg);
		static void onJewelrySell(const ConnId & connId, CGJewelrySellReq& msg);
		static void onJewelryUpdate(const ConnId & connId, CGJewelryUpdateReq &msg);
		static void onJewelrySetOnce(const ConnId & connId, CGJewelrySetOnceReq &msg);
		
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_JEWELRY_MSG_HANDLER_H__