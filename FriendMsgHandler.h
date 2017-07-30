#ifndef __LYNX_GAME_SERVER_FRIEND_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_FRIEND_MSG_HANDLER_H__

#include "CommonLib.h"
#include "FriendBlackManager.h"

namespace Lynx
{
	struct FriendMsgHandler : public MainThread ,public Singleton<FriendMsgHandler>
	{
	
		static void onFriendReq(const ConnId&, CGFriendBlackListReq&);
		static void onAddFriendReq(const ConnId&, CGFriendAddReq&);
		static void onAddFriendBack(const ConnId&, CGFriendAddBackReq&);
		static void onDelFriendReq(const ConnId&, CGFriendDelReq&);
		static void onAddBlackReq(const ConnId&, CGBlackAddReq&);
		static void onDelBlackReq(const ConnId&, CGBlackDelReq&);
		static void onRecommendListReq(const ConnId&, CGRecommendListReq&);
		static void onFindPlayerReq(const ConnId &,  CGFindPlayerReq&);

		 void dealAddFriendReq(const ConnId& connId, CGFriendAddReq& msg,	Json::Value &sendroot);


		bool judgeFriendBack(UInt64 playerUid,UInt64 otherUid,FriendBlackInfo &otherFriendInfo);

		bool judgeBlackBack(UInt64 playerUid,UInt64 otherUid,FriendBlackInfo &otherFriendInfo);

		bool addFriendBeApplyBack(UInt64 playerUid,BaseInfoData baseInfoData,List<UInt64> &friendBeApplyGuidList);

		bool delFriendBeApplyBack(UInt64 playerUid,List<UInt64> &friendBeApplyGuidList);

		bool delFriendBeAllApplyBack(UInt64 playerUid,List<UInt64> &friendBeApplyGuidList);

		bool getFriendApplyListBack(UInt64 playerUid,List<BaseInfoData>& baseInfoDataList);

		bool judgeOtherBeApplyBack(UInt64 playerUid,UInt64 otherUid,List<UInt64> &friendBeApplyGuidList);

		void saveFriendDataBack(UInt64 playerUid,FriendBlackInfo &otherFriendInfo);


		bool delFriendBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo);

		bool addFriendBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo);

		bool delBlackBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo);

		bool addBlackBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo);

		bool judgeFriendNumBack(UInt64 playerUid,FriendBlackInfo &friendInfo);

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_FRIEND_MSG_HANDLER_H__