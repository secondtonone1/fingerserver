#include "FireConfirmManager.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "../TimeManager.h"
#include "PVPSystem.h"
#include "../Ranking/Ranking.h"
#include "StageCalc.h"
#include "ClimbTower.h"
#include "NewException.h"
#include "Box.h"
#include "Shop.h"
#include "ShopTable.h"
#include "CourageChallenge.h"
#include "BaseChang.h"

//for test
#include "Code.h"

using namespace Lynx;

FireConfirmManager::FireConfirmManager()
{
}
FireConfirmManager::~FireConfirmManager()
{
	
}

bool FireConfirmManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	// 	m_pPlayer = player;
	return true;
}

void FireConfirmManager::release()
{
}

//在n个数中随机选取不相同的m个
void FireConfirmManager::ChoiseNFromEnum(UInt32 num ,UInt32 maxnum,List<UInt32> &OutList)
{

	UInt32 randomnum;
	char Arry[32];
	for (int i =0;i< maxnum; i++)
	{
		Arry[i] = '0';
	}

	if (num<1)
	{
		return ;
	}
	if (num > maxnum-1)
	{
		return ;
	}
	do
	{
		randomnum = rand()%(maxnum);
		if (Arry[randomnum] == '0')
		{
			Arry[randomnum] = '1';
			OutList.insertHead(randomnum + 1);//1 2 3...
		}
	}while ( OutList.size() < num);
	return ;
}


//根据等级获取需验证几项
UInt32 FireConfirmManager::GetConfirmTimes(UInt32 ConfirmLevel)
{
	UInt32 tmp=(UInt32) CONFIRM_LEVEL1_NEED_NUM;

	if (ConfirmLevel==1)
	{
		return (UInt32)CONFIRM_LEVEL1_NEED_NUM;
	}
	else if (ConfirmLevel==2)
	{
		return (UInt32)CONFIRM_LEVEL2_NEED_NUM;
	}
	else if (ConfirmLevel==3)
	{
		return (UInt32) CONFIRM_LEVEL3_NEED_NUM;
	}
	return -1;
}

void FireConfirmManager::UpdataConfirmLevel(ConnId connId,bool flag)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	FireConfirmData = player->GetFireConfirmData();

	if (flag == true)//验证正确
	{
		FireConfirmData.m_ConfirnRightTimes ++;
	}
	else if (flag == false)//验证失败
	{
		FireConfirmData.m_ConfirnFailTimes ++;
		FireConfirmData.m_CopyStartTime = -1;
	}

	if (FireConfirmData.m_ConfirmLevel == 1) //是否升级验证等级
	{
		if (FireConfirmData.m_ConfirnFailTimes == (UInt32)CONFIRM_LEVEL1_ERROR_TIMES)
		{
			FireConfirmData.m_ConfirmLevel++;
			FireConfirmData.m_ConfirnFailTimes = 0;		

		}		 
	}
	else if (FireConfirmData.m_ConfirmLevel== 2)
	{
		if (FireConfirmData.m_ConfirnFailTimes == (UInt32)CONFIRM_LEVEL2_ERROR_TIMES)
		{
			FireConfirmData.m_ConfirmLevel++;
			FireConfirmData.m_ConfirnFailTimes = 0;		

		}
	}
	else if (FireConfirmData.m_ConfirmLevel== 3)
	{

		if (FireConfirmData.m_ConfirnFailTimes == (UInt32)CONFIRM_LEVEL3_SEND_MAIL)
		{
			FireConfirmData.m_ConfirmLevel++;			 
			FireConfirmData.m_ConfirnRightTimes =0;

		}
		if (FireConfirmData.m_ConfirnFailTimes == (UInt32)CONFIRM_LEVEL3_FORBID_LOGIN)
		{
			FireConfirmData.m_ConfirnRightTimes =0;
			FireConfirmData.m_ConfirnFailTimes = 0;
			
			player->SetFireConfirmData(FireConfirmData);
			SendForbiddenLogin(connId);
			return;			
		}
	}
	else if (FireConfirmData.m_ConfirnRightTimes==(UInt32)CONFIRM_LEVEL_RESTET_TIMES)
	{
		FireConfirmData.m_ConfirmLevel=1;
		FireConfirmData.m_ConfirnFailTimes=0;
		FireConfirmData.m_ConfirnRightTimes=0;
	}	
	player->SetFireConfirmData(FireConfirmData);
}

void FireConfirmManager::ConfirmDataReq(const ConnId& connId, CGConfirmDataReq& msg)
{
// 	UInt32 iFlag = 0;
// 	bool bFlag = false;
// 	List<Int32> RecList;
// 
// 	msg.convertJsonToData(msg.strConfirmData);
// 	iFlag = msg.confirmID;
// 	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
// 	if (player == NULL)
// 	{
// 		LOG_WARN("player not found!!");
// 		return;
// 	}
// 
// 	List<ItemData>* m_pListItemData = new List<ItemData>();
// 	ItemData data;
// 	*m_pListItemData->insertTail(data);
// 	m_pListItemData->insertTail(data);
// 	List<ItemData>::Iter *it = m_pListItemData->begin();
// 
// 
// 	FireConfirmData = player->GetFireConfirmData();
// 	if (FireConfirmData.m_RecConfirmIDs.size()<31)
// 	{
// 		FireConfirmData.m_RecConfirmIDs.insertTail(iFlag);
// 		player->SetFireConfirmData(FireConfirmData);
// 	}
// 
// 	for( List<Int32>::Iter * it = msg.confirmDataList.begin(); it != NULL; it = msg.confirmDataList.next(it))
// 	{
// 		RecList.insertTail(it->mValue);
// 	}
// 
// 	HeroTemplate herotemplate;
// 	for (Map<UInt32, HeroTemplate>::Iter *iter = gHeroTable->mMap.begin();iter != NULL;iter = gHeroTable->mMap.next(iter))
// 	{
// 		if ((iter->mKey) == player->mPlayerData.mBaseData.m_nModelID)
// 		{
// 			herotemplate = iter->mValue; 
// 		}
// 	}
// 
// 	if (iFlag == CONFIRM_SPEED ) //RecList //speed speed
// 	{
// 
// 		Int32 MaxSpeed = herotemplate.mMoveSpeed;			
// 
// 		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 		{
// 
// 			if (MaxSpeed * OUT_OF_RANGE < iter->mValue )
// 			{
// 				bFlag = false;
// 			}
// 			else
// 			{
// 				bFlag = true;
// 			}
// 		}
// 
// 		UpdataConfirmLevel( connId, bFlag);
// 	}
// 	if (iFlag == CONFIRM_ATTACK_RATE ) //RecList //time1 time2
// 	{
// 		double AttackSpeed =-2/1/* herotemplate.mAttackSpeed*/;
// 		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 		{
// 
// 			if (RecList.next(iter) == NULL)
// 			{
// 				break;
// 			}
// 
// 			if (AttackSpeed* OUT_OF_RANGE < 1/( ( RecList.next(iter)->mValue) -  (iter->mValue) ) )
// 			{
// 				bFlag = false;
// 				break;
// 			}
// 			else
// 			{
// 				bFlag = true;
// 			}
// 		}
// 
// 		UpdataConfirmLevel( connId, bFlag);
// 	}
// 
// 	if (iFlag == CONFIRM_SKILL_CD ) //RecList //time1 skill1 time2  skill2 
// 	{
// 		Int32 SkillCDTime = 0;	
// 		SkillData skillData;
// 		List<SkillData> listskills =  player->mPlayerData.mSkillListData.m_listSkills;
// 		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 		{
// 			iter = RecList.next(iter);
// 			if (iter == NULL)
// 			{
// 				break;
// 			}
// 			//两个技能之间释放间隔
// 
// 
// 			for( List<Int32>::Iter * iter1 = RecList.next(iter); iter1 != NULL; iter1 = RecList.next(iter1)) //判断一个技能释放后到下一次释放时间
// 			{
// 				iter1 = RecList.next(iter1);
// 				if (iter1 == NULL)
// 				{
// 					break;
// 				}
// 
// 				if(iter->mValue == iter1->mValue)
// 				{
// 					if (((RecList.prev(iter1)->mValue - RecList.prev(iter1)->mValue) * OUT_OF_RANGE) < SkillCDTime)
// 					{
// 						bFlag = false;
// 						break;
// 					}
// 					bFlag = true;
// 				}
// 			}
// 			iter = RecList.next(iter);			 
// 		}
// 
// 		UpdataConfirmLevel( connId, bFlag);
// 	}
// 	//mark:技能伤害还没读配置文件 ActorSkillInfoMap
// 	if (iFlag == CONFIRM_SKILL_DAMAGE ) //RecList //damage1 skill1 damage2  skill2 
// 	{
// 
// // 		Int32 Damage;
// 
// 		//sec
// 		//SkillTemplate skilltemplate;
// 
// 		List<SkillData> listskills =  player->mPlayerData.mSkillListData.m_listSkills;
// 		for( List<Int32>::Iter * iter = RecList.next( RecList.begin() ); iter != NULL; iter = RecList.next(iter))
// 		{
// 
// 			/*skilltemplate = *gSkillTable->get(RecList.next(iter)->mValue);
// 
// 			Damage = skilltemplate.mPhysicsAttack + skilltemplate.mMagicAttack;
// 			if (Damage * OUT_OF_RANGE <  RecList.prev(iter)->mValue)
// 			{
// 				bFlag = false;
// 			}
// 			else
// 			{
// 				bFlag = true;
// 			}
// 
// 
// 			iter = RecList.next(iter);*/
// 
// 		}
// 
// 		UpdataConfirmLevel( connId, bFlag);
// 	}
// 	if (iFlag == CONFIRM_CRITICAL_STRIKE_RATE )//mark:难以计算 暂放下
// 	{
// 		//BattleExtendData
// 		Int32 SkillID = 0;
// 		Int32 SkillCDTime = 0;
// 		BattleExtendData battleextenddata;
// 		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 		{
// 			if (SkillID ==  iter->mValue)
// 			{
// 				if (SkillCDTime * OUT_OF_RANGE <  iter->mValue )
// 				{
// 					bFlag = false;
// 				}
// 				else
// 				{
// 					bFlag = true;
// 				}
// 			}
// 			iter = RecList.next(iter);
// 
// 		}
// 
// 		UpdataConfirmLevel( connId, bFlag);
// 	}
// 	if (iFlag == CONFIRM_CRITICAL_STRIKE_MAX_DAMAGE )
// 	{
// 		Int32 SkillID = 0;
// 		Int32 SkillCDTime = 0;
// 		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 		{
// 			if (SkillID ==  iter->mValue)
// 			{
// 				if (SkillCDTime * OUT_OF_RANGE <  iter->mValue )
// 				{
// 					bFlag = false;
// 				}
// 				else
// 				{
// 					bFlag = true;
// 				}
// 			}
// 			iter = RecList.next(iter);
// 
// 		}
// 
// 		UpdataConfirmLevel( connId, bFlag);
// 	}
// 	if (iFlag == CONFIRM_SKILL_RANGE )//mark:技能范围没读取配置文件 //skillmulti.xlsx attackarea1? //skillid posx,posy,posx2,posy2
// 	{
// 		Int32 SkillID = 0;
// 		Int32 SkillCDTime = 0;		
// 		Int32 SkillLevel;
// 		Int32 Damage;
// 
// 		List<SkillData> listskills =  player->mPlayerData.mSkillListData.m_listSkills;
// 
// 		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 		{
// 
// 			for( List<SkillData>::Iter * it = listskills.begin(); it != NULL; it = listskills.next(it))			 
// 			{				  
// 				SkillID = (Int32) (it->mValue.m_nID);
// 				SkillCDTime = (Int32) (it->mValue.m_nCD);
// 				SkillLevel = (Int32) (it->mValue.m_nLevel);
// 
// 				if (SkillID ==  iter->mValue)
// 				{
// 					Damage = SkillLevel;
// 
// 					if (SkillCDTime * OUT_OF_RANGE <  RecList.prev(iter)->mValue)
// 					{
// 						bFlag = false;
// 					}
// 					else
// 					{
// 						bFlag = true;
// 					}
// 				}
// 				iter = RecList.next(iter);
// 			}
// 			UpdataConfirmLevel( connId, bFlag);
// 		}
// 
// 		if (iFlag == CONFIRM_POS )
// 		{
// 			Int32 SkillID = 0;
// 			Int32 SkillCDTime = 0;
// 			for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 			{
// 				if (SkillID ==  iter->mValue)
// 				{
// 					if (SkillCDTime * OUT_OF_RANGE <  iter->mValue )
// 					{
// 						bFlag = false;
// 					}
// 					else
// 					{
// 						bFlag = true;
// 					}
// 				}
// 				iter = RecList.next(iter);
// 			}
// 
// 			UpdataConfirmLevel( connId, bFlag);
// 		}
// 		if (iFlag == CONFIRM_HP || iFlag == CONFIRM_MP || iFlag == CONFIRM_SP)//受到攻击 吃药膏 一段时间后血量变化
// 		{
// 			Int32 ChangValue = 0;
// 			Int32 AllUpDownValue = 0;
// 
// 			ChangValue =(RecList.end()->mValue - RecList.begin()->mValue)* OUT_OF_RANGE ;
// 
// 			for( List<Int32>::Iter * iter = RecList.next(RecList.begin()); iter != RecList.prev(RecList.end()); iter = RecList.next(iter))
// 			{
// 				AllUpDownValue += iter->mValue;
// 			}
// 
// 			if (ChangValue < 0)
// 			{
// 				if (ChangValue > AllUpDownValue)//-120 > -100
// 				{
// 					bFlag = false;
// 				}
// 				else
// 				{
// 					bFlag = true;
// 				}
// 			}
// 			else
// 			{
// 				if (ChangValue < AllUpDownValue)
// 				{
// 					bFlag = false;
// 				}
// 				else
// 				{
// 					bFlag = true;
// 				}
// 			}			
// 			UpdataConfirmLevel( connId, bFlag);
// 		}
// 
// 		if (iFlag == CONFIRM_BUFFER )//RecList //bufferid gettime //不能是同类buff，不是自己的buff或者不是队友的buff或者不是怪物伤害buff //每30s发送一次
// 		{
// 			// 			Int32 SkillCDTime;
// 			List<BufferData>BuffListData =  player->mPlayerData.mBuffListData.m_listBuffers;
// 			for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
// 			{
// 				for( List<BufferData>::Iter * it = BuffListData.begin(); it != NULL; it = BuffListData.next(it))
// 				{
// 					if (iter->mValue == (Int64)it->mValue.m_nBufferID)
// 					{
// 						RecList.erase(iter);
// 					}
// 				}		
// 
// 			}
// 			if (RecList.mIterCount > 0 )
// 			{
// 				bFlag = false;
// 			}
// 			else
// 			{
// 				bFlag = true;
// 			}
// 
// 			UpdataConfirmLevel( connId, bFlag);
// 		}
// 		return;
// 	}

	return;
}

void FireConfirmManager::onConfirmDataReq(const ConnId& connId, CGConfirmDataReq& msg)
{
	FireConfirmManager::getSingleton().ConfirmDataReq( connId,msg);
	return;	

}
//调用之前必须reset
void FireConfirmManager::SetCopyStart(Player* player,UInt32 CopyID)
{
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
	mFireConfirmData.m_CopyID = CopyID;
	mFireConfirmData.m_CopyStartTime =  TimeUtil::getTimeSec();
	player->SetFireConfirmData(mFireConfirmData);

}


UInt32 FireConfirmManager::CheckCopyFinishTime(Player* player)
{
	List<CopyFinishTemplate> FinishCopyData;
	UInt32 DiffTime;
	UInt32 MaxAttack;

	for(List<CopyFinishTemplate>::Iter *it = gCopyFinishTable->mList.begin();it != NULL;it = gCopyFinishTable->mList.next(it))
	{
		FinishCopyData.insertTail(it->mValue);
	}
	FireConfirmData = player->GetFireConfirmData();

	MaxAttack = 10000;

	DiffTime = TimeUtil::getTimeSec() - FireConfirmData.m_CopyStartTime;

	//todo
	if (gStageTable->get(FireConfirmData.m_CopyID) == NULL)
	{
		LOG_WARN("gStageTable->get(FireConfirmData.m_CopyID) not found!!");
		return LynxErrno::TimeNotRight;
	}
	if(DiffTime < gStageTable->get(FireConfirmData.m_CopyID)->limittime1)
	{
		return LynxErrno::TimeNotRight;

	}


// 	for(List<CopyFinishTemplate>::Iter *iter =FinishCopyData.begin();iter != NULL;iter = FinishCopyData.next(iter))
// 	{
// 
// 		if ((iter->mValue.CopyID) == FireConfirmData.m_CopyID)
// 		{
// 			if (iter->mValue.MaxAttack < MaxAttack)
// 			{
// 				continue;
// 			}
// 			if (iter->mValue.FinishTime * OUT_OF_RANGE > DiffTime)
// 			{
// 				UpdataConfirmLevel(player->getConnId(),false);
// 				FireConfirmData.m_CopyStartTime =-1;
// 				player->SetFireConfirmData(FireConfirmData);
// 			}
// 		}
// 	}

	return LynxErrno::None;
	
}


List<UInt32>& FireConfirmManager::getFireConfirmCondition(const ConnId& connId)
{

	
	UInt32 ConfirmMaxNum;
	UInt32 NeedConfirmIDNum;
	FireConfirmDataNotifyToJson resp;	
	List<UInt32> ConfirmIDs;
	FireConfirmData.m_ConfirmIDs.clear();

	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return FireConfirmData.m_ConfirmIDs;
	}

	FireConfirmData = player->GetFireConfirmData();

	ConfirmMaxNum =(UInt32) CONFIRM_MAX-1;
	NeedConfirmIDNum = GetConfirmTimes(FireConfirmData.m_ConfirmLevel);
	ChoiseNFromEnum(NeedConfirmIDNum,ConfirmMaxNum,ConfirmIDs);

	
	for (List<UInt32>::Iter  *iter = ConfirmIDs.begin();iter != ConfirmIDs.end(); iter = ConfirmIDs.next(iter))
	{
		FireConfirmData.m_ConfirmIDs.insertTail(iter->mValue);
	}

	//成就添加
	FireConfirmData.m_ConfirmIDs.insertTail(CONFIRM_USERHYMESKILL);
	FireConfirmData.m_ConfirmIDs.insertTail(CONFIRM_USERHYMESPEED);
	FireConfirmData.m_ConfirmIDs.insertTail(CONFIRM_CALLSERVANTCNT);
	FireConfirmData.m_ConfirmIDs.insertTail(CONFIRM_FINISHTIME);

	player->SetFireConfirmData(FireConfirmData);
	return FireConfirmData.m_ConfirmIDs;
	
}


void FireConfirmManager::SendForbiddenLogin(const ConnId& connId)
{
	ForbiddenLoginToJson resp;
	resp.ForbiddenLoginType =1;
	resp.ForbiddenLoginTime =0;

	for(List<GlobalTemplate>::Iter *iter = gGlobalVarTable->mList.begin();iter != NULL;iter = gGlobalVarTable->mList.next(iter))
	{
		if (iter->mValue.sKey == "forbidenlogintime")
		{
			const char *str = iter->mValue.sValue.c_str();
			resp.ForbiddenLoginTime =lynxAtoi<UInt32>(str) ;
		}
	}
	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,FIRE_CONFIRM_DATA_RESP,jsonStr);
}

void FireConfirmManager::updateFireConfirmData(const ConnId& connId)
{
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	PersistFireConfirmSaveReq req;
	req.playerGuid = player->getPlayerGuid();

 	req.FireConfirmData = player->GetFireConfirmData();
	PersistSystem::getSingleton().postThreadMsg(req, 0);
}


//onTwelvePalaceStartReq 拷贝了onStartCopy 代码
void FireConfirmManager::onStartCopy(const  ConnId& connId,CGChapterStart & msg)		//挑战关卡
{
	ChapterStartResp resp;
	UInt32 flag;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	msg.convertJsonToData(msg.jsonStr);
	player->ResetFireConfirmData();
	flag = StageManager::getSingleton().canChallengeStage( connId, msg.stageID);
	if (flag != LynxErrno::None)
	{
		resp.chapterID = msg.stageID;
		resp.result =flag;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_START_RESP,jsonStr);
		return;
	}

	FireConfirmManager::getSingleton().SetCopyStart(player,msg.stageID);	
// 	resp.confirmIDs =  FireConfirmManager::getSingleton().getFireConfirmCondition(connId);


	
	resp.chapterID = msg.stageID;
	resp.result =LynxErrno::None;
	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,CHAPTER_START_RESP,jsonStr);

	

	
}

void FireConfirmManager::onLeaveCopy(const  ConnId& connId,CGChapterLeave & msg)		//离开关卡
{
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	msg.convertJsonToData(msg.jsonStr);
	player->ResetFireConfirmData();
	ChapterLeaveResp resp;
	resp.chapterID = msg.chapterID;
	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,CHAPTER_LEAVE_RESP,jsonStr);
}


//CGRelive 的flag 123选哪一个buff 4是复活队友
void FireConfirmManager::onRelive(const  ConnId& connId, CGRelive & msg)		//复活
{
	UInt32 cost = 0;
	UInt32 i =0;
	std::string str;
	ReliveResp resp;
	Goods goods;
	List<Goods> itemList;
	PlayerFireConfirmData fireConfirmData;

	//LOG_INFO("onRelive  = %s ",msg.jsonStr.c_str());

	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	fireConfirmData = player->GetFireConfirmData();
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	msg.convertJsonToData(msg.jsonStr);
	resp.flag = msg.flag;


	//无限挑战去掉
	if (fireConfirmData.m_CopyID < 100)//招福集市 
	{
	}
	else
	{
		StageTemplate *stageTemplate = gStageTable->get(fireConfirmData.m_CopyID);
		if (stageTemplate == NULL)
		{
			LOG_WARN("stageTemplate not found!!");
			return;
		}
		if (stageTemplate->mType == STAGE_NORMAL || stageTemplate->mType == STAGE_ELITE ||stageTemplate->mType == STAGE_BONUS ||stageTemplate->mType == STAGE_MASTAR ||stageTemplate->mType == STAGE_TRIAL ||stageTemplate->mType == STAGE_TWELVEPALACE ||stageTemplate->mType == STAGE_COURAGE )
		{			
		}
		else
		{
			resp.result = LynxErrno::ClienServerDataNotMatch;
			std::string sendStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,CHAPTER_RELIVE_RESP,sendStr);
			return;
		}
	}
	

	if (msg.flag == 4)//复活队友
	{	
 		cost = pow((float)(globalValue.uRelivePlayer2BaseCost),(int)(fireConfirmData.m_Player2ReliveTimes+1))*globalValue.uRelivePlayer2Ratio;
		if (cost > globalValue.uRelivePlayer2MaxCost )
		{
			cost = globalValue.uRelivePlayer2MaxCost;
		}		
	}
	else
	{		
 		cost = pow((float)(globalValue.uReliveBaseCost),(int)(fireConfirmData.m_ReliveTimes+1))*globalValue.uReliveRatio;
		if (cost > globalValue.uReliveMaxCost )
		{
			cost = globalValue.uReliveMaxCost;
		}
	}
	
	if (player->getPlayerGold() < cost)
	{
		resp.result = LynxErrno::RmbNotEnough;				
	}
	
	if(resp.result == LynxErrno::None)
	{
		if (msg.flag == 4)//复活队友
		{
			fireConfirmData.m_Player2ReliveTimes ++;
		}
		else
		{
			fireConfirmData.m_ReliveTimes ++;
		}
		
		goods.resourcestype =1;
		goods.subtype = 2;
		goods.num =0 - cost;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog140);
		player->SetFireConfirmData(fireConfirmData);
	}	


	if (msg.flag == 4)//复活队友
	{
		resp.reliveTimes = fireConfirmData.m_Player2ReliveTimes;
	}
	else
	{
		resp.reliveTimes = fireConfirmData.m_ReliveTimes;
	}

	resp.coinCost = player->getPlayerCoin();	
	resp.yuanbaoCost = player->getPlayerGold();
	str = resp.convertDataToJson();
	//LOG_INFO("CHAPTER_RELIVE_RESP  = %s ",str.c_str());
	NetworkSystem::getSingleton().sender(connId,CHAPTER_RELIVE_RESP,str);

}
void FireConfirmManager::onisRecieve(const  ConnId& connId, CGisRecievedReq & msg)		//客户端接收到协议
{
	msg.convertJsonToData(msg.jsonStr);
	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	mFireConfirmData.m_LastSendItemList.clear();
	player->SetFireConfirmData(mFireConfirmData);

}
UInt32 FireConfirmManager::checkIsCheat(Guid playerID, List<FireConfirm> &fireConfirmDatas,UInt32 reqResult)
{

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	//成就统计 不管战斗成功或者失败都统计
	for(List<FireConfirm>::Iter *iter = fireConfirmDatas.begin();iter != NULL;iter = fireConfirmDatas.next(iter))
	{
		if (iter->mValue.index == CONFIRM_USERHYMESKILL)
		{
			if (iter->mValue.count > 0)
			{
				player->getAchieveManager().updateAchieveData(USERHYMESKILL,iter->mValue.count);
			}
		}
		else if (iter->mValue.index == CONFIRM_USERHYMESPEED)
		{
			if (iter->mValue.count > 0)
			{
				player->getAchieveManager().updateAchieveData(USERHYMESPEED,iter->mValue.count);
			}
		}
		else if (iter->mValue.index == CONFIRM_CALLSERVANTCNT)
		{
			if (iter->mValue.count > 0)
			{
				player->getAchieveManager().updateAchieveData(CALLSERVANTCNT,iter->mValue.count);
			}
		}
		else if (iter->mValue.index == CONFIRM_FINISHTIME)
		{
			UInt32 copyFinishTime = TimeUtil::getTimeSec() - mFireConfirmData.m_CopyStartTime;
			if (iter->mValue.keyValue.key >= (copyFinishTime + globalValue.uCopyFinishTimeDeviation))
			{
				return LynxErrno::PlayerCheating;
			}
			if (iter->mValue.keyValue.value >= (copyFinishTime + globalValue.uCopyFinishTimeDeviation))
			{
				return LynxErrno::PlayerCheating;
			}
		}
	}
	if (reqResult != LynxErrno::None)//失败不统计战斗验证
	{
		return LynxErrno::None;
	}
	//for test
	return LynxErrno::None;

	List<FireConfirm> tmpFireConfirmDatas;
	for(List<FireConfirm>::Iter *iter = fireConfirmDatas.begin();iter != NULL;iter = fireConfirmDatas.next(iter))
	{
		if (iter->mValue.index == CONFIRM_SPEED)
		{
			if (iter->mValue.count > 0)
			{
				return LynxErrno::PlayerCheating;
			}
			
		}
		else if (iter->mValue.index == CONFIRM_ATTACK_RATE)
		{
			if (iter->mValue.count > 0)
			{
				return LynxErrno::PlayerCheating;
			}

		}
		else if (iter->mValue.index == CONFIRM_POS)
		{
			if (iter->mValue.count > 0)
			{
				return LynxErrno::PlayerCheating;
			}
		}
		else if (iter->mValue.index == CONFIRM_HP ||iter->mValue.index == CONFIRM_MP ||iter->mValue.index == CONFIRM_SP)
		{
			UInt32 ret = checkBaseAttr(playerID,iter->mValue.index,iter->mValue.keyValue,tmpFireConfirmDatas);
			if (ret != LynxErrno::None)
			{
				return LynxErrno::PlayerCheating;
			}
		}
		else if (iter->mValue.index == CONFIRM_SKILL_DAMAGE)
		{
			if (iter->mValue.count > 0)
			{
				return LynxErrno::PlayerCheating;
			}
		}
		else if (iter->mValue.index == CONFIRM_CRITICAL_STRIKE_RATE)
		{
			if (iter->mValue.count > 8000)//暴击率*10000
			{
				return LynxErrno::PlayerCheating;
			}
		}
		

	

	}
	KeyValue keyValue;
	UInt32 ret = checkBaseAttr(playerID,0,keyValue,fireConfirmDatas);
	if (ret != LynxErrno::None)
	{
		return LynxErrno::PlayerCheating;
	}

	return LynxErrno::None;
}

UInt32 FireConfirmManager::checkBaseAttr(Guid playerID,UInt32 subIndex, KeyValue kevalue, List<FireConfirm> &fireConfirmDatas)
{
	String playerUidStr;
	std::string str;
	String detailStr;
	char dest[64] = {};


	sprintf(dest,"%llu",playerID);
	playerUidStr = (String)dest;
	str = LogicSystem::getSingleton().getDetailInfo(playerUidStr.c_str());
	detailStr = str.c_str();
// 	if (detailStr == NULL)
// 	{
// 		return LynxErrno::None;
// 	}	

	//for test
// 	detailStr = "[1,10100,51,\"\xe7\x94\x9f\xe7\x9a\x84\xe6\x99\x93\xe6\xa2\xa6\",10,40569,\"\",\"\",[700020,814,435,7344,11300,42275,10263,5568],[2171540.6,105,31701.6,78179.2,70338.4,41573.3,41984.3,0,0,14400,30,800,600,175,99,1378,0,0,0,0,0,0,13077.4,6727,14253,0,0,0,0,0,0,99999,0,0,0,0],[101,-104],[6010,1012,2014,3015,4017,5019],[{},{},[1003,7,0],[1004,8,1],[1005,42,3],[1006,1,0],[1007,1,0],[1008,51,2],[1009,1,0],[1010,1,0],[1011,1,0]],[10,132,[74,73,75,72,71,70,69,68,67,66,65,64]],[1000,500,600,100,100,2700,7000,8000,9000,0,0,0],[[11391],[11473],[0],[0],[0],[0]],[[[13,0,1,1],[24,0,1,1],[26,0,1,1],[0,0,0,0],[0,0,0,0]],[[39,0,1,1,1],[31,0,1,1,1],[19,0,1,1,1],[11,0,1,1,1],[6,0,1,1,1],[8,0,1,1,1],[32,0,1,1,1]],[42,41,36,56,48,49,39,29,21,1,24,22,31,19,5,20,6,50,8,27,26,13,30,11,32]],[[6010,[1015,1015,1015,1015,1015,1015]],[1012,[2015,2015,2015,2015,2015,2015]],[2014,[3015,3015,3015,3015,3015,3015]],[3015,[4015,4015,4015,4015,4015,4015]],[4017,[5015,5015,5015,5015,5015,5015]],[5019,[6015,6015,6015,6015,6015,6015]]],[[[11391,[1,91805]],[11473,[1,60989]]],[[11391,[[5,714,505],[24,106,503],[0,0,0],[0,0,0]]],[11473,[[25,194,504],[7,416,502],[0,0,0]]]],[[11391,5051],[11473,5042]]],[[111,111,112,112,122,123],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0]],[[[3,14868],[1,233969],[10,1404],[6,8300],[25,7800],[7,8309],[23,8496],[5,12893],[4,12799]],[[2,5],[3,293],[1,5264],[5,91],[4,164]],[[7,416],[1,152794],[25,194],[5,714],[24,106]],[[16,671],[3,6006],[4,7684],[10,248],[6,4276],[23,652],[7,4272],[24,1276],[25,304],[5,7685],[1,240854]],[[4,7792.2],[3,325.6],[1,20637.6],[7,266.3],[6,266.3],[23,103.4],[5,685.4]],[[7,1419],[3,10],[4,2148],[23,14],[6,1452],[5,1177],[1,270]]]]";


	Vector<String> strVector;
	Vector<String> strVector1;
	lynxStrSplit(detailStr, ",", strVector, true);
	if (strVector.size() < 1)
	{
		return LynxErrno::None;
	}
	
	lynxStrSplit(strVector[16], "[", strVector1, true);
	if (strVector1.size() < 1)
	{
		return LynxErrno::None;
	}
	
	UInt32 hp = atoi(strVector1[0].c_str());
	UInt32 mp = atoi(strVector[17].c_str());
	UInt32 sp = atoi(strVector[18].c_str());
	UInt32 ap = atoi(strVector[19].c_str());
	UInt32 magicAP = atoi(strVector[20].c_str());
	UInt32 df = atoi(strVector[21].c_str());
	UInt32 magicDF = atoi(strVector[22].c_str());
	


	if (subIndex == CONFIRM_HP)
	{
		if (kevalue.key > mp - kevalue.value)
		{
			return LynxErrno::PlayerCheating;
		}
	}
	else if (subIndex == CONFIRM_MP)
	{
		if (kevalue.key > hp - kevalue.value)
		{
			return LynxErrno::PlayerCheating;
		}
	}
	else if (subIndex == CONFIRM_SP)
	{
		if (kevalue.key > sp - kevalue.value)
		{
			return LynxErrno::PlayerCheating;
		}
	}

	for(List<FireConfirm>::Iter *iter = fireConfirmDatas.begin();iter != NULL;iter = fireConfirmDatas.next(iter))
	{
		if (iter->mValue.index == CONFIRM_MAX_AP)
		{
			if (iter->mValue.count > ap*2)
			{
				return LynxErrno::PlayerCheating;
			}

		}
		else if(iter->mValue.index == CONFIRM_MAX_DF)
		{

			if (iter->mValue.count > df*2)
			{
				return LynxErrno::PlayerCheating;
			}

		}
		else if (iter->mValue.index == CONFIRM_MAX_MAGIC_AP)
		{
			if (iter->mValue.count > magicAP*2)
			{
				return LynxErrno::PlayerCheating;
			}
		}
		else if(iter->mValue.index == CONFIRM_MAX_MAGIC_DF)
		{

			if (iter->mValue.count > magicDF*2)
			{
				return LynxErrno::PlayerCheating;
			}
		}
		else if(iter->mValue.index == CONFIRM_BUFFER)
		{
			for (List<IndexList>::Iter *it = iter->mValue.groupList.begin();it!=NULL;it = iter->mValue.groupList.next(it))
			{
				if (it->mValue.subIndex == CONFIRM_INVINCIBLE)
				{
					UInt32 count = 0;
					for (List<float>::Iter*it1= it->mValue.valueList.begin();it1!= NULL;it1= it->mValue.valueList.next(it1))
					{
						count ++;
						if (count  == 1)
						{
							if (it1->mValue > 5)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}
						if (count  == 2)
						{
							if (it1->mValue > 10)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}						
					}					
				}
				else if (it->mValue.subIndex == CONFIRM_SPBUFF)
				{
					UInt32 count = 0;
					for (List<float>::Iter*it1= it->mValue.valueList.begin();it1!= NULL;it1= it->mValue.valueList.next(it1))
					{
						count ++;
						if (count  == 1)
						{
							if (it1->mValue > 5)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}
						if (count  == 2)
						{
							if (it1->mValue > 10)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}						
					}					
				}
				else if (it->mValue.subIndex == CONFIRM_SPBUFF)
				{
					UInt32 count = 0;
					for (List<float>::Iter*it1= it->mValue.valueList.begin();it1!= NULL;it1= it->mValue.valueList.next(it1))
					{
						count ++;
						if (count  == 1)
						{
							if (it1->mValue > 5)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}
						if (count  == 2)//回复最小值
						{
							if (it1->mValue < 10)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}						
					}					
				}
				else if (it->mValue.subIndex == CONFIRM_INFINITEHIT)
				{
					UInt32 count = 0;
					for (List<float>::Iter*it1= it->mValue.valueList.begin();it1!= NULL;it1= it->mValue.valueList.next(it1))
					{
						count ++;
						if (count  == 1)
						{
							if (it1->mValue > 5)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}
						if (count  == 2)//
						{
							if (it1->mValue < 5)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}	
						if (count  == 3)
						{
							if (it1->mValue > 10)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}
						if (count  == 4)//
						{
							if (it1->mValue < 10)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}		
					}					
				}
				else if (it->mValue.subIndex == CONFIRM_VERTIGO)
				{
					UInt32 count = 0;
					for (List<float>::Iter*it1= it->mValue.valueList.begin();it1!= NULL;it1= it->mValue.valueList.next(it1))
					{
						count ++;
						if (count  == 1)
						{
							if (it1->mValue > 5)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}
						if (count  == 2)
						{
							if (it1->mValue > 10)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}						
					}					
				}
				else if (it->mValue.subIndex == CONFIRM_DECELERATION)
				{
					UInt32 count = 0;
					for (List<float>::Iter*it1= it->mValue.valueList.begin();it1!= NULL;it1= it->mValue.valueList.next(it1))
					{
						count ++;
						if (count  == 1)
						{
							if (it1->mValue > 5)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}
						if (count  == 2)
						{
							if (it1->mValue > 10)
							{
								return LynxErrno::PlayerCheating;		
							}							 
						}						
					}					
				}
			}
				
		}
	}

	return LynxErrno::None;
}
// onTwelvePalaceEndReq拷贝了onEndCopy代码
void FireConfirmManager::onEndCopy(const  ConnId& connId, CGChapterEnd & msg)		//关卡结束 
{
	UInt32 isRecord =0;//破纪录	
	UInt32 finishTimes =0;
	UInt32 recordBreaking =0;
	UInt32 star = 0;
	KeyValue keyValue;
	Goods goods;
	List<UInt32> checkList;
	List<Award> awards;
	List<Goods> award;
	List<Goods> cost;
	List<Goods> card;
	List<Goods> itemList;	
	List<KeyValue> counter;	
	ChapterEndResp resp;
	
	//LOG_INFO("onEndCopy %s",msg.jsonStr.c_str());


	BaseChangManager::getSingleton().cGChapterEndconvertJsonToData(msg);
	resp.chapterID = msg.chapterID;
	resp.result = msg.result;

	
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	StageTemplate* stageTemplate = gStageTable->get(msg.chapterID);
	if (stageTemplate == NULL)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,sendStr);
		return;
	}

	//判断是否作弊
	UInt32 checkRet =  FireConfirmManager::getSingleton().checkIsCheat(player->getPlayerGuid(), msg.fireConfirmData,msg.result);	
	if(checkRet != LynxErrno::None)
	{
		FireConfirmManager::getSingleton().UpdataConfirmLevel(connId,true);
		resp.result = checkRet;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
		return;
	}
	
	if (stageTemplate->mType == STAGE_TRIAL)//多人副本
	{
		PVPSystem::getSingleton().removeRoom(player->getRoomID());
		player->setRoomID(0);		
	}


	UInt32 ret = FireConfirmManager::getSingleton().CheckCopyFinishTime(player);
	if ( ret != LynxErrno::None && msg.result == LynxErrno::None)
	{
		resp.result = msg.result;
	}

	if (msg.result != LynxErrno::None)
	{
		player->ResetFireConfirmData();
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
		return;
	}	
		
	resp.star = msg.star;

	StageCalcManager::getSingleton().getAwards( connId ,1, msg.awardMonsterList,msg.chapterID,1);

	FireConfirmManager::getSingleton().updateFireConfirmData(connId);

	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	mFireConfirmData.m_Star = msg.star;
	finishTimes = TimeUtil::getTimeSec() - mFireConfirmData.m_CopyStartTime;

	if (mFireConfirmData.m_CopyStartTime == -1 || mFireConfirmData.m_CopyID != msg.chapterID)
	{		
		resp.result =LynxErrno::AttackPowerNotRight;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
		return;
	}

	UInt32 flag =0;
	if (flag != LynxErrno::None)
	{
		resp.result =flag;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
		return;
	}


	UInt32 goldcost = 0;
	UInt32  gold = player->getPlayerGold();
 	if (stageTemplate->mType == STAGE_TRIAL)//多人副本 
	{
		resp.star = finishTimes;
		
		GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
		UInt32 ii = 0;
		//正常是10000不扣钱，2倍是20000 扣钱
	
		PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();
		
		
	
		if (resp.result != LynxErrno::None)//失败
		{
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
			return;
		}	
		double multiply = 0;
		multiply += (double)mFireConfirmData.m_AwardTimes/10000;//10000是0倍
		//LOG_INFO("multiply1  = %f ",multiply);
		Record record;
		record.name = player->getPlayerName();
		record.playerID = player->getPlayerGuid();		
		record.val = finishTimes;

		if (finishTimes <= stageTemplate->limittime1)
		{
			multiply += (double)globalValue.uTRIALFastRate/10000;
			//LOG_INFO("multiply2  = %f ",multiply);
			if (RankingManager::getSingleton().setRecord(STAGE_RECORD_TYPE,mFireConfirmData.m_CopyID,record,true) <4)//todo破纪录 4以下是成功
			{
				CGRecord recordMsg;
				recordMsg.typeID = STAGE_RECORD_TYPE;
				RankingManager::getSingleton().onRecordReq( connId,recordMsg);
				recordBreaking =1;
				//todo 加入到成就 
				
			}
		}
		
		if (player->getFriendBlackManager().judgeFriend(mFireConfirmData.m_OtherPlayerID) == true)//是否是好友
		{
			multiply += (double)globalValue.uTRIALFriendRate/10000;
		}
		//LOG_INFO("multiply3  = %f ",multiply);


		for( List<Goods>::Iter * iter = mFireConfirmData.m_FixedList.begin();iter != NULL;iter = mFireConfirmData.m_FixedList.next(iter))
		{
			//LOG_INFO("iter->mValue.num1  = %d ",iter->mValue.num);
			iter->mValue.num = iter->mValue.num * (multiply); 
			//LOG_INFO("iter->mValue.num2  = %d ",iter->mValue.num);
		}

		for(List<Award>::Iter * item = mFireConfirmData.m_AwardsList.begin();item != NULL; item = mFireConfirmData.m_AwardsList.next(item))
		{			
			for( List<Goods>::Iter * iter = item->mValue.award.begin();iter != NULL;iter = item->mValue.award.next(iter))
			{
				iter->mValue.num = iter->mValue.num * (multiply); 
			}
			break;
		}
		for(List<Card>::Iter * item1 = mFireConfirmData.m_CardsList.begin();item1 != NULL; item1 = mFireConfirmData.m_CardsList.next(item1))
		{			
			for( List<Goods>::Iter * iter = item1->mValue.card.begin();iter != NULL;iter = item1->mValue.card.next(iter))
			{
				iter->mValue.num = iter->mValue.num * (multiply); 
			}
			break;
		}

				
	}

	//普通关卡 一般消耗，多人副本为零
	stageTemplate = gStageTable->get(mFireConfirmData.m_CopyID);
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,sendStr);
		return;
	}
	if (stageTemplate->mStrengthCost >0)
	{
		goods.resourcestype =1;
		goods.subtype =3;
		goods.num = 0 - stageTemplate->mStrengthCost;
		mFireConfirmData.m_CostList.insertTail(goods);
	}
	
	player->SetFireConfirmData(mFireConfirmData);
	

	if (stageTemplate->mType == STAGE_TRIAL)//多人副本
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,resp,1,MiniLog9);	
	}
	else if (stageTemplate->mType == STAGE_ELITE)
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,resp,1,MiniLog53);	
	}
	else if (stageTemplate->mType == STAGE_BONUS)
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,resp,1,MiniLog52);	
	}
	else if (stageTemplate->mType == STAGE_MASTAR)
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,resp,1,MiniLog54);	
	}
	else
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,resp,1,MiniLog56);	
	}

	mFireConfirmData =  player->GetFireConfirmData();

	//LOG_INFO("finishTimes  = %d ",finishTimes);
	//LOG_INFO("resp.star  = %d ",resp.star);

	resp.result =LynxErrno::None;	
	resp.recordBreaking =recordBreaking;
// 	resp.allAttr = mFireConfirmData.m_AddSendjs;
// 	resp.stages = 

	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
	//LOG_INFO("CHAPTER_END_RESP = %s",jsonStr.c_str());
	//更新成就系统 wwc
	player->getAchieveManager().updateAchieveData(BARRIER,msg.chapterID);
	player->getAchieveManager().updateAchieveData(ELITEBARRIER,msg.chapterID);
	//更新每日系统 wwc
	if (gStageTable->get(msg.chapterID) == NULL)
	{
		LOG_WARN("gStageTable->get(msg.chapterID) not found!!");
		return;
	}
	UInt32 stateType = gStageTable->get(msg.chapterID)->mType;
	if(stateType == STAGE_NORMAL || stateType==STAGE_ELITE )
	{
		player->getAchieveManager().updateDailyTaskData(DLYBARRIERCNT, 1);
	}
	
	if(stateType == STAGE_ELITE)
	{		
		player->getAchieveManager().updateDailyTaskData(DLYELITEBARRIRECNT, 1);
	}

	

	if (stageTemplate->mType == STAGE_MASTAR)
	{
		//更新七日训
		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT15,1);
	}
	if (stageTemplate->mType == STAGE_TRIAL)
	{
		if (resp.result == LynxErrno::None)
		{
			LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT11,1);
		}
	}
	LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT01,1);
}
	 
// 
// 
// UInt32 FireConfirmManager::checkStar(CGChapterEnd & msg)
// {
// 
// 	return 1;
// // 	msg.convertJsonToData(msg);
// 	UInt32 i =0;
// 	UInt32 star =0;
// 	UInt32 recvStar = 0;
// 	for(Map<UInt64, StageTemplate>::Iter *item = gStageTable->mMap.begin();item != NULL ; item = gStageTable->mMap.next(item))//地图掉落
// 	{
// 		if (msg.chapterID == item->mKey)
// 		{
// 			Vector<String> strVector1;
// 			lynxStrSplit(item->mValue.mStarRequire1, ";", strVector1, true);
// // 			Vector<String> strVector2;
// 			lynxStrSplit(item->mValue.mStarRequire2, ";", strVector2, true);
// // // 			Vector<String> strVector3;
// 			lynxStrSplit(item->mValue.mStarRequire3, ";", strVector3, true);
// 			
// 			for(List<UInt32>::Iter * iter = msg.checkList.begin();iter != NULL ;iter = msg.checkList.next(iter))
// 			{
// 				i++;
// 				if (i == 1)
// 				{
// 					recvStar = iter->mValue;
// 				}
// 				if (i%2 == 0)
// 				{
// 					if(atoi(strVector1[0].c_str()) == iter->mValue)
// 					{
// 						if(msg.checkList.next(iter)->mValue <= atoi(strVector1[1].c_str()) )
// 						{
// 							star ++;
// 						}
// 
// 					}
// 					if(atoi(strVector2[0].c_str()) == iter->mValue)
// 					{
// 						if(msg.checkList.next(iter)->mValue <= atoi(strVector2[1].c_str()) )
// 						{
// 							star ++;
// 						}
// 
// 					}
// 					if(atoi(strVector3[0].c_str()) == iter->mValue)
// 					{
// 						if(msg.checkList.next(iter)->mValue <= atoi(strVector3[1].c_str()) )
// 						{
// 							star ++;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	if (star != recvStar)
// 	{
// 		return 1;	
// 	}
// 	return 2;
// 
// }

 void FireConfirmManager::onChapterAwardCard(const  ConnId& connId ,CGChapterAwardCard &msg )	//翻牌
{
	ChapterAwardCardResp resp;
	UInt32 needGoldNum =0;
	List<Goods> itemList;
	List<Goods> costList;
	List<Goods> awardList;
	List<Goods> cardList;
	UInt32 i = 0;
	Goods goods;
	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	msg.convertJsonToData(msg.jsonStr);	
	resp.flag = msg.flag;
	resp.chapterID = msg.chapterID;
	for(List<Card>::Iter *iter = mFireConfirmData.m_CardsList.begin();iter != NULL ;iter = mFireConfirmData.m_CardsList.next(iter))
	{
		cardList = iter->mValue.card;
		break;
	}
	if ((4 - msg.flag + 1) != cardList.size() )
	{
		//todo
		if ((4 - msg.flag ) == cardList.size())//todo重复发上一次
		{
// 			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD_CARD,itemList);
		}
		return;
	}
	
	for(List<VipTemplate>::Iter * iter1 = gVipTable->mVip.begin();iter1!=NULL;iter1 = gVipTable->mVip.next(iter1))
	{
		if (iter1->mValue.id == player->getVipLevel())
		{
			if (msg.flag == 2)
			{
				needGoldNum = iter1->mValue.stageCard2;
			}
			else if (msg.flag == 3)
			{
				needGoldNum = iter1->mValue.stageCard3;
			}
			else if (msg.flag == 4)
			{
				needGoldNum = iter1->mValue.stageCard4;
			}
			break;
		}
	}
	if(player->getPlayerGold() <needGoldNum)
	{
		resp.result = LynxErrno::RmbNotEnough;
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,CHAPTER_AWARD_CARD_RESP,jsonStr);
		return;
	}

	for(List<Card>::Iter *iter = mFireConfirmData.m_CardsList.begin();iter != NULL ;iter = mFireConfirmData.m_CardsList.next(iter))
	{
		for(List<Goods>::Iter *it = iter->mValue.card.begin();it!=NULL;it = iter->mValue.card.next(it))
		{
			itemList.insertTail(it->mValue);
			iter->mValue.card.erase(it);
			break;
		}
		break;
	}
	player->SetFireConfirmData(mFireConfirmData);
	goods.resourcestype =1;
	goods.subtype =2;
	goods.num -= needGoldNum ;	
	costList.insertTail(goods);

	resp.award = itemList;

	resp.cost = costList;
	itemList += costList;

	resp.ends = itemList;
		
	GiftManager::getSingleton().saveEndsGetAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,MiniLog59);

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,CHAPTER_AWARD_CARD_RESP,jsonStr);
 }

 void FireConfirmManager::getAwardOnce(const  ConnId& connId,UInt32 copyID)
 {
	 List<AwardMonsterDamage> awardMonsterList;
	 Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	 StageCalcManager::getSingleton().getAwards( connId ,1, awardMonsterList,copyID,1);

	
 }
 void FireConfirmManager::foodGain(const  ConnId& connId, UInt32 foodType)
 {
	 Goods goods;
	 List<Goods> award;
	 List<Goods> cost;
	 List<Goods> card;
	 Award tmpAward;
	 List<Award> awards;
	 UInt32 getIt = 0;
	 UInt32 i = 0;
	 UInt32 ii = 0;

	 Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	 PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
	 GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();


	 if (foodType == 1)
	 {
		 mFireConfirmData.m_Gain = globalValue.uMSWrate1;
	 }
	 else if (foodType == 2)
	 {
		 mFireConfirmData.m_Gain = globalValue.uMSWrate2;
	 }
	 else if (foodType == 3)
	 {
		 mFireConfirmData.m_Gain = globalValue.uMSWrate3;
	 }
	 else if (foodType == 4)//元宝扫荡
	 {
		 mFireConfirmData.m_Gain = 10000;
	 }
	 else if (foodType == 5)//狭路相逢，入侵者
	 {
		 mFireConfirmData.m_Gain = 20000;
	 }
	 else
	 {
		 mFireConfirmData.m_Gain = 10000;//初始化
	 }

	
	 player->SetFireConfirmData(mFireConfirmData);
	
	 return;
 }

//扫荡总次数，扫荡免费次数，扫荡需要体力，扫荡开启条件星数，扫荡剩余的食物个数，需要的元宝，
void FireConfirmManager::onStageMopUp(const  ConnId& connId,CGStageMopUp &msg )		//关卡扫荡
{


	UInt32 flag =0 ;
	UInt32 goldCost = 0;
	UInt32 food1 = 0;
	UInt32 food2 = 0;
	UInt32 food3 = 0;
	UInt32 food4 = 0;
	UInt32 mopupTimes = 0;
	Goods goods;
	List<Goods> cost;	
	List<Goods> itemList;
	List<Goods> award;
	List<Goods> tmpItemList;
	List<AwardMonsterDamage> awardMonsterList;
	StageMopUpResp resp;

	
	msg.convertJsonToData(msg.jsonStr);
	if( msg.id == 0)
	{
		return;
	}
	resp.id = msg.id;
	resp.mopUpType = msg.mopUpType;

	if (msg.useList.size() ==4)
	{
		UInt32 i=0;
		for (List<UInt32>::Iter*iter = msg.useList.begin();iter!=NULL;iter = msg.useList.next(iter))
		{
			i++;
			if (i == 1)
			{
				food1 = iter->mValue;
			}
			if (i == 2)
			{
				food2 = iter->mValue;
			}
			if (i == 3)
			{
				food3 = iter->mValue;
			}
			if (i == 4)
			{
				food4 = iter->mValue;
			}
		}		
	}
	else
	{
		resp.result =LynxErrno::NetWorkError;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
		return;
	}	
	if (StageManager::getSingleton().getStageStar(connId,msg.id) < 3)
	{
		resp.result =LynxErrno::StarNotEnouth;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
		return;
	}

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	UInt32 leftTimes = StageManager::getSingleton().getNormalStageLeftTimes(connId,msg.id);
	player->ResetFireConfirmData();
	FireConfirmManager::getSingleton().SetCopyStart(player,msg.id);	
	StageTemplate *stageTemplate =gStageTable->get(msg.id);
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,sendStr);
		return;
	}

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	PlayerFireConfirmData mFireConfirmData = player->GetFireConfirmData();
	mFireConfirmData.m_IsMopUp = 1;
	player->SetFireConfirmData(mFireConfirmData);
	

	PlayerDailyResetData playerDailyResetData = player->getPlayerDailyResetData();
	CGFoods foodsmsg;
	foodsmsg.reqType =0;
	FireConfirmManager::onFoodsReq(connId ,foodsmsg);	
	PlayerFoodsData foodsData = player->getFoodsData();//位置不能错


	if (msg.mopUpType >3)
	{
		mopupTimes = food4;
	}
	else
	{
		mopupTimes = food1 + food2 + food3;
	}

	//有食物就能扫荡，有体力就能扫荡
// 	if (leftTimes < mopupTimes)
// 	{		
// 		resp.result = LynxErrno::TimesNotEnough;
// 		std::string jsonStr = resp.convertDataToJson();	
// 		NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
// 		return;
// 	}

	if (msg.mopUpType > 3)//精英关卡扫荡消耗体力
	{
		int needSubTimes = food4;
		if (globalValue.uSDsweeptimes > playerDailyResetData.m_EliteMopUpTimes)
		{
			if (needSubTimes > (globalValue.uSDsweeptimes - playerDailyResetData.m_EliteMopUpTimes) )
			{
				needSubTimes = globalValue.uSDsweeptimes - playerDailyResetData.m_EliteMopUpTimes;
			}
			else
			{
				needSubTimes = needSubTimes;
			}
		}
		else
		{
			needSubTimes = 0;			
		}

		if (needSubTimes > 0)
		{
			if (needSubTimes >leftTimes)
			{
				needSubTimes = leftTimes;
			}
			else
			{
				needSubTimes = needSubTimes;
			}			
		}
	
		if (needSubTimes <= 0)
		{
			resp.result =LynxErrno::NumberNotRight;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}
	

		UInt32 strengthCount = FireConfirmManager::getSingleton().refreshgetStrength(connId);
		if (strengthCount < (stageTemplate->mStrengthCost* needSubTimes) )
		{
			needSubTimes = strengthCount/stageTemplate->mStrengthCost;			
		}
		if (needSubTimes == 0)
		{
			resp.result =LynxErrno::StrengthNotEnough;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}
		
		playerDailyResetData.m_EliteMopUpTimes += needSubTimes;
		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_STRENGTH;
		goods.num -= stageTemplate->mStrengthCost*needSubTimes;
		mFireConfirmData.m_CostList.insertTail(goods);
		player->SetFireConfirmData(mFireConfirmData);
		player->setPlayerDailyResetData(playerDailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);

		for (UInt32 j=0;j<needSubTimes;j++)
		{
			FireConfirmManager::getSingleton().getAwardOnce(connId,msg.id);
		}

	}

	if (msg.mopUpType <=3)//普通关卡扫荡消耗食物
	{

		int needSubTimes = food1 +food2 + food3;
		if (globalValue.uSDsweeptimes > playerDailyResetData.m_NormalMopUpTimes)
		{
			if (needSubTimes > (globalValue.uSDsweeptimes - playerDailyResetData.m_NormalMopUpTimes) )
			{
				needSubTimes = globalValue.uSDsweeptimes - playerDailyResetData.m_NormalMopUpTimes;
			}
			else
			{
				needSubTimes = needSubTimes;
			}
		}
		else
		{
			needSubTimes = 0;			
		}

		if (needSubTimes > 0)
		{
			if (needSubTimes >leftTimes)
			{
				needSubTimes = leftTimes;
			}
			else
			{
				needSubTimes = needSubTimes;
			}			
		}

		if (needSubTimes <= 0)
		{
			resp.result =LynxErrno::NumberNotRight;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}
		
		if ((foodsData.food1+foodsData.food11) <food1 || (foodsData.food2+foodsData.food12)  <food2 ||(foodsData.food3+foodsData.food13)  <food3 )
		{
			resp.result =LynxErrno::FoodNotEnough;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}

		if (food1 > 0)
		{
			FireConfirmManager::getSingleton().typeFoodSubNum(player->getPlayerGuid(),foodsData,food1,1 );
		}
		if (food2 > 0)
		{
			FireConfirmManager::getSingleton().typeFoodSubNum(player->getPlayerGuid(),foodsData,food2,2 );
		}
		if (food3 > 0)
		{
			FireConfirmManager::getSingleton().typeFoodSubNum(player->getPlayerGuid(),foodsData,food3,3 );
		}
		
		if(foodsData.beginTime ==0)
		{
			foodsData.beginTime = TimeUtil::getTimeSec();
		}

		playerDailyResetData.m_NormalMopUpTimes += (food1 + food2 + food3);
		player->setPlayerDailyResetData(playerDailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);

		if( (food1+food2+food3) == 0)
		{
			resp.result = LynxErrno::NumberNotRight;
		}
		player->setFoodsData(foodsData);		
		player->getPersistManager().setDirtyBit(FOODDATABIT );

		player->SetFireConfirmData(mFireConfirmData);

		FireConfirmManager::getSingleton().foodGain(connId, 1);
		for (UInt32 j=0;j<food1;j++)
		{
			FireConfirmManager::getSingleton().getAwardOnce(connId,msg.id);
		}
		

		FireConfirmManager::getSingleton().foodGain(connId, 2);
		for (UInt32 j=0;j<food2;j++)
		{
			FireConfirmManager::getSingleton().getAwardOnce(connId,msg.id);		
		}
		

		FireConfirmManager::getSingleton().foodGain(connId, 3);
		for (UInt32 j=0;j<food3;j++)
		{
			FireConfirmManager::getSingleton().getAwardOnce(connId,msg.id);			
		}
		FireConfirmManager::getSingleton().foodGain(connId, 0);//用完了重置
		
	}

	FireConfirmManager::getSingleton().updateFireConfirmData(connId);
	ChapterEndResp awardResult;


	if (stageTemplate->mType == STAGE_ELITE)
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,awardResult,0,MiniLog63);	
	}
	else if (stageTemplate->mType == STAGE_BONUS)
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,awardResult,0,MiniLog62);	
	}
	else if (stageTemplate->mType == STAGE_MASTAR)
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,awardResult,0,MiniLog64);	
	}
	else
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,awardResult,0,MiniLog57);	
	}

	
	resp.foodRecoverList.insertTail(foodsData.food1);
	resp.foodRecoverList.insertTail(foodsData.food2);
	resp.foodRecoverList.insertTail(foodsData.food3);
	resp.foodbuyList.insertTail(foodsData.food11);
	resp.foodbuyList.insertTail(foodsData.food12);
	resp.foodbuyList.insertTail(foodsData.food13);

	resp.firstAwards = awardResult.firstAwards;	
	resp.awards = awardResult.awards;
	resp.cost = awardResult.cost;
	resp.cards = awardResult.cards;
	resp.monsterDropList = awardResult.monsterDropList;
	resp.fixedList = awardResult.fixedList;
	resp.ends = awardResult.ends;
	resp.result =LynxErrno::None;
	resp.leftTimes =  leftTimes - (food1 + food2 + food3 + food4);//免费次数不为0时要修改

	resp.mopupTimes = mopupTimes;
	

	mFireConfirmData = player->GetFireConfirmData();
	resp.allAttr =awardResult.allAttr;
	resp.stages =awardResult.stages;

	std::string jsonStr = resp.convertDataToJson();

	LOG_WARN("jsonStr = %s",jsonStr.c_str());

	NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);

	//添加扫荡每日副本和精英副本日常

	if(stageTemplate->mType == STAGE_NORMAL ||stageTemplate->mType == STAGE_ELITE)
	{
		player->getAchieveManager().updateDailyTaskData(DLYBARRIERCNT, mopupTimes );
	}

	if(stageTemplate->mType == STAGE_ELITE)
	{
		player->getAchieveManager().updateDailyTaskData(DLYELITEBARRIRECNT, mopupTimes );
	}

	
	//更新七日训
	LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT06,food4);

	LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT01,mopupTimes);

	if (stageTemplate->mType == STAGE_MASTAR)
	{
		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT15,mopupTimes);
	}
	

}
//fanPaiType 0 是只发送免费的翻牌，1 是发送4张
void FireConfirmManager::saveAndGetResult(const  ConnId& connId,ChapterEndResp &resp,UInt32 fanPaiType = 0,UInt32 systemID = 0)
{


	bool servantExist = false;
	Goods goods;	
	Card tmpCard;
	List<Goods> award;
	List<Goods> cost;
	List<Goods> card;
	List<Goods> fourCards;	
	List<Award> awards;
	Award tmpAward;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	GiftManager::getSingleton().servanNeedChangeToPiece(player->getPlayerGuid(),mFireConfirmData.m_AwardsList,resp.ends);
	resp.awards = mFireConfirmData.m_AwardsList;

	GiftManager::getSingleton().servanNeedChangeToPiece(player->getPlayerGuid(),mFireConfirmData.m_FirstAwardsList,resp.ends);
	resp.firstAwards = mFireConfirmData.m_FirstAwardsList;

	GiftManager::getSingleton().servanNeedChangeToPiece(player->getPlayerGuid(),mFireConfirmData.m_MonsterAwardList,resp.ends);
	resp.monsterDropList = mFireConfirmData.m_MonsterAwardList;
	
	VipTemplate vipTemplate;//todo mei yanzheng
	for(List<VipTemplate>::Iter *iter1 = gVipTable->mVip.begin();iter1 != NULL;iter1 = gVipTable->mVip.next(iter1))
	{
		if (iter1->mValue.id == player->getVipLevel())
		{
			vipTemplate = iter1->mValue;
			break;
		}
	}

	for(List<Card>::Iter * item = mFireConfirmData.m_CardsList.begin();item != NULL; item = mFireConfirmData.m_CardsList.next(item))
	{

		fourCards = item->mValue.card;
		UInt32 count =0;
		card.clear();
		for(List<Goods>::Iter * it = item->mValue.card.begin();it!= NULL;it = item->mValue.card.next(it))
		{			
			count ++;
			if (count ==1)//vip dengji 
			{
				if(vipTemplate.stageCard1 !=0)
				{
					break;
				}					
			}	
			if (count ==2)//vip 
			{
				if(vipTemplate.stageCard2 !=0)
				{
					break;
				}					
			}	
			if (count ==3)//vip 
			{
				if(vipTemplate.stageCard3 !=0)
				{
					break;
				}					
			}	
			if (count ==4)//vip 
			{
				if(vipTemplate.stageCard4 !=0)
				{
					break;
				}					
			}
			card.insertTail(it->mValue);
			item->mValue.card.erase(it);

		}


		player->SetFireConfirmData(mFireConfirmData);

		resp.ends += card;
		//翻牌的不能合也不能拆
		
		if(fanPaiType == 0)//只发免费的
		{
			if (card.size() != 0)
			{
				tmpCard.card = card;
				resp.cards.insertTail(tmpCard);
			}
		}
		else
		{
			if (fourCards.size() != 0)
			{
				tmpCard.card = fourCards;
				resp.cards.insertTail(tmpCard);
			}	
		}		
	}

	resp.cost = mFireConfirmData.m_CostList;	
	GiftManager::getSingleton().combineSame(resp.cost);
	resp.ends += resp.cost;	

	resp.intruderAwardList = mFireConfirmData.m_IntruderAwardList;
	GiftManager::getSingleton().combineSame(resp.intruderAwardList);
	resp.ends += resp.intruderAwardList;

	resp.fixedList = mFireConfirmData.m_FixedList;
	GiftManager::getSingleton().combineSame(resp.fixedList);
	resp.ends += resp.fixedList;

	GiftManager::getSingleton().combineSame(resp.ends);

// 	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,resp.ends);

	List<Goods> stagesEnds;
	List<Goods> stagesNullEnds;
	for(List<Goods>::Iter *endsIter = resp.ends.begin();endsIter!=NULL;endsIter = resp.ends.next(endsIter))
	{
		if (endsIter->mValue.resourcestype == AWARD_STAGEDATA||endsIter->mValue.resourcestype == AWARD_TWELVEPALACE_STAGEDATA)
		{
			stagesEnds.insertTail(endsIter->mValue);
		}
		else
		{

			stagesNullEnds.insertTail(endsIter->mValue);
		}

	}
	
	List<ReturnItemEle> rtItemList;
	GiftManager::getSingleton().addToPlayerAttr(player->getPlayerGuid(),rtItemList,resp.allAttr, stagesNullEnds,systemID);

	List<ReturnItemEle> stageRtItemList;
	GiftManager::getSingleton().addToPlayerAttr(player->getPlayerGuid(),stageRtItemList,resp.stages, stagesEnds,systemID);


	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);



// 	mFireConfirmData =  player->GetFireConfirmData();
// 	resp.allAttr = mFireConfirmData.m_AddSendjs;	
	return;
}

void FireConfirmManager::getChapterCounter(const  ConnId& connId)
{

	List<KeyValue> copyCounter;
	Player * player  = LogicSystem::getSingleton().getPlayerByConnId(connId);
	copyCounter = player->GetCopyCounter();
	ChapterCounterResp resp;
	resp.keyValue = copyCounter;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,CHAPTER_COUNTER_RESP,jsonStr);
}

UInt32 FireConfirmManager::typeFoodLeftNum(PlayerFoodsData &foodsData,UInt32 num,UInt32 type )	
{
	UInt32 canSubNum = 0;
	if (type == 1)
	{
		if (foodsData.food11 >= num)
		{
			canSubNum = num;
		}
		else
		{
			if (foodsData.food1 >= (num - foodsData.food11))
			{
				canSubNum = num;
			}
			else
			{
				canSubNum = num - foodsData.food1 - foodsData.food11;
			}
		}
	}
	else if (type == 2)
	{
		if (foodsData.food12 >= num)
		{
			canSubNum = num;
		}
		else
		{
			if (foodsData.food2 >= (num - foodsData.food12))
			{
				canSubNum = num;
			}
			else
			{
				canSubNum = num - foodsData.food2 - foodsData.food12;
			}
		}
	}
	else if (type == 3)
	{
		if (foodsData.food13 >= num)
		{
			canSubNum = num;
		}
		else
		{
			if (foodsData.food3 >= (num - foodsData.food13))
			{
				canSubNum = num;
			}
			else
			{
				canSubNum = num - foodsData.food3 - foodsData.food13;
			}
		}
	}


	return canSubNum;
	
}
void FireConfirmManager::typeFoodSubNum(UInt64 playerID, PlayerFoodsData &foodsData,UInt32 num,UInt32 type)	
{
	char dest[1024]={0};

	UInt32 lastFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;
	UInt32 canSubNum = 0;
	if (type == 1)
	{
		if (foodsData.food11 >= num)
		{
			foodsData.food11 -= num;
		}
		else
		{
			if (foodsData.food1 >= (num - foodsData.food11))
			{				
				foodsData.food1 -=  (num - foodsData.food11);
				foodsData.food11 = 0;//顺序不能错
			}
			else
			{
				foodsData.food11 = 0;
				foodsData.food1 = 0;
			}
		}
	}
	else if (type == 2)
	{
		if (foodsData.food12 >= num)
		{
			foodsData.food12 -= num;
		}
		else
		{
			if (foodsData.food2 >= (num - foodsData.food12))
			{
				
				foodsData.food2 -=  (num - foodsData.food12);
				foodsData.food12 = 0;
			}
			else
			{
				foodsData.food12 = 0;
				foodsData.food2 = 0;
			}
		}
	}
	else if (type == 3)
	{
		if (foodsData.food13 >= num)
		{
			foodsData.food13-= num;
		}
		else
		{
			if (foodsData.food3 >= (num - foodsData.food13))
			{
				
				foodsData.food3 -=  (num - foodsData.food13);
				foodsData.food13 = 0;
			}
			else
			{
				foodsData.food13 = 0;
				foodsData.food3 = 0;
			}
		}
	}
	UInt32 nowFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;

	snprintf(dest,sizeof(dest),"%d,%d",lastFoodNum,nowFoodNum );
	LogicSystem::getSingleton().write_log( LogType66,playerID, dest,LogInfo);
}

static UInt32 kickCount =0;

void FireConfirmManager::onFoodsReq(const  ConnId& connId ,CGFoods &msg )	//美食屋
{

	//for test
// 	RankGameManager::getSingleton().getNullPlayerID();

// 	CodeReq req;
// 	req.reqType = 1;
// 	req.code = "EPLMN4FQ";
// 	CodeManager::getSingleton().onCodeReq( connId,req);

	UInt32 maxBuyTimes =0;
	double maxCanAddCount = 0;
	double maxCanRecoverTimes = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	int finishCount = 0;
	UInt32 flag = 1;
	int lastFoodNum = 0;
	UInt32 nowFoodNum = 0;
	char dest[1024]={0};

	double lastFinishCount =0;
	List<Goods> itemList;
	Goods goods;
	List<UInt32> foods;
	PlayerFoodsData foodsData;
	PlayerBaseData baseData;
	FoodsResp resp;
	
	msg.convertJsonToData(msg.jsonStr);
	
	Player * player  = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if(!player)
	{
		return;
	}
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	foodsData = player->getFoodsData();
	baseData = player->getPlayerBaseData();
	SystemInfoTemplate *systemInfoTemplate  = SYSTEMINFO_TABLE().get(SYSTEM_FOOD);
	if (systemInfoTemplate == NULL)
	{
		return;
	}



	if (foodsData.resetState == 0 &&player->getPlayerLeval() < systemInfoTemplate->openConditionValue)
	{
		foodsData.leftTimes = 3;
		player->setFoodsData(foodsData);
		player->getPersistManager().setDirtyBit(FOODDATABIT );

		foodsData.beginTime = nowTime;
	}
	else
	{
		if (foodsData.resetState == 0)
		{
			foodsData.resetState = 1;
			player->setFoodsData(foodsData);
			player->getPersistManager().setDirtyBit(FOODDATABIT );
		}		
	}
	
	
	for (List<VipTemplate>::Iter * iter = gVipTable->mVip.begin();iter!= NULL;iter = gVipTable->mVip.next(iter))
	{
		if (iter->mValue.id == baseData.m_nVipLevel)
		{
			maxBuyTimes =  iter->mValue.foodBuy; 
			if(TimeManager::timeIsToday(foodsData.buyTime) ==false)
			{
				foodsData.buyTime = nowTime;
				foodsData.vipLeftTimes = 0;
			}
			break;
		}
	}
		
	lastFinishCount = (double)nowTime - foodsData.beginTime;
	if (lastFinishCount < 0)
	{
		lastFinishCount = 0;
		foodsData.beginTime = nowTime;
		player->setFoodsData(foodsData);
		player->getPersistManager().setDirtyBit(FOODDATABIT );
	}
	lastFinishCount = lastFinishCount / globalValue.uMSWtime;
	if(lastFinishCount < 0)//有时时间异常
	{
		finishCount = 0;
	}
	else
	{
		finishCount = lastFinishCount;
	}

// 	int canRecoverCount = ( globalValue.uMSWrecover - (foodsData.leftTimes + foodsData.food1 + foodsData.food2 + foodsData.food3) );
// 	if (canRecoverCount < 0)
// 	{
// 		canRecoverCount = 0;
// 	}
// 	if (finishCount >canRecoverCount)
// 	{
// 		finishCount = canRecoverCount;
// 	}
// 	else
// 	{
// 		finishCount = finishCount;
// 	}

	maxCanAddCount =(double)(globalValue.uMSWmax ) - (foodsData.leftTimes + foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13);

	if (maxCanAddCount > 0 )
	{
		if (maxCanAddCount <= finishCount )//次数满了
		{
			lastFoodNum = foodsData.leftTimes;
			foodsData.leftTimes += maxCanAddCount;
			foodsData.beginTime = nowTime;

			snprintf(dest,sizeof(dest),"%d,%d,%d",lastFoodNum,maxCanAddCount,foodsData.leftTimes );
			LogicSystem::getSingleton().write_log( LogType65,baseData.m_nPlayerID, dest,LogInfo);
		}
		else //次数没满还要倒计时
		{
			lastFoodNum = foodsData.leftTimes;
			foodsData.leftTimes += finishCount;

			if (finishCount != 0)
			{
				snprintf(dest,sizeof(dest),"%d,%d,%d",lastFoodNum,finishCount,foodsData.leftTimes );
				LogicSystem::getSingleton().write_log( LogType65,baseData.m_nPlayerID, dest,LogInfo);
			}
		

			if ((foodsData.leftTimes + foodsData.food1 + foodsData.food2 + foodsData.food3) == globalValue.uMSWmax)
			{
				foodsData.beginTime = nowTime;
			}			
			else
			{
				if ((foodsData.leftTimes + foodsData.food1 + foodsData.food2 + foodsData.food3) > globalValue.uMSWmax)
				{
					ASSERT(false);				
				}
				else
				{
					foodsData.beginTime = foodsData.beginTime + finishCount * (globalValue.uMSWtime);
				}				
			}
			
		}			
	}
	else
	{
		foodsData.beginTime = nowTime;
	}

	maxCanAddCount =(double)(globalValue.uMSWmax ) - (foodsData.leftTimes + foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13);
	if (maxCanAddCount < 0)
	{
		maxCanAddCount = 0;
	}
	resp.flag = flag;
	
	
	if (msg.reqType == 0)//处理一次 不发送数据
	{	
		player->setFoodsData(foodsData);
		return;
	}
	else if (msg.reqType == 1)//获取信息
	{		
		resp.viableNum  = foodsData.leftTimes;
		if (foodsData.beginTime ==0)
		{
			resp.needTime =  globalValue.uMSWtime;
		}
		else
		{
			resp.needTime =  globalValue.uMSWtime -  (nowTime - foodsData.beginTime)%(globalValue.uMSWtime);	
		}	
	}
	else if (msg.reqType == 2)//烹饪
	{
		if (foodsData.leftTimes > 0)
		{
			lastFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;

			UInt32 tmp =0;
			UInt32 i =0;
			foods.insertTail(tmp);
			foods.insertTail(tmp);
			foods.insertTail(tmp);
			FireConfirmManager::getSingleton().getfoods( foodsData.leftTimes,foods);
			for (List<UInt32>::Iter * it = foods.begin();it != NULL;it = foods.next(it))
			{
				if (i == 0)
				{
					foodsData.food1 += it->mValue;
				}
				else if (i == 1)
				{
					foodsData.food2 += it->mValue;
				}
				else if (i == 2)
				{
					foodsData.food3 += it->mValue;
				}
				i++;
			}
			foodsData.leftTimes = 0;

			//美食屋烹饪打点wwc
			player->getAchieveManager().updateDailyTaskData(DLYFOODCOOK, 1 );
			resp.reqType = msg.reqType;//成功才发

			nowFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;
			snprintf(dest,sizeof(dest),"%d,%d",lastFoodNum,nowFoodNum);
			LogicSystem::getSingleton().write_log( LogType66,baseData.m_nPlayerID, dest,LogInfo);

		}
		else
		{

			resp.viableNum  = foodsData.leftTimes;
			if (foodsData.beginTime ==0)
			{
				resp.needTime =  globalValue.uMSWtime;
			}
			else
			{
				resp.needTime =  globalValue.uMSWtime -  (nowTime - foodsData.beginTime)%(globalValue.uMSWtime);	
			}
			
		}		
	}
	else if (msg.reqType == 3)//购买次数
	{		
		do 
		{
			if(baseData.m_nGold < msg.num * globalValue.uMSWprice )
			{
				flag = LynxErrno::RmbNotEnough;
				break;
			}

			lastFoodNum =  foodsData.leftTimes;


			if( (maxBuyTimes - foodsData.vipLeftTimes) < msg.num  )
			{
				flag = LynxErrno::TimesNotEnough;
				break;
			}

// 			if ((lastFoodNum + foodsData.leftTimes+ msg.num) > globalValue.uMSWrecover)
// 			{
// 				flag = LynxErrno::FoodIsFull;
// 				break;
// 			}

			if (maxCanAddCount < (msg.num * globalValue.uMSWeachBuy))
			{
				flag = LynxErrno::OverMaxTimes;
				break;
			}
			
			foodsData.vipLeftTimes += msg.num ;
			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num -= msg.num * globalValue.uMSWprice;		
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,itemList,MiniLog70);

			 foodsData.leftTimes += msg.num * globalValue.uMSWeachBuy;

			snprintf(dest,sizeof(dest),"%d,%d",lastFoodNum, foodsData.leftTimes);
			LogicSystem::getSingleton().write_log( LogType67,baseData.m_nPlayerID, dest,LogInfo);
		} while (0);

		
	}
	else if (msg.reqType == 4)//开箱子奖励可用次数
	{
		lastFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;
		if (maxCanAddCount < msg.num)
		{
			msg.num = maxCanAddCount;
		}
		else
		{
			 msg.num =  msg.num;
		}
// 		1	美食烧制次数
// 			2	全鱼火锅
// 			3	糖稀鱼丸
// 			4	鱼尾烧麦


		if (msg.subType == 1)
		{
			foodsData.leftTimes += msg.num;			
		}
		else 	if (msg.subType == 2)
		{
			foodsData.food3 += msg.num;
		}
		else 	if (msg.subType == 3)
		{			
			foodsData.food2 += msg.num;
		}
		else 	if (msg.subType == 4)
		{
			foodsData.food1 += msg.num;			
		}
		
		nowFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;
		snprintf(dest,sizeof(dest),"%d,%d",lastFoodNum,nowFoodNum);
		LogicSystem::getSingleton().write_log( LogType66,baseData.m_nPlayerID, dest,LogInfo);
		
	}
	else if (msg.reqType == 10)//清空美食屋
	{

		lastFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;

		foodsData.leftTimes = 0;
		foodsData.beginTime = nowTime;
		foodsData.food1 = 0;		
		foodsData.food2 = 0;
		foodsData.food3 = 0;
		foodsData.food11 = 0;		
		foodsData.food12 = 0;
		foodsData.food13 = 0;
		foodsData.vipLeftTimes = 0;	
		player->setFoodsData(foodsData);
		player->getPersistManager().setDirtyBit(FOODDATABIT );

		nowFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;
		snprintf(dest,sizeof(dest),"%d,%d",lastFoodNum,nowFoodNum);
		LogicSystem::getSingleton().write_log( LogType66,baseData.m_nPlayerID, dest,LogInfo);

		return;
	}
	else if (msg.reqType == 11)//设置成最大值
	{
		lastFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;

// 		int addNum = (int)globalValue.uMSWrecover - (foodsData.food1 + foodsData.food2 + foodsData.food3);
// 		if (addNum < 0)
// 		{
// 			addNum = 0;
// 		}
		foodsData.leftTimes = maxCanAddCount;
		foodsData.beginTime = nowTime;
		foodsData.vipLeftTimes = 0;	
		player->setFoodsData(foodsData);
		player->getPersistManager().setDirtyBit(FOODDATABIT );

		UInt32 nowFoodNum = foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.food11 + foodsData.food12 + foodsData.food13;
		snprintf(dest,sizeof(dest),"%d,%d",lastFoodNum,nowFoodNum);
		LogicSystem::getSingleton().write_log( LogType66,baseData.m_nPlayerID, dest,LogInfo);

		return;
	}
	player->setFoodsData(foodsData);
	if (msg.reqType != 1)
	{
		player->getPersistManager().setDirtyBit(FOODDATABIT );
	}
	
	resp.flag = flag;
	resp.viableNum  = foodsData.leftTimes;
	if (foodsData.beginTime ==0)
	{
		ASSERT(false);
	}	
		
	resp.needTime =  globalValue.uMSWtime -  (nowTime - foodsData.beginTime)%(globalValue.uMSWtime);	
// 	resp.viableAmount = globalValue.uMSWrecover;
	resp.viableNum = foodsData.food1 + foodsData.food2 + foodsData.food3 +foodsData.leftTimes;
	resp.restbuyNumber = foodsData.vipLeftTimes;

	resp.foodRecoverList.insertTail(foodsData.food1);
	resp.foodRecoverList.insertTail(foodsData.food2);
	resp.foodRecoverList.insertTail(foodsData.food3);
	resp.foodbuyList.insertTail(foodsData.food11);
	resp.foodbuyList.insertTail(foodsData.food12);
	resp.foodbuyList.insertTail(foodsData.food13);

	resp.buyCost = globalValue.uMSWprice;
	resp.onceBuyFoodNum = globalValue.uMSWeachBuy;
	resp.recoverNum = foodsData.leftTimes;
	resp.vipFoodLeftNumber = foodsData.food11+ foodsData.food12+ foodsData.food13;
	resp.gold = player->getPlayerGold();


	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,CHAPTER_FOOD_RESP,jsonStr);


}

void  FireConfirmManager::foodsInit(UInt64 playerID)
{
	PlayerFoodsData foodsData;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	SystemInfoTemplate *systemInfoTemplate  = SYSTEMINFO_TABLE().get(SYSTEM_FOOD);
	if (player->getPlayerLeval() < systemInfoTemplate->openConditionValue)
	{		
		return;
	}
	


	
	foodsData = player->getFoodsData();
	if(foodsData.resetState == 1)//已结重置
	{
		return;
	}
	foodsData.food1 = 0;
	foodsData.food2 = 0;
	foodsData.food3 = 0;	
	foodsData.beginTime =  TimeUtil::getTimeSec();
	
	foodsData.leftTimes = 3;
	foodsData.resetState = 1;



	player->setFoodsData(foodsData);
	player->getPersistManager().setDirtyBit(FOODDATABIT );
}



void FireConfirmManager::getfoods(UInt32 num,List<UInt32> &foods)
{
	List<UInt32> randomNumbers;
	UInt32 allWeight = 0;
	UInt32 randNum = 0;
	UInt32 weight = 0;
	UInt32 ii;
	UInt32 jj;

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();


	allWeight = globalValue.uMSWweight1 + globalValue.uMSWweight2 + globalValue.uMSWweight3;
	randomNumbers.insertTail(globalValue.uMSWweight1);
	randomNumbers.insertTail(globalValue.uMSWweight2);
	randomNumbers.insertTail(globalValue.uMSWweight3);
	
	if (allWeight == 0)
	{
		return;
	}
	for(int i= 0;i<num;i++)
	{
		randNum = rand()%allWeight;		
		ii =0;
		weight =0;
		for (List<UInt32>::Iter * iter = randomNumbers.begin();iter!=NULL;iter = randomNumbers.next(iter))
		{			
			weight += iter->mValue;
			if (randNum < weight)
			{
				jj =0;
				for (List<UInt32>::Iter *iter1 = foods.begin();iter1 != NULL;iter1 = foods.next(iter1))
				{
					if (ii == jj)
					{
						iter1->mValue ++;
						break;
					}					
					jj ++;
				}
				break;
			}
			ii++;
		}
	}	

}


void FireConfirmManager::onWelfaleBeginReq(const  ConnId& connId ,CGWelfareBegin &msg)
{
	UInt32 maxTimes = 0;
	UInt32 difficulty = 0;
	UInt32 finishTimes = 0;
	UInt32 vipAddTimes =0;
	UInt32 cost = 0;
	UInt32 isOpendDay = 0;
	WelfareBeginResp resp;	
	WelfareMarket welfareMarket;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	player->ResetFireConfirmData();

	UInt32 level = player->getPlayerLeval();
	msg.convertJsonToData(msg.jsonStr);

	//判断是否作弊
	UInt32 checkRet =  FireConfirmManager::getSingleton().checkIsCheat(player->getPlayerGuid(), msg.fireConfirmData,0);	
	if(checkRet != LynxErrno::None)
	{
		FireConfirmManager::getSingleton().UpdataConfirmLevel(connId,true);
		resp.result = checkRet;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,WELFALE_MARKET_BEGIN_RESP,jsonStr);
		return;
	}

	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();
	for(List<VipTemplate>::Iter * iter = gVipTable->mVip.begin();iter!=NULL;iter = gVipTable->mVip.next(iter))
	{
		if (iter->mValue.id == player->getVipLevel())
		{
			vipAddTimes = iter->mValue.welfaremarket;
			break;
		}
	}

	for(List<WelfareMarketTemplate>::Iter *it = gWelfareMarketTable->mWelfareMarket.begin();it!=NULL;it =gWelfareMarketTable->mWelfareMarket.next(it))		
	{
		if (it->mValue.id == msg.id)
		{
			String weekDay = TimeUtil::getDay(-1);
			
			if(weekDay == "Mon")
			{
				isOpendDay =it->mValue.Monday;
			}
			else if(weekDay == "Tue")
			{
				isOpendDay =it->mValue.Tuesday;
			}
			else if(weekDay == "Wed")
			{
				isOpendDay =it->mValue.Wednesday;
			}
			else if(weekDay == "Thu")
			{
				isOpendDay =it->mValue.Thursday;
			}
			else if(weekDay == "Fri")
			{
				isOpendDay =it->mValue.Friday;
			}
			else if(weekDay == "Sat")
			{
				isOpendDay =it->mValue.Saturday;
			}
			else if(weekDay == "Sun")
			{
				isOpendDay =it->mValue.Sunday;
			}
			
			welfareMarket.id =it->mValue.id;		

			if (it->mValue.id ==1)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyChaiCount;
			}
			else if (it->mValue.id ==2)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyMiCount;
			}
			else if (it->mValue.id ==3)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyYouCount;
			}
			else if (it->mValue.id ==4)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyYanCount;
			}
			do 
			{
				if (level >= it->mValue.difficulty6.levelRequireDifficulty)
				{
					difficulty =6;
					break;
				}
				if (level >= it->mValue.difficulty5.levelRequireDifficulty)
				{
					difficulty =5;
					break;
				}
				if (level >= it->mValue.difficulty4.levelRequireDifficulty)
				{
					difficulty =4;
					break;
				}
				if (level >= it->mValue.difficulty3.levelRequireDifficulty)
				{
					difficulty =3;
					break;
				}
				if (level >= it->mValue.difficulty2.levelRequireDifficulty)
				{
					difficulty =2;
					break;
				}
				if (level >= it->mValue.difficulty1.levelRequireDifficulty)
				{
					difficulty =1;
					break;
				}
			} while (0);
			maxTimes = it->mValue.dailyBaseTimes + vipAddTimes;
			welfareMarket.difficulty = difficulty;
			welfareMarket.maxTimes = maxTimes;

			break;
		}	
	}

	resp.id = msg.id;
	resp.difficulty = msg.difficulty;
	if (difficulty < msg.difficulty)
	{
		resp.difficulty = difficulty;
		resp.result =LynxErrno::NotOpen;
	}
	if (isOpendDay != 1)
	{
		resp.result =LynxErrno::TimeNotRight;
	}
	if (maxTimes <= finishTimes)
	{
		resp.result =LynxErrno::TimesNotEnough;
	}
	
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,WELFALE_MARKET_BEGIN_RESP,jsonStr);

}
void FireConfirmManager::onWelfaleEndReq(const  ConnId& connId ,CGWelfareEnd &msg)
{	
	UInt32 maxTimes = 0;
	UInt32 difficulty = 0;
	UInt32 finishTimes = 0;
	UInt32 vipAddTimes =0;
	UInt32 costNum = 0;
	UInt32 doneTimes = 0;
	UInt32 awardID =0;	
	UInt32 isOpendDay = 0;
	UInt32 level;
	float awardChange =0;
	Goods goods;
	WelfareMarket welfareMarket;
	List<Goods> tmpItemList;
	WelfareEndResp resp;
	List<Goods> award;//物品
	List<Goods> cost;//物品

	msg.convertJsonToData(msg.jsonStr);
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	
	if(msg.doType != 0 &&msg.doType !=  1 )
	{
		resp.result = msg.doType;
		msg.doType = 1;
	}
	level = player->getPlayerLeval();
	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();

	for(List<VipTemplate>::Iter * iter = gVipTable->mVip.begin();iter!=NULL;iter = gVipTable->mVip.next(iter))
	{
		if (iter->mValue.id == player->getVipLevel())
		{
			vipAddTimes = iter->mValue.welfaremarket;
			break;
		}
	}

	for(List<WelfareMarketTemplate>::Iter *it = gWelfareMarketTable->mWelfareMarket.begin();it!=NULL;it =gWelfareMarketTable->mWelfareMarket.next(it))		
	{
		if (it->mValue.id == msg.id)
		{
			String weekDay = TimeUtil::getDay(-1);

			if(weekDay == "Mon")
			{
				isOpendDay =it->mValue.Monday;
			}
			else if(weekDay == "Tue")
			{
				isOpendDay =it->mValue.Tuesday;
			}
			else if(weekDay == "Wed")
			{
				isOpendDay =it->mValue.Wednesday;
			}
			else if(weekDay == "Thu")
			{
				isOpendDay =it->mValue.Thursday;
			}
			else if(weekDay == "Fri")
			{
				isOpendDay =it->mValue.Friday;
			}
			else if(weekDay == "Sat")
			{
				isOpendDay =it->mValue.Saturday;
			}
			else if(weekDay == "Sun")
			{
				isOpendDay =it->mValue.Sunday;
			}

			welfareMarket.id =it->mValue.id;		

			if (it->mValue.id ==1)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyChaiCount;
			}
			else if (it->mValue.id ==2)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyMiCount;
			}
			else if (it->mValue.id ==3)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyYouCount;
			}
			else if (it->mValue.id ==4)
			{
				finishTimes =player->getPlayerDailyResetData().m_nDailyYanCount;
			}
			do 
			{
				if (level >= it->mValue.difficulty6.levelRequireDifficulty)
				{
					difficulty =6;
					if (msg.doType == 0 ||(msg.doType == 1 && msg.difficulty == difficulty))//扫荡 最高难度
					{
						awardID = it->mValue.difficulty6.awardDifficulty;
						awardChange = it->mValue.difficulty6.awardChangeDifficulty;
						break;
					}
					
				}
				if (level >= it->mValue.difficulty5.levelRequireDifficulty)
				{
					difficulty =5;
					
					if (msg.doType == 0 ||(msg.doType == 1 && msg.difficulty == difficulty))//扫荡 最高难度
					{
						awardID = it->mValue.difficulty5.awardDifficulty;
						awardChange = it->mValue.difficulty5.awardChangeDifficulty;
						break;
					}
				}
				if (level >= it->mValue.difficulty4.levelRequireDifficulty)
				{
					difficulty =4;
					if (msg.doType == 0 ||(msg.doType == 1 && msg.difficulty == difficulty))//扫荡 最高难度
					{
						awardID = it->mValue.difficulty4.awardDifficulty;
						awardChange = it->mValue.difficulty4.awardChangeDifficulty;
						break;
					}
				}
				if (level >= it->mValue.difficulty3.levelRequireDifficulty)
				{
					difficulty =3;
					if (msg.doType == 0 ||(msg.doType == 1 && msg.difficulty == difficulty))//扫荡 最高难度
					{
						awardID = it->mValue.difficulty3.awardDifficulty;
						awardChange = it->mValue.difficulty3.awardChangeDifficulty;
						break;
					}
				}
				if (level >= it->mValue.difficulty2.levelRequireDifficulty)
				{
					difficulty =2;
					if (msg.doType == 0 ||(msg.doType == 1 && msg.difficulty == difficulty))//扫荡 最高难度
					{
						awardID = it->mValue.difficulty2.awardDifficulty;
						awardChange = it->mValue.difficulty2.awardChangeDifficulty;
						break;
					}
				}
				if (level >= it->mValue.difficulty1.levelRequireDifficulty)
				{
					difficulty =1;
					if (msg.doType == 0 ||(msg.doType == 1 && msg.difficulty == difficulty))//扫荡 最高难度
					{
						awardID = it->mValue.difficulty1.awardDifficulty;
						awardChange = it->mValue.difficulty1.awardChangeDifficulty;
						break;
					}
				}
			} while (0);
			maxTimes = it->mValue.dailyBaseTimes + vipAddTimes;
			welfareMarket.difficulty = difficulty;
			welfareMarket.maxTimes = maxTimes;
			costNum = it->mValue.sweepCostRmbEachTimes;
			break;
		}
	}

	if (awardID == 0&&resp.result ==LynxErrno::None&&(msg.doType == 0 ||msg.doType == 1))
	{
		resp.result =LynxErrno::LevelNotEnough;
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender(connId,WELFALE_MARKET_END_RESP,jsonStr);
		return;
	}

	if (isOpendDay != 1)
	{
		resp.result =LynxErrno::TimeNotRight;
	}
	if (maxTimes <= finishTimes)
	{
		resp.result =LynxErrno::TimesNotEnough;
	}
	
	if (resp.result == LynxErrno::None)
	{
		if (msg.doType ==0)
		{
			player->ResetFireConfirmData();
			if (player->getPlayerGold() < costNum*(maxTimes - finishTimes))
			{
				resp.result =LynxErrno::RmbNotEnough;
			}
			else
			{
				doneTimes = (maxTimes - finishTimes);

				goods.resourcestype =1;
				goods.subtype = 2;
				goods.num = 0 - costNum*(maxTimes - finishTimes);
				cost.insertTail(goods);
				finishTimes = maxTimes;
			}		
		}
		else if (msg.doType ==1)
		{
			doneTimes =1;
			finishTimes ++;		
		}
	}
	
	if (resp.result == LynxErrno::None)//身上加物品并发送给客户端
	{	
		if (msg.id == 3)
		{
			dailyResetData.m_nDailyYouCount = finishTimes;
			for(UInt32 jj =0;jj< doneTimes;jj++)
			{
				GiftManager::getSingleton().getAwardByID(awardID,0, tmpItemList);
			}		
			for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
			{
				GiftManager::getSingleton().getContentByID(it->mValue.subtype,award);
			}


		}
		else if (msg.id == 1)
		{	
			dailyResetData.m_nDailyChaiCount = finishTimes;
			goods.num = 0;
			
			for(UInt32 jj =0;jj< doneTimes;jj++)
			{
				float randNum = rand()%100;			
				goods.num += (1 + ((randNum/100) *2 - 1)*awardChange) * awardID;					
			}
			goods.resourcestype =1;
			goods.subtype = 1;			
			award.insertTail(goods);
		}
		else if (msg.id == 2)
		{		
			dailyResetData.m_nDailyMiCount = finishTimes;
			goods.num = 0;
			for(UInt32 jj =0;jj< doneTimes;jj++)
			{
				float randNum = rand()%100;			
				goods.num += (1 + ((randNum/100) *2 - 1)*awardChange) * awardID;			
			}
			goods.resourcestype =1;
			goods.subtype = 4;			
			award.insertTail(goods);
		}
		else if (msg.id == 4)
		{	
			dailyResetData.m_nDailyYanCount = finishTimes;
			goods.num = 0;
			for(UInt32 jj =0;jj< doneTimes;jj++)
			{
				float randNum = rand()%100;			
				goods.num += (1 + ((randNum/100) *2 - 1)*awardChange) * awardID;	
			}
			goods.resourcestype =1;
			goods.subtype = 6;			
			award.insertTail(goods);
		}
		player->setPlayerDailyResetData(dailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);
	
		resp.cost = cost;
		Award tmpAward;
		tmpAward.award= award;
		resp.awards.insertTail(tmpAward);

	}


// 	奖励合起来这里没有翻牌
// 	for(List<Award>::Iter * item = mFireConfirmData.m_AwardsList.begin();item != NULL; item = mFireConfirmData.m_AwardsList.next(item))
// 	{
// 		for(List<Goods>::Iter * it = item->mValue.award.begin(); it != NULL; it = item->mValue.award.next(it))
// 		{				
// 			tmpItemList.insertTail(it->mValue);
// 		}
// 	}
// 	award.award=tmpItemList;		
// 	mFireConfirmData.m_AwardsList.clear();
// 	mFireConfirmData.m_AwardsList.insertTail(award);

	ChapterEndResp tmpResp;

	
	PlayerFireConfirmData mFireConfirmData = player->GetFireConfirmData();

// 	mFireConfirmData.m_FirstAwardsList = resp.firstAwards;
	mFireConfirmData.m_AwardsList = resp.awards;
	mFireConfirmData.m_CostList = resp.cost;
	mFireConfirmData.m_CardsList = resp.cards;
	mFireConfirmData.m_MonsterAwardList = resp.monsterDropList;
	player->SetFireConfirmData(mFireConfirmData);

	if (msg.doType == 0)
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,tmpResp,0,MiniLog61);
	}
	else
	{
		FireConfirmManager::getSingleton().saveAndGetResult(connId,tmpResp,1,MiniLog60);
	}
	
// 	resp.firstAwards = tmpResp.firstAwards;
	resp.awards = tmpResp.awards;
	resp.cost = tmpResp.cost;
	resp.cards = tmpResp.cards;
	resp.monsterDropList = tmpResp.monsterDropList;
	resp.fixedList = tmpResp.fixedList;
// 	resp.ends = tmpResp.ends;

	resp.chaiCount = dailyResetData.m_nDailyChaiCount;
	resp.miCount = dailyResetData.m_nDailyMiCount;
	resp.youCount = dailyResetData.m_nDailyYouCount;
	resp.yanCount = dailyResetData.m_nDailyYanCount;
	resp.id = msg.id;
	resp.doType = msg.doType;
 	resp.difficulty = msg.difficulty;
	resp.allAttr = tmpResp.allAttr;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,WELFALE_MARKET_END_RESP,jsonStr);

	//招福集市每日任务打点
	if(msg.doType == 0 || msg.doType == 1)
	{
		player->getAchieveManager().updateDailyTaskData(DLYDOMARKETCNT, 1 *doneTimes);
	}

	if( msg.doType == 0  )
	{
// 		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT01,doneTimes);	
		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT12,doneTimes);		
	}
	else if(msg.doType == 1)
	{
		//更新七日训
		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT12,1);		
	}
	

	
}



void FireConfirmManager::onStrengthReq(const  ConnId& connId ,CGStrength &msg )	//体力
{

	UInt32 maxBuyTimes =0;	
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 finishTimes;
	UInt32 flag = 1;
	UInt32 price = 0;
	UInt32 num = 0;
	UInt32 nowBuyTimes = 0;
	UInt32 lastStrengthNum = 0;
	double maxCanAddTimes;
	double lastFinishTimes =0;
	char dest[1024]={0};
	List<UInt32> foods;
	List<Goods> itemList;
	Goods goods;
	PlayerStrengthData strengthData;
	PlayerBaseData baseData;
	StrengthResp resp;

	msg.convertJsonToData(msg.jsonStr);
	resp.reqType = msg.reqType;
	Player * player  = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if(!player)
	{
		return;
	}
	PlayerExpTemplate *expTemplate = gPlayerExpTable->get(player->getPlayerLeval());

	if (expTemplate == NULL)
	{
		LOG_WARN("expTemplate not found!!");
		return;
	}
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	strengthData = player->getStrengthData();
	baseData = player->getPlayerBaseData();



	maxBuyTimes = ShopManager::getSingleton().getStrengthBuyMaxTimes(player->getVipLevel());

	UInt32 systemRefreshHour =  LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID04) /3600;

	if(TimeManager::timeIsTodayDelayHours(strengthData.buyTime, systemRefreshHour) ==false)
	{
		strengthData.buyTime = nowTime;
		strengthData.vipLeftTimes = 0;
		player->setStrengthData(strengthData);
		player->getPersistManager().setDirtyBit(STRENGTHDATABIT );
	}

	lastFinishTimes = (nowTime - strengthData.beginTime) / (globalValue.uSTtime);


	if(lastFinishTimes < 0)//有时时间异常
	{
		finishTimes = 0;
	}
	else if (lastFinishTimes > expTemplate->mStrength)
	{
		finishTimes = expTemplate->mStrength;
	}
	else
	{
		finishTimes = lastFinishTimes;
	}


	lastStrengthNum = strengthData.strength;
	maxCanAddTimes =(double) expTemplate->mStrength -  strengthData.strength;
	if (maxCanAddTimes > 0 )
	{
		if (maxCanAddTimes <= finishTimes )//次数满了
		{
			strengthData.strength += maxCanAddTimes;	
			strengthData.beginTime = nowTime;

			snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,maxCanAddTimes,strengthData.strength );
			LogicSystem::getSingleton().write_log( LogType62,baseData.m_nPlayerID, dest,LogInfo);

		}
		else //次数没满还要倒计时
		{
			strengthData.strength += finishTimes;
			strengthData.beginTime = strengthData.beginTime + finishTimes * (globalValue.uSTtime);	

			snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,finishTimes,strengthData.strength );
			LogicSystem::getSingleton().write_log( LogType62,baseData.m_nPlayerID, dest,LogInfo);
		}			
	}
	else
	{
		strengthData.beginTime = nowTime;
	}

	resp.flag = flag;
	resp.gold = player->getPlayerGold();

	if (msg.reqType == 0)//处理一次 不发送数据
	{	
		player->setStrengthData(strengthData);
		return;
	}
	else if (msg.reqType == 1)//获取信息
	{		
		if (strengthData.beginTime ==0)
		{
			resp.needTime =  globalValue.uSTtime;
		}
		else
		{
			resp.needTime =   globalValue.uSTtime -  (nowTime - strengthData.beginTime)%(globalValue.uSTtime);	
		}

		resp.viableAmount = expTemplate->mStrength;
		resp.restbuyNumber = strengthData.vipLeftTimes;
// 		resp.vipStrengthLeftNumber = maxBuyTimes;
		resp.strength = strengthData.strength;
		

		if (msg.needSend == true)
		{
			std::string jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,CHAPTER_STRENGTH_RESP,jsonStr);
		}	

		player->setStrengthData(strengthData);
		player->getPersistManager().setDirtyBit(STRENGTHDATABIT);
		return;
	}
	
	else if (msg.reqType == 3)//购买次数
	{	
		if (msg.num != 1)
		{
			resp.flag = LynxErrno::ClienServerDataNotMatch;
			std::string sendStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,CHAPTER_STRENGTH_RESP,sendStr);
			return;
		}
		
		nowBuyTimes = strengthData.vipLeftTimes + 1;
		FireConfirmManager::getSingleton().getBuyStrengthPrice(player->getPlayerGuid(),nowBuyTimes,price,num);

		do 
		{
			if(baseData.m_nGold < msg.num * price )
			{
				flag = LynxErrno::RmbNotEnough;
				break;
			}
			if( (int)(maxBuyTimes - strengthData.vipLeftTimes) < msg.num  )
			{
				flag = LynxErrno::TimesNotEnough;
				break;
			}
			if ((strengthData.strength+ msg.num * num)  > globalValue.uSTmax)
			{
				flag = LynxErrno::OverMaxTimes;
				break;
			}
			strengthData.vipLeftTimes += msg.num ;
			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num -= msg.num * price;		
			itemList.insertTail(goods);
			lastStrengthNum = strengthData.strength;
			strengthData.strength += msg.num * num;
			strengthData.buyTime = nowTime;
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog58);

			snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,msg.num * num,strengthData.strength );
			LogicSystem::getSingleton().write_log( LogType63,baseData.m_nPlayerID, dest,LogInfo);

		} while (0);
	}
	else if (msg.reqType == 8)//增加 todo 规则不明确
	{		

		if(strengthData.strength > globalValue.uSTmax)
		{
			flag = LynxErrno::OverMaxTimes;

// 			int addCount = globalValue.uSTmax - strengthData.strength;
// 			if (addCount < 0)
// 			{
// 				strengthData.strength +=0;
// 			}
// 			else
// 			{
// 				lastStrengthNum = strengthData.strength;
// 				strengthData.strength +=addCount;
// 
// 				snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,addCount,strengthData.strength );
// 				LogicSystem::getSingleton().write_log( LogType63,baseData.m_nPlayerID, dest,LogInfo);
// 			}
		}
		else
		{
			lastStrengthNum = strengthData.strength;
			strengthData.strength += msg.num;		

			if ( msg.num != 0)
			{
				snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,msg.num,strengthData.strength );
				LogicSystem::getSingleton().write_log( LogType63,baseData.m_nPlayerID, dest,LogInfo);
			}
			
		}

	}
	else if (msg.reqType == 9)//减少 //时间前面已经重置
	{				
		if(strengthData.strength  < msg.num)
		{
			lastStrengthNum = strengthData.strength;
			strengthData.strength = 0;		

			int changNum = strengthData.strength - lastStrengthNum;
			snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,changNum,strengthData.strength );
			LogicSystem::getSingleton().write_log( LogType64,baseData.m_nPlayerID, dest,LogInfo);

		}
		else
		{
			lastStrengthNum = strengthData.strength;
			strengthData.strength -= msg.num;
			int changNum = strengthData.strength - lastStrengthNum;
			snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,changNum,strengthData.strength );
			LogicSystem::getSingleton().write_log( LogType64,baseData.m_nPlayerID, dest,LogInfo);

		}
	}
	else if (msg.reqType == 10)//清空美食屋
	{
		lastStrengthNum = strengthData.strength;
		strengthData.leftTimes = 0;
		strengthData.beginTime = nowTime;
		strengthData.strength = 0;		
		strengthData.vipLeftTimes = 0;
		player->setStrengthData(strengthData);
		player->getPersistManager().setDirtyBit(STRENGTHDATABIT );

		int changNum = strengthData.strength - lastStrengthNum;
		snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,changNum,strengthData.strength );
		LogicSystem::getSingleton().write_log( LogType64,baseData.m_nPlayerID, dest,LogInfo);
		return;
	}
	else if (msg.reqType == 11)//设置成最大值
	{

		lastStrengthNum = strengthData.strength;
		
		strengthData.strength = expTemplate->mStrength;
		strengthData.beginTime = nowTime;
		strengthData.vipLeftTimes = 0;
		strengthData.vipFoodLeftNumber = 0;
		player->setStrengthData(strengthData);
		player->getPersistManager().setDirtyBit(STRENGTHDATABIT );

		int changNum = strengthData.strength - lastStrengthNum;
		snprintf(dest,sizeof(dest),"%d,%d,%d",lastStrengthNum,changNum,strengthData.strength );
		LogicSystem::getSingleton().write_log( LogType64,baseData.m_nPlayerID, dest,LogInfo);

		return;
	}

	player->setStrengthData(strengthData);
	player->getPersistManager().setDirtyBit(STRENGTHDATABIT);
	

	resp.gold = player->getPlayerGold();
	resp.flag = flag;
	if (strengthData.beginTime ==0)
	{
		resp.needTime =  globalValue.uSTtime;
	}
	else
	{
		resp.needTime =  globalValue.uSTtime -  (nowTime - strengthData.beginTime)%(globalValue.uSTtime);	
	}

	resp.viableAmount = expTemplate->mStrength;
	resp.restbuyNumber = strengthData.vipLeftTimes;
// 	resp.vipStrengthLeftNumber = maxBuyTimes;
	resp.strength = strengthData.strength;
	
	if (msg.needSend == true)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,CHAPTER_STRENGTH_RESP,jsonStr);
	}

	if(msg.reqType == 3&&flag == 1)//flag和result有区别
	{
		//每日任务购买体力打点 wwc
		player->getAchieveManager().updateDailyTaskData(DLYBUYENERGE, 1 );
		//更新七日训
		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT05,1);
	}

}

UInt32 FireConfirmManager::getBuyStrengthPrice(Guid playerID,UInt32 nowBuyTimes,UInt32 &cost,UInt32 &num)
{
	Player * player  = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	UInt32 vipLevel = player->getVipLevel();


	for(List<GainStrengthTemplate>::Iter *iter = gGainStrengthTable->mMap.begin();iter != NULL;iter = gGainStrengthTable->mMap.next(iter) )
	{
		if (iter->mValue.viplevel == vipLevel&&iter->mValue.buytimes == nowBuyTimes)
		{
			cost = iter->mValue.cost;
			num =  iter->mValue.addstrength;
			break;
		}
	}
	return 0;

}

UInt32 FireConfirmManager::getBuyCoinPrice(Guid playerID,UInt32 nowBuyTimes,UInt32 &cost,UInt32 &num)
{
	Player * player  = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	UInt32 vipLevel = player->getVipLevel();


	for(List<GainWealthTemplate>::Iter *iter = gGainWealthTable->mMap.begin();iter != NULL;iter = gGainWealthTable->mMap.next(iter) )
	{
		if (iter->mValue.VIPlevel == vipLevel&&iter->mValue.times == nowBuyTimes)
		{
			cost = iter->mValue.cost;	
			num =  iter->mValue.gaincoin;
			break;
		}
	}
	return 0;

}

UInt32 FireConfirmManager::refreshgetStrength(const ConnId& connId)
{
	CGStrength strengthmsg;
	strengthmsg.reqType = 1;
	strengthmsg.needSend = false;
	FireConfirmManager::onStrengthReq(connId,strengthmsg);

	Player * player  = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return 0;
	}
	PlayerStrengthData strengthData = player->getStrengthData();

	return strengthData.strength;
}