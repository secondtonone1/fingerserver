#ifndef __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct GMMsgHandler
	{
	    static void onGMLoginReq(const ConnId& connId, GMLoginReq&);
        static void onGMGetPlayerByOnlineReq(const ConnId& connId, GMGetPlayerByOnlineReq&);
        static void onGMGetPlayerByPlayerGuidReq(const ConnId&, GMGetPlayerByPlayerGuidReq&);
        static void onGMGetPlayerByPlayerNameReq(const ConnId&, GMGetPlayerByPlayerNameReq&);
        static void onGMGetPlayerByAccountGuidReq(const ConnId&, GMGetPlayerByAccountGuidReq&);
        static void onGMGetPlayerByAccountNameReq(const ConnId&, GMGetPlayerByAccountNameReq&);
        static void onGMGetPlayerDataReq(const ConnId&, GMGetPlayerDataReq&);
        static void onGMSetPlayerExpReq(const ConnId&, GMSetPlayerExpReq&);
        static void onGMSetPlayerLevelReq(const ConnId&, GMSetPlayerLevelReq&);
        static void onGMSetPlayerEnergyReq(const ConnId&, GMSetPlayerEnergyReq&);
        static void onGMSetPlayerGoldReq(const ConnId&, GMSetPlayerGoldReq&);
        static void onGMAddItemReq(const ConnId&, GMAddItemReq&);
        static void onGMDelItemReq(const ConnId&, GMDelItemReq&);
        static void onGMAddEquipReq(const ConnId&, GMAddEquipReq&);
        static void onGMDelEquipReq(const ConnId&, GMDelEquipReq&);
        static void onGMAddRuneReq(const ConnId&, GMAddRuneReq&);
        static void onGMDelRuneReq(const ConnId&, GMDelRuneReq&);
        static void onGMAddHeroDebrisReq(const ConnId&, GMAddHeroDebrisReq&);
        static void onGMDelHeroDebrisReq(const ConnId&, GMDelHeroDebrisReq&);
        static void onGMAddHeroReq(const ConnId&, GMAddHeroReq&);
        static void onGMDelHeroReq(const ConnId&, GMDelHeroReq&);
        static void onGMAddPetReq(const ConnId&, GMAddPetReq&);
        static void onGMDelPetReq(const ConnId&, GMDelPetReq&);
        static void onGMSetHeroExpReq(const ConnId&, GMSetHeroExpReq&);
        static void onGMSetHeroAdvancedReq(const ConnId&, GMSetHeroAdvancedReq&);
        static void onGMSetHeroLevelReq(const ConnId&, GMSetHeroLevelReq&);
        static void onGMSetHeroPresentPosReq(const ConnId&, GMSetHeroPresentPosReq&);
        static void onGMSetHeroEquipReq(const ConnId&, GMSetHeroEquipReq&);
        static void onGMSetHeroRuneReq(const ConnId&, GMSetHeroRuneReq&);
        static void onGMSetEquipRefineReq(const ConnId&, GMSetEquipRefineReq&);
        static void onGMSetRuneRefineReq(const ConnId&, GMSetRuneRefineReq&);
        static void onGMSetTollgateReq(const ConnId&, GMSetTollgateReq&);
        static void onGMAddPetDebrisReq(const ConnId&, GMAddPetDebrisReq&);
        static void onGMDelPetDebrisReq(const ConnId&, GMDelPetDebrisReq&);
        static void onGMSendSystemNoticeReq(const ConnId&, GMSendSystemNoticeReq&);

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__
