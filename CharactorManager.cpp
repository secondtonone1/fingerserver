#include "CharactorManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
#include "FireConfirm/Gift.h"
using namespace Lynx;


CharactorManager::CharactorManager()
{
	m_pPlayer = NULL;
	

}

CharactorManager:: ~CharactorManager()
{

}


bool CharactorManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pCharactorList = &(player->mPlayerData.mPlayerCharactorData.charactorList);

	for(List<CharactorData>::Iter * charactorIter = m_pCharactorList->begin();  charactorIter != NULL;
		charactorIter = m_pCharactorList->next(charactorIter))
	{
		m_mapCharactor.insert(charactorIter->mValue.m_nCharactorId, &(charactorIter->mValue));
	}

	return true;
}

void CharactorManager::release()
{
	m_pPlayer = NULL;
	m_pCharactorList = NULL;
	m_mapCharactor.clear();
	
}


std::string CharactorManager::convertCharactorDataToJson()
{
	Json::Value root;
	Json::Value buyNode;

	for(List<CharactorData>::Iter * charactorIter = m_pCharactorList->begin();  charactorIter != NULL;
		charactorIter = m_pCharactorList->next(charactorIter))
	{
			Json::Value charactorRoot;
			charactorRoot["modelid"] = charactorIter->mValue.m_nCharactorId;
			charactorRoot["equipfashion"] = charactorIter->mValue.m_nEquipFashion;
			root.append(charactorRoot);
	}


	Json::StyledWriter writer;
	std::string strWrite = writer.write(root);
	LOG_INFO("convertCharactorDataToJson = %s",strWrite.c_str());
	return strWrite;

}

void CharactorManager::buyCharactor(UInt64 charactorId)
{
	Map<UInt64, CharactorData*>::Iter * find = m_mapCharactor.find(charactorId);
	const ConnId & connId = m_pPlayer->getConnId();
	if(find)
	{
		GCCharactorBuyResp buyResp;
		buyResp.mPacketID = BOC_CHARACTORBUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::CharactorExits;
		Json::StyledWriter writer;
		
		buyResp.mRespJsonStr = writer.write(root);
		cout <<buyResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(buyResp, connId	);
		return;
	}

	HeroTemplate * heroTemp = HERO_TABLE().get(charactorId);
	if (heroTemp == NULL)
	{
		LOG_WARN("heroTemp not found!!");
		GCCharactorBuyResp buyResp;
		buyResp.mPacketID = BOC_CHARACTORBUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		Json::StyledWriter writer;

		buyResp.mRespJsonStr = writer.write(root);
		cout <<buyResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(buyResp, connId	);
		return;
	}
	UInt64 currentGold = m_pPlayer->getPlayerGold();
	UInt32 currentLv = m_pPlayer->getPlayerLeval();
	UInt32 curVipLv = m_pPlayer->getVipLevel();

	if(currentLv < heroTemp->mNeedLv || curVipLv < heroTemp->mNeedVipLv)
	{
		GCCharactorBuyResp buyResp;
		buyResp.mPacketID = BOC_CHARACTORBUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		buyResp.mRespJsonStr = writer.write(root);
		cout <<buyResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(buyResp, connId	);
		return;
	}

	if(currentGold < heroTemp->mCost)
	{
		GCCharactorBuyResp buyResp;
		buyResp.mPacketID = BOC_CHARACTORBUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::RmbNotEnough;
		Json::StyledWriter writer;

		buyResp.mRespJsonStr = writer.write(root);
		cout <<buyResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(buyResp, connId	);
		return;
	}

	//扣钱

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_GOLD;
	goods.num = 0 - heroTemp->mCost;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog11);

	//获得角色，角色会穿戴默认时装，所以时装表需要添加对应时装

	HeroFashionTemplate * heroFashionTemp = HEROFASHION_TABLE().get(charactorId);
	if (heroFashionTemp == NULL)
	{
		LOG_WARN("player not found!!");
		GCCharactorBuyResp buyResp;
		buyResp.mPacketID = BOC_CHARACTORBUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		Json::StyledWriter writer;

		buyResp.mRespJsonStr = writer.write(root);
		cout <<buyResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(buyResp, connId	);
		return;
	}
	m_pPlayer->getFashionManager().addFashion(heroFashionTemp->mFashionId, connId);


	//将角色加入内存数据结构
	UInt64 charactorUid = LogicSystem::getSingleton().generateCharactorGuid();
	CharactorData charactorData;
	charactorData.m_nCharactorId = charactorId;
	charactorData.m_nEquipFashion = heroFashionTemp->mFashionId;
	

	List<CharactorData>::Iter * insertIter = m_pCharactorList->insertTail(charactorData);
	m_mapCharactor.insert(charactorId, &(insertIter->mValue));
	//添加角色到角色表
	
	CharactorAdd charactorAdd;
	charactorAdd.charactorId = charactorId;
	
	charactorAdd.fashionId = heroFashionTemp->mFashionId;
	charactorAdd.playerUid = m_pPlayer->getPlayerGuid();
	PersistSystem::getSingleton().postThreadMsg(charactorAdd, charactorAdd.playerUid);

	m_pPlayer ->getAchieveManager().updateAchieveData(BUYCHARACTOR,1);

	//购买角色公告

	LogicSystem::getSingleton().sendSystemMsg(9, m_pPlayer->getPlayerName(), charactorId);
	

	//回复正确包
	
	GCCharactorBuyResp buyResp;
	buyResp.mPacketID = BOC_CHARACTORBUY_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["gold"] = currentGold - heroTemp->mCost;
	root["modelid"] = charactorId;
	root["equipfashion"] = heroFashionTemp->mFashionId;
	root["fashion"] = heroFashionTemp->mFashionId;
	Json::StyledWriter writer;

	buyResp.mRespJsonStr = writer.write(root);
//	cout <<buyResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(buyResp, connId	);
	return;


}

void CharactorManager::equipFashion(UInt32 modelId, UInt64 fashionId)
{
	UInt32 currentId = m_pPlayer->getPlayerModelID();
	const ConnId & connId = m_pPlayer->getConnId();
	if(currentId != modelId)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::IsNotCurCharactor;
		Json::StyledWriter writer;
		GCFashionEquipResp equipResp;
		equipResp.mPacketID = BOC_PLAYER_FASHION_EQUIP_RESP;
		equipResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(equipResp, connId);

		return ;
		
	}

	Map<UInt64, CharactorData*>::Iter * findIter = m_mapCharactor.find(modelId);

	if (findIter == NULL)
	{
		LOG_ERROR("Failed not found findIter modelId = %d",modelId);
		return;
	}
	CharactorData* pCharactorData = findIter->mValue;
	if (pCharactorData == NULL)
	{
		LOG_ERROR("Failed not found pCharactorData ");
		return;
	}
	UInt32 oldCount = pCharactorData->m_nEquipFashion;

	pCharactorData->m_nEquipFashion = fashionId;

	UInt32 nowCount = fashionId;
	char dest[1024]={0};
	nowCount = pCharactorData->m_nEquipFashion;
	snprintf(dest,sizeof(dest),"%d,%d",oldCount,nowCount);
	LogicSystem::getSingleton().write_log(LogType87,m_pPlayer->getPlayerGuid(), dest,LogInfo);

	CharactorUpdate charactorUpdate;
	
	charactorUpdate.fashionId = pCharactorData->m_nEquipFashion;

	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	PersistSystem::getSingleton().postThreadMsg(charactorUpdate,  playerUid);

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["modelid"] = modelId;
	root["equipfashion"] = fashionId;

	Json::StyledWriter writer;
	GCFashionEquipResp equipResp;
	equipResp.mPacketID = BOC_PLAYER_FASHION_EQUIP_RESP;
	equipResp.mRespJsonStr = writer.write(root);

	NetworkSystem::getSingleton().sendMsg(equipResp, connId);
}


void CharactorManager::changeCharactor(UInt64 charactor)
{
	Map<UInt64, CharactorData*>::Iter * findIter = m_mapCharactor.find(charactor);
	const ConnId & connId = m_pPlayer->getConnId();

	if(!findIter)
	{
		GCCharactorSwitchResp switchResp;
		switchResp.mPacketID = BOC_CHARACTORSWITCH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::CharactorNotExits;
		
		Json::StyledWriter writer;
		switchResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(switchResp, connId);

		return;
	}
	UInt32 oldCount = m_pPlayer->getPlayerModelID();

	m_pPlayer->setPlayerModelID(charactor);
	m_pPlayer->getSkillManager().changeCharactorSkill(charactor);
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT|SKILLDATABIT|HEROEQUIPDATABIT);
	UInt64 weapon = m_pPlayer->getHeroEquipManager().changeCharacterEquip(charactor);


	UInt32 nowCount = m_pPlayer->getPlayerModelID();
	char dest[1024]={0};
	nowCount = m_pPlayer->getPlayerModelID();
	snprintf(dest,sizeof(dest),"%d,%d",oldCount,nowCount);
	LogicSystem::getSingleton().write_log(LogType88,m_pPlayer->getPlayerGuid(), dest,LogInfo);


	GCCharactorSwitchResp switchResp;
	switchResp.mPacketID = BOC_CHARACTORSWITCH_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	  
	root["modelid"] = charactor;
	root["equipfashion"] = findIter->mValue->m_nEquipFashion;
	root["weapon"] = weapon;

	Map<UInt64, SkillData*> * skillmap = m_pPlayer->getSkillManager().getSkillMap();
	for(Map<UInt64, SkillData*>::Iter * iter = skillmap->begin(); iter != NULL; iter = skillmap->next(iter))
	{
			Json::Value skillNode;
				skillNode["id"] = Json::Value(iter->mValue->m_nID);
				skillNode["cd"] = Json::Value(iter->mValue->m_nCD);
				skillNode["level"] = Json::Value(iter->mValue->m_nLevel);
				skillNode["equip"] = Json::Value(iter->mValue->m_nEquipPos);

				root["skilllist"].append(skillNode);
				
	}



	Json::StyledWriter writer;
	switchResp.mRespJsonStr = writer.write(root);
	//cout << switchResp.mRespJsonStr; 
	NetworkSystem::getSingleton().sendMsg(switchResp, connId);
}

void CharactorManager::syncCharactor(UInt64 charactor)
{
	Map<UInt64, CharactorData*>::Iter * findIter = m_mapCharactor.find(charactor);
	const ConnId & connId = m_pPlayer->getConnId();

	if(!findIter)
	{
		
		return;
	}

	m_pPlayer->setPlayerModelID(charactor);
	m_pPlayer->getSkillManager().changeCharactorSkill(charactor);
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT|SKILLDATABIT|HEROEQUIPDATABIT);
	UInt64 weapon = m_pPlayer->getHeroEquipManager().changeCharacterEquip(charactor);


	
}











