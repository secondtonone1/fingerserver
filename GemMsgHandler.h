#ifndef __LYNX_GAME_SERVER_GEM_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_GEM_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct GemMsgHandler
	{
		static void onGemCombineReq(const ConnId&, CGGemCombineReq&);
		static void onGemCombineOnce(const ConnId& connId, CGGemCombineOnceReq& msg);

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RHYME_MSG_HANDLER_H__