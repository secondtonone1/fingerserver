#ifndef __LYNX_GAME_SERVER_SERVANT_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_SERVANT_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct ServantMsgHandler
	{

		static void onServantCombine(const ConnId&, CGServantCombineReq&);
		static void onServantLvUp(const ConnId&connId, CGServantLvUpReq&msg);
		static void onServantStarUp(const ConnId&connId, CGServantStarUpReq & msg);
		static void onServantFloorUp(const ConnId&connId, CGServantFloorUpReq & msg);
		static void onServantTreasureCombine(const ConnId&connId, CGTreasureCombineReq & msg);
		static void onServantTreasureEquip(const ConnId&connId, CGTreasureEquipReq & msg);
		static void onServantBattleOpen(const ConnId& connId, CGServantBattleOpenReq & msg);
		static void onAssistBattleEnhance(const ConnId& connId, CGAssistBattleEnhanceReq & msg);
		static void onServantBattleSet(const ConnId& connId, CGServantBattleSetReq & msg);
		static void onAssistBattleOneset(const ConnId& connId,   CGAssistBattleOnSetReq & msg);
		static void onServantSwitch(const ConnId & connId, CGServantSwitchReq & msg);
		static void onServantEquipOnce(const ConnId & connId, CGServantEquipOnceReq & msg);
		static void onServantInfoLock(const ConnId & connId, CGServantInfoLockReq & msg);
		
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_SERVANT_MSG_HANDLER_H__