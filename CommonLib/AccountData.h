#ifndef __LYNX_COMMON_LIB_ACCOUNT_DATA_H__
#define __LYNX_COMMON_LIB_ACCOUNT_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct AccountData
	{
		AccountData() : mAccountId(0), mAccountName(""), mLoginClientIp(""), mIdCard(""),
            mAdult(false), mApplingAdult(false), mLoginTime(0), mRmb(0),
            mGMLevel(0), mIsActived(false), mLogoutTime(0) {}

        // 账户ID
		Guid mAccountId;
        // 账户名
		NameString mAccountName;
        // 客户端登陆IP
        CharArray<16> mLoginClientIp;
        // 身份证
        CharArray<20> mIdCard;
        // 已通过防沉迷验证
        bool mAdult;
        // 已申请防沉迷验证
        bool mApplingAdult;
        // 登录的时间戳
        UInt32 mLoginTime;
        // 上次登出时间
        UInt32 mLogoutTime;
        // 帐户余额
        UInt32 mRmb;
        // GM等级(0为普通账户，>=1 为GM帐户)
        UInt8 mGMLevel;
        // 是否冻结（已经冻结true  未冻结false）
        bool mIsActived;
        //// vip等级经验
        //UInt32 mVipExp;

		LYNX_S11N_11(AccountData, Guid, mAccountId, NameString, mAccountName, CharArray<16>, mLoginClientIp,
            CharArray<20>, mIdCard, bool, mAdult, bool, mApplingAdult, UInt32, mLoginTime, UInt32, mRmb,
            UInt8, mGMLevel, bool, mIsActived, UInt32, mLogoutTime);

#if __CSharp__
        void testFunc()
        {
            
        }
#endif
	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ACCOUNT_DATA_H__

