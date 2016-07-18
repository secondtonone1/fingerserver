#ifndef __LYNX_GAME_SERVER_RUNE_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_RUNE_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct RuneMsgHandler
    {
        static void onUseRuneReq(const ConnId& connId, UseRuneReq& msg);
        static void onRefineRuneReq(const ConnId& connId, RefineRuneReq& msg);
        static void onSellRuneReq(const ConnId& connId, SellRuneReq& msg);
        static void onCombineRuneReq(const ConnId& connId, CombineRuneReq& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RUNE_MSG_HANDLER_H__

