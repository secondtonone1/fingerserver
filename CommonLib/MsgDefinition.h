#ifndef __LYNX_COMMON_LIB_MSG_DEFINITION_H__
#define __LYNX_COMMON_LIB_MSG_DEFINITION_H__

#include "TypeDefinition.h"

namespace Lynx 
{
	static const char* sClientConnectionStr = "ClientConnection_2014_10_22";
    static const char* sGMConnectionStr = "GMTClientConnection_2014_11_20";

    static const UInt16 sGMMsgBase           = 0;
	static const UInt16 sCommonMsgBase       = 1000;
	static const UInt16 sLoginMsgBase        = 2000;
	static const UInt16 sPlayerMsgBase       = 3000;
	static const UInt16 sKillMsgBase         = 4000;
	static const UInt16 sBuffMsgBase         = 5000;
	static const UInt16 sItemMsgBase         = 6000;
	static const UInt16 sStageMsgBase		 = 7000;
	static const UInt16 sFashionMsgBase		 = 8000;
	
	static const UInt16 sEquipMsgBase		 = 10000;
	static const UInt16 sFireMsgBase		 = 13000;
	static const UInt16 sRhymeMsgBase        = 14000;
	static const UInt16 sPVPMsgBase			 = 15000;
	static const UInt16 sHoarStoneBase       = 16000;
	static const UInt16 sHeroEquipMsgBase    = 17000;
	static const UInt16 sGemMsgBase          = 18000;

	static const UInt16 sWelfaleMarketBase   = 19000;

	static const UInt16 sJewelryBase         = 20000;
	static const UInt16 sMaterialMsgBase     = 21000;
	static const UInt16 sServantMsgBase      = 22000;

	static const UInt16 sChatMsgBase         = 23000;
	static const UInt16 sClimTowerMsgBase    = 24000;
	static const UInt16 sDetailInfoMsgBase   = 25000;

	static const UInt16 sFriendBlackMsgBase  = 26000;

	static const UInt16 sEmailMsgBase		 = 27000;

	static const UInt16 sShopBase			 = 28000;

	static const UInt16 sCharactorBase     = 29000;

	static const UInt16 sAchieveDailyBase  = 30000;

	static const UInt16 sInLineActivityBase     = 31000;

	static const UInt16 sConsortBase = 9000;

	static const UInt16 sRankGameBase     = 31500;
	static const UInt16 sPVPBase = 32000;


	
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_MSG_DEFINITION_H__