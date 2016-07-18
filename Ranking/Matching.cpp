#include "Ranking.h"
#include "Matching.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"


using namespace Lynx;
// 
// //一定要在load后面处理，不然就会把数据清除了
// UInt32 RankingManager::initRankingData()
// {
// 	Record record;
// 	Map<UInt32,MapRecord>::Iter *item = mTypeRecords.find(STAGE_RECORD_TYPE);
// 	if(item ==NULL)
// 	{
// 		for(Map<UInt64, StageTemplate>::Iter* iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
// 		{
// 			if (iter->mValue.mType == STAGE_TRIAL &&iter->mValue.limittime1 > 0)
// 			{
// 				record.playerID =0;
// 				record.name ="";
// 				record.modleID =0;
// 				record.val = iter->mValue.limittime1;	
// 				RankingManager::getSingleton().setRecord(STAGE_RECORD_TYPE,iter->mValue.mStageId,record,true);
// 			}
// 		}
// 	}
// 
// 	Map<UInt32,MapRecord>::Iter *item1 = mTypeRecords.find(TWELVE_PALACE_RECORD_TYPE);
// 	if(item1 ==NULL)
// 	{
// 		for(Map<UInt64, StageTemplate>::Iter* iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
// 		{
// 			if (iter->mValue.mType == STAGE_TWELVEPALACE &&iter->mValue.limittime1 > 0)
// 			{
// 				record.playerID =0;
// 				record.name ="";
// 				record.modleID =0;
// 				record.val = iter->mValue.limittime1;	
// 				RankingManager::getSingleton().setRecord(TWELVE_PALACE_RECORD_TYPE,iter->mValue.mStageId,record,true);
// 			}
// 		}
// 	}
// 	return 1;
// 
// }
// void RankingManager::onRecordReq(const  ConnId& connId,CGRecord &msg)
// {
// 
// 	msg.convertJsonToData(msg.jsonStr);
// 	RecordResp resp;
// 	resp.typeID = msg.typeID;
// 	
//  	resp.recods = RankingManager::getSingleton().getRecords(msg.typeID).mapRecord;
// 	if (resp.recods.size()==0)
// 	{
// 		return;
// 	}
// 	 std::string str =  resp.convertDataToJson(); 
// 	
// 	
// 	 if (msg.typeID == TWELVE_PALACE_RECORD_TYPE )
// 	 {
// 		 NetworkSystem::getSingleton().sender(connId,CHAPTER_RECORD_RESP,str);
// 	 }
// 	 else
// 	 {
// 		 NetworkSystem::getSingleton().sender(connId,CHAPTER_TWELVE_PALACE_RECORD_REQ,str);
// 	 }
// 	
// }
// //用两个协议发
// void RankingManager::onTwelvePalaceRecordReq(const  ConnId& connId,CGTwelvePalaceRecord &msg)
// {
// 
// 	CGRecord msg1;
// 	
// 	msg.convertJsonToData(msg.jsonStr);
// 	msg1.typeID = msg.typeID;
// 	
// 	onRecordReq(connId, msg1);
// }
// 	
// 
// //todo 如果要替换自己的成绩bug
// UInt32 RankingManager::setRecord(UInt32 type,UInt32 rank, Record record, bool needSave =false)
// {
// 
// 	UInt32 flag =5;
// 	PersistRankingSaveReq req;
// 	MapRecord mapRecords;
// 
// 	Map<UInt32,MapRecord>::Iter *item = mTypeRecords.find(type);
// 
// 
// 	if (item ==NULL)
// 	{  		
// 		mapRecords.mapRecord.insert(rank,record);
// 		mTypeRecords.insert(type,mapRecords);
// 		flag =1;
// 	}
// 	else
// 	{
// 		Record mRecord;
// 
// 		for(MapRecord::Iter *it = item->mValue.mapRecord.begin(); it != NULL;it = item->mValue.mapRecord.next(it))
// 		{
// 			if (it->mKey == rank)
// 			{
// 				if (record.val < it->mValue.val)
// 				{
// 					it->mValue.playerID = record.playerID;
// 					it->mValue.name = record.name;
// 					it->mValue.modleID = record.modleID;
// 					it->mValue.val = record.val;
// 					flag =3;
// 					break;
// 				}		
// 				flag =4;
// 				break;
// 			}
// 
// 		}
// 
// 		if (flag != 3 && flag !=4)
// 		{
// 			item->mValue.mapRecord.insert(rank,record);
// 			flag =2;
// 		}		
// 	}
// 	if (flag < 4 && needSave == true)
// 	{
// 		req.type = type;
// 		req.rank = rank;
// 		req.playerID = record.playerID;		
// 		req.val = record.val;
// 		PersistSystem::getSingleton().postThreadMsg(req, 0);
// 	}
// 	return flag;
// }
// 
// MapRecord RankingManager::getRecords(UInt32 type)
// {
// 	MapRecord mapRecords;
//  	 
// 	for(Map<UInt32,MapRecord>::Iter *iter = mTypeRecords.begin();iter != NULL;iter = mTypeRecords.next(iter))
// 	{
// 		if (iter->mKey == type)
// 		{
// 			mapRecords = iter->mValue;
// 			break;
// 		}
// 
// 	}
// 
// 	return mapRecords;
// }
// 
// Record RankingManager::getRecord(UInt32 type,UInt32 rank)
// {
// 
// 	Record record;
// 	MapRecord mapRecords;
// 
// 	for(Map<UInt32,MapRecord>::Iter *iter = mTypeRecords.begin();iter != NULL;iter = mTypeRecords.next(iter))
// 	{
// 		if (iter->mKey == type)
// 		{
// 			for (MapRecord::Iter * it = iter->mValue.mapRecord.begin();it != NULL;it = iter->mValue.mapRecord.next(it))
// 			{
// 				if (it->mKey == rank)
// 				{
// 					return it->mValue;
// 					break;
// 				}
// 			}
// 			break;
// 		}
// 
// 	}
// 	return record;
// }
// 
// void RankingManager::setRecords(UInt32 type,MapRecord mapRecord)
// {
// 	UInt32 isExit =0 ;
// 	for(Map<UInt32,MapRecord>::Iter *iter = mTypeRecords.begin();iter!= NULL;iter = mTypeRecords.next(iter))
// 	{
// 		if (iter->mKey == type)
// 		{
// 			iter->mValue = mapRecord;	
// 			isExit =1;
// 			break;
// 		}		
// 	}
// 	if (isExit == 0)
// 	{
// 		mTypeRecords.insert(type,mapRecord);
// 	}
// }
// 
// Map<UInt32,MapRecord> RankingManager::getTypeRecords()
// {
// 	return mTypeRecords;
// }
// 
// void RankingManager::setTypeRecords(Map<UInt32,MapRecord> val)
// {
// 	mTypeRecords = val;
// }

