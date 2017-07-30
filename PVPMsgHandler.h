#ifndef __LYNX_GAME_SERVER_PVP_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_PVP_MSG_HANDLER_H__

#include "CommonLib.h"


namespace Lynx
{
	struct PVPMsgHandler
	{
	
		static void onPVPReq(const ConnId&, CGPVPReq&);
	

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_PVP_MSG_HANDLER_H__