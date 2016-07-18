#ifndef __LYNX_GAME_SERVER_COMMON_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_COMMON_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct CommonMsgHandler
	{
	    static void onMsgAckNotify(const ConnId&, MsgAckNotify&);
        static void onSyncTimeReq(const ConnId&, SyncTimeReq&);
        static void onKeepalivePingReq(const ConnId&, KeepalivePingReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_COMMON_MSG_HANDLER_H__

