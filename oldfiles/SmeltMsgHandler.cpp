#include "SmeltMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void 
SmeltMsgHandler::onSmeltReq(const ConnId& connId, SmeltReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    SmeltResp resp;
    resp.mErrno = LynxErrno::None;

    // 检查是否全部能熔炼成功
    for (List<Guid>::Iter* iter = msg.mHeroList.begin();
        iter != NULL; iter = msg.mHeroList.next(iter))
    {
        const HeroData* heroData = player->getHeroManager().getHeroDataByGuid(iter->mValue);
        if (!heroData)
        {
            LOG_WARN("Failed to get hero %llu", iter->mValue);
            resp.mErrno = LynxErrno::EquipNotExist;
            player->sendMsgWithAck(resp);
            return;
        }

        HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
        if (!heroTemplate)
        {
            LOG_WARN("Failed to get hero template %u", heroData->mBaseData.mTemplateId);
            resp.mErrno = LynxErrno::TemplateNotExist;
            player->sendMsgWithAck(resp);
            return;
        }
    }

    // 检查是否全部能熔炼成功
    for (List<Guid>::Iter* iter = msg.mEquipList.begin();
        iter != NULL; iter = msg.mEquipList.next(iter))
    {
        const EquipData* equipData = player->getEquipManager().getEquipDataByGuid(iter->mValue);
        if (!equipData)
        {
            LOG_WARN("Failed to get equip %llu", iter->mValue);
            resp.mErrno = LynxErrno::EquipNotExist;
            player->sendMsgWithAck(resp);
            return;
        }

        EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipData->mTemplateId);
        if (!equipTemplate)
        {
            LOG_WARN("Failed to get equip template %u", equipData->mTemplateId);
            resp.mErrno = LynxErrno::TemplateNotExist;
            player->sendMsgWithAck(resp);
            return;
        }
    }

    // 执行熔炼
    for (List<Guid>::Iter* iter = msg.mHeroList.begin();
        iter != NULL; iter = msg.mHeroList.next(iter))
    {
        player->getHeroManager().smeltHero(iter->mValue, resp.mGold, resp.mLevelExp, resp.mSoul);
    }

    for (List<Guid>::Iter* iter = msg.mEquipList.begin();
        iter != NULL; iter = msg.mEquipList.next(iter))
    {
        player->getEquipManager().smeltEquip(iter->mValue, resp.mGold, resp.mSoul);
    }

    player->sendMsgWithAck(resp);
}

void 
SmeltMsgHandler::onRebornHeroReq(const ConnId& connId, RebornHeroReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    RebornHeroResp resp;
    resp.mErrno = player->getHeroManager().rebornHero(msg.mHeroId, resp.mGold, resp.mLevelExp,
        resp.mHeroList, resp.mItemList);

    player->sendMsgWithAck(resp);
}

void 
SmeltMsgHandler::onRebornEquipReq(const ConnId& connId, RebornEquipReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    RebornEquipResp resp;
    resp.mErrno = player->getEquipManager().rebornEquip(msg.mEquipId, resp.mGold, resp.mItemList);

    player->sendMsgWithAck(resp);
}


