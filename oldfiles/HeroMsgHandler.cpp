#include "HeroMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void
HeroMsgHandler::onRefineHeroReq(const ConnId& connId, RefineHeroReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    RefineHeroResp resp;
    resp.mHeroId = msg.mHeroId;
    resp.mErrno = player->getHeroManager().refineHero(msg.mHeroId, msg.mAddLevel);
    player->sendMsgWithAck(resp);
}

void 
HeroMsgHandler::onAdvancedHeroReq(const ConnId& connId, AdvancedHeroReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    AdvancedHeroResp resp;
    resp.mHeroId = msg.mHeroId;
    resp.mErrno = player->getHeroManager().advancedHero(msg.mHeroId, msg.mHeroIdList, msg.mItemIdList);
    player->sendMsgWithAck(resp);
}

void 
HeroMsgHandler::onAwakenHeroReq(const ConnId& connId, AwakenHeroReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    AwakenHeroResp resp;
    resp.mHeroId = msg.mHeroId;
    resp.mErrno = player->getHeroManager().awakenHero(msg.mHeroId, msg.mHeroIdList, msg.mItemIdList);
    player->sendMsgWithAck(resp);
}

void 
HeroMsgHandler::onCombineHeroReq(const ConnId& connId, CombineHeroReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connId %llu", connId);
        return;
    }

    CombineHeroResp resp;
    resp.mErrno = player->getHeroDebrisManager().combineHero(msg.mHeroDebrisId, resp.mHeroId);
    player->sendMsgWithAck(resp);
}

void 
HeroMsgHandler::onExchangeHeroPresentPosReq(const ConnId& connId, ExchangeHeroPresentPosReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connId %llu", connId);
        return;
    }

    ExchangeHeroPresentPosResp resp;
    resp.mErrno = LynxErrno::None;
    HeroManager& heroManager = player->getHeroManager();

    for (List<HeroGuidPair>::Iter* iter = msg.mHeroGuidPairList.begin();
        iter != NULL; iter = msg.mHeroGuidPairList.next(iter))
    {
        const HeroData* hero1 = heroManager.getHeroDataByGuid(iter->mValue.mHeroGuid1);
        if (!hero1)
        {
            LOG_WARN("Failed to get hero %llu", iter->mValue.mHeroGuid1);
            resp.mErrno = LynxErrno::HeroNotExist;
            break;
        }

        const HeroData* hero2 = heroManager.getHeroDataByGuid(iter->mValue.mHeroGuid2);
        if (!hero2)
        {
            LOG_WARN("Failed to get hero %llu", iter->mValue.mHeroGuid2);
            resp.mErrno = LynxErrno::HeroNotExist;
            break;
        }

        UInt8 hero1Pos = hero1->mBaseData.mPresentPos;
        UInt8 hero2Pos = hero2->mBaseData.mPresentPos;

        if (hero1Pos < 1 || hero1Pos > 5)
        {
            LOG_WARN("Failed to change hero pos for hero pos invalid.");
            resp.mErrno = LynxErrno::InvalidParameter;
            break;
        }

        if (hero2Pos < 1 || hero2Pos > 5)
        {
            LOG_WARN("Failed to change hero pos for hero pos invalid.");
            resp.mErrno = LynxErrno::InvalidParameter;
            break;
        }

        if (hero1Pos == hero2Pos)
        {
            LOG_WARN("Failed to change hero pos for hero pos repeated.");
            resp.mErrno = LynxErrno::InvalidOperation;
            break;
        }
    }

    if (resp.mErrno != LynxErrno::None)
    {
        player->sendMsg(resp);
        return;
    }

    for (List<HeroGuidPair>::Iter* iter = msg.mHeroGuidPairList.begin();
        iter != NULL; iter = msg.mHeroGuidPairList.next(iter))
    {
        player->exchangeHeroPresentPos(iter->mValue.mHeroGuid1, iter->mValue.mHeroGuid2);
    }

    ExchangeHeroPresentPosNotify notify;
    notify.mHeroGuidPairList = msg.mHeroGuidPairList;
    player->sendMsgWithAck(notify);
    player->sendMsg(resp);
}

void 
HeroMsgHandler::onSetHeroPresentPosReq(const ConnId& connId, SetHeroPresentPosReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connId %llu", connId);
        return;
    }

    SetHeroPresentPosResp resp;
    resp.mHeroGuid = msg.mHeroGuid;
    resp.mErrno = player->setHeroPresentPos(msg.mHeroGuid, msg.mPresentPos);

    if (resp.mErrno == LynxErrno::None)
    {
        SetHeroPresentPosNotify notify;
        notify.mHeroGuid = msg.mHeroGuid;
        notify.mPresentPos = msg.mPresentPos;
        player->sendMsgWithAck(notify);
    }

    player->sendMsgWithAck(resp);
}

void 
HeroMsgHandler::onSellHeroReq(const ConnId& connId, SellHeroReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connId %llu", connId);
        return;
    }

    SellHeroResp resp;
    resp.mErrorId = player->getHeroManager().sellHero(msg.mSellList, resp.mGetGold);
    player->sendMsgWithAck(resp);
}
