#include "PetMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void
PetMsgHandler::onRefinePetReq(const ConnId& connId, RefinePetReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    RefinePetResp resp;
    resp.mIsAutoFlag = msg.mIsAutoFlag;
    resp.mPetId = msg.mPetId;

    if (msg.mIsAutoFlag)
    {
        resp.mErrno = player->getPetManager().refinePet(msg.mPetId, resp.mUpLevelCount);
    }
    else
    {
        resp.mErrno = player->getPetManager().refinePet(msg.mPetId, resp.mUpLevelCount, msg.mItemId);
    }

    player->sendMsgWithAck(resp);
}

void 
PetMsgHandler::onAwakenPetReq(const ConnId& connId, AwakenPetReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    AwakenPetResp resp;
    resp.mPetId = msg.mPetId;
    resp.mErrno = player->getPetManager().awakenPet(msg.mPetId, msg.mPetIdList, msg.mPetDebrisList,
        resp.mUpLevelCount, resp.mPetTemplateId);

    player->sendMsgWithAck(resp);
}

void 
PetMsgHandler::onGetTalentReq(const ConnId& connId, GetTalentReq& msg)
{
   /* Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    GetTalentResp resp;
    resp.mPetId = msg.mPetId;
    
    if (msg.mIsAdvanceFlag)
    {
       resp.mErrno = player->getPetManager().getAdvanceTalent(msg.mPetId, resp.mTalentData);
    }
    else
    {
        resp.mErrno = player->getPetManager().getCommonTalent(msg.mPetId, resp.mTalentData);
    }

    player->sendMsgWithAck(resp);*/
}

void 
PetMsgHandler::onSaveTalentReq(const ConnId& connId, SaveTalentReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    SaveTalentResp resp;
    resp.mPetId = msg.mPetId;

    if (msg.mIsSavedFlag)
    {
        resp.mErrno = player->getPetManager().saveTalent(msg.mPetId);
        player->sendMsgWithAck(resp);
    }
    else
    {
       resp.mErrno = player->getPetManager().cancelTalent(msg.mPetId);
    }
}

void 
PetMsgHandler::onCombinePetReq(const ConnId& connId, CombinePetReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("failed to get player by connId %llu", connId);
        return;
    }

    CombinePetResp resp;
    resp.mErrno = player->getPetDebrisManager().combinePet(msg.mPetDebrisId, resp.mPetId);
    player->sendMsgWithAck(resp);
}

void 
PetMsgHandler::onSetPetPresentPosReq(const ConnId& connId, SetPetPresentPosReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("failed to get player by connId %llu", connId);
        return;
    }

    SetPetPresentPosResp resp;
    resp.mPetGuid = msg.mPetGuid;
    resp.mErrno = player->getPetManager().setPetPresentPos(msg.mPetGuid, 1);
    player->sendMsgWithAck(resp);
}

void
PetMsgHandler::onAdvancePetReq(const ConnId& connId, AdvancePetReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("failed to get player by connId %llu", connId);
        return;
    }

    AdvancePetResp ap;
    ap.mErrno = player->getPetManager().advancePet(msg.mPetId);
    if (ap.mErrno != 0)
        player->sendMsgWithAck(ap);
}