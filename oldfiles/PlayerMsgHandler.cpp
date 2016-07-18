#include "PlayerMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void 
PlayerMsgHandler::onUpgreadGoldMineralReq(const ConnId& connId, UpgreadGoldMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    UpgreadGoldMineralResp resp;
    resp.mErrno = LynxErrno::None;
    if (!player->getProduceManager().upgreadGoldMineral())
    {
        resp.mErrno = LynxErrno::InvalidParameter;
    }
	//getPlayerData().mProduceData 字段注释 sec
   /* resp.mGoldMineralLevel = player->getPlayerData().mProduceData.mGoldMineralLevel;
    resp.mRemainGoldMineralCount = player->getPlayerData().mProduceData.mRemainGoldMineralCount;
    resp.mCalculateTime = player->getPlayerData().mProduceData.mGoldMineralCalculateTime;*/

    player->sendMsgWithAck(resp);
}

void 
PlayerMsgHandler::onUpspeedGoldMineralReq(const ConnId& connId, UpspeedGoldMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    UpspeedGoldMineralResp resp;
    resp.mErrno = LynxErrno::None;
    if (!player->getProduceManager().upspeedGoldMineral())
    {
        resp.mErrno = LynxErrno::InvalidParameter;
    }
	//getPlayerData().mProduceData 字段注释 sec
    /*resp.mGoldMineralSpeedUpTime = player->getPlayerData().mProduceData.mGoldMineralSpeedUpTime;
    resp.mGoldMineralStartSpeedTime = player->getPlayerData().mProduceData.mGoldMineralStartSpeedTime;*/

    player->sendMsgWithAck(resp);
}

void 
PlayerMsgHandler::onUpgreadDiamondMineralReq(const ConnId& connId, UpgreadDiamondMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    UpgreadDiamondMineralResp resp;
    resp.mErrno = LynxErrno::None;
    if (!player->getProduceManager().upgreadDiamondMineral())
    {
        resp.mErrno = LynxErrno::InvalidParameter;
    }

	//getPlayerData().mProduceData 字段注释 sec
   /* resp.mDiamondMineralLevel = player->getPlayerData().mProduceData.mDiamondMineralLevel;
    resp.mRemainDiamondMineralCount = player->getPlayerData().mProduceData.mRemainDiamondMineralCount;
    resp.mCalculateTime = player->getPlayerData().mProduceData.mDiamondMineralCalculateTime;*/

    player->sendMsgWithAck(resp);
}

void 
PlayerMsgHandler::onUpspeedDiamondMineralReq(const ConnId& connId, UpspeedDiamondMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    UpspeedDiamondMineralResp resp;
    resp.mErrno = LynxErrno::None;
    if (!player->getProduceManager().upspeedDiamondMineral())
    {
        resp.mErrno = LynxErrno::InvalidParameter;
    }
	//PlayerData().mProduceData字段注释了
    //resp.mDiamondMineralSpeedUpTime = player->getPlayerData().mProduceData.mDiamondMineralSpeedUpTime;
    //resp.mDiamondMineralStartSpeedTime = player->getPlayerData().mProduceData.mDiamondMineralStartSpeedTime;

    player->sendMsgWithAck(resp);
}

void 
PlayerMsgHandler::onUpgreadExpMineralReq(const ConnId& connId, UpgreadExpMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    UpgreadExpMineralResp resp;
    resp.mErrno = LynxErrno::None;
    if (!player->getProduceManager().upgreadExpMineral())
    {
        resp.mErrno = LynxErrno::InvalidParameter;
    }

	//PlayerData().mProduceData 字段注释

   /* resp.mExpMineralLevel = player->getPlayerData().mProduceData.mExpMineralLevel;
    resp.mCalculateTime = player->getPlayerData().mProduceData.mExpMineralCalculateTime;
    resp.mRemainExpMineralCount = player->getPlayerData().mProduceData.mRemainExpMineralCount;*/

    player->sendMsgWithAck(resp);
}

void 
PlayerMsgHandler::onUpspeedExpMineralReq(const ConnId& connId, UpspeedExpMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    UpspeedExpMineralResp resp;
    resp.mErrorId = LynxErrno::None;
    if (!player->getProduceManager().upspeedExpMineral())
    {
        resp.mErrorId = LynxErrno::InvalidParameter;
    }
	//PlayerData().mProduceData注释了sec
   /* resp.mExpMineralSpeedUpTime = player->getPlayerData().mProduceData.mExpMineralSpeedUpTime;
    resp.mExpMineralStartSpeedTime = player->getPlayerData().mProduceData.mExpMineralStartSpeedTime;*/

    player->sendMsgWithAck(resp);
}

void
PlayerMsgHandler::onGetGoldMineralReq(const ConnId& connId, GetGoldMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    GetGoldMineralResp resp;
    resp.mErrno = LynxErrno::None;
    resp.mGoldMineralCount = player->getProduceManager().getGoldMineral();
    //字段注释 sec
	//resp.mCalculateTime = player->getPlayerData().mProduceData.mGoldMineralCalculateTime;
    
    player->sendMsgWithAck(resp);
}

void 
PlayerMsgHandler::onGetDiamondMineralReq(const ConnId& connId, GetDiamondMineralReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    GetDiamondMineralResp resp;
    resp.mErrno = LynxErrno::None;
    resp.mDiamondMineralCount = player->getProduceManager().getDiamondMineral();
    resp.mCalculateTime = TimeUtil::getTimeSec();
    
    player->sendMsgWithAck(resp);
}

void 
PlayerMsgHandler::onViewPlayerInfoReq(const ConnId& connId, ViewPlayerInfoReq& msg)
{
    //客户端发来的查看玩家信息的请求回调函数，因为目前需求没有定义，所以先注释了
	
	/*Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    ViewPlayerInfoResp resp;
    resp.mPlayerGuid = msg.mPlayerGuid;
    resp.mErrno = LynxErrno::None;

    const PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(msg.mPlayerGuid);
    if (!playerInfo)
    {
        LOG_WARN("Failed to get player info %llu", msg.mPlayerGuid);
        resp.mErrno = LynxErrno::PlayerInfoNotExist;
        player->sendMsg(resp);
        return;
    }

    resp.mPlayerName = playerInfo->mPlayerName;
    resp.mPlayerIconName = playerInfo->mPlayerIconName;
    resp.mPlayerLevel = playerInfo->mPlayerLevel;
    resp.mPlayerVipLevel = playerInfo->mPlayerVipLevel;
    resp.mPlayerBattleValue = playerInfo->mPlayerBattleValue;

    for (UInt32 i = 0; i < 5; ++i)
    {
        const HeroData& heroData = playerInfo->mPresentHeroArray[i].mHeroData;
        if (heroData.mBaseData.mHeroId)
        {
            resp.mPlayerHeroData.mHeroDataList.insertTail(heroData);
            for (UInt32 j = 0; j < 5; ++j)
            {
                const EquipData& equipData = playerInfo->mPresentHeroArray[i].mEquipDataArray[j];
                if (equipData.mEquipId)
                {
                    resp.mPlayerEquipData.mEquipDataList.insertTail(equipData);
                }

                const RuneData& runeData = playerInfo->mPresentHeroArray[i].mRuneDataArray[j];
                if (runeData.mRuneId)
                {
                    resp.mPlayerRuneData.mRuneDataList.insertTail(runeData);
                }
            }
        }
    }

    if (playerInfo->mPresentPet.mPetData.mBaseData.mPetId)
    {
        resp.mPlayerPetData.mPetDataList.insertTail(playerInfo->mPresentPet.mPetData);
    }

    player->sendMsg(resp);*/
}

void 
PlayerMsgHandler::onSummonHeroReq(const ConnId& connId, SummonHeroReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player.");
        return;
    }

    SummonHeroResp resp;
    resp.mType = msg.mType;

    switch (msg.mType)
    {
    case 0: // 0 普通道具抽
        resp.mErrno = player->getSummonManager().summonHeroByItem(resp.mHeroGuidList);
        break;
    case 1: // 1 免费钻石抽
        resp.mErrno = player->getSummonManager().summonHeroByFreeDiamond(resp.mHeroGuidList);
        break;
    case 2: // 2 钻石抽
        resp.mErrno = player->getSummonManager().summonHeroByDiamond(resp.mHeroGuidList);
        break;
    case 3: // 3 钻石十连抽
        resp.mErrno = player->getSummonManager().summonHeroByBatch(resp.mHeroGuidList);
        break;
    default:
        LOG_WARN("Unknown summon type %u", msg.mType);
        resp.mErrno = LynxErrno::InvalidPermission;
        break;
    };

    player->sendMsg(resp);
}

