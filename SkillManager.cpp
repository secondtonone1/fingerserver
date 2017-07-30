#include "SkillManager.h"
#include "Player.h"
#include "Table.h"
#include "GameServer.h"

using namespace Lynx;
SkillManager::SkillManager()
{
	m_pPlayer = NULL;
	m_pListSkillDatas = NULL;
}	


SkillManager:: ~SkillManager()
{
	
}

bool SkillManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pListSkillDatas = &m_pPlayer->mPlayerData.mSkillListData.m_listSkills;
	
	skillMapInitial();

	return true;
	
}

void SkillManager::release()
{
	//指针置空，map 清空
	m_mapIDToSkillData.clear();
	m_pVecSkillDatas.clear();
	m_pPlayer = NULL;
	m_pListSkillDatas = NULL;
}

void SkillManager::skillLevelUp(UInt64 skillId)
{
	cout << "receive skilllevel up msg!";
	Map<UInt64, SkillData*>::Iter * iterSkill = m_mapIDToSkillData.find(skillId);
	
	if(iterSkill)
	{
		
		SkillLevelTemplate * skillNextLvTemplate = SKILLLEVEL_TABLE().reverseGetNextLv(skillId,iterSkill->mValue->m_nLevel);
		 
		if(skillNextLvTemplate)
		{
			UInt64 playerCoin = m_pPlayer->getPlayerCoin();

			if(playerCoin < skillNextLvTemplate->mLearnCoin)
			{
				GCSkillLevelUpResp skillLevelUpResp;
				skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

				Json::Value root;
				Json::FastWriter writer;

				root["errorId"] = LynxErrno::CoinNotEnough;
				
				skillLevelUpResp.mRespJsonStr = writer.write(root);

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

				return;
			}
			
			if(m_pPlayer->mPlayerData.mBaseData.m_nLevel < skillNextLvTemplate->mLearnLevel)
			{
				GCSkillLevelUpResp skillLevelUpResp;
				skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

				Json::Value root;
				Json::FastWriter writer;

				root["errorId"] = LynxErrno::LevelNotEnough;

				skillLevelUpResp.mRespJsonStr = writer.write(root);

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

				return;
			}

			playerCoin -= skillNextLvTemplate->mLearnCoin;


			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - skillNextLvTemplate->mLearnCoin;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog15);
			
			iterSkill->mValue->m_nLevel = skillNextLvTemplate->mLevel;


			m_pPlayer->getPersistManager().setDirtyBit(SKILLDATABIT);

			//
			GCSkillLevelUpResp skillLevelUpResp;
			skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;
			
			Json::Value root;
			Json::FastWriter writer;
			
			root["errorId"] = LynxErrno::None;
			root["skillID"] = iterSkill->mValue->m_nID;
			root["skillLevel"] = iterSkill->mValue->m_nLevel;
			root["coin"] = playerCoin;
			skillLevelUpResp.mRespJsonStr = writer.write(root);

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

			//更新七日训
			LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT04,1);

			return;
			
		}
		else
		{
			//提示玩家已经满级

			GCSkillLevelUpResp skillLevelUpResp;
			skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

			Json::Value root;
			Json::FastWriter writer;

			root["errorId"] = LynxErrno::SkillLevelLimit;
			root["skillID"] = iterSkill->mValue->m_nID;
			root["skillLevel"] = iterSkill->mValue->m_nLevel;
			skillLevelUpResp.mRespJsonStr = writer.write(root);

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

			return;
		}
	}
	else
	{
		//提示玩家技能id无效

		GCSkillLevelUpResp skillLevelUpResp;
		skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

		Json::Value root;
		Json::FastWriter writer;

		root["errorId"] = LynxErrno::InvalidParameter;
		root["skillID"] = 0;
		root["skillLevel"] = 0;
		skillLevelUpResp.mRespJsonStr = writer.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

		return;
	}
}

void SkillManager::skillLvUpOnce(UInt64 skillId)
{

	cout << "receive skilllevel up msg!";
	Map<UInt64, SkillData*>::Iter * iterSkill = m_mapIDToSkillData.find(skillId);
	
	if(iterSkill)
	{
		
		SkillLevelTemplate * skillNextLvTemplate = SKILLLEVEL_TABLE().reverseGetNextLv(skillId,iterSkill->mValue->m_nLevel);
		 
		if(skillNextLvTemplate)
		{
			UInt64 playerCoin = m_pPlayer->getPlayerCoin();

			if(playerCoin < skillNextLvTemplate->mLearnCoin)
			{
				GCSkillLevelUpResp skillLevelUpResp;
				skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

				Json::Value root;
				Json::FastWriter writer;

				root["errorId"] = LynxErrno::CoinNotEnough;
				
				skillLevelUpResp.mRespJsonStr = writer.write(root);

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

				return;
			}
			
			if(m_pPlayer->mPlayerData.mBaseData.m_nLevel < skillNextLvTemplate->mLearnLevel)
			{
				GCSkillLevelUpResp skillLevelUpResp;
				skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

				Json::Value root;
				Json::FastWriter writer;

				root["errorId"] = LynxErrno::LevelNotEnough;

				skillLevelUpResp.mRespJsonStr = writer.write(root);

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

				return;
			}

			UInt32 cost =0;
			cost += skillNextLvTemplate->mLearnCoin;
			playerCoin -= skillNextLvTemplate->mLearnCoin;

			//更新七日训
			LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT04,1);

			iterSkill->mValue->m_nLevel = skillNextLvTemplate->mLevel;

            skillNextLvTemplate = SKILLLEVEL_TABLE().reverseGetNextLv(skillId,iterSkill->mValue->m_nLevel);

			while( skillNextLvTemplate && playerCoin >= skillNextLvTemplate->mLearnCoin && 
				m_pPlayer->mPlayerData.mBaseData.m_nLevel >= skillNextLvTemplate->mLearnLevel)
			{
					playerCoin -= skillNextLvTemplate->mLearnCoin;
					cost += skillNextLvTemplate->mLearnCoin;

					iterSkill->mValue->m_nLevel = skillNextLvTemplate->mLevel;
					//更新七日训
					LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT04,1);

					skillNextLvTemplate = SKILLLEVEL_TABLE().reverseGetNextLv(skillId,iterSkill->mValue->m_nLevel);
			}


			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - cost;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog145);

			m_pPlayer->getPersistManager().setDirtyBit(SKILLDATABIT);

			//
			GCSkillLevelUpResp skillLevelUpResp;
			skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;
			
			Json::Value root;
			Json::FastWriter writer;
			
			root["errorId"] = LynxErrno::None;
			root["skillID"] = iterSkill->mValue->m_nID;
			root["skillLevel"] = iterSkill->mValue->m_nLevel;
			root["coin"] = playerCoin;
			skillLevelUpResp.mRespJsonStr = writer.write(root);

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

		

			return;
			
		}
		else
		{
			//提示玩家已经满级

			GCSkillLevelUpResp skillLevelUpResp;
			skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

			Json::Value root;
			Json::FastWriter writer;

			root["errorId"] = LynxErrno::SkillLevelLimit;
			root["skillID"] = iterSkill->mValue->m_nID;
			root["skillLevel"] = iterSkill->mValue->m_nLevel;
			skillLevelUpResp.mRespJsonStr = writer.write(root);

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

			return;
		}
	}
	else
	{
		//提示玩家技能id无效

		GCSkillLevelUpResp skillLevelUpResp;
		skillLevelUpResp.mPacketID = BOC_SKILL_LEVELUP_RESP;

		Json::Value root;
		Json::FastWriter writer;

		root["errorId"] = LynxErrno::InvalidParameter;
		root["skillID"] = 0;
		root["skillLevel"] = 0;
		skillLevelUpResp.mRespJsonStr = writer.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(skillLevelUpResp,connId);

		return;
	}
}


void SkillManager::skillPositonSet(Map<UInt64,UInt32> skillEquipMap)
{

	char dest[1024]={0};

	for(List<SkillData>::Iter * iter = m_pListSkillDatas->begin(); iter != NULL; iter = m_pListSkillDatas->next(iter) )
	{
		iter->mValue.m_nEquipPos = 0;
		Map<UInt64,UInt32>::Iter * skillEquipIter = skillEquipMap.find(iter->mValue.m_nID);
		
		if(skillEquipIter)
		{
			//在更新列表里，替换位置信息
			iter->mValue.m_nEquipPos = skillEquipIter->mValue;

			char tmp[64] = {0};
			snprintf(tmp,sizeof(tmp),"%d,",skillEquipIter->mValue);
			strcat(dest,tmp);
			
		}
		else
		{
			
		}
	}

	LogicSystem::getSingleton().write_log(LogType96,m_pPlayer->getPlayerGuid(), dest,LogInfo);

	m_pPlayer->getPersistManager().setDirtyBit(SKILLDATABIT);

	GCSkillListResp skillListResp;
	skillListResp.mPacketID = BOC_SKILL_LIST_RESP;
	skillListResp.mRespJsonStr = convertSkillDataToJson();
	const ConnId& connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(skillListResp, connId);

	
}

List<SkillData> * SkillManager::getSkillList(void)
{
	return m_pListSkillDatas;
}

Vector<SkillData *> &SkillManager::getSkillVec(void)
{
	return m_pVecSkillDatas;
}

void SkillManager::skillMapInitial()
{
	m_mapIDToSkillData.clear();
	m_pVecSkillDatas.clear();

	for(List<SkillData>::Iter * iter = m_pListSkillDatas->begin(); iter != NULL; iter = m_pListSkillDatas->next(iter) )
	{
		SkillData & skillData = iter->mValue;
		
		if(skillData.m_nID)
		{
			m_mapIDToSkillData.insert(skillData.m_nID,&iter->mValue);
		}

		m_pVecSkillDatas.push_back(&iter->mValue);
	}

	
}

std::string SkillManager::convertSkillDataToJson()
{
	MsgSkillDataToJson skillDataToJson;
	skillDataToJson.m_mapIDToSkillData = m_mapIDToSkillData;
	return skillDataToJson.convertDataToJson();
}


List<SkillData> SkillManager::activeSkill(void)
{
	List<SkillData> skillList;
	//循环遍历，找出所有等级为0的技能
	for(List<SkillData>::Iter * iter = m_pListSkillDatas->begin(); iter != NULL; iter = m_pListSkillDatas->next(iter))
	{
		if(iter->mValue.m_nLevel == 0 && iter->mValue.m_nID)
		{
			SkillLevelTemplate * skillTemp = SKILLLEVEL_TABLE().reverseGet(iter->mValue.m_nID,1);
			//判断技能学习等级是否和玩家等级相等
			if(skillTemp->mLearnLevel <= m_pPlayer->mPlayerData.mBaseData.m_nLevel)
			{
				//技能激活,等级为1
				iter->mValue.m_nLevel = 1;

				skillList.insertTail(iter->mValue);
			}
		}
	}

	m_pPlayer->getPersistManager().setDirtyBit(SKILLDATABIT);
	return skillList;
}

Map<UInt64, SkillData*> *SkillManager::getSkillMap()
{
	return &m_mapIDToSkillData;
}
		


void SkillManager::changeCharactorSkill(UInt64 charactorId)
{
	for(List<SkillData>::Iter * iter = m_pListSkillDatas->begin(); iter != NULL; iter = m_pListSkillDatas->next(iter))
	{
		if(iter->mValue.m_nLevel == 0 && !(iter->mValue.m_nID) )
		{
			continue;
			
		}

		//UInt64 tail = (iter->mValue.m_nID)%1000;
		//UInt64 newSkill = charactorId * 1000 + tail;
		SkillListTemplate * skillListtmep = SKILLCONVERT_TABLE().get(iter->mValue.m_nID);
		if(!skillListtmep)
		{
			continue;
		}

		List<UInt32>::Iter * skillIter = skillListtmep->skillList.getn(charactorId);
		if( !skillIter )
		{
			continue;
		}
		//iter->mValue.m_nID = newSkill;
		iter->mValue.m_nID = skillIter->mValue;
		//LOG_INFO("skillid is: %llu",iter->mValue.m_nID);
	}

	skillMapInitial();
	m_pPlayer->getPersistManager().setDirtyBit(SKILLDATABIT);
}