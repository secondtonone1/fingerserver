#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_MATCHING_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_MATCHING_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"

namespace Lynx
{

	class Player;

	
// 	struct MapRecord
// 	{	
// 		MapRecord mapRecord;
// 	};
// 
// 	struct CGRecord
// 	{
// 		CGRecord(): typeID(0){}
// 
// 		UInt32 typeID;
// 		
// 		std::string jsonStr;
// 		void convertJsonToData(std::string jsonStr)
// 		{
// 			Json::Reader reader;    
// 			Json::Value root;    
// 			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
// 			{
// 				typeID = root["typeID"].asUInt();				
// 			}
// 		}
// 		LYNX_MESSAGE_1(CHAPTER_RECORD_REQ, CGRecord,std::string, jsonStr);
// 	};
// 
// 	struct RecordResp	
// 	{
// 		RecordResp():typeID(0) {}
// 
// 
// 		UInt32 typeID;
// 		MapRecord recods;
// 
// 		std::string convertDataToJson()
// 		{
// 			Json::Value root;     	
// 			root["typeID"] = Json::Value(typeID);
// 			for(MapRecord::Iter *iter = recods.begin();iter!=NULL;iter=recods.next(iter))
// 			{
// 				Json::Value son; 
// 				son["stageID"] = Json::Value(iter->mKey);
// 				son["playerID"] = Json::Value(iter->mValue.playerID);
// 				son["name"] = Json::Value(iter->mValue.name);
// 				son["modleID"] = Json::Value(iter->mValue.modleID);
// 				son["time"] = Json::Value(iter->mValue.val);
// 
// 				root["recods"].append(son);
// 			}
// 			Json::FastWriter writer;  
// 			std::string strWrite = writer.write(root);
// 			return strWrite;
// 		}
// 	};
// 
// 
// 	struct CGTwelvePalaceRecord
// 	{
// 		CGTwelvePalaceRecord(): typeID(0){}
// 
// 		UInt32 typeID;
// 
// 		std::string jsonStr;
// 		void convertJsonToData(std::string jsonStr)
// 		{
// 			Json::Reader reader;    
// 			Json::Value root;    
// 			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
// 			{
// 				typeID = root["typeID"].asUInt();				
// 			}
// 		}
// 		LYNX_MESSAGE_1(CHAPTER_TWELVE_PALACE_RECORD_REQ, CGTwelvePalaceRecord,std::string, jsonStr);
// 	};
// 
// 	struct TwelvePalaceRecordResp	
// 	{
// 		TwelvePalaceRecordResp():typeID(0) {}
// 
// 		UInt32 typeID;
// 		MapRecord recods;
// 
// 		std::string convertDataToJson()
// 		{
// 			Json::Value root;     	
// 			root["typeID"] = Json::Value(typeID);
// 			for(MapRecord::Iter *iter = recods.begin();iter!=NULL;iter=recods.next(iter))
// 			{
// 				Json::Value son; 
// 				son["stageID"] = Json::Value(iter->mKey);
// 				son["playerID"] = Json::Value(iter->mValue.playerID);
// 				son["name"] = Json::Value(iter->mValue.name);
// 				son["modleID"] = Json::Value(iter->mValue.modleID);
// 				son["time"] = Json::Value(iter->mValue.val);
// 
// 				root["recods"].append(son);
// 			}
// 			Json::FastWriter writer;  
// 			std::string strWrite = writer.write(root);
// 			return strWrite;
// 		}
// 	};

// 	
	class MatchingManager : public Singleton<MatchingManager>
	{
	
	public:

		
// 		UInt32 initRankingData();
// 
//  		static void onRecordReq(const  ConnId& connId,CGRecord &msg);
// 
// 		static void onTwelvePalaceRecordReq(const  ConnId& connId,CGTwelvePalaceRecord &msg);
// 
// 
// 		UInt32 setRecord(UInt32 type,UInt32 rank, Record record, bool needSave);//插入更高纪录
// 
// 
// 		MapRecord getRecords(UInt32 type);
// 
// 		void setRecords(UInt32 type,MapRecord mapRecord);
// 
// 		Map<UInt32,MapRecord>  getTypeRecords();
// 
// 		void setTypeRecords(Map<UInt32,MapRecord> val);
// 
// 		Record getRecord(UInt32 type,UInt32 rank);
// 
// 	private:
// 	
// 		Map<UInt32,MapRecord> mTypeRecords;
// 		
// 		
	};





}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_MATCHING_MANAGER_H__
