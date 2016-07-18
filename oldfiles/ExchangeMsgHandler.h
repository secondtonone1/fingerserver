#ifndef __LYNX_GAME_SERVER_EXCHANGE_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_EXCHANGE_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct ExchangeMsgHandler
    {
        static void onExchangeBuyItemReq(const ConnId&, ExchangeBuyItemReq&);
        static void onGetArenaExchangeDataReq(const ConnId&, GetArenaExchangeDataReq&);
        static void onReceiveArenaHonorAwardReq(const ConnId&, ReceiveArenaHonorAwardReq&);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_EXCHANGE_MSG_HANDLER_H__

