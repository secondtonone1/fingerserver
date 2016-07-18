#ifndef __LYNX_GAME_SERVER_STORE_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_STORE_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct StoreMsgHandler
    {
        static void onStoreBuyItemReq(const ConnId&, StoreBuyItemReq&);
        static void onSecretStoreBuyItemReq(const ConnId&, SecretStoreBuyItemReq&);
        static void onResetSecretStoreItemReq(const ConnId&, ResetSecretStoreItemReq&);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_STORE_MSG_HANDLER_H__

