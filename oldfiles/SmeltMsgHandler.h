#ifndef __LYNX_GAME_SERVER_SMELT_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_SMELT_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct SmeltMsgHandler
	{
        static void onSmeltReq(const ConnId& connId, SmeltReq& msg);
        static void onRebornHeroReq(const ConnId& connId, RebornHeroReq& msg);
        static void onRebornEquipReq(const ConnId& connId, RebornEquipReq& msg);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_SMELT_MSG_HANDLER_H__

