#include "StoreMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void 
StoreMsgHandler::onStoreBuyItemReq(const ConnId& connId, StoreBuyItemReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connId: %llu", connId);
        return;
    }
    StoreBuyItemResp resp;
    resp.mErrorId = player->getStoreManager().buyStoreItem(msg.mTemplateId, msg.mType, msg.mCount);
    player->sendMsgWithAck(resp);
}

void 
StoreMsgHandler::onSecretStoreBuyItemReq(const ConnId& connId, SecretStoreBuyItemReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player bu connId: %llu", connId);
        return;
    }
    SecretStoreBuyItemResp resp;
    resp.mErrorId = player->getStoreManager().buySecretStoreItem(msg.mTemplateId);
    player->sendMsgWithAck(resp);
}

void 
StoreMsgHandler::onResetSecretStoreItemReq(const ConnId& connId, ResetSecretStoreItemReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player bu connId: %llu", connId);
        return;
    }
    ResetSecretStoreItemResp resp;
    resp.mErrorId = player->getStoreManager().resetSecretStoreItemByUseItem();
    player->sendMsgWithAck(resp);
}