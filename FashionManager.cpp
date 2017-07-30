#include "FashionManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
#include "FireConfirm/Gift.h"


using namespace Lynx;


FashionManager::FashionManager()
{
	m_pPlayer = NULL;
	m_pListFashionDatas = NULL;

}

FashionManager:: ~FashionManager()
{

}

	
bool FashionManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pListFashionDatas = &player->mPlayerData.mFashionData.m_listFashionDatas;

	for(List<FashionData>::Iter * fashionIter = m_pListFashionDatas->begin(); fashionIter != NULL; 
		fashionIter = m_pListFashionDatas->next(fashionIter))
	{
		if( fashionIter->mValue.m_nFahionID)
		{
			m_mapIdToFsData.insert(fashionIter->mValue.m_nFahionID, &fashionIter->mValue);
		}
		
	}

	return true;
}

void FashionManager::release()
{
	m_pPlayer = NULL;
	m_pListFashionDatas = NULL;

	m_mapIdToFsData.clear();
}


std::string FashionManager::convertFashionDataToJson()
{
	Json::Value root;
	Json::Value buyNode;

	for(Map<UInt64, FashionData*>::Iter * iter = m_mapIdToFsData.begin(); iter != NULL; 
		iter = m_mapIdToFsData.next(iter))
	{
		Json::Value fashionNode;
		fashionNode["id"] = iter->mKey;

		buyNode.append(fashionNode);

	}

	root["haveBuyList"] = buyNode;

	Json::StyledWriter writer;
	std::string strWrite = writer.write(root);
	//cout << strWrite;
	return strWrite;

}


void FashionManager::buyFashion(UInt64 fashionId)
{
	const ConnId & connId = m_pPlayer->getConnId();
	
	Map<UInt64, FashionData* >::Iter * findIter = m_mapIdToFsData.find(fashionId);
	if(findIter)
	{
		//提示已经购买过此时装
		GCFashionBuyResp fashionBuyResp;
		fashionBuyResp.mPacketID = BOC_PLAYER_FASHION_BUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::FashionHasExit;
		Json::StyledWriter writer;
		
		fashionBuyResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(fashionBuyResp, connId);

		return;
	}

	FahionTemplate* fashionTemp = FASHION_TABLE().get(fashionId);
	if(!fashionTemp)
	{
		//提示id错误
		GCFashionBuyResp fashionBuyResp;
		fashionBuyResp.mPacketID = BOC_PLAYER_FASHION_BUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		fashionBuyResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(fashionBuyResp, connId);
		return;
	}

	UInt32 vipLv = m_pPlayer->getVipLevel();
	if(vipLv < fashionTemp->mLimitVip)
	{
		//提示id错误
		GCFashionBuyResp fashionBuyResp;
		fashionBuyResp.mPacketID = BOC_PLAYER_FASHION_BUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::VipLevelNotEnough;
		Json::StyledWriter writer;

		fashionBuyResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(fashionBuyResp, connId);
	}

	UInt64 gold = m_pPlayer->getPlayerGold();
	if(gold < fashionTemp->mLimitRMB)
	{
		//提示元宝不足
		GCFashionBuyResp fashionBuyResp;
		fashionBuyResp.mPacketID = BOC_PLAYER_FASHION_BUY_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::RmbNotEnough;
		Json::StyledWriter writer;

		fashionBuyResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(fashionBuyResp, connId);
	}

	//扣除指定元宝


	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_GOLD;
	goods.num = 0 - fashionTemp->mLimitRMB;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog10);

	addFashion(fashionId, connId);

	GCFashionBuyResp fashionBuyResp;
	fashionBuyResp.mPacketID = BOC_PLAYER_FASHION_BUY_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["fashionId"] = fashionId;
	root["gold"] = m_pPlayer->getPlayerGold();

	Json::StyledWriter writer;

	fashionBuyResp.mRespJsonStr = writer.write(root);
	cout << fashionBuyResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(fashionBuyResp, connId);

	return;
}

void FashionManager::addFashion(UInt64 fashionId, const ConnId &connId)
{
	FashionData fashionData;
	fashionData.m_nFahionID = fashionId;
	

	List<FashionData>::Iter * insertIter = m_pListFashionDatas->insertTail(fashionData);
	m_mapIdToFsData.insert(fashionId, &insertIter->mValue);

	FashionAdd fashionAdd;
	fashionAdd.mFashionId = fashionId;
	
	fashionAdd.mPlayerUid = m_pPlayer->getPlayerGuid();

	PersistSystem::getSingleton().postThreadMsg(fashionAdd, connId);
}


