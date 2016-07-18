#ifndef __LYNX_GAME_SERVER_EQUIP_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_EQUIP_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct EquipMsgHandler
    {
        static void onUseEquipReq(const ConnId&, UseEquipReq&);
        static void onRefineEquipReq(const ConnId&, RefineEquipReq&);
        static void onEnchantEquipReq(const ConnId&, EnchantEquipReq&);
        static void onCombineEquipReq(const ConnId&, CombineEquipReq&);
        static void onSellEquipReq(const ConnId& connId, SellEquipReq& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_EQUIP_MSG_HANDLER_H__

