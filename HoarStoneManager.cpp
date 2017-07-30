#include "HoarStoneManager.h"
#include "Player.h"
#include "Table.h"
#include "GameServer.h"

using namespace Lynx;
HoarStoneManager::HoarStoneManager()
{
	m_pPlayer = NULL;
	m_pHoarStoneList = NULL;
	m_pRuneDataMap = NULL;
}	


HoarStoneManager:: ~HoarStoneManager()
{

}

bool HoarStoneManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pHoarStoneList = &m_pPlayer->mPlayerData.mHoarStoneData.mHoarStoneList;

	m_pRuneDataMap = &m_pPlayer->mPlayerData.mHoarStoneData.mRuneDataMap;

	for(List<HoarStoneData>::Iter * hoarStoneIter = m_pHoarStoneList->begin(); hoarStoneIter != NULL;
		hoarStoneIter = m_pHoarStoneList->next(hoarStoneIter))
	{
		mHoarStoneMap.insert(hoarStoneIter->mValue.mHoarStoneId,&hoarStoneIter->mValue);
	}

	return true;

}

void HoarStoneManager::release()
{
	//指针置空，map 清空
	
	m_pPlayer = NULL;
	m_pHoarStoneList = NULL;
	m_pRuneDataMap = NULL;
}


std::string HoarStoneManager::convertDataToJson()
{
	Json::Value root;
	
	for(List<HoarStoneData>::Iter * hoarStoneIter = m_pHoarStoneList->begin(); hoarStoneIter != NULL; 
		hoarStoneIter = m_pHoarStoneList->next(hoarStoneIter))
	{
		Json::Value hoarStoneRoot;
		hoarStoneRoot["level"] = hoarStoneIter->mValue.mStoneLv;
		hoarStoneRoot["piece"] = hoarStoneIter->mValue.mPieceCount;
		hoarStoneRoot["star"] = hoarStoneIter->mValue.mStoneStar;

		for(List<UInt64>::Iter * equipIter = hoarStoneIter->mValue.mEquipList.begin(); equipIter != NULL; 
			equipIter = hoarStoneIter->mValue.mEquipList.next(equipIter))
		{
			hoarStoneRoot["equip"].append(equipIter->mValue);
		}

		root["base"].append(hoarStoneRoot);

	}


	std::stringstream mystream;

	for(RuneDataMap::Iter*  runeIter = m_pRuneDataMap->begin(); runeIter != NULL; runeIter = m_pRuneDataMap->next(runeIter))
	{
		mystream.clear();
		mystream.str("");

		mystream << runeIter->mKey;
		std::string runeKeyStr;
		mystream >> runeKeyStr; 

		root["equip"][runeKeyStr] = runeIter->mValue;
	}


	Json::StyledWriter writer;

	std::string jsonStr = writer.write(root);

	//cout << jsonStr;

	return jsonStr;
}


void HoarStoneManager::hoarStoneActive(UInt64 playerUid, UInt64 hoarstoneIdx)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarstoneIdx);
	
	if(mapHoarStoneIter->mValue->mStoneLv)
	{
		//提示玩家界石等级不为0，也就是该界石已经激活了
		GCHoarStoneActiveResp activeResp;
		activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::HoarStoneActived;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		activeResp.mRespJsonStr = respStr;
		
		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(activeResp,connId);
		cout << respStr;
		return ;
	}

	if(mapHoarStoneIter)
	{
		bool checkRes = checkCondition(hoarstoneIdx);
		
		if(checkRes)
		{
			UInt64 hoarStoneId = mapHoarStoneIter->mValue->mHoarStoneId;
			UInt32 hoarStoneLv = mapHoarStoneIter->mValue->mStoneLv;
		
			//数据表有更改，原来是按照下一级id查找，现在在本级id就可以查找，所以注释
			//UInt32 nextHoarStoneLv = hoarStoneLv + 1;

			//HoarstoneLvTemplate * hoarStoneLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,nextHoarStoneLv);
			
			//本级id，等级查找temp
			HoarstoneLvTemplate * hoarStoneLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,hoarStoneLv);
			if(!hoarStoneLvTemp)
			{
				//提示玩家界石等级最高了，不可以升级
				GCHoarStoneActiveResp activeResp;
				activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::HoarStoneLvLimit;

				Json::StyledWriter writer;
				std::string respStr = writer.write(root);

				activeResp.mRespJsonStr = respStr;

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(activeResp,connId);

				cout << respStr;
				return;
			}

			UInt64 nowCoin = m_pPlayer->getPlayerCoin();
			if(nowCoin < hoarStoneLvTemp->mCostCoin)
			{
				//提示玩家金币不足

				GCHoarStoneActiveResp activeResp;
				activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::CoinNotEnough;

				Json::StyledWriter writer;
				std::string respStr = writer.write(root);

				activeResp.mRespJsonStr = respStr;

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(activeResp,connId);

				cout << respStr;

				return;
			}
			
			if(mapHoarStoneIter->mValue->mPieceCount < hoarStoneLvTemp->mCostPiece)
			{
				//提示玩家碎片不足
				GCHoarStoneActiveResp activeResp;
				activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;

				Json::StyledWriter writer;
				std::string respStr = writer.write(root);

				activeResp.mRespJsonStr = respStr;

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(activeResp,connId);

				cout << respStr;
				
				return;
			}

			nowCoin -= hoarStoneLvTemp->mCostCoin;
			mapHoarStoneIter->mValue->mPieceCount -= hoarStoneLvTemp->mCostPiece;

			mapHoarStoneIter->mValue->mStoneLv = 1;


			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - hoarStoneLvTemp->mCostCoin;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog30);


			m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

			//更新界石成就
			m_pPlayer->getAchieveManager().updateAchieveData(HONORSTONEACTIVE, 1);

			GCHoarStoneActiveResp activeResp;
			activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["hoarstoneIdx"] = hoarstoneIdx;
			root["coin"] = nowCoin;
			root["piece"] = mapHoarStoneIter->mValue->mPieceCount;

			Json::StyledWriter writer;
			std::string respStr = writer.write(root);

			activeResp.mRespJsonStr = respStr;

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(activeResp,connId);

			cout << respStr;

		}
		else
		{
			//提示玩家不满足激活条件
			GCHoarStoneActiveResp activeResp;
			activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::HoarStoneCondition;

			Json::StyledWriter writer;
			std::string respStr = writer.write(root);

			activeResp.mRespJsonStr = respStr;

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(activeResp,connId);

			cout << respStr;

		}

	}
	else
	{
		//提示玩家id有问题

		GCHoarStoneActiveResp activeResp;
		activeResp.mPacketID = LynxErrno::InvalidParameter;

		Json::Value root;
		root["errorId"] = LynxErrno::HoarStoneCondition;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		activeResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(activeResp,connId);


		cout << respStr;

	}
}
UInt32 HoarStoneManager::getHoarStonePiece(UInt64 hoarStoneId)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);
	if(mapHoarStoneIter)
	{
		return mapHoarStoneIter->mValue->mPieceCount;		
	}
	else
	{
		return 0;
	}
}
bool HoarStoneManager::addHoarStonePiece(UInt64 hoarStoneId, UInt32 pieceCount)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);

	

	if(mapHoarStoneIter)
	{
		UInt32 lv = mapHoarStoneIter->mValue->mStoneLv;
		UInt32 pCount = mapHoarStoneIter->mValue->mPieceCount;
		HoarstoneLvTemplate * stonelvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId, lv);

		if(stonelvTemp == NULL)
		{
			LOG_WARN("stonelvTemp not found!!");
			return false;
		}


		//if(pCount + pieceCount > stonelvTemp->mToMax)
		//{
		//	//多余碎片变为铜钱
		//	mapHoarStoneIter->mValue->mPieceCount = stonelvTemp->mToMax;
		//	UInt32 more = pCount + pieceCount - stonelvTemp->mToMax;
		//	HoarstoneBaseTemplate * hoarBaseTemp = HOARSTONEBASE_TABLE().get(hoarStoneId);

		//	UInt64 curCoin = m_pPlayer->getPlayerCoin();
		//	curCoin += hoarBaseTemp->mExchange * more;
		//
		//	
		//}
		//else
		//{
					mapHoarStoneIter->mValue->mPieceCount += pieceCount;
		/*}*/
	
		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);




		return true;
	}
	else
	{
		return false;
	}
}

bool HoarStoneManager::setHoarStonePiece(UInt64 hoarStoneId, UInt32 pieceCount)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);

	if(mapHoarStoneIter)
	{
		mapHoarStoneIter->mValue->mPieceCount = pieceCount;
		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);
		return true;
	}
	else
	{
		return false;
	}
}




void HoarStoneManager::hoarStoneLvUp(UInt64 hoarStoneLvUpId)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneLvUpId);
	
	if(!mapHoarStoneIter)
	{
		//提示玩家界石id不正确

		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;
		return;
	}


	UInt64 hoarStoneId = mapHoarStoneIter->mValue->mHoarStoneId;
	UInt32 hoarStoneLv = mapHoarStoneIter->mValue->mStoneLv;

	UInt32 nextHoarStoneLv = hoarStoneLv + 1;

	HoarstoneLvTemplate * hoarStoneNextLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,nextHoarStoneLv);
    HoarstoneLvTemplate * hoarStoneCurLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,hoarStoneLv);

	if(!hoarStoneNextLvTemp)
	{
		//提示玩家界石达到最高等级
		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::HoarStoneLvLimit;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;
		return;

	}

	UInt64 nowCoin = m_pPlayer->getPlayerCoin();
	UInt32 &nowPieceCount = mapHoarStoneIter->mValue->mPieceCount;

	if(nowCoin < hoarStoneCurLvTemp->mCostCoin)
	{
		//提示玩家金币不足

		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;

		return;
	}	
			
	if(nowPieceCount < hoarStoneCurLvTemp->mCostPiece)
	{
		//提示玩家碎片不足
		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::MaterialNotEnough;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;

		return;
	}

	//原来的逻辑是处理多次升级，目前改为只升级一次
	UInt32 addLvel = 0;		
	/*
			while(nowPieceCount >= hoarStoneLvTemp->mCostPiece && nowCoin >= hoarStoneLvTemp->mCostCoin)
			{
				addLvel++;
				nowCoin -= hoarStoneLvTemp->mCostCoin;
				nowPieceCount -= hoarStoneLvTemp->mCostPiece;
	

				UInt32 nextLv = hoarStoneLvTemp->mLevel + 1;
				hoarStoneLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,nextLv);

				if(!hoarStoneLvTemp)
				{
					break;
				}
						
			}*/	
			
	//新版本改为强化只升级一级
	if(nowPieceCount >= hoarStoneCurLvTemp->mCostPiece && nowCoin >= hoarStoneCurLvTemp->mCostCoin)
	{
		addLvel++;
		nowCoin -= hoarStoneCurLvTemp->mCostCoin;
		nowPieceCount -= hoarStoneCurLvTemp->mCostPiece;

	}

	mapHoarStoneIter->mValue->mStoneLv += addLvel;

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_COIN;
	goods.num = 0 - hoarStoneCurLvTemp->mCostCoin;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog31);

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

	GCHoarStoneLvUpResp lvUpResp;
	lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["hoarstoneIdx"] = hoarStoneId;
	root["coin"] = nowCoin;
	root["piece"] = mapHoarStoneIter->mValue->mPieceCount;
	root["targetLevel"] = mapHoarStoneIter->mValue->mStoneLv;
	root["levelAdd"] = addLvel;

	Json::StyledWriter writer;
	std::string respStr = writer.write(root);

	lvUpResp.mRespJsonStr = respStr;

	const ConnId& connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

	cout << respStr;
	//更新界石强化日常任务
	m_pPlayer->getAchieveManager().updateDailyTaskData(DLYHORNORSTONELVUPCNT, 1 );

}

void HoarStoneManager::sendStarResp(errorId errorId,const ConnId& connId)
{
	GCHoarStoneStarResp starResp;
	starResp.mPacketID = BOC_PLAYER_HOARSTONE_STARUP_RESP;
	Json::Value root;
	root["errorId"] = errorId;
	Json::StyledWriter writer;

	starResp.mRespJsonStr = writer.write(root);
	NetworkSystem::getSingleton().sendMsg(starResp,connId);
	cout << starResp.mRespJsonStr;
}

void HoarStoneManager::hoarStoneStarUp(UInt64 hoarStoneId)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);
	UInt32 starLv = mapHoarStoneIter->mValue->mStoneStar;
	UInt32 nextStarLv = starLv + 1;
	HoarstoneStarTemplate * nextHoarStoneTemp = HOARSTONESTAR_TABLE().reverseGet(hoarStoneId, nextStarLv);
	HoarstoneStarTemplate * hoarStoneTemp = HOARSTONESTAR_TABLE().reverseGet(hoarStoneId, starLv);

	const ConnId& connId = m_pPlayer->getConnId();

	if(nextHoarStoneTemp)
	{
		//判断界石装备的符文是否满足条件
		List<UInt64> &runeList = mapHoarStoneIter->mValue->mEquipList;
		
		List<UInt64>::Iter * runeIter1 = runeList.getn(0); 
		if(runeIter1->mValue != hoarStoneTemp->needEquipId1)
		{
			//提示玩家符文不足，不满足升星条件
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
	
			return ;
		}

		List<UInt64>::Iter * runeIter2 = runeList.getn(1); 
		if(runeIter2->mValue != hoarStoneTemp->needEquipId2)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter3 = runeList.getn(2); 
		if(runeIter3->mValue != hoarStoneTemp->needEquipId3)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter4 = runeList.getn(3); 
		if(runeIter4->mValue != hoarStoneTemp->needEquipId4)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter5 = runeList.getn(4); 
		if(runeIter5->mValue != hoarStoneTemp->needEquipId5)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter6 = runeList.getn(5); 
		if(runeIter6->mValue != hoarStoneTemp->needEquipId6)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		for(List<UInt64>::Iter * runeDataIter = runeList.begin(); runeDataIter != NULL; runeDataIter = runeList.next(runeDataIter))
		{
			runeDataIter->mValue = 0;
		}

		mapHoarStoneIter->mValue->mStoneStar++;

		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

		//提示玩家升星成功

		GCHoarStoneStarResp starResp;
		starResp.mPacketID = BOC_PLAYER_HOARSTONE_STARUP_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["hoarstoneIdx"] = mapHoarStoneIter->mValue->mHoarStoneId;
		root["targetStar"] = mapHoarStoneIter->mValue->mStoneStar;
		root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

		Json::StyledWriter writer;

		starResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(starResp,connId);
		cout << starResp.mRespJsonStr;
		return;

	}
	else
	{
		//提示玩家当前星级已经满级
		sendStarResp(LynxErrno::HoarStoneLvMax,connId);
			return ;
	}
	
}

//原来升星逻辑，保留
//
//void HoarStoneManager::hoarStoneStarUp(UInt64 hoarStoneId)
//{
//	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);
//	UInt32 nextStarLv = mapHoarStoneIter->mValue->mStoneStar + 1;
//	HoarstoneStarTemplate * nextHoarStoneTemp = HOARSTONESTAR_TABLE().reverseGet(hoarStoneId, nextStarLv);
//	const ConnId& connId = m_pPlayer->getConnId();
//
//	if(nextHoarStoneTemp)
//	{
//		//判断界石装备的符文是否满足条件
//		List<UInt64> &runeList = mapHoarStoneIter->mValue->mEquipList;
//		
//		List<UInt64>::Iter * runeIter1 = runeList.getn(0); 
//		if(runeIter1->mValue != nextHoarStoneTemp->needEquipId1)
//		{
//			//提示玩家符文不足，不满足升星条件
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//	
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter2 = runeList.getn(1); 
//		if(runeIter2->mValue != nextHoarStoneTemp->needEquipId2)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter3 = runeList.getn(2); 
//		if(runeIter3->mValue != nextHoarStoneTemp->needEquipId3)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter4 = runeList.getn(3); 
//		if(runeIter4->mValue != nextHoarStoneTemp->needEquipId4)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter5 = runeList.getn(4); 
//		if(runeIter5->mValue != nextHoarStoneTemp->needEquipId5)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter6 = runeList.getn(5); 
//		if(runeIter6->mValue != nextHoarStoneTemp->needEquipId6)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		for(List<UInt64>::Iter * runeDataIter = runeList.begin(); runeDataIter != NULL; runeDataIter = runeList.next(runeDataIter))
//		{
//			runeDataIter->mValue = 0;
//		}
//
//		mapHoarStoneIter->mValue->mStoneStar++;
//
//		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);
//
//		//提示玩家升星成功
//
//		GCHoarStoneStarResp starResp;
//		starResp.mPacketID = BOC_PLAYER_HOARSTONE_STARUP_RESP;
//		Json::Value root;
//		root["errorId"] = LynxErrno::None;
//		root["hoarstoneIdx"] = mapHoarStoneIter->mValue->mHoarStoneId;
//		root["targetStar"] = mapHoarStoneIter->mValue->mStoneStar;
//		root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
//
//		Json::StyledWriter writer;
//
//		starResp.mRespJsonStr = writer.write(root);
//		NetworkSystem::getSingleton().sendMsg(starResp,connId);
//		cout << starResp.mRespJsonStr;
//		return;
//
//	}
//	else
//	{
//		//提示玩家当前星级已经满级
//	}
//	
//}

void HoarStoneManager::addHoarStoneRune(UInt64 runeId, UInt32 count)
{
	RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runeId);
	if(runeDataIter)
	{
		runeDataIter->mValue += count;
	}
	else
	{
		m_pRuneDataMap->insert(runeId,count);
	}

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

}

UInt32 HoarStoneManager::getHoarStoneRune(UInt64 runeId)
{
	RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runeId);
	if(runeDataIter)
	{
		return runeDataIter->mValue;
	}
	else
	{
		return 0;
	}

}


void HoarStoneManager::GMHoarStoneRuneSet(UInt64 runeId, UInt32 count)
{
	RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runeId);
	
	if(runeDataIter)
	{
		
			runeDataIter->mValue = count;
		
			if(!count)
			{
				m_pRuneDataMap->erase(runeDataIter);
			}
	}
	else
	{
		if(count)
		{
			m_pRuneDataMap->insert(runeId,count);
		}
		
	}

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

}


void HoarStoneManager::equipHoarStoneRune(List<runePosData> runeDataList, UInt64 hoarStoneID)
{
	Map<UInt64, HoarStoneData*>::Iter * hoarStoneIter =  mHoarStoneMap.find(hoarStoneID);
	List<UInt64> & equipList = hoarStoneIter->mValue->mEquipList;
	const ConnId& connId = m_pPlayer->getConnId();

	for(List<runePosData>::Iter * runePosIter = runeDataList.begin(); runePosIter != NULL; runePosIter = runeDataList.next(runePosIter))
	{
		RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runePosIter->mValue.runeID);
		
		if(!runeDataIter)
		{
			//提示玩家信息不对
			GCHoarStoneRuneEquipResp equipResp;
			equipResp.mPacketID = BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;

			Json::StyledWriter writer; 

			equipResp.mRespJsonStr = writer.write(root);


			NetworkSystem::getSingleton().sendMsg(equipResp,connId);

			cout << equipResp.mRespJsonStr;
			return;
		}

		if(!runeDataIter->mValue)
		{
			//提示玩家信息不对
			GCHoarStoneRuneEquipResp equipResp;
			equipResp.mPacketID = BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;

			Json::StyledWriter writer;

			equipResp.mRespJsonStr = writer.write(root);


			NetworkSystem::getSingleton().sendMsg(equipResp,connId);

			cout << equipResp.mRespJsonStr;
			return;
		}

		runeDataIter->mValue --;
		
		if(!runeDataIter->mValue)
		{
			m_pRuneDataMap->erase(runeDataIter);
		}
		
		List<UInt64>::Iter * equipIter = equipList.getn(runePosIter->mValue.runePos);
		equipIter->mValue = runePosIter->mValue.runeID;

		char dest[1024]={0};
		snprintf(dest,sizeof(dest),"%d,%d",runePosIter->mValue.runePos,runePosIter->mValue.runeID);
		LogicSystem::getSingleton().write_log(LogType93,m_pPlayer->getPlayerGuid(), dest,LogInfo);

	}

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

	GCHoarStoneRuneEquipResp equipResp;
	equipResp.mPacketID = BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["hoarstoneIdx"] = hoarStoneID;

	UInt32 id = 0;
	std::stringstream mystream;
	

	for(List<UInt64>::Iter * equipIter = equipList.begin(); equipIter != NULL; equipIter = equipList.next(equipIter) )
	{
		mystream.clear();
		mystream.str("");
		std::string idStr;
		mystream << id;
		mystream >> idStr;

		root["equipList"][idStr] = equipIter->mValue;
		id++;

		//如果没有镶嵌符文，即equipIter的value值为0,那么跳过runeRoot的构造
		if(!equipIter->mValue)
		{
			continue;
		}

		Json::Value runeRoot;
		RuneDataMap::Iter * runemapIter = m_pRuneDataMap->find(equipIter->mValue);
		
		//符文map里有这个符文
		if(runemapIter)
		{
			runeRoot["runeId"] = runemapIter->mKey;
			runeRoot["runeCount"] = runemapIter->mValue;
			root["runeList"].append(runeRoot);

			
		}
		else
		{
			//符文map里没有符文
			runeRoot["runeId"] = equipIter->mValue;
			runeRoot["runeCount"] = 0;
			root["runeList"].append(runeRoot);
		}
		

	}

	

	Json::StyledWriter writer;
	
	equipResp.mRespJsonStr = writer.write(root);

	//cout <<equipResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(equipResp,connId);

	cout << equipResp.mRespJsonStr;
	
}

bool HoarStoneManager::checkCondition(UInt64 hoarStoneIndex)
{
	HoarstoneBaseTemplate * hoarStoneBaseTemp = HOARSTONEBASE_TABLE().get(hoarStoneIndex);
	if (hoarStoneBaseTemp == NULL)
	{
		LOG_WARN("hoarStoneBaseTemp not found!!");
		return false;
	}
 
	if(hoarStoneBaseTemp->mActiveConditionType1)
	{
		 
		UInt64 activeCondition1Param1 = hoarStoneBaseTemp->mActiveCondition1Param1;
		UInt64 activeCondition1Param2 = hoarStoneBaseTemp->mActiveCondition1Param2;

	
		bool res1 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType1,activeCondition1Param1,activeCondition1Param2);
		if(res1)
		{
			if(hoarStoneBaseTemp->mActiveConditionType2)
			{
				UInt64 activeCondition2Param1 = hoarStoneBaseTemp->mActiveCondition2Param1;
				UInt64 activeCondition2Param2 = hoarStoneBaseTemp->mActiveCondition2Param2;

				bool res2 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType2,activeCondition2Param1,activeCondition2Param2);
				if(res2)
				{
					if(hoarStoneBaseTemp->mActiveConditionType3)
					{
						UInt64 activeCondition3Param1 = hoarStoneBaseTemp->mActiveCondition3Param1;
						UInt64 activeCondition3Param2 = hoarStoneBaseTemp->mActiveCondition3Param2;

						bool res3 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType3,activeCondition3Param1,activeCondition3Param2);
						if(res3)
						{
							if(hoarStoneBaseTemp->mActiveConditionType4)
							{
								UInt64 activeCondition4Param1 = hoarStoneBaseTemp->mActiveCondition4Param1;
								UInt64 activeCondition4Param2 = hoarStoneBaseTemp->mActiveCondition4Param2;

								bool res4 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType4,activeCondition4Param1,activeCondition4Param2);
								
								if(res4)
								{
									if(hoarStoneBaseTemp->mActiveConditionType5)
									{
										UInt64 activeCondition5Param1 = hoarStoneBaseTemp->mActiveCondition5Param1;
										UInt64 activeCondition5Param2 = hoarStoneBaseTemp->mActiveCondition5Param2;

										bool res5 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType5,activeCondition5Param1,activeCondition5Param2);
										if(res5)
										{
											return true;
										}
										else
										{
											return false;
										}
									}
									else
									{
										return true;
									}
								}
								else
								{
									return false;
								}
							}
							else
							{
								return true;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

void HoarStoneManager::combinHoarStoneRune(UInt64 dstRuneId,UInt32 dstRuneCount, UInt32 transferId)
{
	HoarstoneRuneTemplate *hoarStoneRuneTemp = HOARSTONERUNE_TABLE().get(dstRuneId);
	if (hoarStoneRuneTemp == NULL)
	{
		LOG_WARN("hoarStoneRuneTemp not found!!");
		return;
	}
	const ConnId& connId = m_pPlayer->getConnId();

	if(hoarStoneRuneTemp)
	{
		Map<UInt64, UInt32> runeMaterialMap;
		UInt64 rune1 = hoarStoneRuneTemp->mCombine1;
		if(rune1)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(rune1);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount1 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}
			runeMaterialMap.insert(rune1,hoarStoneRuneTemp->mAmount1 * dstRuneCount);
		}
		
		if(hoarStoneRuneTemp->mCombine2)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(hoarStoneRuneTemp->mCombine2);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount2 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;
				return;
			}
			runeMaterialMap.insert(hoarStoneRuneTemp->mCombine2,hoarStoneRuneTemp->mAmount2 * dstRuneCount);
		}
		
		if(hoarStoneRuneTemp->mCombine3)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(hoarStoneRuneTemp->mCombine3);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount3 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;
				return;
			}
			runeMaterialMap.insert(hoarStoneRuneTemp->mCombine3,hoarStoneRuneTemp->mAmount3 * dstRuneCount);
		}

		if(hoarStoneRuneTemp->mCombine4)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(hoarStoneRuneTemp->mCombine4);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount4 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;
				return;
			}

			runeMaterialMap.insert(hoarStoneRuneTemp->mCombine3,hoarStoneRuneTemp->mAmount4 * dstRuneCount);
		}

		Json::Value root;
		root["errorId"] = LynxErrno::None;
		std::stringstream mystream;

		for(Map<UInt64, UInt32>::Iter* runeMaterialIter = runeMaterialMap.begin();runeMaterialIter!=NULL;
			runeMaterialIter = runeMaterialMap.next(runeMaterialIter))
		{
			mystream.clear();
			mystream.str("");
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(runeMaterialIter->mKey);
			runeDataIter->mValue -= runeMaterialIter->mValue;
			mystream << runeDataIter->mKey;
			std::string keyStr;
			mystream >> keyStr;
			root["changeList"][keyStr] = runeDataIter->mValue;

			if(!runeDataIter->mValue)
			{
				m_pRuneDataMap->erase(runeDataIter);
			}
		}
		
		root["coin"] = m_pPlayer->getPlayerCoin();

		addHoarStoneRune(dstRuneId,dstRuneCount);
		RuneDataMap::Iter *runeDataIterNew = m_pRuneDataMap->find(dstRuneId);
		mystream.clear();
		mystream.str("");
		mystream << dstRuneId;
		std::string keyStr;
		mystream >> keyStr;
		root["changeList"][keyStr] = runeDataIterNew->mValue;

		root["transferId"] = transferId;
		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

		GCHoarStoneRuneCombResp resp;
		resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(resp,connId);

		cout << resp.mRespJsonStr;

	}
	else
	{
		//提示玩家id不正确
		GCHoarStoneRuneCombResp resp;
		resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(resp,connId);

		cout << resp.mRespJsonStr;
		return ;
	}
}


HoarStoneData * HoarStoneManager::getHoarStoneData(UInt64 hoarStoneID)
{
	Map<UInt64, HoarStoneData*>::Iter * hoarStoneIter =  mHoarStoneMap.find(hoarStoneID);
	if(hoarStoneIter)
	{
		return hoarStoneIter->mValue;
	}
	else
	{
		return NULL;
	}
}