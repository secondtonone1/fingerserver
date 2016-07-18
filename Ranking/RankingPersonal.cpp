#include "Ranking.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "RankingPersonal.h"


using namespace Lynx;

RankingPersonalManager::RankingPersonalManager()
{
	mPlayer = NULL;
	mTypeRecords = NULL;

}



RankingPersonalManager:: ~RankingPersonalManager()
{

}


//一定要在load后面处理，不然就会把数据清除了
UInt32 RankingPersonalManager::initial(Player* player)
{

	mPlayer = player;

	mTypeRecords = &player->mPlayerData.mRecordsData.m_TypeRecords;

// 	Record record;
// 	TypeRecords::Iter *item = mTypeRecords->find(STAGE_RECORD_TYPE);
// 	if(item ==NULL)
// 	{
// 		for(Map<UInt64, StageTemplate>::Iter* iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
// 		{
// 			if (iter->mValue.mType == STAGE_TRIAL &&iter->mValue.limittime2 > 0)
// 			{
// 				record.playerID =0;
// 				record.name ="";
// 				record.modleID =0;
// 				record.val = iter->mValue.limittime2;	
// 				RankingPersonalManager::getSingleton().setRecord(STAGE_RECORD_TYPE,iter->mValue.mStageId,record,true);
// 			}
// 		}
// 	}
// 
// 	TypeRecords::Iter *item1 = mTypeRecords->find(TWELVE_PALACE_RECORD_TYPE);
// 	if(item1 ==NULL)
// 	{
// 		for(Map<UInt64, StageTemplate>::Iter* iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
// 		{
// 			if (iter->mValue.mType == STAGE_TWELVEPALACE &&iter->mValue.limittime2 > 0)
// 			{
// 				record.playerID =0;
// 				record.name ="";
// 				record.modleID =0;
// 				record.val = iter->mValue.limittime2;	
// 				RankingPersonalManager::getSingleton().setRecord(TWELVE_PALACE_RECORD_TYPE,iter->mValue.mStageId,record,true);
// 			}
// 		}
// 	}
	return 1;

}

void RankingPersonalManager::release()
{
	mPlayer = NULL;
	mTypeRecords = NULL;
}

//todo 如果要替换自己的成绩bug
UInt32 RankingPersonalManager::setRecord(UInt32 type,UInt32 rank, Record record, bool needSave =false)
{

	UInt32 flag =5;
	PersistRankingSaveReq req;
	MapRecord mapRecords;

	TypeRecords::Iter *item = mTypeRecords->find(type);


	if (item ==NULL)
	{  		
		mapRecords.insert(rank,record);
		mTypeRecords->insert(type,mapRecords);
		flag =1;
	}
	else
	{
		Record mRecord;

		for(MapRecord::Iter *it = item->mValue.begin(); it != NULL;it = item->mValue.next(it))
		{
			if (it->mKey == rank)
			{
				if (record.val < it->mValue.val)
				{
					it->mValue.playerID = record.playerID;
					it->mValue.name = record.name;
					it->mValue.modleID = record.modleID;
					it->mValue.val = record.val;
					flag =3;
					break;
				}		
				flag =4;
				break;
			}

		}

		if (flag != 3 && flag !=4)
		{
			item->mValue.insert(rank,record);
			flag =2;
		}		
	}
	if (flag < 4 && needSave == true)
	{
		req.type = type;
		req.rank = rank;
		req.playerID = record.playerID;		
		req.val = record.val;
		PersistSystem::getSingleton().postThreadMsg(req, 0);
	}
	return flag;
}

MapRecord RankingPersonalManager::getRecords(UInt32 type)
{
	MapRecord mapRecords;
	if (mTypeRecords == NULL)
	{
		return mapRecords;
	}
 	 
	for(TypeRecords::Iter *iter = mTypeRecords->begin();iter != NULL;iter = mTypeRecords->next(iter))
	{
		if (iter->mKey == type)
		{
			mapRecords = iter->mValue;
			break;
		}

	}

	return mapRecords;
}

Record RankingPersonalManager::getRecord(UInt32 type,UInt32 rank)
{

	Record record;
	MapRecord mapRecords;

	for(TypeRecords::Iter *iter = mTypeRecords->begin();iter != NULL;iter = mTypeRecords->next(iter))
	{
		if (iter->mKey == type)
		{
			for (MapRecord::Iter * it = iter->mValue.begin();it != NULL;it = iter->mValue.next(it))
			{
				if (it->mKey == rank)
				{
					return it->mValue;
					break;
				}
			}
			break;
		}

	}
	return record;
}

void RankingPersonalManager::setRecords(UInt32 type,MapRecord mapRecord)
{
	UInt32 isExit =0 ;
	for(TypeRecords::Iter *iter = mTypeRecords->begin();iter!= NULL;iter = mTypeRecords->next(iter))
	{
		if (iter->mKey == type)
		{
			iter->mValue = mapRecord;	
			isExit =1;
			break;
		}		
	}
	if (isExit == 0)
	{
		mTypeRecords->insert(type,mapRecord);
	}
}

TypeRecords * RankingPersonalManager::getTypeRecords()
{
	return mTypeRecords;
}

void RankingPersonalManager::setTypeRecords(TypeRecords val)
{
 	mTypeRecords = &val;
}

