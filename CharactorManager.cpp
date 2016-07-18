#include "CharactorManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
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
	cout << strWrite;
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
	m_pPlayer->setPlayerGold(currentGold-heroTemp->mCost);

	//获得角色，角色会穿戴默认时装，所以时装表需要添加对应时装

	HeroFashionTemplate * heroFashionTemp = HEROFASHION_TABLE().get(charactorId);
	m_pPlayer->getFashionManager().addFashion(heroFashionTemp->mFashionId, connId);


	//将角色加入内存数据结构
	UInt64 charactorUid = LogicSystem::getSingleton().generateCharactorGuid();
	CharactorData charactorData;
	charactorData.m_nCharactorId = charactorId;
	charactorData.m_nEquipFashion = heroFashionTemp->mFashionId;
	charactorData.m_nCharactorUid = charactorUid;

	List<CharactorData>::Iter * insertIter = m_pCharactorList->insertTail(charactorData);
	m_mapCharactor.insert(charactorId, &(insertIter->mValue));
	//添加角色到角色表
	
	CharactorAdd charactorAdd;
	charactorAdd.charactorId = charactorId;
	charactorAdd.charactorUid = charactorUid;
	charactorAdd.fashionId = heroFashionTemp->mFashionId;
	charactorAdd.playerUid = m_pPlayer->getPlayerGuid();
	PersistSystem::getSingleton().postThreadMsg(charactorAdd, charactorAdd.playerUid);

	m_pPlayer ->getAchieveManager().updateAchieveData(BUYCHARACTOR,1);

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
	cout <<buyResp.mRespJsonStr;
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

	CharactorData* pCharactorData = findIter->mValue;
	pCharactorData->m_nEquipFashion = fashionId;

	CharactorUpdate charactorUpdate;
	charactorUpdate.charactorUid = pCharactorData->m_nCharactorUid;
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

	m_pPlayer->setPlayerModelID(charactor);
	m_pPlayer->getSkillManager().changeCharactorSkill(charactor);
	UInt64 weapon = m_pPlayer->getHeroEquipManager().changeCharacterEquip(charactor);


	GCCharactorSwitchResp switchResp;
	switchResp.mPacketID = BOC_CHARACTORSWITCH_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	  
	root["modelid"] = charactor;
	root["equipfashion"] = findIter->mValue->m_nEquipFashion;
	root["weapon"] = weapon;


	Json::StyledWriter writer;
	switchResp.mRespJsonStr = writer.write(root);
	//cout << switchResp.mRespJsonStr; 
	NetworkSystem::getSingleton().sendMsg(switchResp, connId);
}






