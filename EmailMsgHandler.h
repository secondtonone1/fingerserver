#ifndef __LYNX_GAME_SERVER_EMAIL_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_EMAIL_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct EmailMsgHandler
	{

		static void onGMSendEmail(const ConnId&connId, GMSendReq& msg);
		static void onDelEmailReq(const ConnId & connId, DelEmailReq& msg);
		static void onDelEmailOnce(const ConnId & connId, EmailDelOnceReq& msg);
		static void onReadEmailReq(const ConnId & connId, EmailOpenReq& msg);
		static void onGetContantReq(const ConnId & connId, EmailGetContantReq& msg);
		static void onGetContantListReq(const ConnId & connId, EmailGetOnceReq& msg);

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_FRIEND_MSG_HANDLER_H__