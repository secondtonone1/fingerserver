#ifndef __LYNX_COMMON_LIB_ACHIEVEMENT_MSG_H__
#define __LYNX_COMMON_LIB_ACHIEVEMENT_MSG_H__

#include "Data.h"

namespace Lynx
{
    enum
    {
        UPDATE_ACHIEVEMENT_STATUS_NOTIFY        = sAchievementMsgBase + 1,
        UPDATE_ACHIEVEMENT_BASE_DATA_NOTIFY     = sAchievementMsgBase + 2,
        COMPLETE_ACHIEVEMENT_NOTIFY             = sAchievementMsgBase + 3,
        GET_ACHIEVEMENT_AWARD_REQ               = sAchievementMsgBase + 4,
        GET_ACHIEVEMENT_AWARD_RESP              = sAchievementMsgBase + 5,

    };

    struct UpdateAchievementStatusNotify
    {
        UInt8 mIndex;
        UInt32 mCurId;
        UInt32 mCompleteId;
        UInt32 mOldId;

        UpdateAchievementStatusNotify(): mIndex(0), mCurId(0), mCompleteId(0), mOldId(0) {}

        LYNX_MESSAGE_4(UPDATE_ACHIEVEMENT_STATUS_NOTIFY, UpdateAchievementStatusNotify, UInt8, mIndex, 
            UInt32, mCurId, UInt32, mCompleteId, UInt32, mOldId);
    };

    struct UpdateAchievementBaseDataNotify
    {
        AchievementBaseData mBaseData;

        UpdateAchievementBaseDataNotify() {}

        LYNX_MESSAGE_1(UPDATE_ACHIEVEMENT_BASE_DATA_NOTIFY, UpdateAchievementBaseDataNotify, AchievementBaseData, mBaseData);
    };

    struct CompleteAchievementNotify
    {
        UInt32 mTemplateId;

        CompleteAchievementNotify(): mTemplateId(0) {}

        LYNX_MESSAGE_1(COMPLETE_ACHIEVEMENT_NOTIFY, CompleteAchievementNotify, UInt32, mTemplateId);
    };

    struct GetAchievementAwardReq
    {
        UInt32 mTemplateId;

        GetAchievementAwardReq(): mTemplateId(0) {}

        LYNX_MESSAGE_1(GET_ACHIEVEMENT_AWARD_REQ, GetAchievementAwardReq, UInt32, mTemplateId);
    };

    struct GetAchievementAwardResp
    {
        ErrorId mErrorId;
        UInt32 mTemplateId;
        bool mMaxLevel;

        GetAchievementAwardResp(): mErrorId(0), mTemplateId(0), mMaxLevel(false) {}

        LYNX_MESSAGE_3(GET_ACHIEVEMENT_AWARD_RESP, GetAchievementAwardResp, ErrorId, mErrorId, UInt32, mTemplateId, bool, mMaxLevel);
    };

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ACHIEVEMENT_MSG_H__