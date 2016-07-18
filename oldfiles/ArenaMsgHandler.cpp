#include "ArenaMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void
ArenaMsgHandler::onOpenArenaReq(const ConnId& connId, OpenArenaReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    LogicSystem::getSingleton().openArena(player->getPlayerGuid());
}

void
ArenaMsgHandler::onStartArenaBattleReq(const ConnId& connId, StartArenaBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    LogicSystem::getSingleton().startArenaBattle(player->getPlayerGuid(), msg.mOppGuid, msg.mOppRank);
}

void
ArenaMsgHandler::onRefreshArenaOppReq(const ConnId& connId, RefreshArenaOppReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    LogicSystem::getSingleton().refreshArenaOpp(player->getPlayerGuid());
}

void
ArenaMsgHandler::onOpenArenaRankReq(const ConnId& connId, OpenArenaRankReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    LogicSystem::getSingleton().openArenaRank(player->getPlayerGuid());
}
