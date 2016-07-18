#ifndef __LYNX_GAME_SERVER_ITEM_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_ITEM_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct ItemMsgHandler
    {
        static void onUseItemReq(const ConnId& connId, UseItemReq& msg);
        static void onSellItemReq(const ConnId& connId, SellItemReq& msg);
		static void onEhanceEquipReq(const ConnId& connId, EquipEhanceReq& msg);
		static void onPolishEquipReq(const ConnId& connId, EquipPolishReq& msg);
		static void onPolishResetEquipReq(const ConnId& connId, EquipPolishResetReq& msg);
		static void onGemCombineReq(const ConnId& connId, GemCombineReq& msg);
		static void onGemCombineOnceReq(const ConnId& connId, GemCombineOnceReq& msg);
		static void onGemSetReq(const ConnId& connId, GemSetReq& msg);
		static void onEquipGemCombineReq(const ConnId& connId, EquipGemCombineReq& msg);
		static void onGemUnloadReq(const ConnId& connId, GemUnloadReq& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_ITEM_MSG_HANDLER_H__

