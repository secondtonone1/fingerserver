#ifndef __LYNX_COMMON_LIB_ACK_MSG_H__
#define __LYNX_COMMON_LIB_ACK_MSG_H__

#include "Data.h"
#include "../jsoncpp/include/json/json.h"

namespace Lynx
{
	enum 
	{
		MSG_ACK_NOTIFY                        = sCommonMsgBase + 1,
        SYNC_TIME_REQ                         = sCommonMsgBase + 2,
        SYNC_TIME_RESP                        = sCommonMsgBase + 3,
        ACCOUNT_WAS_LOGIN_NOTIFY              = sCommonMsgBase + 4,
        KEEPALIVE_PING_REQ                    = sCommonMsgBase + 5,
        KEEPALIVE_PING_RESP                   = sCommonMsgBase + 6,
        SYSTEM_NOTICE_NOTIFY                  = sCommonMsgBase + 7,

		
		SYSTEM_SHUTDOWN_NOTIFY              = sCommonMsgBase + 101,

	};
	
	struct ServerShutDown
	{
		ServerShutDown() : time(0) {}

		UInt32 time;

		LYNX_MESSAGE_1(SYSTEM_SHUTDOWN_NOTIFY, ServerShutDown, UInt32, time);
	};

	struct MsgAckNotify
	{
		MsgAckNotify() : mAckId(0) {}

		UInt32 mAckId;

		LYNX_MESSAGE_1(MSG_ACK_NOTIFY, MsgAckNotify, UInt32, mAckId);
	};

    struct SyncTimeReq
    {
        SyncTimeReq() :/* mSyncTimes(0),*/ mClientTime(0) {}

//         UInt8 mSyncTimes;
        UInt32 mClientTime;
		std::string strRec;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				mClientTime = root["mClientTime"].asUInt();
			}
		}

        LYNX_MESSAGE_1(SYNC_TIME_REQ, SyncTimeReq, std::string, strRec);
    };

    struct SyncTimeResp
    {
        SyncTimeResp() : /*mSyncTimes(0),*/ mClientTime(0), mServerTime(0) {}

//         UInt8 mSyncTimes;
        UInt32 mClientTime;
        UInt32 mServerTime;

        LYNX_MESSAGE_2(SYNC_TIME_RESP, SyncTimeResp,
            /*UInt8, mSyncTimes,*/ UInt32, mClientTime, UInt32, mServerTime);
    };

 /*   struct AccountWasLoginNotify
    {
        AccountWasLoginNotify() {}

        LYNX_MESSAGE_0(ACCOUNT_WAS_LOGIN_NOTIFY, AccountWasLoginNotify);
    };*/

	struct AccountWasLoginNotify
	{
		AccountWasLoginNotify(): mPacketID(ACCOUNT_WAS_LOGIN_NOTIFY), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(ACCOUNT_WAS_LOGIN_NOTIFY, AccountWasLoginNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

    struct KeepalivePingReq
    {
        KeepalivePingReq() : mPlayerGuid(0) {}

        Guid mPlayerGuid;

        LYNX_MESSAGE_1(KEEPALIVE_PING_REQ, KeepalivePingReq, Guid, mPlayerGuid);
    };

    struct KeepalivePingResp
    {
        KeepalivePingResp() {}

        LYNX_MESSAGE_0(KEEPALIVE_PING_RESP, KeepalivePingResp);
    };

    struct SystemNoticeNotify
    {
        SystemNoticeNotify() : mPriority(0), mContents("") {}

        UInt8 mPriority;
        String mContents;

        LYNX_MESSAGE_2(SYSTEM_NOTICE_NOTIFY, SystemNoticeNotify,
            UInt8, mPriority, String, mContents);
    };

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ACK_MSG_H__