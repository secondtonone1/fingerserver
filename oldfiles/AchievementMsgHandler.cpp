#include "AchievementMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void 
AchievementMsgHandler::onGetAchievementAwardReq(const ConnId& connId, GetAchievementAwardReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connid: %llu", connId);
        return;
    }
    
    GetAchievementAwardResp resp;
    resp.mErrorId = player->getAchievementManager().getAchievementAward(msg.mTemplateId, resp.mMaxLevel, resp.mTemplateId);
    player->sendMsgWithAck(resp);
}