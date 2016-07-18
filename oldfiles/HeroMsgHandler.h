#ifndef __LYNX_GAME_SERVER_HERO_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_HERO_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct HeroMsgHandler
    {
        static void onRefineHeroReq(const ConnId&, RefineHeroReq&);
        static void onAdvancedHeroReq(const ConnId&, AdvancedHeroReq&);
        static void onAwakenHeroReq(const ConnId&, AwakenHeroReq&);
        static void onCombineHeroReq(const ConnId&, CombineHeroReq&);
        static void onExchangeHeroPresentPosReq(const ConnId&, ExchangeHeroPresentPosReq&);
        static void onSetHeroPresentPosReq(const ConnId&, SetHeroPresentPosReq&);
        static void onSellHeroReq(const ConnId& connId, SellHeroReq& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_HERO_MSG_HANDLER_H__

