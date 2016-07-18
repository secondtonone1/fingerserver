#include "GMMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void
GMMsgHandler::onGMLoginReq(const ConnId& connId, GMLoginReq& msg)
{
    PassportGMAuthReq authReq;
    authReq.mConnId = connId;
    authReq.mAccountName = msg.mAccountName;
    authReq.mAccountPassword = msg.mAccountPassword;
    authReq.mFromIp = NetworkSystem::getSingleton().getClientConnectionFromIp(connId);
    authReq.mFromPort = NetworkSystem::getSingleton().getClientConnectionFromPort(connId);
    PassportSystem::getSingleton().postThreadMsg(authReq, 0);
}

void 
GMMsgHandler::onGMGetPlayerByOnlineReq(const ConnId& connId, GMGetPlayerByOnlineReq& msg)
{
    GMGetPlayerByOnlineResp resp;
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    const Map<ConnId, Player*>& playerConnectionMap = LogicSystem::getSingleton().getPlayerConnectionMap();

    for (Map<ConnId, Player*>::ConstIter* iter = playerConnectionMap.begin();
        iter != NULL; iter = playerConnectionMap.next(iter))
    {
        Player* player = iter->mValue;
        if (player)
        {
            resp.mPlayerList.insertTail(player->getPlayerData().mBaseData);
        }
    }

    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMGetPlayerByPlayerGuidReq(const ConnId& connId, GMGetPlayerByPlayerGuidReq& msg)
{
    GMGetPlayerByPlayerGuidResp resp;
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByGuid(msg.mPlayerGuid);
    if (player)
    {
        resp.mErrno = LynxErrno::None;
        PlayerBaseData playerBaseData;
        List<PlayerBaseData>::Iter* iter = resp.mPlayerDataList.insertTail(playerBaseData);
        iter->mValue = player->getPlayerData().mBaseData;
    }
    else
    {
        PersistLoadOffLinePlayerBaseDataReq req;
        req.mPlayerGuid = msg.mPlayerGuid;
        req.mConnId = connId;
        PersistSystem::getSingleton().postThreadMsg(req, connId);
        return;
    }

    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMGetPlayerByPlayerNameReq(const ConnId& connId, GMGetPlayerByPlayerNameReq& msg)
{
    GMGetPlayerByPlayerGuidResp resp;
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (player)
    {
        resp.mErrno = LynxErrno::None;
        PlayerBaseData playerBaseData;
        List<PlayerBaseData>::Iter* iter = resp.mPlayerDataList.insertTail(playerBaseData);
        iter->mValue = player->getPlayerData().mBaseData;
    }
    else
    {
        PersistLoadOffLinePlayerBaseDataReq req;
        req.mPlayerName = msg.mPlayerName;
        req.mConnId = connId;
        PersistSystem::getSingleton().postThreadMsg(req, connId);
        return;
    }

    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMGetPlayerByAccountGuidReq(const ConnId& connId, GMGetPlayerByAccountGuidReq& msg)
{
    GMGetPlayerByAccountGuidResp resp;
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByAccountGuid(msg.mAccountGuid);
    if (player)
    {
        resp.mErrno = LynxErrno::None;
        PlayerBaseData playerBaseData;
        List<PlayerBaseData>::Iter* iter = resp.mPlayerDataList.insertTail(playerBaseData);
        iter->mValue = player->getPlayerData().mBaseData;
    }
    else
    {
        PersistLoadOffLinePlayerBaseDataReq req;
        req.mAccountGuid = msg.mAccountGuid;
        req.mConnId = connId;
        PersistSystem::getSingleton().postThreadMsg(req, connId);
        return;
    }

    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMGetPlayerByAccountNameReq(const ConnId& connId, GMGetPlayerByAccountNameReq& msg)
{
    GMGetPlayerByAccountNameResp resp;
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByAccountName(msg.mAccountName);
    if (player)
    {
        resp.mErrno = LynxErrno::None;
        PlayerBaseData playerBaseData;
        List<PlayerBaseData>::Iter* iter = resp.mPlayerDataList.insertTail(playerBaseData);
        iter->mValue = player->getPlayerData().mBaseData;
    }
    else
    {
        PersistLoadOffLinePlayerBaseDataReq req;
        req.mAccountName = msg.mAccountName;
        req.mConnId = connId;
        PersistSystem::getSingleton().postThreadMsg(req, connId);
        return;
    }

    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMGetPlayerDataReq(const ConnId& connId, GMGetPlayerDataReq& msg)
{
    GMGetPlayerDataResp resp;
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByGuid(msg.mPlayerGuid);
    if (player)
    {
        resp.mErrno = LynxErrno::None;
        PlayerData playerData;
        List<PlayerData>::Iter* iter = resp.mPlayerDataList.insertTail(playerData);
        iter->mValue = player->getPlayerData();
    }
    else
    {
        PersistLoadOffLinePlayerDataReq req;
        req.mPlayerGuid = msg.mPlayerGuid;
        req.mConnId = connId;
        PersistSystem::getSingleton().postThreadMsg(req, connId);
        return;
    }

    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetPlayerExpReq(const ConnId& connId, GMSetPlayerExpReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    UInt32 currentExp = player->getPlayerExp();
    if (msg.mLevelExp > currentExp)
    {
        player->increasePlayerLevelExp(msg.mLevelExp - currentExp);
    }
    else
    {
        player->reducePlayerLevelExp(currentExp - msg.mLevelExp);
    }
    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMSetPlayerLevelReq(const ConnId& connId, GMSetPlayerLevelReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    UInt32 currentLevel = player->getPlayerLeval();
    if (msg.mLevel > currentLevel)
    {
        player->increasePlayerLevel(msg.mLevel - currentLevel);
    }
    else
    {
        player->reducePlayerLevel(currentLevel - msg.mLevel);
    }
    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetPlayerEnergyReq(const ConnId& connId, GMSetPlayerEnergyReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

	//字段没有精力值字段，先注释 sec
   /* UInt32 currentEnergy = player->getPlayerEnergy();
    if (msg.mEnergy > currentEnergy)
    {
        player->increasePlayerEnergy(msg.mEnergy - currentEnergy);
    }
    else
    {
        player->reducePlayerEnergy(currentEnergy - msg.mEnergy);
    }*/
    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetPlayerGoldReq(const ConnId& connId, GMSetPlayerGoldReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    UInt32 currentGold = player->getPlayerGold();
    if (msg.mGold > currentGold)
    {
        player->increasePlayerGold(msg.mGold - currentGold);
    }
    else
    {
        player->reducePlayerGold(currentGold - msg.mGold);
    }
    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMAddPetDebrisReq(const ConnId& connId, GMAddPetDebrisReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(msg.mDebrisTemplateId);
    if (!petDebrisTemplate)
    {
        LOG_DEBUG("failed to get petDebrisTemplate by tempalteid %lu", msg.mDebrisTemplateId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 无效的模板id";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }
    player->getPetDebrisManager().createPetDebris(msg.mDebrisTemplateId, msg.mCount, ITEM_SOURCE_GM_COMMAND);

    GMCommandResp resp;
    resp.mContents = "执行指令成功!";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMAddItemReq(const ConnId& connId, GMAddItemReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }
   
    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    ItemTemplate* itemTemplate = ITEM_TABLE().get(msg.mItemTemplateId);
    if (!itemTemplate)
    {
        LOG_DEBUG("failed to get itemtemplate by tempalteid %lu", msg.mItemTemplateId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 无效的道具模板id";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }
    
    player->getItemManager().createItem(msg.mItemTemplateId, msg.mCount, ITEM_SOURCE_GM_COMMAND);

    GMCommandResp resp;
    resp.mContents = "执行指令成功!";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMDelPetDebrisReq(const ConnId& connId, GMDelPetDebrisReq& msg)
{
    //PlayerData 的mPetDebrisData 字段注释
	/*GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    const PlayerData& playerData = player->getPlayerData();
    for (List<PetDebrisData>::Iter* it = playerData.mPetDebrisData.mPetDebrisList.begin();
        it != playerData.mPetDebrisData.mPetDebrisList.end();
        it = playerData.mPetDebrisData.mPetDebrisList.next(it))
    {
        if (it->mValue.mTemplateId == msg.mDebrisTemplateId)
        {
            player->getPetDebrisManager().delPetDebris(it->mValue.mPetDebrisId, msg.mCount);

            GMCommandResp resp;
            resp.mContents = "执行指令成功!";
            NetworkSystem::getSingleton().sendMsg(resp, connId);
            return;
        }
    }

    GMCommandResp resp;
    resp.mContents = "执行指令失败, 道具[";
    resp.mContents += msg.mDebrisTemplateId + "]不存在";
    NetworkSystem::getSingleton().sendMsg(resp, connId);*/
}

void 
GMMsgHandler::onGMSendSystemNoticeReq(const ConnId& connId, GMSendSystemNoticeReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    if (msg.mCycleCount > 3)
    {
        LOG_WARN("Failed to exec send horse lamp cmd for cycle count > 3.");
        return;
    }

    for (UInt32 i = 0; i < msg.mCycleCount; ++i)
    {
        LogicSystem::getSingleton().broadcaseSystemNotice(msg.mContents, 1);
    }
}

void
GMMsgHandler::onGMDelItemReq(const ConnId& connId, GMDelItemReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

	//playerData.mItemData 注释 了
   /* const PlayerData& playerData = player->getPlayerData();
    for (List<ItemData>::Iter* it = playerData.mItemData.mItemDataList.begin();
        it != playerData.mItemData.mItemDataList.end();
        it = playerData.mItemData.mItemDataList.next(it))
    {
        if (it->mValue.mItemId == msg.mItemGuid)
        {
            player->getItemManager().delItem(msg.mItemGuid, msg.mCount);

            GMCommandResp resp;
            resp.mContents = "执行指令成功!";
            NetworkSystem::getSingleton().sendMsg(resp, connId);
            return;
        }
    }*/

    GMCommandResp resp;
    resp.mContents = "执行指令失败, 道具[";
    resp.mContents += msg.mItemGuid + "]不存在";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMAddEquipReq(const ConnId& connId, GMAddEquipReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    EquipTemplate* equipTemplate = EQUIP_TABLE().get(msg.mEquipTemplateId);
    if (!equipTemplate)
    {
        LOG_DEBUG("failed to get equipTemplate by tempalteid %lu", msg.mEquipTemplateId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 无效的道具模板id";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getEquipManager().createEquip(msg.mEquipTemplateId, ITEM_SOURCE_GM_COMMAND);

    GMCommandResp resp;
    resp.mContents = "执行指令成功!";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMDelEquipReq(const ConnId& connId, GMDelEquipReq& msg)
{
    //playerData.mEquipData 字段注释 sec
	/*GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }
    const PlayerData& playerData = player->getPlayerData();
    for (List<EquipData>::Iter* it = playerData.mEquipData.mEquipDataList.begin();
        it != playerData.mEquipData.mEquipDataList.end();
        it = playerData.mEquipData.mEquipDataList.next(it))
    {
        if (it->mValue.mEquipId == msg.mEquipGuid)
        {
            player->getEquipManager().destroyEquip(msg.mEquipGuid);

            GMCommandResp resp;
            resp.mContents = "执行指令成功!";
            NetworkSystem::getSingleton().sendMsg(resp, connId);
            return;
        }
    }*/

}

void 
GMMsgHandler::onGMAddRuneReq(const ConnId& connId, GMAddRuneReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    RuneTemplate* runeTemplate = RUNE_TABLE().get(msg.mRuneTemplateId);
    if (!runeTemplate)
    {
        LOG_DEBUG("failed to get runeTemplate by tempalteid %lu", msg.mRuneTemplateId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 无效的道具模板id";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getRuneManager().createRune(msg.mRuneTemplateId, ITEM_SOURCE_GM_COMMAND);

    GMCommandResp resp;
    resp.mContents = "执行指令成功!";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMDelRuneReq(const ConnId& connId, GMDelRuneReq& msg)
{
   
	// sec  playerData数据字段有更改
	/* GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }
    const PlayerData& playerData = player->getPlayerData();
    for (List<RuneData>::Iter* it = playerData.mRuneData.mRuneDataList.begin();
        it != playerData.mRuneData.mRuneDataList.end();
        it = playerData.mRuneData.mRuneDataList.next(it))
    {
        if (it->mValue.mRuneId == msg.mRuneGuid)
        {
            player->getRuneManager().destroyRune(msg.mRuneGuid);

            GMCommandResp resp;
            resp.mContents = "执行指令成功!";
            NetworkSystem::getSingleton().sendMsg(resp, connId);
            return;
        }
    }*/
}

void 
GMMsgHandler::onGMAddHeroDebrisReq(const ConnId& connId, GMAddHeroDebrisReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行指令失败, 玩家[";
        resp.mContents += msg.mPlayerName + "]不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    HeroDebrisTemplate* itemTemplate = HERO_DEBRIS_TABLE().get(msg.mHeroDebrisTemplateId);
    if (!itemTemplate)
    {
        LOG_DEBUG("failed to get itemtemplate by tempalteid %lu", msg.mHeroDebrisTemplateId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 无效的道具模板id";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getHeroDebrisManager().createHeroDebris(msg.mHeroDebrisTemplateId, msg.mCount, ITEM_SOURCE_GM_COMMAND);

    GMCommandResp resp;
    resp.mContents = "执行指令成功!";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMDelHeroDebrisReq(const ConnId& connId, GMDelHeroDebrisReq& msg)
{
	//playerdata 数据有更改 sec
	/*GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
	if (!gmClient)
	{
	LOG_WARN("Failed to get gmclient by connId %llu", connId);
	return;
	}

	Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
	if (!player)
	{
	LOG_DEBUG("Failed to get Player %s", msg.mPlayerName.c_str());
	GMCommandResp resp;
	resp.mContents = "执行指令失败, 玩家[";
	resp.mContents += msg.mPlayerName + "]不存在或者未登录";
	NetworkSystem::getSingleton().sendMsg(resp, connId);
	return;
	}

	const PlayerData& playerData = player->getPlayerData();
	for (List<HeroDebrisData>::Iter* it = playerData.mHeroDebrisData.mHeroDebrisList.begin();
	it != playerData.mHeroDebrisData.mHeroDebrisList.end();
	it = playerData.mHeroDebrisData.mHeroDebrisList.next(it))
	{
	if (it->mValue.mHeroDebrisId == msg.mHeroDebrisGuid)
	{
	player->getHeroDebrisManager().delHeroDebris(msg.mHeroDebrisGuid, msg.mCount);

	GMCommandResp resp;
	resp.mContents = "执行指令成功!";
	NetworkSystem::getSingleton().sendMsg(resp, connId);
	return;
	}
	}

	GMCommandResp resp;
	resp.mContents = "执行指令失败, 道具[";
	resp.mContents += msg.mHeroDebrisGuid + "]不存在";
	NetworkSystem::getSingleton().sendMsg(resp, connId);*/
}

void
GMMsgHandler::onGMAddHeroReq(const ConnId& connId, GMAddHeroReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    if (msg.mCount >= 10)
    {
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 一次最多添加10个英雄";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    for (UInt32 i = 0; i < msg.mCount; i++)
    {
        if (!player->getHeroManager().createHero(msg.mHeroTemplateId))
        {
            GMCommandResp resp;
            resp.mContents = "执行命令失败, 英雄模板ID不存在";
            NetworkSystem::getSingleton().sendMsg(resp, connId);
            return;
        }

    }
    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMDelHeroReq(const ConnId& connId, GMDelHeroReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    if (!player->getHeroManager().destroyHero(msg.mHeroGuid))
    {
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 英雄不存在";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMAddPetReq(const ConnId& connId, GMAddPetReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    if (msg.mCount >= 10)
    {
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 一次最多添加10个神兽";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    for (UInt32 i = 0; i < msg.mCount; i++)
    {
       /* if (!player->getPetManager().createPet(msg.mPetTemplateId))
        {
            GMCommandResp resp;
            resp.mContents = "执行命令失败, 神兽模板ID不存在";
            NetworkSystem::getSingleton().sendMsg(resp, connId);
            return;
        }*/
    }
    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void
GMMsgHandler::onGMDelPetReq(const ConnId& connId, GMDelPetReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    if (!player->getPetManager().destroyPet(msg.mPetGuid))
    {
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 神兽不存在";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetHeroExpReq(const ConnId& connId, GMSetHeroExpReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    const HeroData* heroData = player->getHeroManager().getHeroDataByGuid(msg.mHeroId);
    if (!heroData)
    {
        LOG_DEBUG("Failed to get hero %llu", msg.mHeroId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 英雄不存在";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    UInt32 currentExp = heroData->mBaseData.mLevelExp;

    if (msg.mLevelExp > currentExp)
    {
        player->getHeroManager().increaseHeroLevelExp(msg.mHeroId, msg.mLevelExp - currentExp);
    }
    else if (msg.mLevelExp < currentExp)
    {
        player->getHeroManager().reduceHeroLevelExp(msg.mHeroId, currentExp - msg.mLevelExp);
    }

    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetHeroAdvancedReq(const ConnId& connId, GMSetHeroAdvancedReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    const HeroData* heroData = player->getHeroManager().getHeroDataByGuid(msg.mHeroId);
    if (!heroData)
    {
        LOG_DEBUG("Failed to get hero %llu", msg.mHeroId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 英雄不存在";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    UInt32 currentAdvanced = heroData->mBaseData.mAdvanced;

    if (msg.mAdvanced > currentAdvanced)
    {
        player->getHeroManager().increaseHeroAdvanced(msg.mHeroId, msg.mAdvanced - currentAdvanced);
    }
    else if (msg.mAdvanced < currentAdvanced)
    {
        player->getHeroManager().reduceHeroAdvanced(msg.mHeroId, currentAdvanced - msg.mAdvanced);
    }

    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetHeroLevelReq(const ConnId& connId, GMSetHeroLevelReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    const HeroData* heroData = player->getHeroManager().getHeroDataByGuid(msg.mHeroId);
    if (!heroData)
    {
        LOG_DEBUG("Failed to get hero %llu", msg.mHeroId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 英雄不存在";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    UInt32 currentLevel = heroData->mBaseData.mLevel;

    if (msg.mLevel > currentLevel)
    {
        player->getHeroManager().increaseHeroLevel(msg.mHeroId, msg.mLevel - currentLevel);
    }
    else if (msg.mLevel < currentLevel)
    {
        player->getHeroManager().reduceHeroLevel(msg.mHeroId, currentLevel - msg.mLevel);
    }

    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetHeroPresentPosReq(const ConnId& connId, GMSetHeroPresentPosReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    const HeroData* heroData = player->getHeroManager().getHeroDataByGuid(msg.mHeroId);
    if (!heroData)
    {
        LOG_DEBUG("Failed to get hero %llu", msg.mHeroId);
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 英雄不存在";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getHeroManager().setHeroPresentPos(msg.mHeroId, msg.mPresentPos);

    player->updatePlayerPresentData();
    
    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetHeroEquipReq(const ConnId& connId, GMSetHeroEquipReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getEquipManager().useEquip(msg.mEquipId, msg.mPresentPos);

    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetHeroRuneReq(const ConnId& connId, GMSetHeroRuneReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getRuneManager().useRune(msg.mRuneId, msg.mPresentPos);

    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetEquipRefineReq(const ConnId& connId, GMSetEquipRefineReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getEquipManager().setEquipRefine(msg.mEquipId, msg.mRefineLevel);

    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetRuneRefineReq(const ConnId& connId, GMSetRuneRefineReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }

    player->getRuneManager().setRuneRefineLevel(msg.mRuneId, msg.mRefineLevel);

    GMCommandResp resp;
    resp.mContents = "执行指令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}

void 
GMMsgHandler::onGMSetTollgateReq(const ConnId& connId, GMSetTollgateReq& msg)
{
    GMClient* gmClient = LogicSystem::getSingleton().getGMClientByConnId(connId);
    if (!gmClient)
    {
        LOG_WARN("Failed to get gmclient by connId %llu", connId);
        return;
    }

    Player* player = LogicSystem::getSingleton().getPlayerByName(msg.mPlayerName);
    if (!player)
    {
        LOG_DEBUG("Failed to get player %s", msg.mPlayerName.c_str());
        GMCommandResp resp;
        resp.mContents = "执行命令失败, 玩家【";
        resp.mContents += msg.mPlayerName + "】不存在或者未登录";
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        return;
    }
    
    PersistUpdateTollgateDataNotify notify;
    notify.mPlayerGuid = player->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(notify, player->getPlayerGuid());

    GMCommandResp resp;
    resp.mContents = "执行命令成功";
    NetworkSystem::getSingleton().sendMsg(resp, connId);
}