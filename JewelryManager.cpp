#include "JewelryManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
#include "FireConfirm/Gift.h"
using namespace Lynx;


JewelryManager::JewelryManager()
{
	m_pPlayer = NULL;
	m_pJewelryList = NULL;
	m_pJewelryEquipData = NULL;
}

JewelryManager:: ~JewelryManager()
{

}


bool JewelryManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;
	m_pJewelryList = &m_pPlayer->mPlayerData.mJewelryData.mJewelryList;
	
	for(List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->begin(); jewelryDataIter != NULL; 
		jewelryDataIter = m_pJewelryList->next(jewelryDataIter))
	{
		m_jewelryDataMap.insert(jewelryDataIter->mValue.m_nJewelryUid, &jewelryDataIter->mValue);
	}

	m_nPlayerGuid = m_pPlayer->getPlayerGuid();

	m_pJewelryEquipData = & m_pPlayer->mPlayerData.mJewelryEquipData;

	return true;
}

void JewelryManager::release()
{
	m_pPlayer = NULL;
	m_pJewelryList = NULL;
	m_pJewelryEquipData = NULL;
	m_jewelryDataMap.clear();

}

void JewelryManager::postUpdateMsg(JewelryData * jewelryData, UInt64 playerUid)
{
	PersistUpdateJewelryNotify updateJewelryNotify;
	updateJewelryNotify.m_jewelryData = *jewelryData;
	updateJewelryNotify.m_nPlayerUid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(updateJewelryNotify, playerUid);
}

void JewelryManager::postAddMsg(JewelryData * jewelryData, UInt64 playerUid)
{
	PersistAddJewelryNotify addJewelryNotify;
	addJewelryNotify.m_jewelryData = *jewelryData;
	addJewelryNotify.m_nPlayerUid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(addJewelryNotify, playerUid);
}

void JewelryManager::postDelMsg(UInt64 jewelryUid, UInt64 playerUid)
{
	PersistDelJewelryNotify delJewelryNotify;
	delJewelryNotify.m_nJewelryUid = jewelryUid;
	delJewelryNotify.m_nPlayerUid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(delJewelryNotify, playerUid);
}

//1~5之间随机数，产生值范围2~5
UInt32 JewelryManager::getRandomValue(UInt32 minValue, UInt32 maxValue)
{
	UInt32 diffValue = maxValue - minValue;

	if (diffValue <= 0)//todo: 0 的bug
	{
		return minValue;
	}

	UInt32 result = rand()%diffValue;

	return minValue + result + 1;

}

List<JewelryData *> JewelryManager::playerGetJewelry(const UInt64 &jewelryId,const UInt32 & count)
{
	NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryId);
	
	List<JewelryData *> newJewelryDatas;

	if(!newJewelryTemp)
	{
		//参数不对，直接返回
		return newJewelryDatas;
	}
	
	

	for(UInt32 i = 0; i < count; i++)
	{
		JewelryData * jewelryData = addJewelrys(jewelryId);
		
		postAddMsg(jewelryData,m_nPlayerGuid);

		newJewelryDatas.insertTail(jewelryData);
	}

//	jewelryUpdateToClient(newJewelryDatas);

	return newJewelryDatas;
}

void JewelryManager::jewelrySet(UInt32 jewelryPos, UInt64 jewelryUid)
{
	List<UInt64>::Iter * equipJewelryIter = m_pJewelryEquipData->mJewelryList.getn(jewelryPos -1);
	const ConnId & connId = m_pPlayer->getConnId();

	bool isOpen = getJewelrySlotOpenState(jewelryPos);

	if(!isOpen)
	{
		GCJewelrySetResp setResp;
		setResp.mPacketID = BOC_JEWELRY_SET_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::JewelryLock;
		Json::StyledWriter writer;
		setResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(setResp,connId);
		return;
	}

	//if(equipJewelryIter->mValue)
	//{
	//	//提示玩家已经穿戴了饰品
	//	GCJewelrySetResp setResp;
	//	setResp.mPacketID = BOC_JEWELRY_SET_RESP;
	//	Json::Value root;
	//	root["errorId"] = LynxErrno::JewelryEquiped;
	//	Json::StyledWriter writer;
	//	setResp.mRespJsonStr = writer.write(root);

	//	NetworkSystem::getSingleton().sendMsg(setResp,connId);
	//	return;
	//}

	Map<UInt64, JewelryData*>::Iter * jewelryDataIter = m_jewelryDataMap.find(jewelryUid);
	
	if(!jewelryDataIter)
	{
		GCJewelrySetResp setResp;
		setResp.mPacketID = BOC_JEWELRY_SET_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		setResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(setResp,connId);

		return;
	}

	NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryDataIter->mValue->m_nJewelryId);

		if(newJewelryTemp->mEquipSlotIdx != jewelryPos)
		{
			GCJewelrySetResp setResp;
			setResp.mPacketID = BOC_JEWELRY_SET_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::SlotPosError;
			Json::StyledWriter writer;
			setResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(setResp,connId);

			return;
		}

		equipJewelryIter->mValue = jewelryUid;

		postJewelryEquipMsg();

		GCJewelrySetResp setResp;
		setResp.mPacketID = BOC_JEWELRY_SET_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["jewelryPos"] = jewelryPos;
		root["jewelryUid"] = jewelryUid;

		Json::StyledWriter writer;
		setResp.mRespJsonStr = writer.write(root);
		cout << setResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(setResp,connId);

		char dest[1024]={0};
		snprintf(dest,sizeof(dest),"%d,%d",jewelryPos,jewelryUid);
		LogicSystem::getSingleton().write_log(LogType89,m_pPlayer->getPlayerGuid(), dest,LogInfo);
	
}


void JewelryManager::jewelryUnload(UInt32 jewelryPos)
{
	List<UInt64>::Iter * equipJewelryIter = m_pJewelryEquipData->mJewelryList.getn(jewelryPos -1);
	const ConnId & connId = m_pPlayer->getConnId();

	bool isOpen = getJewelrySlotOpenState(jewelryPos);

	if(!isOpen)
	{
		GCJewelryUnloadResp unloadResp;
		unloadResp.mPacketID = BOC_JEWELRY_UNLOAD_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::JewelryLock;
		Json::StyledWriter writer;
		unloadResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(unloadResp,connId);
		return;
	}

	//提示玩家饰品槽为空
	if(!equipJewelryIter->mValue)
	{
		GCJewelryUnloadResp unloadResp;
		unloadResp.mPacketID = BOC_JEWELRY_UNLOAD_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::SlotEmpty;
		Json::StyledWriter writer;
		unloadResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(unloadResp,connId);
		return;
	}

	equipJewelryIter->mValue = (UInt64)0;
	postJewelryEquipMsg();

	GCJewelryUnloadResp unloadResp;
	unloadResp.mPacketID = BOC_JEWELRY_UNLOAD_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["jewelryPos"] = jewelryPos;
	root["jewelryUid"] = equipJewelryIter->mValue;

	Json::StyledWriter writer;
	unloadResp.mRespJsonStr = writer.write(root);
	cout << unloadResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(unloadResp,connId);

	char dest[1024]={0};
	snprintf(dest,sizeof(dest),"%d,%d",jewelryPos,equipJewelryIter->mValue);
	LogicSystem::getSingleton().write_log(LogType90,m_pPlayer->getPlayerGuid(), dest,LogInfo);

	
}

void JewelryManager::jewelrySell(List<UInt64> sellList)
{

	Json::Value root;
	const ConnId & connId = m_pPlayer->getConnId();
	for(List<UInt64>::Iter* sellIter =  sellList.begin(); sellIter != NULL; sellIter = sellList.next(sellIter) )
	{
		Map<UInt64, JewelryData*>::Iter * jewelryDataIter = m_jewelryDataMap.find(sellIter->mValue);
		if(!jewelryDataIter)
		{
			GCJewelrySellResp sellResp;
			sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;

			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;
			sellResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(sellResp,connId);
			return;
		}
		
		List<UInt64>::Iter * equipIter = m_pJewelryEquipData->mJewelryList.find(sellIter->mValue, m_pJewelryEquipData->mJewelryList.begin(), m_pJewelryEquipData->mJewelryList.end());
		if(equipIter)
		{
			//有装备中的饰品
			GCJewelrySellResp sellResp;
			sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;

			root["errorId"] = LynxErrno::JewelryEquiped;
			Json::StyledWriter writer;
			sellResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(sellResp,connId);
			return;
		}

		NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryDataIter->mValue->m_nJewelryId);
		bool success = playerCostJewelry(sellIter->mValue);
		if(success)
		{
			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = newJewelryTemp->mPrice;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog6);

			root["jewelrySellList"].append(sellIter->mValue);
		}
		else
		{
			GCJewelrySellResp sellResp;
			sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;
			
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;
			sellResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(sellResp,connId);
			return;
		}
		
	}


	GCJewelrySellResp sellResp;
	sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;
	
	root["errorId"] = LynxErrno::None;
	root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

	Json::StyledWriter writer;
	sellResp.mRespJsonStr = writer.write(root);
	NetworkSystem::getSingleton().sendMsg(sellResp,connId);
	cout << sellResp.mRespJsonStr;

	
}

JewelryData* JewelryManager::addJewelrys(const UInt64 &jewelryId)
{
	NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryId);
	
	UInt64 jewelryUid = LogicSystem::getSingleton().generateItemGuid();
	JewelryData jewelryData;
	jewelryData.m_nJewelryUid = jewelryUid;
	jewelryData.m_nJewelryId = jewelryId;
	generateJewelryAttr(&jewelryData, newJewelryTemp);
	List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->insertTail(jewelryData);
	m_jewelryDataMap.insert(jewelryUid,&jewelryDataIter->mValue);

	return &jewelryDataIter->mValue;
	
}

bool JewelryManager::delJewelrys(const UInt64& jewelryUid)
{
	//先判断是否有这个饰品
	Map<UInt64, JewelryData*>::Iter * jewelryMapIter = m_jewelryDataMap.find(jewelryUid);
	if(!jewelryMapIter)
	{
		return false;
	}

	m_jewelryDataMap.erase(jewelryMapIter);
	

	for(List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->begin(); jewelryDataIter != NULL; 
		jewelryDataIter = m_pJewelryList->next(jewelryDataIter))
	{
		if(jewelryDataIter->mValue.m_nJewelryUid == jewelryUid)
		{
			m_pJewelryList->erase(jewelryDataIter);
			return true;
		}
	}
		
	return true;
}

void JewelryManager::generateJewelryAttr(JewelryData * jewelryData, NewJewelryTemplate * newJewelryTemp)
{
	jewelryData->m_staticAttrKey = newJewelryTemp->mStaticAttr1Key;

	UInt32 staticAttrValue = getRandomValue(newJewelryTemp->mStaticAttr1ValueMin, newJewelryTemp->mStaticAttr1ValueMax);
	jewelryData->m_staticAttrValue = staticAttrValue;
	generateRandomAttr(jewelryData, newJewelryTemp);

}	

void JewelryManager::generateRandomAttr(JewelryData * jewelryData, NewJewelryTemplate * newJewelryTemp)
{
	switch(newJewelryTemp->mPolishCount)
	{
		case 1:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				
			}
			break;
		case 2:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
			}
			break;
		case 3:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr3);
			}
			break;
		case 4:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr3);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr4);
			}
			break;
		case 5:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr3);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr4);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr5);
			}
			break;
		default:
			break;
	}
}



void JewelryManager::addRandomAttribute(JewelryData* jewelryData, UInt32 randomID)
{
	if(!randomID)
	{
		JewelryAttr randomAttr;
		jewelryData->m_randomAttrList.insertTail(randomAttr);
		return;
	}

	AttrRandomTemplate* attrRandom = ATTRRANDOM_TABLE().get(randomID);

	//PA 百分比的最大值
	UInt32 PAPercent = attrRandom->mPARate;

	//MA 百分比的最大值，判断时要看随机出来的值是否在 PA 和 MA之间
	UInt32 MAPercent = PAPercent + attrRandom->mMARate;

	UInt32 PFPercent = MAPercent + attrRandom->mPFRate;

	UInt32 MFPercent = PFPercent + attrRandom->mMFRate;

	UInt32 MaxHPPercent = MFPercent + attrRandom->mMaxHPRate;

	UInt32 MaxMPPercent = MaxHPPercent + attrRandom->mMaxMPRate;

	UInt32 MaxSPPercent = MaxMPPercent + attrRandom->mMaxSPRate;

	UInt32 CritRatePercent = MaxSPPercent + attrRandom->mCritRateRate;

	UInt32 CritDamagePercent = CritRatePercent + attrRandom->mCritDamageRate;

	UInt32 CritDefPercent =  CritDamagePercent + attrRandom->mCritDefRate;

	UInt32 randomRes = rand()%CritDefPercent;

	JewelryAttr randomAttr;

	randomAttr.mRandomID = attrRandom->mId;

	//如果随机出来的值大于等于critDamagePercent上限值，小于CritDefPercent上限值,那么随机出来的属性为critDef
	if(randomRes < CritDefPercent && randomRes >= CritDamagePercent)
	{
		randomAttr.mKey = BaseToughness;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDefMin,attrRandom->mCritDefMax);
		randomAttr.mValue = randomValue;
	}
	//属性为critdamage
	else	if(randomRes >= CritRatePercent)
	{
		randomAttr.mKey = BaseCrit;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDamageMin,attrRandom->mCritDamageMax);
		randomAttr.mValue = randomValue;
	}
	//大于maxSPPercent最大值，小于CritRate，属性为critrate
	else if(randomRes >= MaxSPPercent)
	{
		randomAttr.mKey = Critrate;
		UInt32 randomValue = getRandomValue(attrRandom->mCritRateMin,attrRandom->mCritRateMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MaxMPPercent)
	{
		//属性为MAXsp
		randomAttr.mKey = MaxSP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxSPMin,attrRandom->mMaxSPMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MaxHPPercent)
	{
		//属性为MaxMP
		randomAttr.mKey = MaxMP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxMPMin, attrRandom->mMaxMPMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MFPercent)
	{
		//属性为最大HP
		randomAttr.mKey = MaxHP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxHPMin, attrRandom->mMaxHPMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= PFPercent)
	{
		//属性为MFPercent
		randomAttr.mKey = MF;
		UInt32 randomValue = getRandomValue(attrRandom->mMFMin,attrRandom->mMFMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MAPercent)
	{
		//属性为PFPercent
		randomAttr.mKey = PF;
		UInt32 randomValue = getRandomValue(attrRandom->mPFMin, attrRandom->mPFMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= PAPercent)
	{
		//属性为MAPercent
		randomAttr.mKey = MA;
		UInt32 randomValue = getRandomValue(attrRandom->mMAMin,attrRandom->mMAMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= 0)
	{
		//属性为PAPercent
		randomAttr.mKey = PA;
		UInt32 randomValue = getRandomValue(attrRandom->mPAMin,attrRandom->mPAMax);
		randomAttr.mValue = randomValue;
	}
	else
	{
		assert(false);
	}

	jewelryData->m_randomAttrList.insertTail(randomAttr);
}


bool JewelryManager::playerCostJewelry( UInt64 jewelryUid)
{
	bool flag = delJewelrys(jewelryUid);
	
	if(flag)
	{
		postDelMsg(jewelryUid,m_nPlayerGuid);
		return true;
	}
	
	return false;
}


void  JewelryManager::jewelryPolish(UInt64 objJewelryUid, UInt64 materialJewelryUid, UInt32 objRdAttrIndex)
{

	Map<UInt64, JewelryData *>::Iter * objJewelryIter = m_jewelryDataMap.find(objJewelryUid);
	Map<UInt64, JewelryData *>::Iter * materialJewelryIter = m_jewelryDataMap.find(materialJewelryUid);

	if(!objJewelryIter || !materialJewelryIter)
	{
		//发送错误提示到客户端
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return ;
	}
	
	List<UInt64>::Iter * equipIter = m_pJewelryEquipData->mJewelryList.find(materialJewelryUid, m_pJewelryEquipData->mJewelryList.begin(), m_pJewelryEquipData->mJewelryList.end());
	if(equipIter)
	{
		//有装备中的饰品
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::JewelryEquiped;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return;
	}
	

	JewelryData * objJewelryData = objJewelryIter->mValue;
	JewelryData * materialJewelryData = materialJewelryIter->mValue;

	NewJewelryTemplate * objJewelryTemp = NEWJEWELRY_TABLE().get(objJewelryData->m_nJewelryId);
	NewJewelryTemplate * materialJewelryTemp = NEWJEWELRY_TABLE().get(materialJewelryData->m_nJewelryId);

	if(objJewelryTemp->mRefreshCoin > m_pPlayer->mPlayerData.mBaseData.m_nCoin)
	{
		//提示玩家铜钱不足
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return ;
	}

	//消耗铜钱

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_COIN;
	goods.num = 0 - objJewelryTemp->mRefreshCoin;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog7);


	AttrPolishTemplate *materialPolishTemp = ATTRPOLISH_TABLE().get(materialJewelryTemp->mLvRequire);

	JewelryAttr *jewelryAttrFind = NULL;
	
	//是否有橙色属性
	bool orangeFlag = false;

	//在材料装备中的橙色属性,随机概率对应的属性组成的map,主要用于目标装备中橙色洗练次数大于等于1，
	//并且材料装备中有橙色属性的情况
	Map<UInt32, JewelryAttr*> orangeRdMap;
	UInt32 orangeRate = 0;

	//是否有紫色属性
	bool purpleFlag = false;
	//在材料装备中的紫色属性，随机概率对应的属性组成的map，主要用于目标装备中紫色洗练次数大于等于1，
	//并且材料装备中有紫色属性的情况
	Map<UInt32, JewelryAttr*> purpleRdMap;
	UInt32 purpleRate = 0;

	//材料装备中所有颜色的属性，随机概率对应的属性组成的map，主要用于目标装备中紫色和橙色积累次数为0，
	//或者材料装备中没有紫色或橙色属性的概率统计情况
	Map<UInt32, JewelryAttr*> totalRdMap;
	UInt32 totalRate = 0;

	Vector<JewelryAttr*> rdAttrVecs; 

	
	creatQualityMap(materialJewelryData,materialPolishTemp,orangeFlag,orangeRdMap,orangeRate, purpleFlag,purpleRdMap,purpleRate,totalRdMap,totalRate);

	if(totalRdMap.empty())
	{
		//提示玩家数据错误，因为材料饰品没有随机属性
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return;
	}

	//先判断橙色，如果有橙色洗练积累的次数
	if(objJewelryData->m_orangeTimes >= 1)
	{

		//防止策划更改，将不同颜色逻辑分开
		if(orangeFlag)
		{
			orangeSuccess(orangeRdMap,objJewelryData,jewelryAttrFind,orangeRate);
		}
		else
		{
			//如果材料装备没有橙色属性，判断目标装备的紫色洗练累计次数是否为大于等于1
			if(objJewelryData->m_purpleTimes >= 1)
			{
				//目标装备的紫色洗练积累次数达到大于等于一,判断材料装备中是否有紫色属性
				if(purpleFlag)
				{
					//如果材料装备有紫色属性
					purpleSuccess(purpleRdMap,objJewelryData,jewelryAttrFind,purpleRate);
				}
				else
				{
					//如果材料装备中没有紫色属性，那么从所有属性map中随机
					totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);
				}
			}
			else
			{
				//如果目标装备的紫色洗练次数为0，橙色次数>= 1,并且材料装备没有橙色属性，那么就要采取材料装备全部属性的随机逻辑
				totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);

			}

		}
	}
	else
	{
		//目标装备的橙色洗练次数为0，判断目标装备紫色积累次数是否大于等于1
		if(objJewelryData->m_purpleTimes >= 1)
		{
			//目标装备的紫色洗练积累次数达到大于等于一,判断材料装备中是否有紫色属性
			if(purpleFlag)
			{
				//如果材料装备有紫色属性
				purpleSuccess(purpleRdMap,objJewelryData,jewelryAttrFind,purpleRate);
			}
			else
			{
				//如果材料装备中没有紫色属性，那么从所有属性map中随机
				totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);
			}
		}
		else
		{
			//目标装备既没有紫色洗练积累次数，也没有橙色洗练积累次数，那么从所有属性中随机选取
			totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);
		}
	}

	assert(jewelryAttrFind);

	List<JewelryAttr>::Iter * objRdIter = objJewelryData->m_randomAttrList.getn(objRdAttrIndex);

	if(!objRdIter)
	{
		//提示玩家数据错误，
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return;
	}
	

	assert(objRdIter);
	PolishPointsTemplate * findPolishPointsTemp = NULL;
	for(PolishPointsTemplateMap::Iter * polishPointsMapIter = POLISHPOINTS_TABLE().mMap.begin(); 
		polishPointsMapIter != NULL;
		polishPointsMapIter = POLISHPOINTS_TABLE().mMap.next(polishPointsMapIter))
	{
		if(m_pPlayer->mPlayerData.mBaseData.m_nLevel < polishPointsMapIter->mValue.mLevel)
		{
			break;
		}
		
		findPolishPointsTemp = &polishPointsMapIter->mValue;
	}

	//如果玩家高于或者等于40级，那么增加洗练积分
	if(findPolishPointsTemp)
	{
		//增加洗练积分，根据材料装备的颜色品质进行增加

		MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialJewelryTemp->mLvRequire);
		switch(materialJewelryTemp->mQuality)
		{

			

		case Blue:
			{

				objJewelryData->m_orangePoints += materialPointsTem->mBluePoints;
				objJewelryData->m_purplePoints += materialPointsTem->mBluePoints;
			}
			break;

		case Purple:
			{

				objJewelryData->m_orangePoints += materialPointsTem->mPurplePoints;
				objJewelryData->m_purplePoints += materialPointsTem->mPurplePoints;
			}
			break;

		case Orange:
			{
				
				objJewelryData->m_orangePoints+= materialPointsTem->mOrangePoints;
				objJewelryData->m_purplePoints+= materialPointsTem->mOrangePoints;
			}
			break;

		default:
			break;

		}


		//重新统计目标装备的强化积分和橙色与紫色的累计次数


		//计算橙色属性的累计次数
		while(objJewelryData->m_orangePoints>=findPolishPointsTemp->mOrangePoints)
		{
			objJewelryData->m_orangePoints -= findPolishPointsTemp->mOrangePoints;
			objJewelryData->m_orangeTimes++;
		}

		//计算紫色属性的累计次数
		while(objJewelryData->m_purplePoints >= findPolishPointsTemp->mPurplePoints)
		{
			objJewelryData->m_purplePoints -= findPolishPointsTemp->mPurplePoints;
			objJewelryData->m_purpleTimes++;
		}

		if(objJewelryData->m_orangeTimes>3)
		{
			objJewelryData->m_orangeTimes = 3;
		}

		if(objJewelryData->m_purpleTimes>3)
		{
			objJewelryData->m_purpleTimes = 3;
		}

	}
 
	//将目标装备的原来属性保存在临时变量里，用于发给客户端,现在改为保存在装备字段里

	objJewelryData ->m_oldJewelryAttr = objRdIter->mValue;
	objJewelryData ->m_oldAttrIndex = objRdAttrIndex;

	//洗练更改属性后，需重新计算新的属性在目标装备中的品质

	UInt64 attrNewId = getNewAttrId(*jewelryAttrFind,objJewelryTemp);
	//属性替换，但是这个时候的品质是相对材料装备的，下面会进行更改为相对目标装备的
	objRdIter->mValue = *jewelryAttrFind;
	//将计算好的新品质赋值给目标装备
	objRdIter->mValue.mRandomID = attrNewId;

	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	//存盘标记
	postUpdateMsg(objJewelryData,playerUid);


	//计算找到的那条随机属性的索引，从0开始
	UInt32 materialAttrIndexFind = 0;
	List<JewelryAttr>& materialRdAttrList =  materialJewelryData->m_randomAttrList;
	for(List<JewelryAttr>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.mKey == jewelryAttrFind->mKey && materialRdIter->mValue.mValue ==  jewelryAttrFind->mValue &&
			materialRdIter->mValue.mRandomID == jewelryAttrFind->mRandomID)
		{
			break;
		}
		materialAttrIndexFind++;
	}

	playerCostJewelry(materialJewelryUid);
	
	//发送成功消息给客户端
	GCJewelryPolishResp polishResp;

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["objuid"] = objJewelryUid;
	root["materialuid"] = materialJewelryUid;
	root["newAttr"]["type"] = objRdIter->mValue.mKey;
	root["newAttr"]["attrId"] = objRdIter->mValue.mRandomID;
	root["newAttr"]["value"] =  objRdIter->mValue.mValue;

	root["oldAttr"]["type"] = objJewelryData->m_oldJewelryAttr.mKey;
	root["oldAttr"]["value"] = objJewelryData->m_oldJewelryAttr.mValue;
	root["oldAttr"]["attrId"] = objJewelryData->m_oldJewelryAttr.mRandomID;

	root["objAttrIndex"] = objRdAttrIndex;
	root["materialAttrIndex"] = materialAttrIndexFind;
	root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

	Json::StyledWriter writer;
	polishResp.mRespJsonStr = writer.write(root);
	polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

	const ConnId& connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(polishResp, connId);

	//cout << polishResp.mRespJsonStr;

	//更新七日训
	LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT10,1);

}

void JewelryManager::jewelryReset(UInt64 objJewelryUid )
{
	Map<UInt64, JewelryData*>::Iter * objJewelryIter = m_jewelryDataMap.find(objJewelryUid);
	if(!objJewelryIter)
	{
		LOG_INFO("equip not find polishResetEquip!!");

		Json::Value root;
		root["errorId"] = LynxErrno::ItemNotExist;
		
		Json::StyledWriter writer;

		GCJewelryResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = BOC_JEWELRY_RESET_RESP;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}

	if(m_pPlayer->mPlayerData.mBaseData.m_nGold < PolishNeedCoin)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::RmbNotEnough;

		Json::FastWriter writer;

		GCJewelryResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = BOC_JEWELRY_RESET_RESP;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}


	UInt32 & rdIndex = objJewelryIter->mValue->m_oldAttrIndex;
	JewelryAttr & rdAttribute = objJewelryIter->mValue->m_oldJewelryAttr;

	List<JewelryAttr>::Iter * rdAttrIter = objJewelryIter->mValue->m_randomAttrList.getn(rdIndex);
	assert(rdAttrIter);
	rdAttrIter->mValue = rdAttribute;

	postUpdateMsg(objJewelryIter->mValue,m_nPlayerGuid);
	

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_GOLD;
	goods.num = 0 - PolishNeedCoin;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog7);

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["objRdAttrIndex"] = objJewelryIter->mValue->m_oldAttrIndex;

	root["oldAttr"]["type"] = rdAttrIter->mValue.mKey;
	root["oldAttr"]["value"] =  rdAttrIter->mValue.mValue;
	root["oldAttr"]["attrId"] = rdAttrIter->mValue.mRandomID;
	root["jewelryUid"] = objJewelryUid;
	root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
	root["gold"] = m_pPlayer->mPlayerData.mBaseData.m_nGold;

	Json::StyledWriter writer;

	GCJewelryResetResp resetResp;
	resetResp.mRespJsonStr = writer.write(root);
	resetResp.mPacketID = BOC_JEWELRY_RESET_RESP;

	const ConnId& connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resetResp,connId);

	cout << resetResp.mRespJsonStr;
}

UInt64 JewelryManager::getPARdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr )
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mPAMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mPAMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mPAMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mPAMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}
}


UInt64 JewelryManager::getMARdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr )
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMAMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMAMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMAMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMAMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}
}


UInt64 JewelryManager::getPFRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mPFMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mPFMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mPFMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mPFMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getMFRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMFMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMFMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMFMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMFMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}


UInt64 JewelryManager::getMaxHPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMaxHPMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMaxHPMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMaxHPMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMaxHPMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getMaxMPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMaxMPMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMaxMPMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMaxMPMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMaxMPMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getMaxSPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMaxSPMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMaxSPMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMaxSPMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMaxSPMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getCritRateRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mCritRateMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mCritRateMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mCritRateMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mCritRateMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}


UInt64 JewelryManager::getCirtDamageRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mCritDamageMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mCritDamageMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mCritDamageMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mCritDamageMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getBaseToughnessRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mCritDefMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mCritDefMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mCritDefMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mCritDefMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getNewAttrId(const JewelryAttr &randomAttr, NewJewelryTemplate * objectTemplate)
{
	//通过equip表找到装备对应的等级

	UInt32 level = objectTemplate->mLvRequire;

	if(randomAttr.mKey == PA)
	{
		return getPARdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MA)
	{
		return getMARdId(level, randomAttr);
	}
	else if(randomAttr.mKey == PF)
	{
		return getPFRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MF)
	{
		return getMFRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MaxHP)
	{
		return getMaxHPRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MaxMP)
	{
		return getMaxMPRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MaxSP)
	{
		return getMaxSPRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == Critrate)
	{
		return getCritRateRdId(level,randomAttr);
	}
	else if(randomAttr.mKey == BaseCrit)
	{
		return getCritRateRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == BaseToughness)
	{
		//韧性
		return getBaseToughnessRdId(level, randomAttr);

	}
	

	return 0;
}



void JewelryManager::creatQualityMap(JewelryData * materialJewelryData,AttrPolishTemplate *materialPolishTemp,bool& orangeFlag,
					 Map<UInt32, JewelryAttr*> &orangeRdMap,
					 UInt32& orangeRate, bool& purpleFlag,Map<UInt32, JewelryAttr*> &purpleRdMap,
					 UInt32& purpleRate,
					 Map<UInt32, JewelryAttr*> &totalRdMap,UInt32& totalRate)
{
	List<JewelryAttr>& materialRdAttrList =  materialJewelryData->m_randomAttrList;


	for(List<JewelryAttr>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.mKey == 0)
		{
			continue;
		}

		AttrRandomTemplate * attrRdTemp = ATTRRANDOM_TABLE().get(materialRdIter->mValue.mRandomID);
		if (attrRdTemp == NULL)
		{
			LOG_WARN("attrRdTemp not found!!");
			return;
		}
		if(attrRdTemp->mQuality == Orange)
		{
			orangeFlag = true;

			//橙色随机值分段增加
			orangeRate += materialPolishTemp->mOrange;
			//全类型随机值分段增加
			totalRate += materialPolishTemp->mOrange;
			//添加橙色,key 为当前属性随机的最大值上限
			orangeRdMap.insert(orangeRate,&materialRdIter->mValue);
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else if(attrRdTemp->mQuality == Purple)
		{
			purpleFlag = true;
			//紫色随机值分段增加
			purpleRate += materialPolishTemp->mPurple;
			//全类型的随机值分段增加
			totalRate += materialPolishTemp->mPurple;
			//添加紫色
			purpleRdMap.insert(purpleRate,&materialRdIter->mValue);
			//将所有种类的插入map
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else
		{
			//将此类属性放入所有种类的map里
			if(attrRdTemp->mQuality == White)
			{
				totalRate += materialPolishTemp->mWhite;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(attrRdTemp->mQuality == Green)
			{
				totalRate += materialPolishTemp->mGreen;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(attrRdTemp->mQuality == Blue)
			{
				totalRate += materialPolishTemp->mBlue;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
		}

	}


}



void JewelryManager::orangeSuccess(Map<UInt32, JewelryAttr*> &orangeRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &orangeRate)
{
	//orangrate主要就是处理材料装备中有多条橙色属性的权重，随机出一个值
	UInt32 orangeValue = rand()%orangeRate;
	UInt32 orangeBase = 0;
	for(Map<UInt32, JewelryAttr*>::Iter* orangeRdIter =  orangeRdMap.begin(); orangeRdIter != NULL; 
		orangeRdIter = orangeRdMap.next(orangeRdIter))
	{

		//如果随机值在某条橙色属性的临界值里，那么取出这条属性，直到遍历结束，一定会找到一个这样的属性
		if(orangeValue < orangeRdIter->mKey)
		{
			materialRdFind = orangeRdIter->mValue;
			//消耗掉橙色一次洗练积累次数
			objectItemData->m_orangeTimes--;
			break;
		}

	}

	assert(materialRdFind);
}


void JewelryManager::purpleSuccess(Map<UInt32, JewelryAttr*> &purpleRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &purpleRate)
{
	UInt32 purpleValue = rand()%purpleRate;
	UInt32 purpleBase = 0;
	for(Map<UInt32, JewelryAttr*>::Iter* purpleRdIter =  purpleRdMap.begin(); purpleRdIter != NULL; 
		purpleRdIter = purpleRdMap.next(purpleRdIter))
	{

		//如果随机值在某条紫色属性的临界值里，那么取出这条属性，直到遍历结束，一定会找到一个这样的属性
		if(purpleValue < purpleRdIter->mKey)
		{
			materialRdFind = purpleRdIter->mValue;
			//消耗掉一次紫色洗练积累次数
			objectItemData->m_purpleTimes--;
			break;
		}

	}
	//断言一定能找到这个属性
	assert(materialRdFind);
}


void JewelryManager::totalAttrRandom(Map<UInt32, JewelryAttr*> &totalRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &totalRate)
{
	UInt32 totalValue = rand()%totalRate;
	UInt32 totalBase = 0;
	for(Map<UInt32, JewelryAttr*>::Iter * totalRdIter  = totalRdMap.begin(); totalRdIter != NULL;
		totalRdIter = totalRdMap.next(totalRdIter))
	{

		//如果随机值在某条属性的临界值里，那么取出这条属性，直到遍历结束，一定会找到这个属性
		if(totalValue < totalRdIter->mKey)
		{
			materialRdFind = totalRdIter->mValue;
			break;
		}
	}

	assert(materialRdFind);
}


std::string JewelryManager::convertDataToJson()
{
	/*UInt64 time1 = TimeUtil::getTimeMilliSec();
	cout << "1111111" <<endl << time1 << endl;*/
	//断言m_pjewelryList 一定非空 sec
	assert(m_pJewelryList);
	Json::Value root;
	
	for(List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->begin(); jewelryDataIter != NULL; 
		jewelryDataIter = m_pJewelryList->next(jewelryDataIter))
	{
		Json::Value jewelryRoot;
		jewelryRoot["uuid"] = jewelryDataIter->mValue.m_nJewelryUid;
		jewelryRoot["typeId"] = jewelryDataIter->mValue.m_nJewelryId;
		

		
		for(List<JewelryAttr>::Iter * jewelryAttrIter = jewelryDataIter->mValue.m_randomAttrList.begin(); jewelryAttrIter != NULL; 
			jewelryAttrIter = jewelryDataIter->mValue.m_randomAttrList.next(jewelryAttrIter))
		{
			Json::Value attrRoot;
			attrRoot["randomID"] = jewelryAttrIter->mValue.mRandomID;
			attrRoot["key"] = jewelryAttrIter->mValue.mKey;
            attrRoot["value"] = jewelryAttrIter->mValue.mValue;

			jewelryRoot["randomAttrList"].append(attrRoot);
			
		}
		
		 
		jewelryRoot["staticAttrKey"] = jewelryDataIter->mValue.m_staticAttrKey;
		jewelryRoot["staticAttrValue"] = jewelryDataIter->mValue.m_staticAttrValue;

		
		
		jewelryRoot["purplePoints"] = jewelryDataIter->mValue.m_purplePoints;

		jewelryRoot["purpleTimes"] = jewelryDataIter->mValue.m_purpleTimes;

		jewelryRoot["orangePoints"] = jewelryDataIter->mValue.m_orangePoints;

		jewelryRoot["orangeTimes"] = jewelryDataIter->mValue.m_orangeTimes;

		root["jewelryList"].append(jewelryRoot);
	}

	/*UInt64 time2 = TimeUtil::getTimeMilliSec() ;
	cout << "2222" <<endl<< time2 - time1 << endl;*/

	UInt32 jewelrySlot = 0;
	for(List<UInt64>::Iter * jewelryIter = m_pJewelryEquipData->mJewelryList.begin(); jewelryIter != NULL; 
		jewelryIter = m_pJewelryEquipData->mJewelryList.next(jewelryIter))
	{
		jewelrySlot++;
		if(jewelryIter->mValue)
		{
			root["equipJewelrys"].append(jewelryIter->mValue);
		}
		else
		{
			bool isOpen = getJewelrySlotOpenState(jewelrySlot);
			if(isOpen)
			{
				root["equipJewelrys"].append(0);
			}
			else
			{
				root["equipJewelrys"].append(-1);
			}
		}
	}

	//UInt64 time3 = TimeUtil::getTimeMilliSec();
	//cout << "3333" <<endl<< time3 - time2 << endl; 


	Json::StyledWriter writer;
	return writer.write(root);
}

void JewelryManager::postJewelryEquipMsg()
{
	PersistUpdateJewelryEquipNotify jewelryEquipMsg;
	jewelryEquipMsg.m_nPlayerUid = m_nPlayerGuid;
	jewelryEquipMsg.m_jewelryEquipData = *m_pJewelryEquipData;

	PersistSystem::getSingleton().postThreadMsg(jewelryEquipMsg, m_nPlayerGuid);
}

void JewelryManager::checkLvAcitve()
{
	bool change = false;

	HeroEquipRuleTemplate *heroEquipRule = HEROEQUIPRULE_TABLE().get(m_pPlayer->mPlayerData.mBaseData.m_nModelID);
	if (heroEquipRule == NULL)
	{
		LOG_WARN("heroEquipRule not found!!");
		return;
	}
	UInt32 &playerLv = m_pPlayer->mPlayerData.mBaseData.m_nLevel;

	bool isOpen1 = getJewelrySlotOpenState(1);
	if(!isOpen1)
	{
		if(heroEquipRule->mJewelry1UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry1OpenBit;
			change = true;
		}
	}

	bool isOpen2 = getJewelrySlotOpenState(2);

	if(!isOpen2)
	{
		if(heroEquipRule->mJewelry2UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry2OpenBit;
			change = true;
		}
	}

	bool isOpen3 = getJewelrySlotOpenState(3);

	if(!isOpen3)
	{
		if(heroEquipRule->mJewelry3UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry3OpenBit;
			change = true;
		}
	}

	bool isOpen4 = getJewelrySlotOpenState(4);

	if(!isOpen4)
	{
		if(heroEquipRule->mJewelry4UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry4OpenBit;
			change = true;
		}
	}

	bool isOpen5 = getJewelrySlotOpenState(5);

	if(!isOpen5)
	{
		if(heroEquipRule->mJewelry5UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry5OpenBit;
			change = true;
		}
	}

	bool isOpen6 = getJewelrySlotOpenState(6);

	if(!isOpen6)
	{
		if(heroEquipRule->mJewelry6UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry6OpenBit;
			change = true;
		}
	}

	if(change)
	{
		postJewelryEquipMsg();
	}
}


bool JewelryManager::getJewelrySlotOpenState(UInt32 jewelrySlot)
{
	bool isOpen = false;
	switch(jewelrySlot)
	{
		case 1:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry1OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 2:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry2OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 3:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry3OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 4:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry4OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}	
			break;
		case 5:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry5OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 6:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry6OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		
	}

	return isOpen;
	
}

Json::Value JewelryManager::getJewelryJsonNode(Json::Value &jewelryRoot,JewelryData* jewelryData)
{
	
	jewelryRoot["uuid"] = jewelryData->m_nJewelryUid;
	jewelryRoot["typeId"] = jewelryData->m_nJewelryId;



	for(List<JewelryAttr>::Iter * jewelryAttrIter = jewelryData->m_randomAttrList.begin(); jewelryAttrIter != NULL; 
		jewelryAttrIter = jewelryData->m_randomAttrList.next(jewelryAttrIter))
	{
		Json::Value attrRoot;
		attrRoot["randomID"] = jewelryAttrIter->mValue.mRandomID;
		attrRoot["key"] = jewelryAttrIter->mValue.mKey;
		attrRoot["value"] = jewelryAttrIter->mValue.mValue;

		jewelryRoot["randomAttrList"].append(attrRoot);

	}


	jewelryRoot["staticAttrKey"] = jewelryData->m_staticAttrKey;
	jewelryRoot["staticAttrValue"] = jewelryData->m_staticAttrValue;



	jewelryRoot["purplePoints"] = jewelryData->m_purplePoints;

	jewelryRoot["purpleTimes"] = jewelryData->m_purpleTimes;

	jewelryRoot["orangePoints"] = jewelryData->m_orangePoints;

	jewelryRoot["orangeTimes"] = jewelryData->m_orangeTimes;

	
	return jewelryRoot;

}

void JewelryManager::jewelryUpdateToClient(List<JewelryData *> getList)
{
	
	const ConnId& connId = m_pPlayer->getConnId();
	Json::Value root;

	Json::Value tmpValue;

	for(List<JewelryData*>::Iter * jewelryDataIter = getList.begin(); jewelryDataIter != NULL; 
		jewelryDataIter = getList.next(jewelryDataIter))
	{
		Json::Value jewelryRoot;
		getJewelryJsonNode(jewelryRoot,jewelryDataIter->mValue);
		root.append(jewelryRoot);
	}

	Json::StyledWriter writer;

	GCJewelryUpdateResp updateResp;
	updateResp.mPacketID = BOC_JEWELRYUPDATE_RESP;
	updateResp.mRespJsonStr = writer.write(root);

	cout << updateResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(updateResp,connId);
	
}

void JewelryManager::diaoluoSend(List<JewelryData *> getList)
{
	Json::Value jsonValue;
	for(List<JewelryData *>::Iter * iter = getList.begin();iter !=  NULL; iter = getList.next(iter) )
	{	
		Json::Value tmpValue;
		getJewelryJsonNode(tmpValue,iter->mValue);
		jsonValue.append(tmpValue);
	}

	 LogicSystem::getSingleton().combinSendData(m_pPlayer->getPlayerGuid(),AWARD_JEWELRY,jsonValue);
	
}

UInt32 JewelryManager::getJewelryCount()
{
	return m_pJewelryList->size();
}


void JewelryManager::jewelryEquipOnce(Map<UInt32 ,UInt64> equipMap)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	for(Map<UInt32 ,UInt64>::Iter * equipIter = equipMap.begin(); equipIter != NULL; equipIter = equipMap.end())
	{
		if(equipIter->mKey > 6 || equipIter ->mKey < 0)
		{

			GCJewelrySetOnceResp setOnceResp;
			setOnceResp.mPacketID = BOC_JEWELRY_SETONCE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;
			
			setOnceResp.mRespJsonStr = writer.write(root);
			cout << setOnceResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(setOnceResp, connId);
			return ;
		}

		Map<UInt64, JewelryData*>::Iter * jewelryFindIter = m_jewelryDataMap.find(equipIter->mValue);
		if(!jewelryFindIter)
		{
			GCJewelrySetOnceResp setOnceResp;
			setOnceResp.mPacketID = BOC_JEWELRY_SETONCE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;

			setOnceResp.mRespJsonStr = writer.write(root);
			cout << setOnceResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(setOnceResp, connId);
			return;
		}


	}

	Json::Value root;

	UInt32 equipIndex = 1;

	for(List<UInt64>::Iter * equipListIter = m_pJewelryEquipData->mJewelryList.begin(); equipListIter != NULL;
		equipListIter = m_pJewelryEquipData->mJewelryList.next(equipListIter))
	{
		bool isOpen = getJewelrySlotOpenState(equipIndex);
	
		if(!isOpen)
		{
			Json::Value equipRoot;

			equipRoot["pos"] = equipIndex;
			equipRoot["jewelryuid"] = -1;

			root["equipdata"].append(equipRoot);
			equipIndex ++;
			continue;
		}
		
		Map<UInt32 ,UInt64>::Iter * exitIter = equipMap.find(equipIndex);
		//map中有次位置的更新
		if(exitIter)
		{
			equipListIter->mValue = exitIter->mValue;
		}

		Json::Value equipRoot;

		equipRoot["pos"] = equipIndex;
		equipRoot["jewelryuid"] = equipListIter->mValue;

		root["equipdata"].append(equipRoot);
		equipIndex ++;
	}

	root["errorId"] = LynxErrno::None;

	Json::StyledWriter writer;

	GCJewelrySetOnceResp setOnceResp;
	setOnceResp.mRespJsonStr = writer.write(root);
	setOnceResp.mPacketID = BOC_JEWELRY_SETONCE_RESP;

	cout << setOnceResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(setOnceResp,connId);

	postJewelryEquipMsg();
	

}