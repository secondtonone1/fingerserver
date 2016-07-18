#ifndef __LYNX_GAME_SERVER_LOGIN_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_LOGIN_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct LoginMsgHandler
	{
        static void onReconnectReq(const ConnId&, ReconnectReq&);
		static void onCGGameLoginReq(const ConnId&, CGGameLoginReq&);
		static void onCGCreatePlayerReq(const ConnId&, CGGameCreatePlayerReq&);
		static void onCGAccountLoginReq(const ConnId&, CGAccountLoginReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_LOGIN_MSG_HANDLER_H__

