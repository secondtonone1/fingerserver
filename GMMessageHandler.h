#ifndef __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct GMMessageHandler
	{
		static void onMsgGMReq(const ConnId&, CGGMReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__

