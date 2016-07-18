#ifndef __LYNX_COMMON_LIB_MSG_STAGEDATA_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_STAGEDATA_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"

namespace Lynx
{

	struct MsgStageDataToJson
	{
		MsgStageDataToJson()
		{

		}


		std::string convertDataToJson(Json::Value &node)
		{
			
			std::stringstream mystream;
			Json::Value chapterNode;

			for(Map<UInt16, PlayerChapterData *>::Iter * chapterIter = m_mapIDToChapter.begin(); chapterIter != NULL;
				chapterIter = m_mapIDToChapter.next(chapterIter))
			{
								
				mystream.clear();
				mystream.str();

				mystream << chapterIter->mValue->m_nChapterId;

				std::string chapterIdStr;
				mystream >> chapterIdStr;

					//宝石数组
					Json::Value treasureNode;
					treasureNode["treasure"].append(chapterIter->mValue->m_nTreasure1getstate);
					treasureNode["treasure"].append(chapterIter->mValue->m_nTreasure2getstate);
					treasureNode["treasure"].append(chapterIter->mValue->m_nTreasure3getstate);

					chapterNode[chapterIdStr] = treasureNode;

					//获取时间

					chapterNode[chapterIdStr]["chapterStartTime"] = chapterIter->mValue->m_nChapterstarttime;


					//结束时间

					chapterNode[chapterIdStr]["chapterFinishTime"] =  chapterIter->mValue->m_nChapterfinishtime;


					Json::Value stageNode;
					for(List<StageData>::Iter* stageIter = chapterIter->mValue->m_listStageDatas.begin();
						stageIter != NULL; stageIter = chapterIter->mValue->m_listStageDatas.next(stageIter))
					{
						if(stageIter->mValue.m_nStageID)
						{
							UInt32 challengeTimes = stageIter->mValue.m_nChallengTimes;
							UInt64 lastChallengeTime = stageIter->mValue.m_nLastChallengTime;
							UInt32 stageID = stageIter->mValue.m_nStageID;
							UInt8 getStar = stageIter->mValue.m_nGetStar;
							mystream.clear();
							mystream.str("");
							mystream << stageID;
							std::string stageIDStr;
							mystream >> stageIDStr;

							stageNode[stageIDStr]["starState"] = getStar;
							stageNode[stageIDStr]["challengeTimes"] = challengeTimes;
							stageNode[stageIDStr]["lastChallengeTime"] = lastChallengeTime;
						}
					}				

					chapterNode[chapterIdStr]["stage"] = stageNode;

				}
				
			
// 			isLock = true,//完成解锁
// 				isSelectUnexcepted = false,//打钩，乱入者
			node = chapterNode;
			Json::FastWriter writer;  
			std::string strWrite = writer.write(chapterNode);
			

			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{	
			
		}


		Map<UInt16, PlayerChapterData *> m_mapIDToChapter;
		Guid playerID;


	};


	struct MsgStageDataToJsonTwelvePalace
	{
		MsgStageDataToJsonTwelvePalace()
		{

		}


		std::string convertDataToJson(Json::Value &node)
		{

			std::stringstream mystream;
			Json::Value chapterNode;

			for(Map<UInt16, PlayerChapterData *>::Iter * chapterIter = m_mapIDToChapter.begin(); chapterIter != NULL;
				chapterIter = m_mapIDToChapter.next(chapterIter))
			{

				mystream.clear();
				mystream.str();

				mystream << chapterIter->mValue->m_nChapterId;

				std::string chapterIdStr;
				mystream >> chapterIdStr;

				//宝石数组 --十二宗宫没有宝箱
// 				Json::Value treasureNode;
// 				treasureNode["treasure"].append(chapterIter->mValue->m_nTreasure1getstate);
// 				treasureNode["treasure"].append(chapterIter->mValue->m_nTreasure2getstate);
// 				treasureNode["treasure"].append(chapterIter->mValue->m_nTreasure3getstate);
// 
// 				chapterNode[chapterIdStr] = treasureNode;

				//获取时间

				chapterNode[chapterIdStr]["chapterStartTime"] = chapterIter->mValue->m_nChapterstarttime;


				//结束时间

				chapterNode[chapterIdStr]["chapterFinishTime"] =  chapterIter->mValue->m_nChapterfinishtime;

				UInt32 getIt = 0;
				for(List<KeyValue>::Iter * it =  m_ChapterUnlockeded.begin();it != NULL;it = m_ChapterUnlockeded.next(it))
				{
					if (it->mValue.key == chapterIter->mValue->m_nChapterId)
					{
						if(it->mValue.value == 1)
						{						
							getIt = 1;
						}
						break;
					}

				}
				if (getIt == 1)
				{
					chapterNode[chapterIdStr]["isLock"] =  1;//已解锁
				}
				else
				{
					chapterNode[chapterIdStr]["isLock"] =  0;
				}

				UInt32 getIt1 = 0;
				for(List<KeyValue> ::Iter * it1 =  m_mapIDToSelect.begin();it1 != NULL;it1 = m_mapIDToSelect.next(it1))
				{
					if (it1->mValue.key == chapterIter->mValue->m_nChapterId )
					{
						if(it1->mValue.value == 1)
						{
							getIt1 = 1;
						}						
						break;
					}

				}
				if (getIt1 == 1)
				{
					chapterNode[chapterIdStr]["isSelectUnexcepted"] =  1;//勾选入侵者
				}
				else
				{
					chapterNode[chapterIdStr]["isSelectUnexcepted"] =  0;
				}

				Json::Value stageNode;
				for(List<StageData>::Iter* stageIter = chapterIter->mValue->m_listStageDatas.begin();
					stageIter != NULL; stageIter = chapterIter->mValue->m_listStageDatas.next(stageIter))
				{
					if(stageIter->mValue.m_nStageID)
					{
						UInt32 challengeTimes = stageIter->mValue.m_nChallengTimes;
						UInt64 lastChallengeTime = stageIter->mValue.m_nLastChallengTime;
						UInt32 stageID = stageIter->mValue.m_nStageID;
						UInt8 getStar = stageIter->mValue.m_nGetStar;
						mystream.clear();
						mystream.str("");
						mystream << stageID;
						std::string stageIDStr;
						mystream >> stageIDStr;

						stageNode[stageIDStr]["starState"] = getStar;
						stageNode[stageIDStr]["challengeTimes"] = challengeTimes;
						stageNode[stageIDStr]["lastChallengeTime"] = lastChallengeTime;
					}
				}				

				chapterNode[chapterIdStr]["stage"] = stageNode;

			}

			for(MapRecord::Iter *iter = records.begin();iter!=NULL;iter=records.next(iter))
			{
				Json::Value son; 
				son["stageID"] = Json::Value(iter->mKey);
				son["playerID"] = Json::Value(iter->mValue.playerID);
				son["name"] = Json::Value(iter->mValue.name);
// 				son["modleID"] = Json::Value(iter->mValue.modleID);
				son["time"] = Json::Value(iter->mValue.val);

				chapterNode["recods"].append(son);
			}

			for(MapRecord::Iter *iter = myRecods.begin();iter!=NULL;iter=myRecods.next(iter))
			{
				Json::Value son; 
// 				son["stageID"] = Json::Value(iter->mKey);
// 				son["playerID"] = Json::Value(iter->mValue.playerID);
// 				son["name"] = Json::Value(iter->mValue.name);
// 				son["modleID"] = Json::Value(iter->mValue.modleID);
// 				son["time"] = Json::Value(iter->mValue.val);

				mystream.clear();
				mystream.str();

				mystream << iter->mKey;

				std::string stageIDStr;
				mystream >> stageIDStr;
				son[stageIDStr] = Json::Value(iter->mValue.val);

				chapterNode["myRecods"].append(son);
			}

			for(Map<UInt32,UInt32>::Iter *it = resetStages.begin();it!=NULL;it=resetStages.next(it))
			{
				Json::Value son; 

				mystream.clear();
				mystream.str();
				mystream << it->mKey;
				std::string stageIDStr;
				mystream >> stageIDStr;
				son[stageIDStr] = Json::Value(it->mValue);
				
				chapterNode["resetStages"].append(son);
			}


			node = chapterNode;
			Json::FastWriter writer;  
			std::string strWrite = writer.write(chapterNode);

			
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{	

		}


		Map<UInt16, PlayerChapterData *> m_mapIDToChapter;
		List<KeyValue> m_ChapterUnlockeded;
		List<KeyValue> m_mapIDToSelect;	
		Guid playerID;

		MapRecord records;
		MapRecord myRecods;

		Map<UInt32,UInt32> resetStages;


	};



} // namespace Lynx



#endif //__LYNX_COMMON_LIB_MSG_SKILLDATA_TO_JSON_H__