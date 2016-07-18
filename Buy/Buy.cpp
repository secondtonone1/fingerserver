#include "Buy.h"

#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
// #include "../CommonLib/PlayerData.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"


using namespace Lynx;

// 
// void BuyManager::onBuyStrength(const  ConnId& connId,CGStageMopUp &msg)
// {
// 
// 	msg.convertJsonToData(msg.jsonStr);
// 	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
// 	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
// 
// 	SaoDangTemplate saoDangTemplate = GlobalVarManager::getSingleton().getSaoDang();
// 
// 	PlayerChapterList chapterList;
// 	chapterList = player->getChapterData();
// // 	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
// // 	{
// // 		if (iter->mValue.m_nChapterId == StageManager::getSingleton().getChapterID(msg.id))
// // 		{
// // 			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
// // 			{
// // 				if (iter1->mValue.m_nStageID == msg.id)
// // 				{
// // 					todayFinishTimes = iter1->mValue.m_nToDayChallengTimes;
// // 				}
// // 			}
// // 		}
// // 	}
// // 
// // 	if (stageTemplate->mStrengthCost <	(todayFinishTimes + food1+food2+food3)	)
// // 	{
// // 		resp.result =LynxErrno::NumberNotRight;
// // 		std::string jsonStr = resp.convertDataToJson();	
// // 		NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
// // 		return;
// // 	}




// 
// 	for(List<UInt32>::Iter *iter = saoDangTemplate.prices.begin();iter!=NULL;iter = saoDangTemplate.prices.next(iter))
// 	{
// 		count ++;
// 		if (count  > mFireConfirmData.m_EliteMopUpTimes)
// 		{
// 			addTimes ++;
// 			if (addTimes >= food4)//次数够了
// 			{
// 				goldCost += iter->mValue;
// 				break;
// 			}
// 		}
// 	}
// 	if (addTimes < food4)//次数超过表里配的次数
// 	{
// 		for(List<UInt32>::Iter *iter = saoDangTemplate.prices.begin();iter!=NULL;iter = saoDangTemplate.prices.next(iter))
// 		{
// 			if (saoDangTemplate.prices.next(iter) == NULL)
// 			{
// 				goldCost += iter->mValue * (food4 - addTimes);
// 				addTimes ++;
// 			}
// 		}
// 	}
// }
// 
