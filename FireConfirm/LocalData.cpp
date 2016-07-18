#include "FireConfirmManager.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
// #include "../CommonLib/PlayerData.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "LocalData.h"

using namespace Lynx;



void LocalDataManager::setTwelvePalaceLocalData(ConnId connId,UInt32 key,UInt32 value)
{
	UInt32 getIt = 0;
	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);	

	PlayerLocalData localData = player->getPlayerLocalData();

	List<KeyValue> m_mapIDToSelect = localData.m_IDToSelect;
	for(List<KeyValue>::Iter *iter = localData.m_IDToSelect.begin();iter != NULL;iter = localData.m_IDToSelect.next(iter))
	{
		if (iter->mValue.key == key)
		{
			iter->mValue.value = value;
			getIt =1;
		}
	}
	if (getIt == 0)
	{
		KeyValue keyValue;
		keyValue.key = key;
		keyValue.value = value;
		localData.m_IDToSelect.insertTail(keyValue);
	}
	player->setPlayerLocalData(localData);

	player->getPersistManager().setDirtyBit(LOCALDATABIT);	

}


