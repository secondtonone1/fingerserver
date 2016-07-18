#include "MemDataSystem.h"
#include "GameServer.h"

using namespace Lynx;

MemDataSystem::MemDataSystem(){}

MemDataSystem::~MemDataSystem()
{}

bool MemDataSystem::initial()
{
	m_mapPlayerFandBs.clear();
	LOG_INFO("Initial MemDataSystem success!!!!");
	return true;
}


void MemDataSystem::release()
{
	m_mapPlayerFandBs.clear();
}

FriendData*  MemDataSystem::getPlayerFandData(UInt64 playerUid)
{
	Map<UInt64, FriendData  >::Iter*  playerFindIter = m_mapPlayerFandBs.find(playerUid);

	if(playerFindIter)
	{
		return &playerFindIter->mValue;
	}
	else
	{
		return NULL;
	}
}

FriendData * MemDataSystem:: insertPlayerFandData(UInt64 playerUid, FriendData friendData)
{
		Map<UInt64,  FriendData  >::Iter * playerFandIter = m_mapPlayerFandBs.insert(playerUid,friendData);

		return &playerFandIter->mValue;
}
