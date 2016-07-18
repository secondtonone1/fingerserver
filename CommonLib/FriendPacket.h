#ifndef __LYNX_COMMON_LIB_FRIEND_PACKET_H__
#define __LYNX_COMMON_LIB_FRIEND_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_FRIENDBLACKLIST_REQ					= sFriendBlackMsgBase + 1,   //客户端向服务器请求好友和和名单列表
		BOC_FRIENDBLACKLIST_RESP				= sFriendBlackMsgBase + 2,  //服务器回复客户端好友和黑名单列表
		BOC_FRIENDADD_REQ                           = sFriendBlackMsgBase + 3,  //客户端请求服务器加好友请求
		BOC_FRIENDADD_NOTIFY                     = sFriendBlackMsgBase + 4,     //服务器通知客户端有人添加好友
		BOC_ADDAGRU_REQ								= sFriendBlackMsgBase + 5,		//客户端同意或者拒绝添加好友，发送给服务器
		BOC_ADDAGRU_RESP								= sFriendBlackMsgBase + 6,    //A请求B，B同意，这个时候服务器审核A好友是否满了，给客户端B回包
		//BOC_FRIENDADD_RESP							= sFriendBlackMsgBase + 7,//服务器通知客户端添加好友结果，同意

		BOC_FRIENDDEL_REQ								= sFriendBlackMsgBase + 8, //客户端请求删除好友
		//BOC_FRIENDDEL_NOTIFY						= sFriendBlackMsgBase + 9 , //A删除B好友，如果B在线，服务器通知客户端B删除好友
		BOC_FRIENDDEL_RESP							= sFriendBlackMsgBase + 10 ,// A删除B好友，服务器回包给A

		BOC_ADDBLACK_REQ                            = sFriendBlackMsgBase + 13,  //客户端添加黑名单请求
		BOC_ADDBLACK_RESP							= sFriendBlackMsgBase + 14,  //服务器添加黑名单回复
	
		BOC_DELBLACK_REQ								= sFriendBlackMsgBase + 15,  //客户端删除黑名单请求
		BOC_DELBALCK_RESP								= sFriendBlackMsgBase + 16,  //服务器删除黑名单回复

		BOC_RECOMMENDLIST_REQ					= sFriendBlackMsgBase + 17, //客户端请求好友列表
		BOC_RECOMMENDLIST_RESP				= sFriendBlackMsgBase + 18, //服务器同步好友列表回复

		BOC_FINDPLAYER_REQ                          = sFriendBlackMsgBase + 19, //查找指定玩家
		BOC_FINDPLAYER_RESP                         = sFriendBlackMsgBase + 20

	};

	//-----------------CG-------------------
	struct CGFriendBlackListReq
	{
		CGFriendBlackListReq(): mPacketID(BOC_FRIENDBLACKLIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_FRIENDBLACKLIST_REQ, CGFriendBlackListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct CGFriendAddReq
	{
		CGFriendAddReq():mPacketID(BOC_FRIENDADD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDADD_REQ, CGFriendAddReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	//A加B，B给服务器回包，同意还是拒绝
	struct CGFriendAddBackReq
	{
		CGFriendAddBackReq():mPacketID(BOC_ADDAGRU_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ADDAGRU_REQ, CGFriendAddBackReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	
	//A请求删除B，客户端向服务器发请求
	struct CGFriendDelReq
	{
		CGFriendDelReq():mPacketID(BOC_FRIENDDEL_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDDEL_REQ, CGFriendDelReq, UInt16, mPacketID, std::string, mReqJsonStr);
		
	};

	//A请求删除黑名单
	struct CGBlackDelReq 
	{
		CGBlackDelReq():mPacketID(BOC_DELBLACK_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_DELBLACK_REQ, CGBlackDelReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};
	
	//A请求添加黑名单
	struct CGBlackAddReq
	{
		CGBlackAddReq():mPacketID(BOC_ADDBLACK_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ADDBLACK_REQ, CGBlackAddReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGRecommendListReq
	{
		CGRecommendListReq():mPacketID(BOC_RECOMMENDLIST_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_RECOMMENDLIST_REQ, CGRecommendListReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	struct CGFindPlayerReq
	{
		CGFindPlayerReq():mPacketID(BOC_FINDPLAYER_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_FINDPLAYER_REQ, CGFindPlayerReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	

	//-----------------GC-------------------
	struct GCFriendBlackListResp
	{
		GCFriendBlackListResp():mPacketID(BOC_FRIENDBLACKLIST_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_FRIENDBLACKLIST_RESP,GCFriendBlackListResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCFriendAddNotify
	{
		GCFriendAddNotify():mPacketID(BOC_FRIENDADD_NOTIFY),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDADD_NOTIFY,GCFriendAddNotify,UInt16, mPacketID,std::string, mRespJsonStr);
	};
	
	//A加B好友，B同意后，服务器审核A目前好友是否满了，将结果回复给B
	struct GCFriendAddBackResp
	{
		GCFriendAddBackResp():mPacketID(BOC_ADDAGRU_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ADDAGRU_RESP,GCFriendAddBackResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	//struct GCFriendAddResp
	//{
	//	GCFriendAddResp():mPacketID(BOC_FRIENDADD_RESP),mRespJsonStr(""){}
	//	UInt16 mPacketID;
	//	std::string mRespJsonStr;

	//	LYNX_MESSAGE_2(BOC_FRIENDADD_RESP,GCFriendAddResp,UInt16, mPacketID,std::string, mRespJsonStr);
	//};
	
	////A删除B好友，如果B在线，服务器通知客户端B删除好友
	//struct GCFriendDelNotify
	//{
	//	GCFriendDelNotify():mPacketID(BOC_FRIENDDEL_NOTIFY),mRespJsonStr(""){}
	//	UInt16 mPacketID;
	//	std::string mRespJsonStr;

	//	LYNX_MESSAGE_2(BOC_FRIENDDEL_NOTIFY,GCFriendDelNotify,UInt16, mPacketID,std::string, mRespJsonStr);
	//};
			
	//A删除B好友，服务器回包给A
	struct GCFriendDelResp
	{
		GCFriendDelResp():mPacketID(BOC_FRIENDDEL_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDDEL_RESP,GCFriendDelResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	
	struct GCBlackAddResp
	{
		GCBlackAddResp():mPacketID(BOC_ADDBLACK_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ADDBLACK_RESP,GCBlackAddResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCBlackDelResp
	{
		GCBlackDelResp():mPacketID(BOC_DELBALCK_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_DELBALCK_RESP,GCBlackDelResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCRecommendListResp
	{
		GCRecommendListResp():mPacketID(BOC_RECOMMENDLIST_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_RECOMMENDLIST_RESP, GCRecommendListResp, UInt16, mPacketID, std::string, mRespJsonStr);

	};
	
	struct GCFindPlayerResp
	{
		GCFindPlayerResp():mPacketID(BOC_FINDPLAYER_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_FINDPLAYER_RESP, GCFindPlayerResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_FRIEND_PACKET_H__

