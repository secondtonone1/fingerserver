#include "MaterialManager.h"
#include "Player.h"
#include "LogicSystem.h"
using namespace Lynx;


MaterialManager::MaterialManager()
{
	m_pPlayer = NULL;
	
}

MaterialManager:: ~MaterialManager()
{

}


bool MaterialManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;
	
	m_pMaterialDataList = & player->mPlayerData.mMaterialData.mMaterialLists;

	

	for(List<MaterialData>::Iter * materialDataIter = m_pMaterialDataList->begin();materialDataIter != NULL; 
		materialDataIter = m_pMaterialDataList->next(materialDataIter))
	{
		m_mapIdMaterialData.insert(materialDataIter->mValue.m_nId, &materialDataIter->mValue);
	}

	return true;
}

void MaterialManager::release()
{
	m_pPlayer = NULL;
	m_pMaterialDataList = NULL;
	
	m_mapIdMaterialData.clear();

}

void MaterialManager::addMaterial(UInt64 materialID, UInt32 count)
{
	Map<UInt64, MaterialData *>::Iter * materialDataIter = m_mapIdMaterialData.find(materialID);
	if(materialDataIter)
	{
		materialDataIter->mValue->m_nCount += count;
		materialDataIter->mValue->m_nDirtyType = UPDATEDIRTY;
		
		m_pPlayer->getPersistManager().setDirtyBit(MATERIALDATABIT);
	}
	else
	{
		Guid materialGuid = LogicSystem::getSingleton().generateMaterialGuid();
		
		MaterialData materialData;
		materialData.m_nId = materialID;
		materialData.m_nUid = materialGuid;
		materialData.m_nCount = count;
		materialData.m_nDirtyType = UPDATEDIRTY;

		List<MaterialData>::Iter * insertIter = m_pMaterialDataList->insertTail(materialData);
		m_mapIdMaterialData.insert(materialID, &insertIter->mValue);
	
		m_pPlayer->getPersistManager().setDirtyBit(MATERIALDATABIT);
	}
}

UInt32 MaterialManager::getMaterial(UInt64 materialID)
{
	Map<UInt64, MaterialData *>::Iter * materialDataIter = m_mapIdMaterialData.find(materialID);
	if(materialDataIter)
	{
		return materialDataIter->mValue->m_nCount;	
	}
	else
	{
		return 0;
	}
}


bool MaterialManager::delMaterial(UInt64 materialID, UInt32 count)
{
	Map<UInt64, MaterialData *>::Iter * materialDataIter = m_mapIdMaterialData.find(materialID);
	if(materialDataIter)
	{
		if(materialDataIter->mValue->m_nCount >= count)
		{
			materialDataIter->mValue->m_nCount -= count;
			materialDataIter->mValue->m_nDirtyType = UPDATEDIRTY;

			m_pPlayer->getPersistManager().setDirtyBit(MATERIALDATABIT);

			return true;
		}
		else
		{
			return false;
		}
		
	}
	else
	{	
		return false;
	}
}

List<MaterialData>* MaterialManager::getMaterialList(void)
{
	return m_pMaterialDataList;
}


UInt32 MaterialManager::getMaterialCount(UInt64 materialID)
{
	Map<UInt64, MaterialData *>::Iter * materialDataIter = m_mapIdMaterialData.find(materialID);
	
	if(materialDataIter)
	{
		return materialDataIter->mValue->m_nCount;
	}
	else
	{
		return 0;
	}
}

MaterialData* MaterialManager::getMaterialData(UInt64 materialID)
{
	Map<UInt64, MaterialData *>::Iter * materialDataIter = m_mapIdMaterialData.find(materialID);

	if(materialDataIter)
	{
		return materialDataIter->mValue;
	}
	else
	{
		return NULL;
	}

	
	
}

std::string MaterialManager::convertDataToJson(void)
{
	
	Json::Value root;
	//遍历list,完成json的转换
	for(List<MaterialData>::Iter * materialDataIter = m_pMaterialDataList->begin();materialDataIter != NULL; 
		materialDataIter = m_pMaterialDataList->next(materialDataIter))
	{
		Json::Value materialRoot;
		materialRoot["itemId"] = materialDataIter->mValue.m_nId;
		materialRoot["stackAmount"] = materialDataIter->mValue.m_nCount;

		root.append(materialRoot);
	
	}

	Json::StyledWriter writer;
	
	
	return writer.write(root);

}