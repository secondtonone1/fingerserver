#ifndef __LYNX_COMMON_LIB_CHAT_PACKET_H__
#define __LYNX_COMMON_LIB_CHAT_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_CHATDATALIST_REQ					= sChatMsgBase + 1,
		BOC_CHATDATALIST_RESP					= sChatMsgBase + 2,
		BOC_SETBLACKLIST_REQ                    = sChatMsgBase + 3,
		BOC_SETBLACKLIST_RESP                   = sChatMsgBase + 4,
		BOC_ADDFRIEND_REQ                       = sChatMsgBase + 5,
		BOC_ADDFRIEND_RESP                      = sChatMsgBase + 6,
		BOC_SHOWDETAIL_REQ                      = sChatMsgBase + 7,
		BOC_SHOWDETAIL_RESP                     = sChatMsgBase + 8,
		BOC_CLIENTCHAT_REQ                      = sChatMsgBase + 9,
		BOC_CLIENTCHAT_RESP                     = sChatMsgBase + 10,
		BOC_OFFLINE_NOTIFY                      = sChatMsgBase + 11, //玩家下线通知
		BOC_FORBIDCHAT_REQ                      = sChatMsgBase + 12, //客户端禁言请求
		BOC_RESETCHAT_REQ                       = sChatMsgBase + 13, //客户端禁言恢复请求
		BOC_RESETCHAT_NOTIFY                    = sChatMsgBase + 14,  //服务器禁言恢复通知客户端
		BOC_FORBIDCHAT_NOTIFY                   = sChatMsgBase + 15,   //服务器通知客户端禁言
		BOC_CLIENTONLINE_REQ                    = sChatMsgBase + 16,   //客户端请求服务器获取在线信息
		BOC_CLIENTONLINE_RESP                   = sChatMsgBase + 17   //服务器回复给客户端聊天的在线情况
		
	};

	//-----------------CG-------------------
	struct CGChatDataListReq
	{
		CGChatDataListReq(): mPacketID(BOC_CHATDATALIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_CHATDATALIST_REQ, CGChatDataListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct CGSetBlackListReq
	{
		CGSetBlackListReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SETBLACKLIST_REQ, CGSetBlackListReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGAddFriendReq
	{
		CGAddFriendReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ADDFRIEND_REQ, CGAddFriendReq, UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGShowDetailReq
	{
		CGShowDetailReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SHOWDETAIL_REQ, CGShowDetailReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};
	

	struct CGClientChatReq
	{
		CGClientChatReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CLIENTCHAT_REQ, CGClientChatReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGForbidChatReq
	{
		CGForbidChatReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_FORBIDCHAT_REQ, CGForbidChatReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGResetChatReq
	{
		CGResetChatReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_RESETCHAT_REQ, CGResetChatReq,UInt16, mPacketID, std::string, mReqJsonStr);
		     
	};

	struct CGChatOnlineReq
	{
		CGChatOnlineReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CLIENTONLINE_REQ, CGChatOnlineReq,UInt16,mPacketID, std::string, mReqJsonStr);

	};
	
	          
		
	

	//-----------------GC-------------------
	struct GCChatDataListResp
	{
		GCChatDataListResp(): mPacketID(BOC_CHATDATALIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CHATDATALIST_RESP, GCChatDataListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


	struct GCSetBlackListResp
	{
		GCSetBlackListResp():mPacketID(BOC_SETBLACKLIST_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SETBLACKLIST_RESP,GCSetBlackListResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};


	struct GCAddFriendResp
	{
		GCAddFriendResp():mPacketID(0),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ADDFRIEND_RESP,GCAddFriendResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCShowDetailResp
	{
		GCShowDetailResp():mPacketID(0),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_SHOWDETAIL_RESP, GCShowDetailResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCClientChatResp
	{
		GCClientChatResp():mPacketID(0),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_CLIENTCHAT_RESP, GCClientChatResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	
	struct GCOffLineNotify
	{
		GCOffLineNotify():mPacketID(0),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_OFFLINE_NOTIFY, GCOffLineNotify,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCResetChatNotify
	{
		GCResetChatNotify():mPacketID(0),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_RESETCHAT_NOTIFY, GCResetChatNotify, UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCForbidChatNotify
	{
		GCForbidChatNotify():mPacketID(0),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_FORBIDCHAT_NOTIFY, GCForbidChatNotify,UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCChatOnlineResp
	{
		GCChatOnlineResp():mPacketID(0),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_CLIENTONLINE_RESP, GCChatOnlineResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};


	 
	


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_CHAT_PACKET_H__

