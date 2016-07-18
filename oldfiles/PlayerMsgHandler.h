#ifndef __LYNX_GAME_SERVER_PLAYER_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_PLAYER_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct PlayerMsgHandler
	{
        static void onUpgreadGoldMineralReq(const ConnId& connId, UpgreadGoldMineralReq&);
        static void onUpspeedGoldMineralReq(const ConnId& connId, UpspeedGoldMineralReq&);
        static void onUpgreadDiamondMineralReq(const ConnId& connId, UpgreadDiamondMineralReq&);
        static void onUpspeedDiamondMineralReq(const ConnId& connId, UpspeedDiamondMineralReq&);
        static void onUpgreadExpMineralReq(const ConnId& connId, UpgreadExpMineralReq&);
        static void onGetGoldMineralReq(const ConnId& connId, GetGoldMineralReq&);
        static void onGetDiamondMineralReq(const ConnId& connId, GetDiamondMineralReq&);
        static void onUpspeedExpMineralReq(const ConnId& connId, UpspeedExpMineralReq&);
        static void onViewPlayerInfoReq(const ConnId& connId, ViewPlayerInfoReq&);
        static void onSummonHeroReq(const ConnId& connId, SummonHeroReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_PLAYER_MSG_HANDLER_H__

