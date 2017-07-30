#ifndef __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct GMMessageHandler
	{
		static void onMsgGMReq(const ConnId&, CGGMReq&);
		//禁止所有人登陆
		static void onMsgGMForbidLoginReq(const ConnId& connId, CGGMForbidLoginReq& msg);
		//踢某个人下线
		static void onMsgGMKickPlayerReq(const ConnId& connId, CGGMKickPlayerReq& msg);
		//禁止聊天
		static void onMsgGMForbidChatReq(const ConnId & connId, CGGMForbidChatReq & msg);
		//发送邮件
		static void onMsgGMSendEmailReq(const ConnId & connId, CGGMSendEmailReq & msg);
		//踢掉所有人，关闭服务器
		static void onMsgGMKickAllReq(const ConnId & connId, CGGMKickAllReq & msg);
		//发送系统通知
		static void onMsgGMNoticeReq(const ConnId & connId, CGGMNoticeReq & msg);
		//GM重置玩家每天玩法次数，如成就，七日训练等
		static void onMsgGMReset(const ConnId & connId, CGGMResetReq & msg);
		//GM发送礼包id
		static void onMsgGMAwardBag(const ConnId & connId, CGGAwardBagReq & msg);

		
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__

