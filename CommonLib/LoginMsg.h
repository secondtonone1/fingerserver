#ifndef __LYNX_COMMON_LIB_LOGIN_MSG_H__
#define __LYNX_COMMON_LIB_LOGIN_MSG_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
        RECONNECT_REQ                  = sLoginMsgBase + 14,
        RECONNECT_SUCCESS_RESP         = sLoginMsgBase + 15,
        RECONNECT_FAIL_RESP            = sLoginMsgBase + 16,
        CREATE_PLAYER_NAME_NOTIFY      = sLoginMsgBase + 17,
        CREATE_PLAYER_NAME_REQ         = sLoginMsgBase + 8,
        CREATE_PLAYER_NAME_RESP        = sLoginMsgBase + 9,
		LOGIN_REQ_TEST				   = sLoginMsgBase + 10
		
	};

	

    struct ReconnectReq
    {
        ReconnectReq() : mPlayerGuid(0), mLoginToken(0) {}

        Guid mPlayerGuid;
        UInt64 mLoginToken;

        LYNX_MESSAGE_2(RECONNECT_REQ, ReconnectReq, Guid, mPlayerGuid, UInt64, mLoginToken);
    };

    struct ReconnectSuccessResp
    {
        ReconnectSuccessResp() : mLoginToken(0) {}

        UInt64 mLoginToken;

        LYNX_MESSAGE_1(RECONNECT_SUCCESS_RESP, ReconnectSuccessResp,
            UInt64, mLoginToken);
    };

    struct ReconnectFailResp
    {
        ReconnectFailResp() : mErrno(LynxErrno::None) {}

        errorId mErrno;

        LYNX_MESSAGE_1(RECONNECT_FAIL_RESP, ReconnectFailResp,
            errorId, mErrno);
    };

    struct CreatePlayerNameNotify
    {
        Guid mAccountId;
        String mAccountName;
        CreatePlayerNameNotify(): mAccountId(0), mAccountName("") {}

        LYNX_MESSAGE_2(CREATE_PLAYER_NAME_NOTIFY, CreatePlayerNameNotify, Guid, mAccountId, String, mAccountName);
    };

    struct CreatePlayerReq
    {
        String mPlayerName;
        Guid mAccountId;
        String mAccountName;

        CreatePlayerReq(): mPlayerName(""), mAccountId(0), mAccountName("") {}

        LYNX_MESSAGE_3(CREATE_PLAYER_NAME_REQ, CreatePlayerReq, String, mPlayerName, Guid, mAccountId, String, mAccountName);
    };

    struct CreatePlayerResp
    {
        errorId merrorId;
        Guid mAccountId;

        CreatePlayerResp(): merrorId(0) {}

        LYNX_MESSAGE_2(CREATE_PLAYER_NAME_RESP, CreatePlayerResp, Guid, mAccountId, errorId, merrorId);
    };
	

//__________________________________________________________________________________________________________________________________________

	//新的发给客户端的协议，都转化为字符串
	
	


} // namespace Lynx

#endif // __LYNX_COMMON_LIB_LOGIN_MSG_H__