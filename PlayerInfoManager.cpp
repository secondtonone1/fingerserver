#include "PlayerInfoManager.h"
#include "Player.h"
using namespace Lynx;

PlayerInfoManager::PlayerInfoManager()
{
	m_pPlayer = NULL;
}

PlayerInfoManager::~PlayerInfoManager()
{

}

bool PlayerInfoManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pBaseData = &player->mPlayerData.mBaseData;

	m_pDailyRestData = &player->mPlayerData.mDailyRestData;

	
	BaseInfoInitial(player);
	DetailInfoInitial(player);
	return true;
}

void PlayerInfoManager::release()
{
	m_pPlayer = NULL;
}

void PlayerInfoManager::DetailInfoInitial(Player* player)
{
	m_PlayerDetailInfo.m_pPlayerID = &player->mPlayerData.mBaseData.m_nPlayerID;
	m_PlayerDetailInfo.m_pModelID = &player->mPlayerData.mBaseData.m_nModelID;
	m_PlayerDetailInfo.m_pPlayerName = &player->mPlayerData.mBaseData.m_strPlayerName;
	m_PlayerDetailInfo.m_pMood = &player->mPlayerData.mBaseData.m_strMood;
	m_PlayerDetailInfo.m_pLevel = &player->mPlayerData.mBaseData.m_nLevel;
	m_PlayerDetailInfo.m_pExp = &player->mPlayerData.mBaseData.m_nLevelExp;
	m_PlayerDetailInfo.m_pVipLevel = &player->mPlayerData.mBaseData.m_nVipLevel;
	m_PlayerDetailInfo.m_pVipExp = &player->mPlayerData.mBaseData.m_nVipExp;
	m_PlayerDetailInfo.m_pCoin = &player->mPlayerData.mBaseData.m_nCoin;
	m_PlayerDetailInfo.m_pGold = &player->mPlayerData.mBaseData.m_nGold;
	m_PlayerDetailInfo.m_pFame= &player->mPlayerData.mBaseData.m_nFame;
	
	m_PlayerDetailInfo.m_pStrength = &player->mPlayerData.mBaseData.m_nStrength;	
	m_PlayerDetailInfo.m_pListSweepCount = &player->mPlayerData.mBaseData.m_listSweepCount;


	m_PlayerDetailInfo.m_pArenaEnterCount = &player->mPlayerData.mDailyRestData.m_nArenaEnterCount;
	m_PlayerDetailInfo.m_pAmphitheaterEnterCount = &player->mPlayerData.mDailyRestData.m_nAmphitheaterEnterCount;
	m_PlayerDetailInfo.m_pAmphitheaterWinCount = &player->mPlayerData.mDailyRestData.m_nAmphitheaterWinCount;
	m_PlayerDetailInfo.m_pAmphitheaterLoseCount = &player->mPlayerData.mDailyRestData.m_nAmphitheaterLoseCount;
	m_PlayerDetailInfo.m_pDailyChaiCount = &player->mPlayerData.mDailyRestData.m_nDailyChaiCount;
	m_PlayerDetailInfo.m_pDailyMiCount = &player->mPlayerData.mDailyRestData.m_nDailyMiCount;
	m_PlayerDetailInfo.m_pDailyYouCount = &player->mPlayerData.mDailyRestData.m_nDailyYouCount;
	m_PlayerDetailInfo.m_pDailyYanCount = &player->mPlayerData.mDailyRestData.m_nDailyYanCount;
	m_PlayerDetailInfo.m_pTwelvepalaceEnterCount = &player->mPlayerData.mDailyRestData.m_nTwelvepalaceEnterCount;
	m_PlayerDetailInfo.m_pStrTwelvepalaceData = &player->mPlayerData.mDailyRestData.m_strTwelvepalaceData;
	m_PlayerDetailInfo.m_pCouragetrialDailyPoint = &player->mPlayerData.mDailyRestData.m_nCouragetrialDailyPoint;
	m_PlayerDetailInfo.m_pStrCourageData = &player->mPlayerData.mDailyRestData.m_strCourageData;
	
	m_PlayerDetailInfo.m_pActivepoint = &player->mPlayerData.mDailyRestData.m_nActivepoint;
	

	//通过某种方式计算战力

	m_PlayerDetailInfo.m_pPower= &player->mPlayerData.mBaseData.m_nPower;

	m_PlayerDetailInfo.m_nDailyMultipleCopyCount =  &player->mPlayerData.mDailyRestData.m_nDailyMultipleCopyCount;
	m_PlayerDetailInfo.m_nTwelvePalaceUnlockCount =  &player->mPlayerData.mDailyRestData.m_nTwelvePalaceUnlockCount;

}

void PlayerInfoManager::BaseInfoInitial(Player* player)
{
	//将baseinfo中的数据和player数据关联起来，方便以后客户端频繁请求发送
	m_PlayerBaseInfo.m_pPlayerID = &player->mPlayerData.mBaseData.m_nPlayerID;

	m_PlayerBaseInfo.m_pModelID = &player->mPlayerData.mBaseData.m_nModelID;

	m_PlayerBaseInfo.m_pPlayerName = &player->mPlayerData.mBaseData.m_strPlayerName;

	m_PlayerBaseInfo.m_pMood = &player->mPlayerData.mBaseData.m_strMood;

	m_PlayerBaseInfo.m_pLevel = &player->mPlayerData.mBaseData.m_nLevel;

	m_PlayerBaseInfo.m_pVipLevel = &player->mPlayerData.mBaseData.m_nVipLevel;


	
	//剩余的根据规则进行计算
	
}

std::string PlayerInfoManager::convertBaseInfoToJson(void)
{
	 MsgBaseInfoToJson baseInfoJson;
	 
	 baseInfoJson.playerUID = *m_PlayerBaseInfo.m_pPlayerID;
	 baseInfoJson.modelID = *m_PlayerBaseInfo.m_pModelID;
	
	 baseInfoJson.level = *m_PlayerBaseInfo.m_pLevel;
	 baseInfoJson.mood = *m_PlayerBaseInfo.m_pMood;
	 baseInfoJson.playerName = *m_PlayerBaseInfo.m_pPlayerName;
	 baseInfoJson.vipLevel = *m_PlayerBaseInfo.m_pVipLevel;
	 
	 baseInfoJson.weaponResID = 0;
	 baseInfoJson.fashionResID = 0;
	 baseInfoJson.power = 100;

	 return baseInfoJson.convertDataToJson();
}

std::string PlayerInfoManager::convertDetailInfoToJson(void)
{
	MsgDetailInfoToJson detailInfoJson;

	detailInfoJson.activepoint = * (m_PlayerDetailInfo.m_pActivepoint);
	detailInfoJson.amphitheaterEnterCount = * (m_PlayerDetailInfo.m_pAmphitheaterEnterCount);
	detailInfoJson.amphitheaterLoseCount = * (m_PlayerDetailInfo.m_pAmphitheaterLoseCount);
	detailInfoJson.amphitheaterWinCount = * (m_PlayerDetailInfo.m_pAmphitheaterWinCount);
	detailInfoJson.arenaEnterCount = * (m_PlayerDetailInfo.m_pArenaEnterCount);
	detailInfoJson.courageData = * (m_PlayerDetailInfo.m_pStrCourageData);
	detailInfoJson.couragetrialDailyPoint = * ( m_PlayerDetailInfo.m_pCouragetrialDailyPoint);
	detailInfoJson.dailyChaiCount = *(m_PlayerDetailInfo.m_pDailyChaiCount);
	detailInfoJson.dailyMiCount = *(m_PlayerDetailInfo.m_pDailyMiCount);
	detailInfoJson.dailyYanCount = *(m_PlayerDetailInfo.m_pDailyYanCount);
	detailInfoJson.dailyYouCount = *(m_PlayerDetailInfo.m_pDailyYouCount);
	detailInfoJson.exp = *(m_PlayerDetailInfo.m_pExp);
	detailInfoJson.gold = *(m_PlayerDetailInfo.m_pGold);
	detailInfoJson.level = *(m_PlayerDetailInfo.m_pLevel);
	detailInfoJson.listSweepCount = *(m_PlayerDetailInfo.m_pListSweepCount);
	detailInfoJson.modelID = *(m_PlayerDetailInfo.m_pModelID);
	detailInfoJson.coin = *(m_PlayerDetailInfo.m_pCoin);
	detailInfoJson.fame = *(m_PlayerDetailInfo.m_pFame);
	detailInfoJson.mood = *(m_PlayerDetailInfo.m_pMood);
	detailInfoJson.playerName = *(m_PlayerDetailInfo.m_pPlayerName);
	detailInfoJson.playerUID = *(m_PlayerDetailInfo.m_pPlayerID);
	detailInfoJson.power = *m_PlayerDetailInfo.m_pPower;
	detailInfoJson.strength = *(m_PlayerDetailInfo.m_pStrength);
	detailInfoJson.twelvepalaceData = *(m_PlayerDetailInfo.m_pStrTwelvepalaceData);
	detailInfoJson.twelvepalaceEnterCount = *(m_PlayerDetailInfo.m_pTwelvepalaceEnterCount);
	detailInfoJson.vipExp = *(m_PlayerDetailInfo.m_pVipExp);
	detailInfoJson.vipLevel = *(m_PlayerDetailInfo.m_pVipLevel);
	detailInfoJson.oldName = m_pPlayer->mPlayerData.mBaseData.m_strOldName;
		
	detailInfoJson.dailyMultipleCopyCount = *(m_PlayerDetailInfo.m_nDailyMultipleCopyCount);

	detailInfoJson.twelvePalaceUnlockCount = *(m_PlayerDetailInfo.m_nTwelvePalaceUnlockCount);

	std::string jsonStr = detailInfoJson.convertDataToJson();
	
	return jsonStr;
}

void PlayerInfoManager::baseInfoSync()
{
	//进行m_pPower， resid，fashionid的同步
}

void PlayerInfoManager::detailInfoSync()
{
	//进行m_pPower
}


UInt64 PlayerInfoManager::getPlayerGold(void)
{
	return m_pBaseData->m_nGold;
}

void  PlayerInfoManager::setPlayerGold(UInt64 gold)
{
	m_pBaseData->m_nGold = gold;
	//设置标志位，方便存盘操作
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);
}

PlayerBaseData* PlayerInfoManager::getPlayerBaseData(void)
{
	return m_pBaseData;
}


std::string PlayerInfoManager::rename(std::string newname)
{
	m_pBaseData->m_strOldName = m_pBaseData->m_strPlayerName;
	m_pBaseData->m_strPlayerName = newname;
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

	return m_pBaseData->m_strOldName;
}