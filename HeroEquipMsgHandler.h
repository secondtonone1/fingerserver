#ifndef __LYNX_GAME_SERVER_HEROEQUIP_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_HEROEQUIP_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct HeroEquipMsgHandler
	{
		static void onHeroEquipLvUp(const ConnId&, CGHeroEquipEnhanceReq&);
		static void onHeroEquipGemSet(const ConnId&, CGHeroEquipGemSetReq&);
		static void onHeroEquipGemSetOnce(const ConnId& ,CGHeroEquipGemSetOnceReq &);
		static void onHeroEquipGemManualActive(const ConnId&, CGHeroEquipGemManualActiveReq &);
		static void onHeroEquipGemLvUp(const ConnId&, CGHeroEquipGemLvUpReq &);
		static void onHeroEquipGemUnload(const ConnId&, CGHeroEquipGemUnloadReq &);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_HEROEQUIP_MSG_HANDLER_H__



