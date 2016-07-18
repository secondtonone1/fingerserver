#ifndef __LYNX_GAME_SERVER_CHAT_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_CHAT_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct ChatMsgHandler
	{
		static void onMsgSetBlack(const ConnId&, CGSetBlackListReq&);
		static void onMsgAddFriend(const ConnId&, CGAddFriendReq&);
		static void onMsgShowDetail(const ConnId&, CGShowDetailReq&);
		static void onMsgClientChat(const ConnId&, CGClientChatReq&);
		static void onMsgClientForbid(const ConnId&, CGForbidChatReq&);
		static void onMsgClientReset(const ConnId&, CGResetChatReq&);
		static void onMsgClientOnline(const ConnId&, CGChatOnlineReq &);

	
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_CHAT_MSG_HANDLER_H__

