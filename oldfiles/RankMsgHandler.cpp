#include "RankMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void 
RankMsgHandler::onGetRankReq(const ConnId& connId, GetRankReq& msg)
{
    GetRankResp resp;
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId: %llu", connId);
        return;
    }
    PlayerRankData playerRankData;
    resp.mPlayerRankData.mLevelRank = LogicSystem::getSingleton().getLevelRank(player->getPlayerGuid());
    resp.mPlayerRankData.mBattleValueRank = LogicSystem::getSingleton().getBattleValueRank(player->getPlayerGuid());
    resp.mPlayerRankData.mTollgateRank = LogicSystem::getSingleton().getTollgateRank(player->getPlayerGuid());
    player->sendMsgWithAck(resp);
}

void 
RankMsgHandler::onGetRankTopReq(const ConnId& connId, GetRankTopReq& msg)
{
    GetRankTopResp resp;
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId: %llu", connId);
        return;
    }
    //resp.mLevelRankList = LogicSystem::getSingleton().getLevelTopList();
    //resp.mBattleValueRankList = LogicSystem::getSingleton().getBattleValueTopList();
    //resp.mTollgateRankList = LogicSystem::getSingleton().getTollgateTopList();
    player->sendMsgWithAck(resp);
}