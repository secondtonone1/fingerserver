#ifndef __LYNX_GAME_SERVER_DETAILINFO_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_DETAILINFO_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct DetailInfoMsgHandler
	{
		static void onClientDetailInfoReq(const ConnId&, CGClientDetailInfoReq&);
		static void onClientUpdateInfoReq(const ConnId&, CGClientUpdateInfoReq&);
		static void onRenameReq(const ConnId&, CGPlayerRenameReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_DETAILINFO_MSG_HANDLER_H__
