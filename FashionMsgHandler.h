#ifndef __LYNX_GAME_SERVER_FASHION_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_FASHION_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct FashionMsgHandler
	{
		static void onFashionBuy(const ConnId&connId,CGFashionBuyReq & msg);
	

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_FASHION_MSG_HANDLER_H__