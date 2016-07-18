#ifndef __LYNX_GAME_SERVER_FRIEND_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_FRIEND_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct FriendMsgHandler
	{
	
		static void onAddFriendReq(const ConnId&, CGFriendAddReq&);
		static void onAddFriendBack(const ConnId&, CGFriendAddBackReq&);
		static void onDelFriendReq(const ConnId&, CGFriendDelReq&);
		static void onAddBlackReq(const ConnId&, CGBlackAddReq&);
		static void onDelBlackReq(const ConnId&, CGBlackDelReq&);
		static void onRecommendListReq(const ConnId&, CGRecommendListReq&);
		static void onFindPlayerReq(const ConnId &,  CGFindPlayerReq&);

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_FRIEND_MSG_HANDLER_H__