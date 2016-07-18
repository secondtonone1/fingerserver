#ifndef __LYNX_GAME_SERVER_CHAT_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_CHAT_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct ChatMsgHandler
	{
        static void onSendChatMsgNotify(const ConnId& connId, SendChatMsgNotify&);
        static void onGetChatMsgReq(const ConnId& connId, GetChatMsgReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_CHAT_MSG_HANDLER_H__

