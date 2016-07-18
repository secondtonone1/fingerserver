#include "PersistManager.h"
#include "Player.h"
#include "PersistThreadMsg.h"
#include "PersistSystem.h"

using namespace Lynx;
PersistManager::PersistManager()
{
	m_pPlayer = NULL;
	m_nDirtyBit = 0;
	m_nLastSyncTime = 0;
	
}	


PersistManager:: ~PersistManager()
{

}

bool PersistManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	return true;

}

void PersistManager::release()
{
	m_pPlayer = NULL;
	m_nDirtyBit = 0;
	m_nLastSyncTime = 0;	
}


UInt32 & PersistManager::getDirtyBit()
{
	return m_nDirtyBit;
}

void PersistManager::setDirtyBit(UInt32 dirtyBit)
{
	m_nDirtyBit = m_nDirtyBit | dirtyBit;
}

void PersistManager::update(const UInt64& accTime)
{
	if(m_nDirtyBit)
	{
		PersistManagerSetDirtyReq msg;
		msg.mPlayerUid = m_pPlayer->mPlayerData.mBaseData.m_nPlayerID;
		msg.mDirtyBit = m_nDirtyBit;

		m_nDirtyBit = 0;
		PersistSystem::getSingleton().postThreadMsg(msg,(UInt16)msg.mPlayerUid);
	}
	
}
