#ifndef __LYNX_COMMON_LIB_PLAYER_MSG_H__
#define __LYNX_COMMON_LIB_PLAYER_MSG_H__

#include "Data.h"

namespace Lynx
{
    enum
    {
        SYNC_PLAYER_LEVEL_NOTIFY                = sPlayerMsgBase + 1,
        SYNC_PLAYER_EXP_NOTIFY                  = sPlayerMsgBase + 2,
        SYNC_PLAYER_ENERGY_NOTIFY               = sPlayerMsgBase + 3,
        SYNC_PLAYER_GOLD_NOTIFY                 = sPlayerMsgBase + 4,
        SYNC_PLAYER_HERO_EXP_POOL_NOTIFY        = sPlayerMsgBase + 5,
        SYNC_PLAYER_RMB_NOTIFY                  = sPlayerMsgBase + 6,
        SYNC_PLAYER_SOUL_NOTIFY                 = sPlayerMsgBase + 7,
        SYNC_PLAYER_HONOR_NOTIFY                = sPlayerMsgBase + 8,

        UPGREAD_GOLD_MINERAL_REQ                = sPlayerMsgBase + 50,
        UPGREAD_GOLD_MINERAL_RESP               = sPlayerMsgBase + 51,
        UPSPEED_GOLD_MINERAL_REQ                = sPlayerMsgBase + 52,
        UPSPEED_GOLD_MINERAL_RESP               = sPlayerMsgBase + 53,
        UPGREAD_DIAMOND_MINERAL_REQ             = sPlayerMsgBase + 54,
        UPGREAD_DIAMOND_MINERAL_RESP            = sPlayerMsgBase + 55,
        UPSPEED_DIAMOND_MINERAL_REQ             = sPlayerMsgBase + 56,
        UPSPEED_DIAMOND_MINERAL_RESP            = sPlayerMsgBase + 57,
        UPGREAD_EXP_MINERAL_REQ                 = sPlayerMsgBase + 58,
        UPGREAD_EXP_MINERAL_RESP                = sPlayerMsgBase + 59,
        GET_GOLD_MINERAL_REQ                    = sPlayerMsgBase + 60,
        GET_GOLD_MINERAL_RESP                   = sPlayerMsgBase + 61,
        GET_DIAMOND_MINERAL_REQ                 = sPlayerMsgBase + 62,
        GET_DIAMOND_MINERAL_RESP                = sPlayerMsgBase + 63,
        UPSPEED_EXP_MINERAL_REQ                 = sPlayerMsgBase + 64,
        UPSPEED_EXP_MINERAL_RESP                = sPlayerMsgBase + 65,

        VIEW_PLAYER_INFO_REQ                    = sPlayerMsgBase + 100,
        VIEW_PLAYER_INFO_RESP                   = sPlayerMsgBase + 101,
        SUMMON_HERO_REQ                         = sPlayerMsgBase + 102,
        SUMMON_HERO_RESP                        = sPlayerMsgBase + 103,
    };

    struct SyncPlayerLevelNotify
    {
        SyncPlayerLevelNotify() : mLevel(0) {}

        UInt32 mLevel;

        LYNX_MESSAGE_1(SYNC_PLAYER_LEVEL_NOTIFY, SyncPlayerLevelNotify,
            UInt32, mLevel);
    };

    struct SyncPlayerLevelExpNotify
    {
        SyncPlayerLevelExpNotify() : mLevelExp(0) {}

        UInt32 mLevelExp;

        LYNX_MESSAGE_1(SYNC_PLAYER_EXP_NOTIFY, SyncPlayerLevelExpNotify, UInt32, mLevelExp);
    };

    struct SyncPlayerEnergyNotify
    {
        SyncPlayerEnergyNotify() : mEnergy(0) {}

        UInt32 mEnergy;

        LYNX_MESSAGE_1(SYNC_PLAYER_ENERGY_NOTIFY, SyncPlayerEnergyNotify,
            UInt32, mEnergy);
    };

    struct SyncPlayerGoldNotify
    {
        SyncPlayerGoldNotify() : mGold(0) {}

        UInt32 mGold;

        LYNX_MESSAGE_1(SYNC_PLAYER_GOLD_NOTIFY, SyncPlayerGoldNotify,
            UInt32, mGold);
    };

    struct SyncPlayerRmbNotify
    {
        SyncPlayerRmbNotify() : mRmb(0) {}

        UInt32 mRmb;

        LYNX_MESSAGE_1(SYNC_PLAYER_RMB_NOTIFY, SyncPlayerRmbNotify,
            UInt32, mRmb);
    };

    struct SyncPlayerHeroExpPoolNotify
    {
        SyncPlayerHeroExpPoolNotify() : mHeroExpPool(0) {}

        UInt32 mHeroExpPool;

        LYNX_MESSAGE_1(SYNC_PLAYER_HERO_EXP_POOL_NOTIFY, SyncPlayerHeroExpPoolNotify,
            UInt32, mHeroExpPool);
    };

    struct SyncPlayerSoulNotify
    {
        SyncPlayerSoulNotify() : mSoul(0) {}

        UInt32 mSoul;

        LYNX_MESSAGE_1(SYNC_PLAYER_SOUL_NOTIFY, SyncPlayerSoulNotify,
            UInt32, mSoul);
    };

    struct SyncPlayerHonorNotify
    {
        SyncPlayerHonorNotify() : mHonor(0) {}

        UInt32 mHonor;

        LYNX_MESSAGE_1(SYNC_PLAYER_HONOR_NOTIFY, SyncPlayerHonorNotify,
            UInt32, mHonor);
    };

    struct UpgreadGoldMineralReq
    {
        UpgreadGoldMineralReq() {}

        LYNX_MESSAGE_0(UPGREAD_GOLD_MINERAL_REQ, UpgreadGoldMineralReq);
    };

    struct UpgreadGoldMineralResp
    {
        UpgreadGoldMineralResp() : mErrno(LynxErrno::None), mGoldMineralLevel(0), mRemainGoldMineralCount(0),
            mCalculateTime(0) {}

        ErrorId mErrno;
        UInt32 mGoldMineralLevel;
        UInt32 mRemainGoldMineralCount;
        UInt32 mCalculateTime;

        LYNX_MESSAGE_4(UPGREAD_GOLD_MINERAL_RESP, UpgreadGoldMineralResp,
            ErrorId, mErrno, UInt32, mGoldMineralLevel, UInt32, mRemainGoldMineralCount,
            UInt32, mCalculateTime);
    };

    struct UpspeedGoldMineralReq
    {
        UpspeedGoldMineralReq() {}

        LYNX_MESSAGE_0(UPSPEED_GOLD_MINERAL_REQ, UpspeedGoldMineralReq);
    };

    struct UpspeedGoldMineralResp
    {
        UpspeedGoldMineralResp() : mErrno(LynxErrno::None), mGoldMineralSpeedUpTime(0),
            mGoldMineralStartSpeedTime(0) {}

        ErrorId mErrno;
        UInt32 mGoldMineralSpeedUpTime;
        UInt32 mGoldMineralStartSpeedTime;

        LYNX_MESSAGE_3(UPSPEED_GOLD_MINERAL_RESP, UpspeedGoldMineralResp,
            ErrorId, mErrno, UInt32, mGoldMineralSpeedUpTime, UInt32, mGoldMineralStartSpeedTime);
    };

    struct UpgreadDiamondMineralReq
    {
        UpgreadDiamondMineralReq() {}

        LYNX_MESSAGE_0(UPGREAD_DIAMOND_MINERAL_REQ, UpgreadDiamondMineralReq);
    };

    struct UpgreadDiamondMineralResp
    {
        UpgreadDiamondMineralResp() : mErrno(LynxErrno::None), mDiamondMineralLevel(0),
            mRemainDiamondMineralCount(0), mCalculateTime(0) {}

        ErrorId mErrno;
        UInt32 mDiamondMineralLevel;
        UInt32 mRemainDiamondMineralCount;
        UInt32 mCalculateTime;

        LYNX_MESSAGE_4(UPGREAD_DIAMOND_MINERAL_RESP, UpgreadDiamondMineralResp,
            ErrorId, mErrno, UInt32, mDiamondMineralLevel, UInt32, mRemainDiamondMineralCount,
            UInt32, mCalculateTime);
    };

    struct UpspeedDiamondMineralReq
    {
        UpspeedDiamondMineralReq() {}

        LYNX_MESSAGE_0(UPSPEED_DIAMOND_MINERAL_REQ, UpspeedDiamondMineralReq);
    };

    struct UpspeedDiamondMineralResp
    {
        UpspeedDiamondMineralResp() : mErrno(LynxErrno::None), mDiamondMineralSpeedUpTime(0),
            mDiamondMineralStartSpeedTime(0) {}

        ErrorId mErrno;
        UInt32 mDiamondMineralSpeedUpTime;
        UInt32 mDiamondMineralStartSpeedTime;

        LYNX_MESSAGE_3(UPSPEED_DIAMOND_MINERAL_RESP, UpspeedDiamondMineralResp,
            ErrorId, mErrno, UInt32, mDiamondMineralSpeedUpTime, UInt32, mDiamondMineralStartSpeedTime);
    };

    struct UpgreadExpMineralReq
    {
        UpgreadExpMineralReq() {}

        LYNX_MESSAGE_0(UPGREAD_EXP_MINERAL_REQ, UpgreadExpMineralReq);
    };

    struct UpgreadExpMineralResp
    {
        UpgreadExpMineralResp() : mErrno(LynxErrno::None), mExpMineralLevel(0),
            mRemainExpMineralCount(0), mCalculateTime(0) {}

        ErrorId mErrno;
        UInt32 mExpMineralLevel;
        UInt32 mRemainExpMineralCount;
        UInt32 mCalculateTime;

        LYNX_MESSAGE_4(UPGREAD_EXP_MINERAL_RESP, UpgreadExpMineralResp,
            ErrorId, mErrno, UInt32, mExpMineralLevel, UInt32, mRemainExpMineralCount,
            UInt32, mCalculateTime);
    };

    struct UpspeedExpMineralReq
    {
        UpspeedExpMineralReq() {}

        LYNX_MESSAGE_0(UPSPEED_EXP_MINERAL_REQ, UpspeedExpMineralReq);
    };

    struct UpspeedExpMineralResp
    {
        UpspeedExpMineralResp(): mErrorId(LynxErrno::None), mExpMineralSpeedUpTime(0),
            mExpMineralStartSpeedTime(0) {}

        ErrorId mErrorId;
        UInt32 mExpMineralSpeedUpTime;
        UInt32 mExpMineralStartSpeedTime;

        LYNX_MESSAGE_3(UPSPEED_EXP_MINERAL_RESP, UpspeedExpMineralResp,
            ErrorId, mErrorId, UInt32, mExpMineralSpeedUpTime, UInt32, mExpMineralStartSpeedTime);
    };

    struct GetGoldMineralReq
    {
        GetGoldMineralReq() {}

        LYNX_MESSAGE_0(GET_GOLD_MINERAL_REQ, GetGoldMineralReq);
    };

    struct GetGoldMineralResp
    {
        GetGoldMineralResp() : mErrno(LynxErrno::None), mGoldMineralCount(0),
            mCalculateTime(0) {}

        ErrorId mErrno;
        UInt32 mGoldMineralCount;
        UInt32 mCalculateTime;

        LYNX_MESSAGE_3(GET_GOLD_MINERAL_RESP, GetGoldMineralResp,
            ErrorId, mErrno, UInt32, mGoldMineralCount, UInt32, mCalculateTime);
    };

    struct GetDiamondMineralReq
    {
        GetDiamondMineralReq() {}

        LYNX_MESSAGE_0(GET_DIAMOND_MINERAL_REQ, GetDiamondMineralReq);
    };

    struct GetDiamondMineralResp
    {
        GetDiamondMineralResp() : mErrno(LynxErrno::None), mDiamondMineralCount(0),
            mCalculateTime(0) {}

        ErrorId mErrno;
        UInt32 mDiamondMineralCount;
        UInt32 mCalculateTime;

        LYNX_MESSAGE_3(GET_DIAMOND_MINERAL_RESP, GetDiamondMineralResp,
            ErrorId, mErrno, UInt32, mDiamondMineralCount, UInt32, mCalculateTime);
    };

    struct ViewPlayerInfoReq
    {
        ViewPlayerInfoReq() : mPlayerGuid(0) {}

        Guid mPlayerGuid;

        LYNX_MESSAGE_1(VIEW_PLAYER_INFO_REQ, ViewPlayerInfoReq, Guid, mPlayerGuid);
    };

    struct ViewPlayerInfoResp
    {
        ViewPlayerInfoResp() : mErrno(0), mPlayerGuid(0), mPlayerName(""), 
            mPlayerIconName(""), mPlayerLevel(0), mPlayerVipLevel(0), mPlayerBattleValue(0) {}

        ErrorId mErrno;

        Guid mPlayerGuid;
        NameString mPlayerName;
        IconString mPlayerIconName;
        UInt32 mPlayerLevel;
        UInt8 mPlayerVipLevel;
        UInt32 mPlayerBattleValue;

        PlayerHeroData mPlayerHeroData;
      /*  PlayerPetData mPlayerPetData;
        PlayerEquipData mPlayerEquipData;*/
        PlayerRuneData mPlayerRuneData;

        LYNX_MESSAGE_11(VIEW_PLAYER_INFO_RESP, ViewPlayerInfoResp, Guid, mPlayerGuid,
            ErrorId, mErrno, NameString, mPlayerName, UInt32, mPlayerLevel, UInt8, mPlayerVipLevel,
            PlayerHeroData, mPlayerHeroData, PlayerPetData, mPlayerPetData, IconString, mPlayerIconName, 
            PlayerEquipData, mPlayerEquipData, PlayerRuneData, mPlayerRuneData, UInt32, mPlayerBattleValue);
    };

    struct SummonHeroReq
    {
        SummonHeroReq() : mType(0) {}

        UInt8 mType; // 0 普通道具抽， 1 免费钻石抽， 2 钻石抽， 3 钻石十连抽

        LYNX_MESSAGE_1(SUMMON_HERO_REQ, SummonHeroReq, UInt8, mType);
    };

    struct SummonHeroResp
    {
        SummonHeroResp() : mErrno(0), mType(0) {}

        ErrorId mErrno;
        UInt8 mType; // 0 普通道具抽， 1 免费钻石抽， 2 钻石抽， 3 钻石十连抽
        List<Guid> mHeroGuidList;

        LYNX_MESSAGE_3(SUMMON_HERO_RESP, SummonHeroResp, ErrorId, mErrno,
            UInt8, mType, List<Guid>, mHeroGuidList);
    };

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_PLAYER_MSG_H__