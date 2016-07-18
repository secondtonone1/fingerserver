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

//for test
#include "Code.h"
#include "InlineActivity.h"
#include "Robot.h"



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
	SendFireConfirmCondition(connId);
}

void FireConfirmManager::ConfirmDataReq(const ConnId& connId, CGConfirmDataReq& msg)
{
	UInt32 iFlag = 0;
	bool bFlag = false;
	List<Int32> RecList;

	msg.convertJsonToData(msg.strConfirmData);
	iFlag = msg.confirmID;
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	List<ItemData>* m_pListItemData = new List<ItemData>();
	ItemData data;
	*m_pListItemData->insertTail(data);
	m_pListItemData->insertTail(data);
	List<ItemData>::Iter *it = m_pListItemData->begin();


	FireConfirmData = player->GetFireConfirmData();
	if (FireConfirmData.m_RecConfirmIDs.size()<31)
	{
		FireConfirmData.m_RecConfirmIDs.insertTail(iFlag);
		player->SetFireConfirmData(FireConfirmData);
	}

	for( List<Int32>::Iter * it = msg.confirmDataList.begin(); it != NULL; it = msg.confirmDataList.next(it))
	{
		RecList.insertTail(it->mValue);
	}

	HeroTemplate herotemplate;
	for (Map<UInt32, HeroTemplate>::Iter *iter = gHeroTable->mMap.begin();iter != NULL;iter = gHeroTable->mMap.next(iter))
	{
		if ((iter->mKey) == player->mPlayerData.mBaseData.m_nModelID)
		{
			herotemplate = iter->mValue; 
		}
	}

	if (iFlag == CONFIRM_SPEED ) //RecList //speed speed
	{

		Int32 MaxSpeed = herotemplate.mMoveSpeed;			

		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
		{

			if (MaxSpeed * OUT_OF_RANGE < iter->mValue )
			{
				bFlag = false;
			}
			else
			{
				bFlag = true;
			}
		}

		UpdataConfirmLevel( connId, bFlag);
	}
	if (iFlag == CONFIRM_ATTACK_RATE ) //RecList //time1 time2
	{
		double AttackSpeed =-2/1/* herotemplate.mAttackSpeed*/;
		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
		{

			if (RecList.next(iter) == NULL)
			{
				break;
			}

			if (AttackSpeed* OUT_OF_RANGE < 1/( ( RecList.next(iter)->mValue) -  (iter->mValue) ) )
			{
				bFlag = false;
				break;
			}
			else
			{
				bFlag = true;
			}
		}

		UpdataConfirmLevel( connId, bFlag);
	}

	if (iFlag == CONFIRM_SKILL_CD ) //RecList //time1 skill1 time2  skill2 
	{
		Int32 SkillCDTime = 0;	
		SkillData skillData;
		List<SkillData> listskills =  player->mPlayerData.mSkillListData.m_listSkills;
		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
		{
			iter = RecList.next(iter);
			if (iter == NULL)
			{
				break;
			}
			//两个技能之间释放间隔


			for( List<Int32>::Iter * iter1 = RecList.next(iter); iter1 != NULL; iter1 = RecList.next(iter1)) //判断一个技能释放后到下一次释放时间
			{
				iter1 = RecList.next(iter1);
				if (iter1 == NULL)
				{
					break;
				}

				if(iter->mValue == iter1->mValue)
				{
					if (((RecList.prev(iter1)->mValue - RecList.prev(iter1)->mValue) * OUT_OF_RANGE) < SkillCDTime)
					{
						bFlag = false;
						break;
					}
					bFlag = true;
				}
			}
			iter = RecList.next(iter);			 
		}

		UpdataConfirmLevel( connId, bFlag);
	}
	//mark:技能伤害还没读配置文件 ActorSkillInfoMap
	if (iFlag == CONFIRM_SKILL_DAMAGE ) //RecList //damage1 skill1 damage2  skill2 
	{

// 		Int32 Damage;

		//sec
		//SkillTemplate skilltemplate;

		List<SkillData> listskills =  player->mPlayerData.mSkillListData.m_listSkills;
		for( List<Int32>::Iter * iter = RecList.next( RecList.begin() ); iter != NULL; iter = RecList.next(iter))
		{

			/*skilltemplate = *gSkillTable->get(RecList.next(iter)->mValue);

			Damage = skilltemplate.mPhysicsAttack + skilltemplate.mMagicAttack;
			if (Damage * OUT_OF_RANGE <  RecList.prev(iter)->mValue)
			{
				bFlag = false;
			}
			else
			{
				bFlag = true;
			}


			iter = RecList.next(iter);*/

		}

		UpdataConfirmLevel( connId, bFlag);
	}
	if (iFlag == CONFIRM_CRITICAL_STRIKE_RATE )//mark:难以计算 暂放下
	{
		//BattleExtendData
		Int32 SkillID = 0;
		Int32 SkillCDTime = 0;
		BattleExtendData battleextenddata;
		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
		{
			if (SkillID ==  iter->mValue)
			{
				if (SkillCDTime * OUT_OF_RANGE <  iter->mValue )
				{
					bFlag = false;
				}
				else
				{
					bFlag = true;
				}
			}
			iter = RecList.next(iter);

		}

		UpdataConfirmLevel( connId, bFlag);
	}
	if (iFlag == CONFIRM_CRITICAL_STRIKE_MAX_DAMAGE )
	{
		Int32 SkillID = 0;
		Int32 SkillCDTime = 0;
		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
		{
			if (SkillID ==  iter->mValue)
			{
				if (SkillCDTime * OUT_OF_RANGE <  iter->mValue )
				{
					bFlag = false;
				}
				else
				{
					bFlag = true;
				}
			}
			iter = RecList.next(iter);

		}

		UpdataConfirmLevel( connId, bFlag);
	}
	if (iFlag == CONFIRM_SKILL_RANGE )//mark:技能范围没读取配置文件 //skillmulti.xlsx attackarea1? //skillid posx,posy,posx2,posy2
	{
		Int32 SkillID = 0;
		Int32 SkillCDTime = 0;		
		Int32 SkillLevel;
		Int32 Damage;

		List<SkillData> listskills =  player->mPlayerData.mSkillListData.m_listSkills;

		for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
		{

			for( List<SkillData>::Iter * it = listskills.begin(); it != NULL; it = listskills.next(it))			 
			{				  
				SkillID = (Int32) (it->mValue.m_nID);
				SkillCDTime = (Int32) (it->mValue.m_nCD);
				SkillLevel = (Int32) (it->mValue.m_nLevel);

				if (SkillID ==  iter->mValue)
				{
					Damage = SkillLevel;

					if (SkillCDTime * OUT_OF_RANGE <  RecList.prev(iter)->mValue)
					{
						bFlag = false;
					}
					else
					{
						bFlag = true;
					}
				}
				iter = RecList.next(iter);
			}
			UpdataConfirmLevel( connId, bFlag);
		}

		if (iFlag == CONFIRM_POS )
		{
			Int32 SkillID = 0;
			Int32 SkillCDTime = 0;
			for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
			{
				if (SkillID ==  iter->mValue)
				{
					if (SkillCDTime * OUT_OF_RANGE <  iter->mValue )
					{
						bFlag = false;
					}
					else
					{
						bFlag = true;
					}
				}
				iter = RecList.next(iter);
			}

			UpdataConfirmLevel( connId, bFlag);
		}
		if (iFlag == CONFIRM_HP || iFlag == CONFIRM_MP || iFlag == CONFIRM_SP)//受到攻击 吃药膏 一段时间后血量变化
		{
			Int32 ChangValue = 0;
			Int32 AllUpDownValue = 0;

			ChangValue =(RecList.end()->mValue - RecList.begin()->mValue)* OUT_OF_RANGE ;

			for( List<Int32>::Iter * iter = RecList.next(RecList.begin()); iter != RecList.prev(RecList.end()); iter = RecList.next(iter))
			{
				AllUpDownValue += iter->mValue;
			}

			if (ChangValue < 0)
			{
				if (ChangValue > AllUpDownValue)//-120 > -100
				{
					bFlag = false;
				}
				else
				{
					bFlag = true;
				}
			}
			else
			{
				if (ChangValue < AllUpDownValue)
				{
					bFlag = false;
				}
				else
				{
					bFlag = true;
				}
			}			
			UpdataConfirmLevel( connId, bFlag);
		}

		if (iFlag == CONFIRM_BUFFER )//RecList //bufferid gettime //不能是同类buff，不是自己的buff或者不是队友的buff或者不是怪物伤害buff //每30s发送一次
		{
			// 			Int32 SkillCDTime;
			List<BufferData>BuffListData =  player->mPlayerData.mBuffListData.m_listBuffers;
			for( List<Int32>::Iter * iter = RecList.begin(); iter != NULL; iter = RecList.next(iter))
			{
				for( List<BufferData>::Iter * it = BuffListData.begin(); it != NULL; it = BuffListData.next(it))
				{
					if (iter->mValue == (Int64)it->mValue.m_nBufferID)
					{
						RecList.erase(iter);
					}
				}		

			}
			if (RecList.mIterCount > 0 )
			{
				bFlag = false;
			}
			else
			{
				bFlag = true;
			}

			UpdataConfirmLevel( connId, bFlag);
		}
		return;
	}

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


void FireConfirmManager::SendFireConfirmCondition(const ConnId& connId)
{

	List<UInt32> ConfirmIDs;
	UInt32 ConfirmMaxNum;
	UInt32 NeedConfirmIDNum;
	FireConfirmDataNotifyToJson resp;	
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	FireConfirmData = player->GetFireConfirmData();

	ConfirmMaxNum =(UInt32) CONFIRM_MAX-1;
	NeedConfirmIDNum = GetConfirmTimes(FireConfirmData.m_ConfirmLevel);
	ChoiseNFromEnum(NeedConfirmIDNum,ConfirmMaxNum,ConfirmIDs);

	FireConfirmData.m_ConfirmIDs.clear();
	for (List<UInt32>::Iter  *iter = ConfirmIDs.begin();iter != ConfirmIDs.end(); iter = ConfirmIDs.next(iter))
	{
		FireConfirmData.m_ConfirmIDs.insertTail(iter->mValue);
		resp.ConfirmIDs.insertTail(iter->mValue);
	}
	player->SetFireConfirmData(FireConfirmData);
	resp.mConfirmLevel = FireConfirmData.m_ConfirmLevel;

	PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();

	resp.m_NormalTimes =  playerDailyResetData.m_NormalTimes;
	resp.m_SpecialTimes =  playerDailyResetData.m_SpecialTimes;
	resp.m_EliteTimes =  playerDailyResetData.m_EliteTimes;
	resp.m_Times = FireConfirmData.m_Times;

	std::string jsonStr = resp.convertDataToJson();	

	NetworkSystem::getSingleton().sender( connId,FIRE_CONDITION_RESP,jsonStr);
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
	
	resp.chapterID = msg.stageID;
	resp.result =LynxErrno::None;
	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,CHAPTER_START_RESP,jsonStr);

	FireConfirmManager::getSingleton().SendFireConfirmCondition(connId);

	
}

void FireConfirmManager::onLeaveCopy(const  ConnId& connId,CGChapterLeave & msg)		//离开关卡
{
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	msg.convertJsonToData(msg.jsonStr);
	player->ResetFireConfirmData();
	ChapterLeaveResp resp;
	resp.chapterID = msg.chapterID;
	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,CHAPTER_LEAVE_RESP,jsonStr);
}



void FireConfirmManager::onRelive(const  ConnId& connId, CGRelive & msg)		//复活
{
	UInt32 cost = 0;
	UInt32 i =0;
	ReliveResp resp;
	Goods goods;
	List<Goods> itemList;

	PlayerFireConfirmData fireConfirmData;

// 	resp.flag =LynxErrno::None;
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	fireConfirmData = player->GetFireConfirmData();
	StageTemplate *stageTemplate = gStageTable->get(fireConfirmData.m_CopyID);

	PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();

	msg.convertJsonToData(msg.jsonStr);

 	resp.flag = msg.flag;
	if (stageTemplate->mType == STAGE_NORMAL)
	{
		NormalReliveTemplate normalReliveTemplate;
		normalReliveTemplate = GlobalVarManager::getSingleton().getNormalRelive();
		
		if (normalReliveTemplate.dayfreetimes > playerDailyResetData.m_NormalTimes )
		{
			playerDailyResetData.m_NormalTimes ++;
		}
		else
		{			
			for(List<UInt32>::Iter* it = normalReliveTemplate.costs.begin();it != NULL;it = normalReliveTemplate.costs.next(it))
			{
				
				if (i == fireConfirmData.m_Times)
				{
					cost = it->mValue;
					break;
				}
				i++;
			}

			if (player->getPlayerGold() >= cost)
			{
				fireConfirmData.m_Times ++;

				goods.resourcestype =1;
				goods.subtype = 2;
				goods.num -= cost;
				itemList.insertTail(goods);
				GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
				resp.yuanbaoCost = player->getPlayerGold();
			}
			else
			{
				resp.result = LynxErrno::RmbNotEnough;				
			}
		}
	}
	if (stageTemplate->mType == STAGE_ELITE)
	{
		EliteReliveTemplate eliteReliveTemplate;
		eliteReliveTemplate = GlobalVarManager::getSingleton().getEliteRelive();

		if (eliteReliveTemplate.dayfreetimes > playerDailyResetData.m_NormalTimes )
		{
			playerDailyResetData.m_NormalTimes ++;
		}
		else
		{			
			for(List<UInt32>::Iter* it = eliteReliveTemplate.costs.begin();it != NULL;it = eliteReliveTemplate.costs.next(it))
			{
				i++;
				if (i == fireConfirmData.m_Times)
				{
					cost = it->mValue;
				}
			}

			if (player->getPlayerGold() >= cost)
			{
				fireConfirmData.m_Times ++;

				goods.resourcestype =1;
				goods.subtype = 2;
				goods.num -= cost;
				itemList.insertTail(goods);
				GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
				resp.yuanbaoCost = player->getPlayerGold();
			}
			else
			{
				resp.flag = LynxErrno::RmbNotEnough;				
			}
		}
	}
	if (stageTemplate->mType == STAGE_BONUS)
	{
		SpecialReliveTemplate specialReliveTemplate;
		specialReliveTemplate = GlobalVarManager::getSingleton().getSpecialRelive();

		if (specialReliveTemplate.dayfreetimes > playerDailyResetData.m_NormalTimes )
		{
			playerDailyResetData.m_NormalTimes ++;
		}
		else
		{			
			for(List<UInt32>::Iter* it = specialReliveTemplate.costs.begin();it != NULL;it = specialReliveTemplate.costs.next(it))
			{
				i++;
				if (i == fireConfirmData.m_Times)
				{
					cost = it->mValue;
				}
			}

			if (player->getPlayerGold() >= cost)
			{
				fireConfirmData.m_Times ++;

				goods.resourcestype =1;
				goods.subtype = 2;
				goods.num -= cost;
				itemList.insertTail(goods);
				GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
				resp.yuanbaoCost = player->getPlayerGold();
			}
			else
			{
				resp.flag = LynxErrno::RmbNotEnough;				
			}
		}
	}
	if (stageTemplate->mType == STAGE_MASTAR)
	{
		SpecialReliveTemplate specialReliveTemplate;
		specialReliveTemplate = GlobalVarManager::getSingleton().getSpecialRelive();

		if (specialReliveTemplate.dayfreetimes > playerDailyResetData.m_NormalTimes )
		{
			playerDailyResetData.m_NormalTimes ++;
		}
		else
		{			
			for(List<UInt32>::Iter* it = specialReliveTemplate.costs.begin();it != NULL;it = specialReliveTemplate.costs.next(it))
			{
				i++;
				if (i == fireConfirmData.m_Times)
				{
					cost = it->mValue;
				}
			}

			if (player->getPlayerGold() >= cost)
			{
				fireConfirmData.m_Times ++;

				goods.resourcestype =1;
				goods.subtype = 2;
				goods.num -= cost;
				itemList.insertTail(goods);
				GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
				resp.yuanbaoCost = player->getPlayerGold();
			}
			else
			{
				resp.flag = LynxErrno::RmbNotEnough;				
			}
		}
	}
	if (stageTemplate->mType == STAGE_SPECIAL)
	{
		SpecialReliveTemplate specialReliveTemplate;
		specialReliveTemplate = GlobalVarManager::getSingleton().getSpecialRelive();

		if (specialReliveTemplate.dayfreetimes > playerDailyResetData.m_NormalTimes )
		{
			playerDailyResetData.m_NormalTimes ++;
		}
		else
		{			
			for(List<UInt32>::Iter* it = specialReliveTemplate.costs.begin();it != NULL;it = specialReliveTemplate.costs.next(it))
			{
				i++;
				if (i == fireConfirmData.m_Times)
				{
					cost = it->mValue;
				}
			}

			if (player->getPlayerGold() >= cost)
			{
				fireConfirmData.m_Times ++;

				goods.resourcestype =1;
				goods.subtype = 2;
				goods.num -= cost;
				itemList.insertTail(goods);
				GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
				resp.yuanbaoCost = player->getPlayerGold();
			}
			else
			{
				resp.flag = LynxErrno::RmbNotEnough;				
			}
		}
	}
	if (stageTemplate->mType == STAGE_TRIAL)
	{

		TrialTemplate trialTemplate = GlobalVarManager::getSingleton().gettrial();

// 		if ((trialTemplate.dayfreetimes+fireConfirmData.m_trialTimes)
// 		{
// 		}
// 
		SpecialReliveTemplate specialReliveTemplate;
		specialReliveTemplate = GlobalVarManager::getSingleton().getSpecialRelive();
		if (trialTemplate.dayfreetimes > fireConfirmData.m_trialTimes )
		{
			fireConfirmData.m_trialTimes ++;
		}
		else
		{			
			for(List<UInt32>::Iter* it = specialReliveTemplate.costs.begin();it != NULL;it = specialReliveTemplate.costs.next(it))
			{
				i++;
				if (i == fireConfirmData.m_Times)
				{
					cost = it->mValue;
				}
			}

			if (player->getPlayerGold() >= cost)
			{
				fireConfirmData.m_Times ++;

				goods.resourcestype =1;
				goods.subtype = 2;
				goods.num -= cost;
				itemList.insertTail(goods);
				GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
				resp.yuanbaoCost = player->getPlayerGold();
			}
			else
			{
				resp.flag = LynxErrno::RmbNotEnough;				
			}
		}
	}
	player->setPlayerDailyResetData(playerDailyResetData);
	resp.reliveTimes = fireConfirmData.m_Times;
	resp.coinCost = player->getPlayerCoin();
	std::string str;
	str = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,CHAPTER_RELIVE_RESP,str);

}
void FireConfirmManager::onisRecieve(const  ConnId& connId, CGisRecievedReq & msg)		//客户端接收到协议
{
	msg.convertJsonToData(msg.jsonStr);
	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	mFireConfirmData.m_LastSendItemList.clear();
	player->SetFireConfirmData(mFireConfirmData);

}
// onTwelvePalaceEndReq拷贝了onEndCopy代码
void FireConfirmManager::onEndCopy(const  ConnId& connId, CGChapterEnd & msg)		//关卡结束 
{
	UInt32 isRecord =0;//破纪录	
	UInt32 finishTimes =0 ;
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
	

	msg.convertJsonToData(msg.jsonStr);
	resp.chapterID = msg.chapterID;
	resp.result = msg.result;

	
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	
	if (gStageTable->get(msg.chapterID)->mType == STAGE_TRIAL)//多人副本
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
		
	star = msg.star;		

	StageCalcManager::getSingleton().getAwards( connId ,1, msg.awardMonsterList,msg.chapterID,1);

	FireConfirmManager::getSingleton().updateFireConfirmData(connId);

	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	mFireConfirmData.m_Star = msg.star;

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


	UInt32 goldcost =0;
	UInt32  gold = player->getPlayerGold();
	if (gStageTable->get(msg.chapterID)->mType == STAGE_TRIAL)//多人副本 
	{
		TrialTemplate trialTemplate = GlobalVarManager::getSingleton().gettrial();
		goldcost = trialTemplate.ratecost.findValue(mFireConfirmData.m_AwardTimes);//倍数 元宝花费

		if (gold < goldcost)
		{
			resp.result = LynxErrno::RmbNotEnough;

		}
		PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();
		
		
		VipTemplate vipTemplate;//todo mei yanzheng
		for(List<VipTemplate>::Iter *iter1 = gVipTable->mVip.begin();iter1 != NULL;iter1 = gVipTable->mVip.next(iter1))
		{
			if (iter1->mValue.id == player->getVipLevel())
			{
				vipTemplate = iter1->mValue;
				break;
			}
		}
		if (resp.result != LynxErrno::None)//失败
		{
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
			return;
		}	
		double multiply =0;
		multiply += mFireConfirmData.m_AwardTimes;
		Record record;
		record.name = player->getPlayerName();
		record.playerID = player->getPlayerGuid();
		record.val = msg.star;
		if (msg.star <= gStageTable->get(msg.chapterID)->limittime1)
		{
			multiply += 0.1;
			if (RankingManager::getSingleton().setRecord(STAGE_RECORD_TYPE,mFireConfirmData.m_CopyID,record,true) <4)//todo破纪录 4以下是成功
			{
				CGRecord recordMsg;
				recordMsg.typeID = STAGE_RECORD_TYPE;
				RankingManager::getSingleton().onRecordReq( connId,recordMsg);
				recordBreaking =1;
				//todo 加入到成就 
				
			}
		}
		
		if (/*isFriend()*/1)//是否是好友
		{
			multiply += 0.1;
		}


		for(List<Award>::Iter * item = mFireConfirmData.m_AwardsList.begin();item != NULL; item = mFireConfirmData.m_AwardsList.next(item))
		{			
			for( List<Goods>::Iter * iter = item->mValue.award.begin();iter != NULL;iter = item->mValue.award.next(iter))
			{
				iter->mValue.num = iter->mValue.num * (multiply + mFireConfirmData.m_AwardTimes); 
			}
			break;
		}
		for(List<Card>::Iter * item1 = mFireConfirmData.m_CardsList.begin();item1 != NULL; item1 = mFireConfirmData.m_CardsList.next(item1))
		{			
			for( List<Goods>::Iter * iter = item1->mValue.card.begin();iter != NULL;iter = item1->mValue.card.next(iter))
			{
				iter->mValue.num = iter->mValue.num * (multiply + mFireConfirmData.m_AwardTimes); 
			}
			break;
		}

		goods.resourcestype =1;
		goods.subtype =2;
		goods.num -= goldcost;
		mFireConfirmData.m_CostList.insertTail(goods);

		
	}
// 	else//不是多人副本 saveand 里有
// 	{
// 		goods.resourcestype =20;
// 		goods.subtype = msg.chapterID;
// 		goods.num = star;
// // 		mFireConfirmData.m_CostList.insertTail(goods);//消耗副本次数
// 
// 		for(List<Award>::Iter * item = mFireConfirmData.m_AwardsList.begin();item != NULL; item = mFireConfirmData.m_AwardsList.next(item))
// 		{		
// 			item->mValue.award.insertTail(goods);
// 			break;
// 		}
// 
// 	}


	//普通关卡 一般消耗，多人副本为零
	StageTemplate *stageTemplate;
	stageTemplate = gStageTable->get(mFireConfirmData.m_CopyID);
	if (stageTemplate->mStrengthCost >0)
	{
		goods.resourcestype =1;
		goods.subtype =3;
		goods.num -= stageTemplate->mStrengthCost;
		mFireConfirmData.m_CostList.insertTail(goods);
	}
	
	player->SetFireConfirmData(mFireConfirmData);
	FireConfirmManager::getSingleton().saveAndGetResult(connId,resp,1);	
	
	mFireConfirmData =  player->GetFireConfirmData();


	resp.result =LynxErrno::None;
	resp.star = star;
	resp.recordBreaking =recordBreaking;
	resp.allAttr = mFireConfirmData.m_AddSendjs;

	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
	//更新成就系统 wwc
	player->getAchieveManager().updateAchieveData(BARRIER,msg.chapterID);
	player->getAchieveManager().updateAchieveData(ELITEBARRIER,msg.chapterID);
	//更新每日系统 wwc
	UInt32 stateType = gStageTable->get(msg.chapterID)->mType;
	if(stateType == STAGE_NORMAL || stateType==STAGE_ELITE )
	{
		player->getAchieveManager().updateDailyTaskData(DLYBARRIERCNT, 1);
	}
	
	if(stateType == STAGE_ELITE)
	{
			player->getAchieveManager().updateDailyTaskData(DLYELITEBARRIRECNT, 1);
	}
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
		
	GiftManager::getSingleton().combineSame(resp.ends);
	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,resp.ends);
	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);

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

	 MeiShiWuTemplate meiShiWuTemplate = GlobalVarManager::getSingleton().getMeiShiWu();
	 Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	 PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();


	 if (foodType == 1)
	 {
		 mFireConfirmData.m_Gain = meiShiWuTemplate.rate1;
	 }
	 else if (foodType == 2)
	 {
		 mFireConfirmData.m_Gain = meiShiWuTemplate.rate2;
	 }
	 else if (foodType == 3)
	 {
		 mFireConfirmData.m_Gain = meiShiWuTemplate.rate3;
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

	rapidjson::Document tmpDocument;
	
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

	UInt32 leftTimes = StageManager::getSingleton().getNormalStageLeftTimes(connId,msg.id);

	player->ResetFireConfirmData();
	FireConfirmManager::getSingleton().SetCopyStart(player,msg.id);	
	StageTemplate *stageTemplate =gStageTable->get(msg.id);
	MeiShiWuTemplate meiShiWuTemplate = GlobalVarManager::getSingleton().getMeiShiWu();
	SaoDangTemplate saoDangTemplate = GlobalVarManager::getSingleton().getSaoDang();
	PlayerFireConfirmData mFireConfirmData = player->GetFireConfirmData();

	PlayerDailyResetData playerDailyResetData = player->getPlayerDailyResetData();

	if (msg.mopUpType >3)
	{
		mopupTimes = food4;
	}
	else
	{
		mopupTimes = food1 + food2 + food3;
	}

	if (leftTimes < mopupTimes)
	{		
		resp.result = LynxErrno::TimesNotEnough;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
		return;
	}

	if (msg.mopUpType > 3)//精英关卡扫荡消耗体力
	{
		int needSubTimes = food4;
		if (saoDangTemplate.sweepfreetimes > playerDailyResetData.m_EliteMopUpTimes)
		{
			needSubTimes -= (saoDangTemplate.sweepfreetimes - playerDailyResetData.m_EliteMopUpTimes);
			if (needSubTimes <0)
			{
				needSubTimes =0;
			}
		}
		
		if (saoDangTemplate.sweeptimes < (playerDailyResetData.m_EliteMopUpTimes +food4)	)
		{
			resp.result =LynxErrno::NumberNotRight;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}
		if (FireConfirmManager::getSingleton().refreshgetStrength(connId) < (stageTemplate->mStrengthCost* needSubTimes) )
		{
			resp.result =LynxErrno::StrengthNotEnough;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}
		
		playerDailyResetData.m_EliteMopUpTimes += food4;
		goods.resourcestype =1;
		goods.subtype = 3;
		goods.num -= stageTemplate->mStrengthCost*needSubTimes;
		mFireConfirmData.m_CostList.insertTail(goods);
		player->SetFireConfirmData(mFireConfirmData);

		for (UInt32 j=0;j<food4;j++)
		{
			FireConfirmManager::getSingleton().getAwardOnce(connId,msg.id);
		}

	}

	if (msg.mopUpType <=3)//普通关卡扫荡消耗食物
	{
		if (saoDangTemplate.sweeptimes < (playerDailyResetData.m_NormalMopUpTimes + food1+ food2+ food3)	)
		{
			resp.result =LynxErrno::NumberNotRight;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}

		

		if (leftTimes < ( food1+ food2+ food3)	)
		{
			if (leftTimes <food1 )
			{
				food1 = leftTimes;
				food2 = 0;
				food3 = 0;
			}
			else if (leftTimes < ( food1+ food2))
			{
				food2 = leftTimes - food1;
				food3 = 0;
			}
			else if (leftTimes < ( food1+ food2+ food3))
			{
				food3 = leftTimes - food1 - food2;
			}			
		}
		

		CGFoods foodsmsg;
		foodsmsg.reqType =0;
		FireConfirmManager::onFoodsReq(connId ,foodsmsg);				
		PlayerFoodsData foodsData = player->getFoodsData();
		if (foodsData.food1 <food1 || foodsData.food2 <food2 || foodsData.food3 <food3 )
		{
			resp.result =LynxErrno::NumberNotRight;
			std::string jsonStr = resp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_MOP_UP_RESP,jsonStr);
			return;
		}

		foodsData.food1 -= food1;
		foodsData.food2 -= food2;
		foodsData.food3 -= food3;
		if(foodsData.beginTime ==0)
		{
			foodsData.beginTime = TimeUtil::getTimeSec();
		}
		if (foodsData.vipFoodLeftNumber > (food1+food2+food3))
		{
			foodsData.vipFoodLeftNumber -= (food1+food2+food3);
		}
		else
		{
			foodsData.vipFoodLeftNumber = 0;
		}

		playerDailyResetData.m_NormalMopUpTimes += (food1 + food2 + food3);
		player->setPlayerDailyResetData(playerDailyResetData);


		player->setFoodsData(foodsData);
		goods.resourcestype = AWARD_FOODDATA;
		goods.subtype = 0;
		goods.num = 0;
		mFireConfirmData.m_CostList.insertTail(goods);
		player->SetFireConfirmData(mFireConfirmData);

		resp.foodList.insertTail(foodsData.food1);
		resp.foodList.insertTail(foodsData.food2);
		resp.foodList.insertTail(foodsData.food3);

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
	FireConfirmManager::getSingleton().saveAndGetResult(connId,awardResult,0);

	

	resp.firstAwards = awardResult.firstAwards;	
	resp.awards = awardResult.awards;
	resp.cost = awardResult.cost;
	resp.cards = awardResult.cards;
	resp.monsterDropList = awardResult.monsterDropList;
	resp.fixedList = awardResult.fixedList;
	resp.ends = awardResult.ends;
	resp.result =LynxErrno::None;
	resp.leftTimes = saoDangTemplate.rate - (playerDailyResetData.m_NormalMopUpTimes + food1+ food2+ food3);

	resp.mopupTimes = mopupTimes;
	

	mFireConfirmData = player->GetFireConfirmData();
	resp.allAttr = mFireConfirmData.m_AddSendjs;

	std::string jsonStr = resp.convertDataToJson();	

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

}
//fanPaiType 0 是只发送免费的翻牌，1 是发送4张
void FireConfirmManager::saveAndGetResult(const  ConnId& connId,ChapterEndResp &resp,UInt32 fanPaiType = 0)
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

	resp.monsterDropList = mFireConfirmData.m_MonsterAwardList;
	GiftManager::getSingleton().combineSame(resp.monsterDropList);
	resp.ends += resp.monsterDropList;

	resp.intruderAwardList = mFireConfirmData.m_IntruderAwardList;
	GiftManager::getSingleton().combineSame(resp.intruderAwardList);
	resp.ends += resp.intruderAwardList;

	resp.fixedList = mFireConfirmData.m_FixedList;
	GiftManager::getSingleton().combineSame(resp.fixedList);
	resp.ends += resp.fixedList;
	
// 	GiftManager::getSingleton().splitKind(resp.monsterDropList, resp.fistKind,AWARD_BASE);
	

	GiftManager::getSingleton().combineSame(resp.ends);


	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,resp.ends);
	
	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);
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

static UInt32 kickCount =0;

void FireConfirmManager::onFoodsReq(const  ConnId& connId ,CGFoods &msg )	//美食屋
{

	UInt32 maxBuyTimes =0;
	double maxCanAddTimes;
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 finishTimes;
	UInt32 flag = 1;
	double lastFinishTimes =0;
	List<Goods> itemList;
	Goods goods;
	List<UInt32> foods;
	PlayerFoodsData foodsData;
	PlayerBaseData baseData;
	FoodsResp resp;
	MeiShiWuTemplate meiShiWuTemplate;

	meiShiWuTemplate = GlobalVarManager::getSingleton().getMeiShiWu();

	msg.convertJsonToData(msg.jsonStr);
	Player * player  = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if(!player)
	{
		return;
	}

	if (player->getPlayerGuid() == 1245)
	{
		SevenDayTaskReq req46;
		InlineActivityManager::onSevenDayTaskReq(connId,req46);

		InlineActivityManager::getSingleton().updateSevenDayTaskData(player->getPlayerGuid(),3002,2);

	

	}


	foodsData = player->getFoodsData();
	baseData = player->getPlayerBaseData();

	
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
		
	lastFinishTimes = (nowTime - foodsData.beginTime) / (meiShiWuTemplate.time);


	if(lastFinishTimes < 0)//有时时间异常
	{
		finishTimes = 0;
	}
	else if (lastFinishTimes > meiShiWuTemplate.recover)
	{
		finishTimes = meiShiWuTemplate.recover;
	}
	else
	{
		finishTimes = lastFinishTimes;
	}
	
	maxCanAddTimes =(double)( meiShiWuTemplate.recover + foodsData.vipFoodLeftNumber) - (foodsData.leftTimes + foodsData.food1 + foodsData.food2 + foodsData.food3);
	if (maxCanAddTimes > 0 )
	{
		if (maxCanAddTimes <= finishTimes )//次数满了
		{
			foodsData.leftTimes += maxCanAddTimes;
			foodsData.beginTime = nowTime;
		}
		else //次数没满还要倒计时
		{
			foodsData.leftTimes += finishTimes;
			foodsData.beginTime = foodsData.beginTime + finishTimes * (meiShiWuTemplate.time);			
		}			
	}
	else
	{
		foodsData.beginTime = nowTime;
	}

	resp.flag = flag;
	resp.gold = player->getPlayerGold();
	
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
			resp.needTime =  meiShiWuTemplate.time;
		}
		else
		{
			resp.needTime =  meiShiWuTemplate.time -  (nowTime - foodsData.beginTime)%(meiShiWuTemplate.time);	
		}
		
		resp.viableAmount = meiShiWuTemplate.recover;
		resp.restbuyNumber = maxBuyTimes - foodsData.vipLeftTimes;
		resp.vipFoodLeftNumber = foodsData.vipFoodLeftNumber;
		resp.foodList.insertTail(foodsData.food1);
		resp.foodList.insertTail(foodsData.food2);
		resp.foodList.insertTail(foodsData.food3);

		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,CHAPTER_FOOD_RESP,jsonStr);

		player->setFoodsData(foodsData);
		return;
	}
	else if (msg.reqType == 2)//烹饪
	{
		if (foodsData.leftTimes > 0)
		{
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
		}
		else
		{

			resp.viableNum  = foodsData.leftTimes;
			if (foodsData.beginTime ==0)
			{
				resp.needTime =  meiShiWuTemplate.time;
			}
			else
			{
				resp.needTime =  meiShiWuTemplate.time -  (nowTime - foodsData.beginTime)%(meiShiWuTemplate.time);	
			}
			resp.viableAmount = meiShiWuTemplate.recover;
			resp.restbuyNumber = maxBuyTimes - foodsData.vipLeftTimes;
			resp.vipFoodLeftNumber = foodsData.vipFoodLeftNumber;
			resp.foodList.insertTail(foodsData.food1);
			resp.foodList.insertTail(foodsData.food2);
			resp.foodList.insertTail(foodsData.food3);

			std::string jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,CHAPTER_FOOD_RESP,jsonStr);

			return;
		}		
	}
	else if (msg.reqType == 3)//购买次数
	{		
		do 
		{
			if(baseData.m_nGold < msg.num * meiShiWuTemplate.price )
			{
				flag = LynxErrno::RmbNotEnough;
				break;
			}
			if( (maxBuyTimes - foodsData.vipLeftTimes) < msg.num  )
			{
				flag = LynxErrno::TimesNotEnough;
				break;
			}
			if (( foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.leftTimes) > meiShiWuTemplate.max)
			{
				flag = LynxErrno::OverMaxTimes;
				break;
			}
			foodsData.vipLeftTimes += msg.num ;
			foodsData.vipFoodLeftNumber += msg.num * meiShiWuTemplate.eachBuy;
			goods.resourcestype = 1;
			goods.subtype = 2;
			goods.num -= msg.num * meiShiWuTemplate.price;		
			itemList.insertTail(goods);

			UInt32 tmp =0;
			UInt32 i =0;
			foods.insertTail(tmp);
			foods.insertTail(tmp);
			foods.insertTail(tmp);
			FireConfirmManager::getSingleton().getfoods( msg.num * meiShiWuTemplate.eachBuy,foods);
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
		} while (0);

		
	}
	else if (msg.reqType == 4)//开箱子奖励可用次数
	{
		if ((foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.leftTimes + msg.num  ) > meiShiWuTemplate.max)//超过最高上限
		{
			foodsData.leftTimes = meiShiWuTemplate.max - (foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.leftTimes );		
		}
		else
		{
			foodsData.leftTimes += msg.num;	
		}
		if (( foodsData.food1 + foodsData.food2 + foodsData.food3 + foodsData.leftTimes - foodsData.vipFoodLeftNumber) >= meiShiWuTemplate.recover)
		{
			foodsData.beginTime = nowTime;
		}			
	}
	else if (msg.reqType == 5)//使用食物
	{
		if (foodsData.food1 >= msg.num)
		{
			foodsData.food1 -= msg.num;
			if (foodsData.vipFoodLeftNumber >= msg.num)
			{
				foodsData.vipFoodLeftNumber -= msg.num;
			}
			else
			{			
				foodsData.vipFoodLeftNumber =0;			
			}
		}
		else 
		{
			flag = 4;
		}
	}
	else if (msg.reqType == 6)//使用食物
	{
		if (foodsData.food2 >= msg.num)
		{
			foodsData.food2 -= msg.num;
			if (foodsData.vipFoodLeftNumber >= msg.num)
			{
				foodsData.vipFoodLeftNumber -= msg.num;
			}
			else
			{			
				foodsData.vipFoodLeftNumber =0;			
			}
		}
		else 
		{
			flag = 4;
		}

	}
	else if (msg.reqType == 7)//使用食物
	{
		if (foodsData.food3 >= msg.num)
		{
			foodsData.food3 -= msg.num;
			if (foodsData.vipFoodLeftNumber >= msg.num)
			{
				foodsData.vipFoodLeftNumber -= msg.num;
			}
			else
			{			
				foodsData.vipFoodLeftNumber =0;			
			}
		}
		else 
		{
			flag = 4;
		}

	}
	else if (msg.reqType == 10)//清空美食屋
	{
		foodsData.leftTimes = 0;
		foodsData.beginTime = nowTime;
		foodsData.food1 = 0;		
		foodsData.food2 = 0;
		foodsData.food3 = 0;
		foodsData.vipLeftTimes = 0;
		foodsData.vipFoodLeftNumber = 0;
		player->setFoodsData(foodsData);
		player->getPersistManager().setDirtyBit(FOODDATABIT );
		return;
	}
	else if (msg.reqType == 11)//设置成最大值
	{
		int addNum = (int)meiShiWuTemplate.recover - (foodsData.food1 + foodsData.food2 + foodsData.food3);
		if (addNum < 0)
		{
			addNum = 0;
		}
		foodsData.leftTimes = addNum;
		foodsData.beginTime = nowTime;
		foodsData.vipLeftTimes = 0;
		foodsData.vipFoodLeftNumber = 0;
		player->setFoodsData(foodsData);
		player->getPersistManager().setDirtyBit(FOODDATABIT );
		return;
	}
	player->setFoodsData(foodsData);
	player->getPersistManager().setDirtyBit(FOODDATABIT );
	
	resp.gold = player->getPlayerGold();
	resp.flag = flag;
	resp.viableNum  = foodsData.leftTimes;
	if (foodsData.beginTime ==0)
	{
		resp.needTime =  meiShiWuTemplate.time;
	}
	else
	{
		resp.needTime =  meiShiWuTemplate.time -  (nowTime - foodsData.beginTime)%(meiShiWuTemplate.time);	
	}

	resp.viableAmount = meiShiWuTemplate.recover;
	resp.restbuyNumber = maxBuyTimes - foodsData.vipLeftTimes;
	resp.vipFoodLeftNumber = foodsData.vipFoodLeftNumber;
	resp.foodList.insertTail(foodsData.food1);
	resp.foodList.insertTail(foodsData.food2);
	resp.foodList.insertTail(foodsData.food3);

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,CHAPTER_FOOD_RESP,jsonStr);
	
}


void FireConfirmManager::getfoods(UInt32 num,List<UInt32> &foods)
{
	List<UInt32> randomNumbers;
	UInt32 allWeight = 0;
	UInt32 randNum = 0;
	UInt32 weight = 0;
	UInt32 ii;
	UInt32 jj;

	MeiShiWuTemplate meiShiWuTemplate;
	meiShiWuTemplate = GlobalVarManager::getSingleton().getMeiShiWu();


	allWeight = meiShiWuTemplate.weight1+ meiShiWuTemplate.weight2 + meiShiWuTemplate.weight3;
	randomNumbers.insertTail(meiShiWuTemplate.weight1);
	randomNumbers.insertTail(meiShiWuTemplate.weight2);
	randomNumbers.insertTail(meiShiWuTemplate.weight3);
	
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
	UInt32 level = player->getPlayerLeval();
	msg.convertJsonToData(msg.jsonStr);

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

	if (awardID == 0)
	{
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
			for(UInt32 jj =0;jj< doneTimes;jj++)
			{
				UInt32 itmp = awardID*2*100*awardChange;
 				goods.num = awardID -(awardID*awardChange - (rand()%itmp )/100 );						
			}
			goods.resourcestype =1;
			goods.subtype = 1;			
			award.insertTail(goods);
		}
		else if (msg.id == 2)
		{		
			dailyResetData.m_nDailyMiCount = finishTimes;
			for(UInt32 jj =0;jj< doneTimes;jj++)
			{
				UInt32 itmp = awardID*awardChange*2*100;
				goods.num = awardID -(awardID*awardChange - (rand()%itmp )/100 );				
			}
			goods.resourcestype =1;
			goods.subtype = 4;			
			award.insertTail(goods);
		}
		else if (msg.id == 4)
		{	
			dailyResetData.m_nDailyYanCount = finishTimes;
			for(UInt32 jj =0;jj< doneTimes;jj++)
			{
				UInt32 itmp = awardID*awardChange*2*100;
				goods.num = awardID -(awardID*awardChange - (rand()%itmp )/100 );				
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


	FireConfirmManager::getSingleton().saveAndGetResult(connId,tmpResp,0);


// 	resp.firstAwards = tmpResp.firstAwards;
	resp.awards = tmpResp.awards;
	resp.cost = tmpResp.cost;
	resp.cards = tmpResp.cards;
	resp.monsterDropList = tmpResp.monsterDropList;
	resp.fixedList = tmpResp.fixedList;
	resp.ends = tmpResp.ends;

	resp.chaiCount = dailyResetData.m_nDailyChaiCount;
	resp.miCount = dailyResetData.m_nDailyMiCount;
	resp.youCount = dailyResetData.m_nDailyYouCount;
	resp.yanCount = dailyResetData.m_nDailyYanCount;
	resp.id = msg.id;
	resp.doType = msg.doType;
 	resp.difficulty = msg.difficulty;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,WELFALE_MARKET_END_RESP,jsonStr);

	//招福集市每日任务打点
	if(msg.doType == 0 || msg.doType == 1)
	{
		player->getAchieveManager().updateDailyTaskData(DLYDOMARKETCNT, 1 );
	}
}



void FireConfirmManager::onStrengthReq(const  ConnId& connId ,CGStrength &msg )	//体力
{

	List<UInt32> foods;
	UInt32 maxBuyTimes =0;
	double maxCanAddTimes;
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 finishTimes;
	UInt32 flag = 1;
	UInt32 price = 0;
	UInt32 num = 0;
	UInt32 nowBuyTimes = 0;
	double lastFinishTimes =0;
	List<Goods> itemList;
	Goods goods;
	PlayerStrengthData strengthData;
	PlayerBaseData baseData;
	StrengthResp resp;
	StrengthTemplate strengthTemplate;

	msg.convertJsonToData(msg.jsonStr);
	Player * player  = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if(!player)
	{
		return;
	}
	PlayerExpTemplate *expTemplate = gPlayerExpTable->get(player->getPlayerLeval());
	strengthTemplate = GlobalVarManager::getSingleton().getStrength();

	strengthData = player->getStrengthData();
	baseData = player->getPlayerBaseData();


	maxBuyTimes = ShopManager::getSingleton().getStrengthBuyMaxTimes(player->getVipLevel());

	if(TimeManager::timeIsToday(strengthData.buyTime - 5) ==false)
	{
		strengthData.buyTime = nowTime;
		strengthData.vipLeftTimes = 0;
	}

	lastFinishTimes = (nowTime - strengthData.beginTime) / (strengthTemplate.time);


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

	maxCanAddTimes =(double) expTemplate->mStrength -  strengthData.strength;
	if (maxCanAddTimes > 0 )
	{
		if (maxCanAddTimes <= finishTimes )//次数满了
		{
			strengthData.strength += maxCanAddTimes;
			strengthData.beginTime = nowTime;
		}
		else //次数没满还要倒计时
		{
			strengthData.strength += finishTimes;
			strengthData.beginTime = strengthData.beginTime + finishTimes * (strengthTemplate.time);			
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
			resp.needTime =  strengthTemplate.time;
		}
		else
		{
			resp.needTime =  strengthTemplate.time -  (nowTime - strengthData.beginTime)%(strengthTemplate.time);	
		}

		resp.viableAmount = expTemplate->mStrength;
		resp.restbuyNumber = strengthData.vipLeftTimes;
		resp.vipStrengthLeftNumber = maxBuyTimes;
		resp.strength = strengthData.strength;
		

		if (msg.needSend == true)
		{
			std::string jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,CHAPTER_STRENGTH_RESP,jsonStr);
		}	

		player->setStrengthData(strengthData);
		return;
	}
	
	else if (msg.reqType == 3)//购买次数
	{	
		if (msg.num != 1)
		{
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
			if (strengthData.strength  > strengthTemplate.max)
			{
				flag = LynxErrno::OverMaxTimes;
				break;
			}
			strengthData.vipLeftTimes += msg.num ;
			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num -= msg.num * price;		
			itemList.insertTail(goods);
			strengthData.strength += msg.num * num;
			strengthData.buyTime = nowTime;
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);

		} while (0);
	}
	else if (msg.reqType == 8)//增加 todo 规则不明确
	{		

		if((strengthData.strength +  msg.num) > strengthTemplate.max)
		{
			strengthData.strength += strengthTemplate.max - strengthData.strength;				
		}
		else
		{
			strengthData.strength += msg.num;				
		}

	}
	else if (msg.reqType == 9)//减少 //时间前面已经重置
	{				
		if(strengthData.strength  < msg.num)
		{
			strengthData.strength = 0;					
		}
		else
		{
			strengthData.strength -= msg.num;

		}
	}
	else if (msg.reqType == 10)//清空美食屋
	{
		strengthData.leftTimes = 0;
		strengthData.beginTime = nowTime;
		strengthData.strength = 0;		
		strengthData.vipLeftTimes = 0;
		strengthData.vipFoodLeftNumber = 0;
		player->setStrengthData(strengthData);
		player->getPersistManager().setDirtyBit(STRENGTHDATABIT );
		return;
	}
	else if (msg.reqType == 11)//设置成最大值
	{
		
		strengthData.strength = expTemplate->mStrength;
		strengthData.beginTime = nowTime;
		strengthData.vipLeftTimes = 0;
		strengthData.vipFoodLeftNumber = 0;
		player->setStrengthData(strengthData);
		player->getPersistManager().setDirtyBit(STRENGTHDATABIT );
		return;
	}

	player->setStrengthData(strengthData);
	player->getPersistManager().setDirtyBit(STRENGTHDATABIT);
	

	resp.gold = player->getPlayerGold();
	resp.flag = flag;
	if (strengthData.beginTime ==0)
	{
		resp.needTime =  strengthTemplate.time;
	}
	else
	{
		resp.needTime =  strengthTemplate.time -  (nowTime - strengthData.beginTime)%(strengthTemplate.time);	
	}

	resp.viableAmount = expTemplate->mStrength;
	resp.restbuyNumber = strengthData.vipLeftTimes;
	resp.vipStrengthLeftNumber = maxBuyTimes;
	resp.strength = strengthData.strength;

	if (msg.needSend == true)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,CHAPTER_STRENGTH_RESP,jsonStr);
	}

	if(msg.reqType == 3)
	{
		//每日任务购买体力打点 wwc
		player->getAchieveManager().updateDailyTaskData(DLYBUYENERGE, 1 );
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