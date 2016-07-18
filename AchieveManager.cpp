#include "AchieveManager.h"
#include "PersistSystem.h"
#include "Player.h"
#include "./FireConfirm/StageCalc.h"
#include "LogicSystem.h"
using namespace Lynx;

AchieveManager::AchieveManager()
{
	m_pAchieveList = NULL;
	m_achieveMap.clear();
	m_dailyTaskMap.clear();
	m_dlyEventMap.clear();
}


AchieveManager:: ~AchieveManager()
{

}

bool AchieveManager::initial(Player* player)
{
	m_pPlayer = player;
	m_pAchieveList = &(player->mPlayerData.mAchieveData.achieveList);
	for(List<AchieveData>::Iter * achItor = m_pAchieveList->begin(); achItor != NULL; achItor = m_pAchieveList->next(achItor))
	{
		m_achieveMap.insert(achItor->mValue.m_nEventId, &(achItor->mValue));
	}
	
	m_nPlayerUid = m_pPlayer->getPlayerGuid();

	m_pDailyTaskList = &(player->mPlayerData.mDailyTaskData.dailytaskList);

	for(List<DailyTaskData>::Iter * dailyIter = m_pDailyTaskList->begin(); dailyIter != NULL; dailyIter = m_pDailyTaskList->next(dailyIter))
	{
		m_dailyTaskMap.insert(dailyIter->mValue.m_nTempId,  &(dailyIter->mValue));
		m_dlyEventMap.insert(dailyIter->mValue.m_nEventId, &(dailyIter->mValue));
	}

	m_pdailyActiveData = &(m_pPlayer->mPlayerData.mDailyAcvData);

	return true;
}


void AchieveManager::release()
{
	m_achieveMap.clear();
	m_dailyTaskMap.clear();
	m_dlyEventMap.clear();
}

bool AchieveManager::acvArgMatch(UInt64 achieveId)
{
	AchievementTemplate * acvTemp = ACHIEVEMENT_TABLE().get(achieveId);
	assert(acvTemp);
	UInt64 acvEvent = acvTemp->mEvent;

	Map<UInt64,  AchieveData *>::Iter * acvDataIter = m_achieveMap.find(acvEvent);
	assert(acvDataIter);

	if(acvDataIter->mValue->m_nArg >= acvTemp->mArg)
	{
		return true;
	}
	else
	{
		return false;
	}
}

AchieveData * AchieveManager::getSameEventAcvData(UInt64 achieveId)
{
	AchievementTemplate * acvTemp = ACHIEVEMENT_TABLE().get(achieveId);
	assert(acvTemp);
	UInt64 acvEvent = acvTemp->mEvent;

	Map<UInt64,  AchieveData *>::Iter * acvDataIter = m_achieveMap.find(acvEvent);
	assert(acvDataIter);

	return acvDataIter->mValue;
}

std::string AchieveManager::convertAchieveDataToJson()
{
	Json::Value root;
	for(Map<UInt64,  AchieveData *>::Iter * achItor = m_achieveMap.begin(); achItor != NULL; 
		achItor = m_achieveMap.next(achItor))
	{
		Json::Value achRoot;
		achRoot["achievementid"] = achItor->mValue->m_nAchieveId;
		achRoot["event"] = achItor->mValue->m_nEventId;
		achRoot["arg"] = achItor->mValue->m_nArg;
		achRoot["flag"] = achItor->mValue->m_nFlag;
		root["achievement"].append(achRoot);
	}

	for(Map<UInt64, DailyTaskData*>::Iter * dailyIter = m_dailyTaskMap.begin();  dailyIter != NULL; 
		dailyIter = m_dailyTaskMap.next(dailyIter))
	{
		Json::Value dailyRoot;
		dailyRoot["dailyid"] = dailyIter->mValue->m_nTempId;
		DailyTaskTemplate * dailyTemp = DAILYTASK_TABLE().get(dailyIter->mKey);
		if(dailyTemp)
		{
					dailyRoot["event"] = dailyTemp->mEvent;
		}

		dailyRoot["arg"] = dailyIter->mValue->m_nArg;
		dailyRoot["flag"] = dailyIter->mValue->m_nFlag;

		root["daily"].append(dailyRoot);
	}

	for(List<UInt32>::Iter * activeIter = m_pdailyActiveData->flags.begin();  activeIter!=NULL; 
		activeIter = m_pdailyActiveData->flags.next(activeIter))
	{
		root["active"]["flags"].append(activeIter->mValue);
	}
	
	root["active"]["curactive"] = m_pdailyActiveData->curActive;

	Json::StyledWriter writer;
	return writer.write(root);
}

void AchieveManager::postUpdateAchMsg(UInt64 playerUid,  const AchieveData & achData)
{
		AchieveUpdateMsg achUpdateMsg;
		achUpdateMsg.playerUid = playerUid;
		achUpdateMsg.achieveData = achData;
		PersistSystem::getSingleton().postThreadMsg(achUpdateMsg, playerUid);
}

void AchieveManager::postUpdateDailyMsg(UInt64 playerUid,  const DailyTaskData & dailyTaskData)
{
		PersistDailyTaskUpdateMsg dailyUpdateMsg;
		dailyUpdateMsg.playerUid = playerUid;
		dailyUpdateMsg.dailyTask = dailyTaskData;
		PersistSystem::getSingleton().postThreadMsg(dailyUpdateMsg, playerUid);
}

void AchieveManager::updateAchieveData(UInt64 eventType, UInt32 change)
{
	Map<UInt64,  AchieveData *>::Iter * findIter = m_achieveMap.find(eventType);

	if(findIter)
	{
		//判断该成就是否是最后一条，并且最后一条已经完成
		//只有最后一条成就领取奖励后标记位才会被设置为1
		if(findIter->mValue->m_nFlag)
		{
			//该成就已经完成，不需要再记录进度了
			return;
		}

		switch(eventType)
		{
			case  BARRIER  :
			{
				//根据当前任务id找到对应的任务结构体
				AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(findIter->mValue->m_nAchieveId);
				//任务需要的参数和change相同，此时change表示关卡id
				if(achieveTemp && achieveTemp->mArg == change)
				{
						findIter->mValue->m_nArg = change;
						//存盘
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				}
			
			}
			
			break;

		case	  ELITEBARRIER :
			{

				//根据当前任务id找到对应的任务结构体
				AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(findIter->mValue->m_nAchieveId);
				//任务需要的参数和change相同，此时change表示关卡id
				if(achieveTemp && achieveTemp->mArg == change)
				{
						findIter->mValue->m_nArg = change;
						//存盘
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				}
			
			}
			
			break;
		case	  RHYMEENHANCELV :
			{
				//change 参数表示韵文阶数和级数组成的数字，如3阶四级，表示3004
			
			   //记录当前强化进度
				findIter->mValue->m_nArg = change;
			   //存盘
			   postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				
			
			}
			
			break;
		case RHYMESKILLACTIVE:
			//韵功激活成就去掉
			break;

		case	  HONORSTONEACTIVE :
			{
				//change 参数表示操作激活界石的个数，改变值，激活两个就传2
				
				
				findIter->mValue->m_nArg += change;
				
					//存盘
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				

			}
			
			break;
		case GETSERVANT:
			{
				//change 参数表示招募佣兵的个数，改变值，招募两个就传2

				findIter->mValue->m_nArg += change;

				//存盘
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));

			}
		
			break;

		case	  PLAYERLV :
			{
				//change表示玩家等级最终值，最终值，
				findIter->mValue->m_nArg = change;

				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case	  VIPLV :
			{
				//change表示玩家vip等级改变的数量，改变值，升两级就传2
				findIter->mValue->m_nArg += change;

				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case  POWER:
			{
				//power表示战力最终的值，最终值，当前战力为10000，就是10000
				findIter->mValue->m_nArg  = change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;

		case	  OWN4STARSERVANT :
			{
				//change 表示新获得的四星佣兵数量，改变值，新升星获得获得n个4星佣兵
				//change = n;
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case	  OWNPURPULESERVANT :
			{
				//change 表示新获得的紫色佣兵数量，改变值，
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case	  UNLOCKSERVANTASSIS :
			{
				//新增解锁助阵位个数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}

			break;
		case	  OWNGREENASSIST :
			{
				//新培养的绿色助阵位，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case OWNBLUEASSIST:
			{
				//新培养的蓝色助阵位，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
	
			break;
		case OWNPURPLEASSIST:
			{
				//新培养的紫色助阵位，改变值
					findIter->mValue->m_nArg += change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
	
			break;
		case QUALIFYDANLV:
			{
					//排位赛段数增加，改变值
					findIter->mValue->m_nArg += change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case  QUALIFYSUCCESS:
			{
				//排位赛胜利次数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case  ARIEASUCCESS:
			{
				//竞技场胜利次数，改变值
					findIter->mValue->m_nArg += change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
	
			break;
		case BUYCHARACTOR:
			{
				//购买角色个数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case COURAGESUCCESS:
			{
				//勇气试炼胜利次数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case WXCHALLENGESUC:
			{
				//无限挑战难度参数赋值
				findIter->mValue->m_nArg = change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case USERHYMESKILL:
			{
				//释放韵功次数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case USERHYMESPEED:
			{
				//释放韵力激发次数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case CALLSERVANTCNT:
			{
				//战斗中召唤佣兵次数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case FRIENDCNT:
			{
				//增加好友个数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case MAXGEMLV:
			
				//拥有宝石最高等级，最终值
				if(findIter->mValue->m_nArg < change)
				{
					findIter->mValue->m_nArg =change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				}
				
			
		
			break;
	
		case LINGRENCALLSERVANT:
			{
				//伶人召唤次数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			

			break;
		case 	XIBANCALLSERVANT:
			{
				//戏班召唤次数，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case MINGLINGCALLSERVANT:
			{
				//名伶召唤，改变值
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		default:
			break;
		}
	}
	//cqy
	InlineActivityManager::getSingleton().updateSevenDayTaskData(m_nPlayerUid,eventType,change);
}


//注意日常任务更新的change都是改变值，所以只需要增加即可
void AchieveManager::updateDailyTaskData(UInt64 eventType,   UInt32 change)
{
	
		Map<UInt64, DailyTaskData *>::Iter * findDlyIter = m_dlyEventMap.find(eventType);
		if(!findDlyIter)
		{
			return;
		}
		//奖励已经领取就不再打点
		if(findDlyIter->mValue->m_nFlag)
		{
			return;
		}

		//增加事件对应的操作次数
		findDlyIter->mValue->m_nArg += change;
		postUpdateDailyMsg(m_nPlayerUid, *(findDlyIter->mValue));

		//cqy
		InlineActivityManager::getSingleton().updateSevenDayTaskData(m_nPlayerUid,eventType,change);

}



void AchieveManager::dealAddDailyReq(UInt64 dailytaskId)
{
	
}

void AchieveManager::getActiveAward(UInt64 index)
{
		const ConnId & connId = m_pPlayer->getConnId();
		List<UInt32>::Iter * listIter = m_pdailyActiveData->flags.getn(index-1);

		if(!listIter)
		{
			GCActiveAwardGetResp getResp;
			getResp.mPacketID = BOC_ACTIVEAWARDGET_RESP;
			Json::Value root;
			Json::StyledWriter writer;
			root["errorId"] = LynxErrno::InvalidParameter;
			getResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(getResp, connId);

			return;
		}

		if(listIter->mValue == 1)
		{
			GCActiveAwardGetResp getResp;
			getResp.mPacketID = BOC_ACTIVEAWARDGET_RESP;
			Json::Value root;
			Json::StyledWriter writer;
			root["errorId"] = LynxErrno::InvalidParameter;
			getResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(getResp, connId);
			return ;
		}

		//设置标记位为1
		listIter->mValue = 1;
		//发奖励

		
		//活跃度表存盘
		PersistDailyActiveUpdateMsg dailyacvMsg;
		dailyacvMsg.playerUid = m_nPlayerUid;
		dailyacvMsg.dailyacvData = m_pPlayer->mPlayerData.mDailyAcvData;
		PersistSystem::getSingleton().postThreadMsg(dailyacvMsg, m_nPlayerUid);

		DailyActiveTemplate * dailyActiveTemp = DAILYACTIVE_TABLE().get(index);
		//下发奖励
		getContant(dailyActiveTemp->mItemList);


		//拼串发给客户端
		sendDlyActiveResp(index, dailyActiveTemp->mItemList);

}

void AchieveManager::finishDailyTask(UInt64 dailytaskId)
{
	const ConnId & connId = m_pPlayer->getConnId();

	Map<UInt64, DailyTaskData*>::Iter * findDailyIter = m_dailyTaskMap.find(dailytaskId);
	if(!findDailyIter)
	{
		GCDailyTaskFinishResp finishResp;
		finishResp.mPacketID = BOC_DAILYTASKFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::DailyTaskIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr = writer.write(root);
		cout << finishResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return ;
	}

	DailyTaskTemplate * dailyTaskTemp = DAILYTASK_TABLE().get(dailytaskId);
	if(findDailyIter->mValue->m_nArg < dailyTaskTemp->mArg)
	{

		GCDailyTaskFinishResp finishResp;
		finishResp.mPacketID = BOC_DAILYTASKFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr = writer.write(root);
		cout << finishResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return;
	}

	//设置领取标志位
	findDailyIter->mValue->m_nFlag = 1;
	

	//增加活跃度
	m_pPlayer->mPlayerData.mDailyAcvData.curActive += dailyTaskTemp->mActive;
	//活跃度表存盘
	PersistDailyActiveUpdateMsg dailyacvMsg;
	dailyacvMsg.playerUid = m_nPlayerUid;
	dailyacvMsg.dailyacvData = m_pPlayer->mPlayerData.mDailyAcvData;
	PersistSystem::getSingleton().postThreadMsg(dailyacvMsg, m_nPlayerUid);

	//下发奖励
	getContant(dailyTaskTemp->mItemList);

	//日常任务完成存盘
	postUpdateDailyMsg(m_nPlayerUid, *(findDailyIter->mValue));

	

	//拼串发给客户端
	sendDlyResp2Client(dailytaskId,findDailyIter->mValue->m_nEventId, findDailyIter->mValue->m_nArg, findDailyIter->mValue->m_nFlag, dailyTaskTemp->mItemList);

}

bool AchieveManager::finishAchieve(UInt64 eventType, UInt64 achId)
{
	Map<UInt64,  AchieveData *>::Iter * findIter = m_achieveMap.find(eventType);
	if(!findIter)
	{ 
		const ConnId & connId = m_pPlayer->getConnId();
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEDAILYLIST_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::EventNotFind;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	switch(eventType)
	{
			case	  PLAYERLV :
			{
				return dealAchPlayerLv(eventType, achId, findIter->mValue);
			}
	
			break;

			case  BARRIER  :
				{
					return dealAchBarrier(eventType, achId, findIter->mValue);
				}

				break;

			case	  ELITEBARRIER :
				{
					return dealAchEliteBarrier(eventType,achId, findIter->mValue);
				}

				break;
			case	  RHYMEENHANCELV :
				{
					return dealAchRhymeEnhance(eventType,achId, findIter->mValue);
				}

				break;
			case RHYMESKILLACTIVE:
				//韵功激活成就去掉
				break;

			case	  HONORSTONEACTIVE :
				{
					return dealAchHonStoneActive(eventType,achId, findIter->mValue);

				}

				break;
			case GETSERVANT:
				{
					return dealgetAchServant(eventType,achId, findIter->mValue);

				}

				break;

			
			case	  VIPLV :
				{
					return dealAchVipLv(eventType,achId, findIter->mValue);
				}

				break;
			case  POWER:
				{
					return dealAchPower(eventType,achId, findIter->mValue);
				}

				break;

			case	  OWN4STARSERVANT :
				{
					return dealAch4StarServant(eventType,achId, findIter->mValue);
				}

				break;
			case	  OWNPURPULESERVANT :
				{
					return dealAchPurpleServant(eventType,achId, findIter->mValue);
				}

				break;
			case	  UNLOCKSERVANTASSIS :
				{
					return dealAchAssistOpen(eventType,achId, findIter->mValue);
				}

				break;
			case	  OWNGREENASSIST :
				{
					
				}

				break;
			case OWNBLUEASSIST:
				{
				
				}

				break;
			case OWNPURPLEASSIST:
				{
					return dealAchPurpleAssist(eventType,achId, findIter->mValue);
				}

				break;
			case QUALIFYDANLV:
				{
					
				}

				break;
			case  QUALIFYSUCCESS:
				{
					return dealAchQualifySucCnt(eventType,achId, findIter->mValue);
				}

				break;
			case  ARIEASUCCESS:
				{
					return dealAchAreaSucCnt(eventType,achId, findIter->mValue);
				}

				break;
			case BUYCHARACTOR:
				{
					return dealAchBuyCharactorCnt(eventType,achId, findIter->mValue);
				}

				break;
			case COURAGESUCCESS:
				{
					return dealCourageSucCnt(eventType,achId, findIter->mValue);
				}

				break;
			case WXCHALLENGESUC:
				{
					return dealWXChallengeCnt(eventType,achId, findIter->mValue);
				}

				break;
			case USERHYMESKILL:
				{
					
				}

				break;
			case USERHYMESPEED:
				{
					
				}

				break;
			case CALLSERVANTCNT:
				{
					
				}

				break;
			case FRIENDCNT:
				{
						return dealAchFriendCnt(eventType,achId, findIter->mValue);
				}

				break;
		
			case MAXGEMLV:
				{
					return dealAchGemMaxLv(eventType,achId, findIter->mValue);
				}
				break;
			case LINGRENCALLSERVANT:
				{
					
					return dealAchLingRenCall(eventType,achId, findIter->mValue);
				}


				break;
			case 	XIBANCALLSERVANT:
				{
					return dealAchXibanCall(eventType,achId, findIter->mValue);
				}

				break;
			case MINGLINGCALLSERVANT:
				{
					return dealAchMingLingCall(eventType,achId, findIter->mValue);
				}

				break;
		
			

			default:
				break;

	}


	return true;
}

bool AchieveManager::dealAchPlayerLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配
		
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配
		
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断玩家等级是否满足条件
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;

	}
	
	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);
	return true;

}

bool AchieveManager::dealAchBarrier(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}
	
	//判断关卡成就参数是否完成
	if(pAchData->m_nArg  !=  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//判断下一个成就需要的关卡是否通关，通关则变为下一个成就关卡id，否则为0
		///////////////
		////////
		AchievementTemplate*  newAchieve = ACHIEVEMENT_TABLE().get(pAchData->m_nAchieveId);
		bool isPass = StageCalcManager::getSingleton().checkStageClearance(m_nPlayerUid,newAchieve->mArg);
		if(isPass)
		{
				pAchData->m_nArg = newAchieve->mArg;
		}
		else
		{
				pAchData->m_nArg = 0;
		}
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

		return true;
}

bool AchieveManager::dealAchEliteBarrier(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断关卡成就参数是否完成
	if(pAchData->m_nArg  !=  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//判断下一个成就需要的关卡是否通关，通关则变为下一个成就关卡id，否则为0
		///////////////
		////////

		AchievementTemplate*  newAchieve = ACHIEVEMENT_TABLE().get(pAchData->m_nAchieveId);
		bool isPass = StageCalcManager::getSingleton().checkStageClearance(m_nPlayerUid,newAchieve->mArg);
		if(isPass)
		{
			pAchData->m_nArg = newAchieve->mArg;
		}
		else
		{
			pAchData->m_nArg = 0;
		}
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);
	
	return true;
}

bool AchieveManager::dealAchRhymeEnhance(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断韵文成就参数是否完成
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//韵文强化不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchHonStoneActive(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断界石成就参数是否完成
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//界石激活个数不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealgetAchServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断获得佣兵成就参数是否完成
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//拥有佣兵个数不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchVipLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断vip等级是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//拥有佣兵个数不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchPower(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAch4StarServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断4星佣兵数量是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//拥有佣兵个数不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchPurpleServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断紫色佣兵数量是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//拥有紫色佣兵个数不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchAssistOpen(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断开启的助阵位数量是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//拥有助阵位个数不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchPurpleAssist(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断紫色助阵位数量是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		//拥有紫色助阵位个数不需要更改arg
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchQualifySucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断排位赛胜利次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchAreaSucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断竞技场胜利次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchBuyCharactorCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断购买角色次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealCourageSucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断勇气试炼成功次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealWXChallengeCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断无限挑战次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;
		
	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchFriendCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断好友个数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchGemMaxLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断宝石等级是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchLingRenCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断伶人招募次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchXibanCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断戏班招募次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchMingLingCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//任务id不匹配

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断名伶招募次数是否满足
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//判断该成就是否达到最后一步，
	//最后一步只需要将领取标记设置为1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//更新到下一个任务id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//下一个任务id对应的完成标记为0；
		pAchData-> m_nFlag = 0;

	}

	//下发奖励
	getContant(achieveTemp->mItemList);

	//存盘
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//拼串发给客户端
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}


void AchieveManager::getContant(const List<AllItemEle> &contantList)
{
	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL; 
		contantIter = contantList.next(contantIter))
	{
		m_pPlayer->getAllItemManager().addAwardMaterial(contantIter->mValue.resType,contantIter->mValue.subType,
			contantIter->mValue.count);
	}
		
}

void AchieveManager::sendResp2Client(UInt64 oldAchId, UInt64 newAchId,  UInt64 eventId, UInt32 arg, UInt32 flag,
									 const List<AllItemEle> &contantList)
{

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["oldachieveid"] = oldAchId;
	root["newachieveid"] = newAchId;
	root["event"] = eventId;
	root["arg"] = arg;
	root["flag"] = flag;

	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL;
		contantIter = contantList.next(contantIter) )
	{
		if(contantIter->mValue.resType == AWARD_SERVANT ||  contantIter->mValue.resType == AWARD_SERVANTPIECE)
		{
			ServantData * servantData = m_pPlayer->getServantManager().getServantById(contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = servantData->pieceCount;
				

				////////////////////////////////////////////////////////////////////////
				
			itemRoot["attr"]["servantUid"] = servantData->servantUid;
			itemRoot["attr"]["servantId"] = servantData->servantId;
			itemRoot["attr"]["pieceCount"] = servantData->pieceCount;
			itemRoot["attr"]["level"] = servantData->level;
			itemRoot["attr"]["lvexp"] = servantData->lvexp;
			itemRoot["attr"]["star"] = servantData->star;
			itemRoot["attr"]["floor"] = servantData->floor;

			for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
					treasureIter = servantData->equipTreasures.next(treasureIter))
				{
					itemRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
				}

				root["contant"].append(itemRoot);
		} //if
		else
		{
			
				UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(contantIter->mValue.resType, contantIter->mValue.subType);
				Json::Value itemRoot;
				itemRoot["resType"] = contantIter->mValue.resType;
				itemRoot["subType"] = contantIter->mValue.subType;
				itemRoot["count"] = count;

				root["contant"].append(itemRoot);
			

			}//else

		}//for

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
	
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
		cout << finishResp.mRespJsonStr; 
		const ConnId & connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

	}


void AchieveManager::sendDlyResp2Client(UInt64 dlyId , UInt64 eventId, UInt32 arg, UInt32 flag,
						const List<AllItemEle> &contantList)
{

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["dailyid"] = dlyId;
	root["event"] = eventId;
	root["arg"] = arg;
	root["flag"] = flag;
	root["curactive"] = m_pPlayer->mPlayerData.mDailyAcvData.curActive;

	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL;
		contantIter = contantList.next(contantIter) )
	{
		if(contantIter->mValue.resType == AWARD_SERVANT ||  contantIter->mValue.resType == AWARD_SERVANTPIECE)
		{
			ServantData * servantData = m_pPlayer->getServantManager().getServantById(contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = servantData->pieceCount;


			////////////////////////////////////////////////////////////////////////

			itemRoot["attr"]["servantUid"] = servantData->servantUid;
			itemRoot["attr"]["servantId"] = servantData->servantId;
			itemRoot["attr"]["pieceCount"] = servantData->pieceCount;
			itemRoot["attr"]["level"] = servantData->level;
			itemRoot["attr"]["lvexp"] = servantData->lvexp;
			itemRoot["attr"]["star"] = servantData->star;
			itemRoot["attr"]["floor"] = servantData->floor;

			for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
				treasureIter = servantData->equipTreasures.next(treasureIter))
			{
				itemRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
			}

			root["contant"].append(itemRoot);
		} //if
		else
		{

			UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(contantIter->mValue.resType, contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = count;

			root["contant"].append(itemRoot);


		}//else

	}//for

	GCDailyTaskFinishResp finishResp;
	finishResp.mPacketID = BOC_DAILYTASKFINISH_RESP;

	Json::StyledWriter writer;
	finishResp.mRespJsonStr =  writer.write(root);
	cout << finishResp.mRespJsonStr; 
	const ConnId & connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(finishResp, connId);


}


void AchieveManager::sendDlyActiveResp(UInt32 index, const List<AllItemEle> &contantList)
{
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	for(List<UInt32>::Iter * flagIter = m_pdailyActiveData->flags.begin();  flagIter != NULL;  
		flagIter = m_pdailyActiveData->flags.next(flagIter))
	{
		root["flags"].append(flagIter->mValue);
	}
	
	root["curactive"] = m_pPlayer->mPlayerData.mDailyAcvData.curActive;


	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL;
		contantIter = contantList.next(contantIter) )
	{
		if(contantIter->mValue.resType == AWARD_SERVANT ||  contantIter->mValue.resType == AWARD_SERVANTPIECE)
		{
			ServantData * servantData = m_pPlayer->getServantManager().getServantById(contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = servantData->pieceCount;


			////////////////////////////////////////////////////////////////////////

			itemRoot["attr"]["servantUid"] = servantData->servantUid;
			itemRoot["attr"]["servantId"] = servantData->servantId;
			itemRoot["attr"]["pieceCount"] = servantData->pieceCount;
			itemRoot["attr"]["level"] = servantData->level;
			itemRoot["attr"]["lvexp"] = servantData->lvexp;
			itemRoot["attr"]["star"] = servantData->star;
			itemRoot["attr"]["floor"] = servantData->floor;

			for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
				treasureIter = servantData->equipTreasures.next(treasureIter))
			{
				itemRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
			}

			root["contant"].append(itemRoot);
		} //if
		else
		{

			UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(contantIter->mValue.resType, contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = count;

			root["contant"].append(itemRoot);


		}//else

	}//for

	GCActiveAwardGetResp getResp;
	getResp.mPacketID = BOC_ACTIVEAWARDGET_RESP;

	Json::StyledWriter writer;
	getResp.mRespJsonStr =  writer.write(root);
	cout << getResp.mRespJsonStr; 
	const ConnId & connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(getResp, connId);

}



	