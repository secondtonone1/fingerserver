#include "GameRobot.h"

using namespace Lynx;

GameRobot::GameRobot(IOService* ioService) : TcpConnection(ioService), mLoginToken(0)
                                           , mIsConnecting(false)
{

}

GameRobot::~GameRobot()
{

}

bool 
GameRobot::initial(const String& accountName, const String& password,
                   const String& ip, UInt16 port)
{
    mAccountName = accountName;
    mPassword = password;
    mLoginIp = ip;
    mLoginPort = port;

    mUserData = &mMsgHdr;
    LYNX_REGISTER_CONNECTED(this, this, &GameRobot::onConnected);
    LYNX_REGISTER_CONNECT_FAILED(this, this, &GameRobot::onConnectFailed);
    LYNX_REGISTER_CONNECT_BROKEN(this, this, &GameRobot::onConnectBroken);
    mMsgHandler.initialize(this);

    registerMsgHandler();

    return true;
}

void 
GameRobot::release()
{

}

void 
GameRobot::update(const UInt64& accTime)
{
    // 检查网络是否断开， 断开就继续连接
    if (mIsConnecting == false)
    {
        asyncConnect(mLoginIp, mLoginPort);
        mIsConnecting = true;
    }
}

void 
GameRobot::registerMsgHandler()
{
    REGISTER_MSG(mMsgHandler, LoginSuccessResp, GameRobot::onLoginSuccessResp);
    REGISTER_MSG(mMsgHandler, LoginFailResp, GameRobot::onLoginFailResp);
    REGISTER_MSG(mMsgHandler, ReconnectSuccessResp, GameRobot::onReconnectSuccessResp);
    REGISTER_MSG(mMsgHandler, ReconnectFailResp, GameRobot::onReconnectFailResp);
    REGISTER_MSG(mMsgHandler, AccountWasLoginNotify, GameRobot::onAccountWasLoginNotify);
    //REGISTER_MSG(mMsgHandler, SyncPlayerChatNotify, GameRobot::onSyncPlayerChatNotify);
    REGISTER_MSG(mMsgHandler, SyncPlayerLevelNotify, GameRobot::onSyncPlayerLevelNotify);
    REGISTER_MSG(mMsgHandler, SyncPlayerLevelExpNotify, GameRobot::onSyncPlayerExpNotify);
    REGISTER_MSG(mMsgHandler, SyncPlayerEnergyNotify, GameRobot::onSyncPlayerEnergyNotify);
    REGISTER_MSG(mMsgHandler, SyncPlayerGoldNotify, GameRobot::onSyncPlayerGoldNotify);
    REGISTER_MSG(mMsgHandler, AddHeroNotify, GameRobot::onAddHeroNotify);
    REGISTER_MSG(mMsgHandler, DelHeroNotify, GameRobot::onDelHeroNotify);
    REGISTER_MSG(mMsgHandler, AddPetNotify, GameRobot::onAddPetNotify);
    REGISTER_MSG(mMsgHandler, DelPetNotify, GameRobot::onDelPetNotify);
   /* REGISTER_MSG(mMsgHandler, GetOneHeroSuccessResp, GameRobot::onGetOneHeroSuccessResp);
    REGISTER_MSG(mMsgHandler, GetOneHeroFailResp, GameRobot::onGetOneHeroFailResp);
    REGISTER_MSG(mMsgHandler, GetTenHeroSuccessResp, GameRobot::onGetTenHeroSuccessResp);
    REGISTER_MSG(mMsgHandler, GetTenHeroFailResp, GameRobot::onGetTenHeroFailResp);*/
    REGISTER_MSG(mMsgHandler, SyncHeroLevelExpNotify, GameRobot::onSyncHeroExpNotify);
    REGISTER_MSG(mMsgHandler, SyncHeroLevelNotify, GameRobot::onSyncHeroLevelNotify);
    REGISTER_MSG(mMsgHandler, SyncHeroAdvancedNotify, GameRobot::onSyncHeroAdvancedNotify);
    REGISTER_MSG(mMsgHandler, UpdateItemNotify, GameRobot::onUpdateItemNotify);
    REGISTER_MSG(mMsgHandler, AddItemNotify, GameRobot::onAddItemNotify);
    REGISTER_MSG(mMsgHandler, AddHeroDebrisNotify, GameRobot::onAddHeroDebrisNotify);
    REGISTER_MSG(mMsgHandler, UpdateHeroDebrisNotify, GameRobot::onUpdateHeroDebrisNotify);
    REGISTER_MSG(mMsgHandler, DelItemNotify, GameRobot::onDelItemNotify);
    REGISTER_MSG(mMsgHandler, DelHeroDebrisNotify, GameRobot::onDelHeroDebrisNotify);
    REGISTER_MSG(mMsgHandler, AddEquipNotify, GameRobot::onAddEquipNotify);
    REGISTER_MSG(mMsgHandler, AddRuneNotify, GameRobot::onAddRuneNotify);
    REGISTER_MSG(mMsgHandler, UpdateEquipRefineLevelNotify, GameRobot::onUpdateEquipRefineLevelNotify);
    REGISTER_MSG(mMsgHandler, UpdateEquipEnchantLevelNotify, GameRobot::onUpdateEquipEnchantLevelNotify);
    /*REGISTER_MSG(mMsgHandler, UpdateRuneRefineLevelNotify, GameRobot::onUpdateRuneRefineLevelNotify);*/
    REGISTER_MSG(mMsgHandler, DelEquipNotify, GameRobot::onDelEquipNotify);
    REGISTER_MSG(mMsgHandler, DelRuneNotify, GameRobot::onDelRuneNotify);
}

void 
GameRobot::onConnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_WARN("Connection Login [%s:%u] success.", conn->getFromIp().c_str(), conn->getFromPort());

    mIsConnecting = true;

    if (mLoginToken == 0)
    {
        LoginReq loginReq;
        loginReq.mAccountName = mAccountName;
        loginReq.mPassword = mPassword;
        sendMsg(loginReq);
    }
    else
    {
        ReconnectReq reconnectReq;
        reconnectReq.mPlayerGuid = mPlayerData.mBaseData.mPlayerId;
        reconnectReq.mLoginToken = mLoginToken;
        sendMsg(reconnectReq);
    }
}

void 
GameRobot::onConnectFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_WARN("Connection Login [%s:%u] failed.", conn->getFromIp().c_str(), conn->getFromPort());
    mIsConnecting = false;
}

void 
GameRobot::onConnectBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_WARN("Connecton Broken with server [%s:%u]", conn->getFromIp().c_str(), conn->getFromPort());
    mIsConnecting = false;
}

void 
GameRobot::calculatePlayerData(PlayerData& playerData)
{
    // 计算所有英雄属性
    for (List<HeroData>::Iter* iter = playerData.mHeroData.mHeroDataList.begin();
        iter != NULL; iter = playerData.mHeroData.mHeroDataList.next(iter))
    {
        HeroData& heroData = iter->mValue;
        // 计算血量
        heroData.mBaseData.mHp = calculateHeroHpUpper(playerData, heroData);
        // 计算怒气
        heroData.mBaseData.mAnger = 0;
        // 计算攻击力
        heroData.mExtendData.mAttack = calculateHeroAttack(playerData, heroData);
        // 计算物理防御力
        heroData.mExtendData.mPhysicsDefense = calculateHeroPhysicsDefense(playerData, heroData);
        // 计算魔法防御力
        heroData.mExtendData.mMagicDefense = calculateHeroMagicDefense(playerData, heroData);
        // 计算暴击
        heroData.mExtendData.mCritical = calculateHeroCritical(playerData, heroData);
        // 计算命中
        heroData.mExtendData.mHit = calculateHeroHit(playerData, heroData);
        // 计算闪避
        heroData.mExtendData.mDodge = calculateHeroDodge(playerData, heroData);
        // 计算血量上限
        heroData.mExtendData.mHpUpper = heroData.mBaseData.mHp;
        // 计算怒气上限
        heroData.mExtendData.mAngerUpper = calculateHeroAngerUpper(playerData, heroData);
        // 计算抗暴值
        heroData.mExtendData.mRescrit = calculateHeroResilience(playerData, heroData);
        // 计算耐力值
        heroData.mExtendData.mStamina = calculateHeroStamina(playerData, heroData);
        // 计算智力值
        heroData.mExtendData.mIntellect = calculateHeroIntellect(playerData, heroData);
        // 计算力量值
        heroData.mExtendData.mStrength = calculateHeroStrength(playerData, heroData);
    }

    // 计算玩家战力值
    playerData.mBaseData.mBattleValue = calculatePlayerBattleValue(playerData);
}

UInt32 
GameRobot::calculatePlayerBattleValue(const PlayerData& playerData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroHpUpper(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroAngerUpper(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroAttack(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroPhysicsDefense(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroMagicDefense(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroCritical(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroHit(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroDodge(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroResilience(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroStamina(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroIntellect(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

UInt32 
GameRobot::calculateHeroStrength(const PlayerData& playerData, const HeroData& heroData)
{
    return 0;
}

void 
GameRobot::onLoginSuccessResp(LoginSuccessResp& msg)
{
    mPlayerData = msg.mPlayerData;
    mLoginToken = msg.mLoginToken;
}

void 
GameRobot::onLoginFailResp(LoginFailResp& msg)
{
    LOG_WARN("Failed to login for errno: %u", msg.mErrno);
}

void 
GameRobot::onReconnectSuccessResp(ReconnectSuccessResp& msg)
{
    mLoginToken = msg.mLoginToken;
    LOG_INFO("Reconnect success.");
}

void 
GameRobot::onReconnectFailResp(ReconnectFailResp& msg)
{
    LOG_WARN("Failed to reconnect for errno: %u", msg.mErrno);
}

void
GameRobot::onAccountWasLoginNotify(AccountWasLoginNotify& msg)
{
    LOG_INFO("Account was login.");
}

//void
//GameRobot::onSyncPlayerChatNotify(SyncPlayerChatNotify& msg)
//{
//	LOG_DEBUG("Chat msg %s", msg.mContents.c_str());
//}

void 
GameRobot::closeConnection()
{
    close();
    mIsConnecting = false;
}

void 
GameRobot::sendChatMsg(const Guid& playerGuid, const String& contents)
{
   /* SyncPlayerChatNotify chatNotify;
    chatNotify.mTargetPlayerGuid = playerGuid;
    chatNotify.mContents = contents;
    sendMsg(chatNotify);*/
}

void 
GameRobot::sendGetOneHeroMsg()
{
    /*GetOneHeroReq req;
    sendMsg(req);*/
}

void 
GameRobot::sendGetTenHeroMsg()
{
   /* GetTenHeroReq req;
    sendMsg(req);*/
}

void 
GameRobot::onSyncPlayerLevelNotify(SyncPlayerLevelNotify& msg)
{
    LOG_INFO("Sync player level %u", msg.mLevel);
    mPlayerData.mBaseData.mLevel = msg.mLevel;
    calculatePlayerData(mPlayerData);
}

void 
GameRobot::onSyncPlayerExpNotify(SyncPlayerLevelExpNotify& msg)
{
    LOG_INFO("Sync player exp %u", msg.mLevelExp);
    mPlayerData.mBaseData.mLevelExp = msg.mLevelExp;
    calculatePlayerData(mPlayerData);
}

void 
GameRobot::onSyncPlayerEnergyNotify(SyncPlayerEnergyNotify& msg)
{
    LOG_INFO("Sync player energy %u", msg.mEnergy);
    mPlayerData.mBaseData.mEnergy = msg.mEnergy;
}

void 
GameRobot::onSyncPlayerGoldNotify(SyncPlayerGoldNotify& msg)
{
    LOG_INFO("Sync player gold %u", msg.mGold);
    mPlayerData.mBaseData.mGold = msg.mGold;
}

void 
GameRobot::onAddHeroNotify(AddHeroNotify& msg)
{
    LOG_INFO("Player add a new hero");
    mPlayerData.mHeroData.mHeroDataList.insertTail(msg.mHeroData);
}

void 
GameRobot::onDelHeroNotify(DelHeroNotify& msg)
{
    LOG_INFO("Player del a old hero");
    for (List<HeroData>::Iter* iter = mPlayerData.mHeroData.mHeroDataList.begin();
        iter != NULL; iter = mPlayerData.mHeroData.mHeroDataList.next(iter))
    {
        if (msg.mHeroGuid == iter->mValue.mBaseData.mHeroId)
        {
            mPlayerData.mHeroData.mHeroDataList.erase(iter);
            calculatePlayerData(mPlayerData);
            break;
        }
    }
}

void 
GameRobot::onAddPetNotify(AddPetNotify& msg)
{
    LOG_INFO("Player add a new pet");
    mPlayerData.mPetData.mPetDataList.insertTail(msg.mPetData);
    calculatePlayerData(mPlayerData);
}

void 
GameRobot::onDelPetNotify(DelPetNotify& msg)
{
    LOG_INFO("Player del a old pet");
    for (List<PetData>::Iter* iter = mPlayerData.mPetData.mPetDataList.begin();
        iter != NULL; iter = mPlayerData.mPetData.mPetDataList.next(iter))
    {
        if (msg.mPetGuid == iter->mValue.mBaseData.mPetId)
        {
            mPlayerData.mPetData.mPetDataList.erase(iter);
            calculatePlayerData(mPlayerData);
            break;
        }
    }
}

//void 
//GameRobot::onGetOneHeroSuccessResp(GetOneHeroSuccessResp& msg)
//{
//    LOG_INFO("Get a new hero success.");
//
//    calculatePlayerData(mPlayerData);
//}
//
//void 
//GameRobot::onGetOneHeroFailResp(GetOneHeroFailResp& msg)
//{
//    LOG_INFO("Get a new hero failed.");
//}
//
//void 
//GameRobot::onGetTenHeroSuccessResp(GetTenHeroSuccessResp& msg)
//{
//    LOG_INFO("Get ten hero success.");
//
//    calculatePlayerData(mPlayerData);
//}
//
//void 
//GameRobot::onGetTenHeroFailResp(GetTenHeroFailResp& msg)
//{
//    LOG_INFO("Get ten hero failed.");
//}

void 
GameRobot::onSyncHeroExpNotify(SyncHeroLevelExpNotify& msg)
{
    LOG_INFO("Sync hero exp %llu %u", msg.mHeroId, msg.mLevelExp);
    for (List<HeroData>::Iter* iter = mPlayerData.mHeroData.mHeroDataList.begin();
        iter != NULL; iter = mPlayerData.mHeroData.mHeroDataList.next(iter))
    {
        if (msg.mHeroId == iter->mValue.mBaseData.mHeroId)
        {
            iter->mValue.mBaseData.mLevelExp = msg.mLevelExp;
            calculatePlayerData(mPlayerData);
            break;
        }
    }
}

void 
GameRobot::onSyncHeroAdvancedNotify(SyncHeroAdvancedNotify& msg)
{
    LOG_INFO("Sync hero advanced %llu %u", msg.mHeroId, msg.mAdvanced);
    for (List<HeroData>::Iter* iter = mPlayerData.mHeroData.mHeroDataList.begin();
        iter != NULL; iter = mPlayerData.mHeroData.mHeroDataList.next(iter))
    {
        if (msg.mHeroId == iter->mValue.mBaseData.mHeroId)
        {
            iter->mValue.mBaseData.mAdvanced = msg.mAdvanced;
            calculatePlayerData(mPlayerData);
            break;
        }
    }
}

void 
GameRobot::onSyncHeroLevelNotify(SyncHeroLevelNotify& msg)
{
    LOG_INFO("Sync hero level %llu %u", msg.mHeroId, msg.mLevel);
    for (List<HeroData>::Iter* iter = mPlayerData.mHeroData.mHeroDataList.begin();
        iter != NULL; iter = mPlayerData.mHeroData.mHeroDataList.next(iter))
    {
        if (msg.mHeroId == iter->mValue.mBaseData.mHeroId)
        {
            iter->mValue.mBaseData.mLevel = msg.mLevel;
            calculatePlayerData(mPlayerData);
            break;
        }
    }
}

void 
GameRobot::onUpdateItemNotify(UpdateItemNotify& msg)
{
    LOG_INFO("Update item %llu", msg.mItemGuid);
    for (List<ItemData>::Iter* it = mPlayerData.mItemData.mItemDataList.begin();
        it != NULL; it = mPlayerData.mItemData.mItemDataList.next(it))
    {
        if (msg.mItemGuid == it->mValue.mItemId)
        {
            it->mValue.mCount = msg.mCount;
            break;
        }
    }
}

void 
GameRobot::onAddItemNotify(AddItemNotify& msg)
{
    LOG_INFO("Add item %llu", msg.mItemData.mItemId);
    mPlayerData.mItemData.mItemDataList.insertTail(msg.mItemData);
}

void 
GameRobot::onAddHeroDebrisNotify(AddHeroDebrisNotify& msg)
{
    LOG_INFO("Add hero debris %llu", msg.mHeroDebrisData.mHeroDebrisId);
    mPlayerData.mHeroDebrisData.mHeroDebrisList.insertTail(msg.mHeroDebrisData);
}

void 
GameRobot::onUpdateHeroDebrisNotify(UpdateHeroDebrisNotify& msg)
{
    LOG_INFO("Update hero debris %llu", msg.mHeroDebrisGuid);

    for (List<HeroDebrisData>::Iter* iter = mPlayerData.mHeroDebrisData.mHeroDebrisList.begin();
        iter != NULL; iter = mPlayerData.mHeroDebrisData.mHeroDebrisList.next(iter))
    {
        if (iter->mValue.mHeroDebrisId == msg.mHeroDebrisGuid)
        {
            iter->mValue.mCount = msg.mCount;
        }
    }
}

void 
GameRobot::onDelItemNotify(DelItemNotify& msg)
{
    LOG_INFO("Del item %llu", msg.mItemGuid);
    for (List<ItemData>::Iter* iter = mPlayerData.mItemData.mItemDataList.begin();
        iter != NULL; iter = mPlayerData.mItemData.mItemDataList.next(iter))
    {
        if (iter->mValue.mItemId == msg.mItemGuid)
        {
            mPlayerData.mItemData.mItemDataList.erase(iter);
            break;
        }
    }
}

void 
GameRobot::onDelHeroDebrisNotify(DelHeroDebrisNotify& msg)
{
    LOG_INFO("Del hero debris %llu", msg.mHeroDebrisGuid);
    for (List<HeroDebrisData>::Iter* iter = mPlayerData.mHeroDebrisData.mHeroDebrisList.begin();
        iter != NULL; iter = mPlayerData.mHeroDebrisData.mHeroDebrisList.next(iter))
    {
        if (iter->mValue.mHeroDebrisId == msg.mHeroDebrisGuid)
        {
            mPlayerData.mHeroDebrisData.mHeroDebrisList.erase(iter);
            break;
        }
    }
}

void 
GameRobot::onAddEquipNotify(AddEquipNotify& msg)
{
    LOG_INFO("Add equip %llu", msg.mEquipData.mEquipId);

    mPlayerData.mEquipData.mEquipDataList.insertTail(msg.mEquipData);
}

void 
GameRobot::onAddRuneNotify(AddRuneNotify& msg)
{
    LOG_INFO("Add rune %llu", msg.mRuneData.mRuneId);

    mPlayerData.mRuneData.mRuneDataList.insertTail(msg.mRuneData);
}

void 
GameRobot::onUpdateEquipRefineLevelNotify(UpdateEquipRefineLevelNotify& msg)
{
    LOG_INFO("Update equip refine level %llu", msg.mEquipId);

    for (List<EquipData>::Iter* iter = mPlayerData.mEquipData.mEquipDataList.begin();
        iter != NULL; iter = mPlayerData.mEquipData.mEquipDataList.next(iter))
    {
        if (iter->mValue.mEquipId == msg.mEquipId)
        {
            iter->mValue.mRefineLevel = msg.mRefineLevel;
        }
    }
}

void 
GameRobot::onUpdateEquipEnchantLevelNotify(UpdateEquipEnchantLevelNotify& msg)
{
    LOG_INFO("Update equip enchant level %llu", msg.mEquipId);

    for (List<EquipData>::Iter* iter = mPlayerData.mEquipData.mEquipDataList.begin();
        iter != NULL; iter = mPlayerData.mEquipData.mEquipDataList.next(iter))
    {
        if (iter->mValue.mEquipId == msg.mEquipId)
        {
            iter->mValue.mEnchantLevel = msg.mEnchantLevel;
        }
    }
}

//void 
//GameRobot::onUpdateRuneRefineLevelNotify(UpdateRuneRefineLevelNotify& msg)
//{
//    LOG_INFO("Update rune refine level %llu", msg.mRuneId);
//
//    for (List<RuneData>::Iter* iter = mPlayerData.mRuneData.mRuneDataList.begin();
//        iter != NULL; iter = mPlayerData.mRuneData.mRuneDataList.next(iter))
//    {
//        if (iter->mValue.mRuneId == msg.mRuneId)
//        {
//            iter->mValue.mRefineLevel = msg.mRefineLevel;
//        }
//    }
//}

void 
GameRobot::onDelEquipNotify(DelEquipNotify& msg)
{
    LOG_INFO("Del equip %llu", msg.mEquipGuid);

    for (List<EquipData>::Iter* iter = mPlayerData.mEquipData.mEquipDataList.begin();
        iter != NULL; iter = mPlayerData.mEquipData.mEquipDataList.next(iter))
    {
        if (iter->mValue.mEquipId == msg.mEquipGuid)
        {
            mPlayerData.mEquipData.mEquipDataList.erase(iter);
            break;
        }
    }
}

void 
GameRobot::onDelRuneNotify(DelRuneNotify& msg)
{
    LOG_INFO("Del rune %llu", msg.mRuneGuid);

    for (List<RuneData>::Iter* iter = mPlayerData.mRuneData.mRuneDataList.begin();
        iter != NULL; iter = mPlayerData.mRuneData.mRuneDataList.next(iter))
    {
        if (iter->mValue.mRuneId == msg.mRuneGuid)
        {
            mPlayerData.mRuneData.mRuneDataList.erase(iter);
            break;
        }
    }
}

