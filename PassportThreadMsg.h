#ifndef __LYNX_GAME_SERVER_PASSPORT_THREAD_MSG_H__
#define __LYNX_GAME_SERVER_PASSPORT_THREAD_MSG_H__

#include "CommonLib.h"

namespace Lynx
{
	enum
	{
        // 账户验证请求
        PASSPORT_AUTH_REQ              = 1,
        // 账户验证通知
        PASSPORT_AUTH_RESP             = 2,
        // GM账户验证请求
        PASSPORT_GM_AUTH_REQ           = 3,
        // GM账户验证相应
        PASSPORT_GM_AUTH_RESP          = 4, 

		// 验证码请求
		PASSPORT_CODE_REQ           = 5,
		// 验证码返回
		PASSPORT_CODE_RESP          = 6, 

	};

    struct PassportAuthReq      
    {
        PassportAuthReq() : mAccountName(""), mPassword(""), mConnId(0), mFromIp(""), mFromPort(0) {}

        ConnId mConnId;
        String mAccountName;
        String mPassword;
        String mFromIp;
        UInt16 mFromPort;

        LYNX_MESSAGE_5(PASSPORT_AUTH_REQ, 
            PassportAuthReq, String, mAccountName, String, mPassword, ConnId, mConnId,
            String, mFromIp, UInt16, mFromPort);
    };

    struct PassportAuthResp      
    {
        PassportAuthResp() : merrorId(0), mConnId(0), mAccountId(0), mAccountName(""),
            mFromIp(""), mFromPort(0) {}

        errorId merrorId;
        ConnId mConnId;
        Guid mAccountId;
        String mAccountName;
        String mFromIp;
        UInt16 mFromPort;

        LYNX_MESSAGE_6(PASSPORT_AUTH_RESP, 
            PassportAuthResp, errorId, merrorId, ConnId, mConnId, Guid, mAccountId,
            String, mAccountName, String, mFromIp, UInt16, mFromPort);
    };

	struct PassportCodeReq      
	{
		PassportCodeReq() : playerID(0), code(""), reqType(0), isSave(0) {}
		
		string code;
		Guid playerID;		
		UInt32 reqType;	
		UInt32 isSave;	//1 存档

		LYNX_MESSAGE_4(PASSPORT_CODE_REQ,PassportCodeReq, Guid, playerID, string, code, UInt32, reqType, UInt32, isSave);
	};

	struct PassporCodeResp      
	{
		PassporCodeResp() : PlayerID(0), reqType(0), beginTime(0), code(""),
			endTime(0), awardID(0), errorId(0) {}

		UInt32 errorId;
		string code;
		Guid PlayerID;		
		UInt32 reqType;
		UInt32 beginTime;
		UInt32 endTime;
		UInt32 awardID;

		LYNX_MESSAGE_7(PASSPORT_CODE_RESP,PassporCodeResp, Guid, PlayerID,string, code,UInt32, reqType,
		UInt32, beginTime,UInt32, endTime,UInt32, awardID,UInt32, errorId);
	};

   

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PASSPORT_THREAD_MSG_H__