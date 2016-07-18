#include "EquipManager.h"
#include "Player.h"
using namespace Lynx;

EquipManager::EquipManager()
{
	m_pPlayer = NULL;
	m_pListItemDatas = NULL;
}	


EquipManager:: ~EquipManager()
{

}

bool EquipManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	
	m_pListItemDatas = m_pPlayer->getItemManager().m_pListItemDatas;
	m_pMapUidEquipType = &m_pPlayer->getItemManager().m_mapUidEquipType;

	return true;

}

void EquipManager::release()
{	
	m_pPlayer = NULL;
	m_pListItemDatas = NULL;
	m_pMapUidEquipType->clear();
}



std::string EquipManager::convertEquipStateToJson()
{
	MsgItemDataToJson itemDataToJson;
	itemDataToJson.m_mapUidEquipType = *m_pMapUidEquipType;

	return itemDataToJson.convertEquipStateData();
	
	
}

