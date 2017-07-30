#include "PersistWorker.h"
#include "ConfigParser.h"

#include "TimeManager.h" 
#include "Player.h"
#include "LogicSystem.h"
#include "RankSystem.h"
#include "../FireConfirm/PVPSystem.h"
#include "../Ranking/Ranking.h"
#include "../FireConfirm/StageCalc.h"
#include "../FireConfirm/Shop.h"
#include "../FireConfirm/InlineActivity.h"
#include "ServerData.h"
#include "../FireConfirm/RankGame.h"
using namespace Lynx;

PersistWorker::PersistWorker()
{

}

PersistWorker::~PersistWorker()
{

}

bool 
PersistWorker::initial(UInt32 index)
{
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerDataReq, PersistWorker::onPersistLoadPlayerDataReq);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistSyncBaseGuidNotify, PersistWorker::onPersistSyncBaseGuidNotify);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerGuidReq, PersistWorker::onPersistLoadPlayerGuidReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistManagerSetDirtyReq, PersistWorker::onPersistManagerSetDirtyReq);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistNotifyWorkerSaveDbReq, PersistWorker::onPersistNotifyWorkerSaveDbReq);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistOffLineSaveReq, PersistWorker::onPersistOffLineSaveReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistPlayerSinglePropertySaveReq, PersistWorker::onPersistUpdateSingleProperty);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistFindSimilarPowerReq, PersistWorker::onPersistFindSimilarPower);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistServerDataReq, PersistWorker::onPersistServerDataReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistServerDailyDataReq, PersistWorker::onPersistServerDailyDataReq);


	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistRobotDataReq, PersistWorker::onPersistRobotDataReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistInlineActivityReq, PersistWorker::onPersistInlineActivityPower);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistFireConfirmSaveReq, PersistWorker::onPersistUpdataFireConfirmData);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistCreatePlayerReq, PersistWorker::onPersistCreatePlayerReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerByAccount, PersistWorker::onLoadPlayerByAccount);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddJewelryNotify, PersistWorker::onPersistJewelryAddReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistRankingSaveReq, PersistWorker::onPersistUpdateStageRanking);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistShopSaveReq, PersistWorker::onPersistUpdateShop);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistShopItemSaveReq, PersistWorker::onPersistUpdateShopItem);

	REGISTER_THREAD_MSG(mThreadMsgHandler, CatCoinSaveReq, PersistWorker::onCatCoinSaveReq);
	REGISTER_THREAD_MSG(mThreadMsgHandler, CoinGroupSaveReq, PersistWorker::onCoinGroupSaveReq);

		
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistOfflineDataReq, PersistWorker::onPersistLoadOfflineData);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelJewelryNotify, PersistWorker::onPersistJewelryDelReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateJewelryNotify, PersistWorker::onPerisitJewelryUpdateReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateJewelryEquipNotify, PersistWorker::onPerisitJewelryEquipUpdateReq);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddServantNotify, PersistWorker::onPersistServantAddReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateServantNotify, PersistWorker::onPersistServantUpdateReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelServantNotify, PersistWorker::onPersistServantDelReq);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddServantTreasureNotify, PersistWorker::onPersistServantTreasureAddReq);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateServantTreasureNotify, PersistWorker::onPersistServantTreasureUpdateReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateFriendNotify, PersistWorker::onPersistFriendUpdate);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateFriendOffLineNotify, PersistWorker::onPersistFriendOffLineUpdate);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddFriendOffLineNotify, PersistWorker::onPersistFriendOffLineAdd);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelFriendOffLineNotify, PersistWorker::onPersistFriendOffLineDel);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateLeaveTime, PersistWorker::onPersistLeaveTime);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistResetAllLeaveTime, PersistWorker::onPersistAllLeaveTime);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PerisistUpdateOnlineLvRank,  PersistWorker::onLoadPlayerOnlineByLv);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGetPowerRank, PersistWorker::onLoadPlayerPowerRank);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGetScoreRank, PersistWorker::onLoadPlayerScoreRank);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistRankGameScoreSave, PersistWorker::onRankGameScoreSave);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateFriendBeApplyOffLineDataToDb, PersistWorker::onPersistUpdateFriendBeApplyOffLineDataToDb);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistRankGameReportSave, PersistWorker::onRankGameReportSave);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistOnlineDaySave,  PersistWorker::onPersistOnlineDaySave);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistBaseDateSave,  PersistWorker::onPersistBaseDateSave);
	

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistPlayerConsortUpdate, PersistWorker::onLoadPlayerConsortInfo);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistPlayerConsortSave, PersistWorker::onSavePlayerConsort);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortSave, PersistWorker::onSaveConsort);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortCreate, PersistWorker::onCreateConsort);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyInsert, PersistWorker::onConsortApplyInsert);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyDel, PersistWorker::onConsortApplyDel);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyClear, PersistWorker::onConsortApplyClear);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyAllClear, PersistWorker::onConsortApplyAllClear);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortDel, PersistWorker::onConsortDel);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortLogInsert, PersistWorker::onConsortLogInsert);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortSignReset, PersistWorker::onConsortSignReset);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortSignUpdate, PersistWorker::onConsortSignUpdate);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortActiveUpdate, PersistWorker::onConsortActiveUpdate);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortLoyalUpdate, PersistWorker::onConsortLoyalUpdate);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortKitQueUpdate, PersistWorker::onKitQueTimesUpdate);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistBusinessCatUpdate, PersistWorker::onBusinessTimesUpdate);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistBusinessCatTimeReset, PersistWorker::onBusinessTimeReset);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistEyeTimesUpdate, PersistWorker::onEyeTimesUpdate);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketTimeReset, PersistWorker::onTicketTimeReset);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketFriendUpdate, PersistWorker::onTicketFriendUpdate);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketQualityUpdate, PersistWorker::onTicketQualityUpdate);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketFriendInit, PersistWorker::onTicketFriendInit);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketAllDel, PersistWorker::onTicketAllDel);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketDataUpdate, PersistWorker::onTicketDataUpdate);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketSupport, PersistWorker::onTicketSupportUpdate);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketAwardAdd, PersistWorker::onTicketAwardAdd);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketAwardUpdate, PersistWorker::onTicketAwardUpdate);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketTimesUpdate, PersistWorker::onTicketTimesUpdate);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketAwardDel, PersistWorker::onTicketAwardDel);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistTicketTimesReset, PersistWorker::onTicketTimesReset);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoyalTimesReset, PersistWorker::onLoyalTimesReset);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistKitchenTimesReset, PersistWorker::onKitchenTimesReset);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistEyeTimesReset, PersistWorker::onEyeTimesReset);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistEloquenceTimesUpdate, PersistWorker::onEloquenceUpdate);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistEloquenceTimesReset, PersistWorker::onEloquenceReset);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistWoodTimesReset, PersistWorker::onWoodTimesReset);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistWoodTotalUpdate, PersistWorker::onWoodTotalUpdate);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistWoodTotalReset, PersistWorker::onWoodTotalReset);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistWoodSelfUpdate, PersistWorker::onWoodSelfUpdate);
	
	//
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddEmail, PersistWorker::onAddEmail);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelEmail, PersistWorker::onDelEmail);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateEmail, PersistWorker::onUpdateEmail);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistClearEmail, PersistWorker::onClearEmail);

	REGISTER_THREAD_MSG(mThreadMsgHandler, FashionAdd, PersistWorker::onFashionAdd);
	REGISTER_THREAD_MSG(mThreadMsgHandler, CharactorAdd, PersistWorker::onCharactorInsert);
	REGISTER_THREAD_MSG(mThreadMsgHandler, CharactorUpdate, PersistWorker::onCharactorUpdate);

	REGISTER_THREAD_MSG(mThreadMsgHandler, AchieveUpdateMsg, PersistWorker::onPersistUpdateAchDb);
	REGISTER_THREAD_MSG(mThreadMsgHandler, OffLineFriendUpdateMsg,  PersistWorker::updateAchOfflineFriendCnt);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistDailyTaskUpdateMsg, PersistWorker::onPersistUpdateDailyTaskDb);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistDailyActiveUpdateMsg, PersistWorker::onPersistUpdateDailyActiveDb);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistDailyLastTimeUpdateMsg, PersistWorker::onPersistUpdateActiveTimeDb);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGuidStepUpdateMsg, PersistWorker::onPersistUpdateNewGuid);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGuidFlagUpdateMsg, PersistWorker::onPersistUpdateGuidFlag);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistForbidChatUpdate, PersistWorker::onPersistUpdateForbidChat);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistForbidLoginTimeUpdate, PersistWorker::onPersistUpdateForbidLoginTime);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetTongbao, PersistWorker::onPersistGMResetTongbaoReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetMengchu, PersistWorker::onPersistGMResetMengchuReq);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetYushiGF, PersistWorker::onPersistGMResetYushiGFReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetJiShiAB, PersistWorker::onPersistGMResetJishiABReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetBashanSL, PersistWorker::onPersistGMResetBashanSLReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetWXTZ, PersistWorker::onPersistGMResetWuxianTZReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMCourage, PersistWorker::onPersistGMResetCourage);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMPaiWei, PersistWorker::onPersistGMResetPaiWei);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMPaiWeiBuy, PersistWorker::onPersistGMResetPaiWeiBuy);
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGM12HaoJiao, PersistWorker::onPersistGMReset12HaoJiao);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMCookFood, PersistWorker::onPersistGMResetFoodCook);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMServantOnce, PersistWorker::onPersistGMResetServantCall1);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMServantTen, PersistWorker::onPersistGMResetServantCall10);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetDailyTask, PersistWorker::onPersistGMResetDailyTask);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetMonthSign, PersistWorker::onPersistGMMonthSign);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetSevenday, PersistWorker::onPersistGMSevenDay);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetSeventrain, PersistWorker::onPersistGMSevenTrain);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetGrowfound, PersistWorker::onPersistGMResetGrowFound);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetFenshi, PersistWorker::onPersistGMResetFenshi);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetOnlineAward, PersistWorker::onPersistGMResetOnlineAward);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetBaiCaiShen, PersistWorker::onPersistGMResetBaiCaiShen);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetBuyStrength, PersistWorker::onPersistGMResetStrengthBuy);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetLianPu, PersistWorker::onPersistGMResetLianPu);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetTB, PersistWorker::onPersistGMResetTB);

	

	

	String ip = ConfigParser::getSingleton().getMainDbIp();
	UInt16 port = ConfigParser::getSingleton().getMainDbPort();
	String username = ConfigParser::getSingleton().getMainDbUsername();
	String password = ConfigParser::getSingleton().getMainDbPassword();
	String dbname = ConfigParser::getSingleton().getMainDbName();

	if (!mDBInterface.initial(ip.c_str(), port, username.c_str(),
		password.c_str(), dbname.c_str()))
	{
		LOG_WARN("Failed to connect mysql server. [%s %s %s %s]",
			ip.c_str(), username.c_str(), password.c_str(), dbname.c_str());
		return false;
	}

	mWorkerIndex = index + 1;
	startup();

	LOG_INFO("Start PersistWorker [%u]", mWorkerIndex);



	return true;
}

void 
PersistWorker::release()
{
	shutdown();
	join();

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerDataReq);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistSyncBaseGuidNotify);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerGuidReq);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistManagerSetDirtyReq);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistNotifyWorkerSaveDbReq);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistOffLineSaveReq);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistCreatePlayerReq);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddJewelryNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelJewelryNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateJewelryNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateJewelryEquipNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddServantNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateServantNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelServantNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddServantTreasureNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateServantTreasureNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerByAccount);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateFriendOffLineNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateFriendNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateLeaveTime);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddFriendOffLineNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelFriendOffLineNotify);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PerisistUpdateOnlineLvRank);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistGetPowerRank);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistGetScoreRank);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistRankGameScoreSave	);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateFriendBeApplyOffLineDataToDb	);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistRankGameReportSave);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistOnlineDaySave);

 	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistBaseDateSave);


	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistPlayerConsortUpdate);
	
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistPlayerConsortSave);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortSave);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortCreate);


	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistAddEmail);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistDelEmail);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistUpdateEmail);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistClearEmail);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, FashionAdd);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, CharactorAdd);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, CharactorUpdate);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, AchieveUpdateMsg);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, OffLineFriendUpdateMsg);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistDailyTaskUpdateMsg);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistDailyActiveUpdateMsg);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistDailyLastTimeUpdateMsg);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyInsert);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyDel);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyClear);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortApplyAllClear);
	
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortDel);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortLogInsert);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortSignReset);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortSignUpdate);
	
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortLoyalUpdate);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortActiveUpdate);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistConsortKitQueUpdate);
	
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistBusinessCatUpdate);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistBusinessCatTimeReset);


	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistGuidStepUpdateMsg);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistGuidFlagUpdateMsg);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistResetAllLeaveTime);
	
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistForbidChatUpdate);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistForbidLoginTimeUpdate);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetTongbao);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistGMResetMengchu);
	
	

	mDBInterface.release();
	LOG_INFO("Shutdown PersistWorker [%u]", mWorkerIndex);
}

void 
PersistWorker::onThreadStarted()
{

}

void 
PersistWorker::onThreadShutdown()
{

}

void 
PersistWorker::onPreUpdate()
{

}

void 
PersistWorker::onPostUpdate()
{

}

void combineInt(List<UInt32> &IDs)
{
	UInt32 getIt = 0;
	List<UInt32> tmpIDs;

	for(List<UInt32>::Iter * it =  IDs.begin();it != NULL;it = IDs.next(it))
	{
		getIt = 0;
		for(List<UInt32>::Iter * iter =  tmpIDs.begin();iter != NULL;iter = tmpIDs.next(iter))
		{
			if (it->mValue == iter->mValue)
			{
				getIt = 1;
				break;
			}
		}
		if (getIt == 0)
		{
			tmpIDs.insertTail(it->mValue);
		}
	}
	IDs.clear();
	IDs = tmpIDs;
	
}

bool PersistWorker::updateResetStagesToDb()
{
	bool result;
	char sql[4096];	
	List<NeedSaveResetStages> resetStages = StageCalcManager::getSingleton().getNeedSaveResetStages();

	for (List<NeedSaveResetStages>::Iter * iter = resetStages.begin();iter != NULL;iter = resetStages.next(iter))
	{
		snprintf(sql, sizeof(sql), "replace into resetstages values(%llu,%u,%u)",
			iter->mValue.playerID,iter->mValue.stageID,iter->mValue.val);


		LOG_DEBUG("Sql:%s", sql);

		result = mDBInterface.execSql(sql);


		if (!result)
		{
			LOG_INFO("deal resetstages fail playerUid: %llu",iter->mValue.playerID);
		}
		else
		{
			LOG_INFO("deal resetstages success playerUid: %llu",iter->mValue.playerID);
		}
	}
	StageCalcManager::getSingleton().resetNeedSaveResetStages();
	
	return result;
}
UInt16 PersistWorker::deleteResetStages()
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call Resetstages_Delete");


	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	mDBInterface.freeResult(&rs);

	return LynxErrno::None;

}
UInt16 PersistWorker::loadResetStages()
{
	UInt64 playerID;
	UInt32 stageID;
	UInt32 count;

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM  resetstages");

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::None;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);



	while(row)
	{
		playerID = lynxAtoi<UInt64>(row[0]);
		stageID = lynxAtoi<UInt32>(row[1]);
		count = lynxAtoi<UInt32>(row[2]);
		StageCalcManager::getSingleton().addResetStages(playerID,stageID,count,false);

		row = mDBInterface.fetchRow(rs);
		
	}


	LOG_INFO("Load ResetStages  from DB successful!");
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}	

bool PersistWorker::updateStageDataToDb(UInt64 playerUid)
{
	bool result = true;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	List<UInt32> stageIDs = player->getStageManager().getNeedSaveDateList();
	player->getStageManager().resetNeedSaveDateList();
	combineInt(stageIDs);

	for (List<UInt32>::Iter *iter = stageIDs.begin();iter != NULL;iter = stageIDs.next(iter) )
	{
		result = updateStageData(playerUid,StageManager::getSingleton().getChapterID(iter->mValue),iter->mValue);	
	}
	return result;
}

bool PersistWorker::updateLotteryDataToDb(UInt64 playerUid)
{
	char sql[4096];	
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", playerUid);
		return false;
	}
	PlayerCounterData counterData;
	counterData = player->GetPlayerCounterData();
	
	snprintf(sql, sizeof(sql), "REPLACE INTO  lottery VALUES('%llu','%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u','%u', '%u', '%u','%u', '%u')",
		playerUid,counterData.m_RewardLotteryOnceTime,counterData.m_RewardLotteryTenTime,counterData.m_RewardLotteryOnceTicket,counterData.m_RewardLotteryTenTicket,counterData.m_RewardLotteryOnceFreeCount,
		counterData.m_RewardLotteryOnceTicketCount,counterData.m_RewardLotteryOnceCount,counterData.m_RewardLotteryTenFreeCount,counterData.m_RewardLotteryTenTicketCount,
		counterData.m_RewardLotteryTenCount,counterData.m_RewardLotteryVipDefaultCount,counterData.m_RewardLotteryVipAwardID,counterData.m_RewardLotteryVipElseCount);


	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);


	if (!result)
	{
		LOG_INFO("deal Lottery_Update fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal Lottery_Update success playerUid: %llu",playerUid);
	}
	return result;
}



UInt16 PersistWorker::loadCourageChallengeData(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM courage_challenge WHERE PlayerUid = %llu", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::None;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);


	while(row)
	{
		playerData.mCourageChallengeData.m_LeftChallengeTimes = lynxAtoi<UInt32>(row[1]);
		playerData.mCourageChallengeData.m_BeginTime = lynxAtoi<UInt32>(row[2]);
		playerData.mCourageChallengeData.m_BuyTime = lynxAtoi<UInt32>(row[3]);
		playerData.mCourageChallengeData.m_BuyTimes = lynxAtoi<UInt32>(row[4]);
		playerData.mCourageChallengeData.monsterID1 = lynxAtoi<UInt32>(row[5]);
		playerData.mCourageChallengeData.monsterID2 = lynxAtoi<UInt32>(row[6]);
		playerData.mCourageChallengeData.monsterID3 = lynxAtoi<UInt32>(row[7]);
		playerData.mCourageChallengeData.contentID1 = lynxAtoi<UInt32>(row[8]);
		playerData.mCourageChallengeData.contentID2 = lynxAtoi<UInt32>(row[9]);
		playerData.mCourageChallengeData.contentID3 = lynxAtoi<UInt32>(row[10]);
		playerData.mCourageChallengeData.m_LightOfLife = lynxAtoi<UInt32>(row[11]);
		playerData.mCourageChallengeData.m_RefreshTimes = lynxAtoi<UInt32>(row[12]);
		playerData.mCourageChallengeData.m_RefreshTime = lynxAtoi<UInt32>(row[13]);
		playerData.mCourageChallengeData.m_LuckyValues1 = lynxAtoi<Int32>(row[14]);
		playerData.mCourageChallengeData.m_LuckyValues2 = lynxAtoi<Int32>(row[15]);
		playerData.mCourageChallengeData.m_LuckyValues3 = lynxAtoi<Int32>(row[16]);
		
		row = mDBInterface.fetchRow(rs);
	}


	LOG_INFO("Load ResetStages  from DB successful!");
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}	



bool PersistWorker::updateCourageChallengeDataToDb(UInt64 playerUid)
{
	char sql[4096];	
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", playerUid);
		return false;
	}

	PlayerCourageChallengeData courageChallengeData;
	courageChallengeData = player->getCourageChallengeData();



	snprintf(sql, sizeof(sql), "REPLACE INTO  courage_challenge VALUES(%llu,%u,%u,%u,%u,%u,%u,%u,%u ,%u,%u,%u,%u ,%d,%d,%d,%d)",playerUid,
		courageChallengeData.m_LeftChallengeTimes,
		courageChallengeData.m_BeginTime,
		courageChallengeData.m_BuyTime,
		courageChallengeData.m_BuyTimes,
		courageChallengeData.monsterID1,
		courageChallengeData.monsterID2,
		courageChallengeData.monsterID3,
		courageChallengeData.contentID1,
		courageChallengeData.contentID2,
		courageChallengeData.contentID3,
		courageChallengeData.m_LightOfLife,
		courageChallengeData.m_RefreshTimes,
		courageChallengeData.m_RefreshTime,
		courageChallengeData.m_LuckyValues1,
		courageChallengeData.m_LuckyValues2,
		courageChallengeData.m_LuckyValues3
		);	


	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);


	if (!result)
	{
		LOG_INFO("deal Lottery_Update fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal Lottery_Update success playerUid: %llu",playerUid);
	}
	return result;
}




bool PersistWorker::updateFoodDataToDb(UInt64 playerUid)
{
	char sql[4096];	
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", playerUid);
		return false;
	}
	PlayerFoodsData foodsData;
	foodsData = player->getFoodsData();
	snprintf(sql, sizeof(sql), "call Food_Update('%u', '%u', '%u', '%u', '%u','%u','%u','%u', '%u', '%u', '%u','%u','%llu')",
		foodsData.beginTime,foodsData.leftTimes,foodsData.food1,foodsData.food2,foodsData.food3,foodsData.food11,foodsData.food12,foodsData.food13,
		foodsData.vipLeftTimes,foodsData.buyTime,foodsData.vipFoodLeftNumber,foodsData.resetState,playerUid);


	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

// 	UInt64 time1 = TimeUtil::getLocalTimeMilliSec();
// 	for(UInt32 i =0;i<1000;i++)
// 	{
// 	 result = mDBInterface.execSql(sql);
// 	
// 	}
// 	UInt64 time2 = TimeUtil::getLocalTimeMilliSec();
// 	UInt64 timeinv = time2 - time1;//32508
	if (!result)
	{
		LOG_INFO("deal Food_Update fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal Food_Update success playerUid: %llu",playerUid);
	}
	return result;
}


bool PersistWorker::updateFriendBeApplyDataToDb(UInt64 playerUid)
{
	char tmp[64];
	char sql[4096];	
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", playerUid);
		return false;
	}

	List<BaseInfoData>friendBeApplyList = player->getFriendBlackManager().getFriendApplyList();
	if (friendBeApplyList.size() ==0)
	{
		return true;
	}


	snprintf(sql, sizeof(sql), "REPLACE INTO  friend_be_apply_list VALUES(%llu",playerUid);	
	for (List<BaseInfoData>::Iter * iter = friendBeApplyList.begin();iter != NULL;iter = friendBeApplyList.next(iter))
	{
		snprintf(tmp, sizeof(tmp), ",%llu",iter->mValue.playerUid);	
		strcat(sql,tmp);
	}
	for (UInt32 i = friendBeApplyList.size();i<=50;i++)
	{
		snprintf(tmp, sizeof(tmp), ",%llu",(UInt64)0);	
		strcat(sql,tmp);
	}



	strcat(sql,")");

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	if (!result)
	{
		LOG_INFO("deal friend_be_apply_list fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal friend_be_apply_list success playerUid: %llu",playerUid);
	}
	return result;
}



void PersistWorker::onPersistUpdateFriendBeApplyOffLineDataToDb(PersistUpdateFriendBeApplyOffLineDataToDb &msg)
{
	char tmp[64];
	char sql[4096];	

	snprintf(sql, sizeof(sql), "REPLACE INTO  friend_be_apply_list VALUES(%llu",msg.m_nPlayerUid);	
	for (List<UInt64>::Iter * iter =  msg.friendBeApplyGuidList.begin();iter != NULL;iter =  msg.friendBeApplyGuidList.next(iter))
	{
		snprintf(tmp, sizeof(tmp), ",%llu",iter->mValue);	
		strcat(sql,tmp);
	}



	strcat(sql,")");

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	if (!result)
	{
		LOG_INFO("deal friend_be_apply_list fail playerUid: %llu",msg.m_nPlayerUid);
	}
	else
	{
		LOG_INFO("deal friend_be_apply_list success playerUid: %llu",msg.m_nPlayerUid);
	}
}


bool PersistWorker::updateStrengthDataToDb(UInt64 playerUid)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return false;
	}

	char sql[4096];	

	if(!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", playerUid);
		return false;
	}
	PlayerStrengthData StrengthData;
	StrengthData = player->getStrengthData();
	snprintf(sql, sizeof(sql), "call Strength_Update('%u', '%u', '%u', '%u',  '%u', '%u','%llu')",
		StrengthData.beginTime,0,StrengthData.strength,StrengthData.vipLeftTimes,StrengthData.buyTime,StrengthData.vipFoodLeftNumber,playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		LOG_INFO("deal Strength_Update fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal Strength_Update success playerUid: %llu",playerUid);
	}
	return result;
}


bool PersistWorker::updateTowerDataToDb(UInt64 playerUid)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return false;
	}

	PlayerTowerData mTowerData = player->getPlayerTowerData();
	char sql[4096];	

	snprintf(sql, sizeof(sql), "REPLACE INTO  towerData VALUES(%llu,%u,%u,%u,%u)",playerUid,mTowerData.m_HighID,mTowerData.m_Score,mTowerData.m_AP,mTowerData.m_HP);	
	

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		LOG_INFO("deal towerData fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal towerData success playerUid: %llu",playerUid);
	}
	return result;
}


bool PersistWorker::updateLocalDataToDb(UInt64 playerUid)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return false;
	}
	List<KeyValue> m_IDToSelect = player->getPlayerLocalData().m_IDToSelect;
	char tmp[32];
	char sql[4096];	

	snprintf(sql, sizeof(sql), "REPLACE INTO  localData VALUES(%llu,%u",playerUid,LOCAL_DATA_SELECT);	
	for (List<KeyValue>::Iter * iter = m_IDToSelect.begin();iter != NULL;iter = m_IDToSelect.next(iter))
	{
		snprintf(tmp, sizeof(tmp), ",%u,%u",iter->mValue.key,iter->mValue.value);	
		strcat(sql,tmp);
	}
	if (m_IDToSelect.size() < 20)//补全20个数据
	{
		for (UInt32 i = 0;i<(20 - m_IDToSelect.size());i++ )
		{
			snprintf(tmp, sizeof(tmp), ",%u,%u",0,0);	
			strcat(sql,tmp);
		}
	}
	

	strcat(sql,")");

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		LOG_INFO("deal localData fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal localData success playerUid: %llu",playerUid);
	}
	return result;
}
bool PersistWorker::updateChapterDataToDb(UInt64 playerUid)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return false;
	}
	List<KeyValue> m_ChapterUnlockeded = player->getChapterUnlocked();
	char tmp[32];
	char sql[4096];	

	snprintf(sql, sizeof(sql), "REPLACE INTO chapter VALUES(%llu,%u",playerUid,CHAPTER_DATA_LOCK);	
	for (List<KeyValue>::Iter * iter = m_ChapterUnlockeded.begin();iter != NULL;iter = m_ChapterUnlockeded.next(iter))
	{
		snprintf(tmp, sizeof(tmp), ",%u,%u",iter->mValue.key,iter->mValue.value);		
		strcat(sql,tmp);
	}
	strcat(sql,")");

	
	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		LOG_INFO("deal chapter_update fail playerUid: %llu",playerUid);
	}
	else
	{
		LOG_INFO("deal chapter_update success playerUid: %llu",playerUid);
	}
	return result;
}
bool PersistWorker::updateBoxCounterToDb(UInt64 playerUid)
{
// 	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
// 	List<KeyValue> m_ChapterUnlockeded = player->getChapterUnlocked();
// 	char tmp[32];
// 	char sql[4096]
// 	for (List<KeyValue>::Iter * iter = m_ChapterUnlockeded.begin();iter != NULL;iter = m_ChapterUnlockeded.next(iter))
// 	{
// 		snprintf(tmp, sizeof(tmp), "%u,%u", 
// 			iter->mValue.key,iter->mValue.value		
// 			);		
// 		strncpy(sql,tmp,sizeof(tmp));
// 	}
// 
// 	snprintf(sql, sizeof(sql), "UPDATE copy_Counter VALUES(%s)WHERE Playerguid  = %llu AND bigtype = %u", 
// 		sql	,playerUid,LOCAL_DATA_SELECT
// 		);
// 
// 	LOG_DEBUG("Sql:%s", sql);
// 
// 	bool result = mDBInterface.execSql(sql);
// 	if (!result)
// 	{
// 		LOG_INFO("deal gem_update fail playerUid: %llu",playerUid);
// 	}
// 	else
// 	{
// 		LOG_INFO("deal gem_update success playerUid: %llu",playerUid);
// 	}
// 	return result;
	return true;
}

UInt16 PersistWorker::loadPlayerChapterData(PlayerData& playerData,UInt64 playerGuid)
{
	KeyValue keyValue;
	List<KeyValue> m_ChapterUnlockeded;
	
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM chapter WHERE playerguid = %llu", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		
		if (lynxAtoi<UInt32>(row[1]) == CHAPTER_DATA_LOCK)
		{
			for(UInt32 i =2;i<42;i+=2)
			{
				keyValue.key = lynxAtoi<UInt32>(row[i]);
				keyValue.value = lynxAtoi<UInt32>(row[i+1]);
				m_ChapterUnlockeded.insertTail(keyValue);

			}
		
			playerData.mChapterListData.m_ChapterUnlocked = m_ChapterUnlockeded;
		}
		//继续添加
		
		

		row = mDBInterface.fetchRow(rs);
	}
	


	LOG_INFO("Load m_ChapterUnlocked %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());

	mDBInterface.freeResult(&rs);

	return LynxErrno::None;
}


UInt16 PersistWorker::loadPlayerLocalData(PlayerData& playerData,UInt64 playerGuid)
{
	KeyValue keyValue;
	
	List<KeyValue> m_IDToSelect;

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM localData WHERE playerguid = %llu", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{

		if (lynxAtoi<UInt32>(row[1]) == LOCAL_DATA_SELECT)
		{
			for(UInt32 i =2;i<42;i+=2)
			{
				keyValue.key = lynxAtoi<UInt32>(row[i]);
				keyValue.value = lynxAtoi<UInt32>(row[i+1]);
				m_IDToSelect.insertTail(keyValue);

			}
			playerData.mLocalData.m_IDToSelect = m_IDToSelect;
		}
		//继续添加

		row = mDBInterface.fetchRow(rs);
	}


	LOG_INFO("Load m_IDToSelect %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());

	mDBInterface.freeResult(&rs);

	return LynxErrno::None;
}










void 
PersistWorker::onDispatchMsg(ThreadMsg& threadMsg)
{
	mThreadMsgHandler.onDispatchMsg(threadMsg);
}

UInt16 PersistWorker::loadPlayerDailyResetData(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM playerdailyresetdata WHERE playerguid = (%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}









	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	UInt64 dbLastUpdateTime = lynxAtoi<UInt64>(row[12]);
	playerData.mDailyRestData.m_nTwelvePalaceResetFlag = dbLastUpdateTime;

	bool resTodayDelay4hours =TimeManager::timeIsTodayDelayHours(dbLastUpdateTime,4);
	bool resTodayDelay5hours =TimeManager::timeIsTodayDelayHours(dbLastUpdateTime,5);
	bool resTodayDelay6hours =TimeManager::timeIsTodayDelayHours(dbLastUpdateTime,6);

	playerData.mDailyRestData.m_nDailyMultipleCopyCount = lynxAtoi<UInt32>(row[13]);
	playerData.mDailyRestData.m_nTwelvePalaceUnlockCount = lynxAtoi<UInt32>(row[14]);

	if (resTodayDelay4hours)
	{
	
// 		 lynxAtoi<UInt32>(row[15]);
	}	
	else
	{
		




	}

	playerData.mDailyRestData.m_nCoinBuyTimes = lynxAtoi<UInt32>(row[19]);

	playerData.mDailyRestData.m_nCoinFreeBuyTimes = lynxAtoi<UInt32>(row[20]);
	playerData.mDailyRestData.m_nTwelvePalaceBuyTimes = lynxAtoi<UInt32>(row[21]);

	if (resTodayDelay5hours)
	{
	
// 		playerData.mDailyRestData.m_nCoinFreeBuyTimes = lynxAtoi<UInt32>(row[20]);
		playerData.mDailyRestData.notUsed7 = lynxAtoi<UInt32>(row[25]);

	}
	else
	{
		playerData.mDailyRestData.notUsed7 = 0;
	}

	playerData.mDailyRestData.m_nTowerBuyTimes = lynxAtoi<UInt32>(row[17]);
	playerData.mDailyRestData.m_RewardLotteryDailyOnceFreeCount = lynxAtoi<UInt32>(row[18]);
	playerData.mDailyRestData.m_nRankGameLeftTimes = lynxAtoi<UInt32>(row[23]);
	playerData.mDailyRestData.m_nRankGameBuyTimes = lynxAtoi<UInt32>(row[24]);


	bool resToday = TimeManager::timeIsToday(dbLastUpdateTime);
	//如果上次更新的时间和现在是同一天，那么取出数据
	playerData.mDailyRestData.m_nDailyChaiCount = lynxAtoi<UInt16>(row[5]);
	playerData.mDailyRestData.m_nDailyMiCount = lynxAtoi<UInt16>(row[6]);
	playerData.mDailyRestData.m_nDailyYouCount = lynxAtoi<UInt16>(row[7]);
	playerData.mDailyRestData.m_nDailyYanCount = lynxAtoi<UInt16>(row[8]);

	playerData.mDailyRestData.m_nLastUpdateTime = lynxAtoi<UInt64>(row[12]);

	if(resToday)
	{
		playerData.mDailyRestData.m_nArenaEnterCount = lynxAtoi<UInt16>(row[1]);
		playerData.mDailyRestData.m_nAmphitheaterEnterCount = lynxAtoi<UInt16>(row[2]);
		playerData.mDailyRestData.m_nAmphitheaterWinCount = lynxAtoi<UInt16>(row[3]);
		playerData.mDailyRestData.m_nAmphitheaterLoseCount = lynxAtoi<UInt16>(row[4]);
		
		playerData.mDailyRestData.m_nCouragetrialDailyPoint = lynxAtoi<UInt16>(row[9]);

		if(row[10])
		{
			playerData.mDailyRestData.m_strCourageData = row[10];
		}

		playerData.mDailyRestData.m_nActivepoint = lynxAtoi<UInt32>(row[11]);
//m_nDailyMultipleCopyCount
//m_nTwelvePalaceUnlockCount		
// 		playerData.mDailyRestData.m_nspare2 = lynxAtoi<UInt32>(row[15]);

		playerData.mDailyRestData.m_nClimbTowerTimes = lynxAtoi<UInt32>(row[16]);

		

	
		playerData.mDailyRestData.m_nFishEatTimes = lynxAtoi<UInt32>(row[22]);

		
		mDBInterface.freeResult(&rs); 
	}
	else
	{
		//如果不是同一天，说明数据需要刷新，那么更新数据
		playerData.mDailyRestData.m_nArenaEnterCount = 0;
		playerData.mDailyRestData.m_nAmphitheaterEnterCount = 0;
		playerData.mDailyRestData.m_nAmphitheaterWinCount = 0;
		playerData.mDailyRestData.m_nAmphitheaterLoseCount = 0;
	
		playerData.mDailyRestData.m_nTwelvepalaceEnterCount = 0;

		playerData.mDailyRestData.m_strTwelvepalaceData = "";


		playerData.mDailyRestData.m_nCouragetrialDailyPoint = 10;

		playerData.mDailyRestData.m_nActivepoint = 0;
		//m_nDailyMultipleCopyCount
		//m_nTwelvePalaceUnlockCount		
		
		
// 		playerData.mDailyRestData.m_nspare2 = 0;
		if (lynxAtoi<UInt32>(row[16]) > globalValue.uTPresetlowerlimit)
		{
			playerData.mDailyRestData.m_nClimbTowerTimes = lynxAtoi<UInt32>(row[16]);

		}
		else
		{
			playerData.mDailyRestData.m_nClimbTowerTimes = globalValue.uTPresetlowerlimit;			
		}
// 		playerData.mDailyRestData.m_nTwelvePalaceBuyTimes = 0;
		//先加载周更新的数据，之后根据判断进行
		if(row[10])
		{
			playerData.mDailyRestData.m_strCourageData = row[10];
		}

		playerData.mDailyRestData.m_nFishEatTimes = globalValue.uFISHEATmaxtimes;
		
		

		mDBInterface.freeResult(&rs); 

		//修改再存盘

		bool resToWeek = TimeManager::timeIsToWeek(dbLastUpdateTime);

		if(!resToWeek)
		{
			//不是同一周，那么更新数据
			playerData.mDailyRestData.m_strCourageData = " ";
			char sql2[2048] = {0};
			snprintf(sql2, sizeof(sql2), "call WeekReset_Update(%llu,'%s',%llu)",
				playerGuid, playerData.mDailyRestData.m_strCourageData.c_str(),
				playerData.mDailyRestData.m_nLastUpdateTime);

			LOG_DEBUG("Sql:%s", sql2);

			bool result2 = mDBInterface.execSql(sql2);
			if(!result2)
			{
				LOG_INFO("WeekReset_Update %llu %s from DB failed!", 
					playerGuid, playerData.mBaseData.m_strPlayerName.c_str());

				return LynxErrno::SqlExecuteFail;
			}


		}




	}


	


	LOG_INFO("Load DailyResetData %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());



	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerBaseData(PlayerData& playerData, UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call Player_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}

	playerData.mBaseData.m_nPlayerID = playerGuid;
	playerData.mBaseData.m_nModelID = lynxAtoi<UInt32>(row[1]);
	playerData.mBaseData.m_strPlayerName = row[2];
	playerData.mBaseData.m_nLevel = lynxAtoi<UInt32>(row[3]);

	playerData.mBaseData.m_nLevelExp = lynxAtoi<Guid>(row[4]);
	playerData.mBaseData.m_nVipLevel = lynxAtoi<UInt32>(row[5]);
	playerData.mBaseData.m_nVipExp = lynxAtoi<Guid>(row[6]);
	playerData.mBaseData.m_nGold = lynxAtoi<Guid>(row[7]);
	playerData.mBaseData.m_nCoin = lynxAtoi<Guid>(row[8]);
	playerData.mBaseData.m_nStrength = lynxAtoi<UInt32>(row[9]);
	playerData.mBaseData.m_strMood = row[10];

	playerData.mBaseData.m_listSweepCount.insertTail(lynxAtoi<UInt32>(row[11]));
	playerData.mBaseData.m_listSweepCount.insertTail(lynxAtoi<UInt32>(row[12]));
	playerData.mBaseData.m_listSweepCount.insertTail(lynxAtoi<UInt32>(row[13]));
	playerData.mBaseData.m_accountID = lynxAtoi<Guid>(row[14]);
	playerData.mBaseData.m_nFame = lynxAtoi<Guid>(row[15]);
	playerData.mBaseData.m_nPower = lynxAtoi<Guid>(row[16]);
	playerData.mBaseData.m_nLeavTime = lynxAtoi<Guid>(row[17]);
	playerData.mBaseData.m_strOldName =row[18];
	playerData.mBaseData.m_strGuid = row[19];
	playerData.mBaseData.m_nGuidGift = lynxAtoi<UInt32>(row[20]);
	playerData.mBaseData.m_nFirstLoinTime = lynxAtoi<UInt32>(row[21]);
	playerData.mBaseData.m_nLoginTime = lynxAtoi<UInt32>(row[22]);
	playerData.mBaseData.m_nForbidLoginTime = lynxAtoi<Guid>(row[23]);
	playerData.mBaseData.m_nForbidBeginTime = lynxAtoi<Guid>(row[24]);
	LOG_INFO("Load playerBaseData %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());


	playerData.mInlineActivityData.m_LastLeaveTime = playerData.mBaseData.m_nLeavTime;


	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}

UInt16 PersistWorker::loadPlayerSkillList(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call SkillList_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}

	for(unsigned int i = 0; i < MAXSKILLLIST; i ++)
	{

		SkillData skillData;
		skillData.m_nID = lynxAtoi<UInt32>(row[i*4]);
		skillData.m_nCD = lynxAtoi<UInt32>(row[i*4+1]);
		skillData.m_nLevel = lynxAtoi<UInt32>(row[i*4+2]);
		skillData.m_nEquipPos = lynxAtoi<UInt32>(row[i*4+3]);
		playerData.mSkillListData.m_listSkills.insertTail(skillData);

	}

	LOG_INFO("Load playerSkillList %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());


	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerBuffList(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call BuffList_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}

	for(unsigned int i = 0; i < MAXBUFFERCOUNT; i ++)
	{
		BufferData buffData;
		buffData.m_nBufferID = lynxAtoi<UInt32>(row[i*2]);
		buffData.m_nGetTime = lynxAtoi<UInt32>(row[i*2] + 1);
		playerData.mBuffListData.m_listBuffers.insertTail(buffData);

	}

	LOG_INFO("Load playerBuffList %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());


	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}	

UInt16 PersistWorker::loadPlayerFashionList(PlayerData& playerData,UInt64 playerGuid)
{
	char sqlFashions[2048] = {0};
	snprintf(sqlFashions,sizeof(sqlFashions),"call fashions_load(%llu)",playerGuid);
	LOG_DEBUG("sql:%s,",sqlFashions);

	bool resultFashions = mDBInterface.execSql(sqlFashions);
	MYSQL_RES * rsFashions = mDBInterface.storeResult();

	if(!rsFashions)
	{
		// 执行失败
		mDBInterface.freeResult(&rsFashions);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW rowFashions = mDBInterface.fetchRow(rsFashions);
	while(rowFashions)
	{
		UInt64 fashionId = lynxAtoi<UInt64>(rowFashions[0]);
		
		FashionData fashionData;
		fashionData.m_nFahionID  = fashionId;
		
		playerData.mFashionData.m_listFashionDatas.insertTail(fashionData);
		rowFashions = mDBInterface.fetchRow(rsFashions);
	}
	
	mDBInterface.freeResult(&rsFashions);

	return LynxErrno::None;
}


UInt16 PersistWorker::loadPlayerItemList(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call Item_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	while(row)
	{
		//已经被删除的物品就不添加到逻辑处理了
		if(lynxAtoi<UInt32>(row[42]))
		{
			row = mDBInterface.fetchRow(rs);
			continue;
		}

		ItemData itemData;
		itemData.m_nUid = lynxAtoi<UInt64>(row[0]);
		itemData.m_nItemId = lynxAtoi<UInt64>(row[1]);
		itemData.m_nCurStackAmount = lynxAtoi<UInt32>(row[3]);
		itemData.m_nItemGetTime = lynxAtoi<UInt64>(row[4]);
		//添加param列表
		for(unsigned int i = 0; i < PARAMSIZE; i++)
		{
			itemData.m_listParam.insertTail(lynxAtoi<UInt32>(row[i+5]));
		}

		itemData.m_nInvalid = lynxAtoi<UInt32>(row[42]);
		itemData.m_nDirtyType = NODIRTY;
		//判断是否为装备，不是装备则不进行加载

		if(row[2])
		{
			itemData.m_itemAttribute.m_nEquipState = lynxAtoi<UInt32>(row[13]); 
			itemData.m_itemAttribute.m_nEnhanceLevel = lynxAtoi<UInt32>(row[14]);
			itemData.m_itemAttribute.m_nEnhanceExp = lynxAtoi<UInt64>(row[15]);
			itemData.m_itemAttribute.m_nStarLevel = lynxAtoi<UInt32>(row[16]);
			itemData.m_itemAttribute.m_nStarExp = lynxAtoi<UInt64>(row[17]);
			itemData.m_itemAttribute.m_nRefineTimes = lynxAtoi<UInt32>(row[18]);

			for(unsigned int i = 0; i < MAXGEMCOUNT; i++)
			{
				UInt64 gemID = lynxAtoi<UInt64>(row[i+19]);

				itemData.m_itemAttribute.m_listGems.insertTail(gemID);

			}

			for(unsigned int i = 0; i < MAXINTRINSICATTRCOUNT; i++)
			{
				IntrinsicAttribute intrinsicAttr;
				intrinsicAttr.m_nKey =  lynxAtoi<UInt32>(row[24+i*2]);
				intrinsicAttr.m_nValue = lynxAtoi<UInt32>(row[25+i*2]);

				itemData.m_itemAttribute.m_listIntrinsicAttrs.insertTail(intrinsicAttr);

			}

			for(unsigned int i = 0; i < MAXRANDOMATTRCOUNT; i++)
			{
				RandomAttribute randomAttr;
				randomAttr.m_nKey = lynxAtoi<UInt32>(row[i*2+32]);
				randomAttr.m_nValue = lynxAtoi<UInt32>(row[i*2+33]);
				UInt32 attrQuality = getAttrQuality(randomAttr,itemData);
				//添加一个品质的字段
				randomAttr.m_nQuality = attrQuality;
				itemData.m_itemAttribute.m_listRandomAttrs.insertTail(randomAttr);

			}

			itemData.m_itemAttribute.m_nPurplePoints = lynxAtoi<UInt32>(row[43]);
			itemData.m_itemAttribute.m_nPurpleTimes = lynxAtoi<UInt32>(row[44]);

			itemData.m_itemAttribute.m_nOrangePoints = lynxAtoi<UInt32>(row[45]);
			itemData.m_itemAttribute.m_nOrangeTimes = lynxAtoi<UInt32>(row[46]);

		}

		playerData.mItemListData.m_listItems.insertTail(itemData);

		row = mDBInterface.fetchRow(rs);

	}

	LOG_INFO("Load itemData %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());


	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}

UInt32 PersistWorker::getAttrQuality(const RandomAttribute &randomAttr, const ItemData & itemData)
{
	//通过equip表找到装备对应的等级
	EquipTemplate *equipTemplate = EQUIP_TABLE().get(itemData.m_nItemId);
	assert(equipTemplate);
	UInt32 level = equipTemplate->mLevelRequire;


	if(randomAttr.m_nKey == PA)
	{
		return getPAQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MA)
	{
		return getMAQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == PF)
	{
		return getPFQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MF)
	{
		return getMFQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxHP)
	{
		return getMaxHPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxMP)
	{
		return getMaxMPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxSP)
	{
		return getMaxSPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == Critrate)
	{
		return getCritRateQuality(level,randomAttr);
	}
	else if(randomAttr.m_nKey == BaseCrit)
	{
		return getCirtDamageQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == BaseToughness)
	{
		//韧性
		return getBaseToughnessQuality(level, randomAttr);

	}
	else
	{
		return 0;
	}
}

UInt32 PersistWorker::getPAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr )
{

	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if (whiteTemplate == NULL)
	{
		LOG_WARN("whiteTemplate not found!!");
		return White;
	}
	if(randomAttr.m_nValue <= whiteTemplate->mPAMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if (greenTemplate == NULL)
		{
			LOG_WARN("greenTemplate not found!!");
			return Green;
		}
		if(randomAttr.m_nValue <= greenTemplate->mPAMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if (blueTemplate == NULL)
			{
				LOG_WARN("blueTemplate not found!!");
				return Blue;
			}
			if(randomAttr.m_nValue <= blueTemplate->mPAMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if (purpleTemplate == NULL)
				{
					LOG_WARN("purpleTemplate not found!!");
					return Purple;
				}
				if(randomAttr.m_nValue <= purpleTemplate->mPAMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 PersistWorker::getMAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr )
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if (whiteTemplate == NULL)
	{
		LOG_WARN("whiteTemplate not found!!");
		return White;
	}
	if(randomAttr.m_nValue <= whiteTemplate->mMAMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if (greenTemplate == NULL)
		{
			LOG_WARN("greenTemplate not found!!");
			return Green;
		}
		if(randomAttr.m_nValue <= greenTemplate->mMAMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if (blueTemplate == NULL)
			{
				LOG_WARN("blueTemplate not found!!");
				return Blue;
			}
			if(randomAttr.m_nValue <= blueTemplate->mMAMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if (purpleTemplate == NULL)
				{
					LOG_WARN("purpleTemplate not found!!");
					return Purple;
				}
				if(randomAttr.m_nValue <= purpleTemplate->mMAMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 PersistWorker::getPFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if (whiteTemplate == NULL)
	{
		LOG_WARN("whiteTemplate not found!!");
		return White;
	}
	if(randomAttr.m_nValue <= whiteTemplate->mPFMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if (greenTemplate == NULL)
		{
			LOG_WARN("greenTemplate not found!!");
			return Green;
		}
		if(randomAttr.m_nValue <= greenTemplate->mPFMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if (blueTemplate == NULL)
			{
				LOG_WARN("blueTemplate not found!!");
				return Blue;
			}
			if(randomAttr.m_nValue <= blueTemplate->mPFMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if (purpleTemplate == NULL)
				{
					LOG_WARN("purpleTemplate not found!!");
					return Purple;
				}
				if(randomAttr.m_nValue <= purpleTemplate->mPFMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 PersistWorker::getMFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMFMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMFMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMFMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMFMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 PersistWorker::getMaxHPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxHPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxHPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxHPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxHPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 PersistWorker::getMaxMPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxMPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxMPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxMPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxMPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 PersistWorker::getMaxSPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxSPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxSPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxSPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxSPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 PersistWorker::getCritRateQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritRateMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritRateMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritRateMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritRateMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 PersistWorker::getCirtDamageQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritDamageMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritDamageMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritDamageMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritDamageMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 PersistWorker::getBaseToughnessQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritDefMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritDefMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritDefMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritDefMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt16 PersistWorker::loadPlayerStageData(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call Stage_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::None;//todo
		return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		PlayerChapterData chapterData;

		chapterData.m_nChapterId = lynxAtoi<UInt16>(row[0]);
		chapterData.m_nIselite = lynxAtoi<UInt8>(row[1]);
		chapterData.m_nTreasure1getstate = lynxAtoi<UInt8>(row[2]);
		chapterData.m_nTreasure2getstate = lynxAtoi<UInt8>(row[3]);
		chapterData.m_nTreasure3getstate = lynxAtoi<UInt8>(row[4]);

		for(unsigned int i = 0; i < CHAPTERSTAGECOUNT; i++)
		{
			StageData stageData;
			stageData.m_nStageID = lynxAtoi<UInt32>(row[i*4+5]);
			stageData.m_nGetStar = lynxAtoi<UInt32>(row[i*4+6]);
			stageData.m_nChallengTimes = lynxAtoi<UInt32>(row[i*4+7]);
			stageData.m_nLastChallengTime = lynxAtoi<UInt64>(row[i*4+8]);
			chapterData.m_listStageDatas.insertTail(stageData);
		}

		chapterData.m_nChapterstarttime = lynxAtoi<UInt64>(row[85]);
		chapterData.m_nChapterfinishtime = lynxAtoi<UInt64>(row[86]);

		playerData.mChapterListData.m_listChapterData.insertTail(chapterData);

		row = mDBInterface.fetchRow(rs);
	}

	LOG_INFO("Load stageData %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());

	mDBInterface.freeResult(&rs);

	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerHoarStoneData(PlayerData& playerData,UInt64 playerUid)
{
	char sql[4096] = {0};
	
	snprintf(sql, sizeof(sql), "call Stele_Load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{

		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}

	for(UInt32 i = 0; i < 12; i++)
	{
		HoarStoneData stoneData;
		stoneData.mStoneLv = lynxAtoi<UInt16>(row[0+i*2]);
		stoneData.mPieceCount = lynxAtoi<UInt32>(row[1+i*2]);
		stoneData.mStoneStar = lynxAtoi<UInt16>(row[24+i]);
		stoneData.mHoarStoneId = i+1;

		std::string equipListStr = row[36+i];
		
		std::string ::size_type findIndex = equipListStr.find(';');

		std::stringstream mystream;
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string equipEleStr = equipListStr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << equipEleStr;

			UInt64 equipEle = 0;
			mystream >> equipEle;

			stoneData.mEquipList.insertTail(equipEle);

			equipListStr = equipListStr.substr(findIndex + 1);

			findIndex = equipListStr.find(';');

		}
	
		if(equipListStr != "")
		{
			mystream.clear();
			mystream.str("");

			mystream << equipListStr;

			UInt64 equipEle = 0;
			mystream >> equipEle;
			stoneData.mEquipList.insertTail(equipEle);
		}


		playerData.mHoarStoneData.mHoarStoneList.insertTail(stoneData);
	}

	std::string runeDataStr = row[48];
	
	std::string ::size_type  findIndex = runeDataStr.find(';');
	std::stringstream mystream;
	while(findIndex != std::string::npos)
	{
		//;号之前的全部截取，放入到列表里
		std::string runeDataEleStr = runeDataStr.substr(0,findIndex);

		std::string ::size_type midIndex = runeDataEleStr.find(',');

		std::string runeKeyStr = runeDataEleStr.substr(0,midIndex);
		std::string runeValueStr = runeDataEleStr.substr(midIndex+1);

		mystream.clear();
		mystream.str("");

		mystream << runeKeyStr;
		
		UInt64 runeKey = 0;
		mystream >> runeKey;

		mystream.clear();
		mystream.str("");

		mystream << runeValueStr;

		UInt64 runeValue = 0;
		mystream >> runeValue;
		
		playerData.mHoarStoneData.mRuneDataMap.insert(runeKey,runeValue);


		runeDataStr = runeDataStr.substr(findIndex + 1);

		findIndex = runeDataStr.find(';');

	}

	if(runeDataStr != "")
	{
		std::string ::size_type  midIndex = runeDataStr.find(',');

		std::string runeKeyStr = runeDataStr.substr(0,midIndex);
		std::string runeValueStr = runeDataStr.substr(midIndex+1);

		mystream.clear();
		mystream.str("");

		mystream << runeKeyStr;

		UInt64 runeKey = 0;
		mystream >> runeKey;

		mystream.clear();
		mystream.str("");

		mystream << runeValueStr;

		UInt64 runeValue = 0;
		mystream >> runeValue;


		playerData.mHoarStoneData.mRuneDataMap.insert(runeKey,runeValue);
	}

	LOG_INFO("Load stele %llu %s from DB successful!", 
		playerUid, playerData.mBaseData.m_strPlayerName.c_str());

	mDBInterface.freeResult(&rs);

	return LynxErrno::None;

}

UInt16 PersistWorker::loadPlayerRhymeData(PlayerData& playerData, UInt64 playerUid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call Rhyme_Load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}

	playerData.mRhymeData.m_PlayerUid = playerUid;
	playerData.mRhymeData.m_RhymeExp = lynxAtoi<UInt32>(row[0]);
	playerData.mRhymeData.m_RhymeLv = lynxAtoi<UInt32>(row[1]);
	playerData.mRhymeData.m_RhymeStep = lynxAtoi<UInt32>(row[2]);
	playerData.mRhymeData.m_RhymeSoul = lynxAtoi<UInt64>(row[3]);
	playerData.mRhymeData.m_RhymeSkillIndex = lynxAtoi<UInt32>(row[4]);
	playerData.mRhymeData.m_RhymeAcupointStr = row[5];
	playerData.mRhymeData.m_RhymeGrass = lynxAtoi<UInt32>(row[6]);
	playerData.mRhymeData.m_RhymeGrassMid = lynxAtoi<UInt32>(row[7]);
	playerData.mRhymeData.m_RhymeGrassHigh = lynxAtoi<UInt32>(row[8]);

	std::string acupointStr = playerData.mRhymeData.m_RhymeAcupointStr;
	std::string ::size_type  findIndex = acupointStr.find(';');
	std::stringstream mystream;
	while(findIndex != std::string::npos)
	{
		//;号之前的全部截取，放入到列表里
		std::string acupointEleStr = acupointStr.substr(0,findIndex);

		mystream.clear();
		mystream.str("");

		mystream << acupointEleStr;

		UInt64 acupointEle = 0;
		mystream >> acupointEle;

		playerData.mRhymeData.m_RhymeAcupointList.insertTail(acupointEle);

		acupointStr = acupointStr.substr(findIndex + 1);

		findIndex = acupointStr.find(';');

	}

	if(acupointStr != "")
	{
		mystream.clear();
		mystream.str("");

		mystream << acupointStr;

		UInt64 acupointEle = 0;
		mystream >> acupointEle;
		playerData.mRhymeData.m_RhymeAcupointList.insertTail(acupointEle);
	}



	LOG_INFO("Load rhymeData %llu %s from DB successful!", 
		playerUid, playerData.mBaseData.m_strPlayerName.c_str());

	mDBInterface.freeResult(&rs);

	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerFireConfirmData(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call FireConfirm_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		PlayerFireConfirmData FireConfirmData;

		FireConfirmData.m_ConfirmLevel = lynxAtoi<UInt32>(row[0]);
		FireConfirmData.m_ConfirnFailTimes = lynxAtoi<UInt32>(row[1]);
		FireConfirmData.m_ConfirnRightTimes = lynxAtoi<UInt32>(row[2]);



		playerData.mFireConfirmData = FireConfirmData;
		// 		playerData.mChapterListData.m_listChapterData.insertTail(chapterData);


		row = mDBInterface.fetchRow(rs);
	}

	LOG_INFO("Load FireConfirm %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());

	mDBInterface.freeResult(&rs);

	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerEquipData(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call heroequipload(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();

	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return LynxErrno::PlayerNotExist;
	}

	if(row)
	{
		for(UInt32 i = 0; i < 6; i++)
		{
			HeroEquipData equipData;
			for(UInt32 j = i*6 + 6; j < i*6 + 6 + 6; j++)
			{
				equipData.m_nGemsList.insertTail(lynxAtoi<UInt64>(row[j]));
				 
			}
			
			equipData.m_nEquipId = lynxAtoi<UInt64>(row[i+42]);
			equipData.m_nEquipLv = lynxAtoi<UInt32>(row[i]);
			equipData.m_nActiveBit = lynxAtoi<UInt64>(row[i+48]);
			equipData.m_nStep = lynxAtoi<UInt64>(row[i+54]);
			playerData.mEquipData.mHeroEquipLists.insertTail(equipData);
			
		}




	}

	LOG_INFO("Load heroEquipData %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());

	mDBInterface.freeResult(&rs);

	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerMaterialData(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call material_load(%llu)", playerUid);
	
	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}
	
	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	while(row)
	{
		UInt64 itemUid = lynxAtoi<UInt64>(row[0]);
		UInt64 itemId = lynxAtoi<UInt64>(row[2]);
		UInt32 itemCount = lynxAtoi<UInt32>(row[3]);

		MaterialData materialData;
		materialData.m_nUid = itemUid;
		materialData.m_nId = itemId;
		materialData.m_nCount = itemCount;
		materialData.m_nDirtyType = NODIRTY;

		playerData.mMaterialData.mMaterialLists.insertTail(materialData);

	    row = mDBInterface.fetchRow(storeRes);
	}


	mDBInterface.freeResult(&storeRes);
	
	LOG_INFO("Load MaterialData %llu %s from DB successful!", 
		playerUid, playerData.mBaseData.m_strPlayerName.c_str());
	return LynxErrno::None;
	
}

UInt16 PersistWorker::loadPlayerGemData(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call gem_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	while(row)
	{
		UInt64 gemUid = lynxAtoi<UInt64>(row[0]);
		UInt64 gemId = lynxAtoi<UInt64>(row[1]);
		UInt32 gemCount = lynxAtoi<UInt32>(row[2]);

		GemData gemData;
		gemData.m_nUid = gemUid;
		gemData.m_nId = gemId;
		gemData.m_nCount = gemCount;
		gemData.m_nDirtyType = NODIRTY;

		playerData.mGemData.mGemLists.insertTail(gemData);

		row = mDBInterface.fetchRow(storeRes);
	}


	mDBInterface.freeResult(&storeRes);

	LOG_INFO("Load MaterialData %llu %s from DB successful!", 
		playerUid, playerData.mBaseData.m_strPlayerName.c_str());
	return LynxErrno::None;
}

void PersistWorker::initJewelryRdAttr(JewelryData& jewelryData,std::string &jewelryStr, std::stringstream &mystream)
{
	std::string ::size_type findRdId = jewelryStr.find(',');
	std::string rdIdStr = jewelryStr.substr(0,findRdId);
	mystream.clear();
	mystream.str("");
	mystream << rdIdStr;
	UInt64 rdId;
	mystream >> rdId;
	jewelryStr = jewelryStr.substr(findRdId + 1);

	mystream.clear();
	mystream.str("");
	std::string ::size_type findRdKey = jewelryStr.find(',');
	std::string rdKeyStr = jewelryStr.substr(0,findRdKey);
	UInt32 rdKey;
	mystream << rdKeyStr;
	mystream >> rdKey;
	jewelryStr = jewelryStr.substr(findRdKey+1);

	mystream.clear();
	mystream.str("");
	mystream << jewelryStr;
	UInt32 rdValue;
	mystream >> rdValue;

	JewelryAttr jewelryAttr;
	jewelryAttr.mKey = rdKey;
	jewelryAttr.mRandomID = rdId;
	jewelryAttr.mValue = rdValue;

	jewelryData.m_randomAttrList.insertTail(jewelryAttr);
}

UInt16 PersistWorker::loadPlayerJewelryData(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call jewelry_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;

		LOG_WARN("Load JewelryData %llu %s from DB failed!", 
			playerUid, playerData.mBaseData.m_strPlayerName.c_str());
	}

	std::stringstream mystream;
	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
	while(row)
	{
		std::string randomAttrStr = row[2];
		
		JewelryData jewelryData;
		jewelryData.m_nJewelryUid = lynxAtoi<UInt64>(row[0]);
		jewelryData.m_nJewelryId = lynxAtoi<UInt64>(row[1]);
		jewelryData.m_staticAttrKey = lynxAtoi<UInt32>(row[3]);
		jewelryData.m_staticAttrValue = lynxAtoi<UInt32>(row[4]);
		jewelryData.m_purplePoints = lynxAtoi<UInt32>(row[5]);
		jewelryData.m_purpleTimes = lynxAtoi<UInt32>(row[6]);

		jewelryData.m_orangePoints = lynxAtoi<UInt32>(row[7]);
		jewelryData.m_orangeTimes = lynxAtoi<UInt32>(row[8]);

	
		std::string::size_type findPos = randomAttrStr.find(';');
		while(findPos != std::string::npos)
		{
			
			std::string jewelryStr = randomAttrStr.substr(0,findPos);
		
			initJewelryRdAttr(jewelryData,jewelryStr,mystream);
			
			randomAttrStr = randomAttrStr.substr(findPos+1);
			findPos = randomAttrStr.find(';');
			
		}
		

		if(!randomAttrStr.empty())
		{
			initJewelryRdAttr(jewelryData,randomAttrStr,mystream);
		}

		
		playerData.mJewelryData.mJewelryList.insertTail(jewelryData);
		row = mDBInterface.fetchRow(storeRes);

	}


	mDBInterface.freeResult(&storeRes);

	LOG_INFO("Load JewelryData %llu %s from DB successful!", 
		playerUid, playerData.mBaseData.m_strPlayerName.c_str());
	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerJewelryEquipData(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call herojewelry_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	std::stringstream mystream;
	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	if(row)
	{
		
		playerData.mJewelryEquipData.mJewelryBit = lynxAtoi<UInt32>(row[1]);
		playerData.mJewelryEquipData.mJewelryList.insertTail( lynxAtoi<UInt64>(row[2]));
		playerData.mJewelryEquipData.mJewelryList.insertTail( lynxAtoi<UInt64>(row[3]));

		playerData.mJewelryEquipData.mJewelryList.insertTail( lynxAtoi<UInt64>(row[4]));
		playerData.mJewelryEquipData.mJewelryList.insertTail( lynxAtoi<UInt64>(row[5]));
		playerData.mJewelryEquipData.mJewelryList.insertTail( lynxAtoi<UInt64>(row[6]));
		playerData.mJewelryEquipData.mJewelryList.insertTail( lynxAtoi<UInt64>(row[7]));

		
		
	}


	mDBInterface.freeResult(&storeRes);

	LOG_INFO("Load HeroJewelryEquipData %llu %s from DB successful!", 
		playerUid, playerData.mBaseData.m_strPlayerName.c_str());
	return LynxErrno::None;
}


UInt16 PersistWorker::loadServantFood(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call servantfood_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	if(row)
	{

		playerData.mServantData.food1count = lynxAtoi<UInt32>(row[0]);
		playerData.mServantData.food2count = lynxAtoi<UInt32>(row[1]);
		playerData.mServantData.food3count = lynxAtoi<UInt32>(row[2]);
		playerData.mServantData.food4count = lynxAtoi<UInt32>(row[3]);
		playerData.mServantData.food5count = lynxAtoi<UInt32>(row[4]);
		playerData.mServantData.battleStone =  lynxAtoi<UInt32>(row[5]);
		playerData.mServantData.servantSwitch = lynxAtoi<UInt32>(row[6]);

	}


	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;
}


UInt16 PersistWorker::loadServantTreasure(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call servanttreasure_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	while(row)
	{
		ServantTreasure servantTreasure;
		servantTreasure.treasureUid = lynxAtoi<UInt64>(row[0]);
		servantTreasure.treasureId = lynxAtoi<UInt64>(row[1]);
		servantTreasure.count = lynxAtoi<UInt32>(row[2]);

		playerData.mServantData.treasureList.insertTail(servantTreasure);

		row = mDBInterface.fetchRow(storeRes);

	}


	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;
}


UInt16 PersistWorker::loadServantList(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call servant_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	while(row)
	{
		ServantData servantData;
		servantData.servantUid = lynxAtoi<UInt64>(row[0]);
		servantData.servantId = lynxAtoi<UInt64>(row[2]);
		servantData.pieceCount = lynxAtoi<UInt32>(row[3]);
		servantData.level = lynxAtoi<UInt32>(row[4]);
		servantData.star = lynxAtoi<UInt32>(row[5]);
		servantData.floor = lynxAtoi<UInt32>(row[6]);
		servantData.equipTreasures.insertTail(lynxAtoi<UInt32>(row[7]));
		servantData.equipTreasures.insertTail(lynxAtoi<UInt32>(row[8]));
		servantData.equipTreasures.insertTail(lynxAtoi<UInt32>(row[9]));
		servantData.equipTreasures.insertTail(lynxAtoi<UInt32>(row[10]));
        servantData.lvexp = lynxAtoi<UInt32>(row[11]);
		servantData.infolock = lynxAtoi<UInt32>(row[12]);

		playerData.mServantData.servantList.insertTail(servantData);

		row = mDBInterface.fetchRow(storeRes);

	}


	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;
}


UInt16 PersistWorker::loadServantBattleData(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call servantbattle_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	if(row)
	{
		playerData.mServantData.servantBattleData.mainBattleBit =  lynxAtoi<UInt32>(row[0]);
		for(UInt32 i = 1; i <= 5; i++)
		{
			playerData.mServantData.servantBattleData.mainServantBattle.insertTail(lynxAtoi<UInt64>(row[i]));	
		}

		playerData.mServantData.servantBattleData.assistBattleBit = lynxAtoi<UInt32>(row[6]);

		for(UInt32 i = 0; i < 7; i++)
		{

			AssistBattleData assistBattleData;
			assistBattleData.equipAssist = lynxAtoi<UInt64>(row[2*i + 7]);
			assistBattleData.quality = lynxAtoi<UInt32>(row[2*i +8]);
			
			playerData.mServantData.servantBattleData.assistServantBattle.insertTail(assistBattleData);
			
		}
				
	}


	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;
}

UInt16 PersistWorker::loadPlayerServantData(PlayerData& playerData,UInt64 playerUid)
{
	
	UInt16 foodRes = loadServantFood(playerData,playerUid);

	if(foodRes != LynxErrno::None)
	{
		return foodRes;
	}
	 
	UInt16 treasureRes = loadServantTreasure(playerData,playerUid);

	if(treasureRes != LynxErrno::None)
	{
		return treasureRes;
	}

	UInt16 servantRes = loadServantList(playerData,playerUid);
	
	if(servantRes != LynxErrno::None)
	{
		return servantRes;
	}

	UInt16 servantBattleRes = loadServantBattleData(playerData,playerUid);

	if(servantBattleRes != LynxErrno::None)
	{
		return servantBattleRes;
	}

	LOG_INFO("Load ServantData %llu %s from DB successful!", 
		playerUid, playerData.mBaseData.m_strPlayerName.c_str());
	return LynxErrno::None;


}


UInt16 PersistWorker::loadChatData(PlayerData& playerData,UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call playerchat_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	if(row)
	{
		for(UInt32 i = 0; i < 10; i++)
		{
			UInt64 playerUid = lynxAtoi<UInt64>(row[2*i]);
			
			if(playerUid)
			{
				UInt64 chatTime = lynxAtoi<UInt64>(row[2*i + 1]);

				ChatData chatData;
				chatData.playerUid = playerUid;
				chatData.chatTime = chatTime;

				playerData.mChatData.chatLists.insertTail(chatData);
			}	
			
		}
		
		playerData.mChatData.disableFlag = lynxAtoi<UInt32>(row[20]);
		playerData.mChatData.disableBeginTime = lynxAtoi<UInt64>(row[21]);
        playerData.mChatData.disableTimeLong = lynxAtoi<UInt64>(row[22]);

	}


	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;
}


UInt16 PersistWorker::loadFriendData(PlayerData& playerData,UInt64 playerUid)
{		
	/*if(playerData.mFriendData.m_nFindBit)
	{
		return LynxErrno::None;
	}*/

	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call friendandblack_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	if(row)
	{
		playerData.mFriendData.m_nFindBit = 0;
		String friends = row[1];
		String blacks = row[2];

		std::string friendstr = friends.c_str();
		std::string blackstr = blacks.c_str();
		//字符串格式为1068;2012;1222;等等
		
		std::string ::size_type findIndex = friendstr.find(';');

		std::stringstream mystream;
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string friendstrEle = friendstr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << friendstrEle;

			UInt64 friendEle = 0;
			mystream >> friendEle;

			playerData.mFriendData.mFriendList.insertTail(friendEle);

			friendstr = friendstr.substr(findIndex + 1);

			findIndex = friendstr.find(';');

		}
		

		findIndex = blackstr.find(';');
	
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string blackstrEle = blackstr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << blackstrEle;

			UInt64 blackEle = 0;
			mystream >> blackEle;

			playerData.mFriendData.mBlackList.insertTail(blackEle);

			blackstr = blackstr.substr(findIndex + 1);

			findIndex = blackstr.find(';');

		}

		

	}

	mDBInterface.freeResult(&storeRes);

	

	return LynxErrno::None;
}


UInt16 PersistWorker::loadEmailData(PlayerData& playerData, UInt64 playerUid)
{
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call email_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	while(row)
	{
		EmailData emailData;
		emailData.m_nEmailUid = lynxAtoi<UInt64>(row[0]);
		emailData.m_nTempId = lynxAtoi<UInt64>(row[1]);
		emailData.m_nOpenState = lynxAtoi<UInt32>(row[2]);
		emailData.m_nGetState = lynxAtoi<UInt32>(row[3]);
		emailData.m_strDes = row[4];
		emailData.m_strContent = row[5];
		emailData.m_nPlayerUid = lynxAtoi<UInt64>(row[6]);
		emailData.m_strFrom = row[7];
		emailData.m_strTittle = row[8];
		emailData.m_nTime = lynxAtoi<UInt64>(row[9]);
		
		playerData.mEmailData.emailList.insertTail(emailData);

		row = mDBInterface.fetchRow(storeRes);
	}

		


	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;
}


UInt16 PersistWorker::loadCharactorData(PlayerData& playerData, UInt64 playerUid)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call charactor_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", charactorsql);

	bool result = mDBInterface.execSql(charactorsql);
	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);

	while(row)
	{
		CharactorData charactorData;
		
		charactorData.m_nCharactorId = lynxAtoi<UInt64>(row[0]);
		charactorData.m_nEquipFashion = lynxAtoi<UInt32>(row[1]);
	
		playerData.mPlayerCharactorData.charactorList.insertTail(charactorData);
		

		row = mDBInterface.fetchRow(storeRes);
	}



	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;
}

UInt16 PersistWorker::loadAchieveData(PlayerData& playerData, UInt64 playerUid)
{
	char achievesql[4096] = {0};
	snprintf(achievesql,sizeof(achievesql),"call achieve_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", achievesql);

	bool result = mDBInterface.execSql(achievesql);
	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
	while(row)
	{
		UInt32 fieldCount = mDBInterface.getNumFields(storeRes);
		for(int i = 0; i < fieldCount/4; i++)
		{
			AchieveData achieveData;
			achieveData.m_nEventId = lynxAtoi<UInt64>(row[i*4]);
			achieveData.m_nAchieveId = lynxAtoi<UInt64>(row[i*4+1]);
			achieveData.m_nArg = lynxAtoi<UInt32>(row[i*4+2]);
			achieveData.m_nFlag = lynxAtoi<UInt32>(row[i*4+3]);

			if(achieveData.m_nEventId)
				playerData.mAchieveData.achieveList.insertTail(achieveData);
		}
		


		row = mDBInterface.fetchRow(storeRes);
	}



	mDBInterface.freeResult(&storeRes);

	return LynxErrno::None;

}

UInt16 PersistWorker::loadDailyTaskData(PlayerData& playerData, UInt64 playerUid)
{
	
	char achievesql[4096] = {0};
	snprintf(achievesql,sizeof(achievesql),"call dailytask_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", achievesql);

	bool result = mDBInterface.execSql(achievesql);
	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	UInt64 times = time(0);
	bool flagOneDay = false;
	UInt64 lastresetTime = 0;
	if(row)
	{
		UInt32 fieldCount = mDBInterface.getNumFields(storeRes);
			
		//获取上次重置时间
		UInt64 resetTime = lynxAtoi<UInt64>(row[51]);
		lastresetTime = resetTime;
		//判断现在时间和数据库时间是否处于同一天
		bool isOneDay = TimeManager::timeIsOneDay(times, resetTime);
		flagOneDay = isOneDay;
		for(int i = 0; i < fieldCount/3; i++)
		{
			
			//是同一天，那么不需要更新时间到数据库，也不需要重置每日任务和每日活跃点以及领取情况
			
			if(isOneDay)
			{
				//是同一天那么不需要重置
				DailyTaskData dailyTaskData;
				dailyTaskData.m_nTempId = lynxAtoi<UInt64>(row[i*3]);
				dailyTaskData.m_nArg = lynxAtoi<UInt32>(row[i*3+1]);
				dailyTaskData.m_nFlag = lynxAtoi<UInt32>(row[i*3+2]);

				DailyTaskTemplate * dailyTemp = DAILYTASK_TABLE().get(dailyTaskData.m_nTempId);
				if(dailyTemp)
				{
					dailyTaskData.m_nEventId = dailyTemp->mEvent;
				}

				if(dailyTaskData.m_nTempId && dailyTemp)
					playerData.mDailyTaskData.dailytaskList.insertTail(dailyTaskData);

			}
			else
			{
				//不是同一天那么需要重置
				DailyTaskData dailyTaskData;
				dailyTaskData.m_nTempId = lynxAtoi<UInt64>(row[i*3]);
				dailyTaskData.m_nArg = 0;
				dailyTaskData.m_nFlag = 0;
				DailyTaskTemplate * dailyTemp = DAILYTASK_TABLE().get(dailyTaskData.m_nTempId);
				if(dailyTemp)
				{
					dailyTaskData.m_nEventId = dailyTemp->mEvent;
				}

				if(dailyTaskData.m_nTempId && dailyTemp)
					playerData.mDailyTaskData.dailytaskList.insertTail(dailyTaskData);	

				
			}
		
			
			
		}

		
	}



	mDBInterface.freeResult(&storeRes);

	//不是同一天
	if(!flagOneDay)
	{
		char resetsql[4096]={0};
		snprintf(resetsql,sizeof(resetsql),"call dailytask_reset(%llu, %llu)", playerUid, times);

		LOG_DEBUG("Sql:%s", resetsql);

		bool result = mDBInterface.execSql(resetsql);
		if(!result)
		{
			return LynxErrno::SqlExecuteFail;
		}

		playerData.mDailyTaskData.lastResetTime = times;
	}
	else
	{
			playerData.mDailyTaskData.lastResetTime = lastresetTime;
	}

	return LynxErrno::None;
}

UInt16 PersistWorker::loadDailyActiveData(PlayerData& playerData, UInt64 playerUid)
{
	char dailyactsql[4096] = {0};
	snprintf(dailyactsql,sizeof(dailyactsql),"call dailyactive_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", dailyactsql);

	bool result = mDBInterface.execSql(dailyactsql);
	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	UInt64 times = time(0);
	bool onedayflag = false;
	UInt64 lasttimes = 0;
	if(row)
	{
			
			UInt32 flag1 = lynxAtoi<UInt64>(row[0]);
			UInt32 flag2 = lynxAtoi<UInt32>(row[1]);
			UInt32 flag3 = lynxAtoi<UInt32>(row[2]);
			UInt32 flag4 = lynxAtoi<UInt32>(row[3]);
			UInt32 active = lynxAtoi<UInt32>(row[4]);
			UInt64 srctime = lynxAtoi<UInt32>(row[5]);
			lasttimes = srctime;

			//判断现在时间和数据库时间是否处于同一天
			bool isOneDay = TimeManager::timeIsOneDay(times, srctime);
			//是同一天，那么不需要更新时间到数据库，也不需要重置每日任务和每日活跃点以及领取情况
			if(isOneDay)
			{
				playerData.mDailyAcvData.flags.insertTail(flag1);
				playerData.mDailyAcvData.flags.insertTail(flag2);
				playerData.mDailyAcvData.flags.insertTail(flag3);
				playerData.mDailyAcvData.flags.insertTail(flag4);
				playerData.mDailyAcvData.curActive = active;
				
			}
			else
			{

				playerData.mDailyAcvData.flags.insertTail(UInt32(0));
				playerData.mDailyAcvData.flags.insertTail(UInt32(0));
				playerData.mDailyAcvData.flags.insertTail(UInt32(0));
				playerData.mDailyAcvData.flags.insertTail(UInt32(0));
				playerData.mDailyAcvData.curActive = 0;
			
			}

			onedayflag = isOneDay;
		}

		

	mDBInterface.freeResult(&storeRes);
	//不是同一天
	if(!onedayflag)
	{
		char resetsql[4096]={0};
		snprintf(resetsql,sizeof(resetsql),"call dailyactive_reset(%llu, %llu)", playerUid, times);

		LOG_DEBUG("Sql:%s", resetsql);

		bool result = mDBInterface.execSql(resetsql);
		if(!result)
		{
			return LynxErrno::SqlExecuteFail;
		}
		playerData.mDailyAcvData.lastRestTime = times;
	}
	else
	{
		playerData.mDailyAcvData.lastRestTime = lasttimes;
	}


	return LynxErrno::None;
}

UInt16  PersistWorker::loadConsortData(PlayerData& playerData, UInt64 playerUid)
{
	char consortsql[4096] = {0};
	snprintf(consortsql,sizeof(consortsql),"call playerconsort_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", consortsql);

	bool result = mDBInterface.execSql(consortsql);
	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::SqlExecuteFail;
	}

	


	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	if(row)
	{
		playerData.mConsortData.m_nConsortId = lynxAtoi<UInt64>(row[1]);
		playerData.mConsortData.m_nCurContribute = lynxAtoi<UInt32>(row[2]);
		playerData.mConsortData.m_nTotalContribute = lynxAtoi<UInt32>(row[3]);
		playerData.mConsortData.m_nConsortJob = lynxAtoi<UInt32>(row[4]);
		playerData.mConsortData.m_nLeaveTime = lynxAtoi<UInt64>(row[5]);
		
		
	}
	else
	{
		mDBInterface.freeResult(&storeRes);
		return LynxErrno::PlayerConsortNotFind;
	}
	

	mDBInterface.freeResult(&storeRes);

	//load 玩家申请列表 王戊辰

	char applysql[4096] = {0};
	snprintf(applysql,sizeof(applysql),"call playerapply_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", applysql);

	bool applyresult = mDBInterface.execSql(applysql);
	MYSQL_RES* applystoreRes = mDBInterface.storeResult();
	if(!applystoreRes)
	{
		mDBInterface.freeResult(&applystoreRes);
		return LynxErrno::SqlExecuteFail;
	}

	
	MYSQL_ROW applyrow = mDBInterface.fetchRow(applystoreRes);
	while(applyrow)
	{
	
		playerData.mConsortData.m_nApplyList.insertTail( lynxAtoi<UInt64>(applyrow[1]));
		applyrow = mDBInterface.fetchRow(applystoreRes);
		
		
	}
	

	mDBInterface.freeResult(&applystoreRes);


	//签到活动相关数据
	
	char signsql[4096] = {0};
	snprintf(signsql,sizeof(signsql),"call consortsign_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", signsql);

	bool signresult = mDBInterface.execSql(signsql);
	MYSQL_RES* signRes = mDBInterface.storeResult();
	if(!signRes)
	{
		mDBInterface.freeResult(&signRes);
		return LynxErrno::SqlExecuteFail;
	}

	
	MYSQL_ROW signrow = mDBInterface.fetchRow(signRes);
	if(signrow)
	{
	
		playerData.mConsortData.m_nSignAwards.insertTail( lynxAtoi<UInt32>(signrow[2]));
		playerData.mConsortData.m_nSignAwards.insertTail(lynxAtoi<UInt32>(signrow[3]));
		playerData.mConsortData.m_nSignAwards.insertTail(lynxAtoi<UInt32>(signrow[4]) );
		playerData.mConsortData.m_nSignAwards.insertTail(lynxAtoi<UInt32>(signrow[5]) );

		playerData.mConsortData.m_nSign =  lynxAtoi<UInt32>(signrow[1]);
		playerData.mConsortData.m_nKitchenTimes = lynxAtoi<UInt32>(signrow[6]);
		playerData.mConsortData.m_nBusinessCatTimes = lynxAtoi<UInt32>(signrow[7]);
		playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[8]) );
			playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[9]) );
				playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[10]) );
					playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[11]) );
						playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[12]) );
							playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[13]) );
								playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[14]) );
									playerData.mConsortData.m_nBuyList.insertTail( lynxAtoi<UInt32>(signrow[15]) );
									playerData.mConsortData.m_nEyeSightTimes = lynxAtoi<UInt32>(signrow[16]);
									playerData.mConsortData.m_nTicketTimes = lynxAtoi<UInt32>(signrow[17]);
										playerData.mConsortData.m_nEloquenceTimes = lynxAtoi<UInt32>(signrow[18]);
										playerData.mConsortData.m_nWoodCatFlag = lynxAtoi<UInt32>(signrow[19]);
									    playerData.mConsortData.m_nWoodCatTimes = lynxAtoi<UInt32>(signrow[20]);
								

		signrow = mDBInterface.fetchRow(signRes);
		
		
	}
	

	mDBInterface.freeResult(&signRes);

	//////////////////////////////////////////////////////////
	//行侠仗义数据

	char loyalsql[4096] = {0};
	snprintf(loyalsql,sizeof(loyalsql),"call consortloyal_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", loyalsql);

	bool loyalresult = mDBInterface.execSql(loyalsql);
	MYSQL_RES* loyalRes = mDBInterface.storeResult();
	if(!loyalRes)
	{
		mDBInterface.freeResult(&loyalRes);
		return LynxErrno::SqlExecuteFail;
	}

	
	MYSQL_ROW loyalRow = mDBInterface.fetchRow(loyalRes);
	if(loyalRow)
	{
		for(UInt32 i = 0; i < 4; i ++)
		{
			   UInt32 index = (i+1)*2;
				XingxiaTask xingxiaTask;
				xingxiaTask.m_nTaskId = lynxAtoi<UInt32>(loyalRow[index]);
				xingxiaTask.m_nGettime = lynxAtoi<UInt64>(loyalRow[index +1]);
				playerData.mConsortData.m_listXingxiaTasks.insertTail(xingxiaTask);
		}
	
		playerData.mConsortData.m_nRefreshTimes = lynxAtoi<UInt32>(loyalRow[1]);
		
		loyalRow = mDBInterface.fetchRow(loyalRes);
		
		
	}
	

	mDBInterface.freeResult(&loyalRes);


	/////////////////////////////////////////////////////////////
	char ticketsql[4096] = {0};
	snprintf(ticketsql,sizeof(ticketsql),"select * from  ticketaward where playeruid = %llu", playerUid);

	LOG_DEBUG("Sql:%s", ticketsql);

	bool ticketbool = mDBInterface.execSql(ticketsql);
	MYSQL_RES* ticketRes = mDBInterface.storeResult();
	if(!ticketRes)
	{
		mDBInterface.freeResult(&ticketRes);
		return LynxErrno::SqlExecuteFail;
	}

	
	MYSQL_ROW ticketRow = mDBInterface.fetchRow(ticketRes);
	if(ticketRow)
	{
		TicketAward ticketAward;
		ticketAward.m_nTicketId = lynxAtoi<UInt32>(ticketRow[1]);
		ticketAward.m_nAwardId = lynxAtoi<UInt64>(ticketRow[2]);
		ticketAward.m_nPeapleCnt = lynxAtoi<UInt32>(ticketRow[3]);
		ticketAward.m_nActiveTime = lynxAtoi<UInt64>(ticketRow[4]);
		ticketRow = mDBInterface.fetchRow(ticketRes);	
		playerData.mConsortData.m_nTicketAwardList.insertTail(ticketAward);
	}
	
	mDBInterface.freeResult(&ticketRes);

	//////////////////////////////////////////////////////////////////

	return LynxErrno::None;
}


void PersistWorker::onPersistLoadPlayerDataReq(PersistLoadPlayerDataReq& msg)
{

	Guid playerGuid = msg.mPlayerGuid;

	PersistLoadPlayerDataResp resp;
	resp.mConnId = msg.mConnId;
	resp.mPlayerGuid = msg.mPlayerGuid;

	//根据需要更改自己的playerdata
	do 
	{

		PlayerData playerData;
		UInt16 resultID = loadPlayerBaseData(playerData,playerGuid);

		if(resultID != LynxErrno::None)
		{
			resp.merrorId = resultID;
			break;
		}

		UInt16 resDailyReset = loadPlayerDailyResetData(playerData,playerGuid);
		if(resultID != LynxErrno::None)
		{
			resp.merrorId = resDailyReset;
			break;
		}

		UInt16 resSkillLoad = loadPlayerSkillList(playerData,playerGuid);
		if(resSkillLoad != LynxErrno::None)
		{
			resp.merrorId = resSkillLoad;
			break;
		}

		UInt16 resBuffLoad = loadPlayerBuffList(playerData,playerGuid);
		if(resBuffLoad != LynxErrno::None)
		{
			resp.merrorId = resBuffLoad;
			break;
		}

		UInt16 resItem = loadPlayerItemList(playerData,playerGuid);

		if(resItem != LynxErrno::None)
		{
			resp.merrorId = resItem;
			break;
		}

		UInt16 resFashion = loadPlayerFashionList(playerData,playerGuid);
		if(resFashion != LynxErrno::None)
		{
			resp.merrorId = resFashion;
			break;
		}

		UInt16 resStage = loadPlayerStageData(playerData,playerGuid);
		if(resStage != LynxErrno::None)
		{
			resp.merrorId = resStage;
			break;
		}

		UInt16 resFireConfirm = loadPlayerFireConfirmData(playerData,playerGuid);
		if(resFireConfirm != LynxErrno::None)
		{
			resp.merrorId = resFireConfirm;
			break;
		}


		UInt16 resCounter = loadCounter(playerData,playerGuid);
		if(resCounter != LynxErrno::None)
		{
			resp.merrorId = resCounter;
			break;
		}
		UInt16 resCopyCounter = loadCopyCounter(playerData,playerGuid);
		if(resCopyCounter != LynxErrno::None)
		{
			resp.merrorId = resCopyCounter;
			break;
		}
	

		UInt16 ret = loadRewardCounter(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		ret = loadFoods(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		ret = loadFriendBeApplyData(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		ret = loadShop(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		ret = loadShopItem(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		ret = loadCatCoin(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}
		ret = loadCoinGroup(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		ret = loadShop(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}


		ret = loadLottery(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		ret = loadStrength(playerData,playerGuid);
		if(ret != LynxErrno::None)
		{
			resp.merrorId = ret;
			break;
		}

		UInt16 resRhyme = loadPlayerRhymeData(playerData,playerGuid);
		if(resRhyme != LynxErrno::None)
		{
			resp.merrorId = resRhyme;
			break;
		}

		UInt16 resHoarStone = loadPlayerHoarStoneData(playerData,playerGuid);
		if(resHoarStone != LynxErrno::None)
		{
			resp.merrorId = resHoarStone;
			break;
		}

		UInt16 resHeroEquip = loadPlayerEquipData(playerData,playerGuid);
		if(resHeroEquip != LynxErrno::None)
		{
			resp.merrorId = resHeroEquip;
			break;
		}

		UInt16 resMaterialData = loadPlayerMaterialData(playerData,playerGuid);
		if(resMaterialData != LynxErrno::None)
		{
			resp.merrorId = resMaterialData;
			break;
		}

		UInt16 resGemData = loadPlayerGemData(playerData,playerGuid);
		if(resGemData != LynxErrno::None)
		{
			resp.merrorId = resGemData;
			break;
		}

		UInt16 resJewelryData = loadPlayerJewelryData(playerData,playerGuid);
		if(resJewelryData != LynxErrno::None)
		{
			resp.merrorId = resJewelryData;
			break;
		}

		UInt16 resJewelryEquip = loadPlayerJewelryEquipData(playerData, playerGuid);
		if(resJewelryEquip != LynxErrno::None)
		{
			resp.merrorId = resJewelryEquip;
			break;
		}


		UInt16 resLocalData = loadPlayerLocalData(playerData, playerGuid);
		if(resLocalData != LynxErrno::None)
		{
			resp.merrorId = resLocalData;
			break;
		}

		UInt16 resChapterData = loadPlayerChapterData(playerData, playerGuid);
		if(resChapterData != LynxErrno::None)
		{
			resp.merrorId = resChapterData;
			break;
		}

		UInt16 resServantData =  loadPlayerServantData(playerData, playerGuid);
		if(resServantData != LynxErrno::None)
		{
			resp.merrorId = resJewelryEquip;
			break;
		}

		UInt16 resChatData = loadChatData(playerData,playerGuid);
		if(resChatData != LynxErrno::None)
		{
			resp.merrorId = resChatData;
			break;
		}

		UInt16 resFriendData = loadFriendData(playerData,playerGuid);
		if(resFriendData != LynxErrno::None)
		{
			resp.merrorId = resFriendData;
			break;
		}

		UInt16 resPersonalRangking = loadPersonalRangking(playerData,playerGuid);
		if(resPersonalRangking != LynxErrno::None)
		{
			resp.merrorId = resPersonalRangking;
			break;
		}

		UInt16 resTowerData = loadTowerData(playerData,playerGuid);
		if(resTowerData != LynxErrno::None)
		{
			resp.merrorId = resTowerData;
			break;
		}
		

		resTowerData = loadInlineActivityList(playerData,playerGuid);
		if(resTowerData != LynxErrno::None)
		{
			resp.merrorId = resTowerData;
			break;
		}		
		resTowerData = loadSevenDayTask(playerData,playerGuid);
		if(resTowerData != LynxErrno::None)
		{
			resp.merrorId = resTowerData;
			break;
		}

		UInt16 resInlineActivity = loadInlineActivity(playerData,playerGuid);
		if(resInlineActivity != LynxErrno::None)
		{
			resp.merrorId = resInlineActivity;
			break;
		}

		UInt16 resCourageChallengeData = loadCourageChallengeData(playerData, playerGuid);
		if(resCourageChallengeData != LynxErrno::None)
		{
			resp.merrorId = resCourageChallengeData;
			break;
		}

		UInt16 resEmailData = loadEmailData(playerData, playerGuid);
		if(resEmailData != LynxErrno::None)
		{
			resp.merrorId = resEmailData;
			break;
		}

		UInt16 resCharactorData = loadCharactorData(playerData, playerGuid);
		if(resCharactorData != LynxErrno::None)
		{
			resp.merrorId = resCharactorData;
			break;
		}

		UInt16 resAchieveData = loadAchieveData(playerData, playerGuid);
		if(resAchieveData != LynxErrno::None)
		{
			resp.merrorId = resAchieveData;
			break;
		}

		UInt16 resDailyTaskData  = loadDailyTaskData(playerData, playerGuid);
		if(resDailyTaskData != LynxErrno::None)
		{
			resp.merrorId = resDailyTaskData;
			break;
		}

		UInt16 resDailyAcvData  = loadDailyActiveData(playerData, playerGuid);
		if(resDailyAcvData != LynxErrno::None)
		{
			resp.merrorId = resDailyAcvData;
			break;
		}

		UInt16 resConsortData = loadConsortData(playerData, playerGuid);
		if(resConsortData != LynxErrno::None)
		{
			resp.merrorId = resConsortData;
			break;
		}
		UInt16 resRankGame = loadRankGameData(playerData,playerGuid);
		if(resRankGame != LynxErrno::None)
		{
			resp.merrorId = resRankGame;
			break;
		}
		UInt16 resOnlineDay = loadOnlineDayData(playerData,playerGuid);
		if(resOnlineDay != LynxErrno::None)
		{
			resp.merrorId = resOnlineDay;
			break;
		}

		UInt16 resBuyCoin = loadBuyCoinData(playerData,playerGuid);
		if(resBuyCoin != LynxErrno::None)
		{
			resp.merrorId = resBuyCoin;
			break;
		}
		


		UInt16 resRankGameReport = loadRankGameReport(playerData,playerGuid);
		if(resRankGameReport != LynxErrno::None)
		{
			resp.merrorId = resRankGameReport;
			break;
		}
		resp.merrorId = LynxErrno::None;
		resp.mPlayerData = playerData;
		postMsgToOutputQueue(resp, 0);
		return ;
	}while(0);
	postMsgToOutputQueue(resp, 0);

}


void 
PersistWorker::onPersistSyncBaseGuidNotify(PersistSyncBaseGuidNotify& msg)
{
	char sql[2048];
	snprintf(sql, sizeof(sql), "call BaseGuid_Update(%llu,%llu,%llu,%llu,%llu,%llu, %llu, %llu, %llu)", msg.mPlayerGuid,msg.mItemGuid,msg.mLansquenetGuid,
		msg.mGuildGuid,msg.mEmailGuid,msg.mMaterialGuid, msg.mCharactorGuid, msg.mConsortGuid, msg.mTicketGuid);
	//LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		LOG_WARN("Failed to update baseGuid!");
	}
	else
	{
		
	}
}

void 
PersistWorker::onPersistLoadPlayerGuidReq(PersistLoadPlayerGuidReq& msg)
{
	Guid accountId = msg.mAccountId;

	PersistLoadPlayerGuidResp resp;
	resp.mConnId = msg.mConnId;
	resp.mAccountName = msg.mAccountName;
	resp.mAccountId = msg.mAccountId;

	// 数据库校验
	char sql[2048];
	snprintf(sql, sizeof(sql), "call Account_LoadPlayerId(%llu)", accountId);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!result)
	{
		mDBInterface.freeResult(&rs);
		resp.merrorId = LynxErrno::SqlExecuteFail;     
		postMsgToOutputQueue(resp, 0);
		return;
	}
	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	if(row)
	{
		resp.mPlayerGuid = lynxAtoi<Guid>(row[0]);

		mDBInterface.freeResult(&rs);

		resp.merrorId = LynxErrno::None;
		postMsgToOutputQueue(resp, 0);            
		return;
	}
	else
	{
		mDBInterface.freeResult(&rs);

		resp.merrorId = LynxErrno::PlayerNotExist;
		postMsgToOutputQueue(resp, 0);
		return;
	}
}

void PersistWorker::onPersistCreatePlayerReq(PersistCreatePlayerReq& msg)
{
	UInt32 modelId = msg.mPlayerModelId;
	char checksql[4096];
	
	snprintf(checksql, sizeof(checksql), "select * from playerBaseData where accountID = %llu", msg.mAccountId );
	LOG_DEBUG("Sql:%s", checksql);
	bool checkRes = mDBInterface.execSql(checksql);
	
	MYSQL_RES* checkrs = mDBInterface.storeResult();
	if(!checkRes)
	{
		mDBInterface.freeResult(&checkrs);
		PersistCreatePlayerResp createPlayerResp;
		createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
		createPlayerResp.mPlayerUid = 0;
		createPlayerResp.mConnId = msg.mConnId;
		createPlayerResp.mName = msg.mName.c_str();

		postMsgToOutputQueue(createPlayerResp, 0);
		LOG_INFO("failed to create Player!");

		
		return;
	}
	else
	{

		MYSQL_ROW row = mDBInterface.fetchRow(checkrs);
		if(row)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::AccountHasExist;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");

			

			mDBInterface.freeResult(&checkrs);
			return;
		}

		mDBInterface.freeResult(&checkrs);
	}

	char sql[4096];
	snprintf(sql,sizeof(sql),"call create_player(%u,'%s',%llu)",modelId, msg.mName.c_str(), msg.mAccountId);
	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	
	if (!result)
	{
		mDBInterface.freeResult(&rs);
		PersistCreatePlayerResp createPlayerResp;
		createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
		createPlayerResp.mPlayerUid = 0;
		createPlayerResp.mConnId = msg.mConnId;
		createPlayerResp.mName = msg.mName.c_str();

		postMsgToOutputQueue(createPlayerResp, 0);
		LOG_INFO("failed to create Player!");

		
		return;

	}
	else
	{
		
		MYSQL_ROW row = mDBInterface.fetchRow(rs);
		UInt64 playerUid = lynxAtoi<Guid>(row[0]);
		mDBInterface.freeResult(&rs);

		//创建玩家成功，需要初始化玩家技能数据表

		bool skillRes = onInitSkillData(playerUid, modelId, msg.mConnId);

		if(!skillRes)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
			
			return;
		}


		bool resEquip = onInitHeroEquipData(playerUid, modelId, msg.mConnId);

		if(!resEquip)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
			
			return;
		}

		bool resJewelry = onInitHeroJewelryData(playerUid,modelId,msg.mConnId);

		if(!resJewelry)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
			
			return;
		}

		bool resServantBattle = onInitServantData(playerUid,modelId,msg.mConnId);

		if(!resServantBattle)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
			
			return;
		}

		//插入角色默认时装
		bool resFashionsInsert = onInitFashionData(playerUid,modelId);
		if(!resFashionsInsert)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
		
			return;
		}


		bool resCharactorInsert = onInitCharactorData(playerUid, modelId);
		if(!resCharactorInsert)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
			
			return;
		}

		UInt64 achtime = TimeUtil::getTimeMilliSec();
		

		bool resInitAchieve = onInitAchieveData(playerUid, modelId);
		if(!resInitAchieve)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
			
			return;
		}

		UInt64 achtime2 = TimeUtil::getTimeMilliSec();
		LOG_INFO("end initachievedata cost ! %llu",achtime2-achtime);

		bool resInitDailyTask = onInitDailyTaskData(playerUid, modelId);
		if(!resInitDailyTask)
		{
			PersistCreatePlayerResp createPlayerResp;
			createPlayerResp.mRes = LynxErrno::SqlExecuteFail;
			createPlayerResp.mPlayerUid = 0;
			createPlayerResp.mConnId = msg.mConnId;
			createPlayerResp.mName = msg.mName.c_str();

			postMsgToOutputQueue(createPlayerResp, 0);
			LOG_INFO("failed to create Player!");
			
			return;
		}
	
		UInt64 achtime3 = TimeUtil::getTimeMilliSec();
		LOG_INFO("end initdailytask cost ! %llu",achtime3-achtime2);
		
		PersistCreatePlayerResp createPlayerResp;
		createPlayerResp.mPlayerUid = playerUid;
		createPlayerResp.mRes = LynxErrno::None;
		createPlayerResp.mConnId = msg.mConnId;
		createPlayerResp.mName = msg.mName.c_str();
		createPlayerResp.mPlayerModelId = msg.mPlayerModelId;

		postMsgToOutputQueue(createPlayerResp, 0);
		LOG_INFO("succeed to create Player %llu !",createPlayerResp.mPlayerUid);
		
	
	}
		
}


void PersistWorker::onLoadPlayerByAccount(PersistLoadPlayerByAccount & msg)
{
	UInt64 accountId = msg.mAccountId;
	char sql[4096];
	snprintf(sql,sizeof(sql),"call player_loadbyaccount(%llu)",accountId);
	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();

	if (!result)
	{
		mDBInterface.freeResult(&rs);
		PersistLoadPlayerByAccountResp loadPlayerResp;
		loadPlayerResp.mAccountId =accountId ;
        loadPlayerResp.mConnId = msg.mConnId;
		loadPlayerResp.mErrorId  = LynxErrno::SqlExecuteFail;

		postMsgToOutputQueue(loadPlayerResp, 0);
		LOG_INFO("player_loadbyaccount() table fail ");
		return;

	}
	else
	{
	
		MYSQL_ROW row = mDBInterface.fetchRow(rs);
		if(!row)
		{
			mDBInterface.freeResult(&rs);
			PersistLoadPlayerByAccountResp loadPlayerResp;
			loadPlayerResp.mAccountId =accountId ;
			loadPlayerResp.mConnId = msg.mConnId;
			loadPlayerResp.mErrorId  = LynxErrno::PlayerNotExist;

			postMsgToOutputQueue(loadPlayerResp, 0);
			LOG_INFO("have not create Player!");
			return;
		}

		UInt64 playerUid = lynxAtoi<Guid>(row[0]);
		mDBInterface.freeResult(&rs);

		PersistLoadPlayerByAccountResp loadPlayerResp;
		loadPlayerResp.mAccountId =accountId ;
		loadPlayerResp.mConnId = msg.mConnId;
		loadPlayerResp.mPlayerUid = playerUid;
		loadPlayerResp.mErrorId  = LynxErrno::None;

		postMsgToOutputQueue(loadPlayerResp, 0);
		LOG_INFO("succeed to load Player!");
	}
}

bool PersistWorker::onInitHeroEquipData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId)
{
	Vector<HeroEquipData> heroEquipVec;
	
	initHeroEquipLock(heroEquipVec,modelId);

	char equipSql[8192] = {0};

	
	snprintf(equipSql,sizeof(equipSql),"call heroequipupdate(%llu,   %u,%u,%u,%u,%u,%u,  %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu, %llu,%llu,%llu,%llu,%llu,%llu, %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu,  %u,%u,%u,%u,%u,%u,  %u,%u,%u,%u,%u,%u)",
		playerUid, heroEquipVec[0].m_nEquipLv, heroEquipVec[1].m_nEquipLv, heroEquipVec[2].m_nEquipLv, heroEquipVec[3].m_nEquipLv, heroEquipVec[4].m_nEquipLv, heroEquipVec[5].m_nEquipLv,
		
		heroEquipVec[0].m_nGemsList.getn(0)->mValue, heroEquipVec[0].m_nGemsList.getn(1)->mValue,heroEquipVec[0].m_nGemsList.getn(2)->mValue,heroEquipVec[0].m_nGemsList.getn(3)->mValue, heroEquipVec[0].m_nGemsList.getn(4)->mValue,heroEquipVec[0].m_nGemsList.getn(5)->mValue,
		
		heroEquipVec[1].m_nGemsList.getn(0)->mValue, heroEquipVec[1].m_nGemsList.getn(1)->mValue,heroEquipVec[1].m_nGemsList.getn(2)->mValue,heroEquipVec[1].m_nGemsList.getn(3)->mValue, heroEquipVec[1].m_nGemsList.getn(4)->mValue,heroEquipVec[1].m_nGemsList.getn(5)->mValue,
		
		heroEquipVec[2].m_nGemsList.getn(0)->mValue, heroEquipVec[2].m_nGemsList.getn(1)->mValue ,heroEquipVec[2].m_nGemsList.getn(2)->mValue ,heroEquipVec[2].m_nGemsList.getn(3)->mValue, heroEquipVec[2].m_nGemsList.getn(4)->mValue, heroEquipVec[2].m_nGemsList.getn(5)->mValue,
		heroEquipVec[3].m_nGemsList.getn(0)->mValue, heroEquipVec[3].m_nGemsList.getn(1)->mValue ,heroEquipVec[3].m_nGemsList.getn(2)->mValue ,heroEquipVec[3].m_nGemsList.getn(3)->mValue, heroEquipVec[3].m_nGemsList.getn(4)->mValue, heroEquipVec[3].m_nGemsList.getn(5)->mValue,
		heroEquipVec[4].m_nGemsList.getn(0)->mValue, heroEquipVec[4].m_nGemsList.getn(1)->mValue, heroEquipVec[4].m_nGemsList.getn(2)->mValue, heroEquipVec[4].m_nGemsList.getn(3)->mValue, heroEquipVec[4].m_nGemsList.getn(4)->mValue, heroEquipVec[4].m_nGemsList.getn(5)->mValue,
		heroEquipVec[5].m_nGemsList.getn(0)->mValue, heroEquipVec[5].m_nGemsList.getn(1)->mValue, heroEquipVec[5].m_nGemsList.getn(2)->mValue, heroEquipVec[5].m_nGemsList.getn(3)->mValue, heroEquipVec[5].m_nGemsList.getn(4)->mValue, heroEquipVec[5].m_nGemsList.getn(5)->mValue,
		heroEquipVec[0].m_nEquipId,heroEquipVec[1].m_nEquipId,heroEquipVec[2].m_nEquipId,heroEquipVec[3].m_nEquipId,heroEquipVec[4].m_nEquipId,heroEquipVec[5].m_nEquipId,
		heroEquipVec[0].m_nActiveBit, heroEquipVec[1].m_nActiveBit, heroEquipVec[2].m_nActiveBit, heroEquipVec[3].m_nActiveBit, heroEquipVec[4].m_nActiveBit, heroEquipVec[5].m_nActiveBit,
		heroEquipVec[0].m_nStep, heroEquipVec[1].m_nStep, heroEquipVec[2].m_nStep, heroEquipVec[3].m_nStep, heroEquipVec[4].m_nStep, heroEquipVec[5].m_nStep
		);

	LOG_DEBUG("Sql:%s", equipSql);

	bool equipRes = mDBInterface.execSql(equipSql);

	if (!equipRes)
	{
		LOG_INFO("update HeroEquipDb failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;


	}
}

bool PersistWorker::onInitSkillData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId)
{
	HeroTemplate *heroTemp = HERO_TABLE().get(modelId);

	List<UInt64> & learnSkills = heroTemp->m_listLearnSkills;
	List<UInt64> & originSkills = heroTemp->m_listOriginSkills;

	Vector<SkillData> skillDataVec;

	for(List<UInt64>::Iter * originSkillIter = originSkills.begin(); originSkillIter != NULL;
		originSkillIter = originSkills.next(originSkillIter))
	{
		SkillLevelTemplate * skillLvTemp = SKILLLEVEL_TABLE().reverseGet(originSkillIter->mValue,1);
		
		//普攻或者跳跃，跳过这个循环 1是普工，2是跳跃
		if(skillLvTemp->mEquip == 1 || skillLvTemp->mEquip == 2)
		{
			continue;
		}
		
		//11,12,13分别表示技能固定位置，未装备的表示0
		SkillData skillData;
		
		skillData.m_nCD = skillLvTemp->mCD1;
		skillData.m_nID = originSkillIter->mValue;
		skillData.m_nLevel = 1;
		skillData.m_nEquipPos = skillLvTemp->mEquip;
		skillDataVec.push_back(skillData);
	}

	for(List<UInt64>::Iter * learnSkillIter = learnSkills.begin(); learnSkillIter != NULL;
		learnSkillIter = learnSkills.next(learnSkillIter))
	{
		SkillData skillData;
		skillData.m_nCD = 0;
		skillData.m_nID = learnSkillIter->mValue;
		skillData.m_nLevel = 0;
		skillData.m_nEquipPos = 0;

		skillDataVec.push_back(skillData);
	}

	for(UInt32 i = skillDataVec.size(); i < 10; i++)
	{
		SkillData skillData;
		skillData.m_nCD = 0;
		skillData.m_nEquipPos = 0;
		skillData.m_nID = 0;
		skillData.m_nLevel = 0;

		skillDataVec.push_back(skillData);
	}

	char sqlSkill[4096];

	snprintf(sqlSkill,sizeof(sqlSkill),"call SkillList_Update(%llu, %llu, %u, %u, %u, %llu, %u, %u, %u, %llu, %u, %u, %u,	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u)",
		playerUid,skillDataVec[0].m_nID,skillDataVec[0].m_nCD, skillDataVec[0].m_nLevel, skillDataVec[0].m_nEquipPos,
		skillDataVec[1].m_nID,skillDataVec[1].m_nCD, skillDataVec[1].m_nLevel, skillDataVec[1].m_nEquipPos,
		skillDataVec[2].m_nID,skillDataVec[2].m_nCD, skillDataVec[2].m_nLevel, skillDataVec[2].m_nEquipPos,
		skillDataVec[3].m_nID,skillDataVec[3].m_nCD, skillDataVec[3].m_nLevel, skillDataVec[3].m_nEquipPos,
		skillDataVec[4].m_nID,skillDataVec[4].m_nCD, skillDataVec[4].m_nLevel, skillDataVec[4].m_nEquipPos,
		skillDataVec[5].m_nID,skillDataVec[5].m_nCD, skillDataVec[5].m_nLevel, skillDataVec[5].m_nEquipPos,
		skillDataVec[6].m_nID,skillDataVec[6].m_nCD, skillDataVec[6].m_nLevel, skillDataVec[6].m_nEquipPos,
		skillDataVec[7].m_nID,skillDataVec[7].m_nCD, skillDataVec[7].m_nLevel, skillDataVec[7].m_nEquipPos,
		skillDataVec[8].m_nID,skillDataVec[8].m_nCD, skillDataVec[8].m_nLevel, skillDataVec[8].m_nEquipPos,
		skillDataVec[9].m_nID,skillDataVec[9].m_nCD, skillDataVec[9].m_nLevel, skillDataVec[9].m_nEquipPos);

	LOG_DEBUG("Sql:%s", sqlSkill);

	bool resultSkill = mDBInterface.execSql(sqlSkill);
	if (!resultSkill)
	{
		LOG_INFO("skillUpdate failed playerUid: %llu",playerUid);

	

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;
	

	}
}


bool PersistWorker::onInitHeroJewelryData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId)
{
	HeroEquipRuleTemplate * heroEquipRuleTemp = HEROEQUIPRULE_TABLE().get(modelId);

	JewelryEquipData jewelryEquipData;

	if(heroEquipRuleTemp->mJewelry1UnlockLv <= 1)
	{
		jewelryEquipData.mJewelryBit = jewelryEquipData.mJewelryBit | Jewelry1OpenBit;
	}
	
	if(heroEquipRuleTemp->mJewelry2UnlockLv <= 1)
	{
		jewelryEquipData.mJewelryBit = jewelryEquipData.mJewelryBit | Jewelry2OpenBit;
	}

	if(heroEquipRuleTemp->mJewelry3UnlockLv <= 1)
	{
		jewelryEquipData.mJewelryBit = jewelryEquipData.mJewelryBit | Jewelry3OpenBit;
	}

	if(heroEquipRuleTemp->mJewelry4UnlockLv <= 1)
	{
		jewelryEquipData.mJewelryBit = jewelryEquipData.mJewelryBit | Jewelry4OpenBit;
	}
	
	if(heroEquipRuleTemp->mJewelry5UnlockLv <= 1)
	{
		jewelryEquipData.mJewelryBit = jewelryEquipData.mJewelryBit | Jewelry5OpenBit;
	}

	if(heroEquipRuleTemp->mJewelry6UnlockLv <= 1)
	{
		jewelryEquipData.mJewelryBit = jewelryEquipData.mJewelryBit | Jewelry6OpenBit;
	}

	

	char jewelrySql[1024] = {0};


	snprintf(jewelrySql,sizeof(jewelrySql),"call herojewelry_update(%llu,%u,%llu,%llu,%llu,%llu,%llu,%llu)",playerUid,jewelryEquipData.mJewelryBit,
		(UInt64)0,(UInt64)0,(UInt64)0,(UInt64)0,(UInt64)0,(UInt64)0);
	

	LOG_DEBUG("Sql:%s", jewelrySql);

	bool jewelryRes = mDBInterface.execSql(jewelrySql);

	if (!jewelryRes)
	{
		LOG_INFO("update heroJewelry failed playerUid: %llu",playerUid);

		

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;


	}

	
}


bool PersistWorker::onInitServantData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId)
{
	UInt32 mainBattleBit = 0;
	
	ServantBattleOpenTemplate * servantBattleOpen1 = SERVANTBATTLEOPEN_TABLE().get(1);
	
	if(servantBattleOpen1->mLevel <= 1)
	{
		mainBattleBit |= SERVANT_MAIN1_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen2 = SERVANTBATTLEOPEN_TABLE().get(2);

	if(servantBattleOpen2->mLevel <= 1)
	{
		mainBattleBit |= SERVANT_MAIN2_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen3 = SERVANTBATTLEOPEN_TABLE().get(3);

	if(servantBattleOpen3->mLevel <= 1)
	{
		mainBattleBit |= SERVANT_MAIN3_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen4 = SERVANTBATTLEOPEN_TABLE().get(4);

	if(servantBattleOpen4->mLevel <= 1)
	{
		mainBattleBit |= SERVANT_MAIN4_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen5 = SERVANTBATTLEOPEN_TABLE().get(5);

	if(servantBattleOpen5->mLevel <= 1)
	{
		mainBattleBit |= SERVANT_MAIN5_OPEN;	
	}

	UInt32 assistBit = 0;
	ServantBattleOpenTemplate * servantBattleOpen6 = SERVANTBATTLEOPEN_TABLE().get(6);

	if(servantBattleOpen6->mLevel <= 1 && !servantBattleOpen6->mCost)
	{
		assistBit |= SERVANT_ASSIST1_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen7 = SERVANTBATTLEOPEN_TABLE().get(7);

	if(servantBattleOpen7->mLevel <= 1 && !servantBattleOpen7->mCost)
	{
		assistBit |= SERVANT_ASSIST2_OPEN;	
	}


	ServantBattleOpenTemplate * servantBattleOpen8 = SERVANTBATTLEOPEN_TABLE().get(8);

	if(servantBattleOpen8->mLevel <= 1 && !servantBattleOpen8->mCost) 
	{
		assistBit |= SERVANT_ASSIST3_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen9 = SERVANTBATTLEOPEN_TABLE().get(9);

	if(servantBattleOpen9->mLevel <= 1 && !servantBattleOpen9->mCost)
	{
		assistBit |= SERVANT_ASSIST4_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen10 = SERVANTBATTLEOPEN_TABLE().get(10);

	if(servantBattleOpen10->mLevel <= 1 && !servantBattleOpen10->mCost)
	{
		assistBit |= SERVANT_ASSIST5_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen11 = SERVANTBATTLEOPEN_TABLE().get(11);

	if(servantBattleOpen11->mLevel <= 1 && !servantBattleOpen11->mCost)
	{
		assistBit |= SERVANT_ASSIST6_OPEN;	
	}

	ServantBattleOpenTemplate * servantBattleOpen12 = SERVANTBATTLEOPEN_TABLE().get(12);

	if(servantBattleOpen12->mLevel <= 1 && !servantBattleOpen12->mCost)
	{
		assistBit |= SERVANT_ASSIST7_OPEN;	
	}


	char servantBattleSql[4096] = {0};


	snprintf(servantBattleSql,sizeof(servantBattleSql),"call servantbattle_update(%llu, %u, %llu,%llu,%llu,%llu,%llu, %u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u)",playerUid, mainBattleBit, 
		(UInt64)0,(UInt64)0, (UInt64)0, (UInt64)0, (UInt64)0,
		assistBit,
		(UInt64)0,(UInt32)1,(UInt64)0,(UInt32)1, (UInt64)0,(UInt32)1, (UInt64)0,(UInt32)1, (UInt64)0,(UInt32)1, (UInt64)0,(UInt32)1, (UInt64)0,(UInt32)1);


	LOG_DEBUG("Sql:%s", servantBattleSql);

	bool servantBattleRes = mDBInterface.execSql(servantBattleSql);

	if (!servantBattleRes)
	{
		LOG_INFO("update servantBattle failed playerUid: %llu",playerUid);



		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;


	}


	
}


bool PersistWorker::onInitFashionData(UInt64 &playerUid, UInt32 &modelId)
{
	HeroFashionTemplate * heroFashionTemp = HEROFASHION_TABLE().get(modelId);
	char herofashionSql[4096] = {0};

	UInt64 fashionUid = LogicSystem::getSingleton().generateItemGuid();

	snprintf(herofashionSql,sizeof(herofashionSql),"call fashions_insert(%llu, %llu)",playerUid, heroFashionTemp->mFashionId);


	LOG_DEBUG("Sql:%s", herofashionSql);

	bool herofashionRes = mDBInterface.execSql(herofashionSql);

	if (!herofashionRes)
	{
		LOG_INFO("insert fashions failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;


	}
	return true;
}


bool PersistWorker::onInitCharactorData(UInt64 &playerUid, UInt32 &modelId)
{
	HeroFashionTemplate * heroFashionTemp = HEROFASHION_TABLE().get(modelId);
	char charactorsql[4096] = {0};

	snprintf(charactorsql,sizeof(charactorsql),"call charactor_insert(%llu, %u,%llu)", heroFashionTemp->mFashionId,
		modelId, playerUid);


	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if (!charactorRes)
	{
		LOG_INFO("charactor_insert failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;


	}
	return true;
}

bool PersistWorker::onInitAchieveData(UInt64 &playerUid, UInt32 &modelId)
{
		//const FirstAchievementMap& firstAchMap = ACHIEVEMENT_TABLE().getFirstAchMap();
		////初始化所有成就项目到数据库
		//for(FirstAchievementMap::Iter * achIter = firstAchMap.begin();  achIter != NULL; 
		//	achIter = firstAchMap.next(achIter))
		//{
		//	AchieveData achdata;
		//	achdata.m_nEventId = achIter->mValue.mEvent;
		//	achdata.m_nAchieveId = achIter->mValue.mId;
		//	achdata.m_nArg = 0;
		//	achdata.m_nFlag = 0;
		//	bool res = initAchieveDb(playerUid, achdata);
		//	if(!res)
		//	{
		//		return false;
		//	}
		//}

	const Vector<AchieveData>& achvec = LogicSystem::getSingleton().getAchieveVec();
		char achsql[8192] = {0};

		char tmp[32]={0};
	

	snprintf(achsql, sizeof(achsql), "call initachiveall(%llu",playerUid);
	for(UInt32 i = 0; i < achvec.size(); i++)
	{
		memset(tmp, 0, sizeof(tmp));
		snprintf(tmp, sizeof(tmp), ",%llu,%llu",achvec[i].m_nEventId, achvec[i].m_nAchieveId);	
			strcat(achsql,tmp);
	}

	

	strcat(achsql,")");

	LOG_DEBUG("Sql:%s", achsql);


	

		bool achRes = mDBInterface.execSql(achsql);

		if (!achRes)
		{
			LOG_INFO("acvbarrier_update failed playerUid: %llu",playerUid);

			return false;

		}
		else
		{
			//创建玩家成功，需要初始化装备列表和装备信息

			return true;

		}


	
}

bool PersistWorker::onInitDailyTaskData(UInt64 &playerUid, UInt32 &modelId)
{
		//DailyTaskTempMap* dailyTaskMap = DAILYTASK_TABLE().getDailyTaskMap();
		//
		//
		//for(DailyTaskTempMap::Iter * dailyIter = dailyTaskMap->begin(); dailyIter != NULL; 
		//	dailyIter = dailyTaskMap->next(dailyIter))
		//{
		//	DailyTaskData dailyTaskData;
		//	dailyTaskData.m_nTempId = dailyIter->mValue.mId;
		//	dailyTaskData.m_nEventId = dailyIter->mValue.mEvent;
		//	dailyTaskData.m_nArg = 0;
		//	dailyTaskData.m_nFlag = 0;
		//	bool res = initDailyTaskDb(playerUid, dailyTaskData);
		//	if(!res)
		//	{
		//		return false;
		//	}
		//}

		////并且更新日常刷新时间到数据库
		//char timesql[4096] = {0};
		//UInt64 times = time(0);
		//snprintf(timesql,sizeof(timesql),"call dailyactive_reset(%llu, %llu)",playerUid, times);

		//LOG_DEBUG("Sql:%s", timesql);

		//bool dlyRes = mDBInterface.execSql(timesql);

		//if (!dlyRes)
		//{
		//	LOG_INFO("dailyactive_reset failed playerUid: %llu",playerUid);

		//	return false;

		//}
	
		const Vector<DailyTaskData> & dailyTaskVec = LogicSystem::getSingleton().getDailyTaskVec();

	

		char achsql[8192] = {0};

		char tmp[32]={0};
	

	snprintf(achsql, sizeof(achsql), "call initalldailytask(%llu",playerUid);
	for(UInt32 i = 0; i < dailyTaskVec.size(); i++)
	{
		memset(tmp, 0, sizeof(tmp));
		snprintf(tmp, sizeof(tmp), ",%llu",dailyTaskVec[i].m_nTempId);	
			strcat(achsql,tmp);
	}

	

	strcat(achsql,")");

	LOG_DEBUG("Sql:%s", achsql);


	

		bool achRes = mDBInterface.execSql(achsql);

		if (!achRes)
		{
			LOG_INFO("dailytask failed playerUid: %llu",playerUid);

			return false;

		}
		
	
		UInt64 times = time(0);
		char timesactive[4096] = {0};
		snprintf(timesactive,sizeof(timesactive),"call dailytask_reset(%llu, %llu)",playerUid, times);

		LOG_DEBUG("Sql:%s", timesactive);

		bool dlyRes2 = mDBInterface.execSql(timesactive);

		if (!dlyRes2)
		{
			LOG_INFO("dailytask_reset failed playerUid: %llu",playerUid);

			return false;

		}
	

		return true;
}

//每日任务更改需求，此代码保留
//bool PersistWorker::initDailyTaskDb(UInt64 &playerUid, UInt32 &modelId)
//{
//	DailyTaskTempMap * taskmap = DAILYTASK_TABLE().getDailyTaskMap();
//	
//	for(DailyTaskTempMap::Iter * taskIter = taskmap->begin(); taskIter != NULL; taskIter = taskmap->next(taskIter))
//	{
//		if(taskIter->mValue.mNeedLv > 1)
//		{
//			continue;
//		}
//
//		char tasksql[4096] = {0};
//
//		//生成每日任务id
//		UInt64 taskUid = LogicSystem::getSingleton().generateCharactorGuid();
//
//		snprintf(tasksql,sizeof(tasksql),"call dailytask_insert(%llu, %llu, %llu, %u)",taskUid, playerUid, taskIter->mValue.mId, (UInt32)0);
//
//		LOG_DEBUG("Sql:%s", tasksql);
//
//		bool taskRes = mDBInterface.execSql(tasksql);
//
//		if (!taskRes)
//		{
//			LOG_INFO("insert dailytask failed playerUid: %llu",playerUid);
//
//			return false;
//
//		}
//	}
//	
//	return true;
//}

bool PersistWorker::initDailyTaskDb(UInt64 &playerUid, const DailyTaskData & dlydata)
{

	switch(dlydata.m_nEventId)
	{
		case DLYBARRIERCNT  :
			return updateDlyBarrierCntDb(playerUid, dlydata);
			break;
		case DLYELITEBARRIRECNT:
			return updateDlyEliteBarrierCntDb(playerUid, dlydata);
			break;
		case  DLYLINGRENCALLCNT:
			return updateDlyLingRenCntDb(playerUid, dlydata);
				break;
		case DLYXIBANCALLCNT:
			return updateDlyXibanCntDb(playerUid,dlydata);
			break;
		case DLYMULTYSUCCESS:
			return updateDlyMultiSucCntDb(playerUid,dlydata);
			break;
		case DLYTELVESUCCESS:
			return updateDlyTwelveSucDb(playerUid,dlydata);
			break;
		case DLYDOMARKETCNT:
			return updateDlyChallengeZhaofuMarket(playerUid,dlydata);
				break;
		case DLYDOCOURAGECNT:
			return updateDlyDoCourage(playerUid, dlydata);
			break;
		case DLYDOWXCHALLENGE:
			return updateDlyDoWXChallenge(playerUid, dlydata);
			break;
		case DLYDOQUALIFYCNT:
			return updateDlyDoQualify(playerUid, dlydata);
			break;
		case DLYDOARIEACNT:
			return updateDlyDoArea(playerUid, dlydata);
			break;
		case DLYRHYMEENHANCECNT:
			return updateDlyRhymeEnhance(playerUid, dlydata);
			break;
		case DLYHORNORSTONELVUPCNT:
			return updateDlyHoarStone(playerUid,dlydata);
			break;
		case DLYBAICAISHEN:
			return updateDlyBaiCaiShen(playerUid, dlydata);
			break;
		case DLYBUYENERGE:
			return updateBuyEnergy(playerUid, dlydata);
			break;
		case DLYFOODCOOK:
			return updateFoodCook(playerUid, dlydata);
			break;
		case DLYCONSORTSIGN:
			return updateConsortSign(playerUid, dlydata);
			break;
			
	
	default:
		break;
	}

	return true;
	
}

bool PersistWorker::updateDailyTaskDb(UInt64 &playerUid, const DailyTaskData & dlydata)
{
	switch(dlydata.m_nEventId)
	{
		case DLYBARRIERCNT  :
			return updateDlyBarrierCntDb(playerUid, dlydata);
			break;
		case DLYELITEBARRIRECNT:
			return updateDlyEliteBarrierCntDb(playerUid, dlydata);
			break;
		case  DLYLINGRENCALLCNT:
			return updateDlyLingRenCntDb(playerUid, dlydata);
				break;
		case DLYXIBANCALLCNT:
			return updateDlyXibanCntDb(playerUid,dlydata);
			break;
		case DLYMULTYSUCCESS:
			return updateDlyMultiSucCntDb(playerUid,dlydata);
			break;
		case DLYTELVESUCCESS:
			return updateDlyTwelveSucDb(playerUid,dlydata);
			break;
		case DLYDOMARKETCNT:
			return updateDlyChallengeZhaofuMarket(playerUid,dlydata);
				break;
		case DLYDOCOURAGECNT:
			return updateDlyDoCourage(playerUid, dlydata);
			break;
		case DLYDOWXCHALLENGE:
			return updateDlyDoWXChallenge(playerUid, dlydata);
			break;
		case DLYDOQUALIFYCNT:
			return updateDlyDoQualify(playerUid, dlydata);
			break;
		case DLYDOARIEACNT:
			return updateDlyDoArea(playerUid, dlydata);
			break;
		case DLYRHYMEENHANCECNT:
			return updateDlyRhymeEnhance(playerUid, dlydata);
			break;
		case DLYHORNORSTONELVUPCNT:
			return updateDlyHoarStone(playerUid,dlydata);
			break;
		case DLYBAICAISHEN:
			return updateDlyBaiCaiShen(playerUid, dlydata);
			break;
		case DLYBUYENERGE:
			return updateBuyEnergy(playerUid, dlydata);
			break;
		case DLYFOODCOOK:
			return updateFoodCook(playerUid, dlydata);
			break;
		case DLYCONSORTSIGN:
			return updateConsortSign(playerUid, dlydata);
			break;
	
	default:
		break;
	}

	return true;

}


bool PersistWorker::updateAchBarrierDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvbarrier_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvbarrier_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateAchEliteBarrierDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acveliteba_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acveliteba_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateRhymeEnhanceDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvrhyenhance_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvrhyenhance_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateActRhymeSkillDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvrhyskillactive_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvrhyskillactive_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateActHonorstoneDb(const UInt64 &playerUid, const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvhostoneactive_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvhostoneactive_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}


bool PersistWorker::updateActGetServantDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvgetservant_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvgetservant_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateActPlayerLvDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvlvup_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvlvup_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateActVipLvDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvviplvup_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvviplvup_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}


bool PersistWorker::updateActPowerDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvpower_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvpower_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateAct4StarServantDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvstarservantcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvstarservantcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateActPurpleServantDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvqualityservantcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvqualityservantcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}


bool PersistWorker::updateDlyBarrierCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlybarrier_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlybarrier_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}	


bool PersistWorker::updateDlyEliteBarrierCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyelite_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyelite_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyLingRenCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlylingren_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlylingren_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyXibanCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyxiban_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyxiban_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyMultiSucCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlymultisuc_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlymultisuc_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyTwelveSucDb(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlytwelvesuc_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlytwelvesuc_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}


bool PersistWorker::updateDlyChallengeZhaofuMarket(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlymarket_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlymarket_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyDoCourage(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlycourage_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlycourage_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyDoWXChallenge(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlywxchallenge_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlywxchallenge_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyDoQualify(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyqualify_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyqualify_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyDoArea(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyarea_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyarea_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyRhymeEnhance(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyrhymeenhance_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyrhymeenhance_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyHoarStone(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyhoarnostone_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyhoarnostone_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateDlyBaiCaiShen(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlybaicaishen_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlybaicaishen_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateBuyEnergy(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlybuyenergy_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlybuyenergy_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateFoodCook(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyfoodcook_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyfoodcook_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

		
bool PersistWorker::updateConsortSign(const UInt64 &playerUid,  const DailyTaskData& dailydata)
{
	char dlysql[4096] = {0};

	snprintf(dlysql,sizeof(dlysql),"call dlyconsortsign_update(%llu, %llu, %u, %u)",playerUid, dailydata.m_nTempId,
		dailydata.m_nArg, dailydata.m_nFlag);

	LOG_DEBUG("Sql:%s", dlysql);

	bool dlyRes = mDBInterface.execSql(dlysql);

	if (!dlyRes)
	{
		LOG_INFO("dlyconsortsign_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}



bool PersistWorker::updateAssistUnlockDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvassistunlock_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvassistunlock_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateOwnGreenAssisDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvassistqua1cnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvassistqua1cnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateOwnBlueAssisDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvassistqua2cnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvassistqua2cnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateOwnPurpleAssisDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvassistqua3cnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvassistqua3cnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateQualifyDanlvDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvqualifydanlv_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvqualifydanlv_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateQualifySucCntDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvqualifysuccnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvqualifysuccnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateAreaSucCntDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvarenasuccnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvarenasuccnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}


bool PersistWorker::updateBuyCharactorDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvbuycharactcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvbuycharactcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}


bool PersistWorker::updateCourageSucDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvcouragesuscnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvcouragesuscnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}
}

bool PersistWorker::updateWxChallengeDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvwxchallengecnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvwxchallengecnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateUseRhymeSkillDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvuserhyskillcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvuserhyskillcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateUseRhymeSpeedDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvuserhyspeed_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvuserhyspeed_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateBatCallServantDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvcallservantcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvcallservantcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateFriendCntDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvfriendcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvfriendcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateLv5GemCntDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvlv5gemcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvlv5gemcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}


}

bool PersistWorker::updateLv10GemCntDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvlv10gemcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvlv10gemcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}



}

bool PersistWorker::updateLv15GemCntDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvlv15gemcnt_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvlv15gemcnt_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateLRCallServantDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvlingrencall_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvlingrencall_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateXBCallServantDb(const UInt64 &playerUid,  const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvxibancall_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvxibancall_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::updateMLCallServantDb(const UInt64 &playerUid, const AchieveData& achdata)
{
		char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call acvminglingcall_update(%llu, %llu, %llu, %u, %u)",playerUid, achdata.m_nEventId,
		achdata.m_nAchieveId, achdata.m_nArg, achdata.m_nFlag);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("acvminglingcall_update failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return true;

	}

}

bool PersistWorker::initAchieveDb(const UInt64 &playerUid, const AchieveData& achdata)
{
		switch(achdata.m_nEventId)
		{
			case BARRIER  :
				return updateAchBarrierDb(playerUid, achdata);
				break;

			case	  ELITEBARRIER :
				return updateAchEliteBarrierDb(playerUid, achdata);
				break;
			case	  RHYMEENHANCELV :
				return updateRhymeEnhanceDb(playerUid, achdata);
				break;
			case RHYMESKILLACTIVE:
				return updateActRhymeSkillDb(playerUid, achdata);
				break;
				
			case	  HONORSTONEACTIVE :
				return updateActHonorstoneDb(playerUid, achdata);
				break;
			case GETSERVANT:
				return updateActGetServantDb(playerUid, achdata);
				break;
				 
			case	  PLAYERLV :
				return updateActPlayerLvDb(playerUid, achdata);
				break;
			case	  VIPLV :
				return updateActVipLvDb(playerUid, achdata);
				break;
			case  POWER:
				return updateActPowerDb(playerUid, achdata);
				break;
				
			case	  OWN4STARSERVANT :
				return updateAct4StarServantDb(playerUid, achdata);
				break;
			case	  OWNPURPULESERVANT :
				return updateActPurpleServantDb(playerUid, achdata);
				break;
			case	  UNLOCKSERVANTASSIS :
				return updateAssistUnlockDb(playerUid, achdata);
				break;
			case	  OWNGREENASSIST :
				return updateOwnGreenAssisDb(playerUid, achdata);
				break;
			case OWNBLUEASSIST:
				return updateOwnBlueAssisDb(playerUid, achdata);
				break;
			case OWNPURPLEASSIST:
				return updateOwnPurpleAssisDb(playerUid, achdata);
				break;
			case QUALIFYDANLV:
				return updateQualifyDanlvDb(playerUid, achdata);
				break;
			case  QUALIFYSUCCESS:
				return updateQualifySucCntDb(playerUid, achdata);
				break;
			case  ARIEASUCCESS:
				return updateAreaSucCntDb(playerUid, achdata);
				break;
			case BUYCHARACTOR:
				return updateBuyCharactorDb(playerUid, achdata);
				break;
			case COURAGESUCCESS:
				return updateCourageSucDb(playerUid, achdata);
				break;
			case WXCHALLENGESUC:
				return updateWxChallengeDb(playerUid, achdata);
				break;
			case USERHYMESKILL:
				return updateUseRhymeSkillDb(playerUid, achdata);
				break;
			case USERHYMESPEED:
				return updateUseRhymeSpeedDb(playerUid, achdata);
				break;
			case CALLSERVANTCNT:
				return updateBatCallServantDb(playerUid, achdata);
				break;
			case FRIENDCNT:
				return updateFriendCntDb(playerUid, achdata);
				break;
			case MAXGEMLV:
				return updateLv5GemCntDb(playerUid, achdata);
				break;
			
			case LINGRENCALLSERVANT:
				return updateLRCallServantDb(playerUid, achdata);
				
				break;
			case 	XIBANCALLSERVANT:
				return updateXBCallServantDb(playerUid, achdata);
				break;
			case MINGLINGCALLSERVANT:
				return updateMLCallServantDb(playerUid, achdata);
				break;
			default:
				break;
		}

		return true;
}

bool PersistWorker::updateAchieveDb(const UInt64 &playerUid, const AchieveData& achdata)
{
	switch(achdata.m_nEventId)
	{
	case BARRIER  :
		return updateAchBarrierDb(playerUid, achdata);
		break;

	case	  ELITEBARRIER :
		return updateAchEliteBarrierDb(playerUid, achdata);
		break;
	case	  RHYMEENHANCELV :
		return updateRhymeEnhanceDb(playerUid, achdata);
		break;
	case RHYMESKILLACTIVE:
		return updateActRhymeSkillDb(playerUid, achdata);
		break;

	case	  HONORSTONEACTIVE :
		return updateActHonorstoneDb(playerUid, achdata);
		break;
	case GETSERVANT:
		return updateActGetServantDb(playerUid, achdata);
		break;

	case	  PLAYERLV :
		return updateActPlayerLvDb(playerUid, achdata);
		break;
	case	  VIPLV :
		return updateActVipLvDb(playerUid, achdata);
		break;
	case  POWER:
		return updateActPowerDb(playerUid, achdata);
		break;

	case	  OWN4STARSERVANT :
		return updateAct4StarServantDb(playerUid, achdata);
		break;
	case	  OWNPURPULESERVANT :
		return updateActPurpleServantDb(playerUid, achdata);
		break;
	case	  UNLOCKSERVANTASSIS :
		return updateAssistUnlockDb(playerUid, achdata);
		break;
	case	  OWNGREENASSIST :
		return updateOwnGreenAssisDb(playerUid, achdata);
		break;
	case OWNBLUEASSIST:
		return updateOwnBlueAssisDb(playerUid, achdata);
		break;
	case OWNPURPLEASSIST:
		return updateOwnPurpleAssisDb(playerUid, achdata);
		break;
	case QUALIFYDANLV:
		return updateQualifyDanlvDb(playerUid, achdata);
		break;
	case  QUALIFYSUCCESS:
		return updateQualifySucCntDb(playerUid, achdata);
		break;
	case  ARIEASUCCESS:
		return updateAreaSucCntDb(playerUid, achdata);
		break;
	case BUYCHARACTOR:
		return updateBuyCharactorDb(playerUid, achdata);
		break;
	case COURAGESUCCESS:
		return updateCourageSucDb(playerUid, achdata);
		break;
	case WXCHALLENGESUC:
		return updateWxChallengeDb(playerUid, achdata);
		break;
	case USERHYMESKILL:
		return updateUseRhymeSkillDb(playerUid, achdata);
		break;
	case USERHYMESPEED:
		return updateUseRhymeSpeedDb(playerUid, achdata);
		break;
	case CALLSERVANTCNT:
		return updateBatCallServantDb(playerUid, achdata);
		break;
	case FRIENDCNT:
		return updateFriendCntDb(playerUid, achdata);
		break;
	case MAXGEMLV:
		return updateLv5GemCntDb(playerUid, achdata);
		break;
	
	case LINGRENCALLSERVANT:
		return updateLRCallServantDb(playerUid, achdata);

		break;
	case 	XIBANCALLSERVANT:
		return updateXBCallServantDb(playerUid, achdata);
		break;
	case MINGLINGCALLSERVANT:
		return updateMLCallServantDb(playerUid, achdata);
		break;
	default:
		break;
	}

	return true;
}


void PersistWorker::updateAchOfflineFriendCnt(OffLineFriendUpdateMsg& msg)
{
	char achsql[4096] = {0};

	snprintf(achsql,sizeof(achsql),"call achofflinefriend_update(%llu, %llu)",msg.playerUid, msg.count);

	LOG_DEBUG("Sql:%s", achsql);

	bool achRes = mDBInterface.execSql(achsql);

	if (!achRes)
	{
		LOG_INFO("achofflinefriend_update failed playerUid: %llu",msg.playerUid);

		return ;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return ;

	}
}




void PersistWorker::initHeroEquipLock(Vector<HeroEquipData> &heroEquipVec, UInt64 modelId)
{
	
	HeroEquipRuleTemplate * heroEquipRuleTemp = HEROEQUIPRULE_TABLE().get(modelId);
	
	if(heroEquipRuleTemp->mEquip1UnlockLv <= 1)
	{
		initEffectEquip(heroEquipVec, heroEquipRuleTemp->mEquipId1);
	}
	else
	{
		initNoneEquip(heroEquipVec);
	}

	if(heroEquipRuleTemp->mEquip2UnlockLv <= 1)
	{
		initEffectEquip(heroEquipVec, heroEquipRuleTemp->mEquipId2);
	}
	else
	{
		initNoneEquip(heroEquipVec);
	}

	if(heroEquipRuleTemp->mEquip3UnlockLv <= 1)
	{
		initEffectEquip(heroEquipVec, heroEquipRuleTemp->mEquipId3);
	}
	else
	{
		initNoneEquip(heroEquipVec);
	}

	if(heroEquipRuleTemp->mEquip4UnlockLv <= 1)
	{
		initEffectEquip(heroEquipVec, heroEquipRuleTemp->mEquipId4);
	}
	else
	{
		initNoneEquip(heroEquipVec);
	}

	if(heroEquipRuleTemp->mEquip5UnlockLv <= 1)
	{
		initEffectEquip(heroEquipVec, heroEquipRuleTemp->mEquipId5);
	}
	else
	{
		initNoneEquip(heroEquipVec);
	}

	if(heroEquipRuleTemp->mEquip6UnlockLv <= 1)
	{
		initEffectEquip(heroEquipVec, heroEquipRuleTemp->mEquipId6);
	}
	else
	{
		initNoneEquip(heroEquipVec);
	}
	
}

void PersistWorker::initNoneEquip(Vector<HeroEquipData> &heroEquipVec)
{
	HeroEquipData heroEquipData;
	heroEquipData.m_nEquipId = 0;
	heroEquipData.m_nEquipLv = 0;
	heroEquipData.m_nActiveBit = AllGemLock;

	for(UInt32 i = 0; i < 6; i++)
	{
		UInt64 gemID = 0;
		heroEquipData.m_nGemsList.insertTail(gemID);
	}

	heroEquipVec.push_back(heroEquipData);
	
}

void PersistWorker::initEffectEquip(Vector<HeroEquipData> &heroEquipVec, UInt64& equipId)
{
	HeroEquipData heroEquipData;
	heroEquipData.m_nEquipId = equipId;
	heroEquipData.m_nEquipLv = 1;

	for(UInt32 i = 0; i < 6; i++)
	{
		UInt64 gemID = 0;
		heroEquipData.m_nGemsList.insertTail(gemID);
	}

	EquipGemRuleTemplate * equipGemRuleTemp = EQUIPGEMRULE_TABLE().get(equipId);

	
	initEquipGemBit(heroEquipData, equipGemRuleTemp->mGemLevel1, equipGemRuleTemp->mCondKey1, Gem1SlotBit);

	initEquipGemBit(heroEquipData, equipGemRuleTemp->mGemLevel2, equipGemRuleTemp->mCondKey2,Gem2SlotBit);

	initEquipGemBit(heroEquipData, equipGemRuleTemp->mGemLevel3, equipGemRuleTemp->mCondKey3, Gem3SlotBit);

	initEquipGemBit(heroEquipData, equipGemRuleTemp->mGemLevel4, equipGemRuleTemp->mCondKey4, Gem4SlotBit);

	initEquipGemBit(heroEquipData, equipGemRuleTemp->mGemLevel5, equipGemRuleTemp->mCondKey5,Gem5SlotBit);

	initEquipGemBit(heroEquipData, equipGemRuleTemp->mGemLevel6, equipGemRuleTemp->mCondKey6,Gem6SlotBit);


	heroEquipVec.push_back(heroEquipData);
}


void PersistWorker::initEquipGemBit(HeroEquipData &heroEquipData,  UInt32& level,UInt32 & condKey, UInt32 gemSlotBit)
{
	if(level <= 1 && condKey == 0)
	{
		heroEquipData.m_nActiveBit = heroEquipData.m_nActiveBit | gemSlotBit;
	}
}


void PersistWorker::onPersistManagerSetDirtyReq(PersistManagerSetDirtyReq & msg)
{
	PersistManagerSetDirtyResp respMsg;
	respMsg.mDirtyBit = msg.mDirtyBit;
	respMsg.mPlayerUid = msg.mPlayerUid;

	postMsgToOutputQueue(respMsg,(UInt16)msg.mPlayerUid);
}

void PersistWorker::onPersistJewelryDelReq(PersistDelJewelryNotify & msg)
{
	char sql[4096];
	snprintf(sql, sizeof(sql), "call jewelry_delete(%llu)", msg.m_nJewelryUid
		);

	LOG_DEBUG("Sql:%s", sql);
	bool jewelryRes = mDBInterface.execSql(sql);

	if (!jewelryRes)
	{
		LOG_INFO("delete jewelrydata failed playerUid: %llu",msg.m_nPlayerUid);

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		LOG_INFO("delete jewelrydata success playerUid: %llu",msg.m_nPlayerUid);

	}


	
}

void PersistWorker::onPersistJewelryAddReq(PersistAddJewelryNotify & msg)
{
	
	List<JewelryAttr>& attrList = msg.m_jewelryData.m_randomAttrList;
	
	std::stringstream mystream;
	std::string randomDataStr;
	for(List<JewelryAttr>::Iter * jewelryAttrIter = attrList.begin(); jewelryAttrIter != NULL;
		jewelryAttrIter = attrList.next(jewelryAttrIter))
	{
		mystream.clear();
		mystream.str("");
		mystream << jewelryAttrIter->mValue.mRandomID << ','<< jewelryAttrIter->mValue.mKey << ','<< jewelryAttrIter->mValue.mValue<<';';
		std::string randomStr;
		
		mystream >> randomStr;
		randomDataStr = randomDataStr + randomStr ;
	}

	randomDataStr = randomDataStr.substr(0,randomDataStr.size() -1);

	char sql[4096];
	snprintf(sql, sizeof(sql), "call jewelry_insert(%llu, %llu, '%s',%u,%u, %llu, %u, %u, %u, %u)", msg.m_jewelryData.m_nJewelryUid,
		msg.m_jewelryData.m_nJewelryId,randomDataStr.c_str(),msg.m_jewelryData.m_staticAttrKey,msg.m_jewelryData.m_staticAttrValue,msg.m_nPlayerUid,
		msg.m_jewelryData.m_purplePoints,msg.m_jewelryData.m_purpleTimes, msg.m_jewelryData.m_orangePoints, msg.m_jewelryData.m_orangeTimes
		);
	LOG_DEBUG("Sql:%s", sql);
	bool jewelryRes = mDBInterface.execSql(sql);
	
	if (!jewelryRes)
	{
		LOG_INFO("insert jewelrydata failed playerUid: %llu,   msg.m_jewelryData.m_nJewelryUId ",msg.m_nPlayerUid,
			msg.m_jewelryData.m_nJewelryUid);

		cout << "now memory baseGuid is :  " << LogicSystem::getSingleton().getItemGuid() <<endl;
		assert(false);

	}
	else
	{
		//插入饰品成功

		LOG_INFO("insert jewelrydata success playerUid: %llu,    msg.m_jewelryData.m_nJewelryUId ",msg.m_nPlayerUid,
			msg.m_jewelryData.m_nJewelryUid);
		cout << "now memory baseGuid is :  " << LogicSystem::getSingleton().getItemGuid() <<endl;
	}


	
}

void PersistWorker::onPerisitJewelryUpdateReq(PersistUpdateJewelryNotify & msg)
{
	List<JewelryAttr>& attrList = msg.m_jewelryData.m_randomAttrList;

	std::stringstream mystream;
	std::string randomDataStr;
	for(List<JewelryAttr>::Iter * jewelryAttrIter = attrList.begin(); jewelryAttrIter != NULL;
		jewelryAttrIter = attrList.next(jewelryAttrIter))
	{
		mystream.clear();
		mystream.str("");
		mystream << jewelryAttrIter->mValue.mRandomID << ','<< jewelryAttrIter->mValue.mKey << ','<< jewelryAttrIter->mValue.mValue<<';';
		std::string randomStr;

		mystream >> randomStr;
		randomDataStr = randomDataStr + randomStr ;
	}

	randomDataStr = randomDataStr.substr(0,randomDataStr.size() -1);

	char sql[4096];
	snprintf(sql, sizeof(sql), "call jewelry_update(%llu, %llu, '%s',%u,%u, %llu, %u, %u, %u, %u)", msg.m_jewelryData.m_nJewelryUid,
		msg.m_jewelryData.m_nJewelryId,randomDataStr.c_str(),msg.m_jewelryData.m_staticAttrKey,msg.m_jewelryData.m_staticAttrValue,msg.m_nPlayerUid,
		msg.m_jewelryData.m_purplePoints,msg.m_jewelryData.m_purpleTimes, msg.m_jewelryData.m_orangePoints, msg.m_jewelryData.m_orangeTimes
		);
	LOG_DEBUG("Sql:%s", sql);
	bool jewelryRes = mDBInterface.execSql(sql);

	if (!jewelryRes)
	{
		LOG_INFO("update jewelrydata failed playerUid: %llu",msg.m_nPlayerUid);
		assert(false);
	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		LOG_INFO("update jewelrydata success playerUid: %llu",msg.m_nPlayerUid);

	}


	
}


void PersistWorker::onPerisitJewelryEquipUpdateReq(PersistUpdateJewelryEquipNotify & msg)
{
	Vector<UInt64> jewelrySlotVec;
	for(List<UInt64>::Iter * jewelrySlotIter = msg.m_jewelryEquipData.mJewelryList.begin(); jewelrySlotIter != NULL;
		jewelrySlotIter = msg.m_jewelryEquipData.mJewelryList.next(jewelrySlotIter))
	{
		jewelrySlotVec.push_back(jewelrySlotIter->mValue);
	}

	char jewelrySql[1024] = {0};


	snprintf(jewelrySql,sizeof(jewelrySql),"call herojewelry_update(%llu,%u,%llu,%llu,%llu,%llu,%llu,%llu)",msg.m_nPlayerUid,msg.m_jewelryEquipData.mJewelryBit,
		jewelrySlotVec[0],jewelrySlotVec[1],jewelrySlotVec[2],jewelrySlotVec[3],jewelrySlotVec[4],jewelrySlotVec[5]);


	LOG_DEBUG("Sql:%s", jewelrySql);

	bool jewelryRes = mDBInterface.execSql(jewelrySql);

	if (!jewelryRes)
	{
		LOG_INFO("update heroJewelry failed playerUid: %llu",msg.m_nPlayerUid);
		assert(false);
		

		return ;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息


		return ;


	}
}


void PersistWorker::onPersistServantAddReq(PersistAddServantNotify & msg)
{
	char servantSql[8192] = {0};
	
	Vector<UInt64> treasureVec;
	for(List<UInt64>::Iter * treasureIter = msg.m_servantData.equipTreasures.begin(); treasureIter != NULL; 
		treasureIter =  msg.m_servantData.equipTreasures.next(treasureIter))
	{
		treasureVec.push_back(treasureIter->mValue);
	}

	snprintf(servantSql,sizeof(servantSql),"call servant_insert(%llu,%llu,%llu, %u,%u,%u,%u,  %llu,%llu,%llu,%llu, %u)",
		msg.m_servantData.servantUid, msg.m_nPlayerUid, msg.m_servantData.servantId, 
		msg.m_servantData.pieceCount, msg.m_servantData.level, msg.m_servantData.star, msg.m_servantData.floor,
		treasureVec[0], treasureVec[1], treasureVec[2], treasureVec[3], msg.m_servantData.lvexp
		);


	LOG_DEBUG("Sql:%s", servantSql);

	bool servantRes = mDBInterface.execSql(servantSql);

	if (!servantRes)
	{
		LOG_INFO("add servant failed playerUid: %llu",msg.m_nPlayerUid);



		return ;

	}
	else
	{
		
		LOG_INFO("add servant success playerUid: %llu",msg.m_nPlayerUid);

		return ;


	}
}


void PersistWorker::onPersistServantTreasureAddReq(PersistAddServantTreasureNotify & msg)
{
	char treasureSql[8192] = {0};
	
	

	snprintf(treasureSql,sizeof(treasureSql),"call servanttreasure_insert(%llu,%llu,%llu, %u)",
		msg.m_servantTreasure.treasureUid, msg.m_servantTreasure.treasureId, msg.m_nPlayerUid, msg.m_servantTreasure.count);


	LOG_DEBUG("Sql:%s", treasureSql);

	bool servantRes = mDBInterface.execSql(treasureSql);

	if (!servantRes)
	{
		LOG_INFO("add treasure failed playerUid: %llu",msg.m_nPlayerUid);



		return ;

	}
	else
	{

		LOG_INFO("add treasure success playerUid: %llu",msg.m_nPlayerUid);

		return ;


	}
}


void PersistWorker::onPersistServantTreasureUpdateReq(PersistUpdateServantTreasureNotify & msg)
{
	char treasureSql[8192] = {0};

	snprintf(treasureSql,sizeof(treasureSql),"call servanttreasure_update(%llu,%llu,%llu, %u)",
		msg.m_servantTreasure.treasureUid, msg.m_servantTreasure.treasureId, msg.m_nPlayerUid, msg.m_servantTreasure.count);


	LOG_DEBUG("Sql:%s", treasureSql);

	bool servantRes = mDBInterface.execSql(treasureSql);

	if (!servantRes)
	{
		LOG_INFO("update treasure failed playerUid: %llu",msg.m_nPlayerUid);

		return ;

	}
	else
	{

		LOG_INFO("update treasure success playerUid: %llu",msg.m_nPlayerUid);

		return ;


	}
}

void PersistWorker::onPersistUpdateAchDb(AchieveUpdateMsg & msg )
{
	updateAchieveDb(msg.playerUid, msg.achieveData);

}

void PersistWorker::onPersistUpdateDailyTaskDb(PersistDailyTaskUpdateMsg & msg)
{
	updateDailyTaskDb(msg.playerUid, msg.dailyTask);

}

void PersistWorker::onPersistUpdateDailyActiveDb(PersistDailyActiveUpdateMsg & msg)
{
	char dailyacv[8192] = {0};
	int flag1 =0, flag2 = 0, flag3 = 0, flag4 = 0;

	int index = 1;
	for(List<UInt32>::Iter * flagIter = msg.dailyacvData.flags.begin(); flagIter!= NULL;
		flagIter = msg.dailyacvData.flags.next(flagIter))
	{
		if(index == 1)
		{
			flag1 = flagIter->mValue;
		}
	
		if(index == 2)
		{
			flag2 = flagIter->mValue;
		}

		if(index == 3)
		{
			flag3 = flagIter->mValue;
		}

		if(index == 4)
		{
			flag4 = flagIter->mValue;
		}

		index++;
	}

	snprintf(dailyacv,sizeof(dailyacv),"call dailyactive_update(%llu,%u,%u, %u, %u, %u)",msg.playerUid, flag1,flag2,flag3, flag4, msg.dailyacvData.curActive);


	LOG_DEBUG("Sql:%s", dailyacv);

	bool dailyacvRes = mDBInterface.execSql(dailyacv);

	if (!dailyacvRes)
	{
		LOG_INFO("dailyactive_update failed playerUid: %llu",msg.playerUid);

		return ;

	}
	
}

void PersistWorker::onPersistUpdateActiveTimeDb(PersistDailyLastTimeUpdateMsg &msg)
{
		char resetsql[4096]={0};
		snprintf(resetsql,sizeof(resetsql),"call dailyactive_reset(%llu, %llu)", msg.playerUid, msg.time);

		LOG_DEBUG("Sql:%s", resetsql);

		bool result = mDBInterface.execSql(resetsql);
		if(!result)
		{
			LOG_INFO("dailyactive_reset failed playerUid: %llu",msg.playerUid);
		}
	
		char resettasksql[4096]={0};
		snprintf(resettasksql,sizeof(resettasksql),"call dailytask_reset(%llu, %llu)", msg.playerUid, msg.time);

		LOG_DEBUG("Sql:%s", resettasksql);

		bool result2 = mDBInterface.execSql(resettasksql);
		if(!result2)
		{
			LOG_INFO("dailyactive_reset failed playerUid: %llu",msg.playerUid);
		}
}

void PersistWorker::onPersistUpdateNewGuid(PersistGuidStepUpdateMsg & msg)
{
	char resetsql[4096]={0};
	snprintf(resetsql,sizeof(resetsql),"call guid_update(%llu, '%s')", msg.playerUid, msg.guidStr.c_str());

		LOG_DEBUG("Sql:%s", resetsql);

		bool result = mDBInterface.execSql(resetsql);
		if(!result)
		{
			LOG_INFO("guid_update failed playerUid: %llu",msg.playerUid);
		}
	
		

}

void PersistWorker::onPersistUpdateGuidFlag(PersistGuidFlagUpdateMsg & msg)
{
	char resetsql[4096]={0};
	snprintf(resetsql,sizeof(resetsql),"call guidgift_update(%llu, %u)", msg.playerUid, msg.giftflag);

		LOG_DEBUG("Sql:%s", resetsql);

		bool result = mDBInterface.execSql(resetsql);
		if(!result)
		{
			LOG_INFO("guidflag_update failed playerUid: %llu",msg.playerUid);
		}
}

//更新禁止登录时间
void PersistWorker::onPersistUpdateForbidLoginTime(PersistForbidLoginTimeUpdate & msg)
{
	char resetsql[4096]={0};
	snprintf(resetsql,sizeof(resetsql),"call forbidlogin_update(%llu, %llu, %llu)", msg.playerUid, msg.forbidlogintime, msg.forbidbegintime);

		LOG_DEBUG("Sql:%s", resetsql);

		bool result = mDBInterface.execSql(resetsql);
		if(!result)
		{
			LOG_INFO("forbidlogin_update failed playerUid: %llu",msg.playerUid);
		}
}


//更新禁言
void PersistWorker::onPersistUpdateForbidChat(PersistForbidChatUpdate & msg)
{
	char resetsql[4096]={0};
	snprintf(resetsql,sizeof(resetsql),"call forbidchat_update(%llu, %u, %llu, %llu)", msg.playerUid, msg.disableflag,
		msg.begintime, msg.timelong);

		LOG_DEBUG("Sql:%s", resetsql);

		bool result = mDBInterface.execSql(resetsql);
		if(!result)
		{
			LOG_INFO("forbidchat_update failed playerUid: %llu",msg.playerUid);
		}
}




void PersistWorker::onPersistServantUpdateReq(PersistUpdateServantNotify & msg)
{
	char servantSql[8192] = {0};

	Vector<UInt64> treasureVec;
	for(List<UInt64>::Iter * treasureIter = msg.m_servantData.equipTreasures.begin(); treasureIter != NULL; 
		treasureIter =  msg.m_servantData.equipTreasures.next(treasureIter))
	{
		treasureVec.push_back(treasureIter->mValue);
	}


	snprintf(servantSql,sizeof(servantSql),"call servant_update(%llu,%llu,%llu, %u,%u,%u,%u,  %llu,%llu,%llu,%llu,%u,%u)",
		msg.m_servantData.servantUid, msg.m_nPlayerUid, msg.m_servantData.servantId, 
		msg.m_servantData.pieceCount, msg.m_servantData.level, msg.m_servantData.star, msg.m_servantData.floor,
		treasureVec[0], treasureVec[1], treasureVec[2], treasureVec[3], msg.m_servantData.lvexp, msg.m_servantData.infolock
	);


	LOG_DEBUG("Sql:%s", servantSql);

	bool servantRes = mDBInterface.execSql(servantSql);

	if (!servantRes)
	{
		LOG_INFO("update servant failed playerUid: %llu",msg.m_nPlayerUid);



		return ;

	}
	else
	{

		LOG_INFO("update servant success playerUid: %llu",msg.m_nPlayerUid);

		return ;


	}
}


void PersistWorker::onPersistServantDelReq(PersistDelServantNotify & msg)
{

}


void PersistWorker::insertItemToDb(List<ItemData>::Iter* iter,bool &exeFailed, UInt64 playerUid)
{
	char sql[4096];
	ItemTemplate * itemTemplate = ITEM_TABLE().get(iter->mValue.m_nItemId);

	if(itemTemplate->mTypeId == 1)
	{
		UInt64 gemIds[MAXGEMCOUNT] = {0};
		int gemIndex = 0;
		for(List<Guid>::Iter * iterGems = iter->mValue.m_itemAttribute.m_listGems.begin();
			iterGems != NULL; iterGems = iter->mValue.m_itemAttribute.m_listGems.next(iterGems))
		{
			gemIds[gemIndex] = iterGems->mValue;
			gemIndex++;
		}

		IntrinsicAttribute intrinsicAttributs[MAXINTRINSICATTRCOUNT];
		int intrinsicIndex = 0;
		for(List<IntrinsicAttribute>::Iter* intrinsicIters = iter->mValue.m_itemAttribute.m_listIntrinsicAttrs.begin();
			intrinsicIters != NULL; intrinsicIters = iter->mValue.m_itemAttribute.m_listIntrinsicAttrs.next(intrinsicIters))
		{
			intrinsicAttributs[intrinsicIndex] = intrinsicIters->mValue;
			intrinsicIndex++;
		}

		RandomAttribute randomAttributs[MAXRANDOMATTRCOUNT];
		int randomIndex = 0;
		for(List<RandomAttribute>::Iter* randomIters = iter->mValue.m_itemAttribute.m_listRandomAttrs.begin();
			randomIters != NULL; randomIters = iter->mValue.m_itemAttribute.m_listRandomAttrs.next(randomIters))
		{
			randomAttributs[randomIndex] = randomIters->mValue;
			randomIndex++;
		}

		snprintf(sql, sizeof(sql), "call ItemEquip_Insert(%llu, %llu, %llu, %u, %u,%u,   %u,%u,%llu,%u,%llu,%u,  %llu,%llu,%llu,%llu,%llu,  %u,%u,%u,%u,%u,%u,%u,%u,  %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,  %u,%u,%u,%u)", 
			iter->mValue.m_nUid,
			playerUid,
			iter->mValue.m_nItemId,
			iter->mValue.m_nCurStackAmount,
			iter->mValue.m_nItemGetTime,
			iter->mValue.m_nInvalid,
			iter->mValue.m_itemAttribute.m_nEquipState,
			iter->mValue.m_itemAttribute.m_nEnhanceLevel,
			iter->mValue.m_itemAttribute.m_nEnhanceExp,
			iter->mValue.m_itemAttribute.m_nStarLevel,
			iter->mValue.m_itemAttribute.m_nStarExp,
			iter->mValue.m_itemAttribute.m_nRefineTimes,
			gemIds[0],
			gemIds[1],
			gemIds[2],
			gemIds[3],
			gemIds[4],
			intrinsicAttributs[0].m_nKey,
			intrinsicAttributs[0].m_nValue,
			intrinsicAttributs[1].m_nKey,
			intrinsicAttributs[1].m_nValue,
			intrinsicAttributs[2].m_nKey,
			intrinsicAttributs[2].m_nValue,
			intrinsicAttributs[3].m_nKey,
			intrinsicAttributs[3].m_nValue,
			randomAttributs[0].m_nKey,
			randomAttributs[0].m_nValue,
			randomAttributs[1].m_nKey,
			randomAttributs[1].m_nValue,
			randomAttributs[2].m_nKey,
			randomAttributs[2].m_nValue,
			randomAttributs[3].m_nKey,
			randomAttributs[3].m_nValue,
			randomAttributs[4].m_nKey,
			randomAttributs[4].m_nValue,
			iter->mValue.m_itemAttribute.m_nPurplePoints,
			iter->mValue.m_itemAttribute.m_nPurpleTimes,
			iter->mValue.m_itemAttribute.m_nOrangePoints,
			iter->mValue.m_itemAttribute.m_nOrangeTimes
			);
		LOG_DEBUG("Sql:%s", sql);
	}
	else
	{
		snprintf(sql, sizeof(sql), "call Item_Insert(%llu, %llu, %llu, %u, %u,%u)", 
			iter->mValue.m_nUid,
			playerUid,
			iter->mValue.m_nItemId,
			iter->mValue.m_nCurStackAmount,
			iter->mValue.m_nItemGetTime,
			iter->mValue.m_nInvalid
			);
		LOG_DEBUG("Sql:%s", sql);
	}



	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		exeFailed = true;
		LOG_INFO("insertItemToDb failed %llu",playerUid);

	}
	else
	{
		//将处理成功的物品dirty标志清零
		iter->mValue.m_nDirtyType = NODIRTY;

	}

}

void PersistWorker::updateItemToDb(List<ItemData>::Iter* iter, bool &exeFailed,UInt64 playerUid)
{
	char sql[4096];
	ItemTemplate * itemTemplate = ITEM_TABLE().get(iter->mValue.m_nItemId);

	if(itemTemplate->mTypeId == 1)
	{
		UInt64 gemIds[MAXGEMCOUNT] = {0};
		int gemIndex = 0;
		for(List<Guid>::Iter * iterGems = iter->mValue.m_itemAttribute.m_listGems.begin();
			iterGems != NULL; iterGems = iter->mValue.m_itemAttribute.m_listGems.next(iterGems))
		{
			gemIds[gemIndex] = iterGems->mValue;
			gemIndex++;
		}

		IntrinsicAttribute intrinsicAttributs[MAXINTRINSICATTRCOUNT];
		int intrinsicIndex = 0;
		for(List<IntrinsicAttribute>::Iter* intrinsicIters = iter->mValue.m_itemAttribute.m_listIntrinsicAttrs.begin();
			intrinsicIters != NULL; intrinsicIters = iter->mValue.m_itemAttribute.m_listIntrinsicAttrs.next(intrinsicIters))
		{
			intrinsicAttributs[intrinsicIndex] = intrinsicIters->mValue;
			intrinsicIndex++;
		}

		RandomAttribute randomAttributs[MAXRANDOMATTRCOUNT];
		int randomIndex = 0;
		for(List<RandomAttribute>::Iter* randomIters = iter->mValue.m_itemAttribute.m_listRandomAttrs.begin();
			randomIters != NULL; randomIters = iter->mValue.m_itemAttribute.m_listRandomAttrs.next(randomIters))
		{
			randomAttributs[randomIndex] = randomIters->mValue;
			randomIndex++;
		}

		snprintf(sql, sizeof(sql), "call ItemEquip_Update(%llu, %u,   %u,%u,%llu,%u,%llu,%u,  %llu,%llu,%llu,%llu,%llu,  %u,%u,%u,%u,%u,%u,%u,%u,  %u,%u,%u,%u,%u, %u,%u,%u,%u,%u,%u,%u,%u,%u)", 
			iter->mValue.m_nUid,

			iter->mValue.m_nCurStackAmount,

			iter->mValue.m_itemAttribute.m_nEquipState,
			iter->mValue.m_itemAttribute.m_nEnhanceLevel,
			iter->mValue.m_itemAttribute.m_nEnhanceExp,
			iter->mValue.m_itemAttribute.m_nStarLevel,
			iter->mValue.m_itemAttribute.m_nStarExp,
			iter->mValue.m_itemAttribute.m_nRefineTimes,
			gemIds[0],
			gemIds[1],
			gemIds[2],
			gemIds[3],
			gemIds[4],
			intrinsicAttributs[0].m_nKey,
			intrinsicAttributs[0].m_nValue,
			intrinsicAttributs[1].m_nKey,
			intrinsicAttributs[1].m_nValue,
			intrinsicAttributs[2].m_nKey,
			intrinsicAttributs[2].m_nValue,
			intrinsicAttributs[3].m_nKey,
			intrinsicAttributs[3].m_nValue,
			randomAttributs[0].m_nKey,
			randomAttributs[0].m_nValue,
			randomAttributs[1].m_nKey,
			randomAttributs[1].m_nValue,
			randomAttributs[2].m_nKey,
			randomAttributs[2].m_nValue,
			randomAttributs[3].m_nKey,
			randomAttributs[3].m_nValue,
			randomAttributs[4].m_nKey,
			randomAttributs[4].m_nValue,
			iter->mValue.m_itemAttribute.m_nPurplePoints,
			iter->mValue.m_itemAttribute.m_nPurpleTimes,
			iter->mValue.m_itemAttribute.m_nOrangePoints,
			iter->mValue.m_itemAttribute.m_nOrangeTimes
			);
		LOG_DEBUG("Sql:%s", sql);
	}
	else
	{
		snprintf(sql, sizeof(sql), "call Item_Update(%llu,%u)", 
			iter->mValue.m_nUid,		
			iter->mValue.m_nCurStackAmount	
			);
		LOG_DEBUG("Sql:%s", sql);
	}



	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		exeFailed = true;
		LOG_INFO("updateItemToDb failed %llu",playerUid);

	}
	else
	{
		//将处理成功的物品dirty标志清零
		iter->mValue.m_nDirtyType = NODIRTY;

	}
}

bool PersistWorker::dealBaseDataSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	PlayerBaseData * pBaseData = mPlayer->getPlayerInfoManager().getPlayerBaseData();

	UInt32 sweepCountArry[3] = {0};
	int arryIndex = 0;
	for(List<UInt32>::Iter* iterList = pBaseData->m_listSweepCount.begin();
		iterList != NULL; iterList = pBaseData->m_listSweepCount.next(iterList))
	{
		sweepCountArry[arryIndex] = iterList->mValue;
		arryIndex++;
	}


	snprintf(sql, sizeof(sql), "call BaseData_Update(%llu,%u,'%s',%u,%llu,%u,%llu,%llu,%llu,%u,'%s',%u,%u,%u,%llu,%llu,%u,'%s',%u,%u)",
		playerUid,pBaseData->m_nModelID,
		pBaseData->m_strPlayerName.c_str(),pBaseData->m_nLevel,
		pBaseData->m_nLevelExp, pBaseData->m_nVipLevel,
		pBaseData->m_nVipExp,pBaseData->m_nGold,
		pBaseData->m_nCoin, pBaseData->m_nStrength,
		pBaseData->m_strMood.c_str(),sweepCountArry[0],sweepCountArry[1],sweepCountArry[2],
		pBaseData->m_accountID,
		pBaseData->m_nFame,pBaseData->m_nPower, pBaseData->m_strOldName.c_str(),
		pBaseData->m_nFirstLoinTime,pBaseData->m_nLoginTime);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_INFO("BaseDataUpdate %llu %s to DB failed!", 
			playerUid, pBaseData->m_strPlayerName.c_str());

		return false;
	}
	else
	{
		return true;
	}
}

bool PersistWorker::dealGemDataSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	List<GemData>* gemDataList = mPlayer->getGemManager().getGemDataList();

	bool exeFailed = false;

	for(List<GemData>::Iter* iter = gemDataList->begin(); iter != NULL; iter = gemDataList->next(iter))
	{

		if(iter->mValue.m_nDirtyType == NODIRTY)
		{
			continue;
		}

		else if(iter->mValue.m_nDirtyType == UPDATEDIRTY)
		{
			updateGemToDb(iter, exeFailed, playerUid);
		}

	}

	return !exeFailed;

}

bool PersistWorker::dealServantCostSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	PlayerServantData servantData = mPlayer->GetPlayerServantData();
	
     snprintf(sql, sizeof(sql), "call servantfood_update(%llu, %u,%u,%u,%u,%u,  %u, %u)",
		 playerUid, servantData.food1count, servantData.food2count, servantData.food3count, servantData.food4count, servantData.food5count,
		 servantData.battleStone, servantData.servantSwitch);

     LOG_DEBUG("Sql:%s", sql);

	 bool result = mDBInterface.execSql(sql);
	 if(!result)
	 {
		 LOG_INFO("ServantFoodUpdate %llu  to DB failed!", 
			 playerUid);

		 return false;
	 }
	 else
	 {
		 LOG_INFO("ServantFoodUpdate %llu  to DB true!", 
			 playerUid);
		 return true;
	 }

}

bool PersistWorker::dealServantBattleSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	PlayerServantData servantData = mPlayer->GetPlayerServantData();
	ServantBattleData battleData = servantData.servantBattleData;

	char servantBattleSql[4096] = {0};

	Vector<UInt64> mainBattleVec;
	mainBattleVec.reserve(5);

	for(List<UInt64>::Iter * mainEqIter = battleData.mainServantBattle.begin(); mainEqIter != NULL; 
		mainEqIter = battleData.mainServantBattle.next(mainEqIter))
	{
		mainBattleVec.push_back(mainEqIter->mValue);
	}

	Vector<AssistBattleData> assistBattleVec;
	assistBattleVec.reserve(7);

	for(List<AssistBattleData>::Iter * assistBattleIter = battleData.assistServantBattle.begin(); assistBattleIter != NULL;
		assistBattleIter = battleData.assistServantBattle.next(assistBattleIter))
	{
		assistBattleVec.push_back(assistBattleIter->mValue);
	}

	snprintf(servantBattleSql,sizeof(servantBattleSql),"call servantbattle_update(%llu, %u, %llu,%llu,%llu,%llu,%llu, %u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u,  %llu,%u)",playerUid, battleData.mainBattleBit, 
		mainBattleVec[0],mainBattleVec[1], mainBattleVec[2], mainBattleVec[3], mainBattleVec[4],
		battleData.assistBattleBit,
		assistBattleVec[0].equipAssist,assistBattleVec[0].quality,
		assistBattleVec[1].equipAssist,assistBattleVec[1].quality, 
		assistBattleVec[2].equipAssist,assistBattleVec[2].quality,
		assistBattleVec[3].equipAssist,assistBattleVec[3].quality,
		assistBattleVec[4].equipAssist,assistBattleVec[4].quality,
		assistBattleVec[5].equipAssist,assistBattleVec[5].quality,
		assistBattleVec[6].equipAssist,assistBattleVec[6].quality
		);


	LOG_DEBUG("Sql:%s", servantBattleSql);

	bool servantBattleRes = mDBInterface.execSql(servantBattleSql);

	if (!servantBattleRes)
	{
		LOG_INFO("update servantBattle failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		LOG_INFO("update servantBattle success playerUid: %llu",playerUid);

		return true;


	}
}


bool PersistWorker::dealChatSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	const PlayerChatData chatData = mPlayer->getPlayerChatData();
	
	std::vector<ChatData> chatDataVec;

	for(List<ChatData>::Iter * chatDataIter = chatData.chatLists.begin(); chatDataIter != NULL;
		chatDataIter = chatData.chatLists.next(chatDataIter))
	{
		chatDataVec.push_back(chatDataIter->mValue);
	}
	
	for(UInt32 i = chatDataVec.size(); i < 10; i++)
	{
		ChatData chatData;
		chatData.chatTime = 0;
		chatData.playerUid = 0;

		chatDataVec.push_back(chatData);
	}
	

	snprintf(sql,sizeof(sql),"call playerchat_update(%llu, %llu,%llu,  %llu,%llu,  %llu,%llu,  %llu,%llu,  %llu,%llu,  %llu,%llu,  %llu,%llu,  %llu,%llu,  %llu,%llu,  %llu,%llu, %u, %llu,%u)",
		playerUid,
		chatDataVec[0].playerUid, chatDataVec[0].chatTime, 
		chatDataVec[1].playerUid, chatDataVec[1].chatTime, 
		chatDataVec[2].playerUid, chatDataVec[2].chatTime, 
		chatDataVec[3].playerUid, chatDataVec[3].chatTime, 
		chatDataVec[4].playerUid, chatDataVec[4].chatTime, 
		chatDataVec[5].playerUid, chatDataVec[5].chatTime, 
		chatDataVec[6].playerUid, chatDataVec[6].chatTime,
		chatDataVec[7].playerUid, chatDataVec[7].chatTime,
		chatDataVec[8].playerUid, chatDataVec[8].chatTime,
		chatDataVec[9].playerUid, chatDataVec[9].chatTime,
		chatData.disableFlag, chatData.disableBeginTime, chatData.disableTimeLong
		);


	LOG_DEBUG("Sql:%s", sql);

	bool chatRes = mDBInterface.execSql(sql);

	if (!chatRes)
	{
		LOG_INFO("update chatdata failed playerUid: %llu",playerUid);

		return false;

	}
	else
	{
		LOG_INFO("update chatdata success playerUid: %llu",playerUid);

		return true;


	}
}


bool PersistWorker::dealRankGameSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}
	PlayerRankGameData rankGameData = mPlayer->getRankGameData();
	 LOG_INFO("rankGameData3 =  %d",rankGameData.m_Sessions);
	  LOG_INFO("rankGameData31 =  %d",rankGameData.m_WinSessions);

	snprintf(sql,sizeof(sql),"REPLACE INTO rankgame VALUES(%llu, %u,%u,%u,%u,%u,%u,%u,%u,'%s',%llu,%u)",
		playerUid,
		rankGameData.m_Score,
		rankGameData.m_Point,
		rankGameData.m_Time,
		rankGameData.m_Sessions,
		rankGameData.m_WinSessions,
		rankGameData.m_MaskNum,
		rankGameData.m_LastIndex,
		rankGameData.m_LastTime,
		rankGameData.m_PlayerIds.c_str()	,
		rankGameData.m_LastChallengeGuid,
		rankGameData.newReportFlag
		);
	

	LOG_DEBUG("Sql:%s", sql);

	bool res = mDBInterface.execSql(sql);

	if (!res)
	{
		LOG_INFO("update rankGameData failed playerUid: %llu",playerUid);
		return false;
	}
	else
	{
		return true;
	}
}


bool PersistWorker::dealBuyCoinSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}
	PlayerBuyCoinData buyCoinData = mPlayer->getBuyCoinData();
	

	snprintf(sql,sizeof(sql),"REPLACE INTO buycoin VALUES(%llu, %u,%u,%u,%u)",
		playerUid,
		buyCoinData.m_box1,
		buyCoinData.m_box2,
		buyCoinData.m_box3,	
		buyCoinData.m_refreshTime	
		);


	LOG_DEBUG("Sql:%s", sql);

	bool res = mDBInterface.execSql(sql);

	if (!res)
	{
		LOG_INFO("update rankGameData failed playerUid: %llu",playerUid);
		return false;
	}
	else
	{
		return true;
	}
}


bool PersistWorker::dealDailyResetDataSaveDb(UInt64 playerUid)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	PlayerDailyResetData dailyResetData = mPlayer->getPlayerDailyResetData();

	snprintf(sql, sizeof(sql), "REPLACE INTO playerdailyresetdata VALUES (%llu,%u,%u,	%u,%u,%u,	%u,%u,%u,%u,'%s',%u,%llu,%u,%u,%u,%u,%u,%u  ,%u,%u,%u,%u,%u,%u,%u )",
		playerUid,
		dailyResetData.m_nArenaEnterCount,
		dailyResetData.m_nAmphitheaterEnterCount,
		dailyResetData.m_nAmphitheaterWinCount,
		dailyResetData.m_nAmphitheaterLoseCount,
		dailyResetData.m_nDailyChaiCount,
		dailyResetData.m_nDailyMiCount,
		dailyResetData.m_nDailyYouCount,
		dailyResetData.m_nDailyYanCount,
		dailyResetData.m_nCouragetrialDailyPoint,
		dailyResetData.m_strCourageData.c_str(),		
		dailyResetData.m_nActivepoint,
		dailyResetData.m_nLastUpdateTime,
		dailyResetData.m_nDailyMultipleCopyCount,
		dailyResetData.m_nTwelvePalaceUnlockCount,
		dailyResetData.m_nTwelvePalaceResetFlag,
		dailyResetData.m_nClimbTowerTimes,
		dailyResetData.m_nTowerBuyTimes,
		dailyResetData.m_RewardLotteryDailyOnceFreeCount,

		dailyResetData.m_nCoinBuyTimes,
		dailyResetData.m_nCoinFreeBuyTimes,
		dailyResetData.m_nTwelvePalaceBuyTimes,
		dailyResetData.m_nFishEatTimes,
		dailyResetData.m_nRankGameLeftTimes,
		dailyResetData.m_nRankGameBuyTimes,
		dailyResetData.notUsed7
	
		);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_INFO("BaseDataUpdate %lluto DB failed!", 
			playerUid);

		return false;
	}
	else
	{
		return true;
	}
}

bool PersistWorker::dealItemSaveDb(UInt64 playerUid)
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	List<ItemData>* itemsList = mPlayer->getItemManager().getItemDatasList();

	bool exeFailed = false;

	for(List<ItemData>::Iter* iter = itemsList->begin(); iter != NULL; iter = itemsList->next(iter))
	{

		if(iter->mValue.m_nDirtyType == NODIRTY && iter->mValue.m_nCurStackAmount)
		{
			continue;
		}

		if(iter->mValue.m_nDirtyType == ADDDIRTY)
		{
			insertItemToDb(iter,exeFailed,playerUid);
		}

		else if(iter->mValue.m_nDirtyType == DELDIRTY)
		{
			delItemToDb(iter,exeFailed,playerUid);
		}

		else if(iter->mValue.m_nDirtyType == UPDATEDIRTY)
		{
			updateItemToDb(iter, exeFailed, playerUid);
		}
		else if(iter->mValue.m_nCurStackAmount == 0)
		{
			iter->mValue.m_nInvalid = 1;
			delItemToDb(iter,exeFailed,playerUid);
		}

	}

	return !exeFailed;

}

bool PersistWorker::dealSkillSaveDb(UInt64 playerUid)
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

    Vector<SkillData*> &skillDataVec = mPlayer->getSkillManager().getSkillVec();

	char sql[4096];

	snprintf(sql,sizeof(sql),"call SkillList_Update(%llu, %llu, %u, %u, %u, %llu, %u, %u, %u, %llu, %u, %u, %u,	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u, 	%llu, %u, %u, %u)",
		playerUid,skillDataVec[0]->m_nID,skillDataVec[0]->m_nCD, skillDataVec[0]->m_nLevel, skillDataVec[0]->m_nEquipPos,
		skillDataVec[1]->m_nID,skillDataVec[1]->m_nCD, skillDataVec[1]->m_nLevel, skillDataVec[1]->m_nEquipPos,
		skillDataVec[2]->m_nID,skillDataVec[2]->m_nCD, skillDataVec[2]->m_nLevel, skillDataVec[2]->m_nEquipPos,
		skillDataVec[3]->m_nID,skillDataVec[3]->m_nCD, skillDataVec[3]->m_nLevel, skillDataVec[3]->m_nEquipPos,
		skillDataVec[4]->m_nID,skillDataVec[4]->m_nCD, skillDataVec[4]->m_nLevel, skillDataVec[4]->m_nEquipPos,
		skillDataVec[5]->m_nID,skillDataVec[5]->m_nCD, skillDataVec[5]->m_nLevel, skillDataVec[5]->m_nEquipPos,
		skillDataVec[6]->m_nID,skillDataVec[6]->m_nCD, skillDataVec[6]->m_nLevel, skillDataVec[6]->m_nEquipPos,
		skillDataVec[7]->m_nID,skillDataVec[7]->m_nCD, skillDataVec[7]->m_nLevel, skillDataVec[7]->m_nEquipPos,
		skillDataVec[8]->m_nID,skillDataVec[8]->m_nCD, skillDataVec[8]->m_nLevel, skillDataVec[8]->m_nEquipPos,
		skillDataVec[9]->m_nID,skillDataVec[9]->m_nCD, skillDataVec[9]->m_nLevel, skillDataVec[9]->m_nEquipPos);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		LOG_INFO("skillUpdate failed playerUid: %llu",playerUid);
		return false;

	}
	else
	{
		return true;

	}

}

bool PersistWorker::delItemToDb(List<ItemData>::Iter* iter, bool &exeFailed,UInt64 playerUid)
{
	char sql[4096];


	snprintf(sql, sizeof(sql), "call Item_Delete(%llu)", 
		iter->mValue.m_nUid		
		);
	LOG_DEBUG("Sql:%s", sql);



	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		exeFailed = true;
		LOG_INFO("delItemToDb failed playerUid: %llu",playerUid);
		return false;

	}
	else
	{
		//将处理成功的物品dirty标志清零
		iter->mValue.m_nDirtyType = NODIRTY;
		return true;

	}
}

bool PersistWorker::dealRhymeSaveDb(UInt64 playerUid)
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	char sql[4096];

	PlayerRhymeData rhymeData = mPlayer->getPlayerData().mRhymeData;

	snprintf(sql, sizeof(sql), "call Rhyme_Update(%llu,%u,%u,%u,%llu,%u,'%s',%u,%u,%u)", playerUid, 
		rhymeData.m_RhymeExp, rhymeData.m_RhymeLv,rhymeData.m_RhymeStep,
		rhymeData.m_RhymeSoul,rhymeData.m_RhymeSkillIndex,
		rhymeData.m_RhymeAcupointStr.c_str(),rhymeData.m_RhymeGrass,
		rhymeData.m_RhymeGrassMid,
		rhymeData.m_RhymeGrassHigh
		);
	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if (!result)
	{

		LOG_INFO("rhymeData save failed playerUid: %llu",playerUid);
		return false;

	}
	else
	{
		return true;
	}
}

bool PersistWorker::dealHoarStoneSaveDb(UInt64 playerUid)
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	char sql[4096];

	PlayerHoarStoneData hoarStoneData = mPlayer->getPlayerData().mHoarStoneData;



	Vector<HoarStoneSave> hoarStoneDataVec;
	std::stringstream mystream;

	for(List<HoarStoneData>::Iter * hoarStoneIter = hoarStoneData.mHoarStoneList.begin(); hoarStoneIter != NULL; 
		hoarStoneIter = hoarStoneData.mHoarStoneList.next(hoarStoneIter))
	{
		HoarStoneSave hoarStoneSave;
		hoarStoneSave.mStoneLv = hoarStoneIter->mValue.mStoneLv;
		hoarStoneSave.mPieceCount = hoarStoneIter->mValue.mPieceCount;
		hoarStoneSave.mStoneStar = hoarStoneIter->mValue.mStoneStar;
		
		List<UInt64>::Iter * equipIter = hoarStoneIter->mValue.mEquipList.begin();
		
		mystream.clear();
		mystream.str("");
		
		std::string equipStr;
		mystream << equipIter->mValue;
		mystream >> equipStr;
	
		hoarStoneSave.mEquipStr += equipStr;
		equipIter = hoarStoneIter->mValue.mEquipList.next(equipIter);

		while(equipIter)
		{
			std::string equipStrTemp;
			mystream.clear();
			mystream.str("");

			mystream << equipIter->mValue;
			mystream >> equipStrTemp;

			hoarStoneSave.mEquipStr = hoarStoneSave.mEquipStr +';'+ equipStrTemp;

			equipIter = hoarStoneIter->mValue.mEquipList.next(equipIter);

		}
	
		hoarStoneDataVec.push_back(hoarStoneSave);
		
	}

	std::string runeDataStr;

	for(RuneDataMap::Iter * runeDataMapIter = hoarStoneData.mRuneDataMap.begin();runeDataMapIter != NULL;
		runeDataMapIter = hoarStoneData.mRuneDataMap.next(runeDataMapIter))
	{
		mystream.clear();
		mystream.str("");
		mystream << runeDataMapIter->mKey;
		std::string runeKeyStr;
		mystream >> runeKeyStr;

		mystream.clear();
		mystream.str("");
		mystream << runeDataMapIter->mValue;
		std::string runeValueStr;
		mystream >> runeValueStr;
		
		runeDataStr = runeDataStr + runeKeyStr + ',' + runeValueStr + ';';
	}

	if(!runeDataStr.empty())
	{
		runeDataStr = runeDataStr.substr(0,runeDataStr.size()-1);
	}

	snprintf(sql,
		sizeof(sql), "call Stele_Update(%llu, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u, %u,%u,   %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s','%s')",
		playerUid,
		hoarStoneDataVec[0].mStoneLv, hoarStoneDataVec[0].mPieceCount, 
		hoarStoneDataVec[1].mStoneLv, hoarStoneDataVec[1].mPieceCount,
		hoarStoneDataVec[2].mStoneLv, hoarStoneDataVec[2].mPieceCount,
		hoarStoneDataVec[3].mStoneLv, hoarStoneDataVec[3].mPieceCount,
		hoarStoneDataVec[4].mStoneLv, hoarStoneDataVec[4].mPieceCount,
		hoarStoneDataVec[5].mStoneLv, hoarStoneDataVec[5].mPieceCount,
		hoarStoneDataVec[6].mStoneLv, hoarStoneDataVec[6].mPieceCount,
		hoarStoneDataVec[7].mStoneLv, hoarStoneDataVec[7].mPieceCount,
		hoarStoneDataVec[8].mStoneLv, hoarStoneDataVec[8].mPieceCount,
		hoarStoneDataVec[9].mStoneLv, hoarStoneDataVec[9].mPieceCount,
		hoarStoneDataVec[10].mStoneLv, hoarStoneDataVec[10].mPieceCount,
		hoarStoneDataVec[11].mStoneLv, hoarStoneDataVec[11].mPieceCount,
		
		hoarStoneDataVec[0].mStoneStar,hoarStoneDataVec[1].mStoneStar,
		hoarStoneDataVec[2].mStoneStar,hoarStoneDataVec[3].mStoneStar,
		hoarStoneDataVec[4].mStoneStar,hoarStoneDataVec[5].mStoneStar,
		hoarStoneDataVec[6].mStoneStar,hoarStoneDataVec[7].mStoneStar,
		hoarStoneDataVec[8].mStoneStar,hoarStoneDataVec[9].mStoneStar,
		hoarStoneDataVec[10].mStoneStar,hoarStoneDataVec[11].mStoneStar,

		hoarStoneDataVec[0].mEquipStr.c_str(),hoarStoneDataVec[1].mEquipStr.c_str(),
		hoarStoneDataVec[2].mEquipStr.c_str(),hoarStoneDataVec[3].mEquipStr.c_str(),
		hoarStoneDataVec[4].mEquipStr.c_str(),hoarStoneDataVec[5].mEquipStr.c_str(),
		hoarStoneDataVec[6].mEquipStr.c_str(),hoarStoneDataVec[7].mEquipStr.c_str(),
		hoarStoneDataVec[8].mEquipStr.c_str(),hoarStoneDataVec[9].mEquipStr.c_str(),
		hoarStoneDataVec[10].mEquipStr.c_str(),hoarStoneDataVec[11].mEquipStr.c_str(),
		runeDataStr.c_str()
		);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if (!result)
	{

		LOG_INFO("hoarStoneData save failed playerUid: %llu",playerUid);
		return false;

	}
	else
	{
		return true;
	}
}

bool PersistWorker::dealMaterialSaveDb(UInt64 playerUid)
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	List<MaterialData>* materialList = mPlayer->getMaterialManager().getMaterialList();

	bool exeFailed = false;

	for(List<MaterialData>::Iter* iter = materialList->begin(); iter != NULL; iter = materialList->next(iter))
	{

		if(iter->mValue.m_nDirtyType == NODIRTY)
		{
			continue;
		}

		else if(iter->mValue.m_nDirtyType == UPDATEDIRTY)
		{
			updateMaterialToDb(iter, exeFailed, playerUid);
		}

	}

	return !exeFailed;

}

bool PersistWorker::dealHeroEquipSaveDb(UInt64 playerUid)
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}
	
	List<HeroEquipData> * heroEquipList = mPlayer->getHeroEquipManager().getHeroEquipList();

	Vector<HeroEquipData *> heroEquipVec;

	for(List<HeroEquipData>::Iter * heroEquipIter = heroEquipList->begin(); heroEquipIter != NULL; 
		heroEquipIter = heroEquipList->next(heroEquipIter) )
	{
		heroEquipVec.push_back(&heroEquipIter->mValue);
	}
	
	char equipSql[8192] = {0};


	snprintf(equipSql,sizeof(equipSql),"call heroequipupdate(%llu,   %u,%u,%u,%u,%u,%u,  %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu, %llu,%llu,%llu,%llu,%llu,%llu, %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu,  %llu,%llu,%llu,%llu,%llu,%llu,  %u,%u,%u,%u,%u,%u,  %u,%u,%u,%u,%u,%u)",
		playerUid, heroEquipVec[0]->m_nEquipLv, heroEquipVec[1]->m_nEquipLv, heroEquipVec[2]->m_nEquipLv, heroEquipVec[3]->m_nEquipLv, heroEquipVec[4]->m_nEquipLv, heroEquipVec[5]->m_nEquipLv,

		heroEquipVec[0]->m_nGemsList.getn(0)->mValue, heroEquipVec[0]->m_nGemsList.getn(1)->mValue,heroEquipVec[0]->m_nGemsList.getn(2)->mValue,heroEquipVec[0]->m_nGemsList.getn(3)->mValue, heroEquipVec[0]->m_nGemsList.getn(4)->mValue,heroEquipVec[0]->m_nGemsList.getn(5)->mValue,

		heroEquipVec[1]->m_nGemsList.getn(0)->mValue, heroEquipVec[1]->m_nGemsList.getn(1)->mValue,heroEquipVec[1]->m_nGemsList.getn(2)->mValue,heroEquipVec[1]->m_nGemsList.getn(3)->mValue, heroEquipVec[1]->m_nGemsList.getn(4)->mValue,heroEquipVec[1]->m_nGemsList.getn(5)->mValue,

		heroEquipVec[2]->m_nGemsList.getn(0)->mValue, heroEquipVec[2]->m_nGemsList.getn(1)->mValue ,heroEquipVec[2]->m_nGemsList.getn(2)->mValue ,heroEquipVec[2]->m_nGemsList.getn(3)->mValue, heroEquipVec[2]->m_nGemsList.getn(4)->mValue, heroEquipVec[2]->m_nGemsList.getn(5)->mValue,
		heroEquipVec[3]->m_nGemsList.getn(0)->mValue, heroEquipVec[3]->m_nGemsList.getn(1)->mValue ,heroEquipVec[3]->m_nGemsList.getn(2)->mValue ,heroEquipVec[3]->m_nGemsList.getn(3)->mValue, heroEquipVec[3]->m_nGemsList.getn(4)->mValue, heroEquipVec[3]->m_nGemsList.getn(5)->mValue,
		heroEquipVec[4]->m_nGemsList.getn(0)->mValue, heroEquipVec[4]->m_nGemsList.getn(1)->mValue, heroEquipVec[4]->m_nGemsList.getn(2)->mValue, heroEquipVec[4]->m_nGemsList.getn(3)->mValue, heroEquipVec[4]->m_nGemsList.getn(4)->mValue, heroEquipVec[4]->m_nGemsList.getn(5)->mValue,
		heroEquipVec[5]->m_nGemsList.getn(0)->mValue, heroEquipVec[5]->m_nGemsList.getn(1)->mValue, heroEquipVec[5]->m_nGemsList.getn(2)->mValue, heroEquipVec[5]->m_nGemsList.getn(3)->mValue, heroEquipVec[5]->m_nGemsList.getn(4)->mValue, heroEquipVec[5]->m_nGemsList.getn(5)->mValue,
		heroEquipVec[0]->m_nEquipId,heroEquipVec[1]->m_nEquipId,heroEquipVec[2]->m_nEquipId,heroEquipVec[3]->m_nEquipId,heroEquipVec[4]->m_nEquipId,heroEquipVec[5]->m_nEquipId,
		heroEquipVec[0]->m_nActiveBit, heroEquipVec[1]->m_nActiveBit, heroEquipVec[2]->m_nActiveBit, heroEquipVec[3]->m_nActiveBit, heroEquipVec[4]->m_nActiveBit, heroEquipVec[5]->m_nActiveBit,
		heroEquipVec[0]->m_nStep, heroEquipVec[1]->m_nStep, heroEquipVec[2]->m_nStep, heroEquipVec[3]->m_nStep, heroEquipVec[4]->m_nStep, heroEquipVec[5]->m_nStep
		);

	LOG_DEBUG("Sql:%s", equipSql);

	bool result = mDBInterface.execSql(equipSql);
	if (!result)
	{

		LOG_INFO("heroequipdata save failed playerUid: %llu",playerUid);
		return false;

	}
	else
	{
		LOG_INFO("heroequipdata save success playerUid: %llu",playerUid);
		return true;
	}

}

void PersistWorker::updateMaterialToDb(List<MaterialData>::Iter* iter, bool &exeFailed,UInt64 playerUid)
{
	char sql[4096];


	snprintf(sql, sizeof(sql), "call material_update(%llu, %llu, %llu,%u)", 
		iter->mValue.m_nUid, playerUid, iter->mValue.m_nId, iter->mValue.m_nCount		
		);
	LOG_DEBUG("Sql:%s", sql);



	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		exeFailed = true;
		LOG_INFO("delMaterialToDb failed playerUid: %llu",playerUid);
		return ;

	}
	else
	{
		//将处理成功的物品dirty标志清零
		iter->mValue.m_nDirtyType = NODIRTY;
		LOG_INFO("delMaterialToDb success playerUid: %llu",playerUid);
		return ;

	}
}

void PersistWorker::updateGemToDb(List<GemData>::Iter* iter, bool &exeFailed,UInt64 playerUid)
{
	char sql[4096];


	snprintf(sql, sizeof(sql), "call gem_update(%llu, %llu, %llu,%u)", 
		iter->mValue.m_nUid, playerUid, iter->mValue.m_nId, iter->mValue.m_nCount		
		);
	LOG_DEBUG("Sql:%s", sql);



	bool result = mDBInterface.execSql(sql);
	if (!result)
	{
		exeFailed = true;
		LOG_INFO("deal gem_update failed playerUid: %llu",playerUid);
		return ;

	}
	else
	{
		//将处理成功的物品dirty标志清零
		iter->mValue.m_nDirtyType = NODIRTY;
		LOG_INFO("deal gem_update success playerUid: %llu",playerUid);
		return ;

	}
}

void PersistWorker::onPersistOffLineSaveReq(PersistOffLineSaveReq& msg)
{
	


}


void PersistWorker::onPersistNotifyWorkerSaveDbReq(PersistNotifyWorkerSaveDbReq & msg)
{
	UInt32 dirtyBitRt = msg.mDirtyBit;
	UInt32 dirtyBit = msg.mDirtyBit;
	UInt64 playerUid = msg.mPlayerUid;

	bool exeFailed = false;
	//做basedata的存盘操作
	if(dirtyBit & BASEDATABIT)
	{
		//做database的存盘 BASEDATAINDEX
		bool result = dealBaseDataSaveDb(playerUid);

		if(result)
		{
			//处理成功后，将标记为清空

			dirtyBitRt = dirtyBitRt & (~BASEDATABIT);
		}
		else
		{
			exeFailed = true;
		}

	}

	//判断dailyrest的标志位是否有更改 DAILYRESETBIT
	if(dirtyBit & DAILYRESETBIT)
	{
		bool result = dealDailyResetDataSaveDb(playerUid);

		if(result)
		{
			//处理成功后，将标记为清空

			dirtyBitRt = dirtyBitRt & (~DAILYRESETBIT);
		}
		else
		{
			exeFailed = true;
		}
		
	}

	
	//判断skillList的标志位是否有更改 SKILLDATABIT
	if(dirtyBit & SKILLDATABIT)
	{
		bool result = dealSkillSaveDb(playerUid);

		if(result)
		{
			//处理成功，将item标记为清零

			dirtyBitRt = dirtyBitRt & (~SKILLDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	
	//判断buffList的标志位是否有更改 BUFFDATABIT
	if(dirtyBit & BUFFDATABIT)
	{

	}

	
	//判断item的标志位是否有更改 ITEMDATABIT 以后开开
	if(dirtyBit & ITEMDATABIT)
	{
 		bool result = dealItemSaveDb(playerUid);

		if(result)
		{
			//处理成功，将item标记为清零

			dirtyBitRt = dirtyBitRt & (~ITEMDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	
	//判断fashion的标志位是否有更改 FASHIONDATABIT
	if(dirtyBit & FASHIONDATABIT)
	{

	}

	
	if(dirtyBit & STAGEDATABIT)
	{
		bool result = updateStageDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~STAGEDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}


	
	//判断rhyme标志位是否有更改 rhymeDATAINDEX
	if(dirtyBit & RHYMEDATABIT)
	{
		bool result = dealRhymeSaveDb(playerUid);

		if(result)
		{
			//处理成功，将item标记为清零

			dirtyBitRt = dirtyBitRt & (~RHYMEDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	//判断hoarstone标志位是否有更改 hoarstoneBit
	if(dirtyBit & HOARSTONEDATABIT)
	{
		bool result = dealHoarStoneSaveDb(playerUid);

		if(result)
		{
			//处理成功，将item标记为清零

			dirtyBitRt = dirtyBitRt & (~HOARSTONEDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & MATERIALDATABIT)
	{
		bool result = dealMaterialSaveDb(playerUid);

		if(result)
		{
			//处理成功，将material标记为清零

			dirtyBitRt = dirtyBitRt & (~MATERIALDATABIT);

		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & HEROEQUIPDATABIT)
	{
		bool result = dealHeroEquipSaveDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~HEROEQUIPDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & GEMDATABIT)
	{
		bool result = dealGemDataSaveDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~GEMDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}



	if(dirtyBit & TOWERDATABIT)
	{
		bool result = updateTowerDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~TOWERDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & LOCALDATABIT)
	{
		bool result = updateLocalDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~LOCALDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}


	if(dirtyBit & FRIENDBEAPPLYDATABIT)
	{
		bool result = updateFriendBeApplyDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~FRIENDBEAPPLYDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}
	

	if(dirtyBit & FOODDATABIT)
	{
		bool result = updateFoodDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~FOODDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & COUNTERDATABIT)
	{
		bool result = updateLotteryDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~COUNTERDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & COURAGECHALLENGEDATABIT)
	{
		bool result = updateCourageChallengeDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~COURAGECHALLENGEDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & STRENGTHDATABIT)
	{
		bool result = updateStrengthDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~STRENGTHDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & CHAPTERUNLOCKEDTABIT)
	{
		bool result = updateChapterDataToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~CHAPTERUNLOCKEDTABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & BOXCOUNTER)
	{
		bool result = updateBoxCounterToDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~BOXCOUNTER);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & SERVANTFOODBIT)
	{
		bool result = dealServantCostSaveDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~SERVANTFOODBIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & SERVANTBATTLEBIT)
	{
		bool result = dealServantBattleSaveDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~SERVANTBATTLEBIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & CHATDATABIT)
	{
		bool result = dealChatSaveDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~CHATDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	if(dirtyBit & RANKGAMEDATABIT)
	{
		bool result = dealRankGameSaveDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~RANKGAMEDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}
	if(dirtyBit & BUYCOINDATABIT)
	{
		bool result = dealBuyCoinSaveDb(playerUid);
		if(result)
		{
			dirtyBitRt = dirtyBitRt & (~BUYCOINDATABIT);
		}
		else
		{
			exeFailed = true;
		}
	}

	PersistNotifyWorkerSaveDbResp saveDbResp;

	saveDbResp.mDirtyBit = dirtyBit;
	if(exeFailed)
	{
		saveDbResp.merrorId = LynxErrno::SqlExecuteFail;
	}
	else
	{
		saveDbResp.merrorId = LynxErrno::None;
	}

	saveDbResp.mPlayerUid = playerUid;
	postMsgToOutputQueue(saveDbResp,playerUid);

}


void 
PersistWorker::onPersistUpdataFireConfirmData(PersistFireConfirmSaveReq& msg )
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(msg.playerGuid);	
	if(!mPlayer)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", msg.playerGuid);
		return;
	}

	char sql[2048];
	snprintf(sql, sizeof(sql), "call fireconfirm_update('%u','%u','%u', %llu)",msg.FireConfirmData.m_ConfirmLevel,msg.FireConfirmData.m_ConfirnFailTimes,msg.FireConfirmData.m_ConfirnRightTimes,msg.playerGuid);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* rs = mDBInterface.storeResult();

	if(!result)
	{
		mDBInterface.freeResult(&rs);
		return;
	}
	else
	{
		mDBInterface.freeResult(&rs);
		return;
	}  
}

void 
PersistWorker::onPersistUpdateSingleProperty(PersistPlayerSinglePropertySaveReq& msg)
{
	char sql[2048] = "";
	PlayerBaseData baseData;

	
	if (msg.strFlag == "1")
	{
		Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(msg.playerGuid);
		if(!mPlayer)
		{
			LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", msg.playerGuid);
			return;
		}
		baseData = mPlayer->getPlayerBaseData();

		snprintf(sql, sizeof(sql), "call BaseData_Update(%llu,%u,'%s',%u,%llu,%u,%llu,%llu,%llu,%u,'%s',%u,%u,%u,%llu,%llu,%u,'%s')",
			baseData.m_nPlayerID,baseData.m_nModelID,baseData.m_strPlayerName.c_str(),baseData.m_nLevel,baseData.m_nLevelExp,baseData.m_nVipLevel,baseData.m_nVipExp,baseData.m_nGold,
			baseData.m_nCoin,baseData.m_nStrength,baseData.m_strMood.c_str(),7,8,9,66,baseData.m_nFame,baseData.m_nPower,
			baseData.m_strOldName.c_str());		
	}
	else if (msg.strFlag == "20")
	{
		Vector<String> strVector;
		lynxStrSplit(msg.strData, ",", strVector, true);

		UInt32 key =atoi(strVector[0].c_str()) ;
		UInt32 sValue = atoi(strVector[1].c_str());
		updateStageData(msg.playerGuid,key/100,key);
		return;

	}
	else if (msg.strFlag == "21")
	{
		Vector<String> strVector;
		lynxStrSplit(msg.strData, ",", strVector, true);

		UInt32 key =atoi(strVector[0].c_str()) ;
		UInt32 sValue = atoi(strVector[1].c_str());

		snprintf(sql, sizeof(sql), "call Counter_Update('%llu','%u','%u' )",
			msg.playerGuid,
			key,
			sValue
			);
	}
	else if (msg.strFlag == "22")
	{
		Vector<String> strVector;
		lynxStrSplit(msg.strData, ",", strVector, true);

		UInt32 key =atoi(strVector[0].c_str()) ;
		UInt32 sValue = atoi(strVector[1].c_str());
		updateRewardCounterData(msg.playerGuid,key,sValue);
		return;

	}
	else if (msg.strFlag == "23")
	{
		Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(msg.playerGuid);
		if(!mPlayer)
		{
			LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", msg.playerGuid);
			return;
		}
		PlayerFoodsData foodsData;
		foodsData = mPlayer->getFoodsData();
		snprintf(sql, sizeof(sql), "call Food_Update('%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u','%llu')",
			foodsData.beginTime,foodsData.leftTimes,foodsData.food1,foodsData.food2,foodsData.food3,foodsData.vipLeftTimes,foodsData.buyTime,foodsData.vipFoodLeftNumber,msg.playerGuid);
		
	}
	else if (msg.strFlag == "24")
	{
		Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(msg.playerGuid);
		if(!mPlayer)
		{
			LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", msg.playerGuid);
			return;
		}
		PlayerStrengthData StrengthData;
		StrengthData = mPlayer->getStrengthData();
		snprintf(sql, sizeof(sql), "call Strength_Update('%u', '%u', '%u', '%u',  '%u', '%u','%llu')",
			StrengthData.beginTime,0,StrengthData.strength,StrengthData.vipLeftTimes,StrengthData.buyTime,StrengthData.vipFoodLeftNumber,msg.playerGuid);

	}
	else if (msg.strFlag == "10000")
	{
		deleteResetStages();
	}
	bool result = true;
	if (sql != "")
	{
		LOG_DEBUG("Sql:%s", sql);
		result = mDBInterface.execSql(sql);
		MYSQL_RES* rs = mDBInterface.storeResult();
		mDBInterface.freeResult(&rs);		
	}
}

void
PersistWorker::textDb()
{
	UInt32 startTime = TimeUtil::getTimeSec();
	NameString name = "";
	char sql[2048];
	for (int i = 11; i <= 1000; i++)
	{
		char temp[100];
		snprintf(temp, sizeof(temp), "test%u", i);
		name = temp;
		Guid id = 10000000 + i;
		String passport = "123456";
		snprintf(sql, sizeof(sql), "call Passport_Insert(%llu, '%s', '%s')",
			id, name.c_str(), passport.c_str());
		mDBInterface.execSql(sql);
	}

	LOG_DEBUG("Finish insert data, use: %u", TimeUtil::getTimeSec() - startTime);
}

UInt16 PersistWorker::loadCounter(PlayerData& playerData,UInt64 playerGuid)
{

	char sql[2048] = {0};
	List<KeyValue> counter;
	snprintf(sql, sizeof(sql), "call Counter_Load(%llu)", playerGuid);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}
	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	while(row)
	{
		while(row)
		{
			KeyValue keyvlaue;
			keyvlaue.key = lynxAtoi<UInt32>(row[0]);
			keyvlaue.value = lynxAtoi<UInt32>(row[1]);

			counter.insertTail(keyvlaue);
			row = mDBInterface.fetchRow(rs);
		}

		row = mDBInterface.fetchRow(rs);
	}
	playerData.mCounterData.m_BoxCounter = counter;

	LOG_INFO("Load playerBuffList %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}	


UInt16 PersistWorker::loadCopyCounter(PlayerData& playerData,UInt64 playerGuid)
{

	char sql[2048] = {0};
	List<KeyValue> copyCounter;
	snprintf(sql, sizeof(sql), "call Copy_Counter_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);



	while(row)
	{
		KeyValue keyvlaue;
		keyvlaue.key = lynxAtoi<UInt32>(row[0]);
		keyvlaue.value = lynxAtoi<UInt32>(row[1]);

		copyCounter.insertTail(keyvlaue);
		row = mDBInterface.fetchRow(rs);
	}

	playerData.mCounterData.m_CopyCounter = copyCounter;

	LOG_INFO("Load playerBuffList %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}	

UInt16 PersistWorker::loadRewardCounter(PlayerData& playerData,UInt64 playerGuid)
{

	char sql[2048] = {0};
	List<UInt32> rewardCounter;
	snprintf(sql, sizeof(sql), "select * from reward where PlayerUid ='%llu'", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);


	UInt32 num;
	while(row)
	{		
		for(UInt32 i = 1; i<=40;i++)
		{
			num = lynxAtoi<UInt32>(row[i]);		
			rewardCounter.insertTail(num);
		}		
		row = mDBInterface.fetchRow(rs);
	}

	playerData.mCounterData.m_RewardCounter = rewardCounter;

	LOG_INFO("Load rewardCounter %llu %s from DB successful!", 
		playerGuid, playerData.mBaseData.m_strPlayerName.c_str());
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}	

UInt16 PersistWorker::loadLottery(PlayerData& playerData,UInt64 playerGuid)
{

	char sql[2048] = {0};

	snprintf(sql, sizeof(sql), "call Lottery_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	while(row)
	{
		playerData.mCounterData .m_RewardLotteryOnceTime = lynxAtoi<UInt32>(row[0]);
		playerData.mCounterData .m_RewardLotteryTenTime = lynxAtoi<UInt32>(row[1]);
		playerData.mCounterData .m_RewardLotteryOnceTicket = lynxAtoi<UInt32>(row[2]);
		playerData.mCounterData .m_RewardLotteryTenTicket = lynxAtoi<UInt32>(row[3]);
		playerData.mCounterData .m_RewardLotteryOnceFreeCount = lynxAtoi<UInt32>(row[4]);
		playerData.mCounterData .m_RewardLotteryOnceTicketCount = lynxAtoi<UInt32>(row[5]);
		playerData.mCounterData .m_RewardLotteryOnceCount = lynxAtoi<UInt32>(row[6]);
		playerData.mCounterData .m_RewardLotteryTenFreeCount = lynxAtoi<UInt32>(row[7]);
		playerData.mCounterData .m_RewardLotteryTenTicketCount = lynxAtoi<UInt32>(row[8]);
		playerData.mCounterData .m_RewardLotteryTenCount = lynxAtoi<UInt32>(row[9]);
		playerData.mCounterData .m_RewardLotteryVipDefaultCount = lynxAtoi<UInt32>(row[10]);
		playerData.mCounterData .m_RewardLotteryVipAwardID = lynxAtoi<UInt32>(row[11]);
		playerData.mCounterData .m_RewardLotteryVipElseCount = lynxAtoi<UInt32>(row[12]);
		row = mDBInterface.fetchRow(rs);
	}

	LOG_INFO("Load mCounterData %llu  from DB successful!", playerGuid);
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}

UInt16 PersistWorker::loadFoods(PlayerData& playerData,UInt64 playerGuid)
{

	char sql[2048] = {0};
		
	snprintf(sql, sizeof(sql), "call Food_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	while(row)
	{
		playerData.mFoodsData .leftTimes = lynxAtoi<UInt32>(row[0]);
		playerData.mFoodsData .beginTime = lynxAtoi<UInt32>(row[1]);
		playerData.mFoodsData .food1 = lynxAtoi<UInt32>(row[2]);
		playerData.mFoodsData .food2 = lynxAtoi<UInt32>(row[3]);
		playerData.mFoodsData .food3 = lynxAtoi<UInt32>(row[4]);
		playerData.mFoodsData .food11 = lynxAtoi<UInt32>(row[5]);
		playerData.mFoodsData .food12 = lynxAtoi<UInt32>(row[6]);
		playerData.mFoodsData .food13 = lynxAtoi<UInt32>(row[7]);
		playerData.mFoodsData .vipLeftTimes = lynxAtoi<UInt32>(row[8]);
		playerData.mFoodsData .buyTime = lynxAtoi<UInt32>(row[9]);
		playerData.mFoodsData .vipFoodLeftNumber = lynxAtoi<UInt32>(row[10]);
		playerData.mFoodsData .resetState = lynxAtoi<UInt32>(row[11]);
		row = mDBInterface.fetchRow(rs);
	}
	if (playerData.mFoodsData .beginTime == 0&& playerData.mFoodsData .leftTimes == 0)
	{
		playerData.mFoodsData .beginTime = TimeUtil::getTimeSec();
	}

	LOG_INFO("Load mFoodsData %llu  from DB successful!", playerGuid);
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}	



UInt16 PersistWorker::loadFriendBeApplyData(PlayerData& playerData,UInt64 playerGuid)
{

	char sql[2048] = {0};

	snprintf(sql, sizeof(sql), "SELECT * FROM friend_be_apply_list WHERE playerGuid = %llu", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	List<UInt64> playerIDList;
	UInt64 playerId;
	while(row)
	{
		for (UInt32 i = 1;i<51;i++)
		{
			playerId =  lynxAtoi<UInt64>(row[i]);
			playerIDList.insertTail( playerId);
		}	
		row = mDBInterface.fetchRow(rs);
	}

	playerData.mFriendData.mBeApplyList = playerIDList;



	LOG_INFO("Load friend_be_apply_list %llu  from DB successful!", playerGuid);
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}	


UInt16 PersistWorker::loadStrength(PlayerData& playerData,UInt64 playerGuid)
{

	char sql[2048] = {0};

	snprintf(sql, sizeof(sql), "call Strength_Load(%llu)", playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	while(row)
	{
		playerData.mStrengthData .leftTimes = lynxAtoi<UInt32>(row[0]);
		playerData.mStrengthData .beginTime = lynxAtoi<UInt32>(row[1]);
		playerData.mStrengthData .strength = lynxAtoi<UInt32>(row[2]);
		
		playerData.mStrengthData .vipLeftTimes = lynxAtoi<UInt32>(row[3]);
		playerData.mStrengthData .buyTime = lynxAtoi<UInt32>(row[4]);
		playerData.mStrengthData .vipFoodLeftNumber = lynxAtoi<UInt32>(row[5]);
		row = mDBInterface.fetchRow(rs);
	}

	LOG_INFO("Load mStrengthData %llu  from DB successful!", playerGuid);
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}	

bool PersistWorker::updateStageData(UInt64 playerUid,UInt32 chapterID,UInt32 stageID)
{

	String strData ;
	char dest[64]={};	
	UInt32 isStageBegin = 0;
	UInt32 isChapterBegin = 0;
	StageData stageData;
	PlayerChapterData chapterData;
	PlayerChapterList chapterList;
	char name1[64] = {};
	char name2[64] = {};
	char name3[64] = {};
	char name4[64] = {};
	UInt32 whichStage;

	whichStage = stageID % 100;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player ==NULL)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	chapterList = player->getChapterList();

	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			chapterData = iter->mValue;
			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{				
				if (iter1->mValue.m_nStageID == stageID)
				{
					stageData = iter1->mValue;
					break;
				}
			}
			break;			
		}
	}
	sprintf(name1,"%s%d%s","stage",whichStage,"id");

	sprintf(name2,"%s%d%s","stage",whichStage,"getstar");

	sprintf(name3,"%s%d%s","stage",whichStage,"challengeTimes");

	sprintf(name4,"%s%d%s","stage",whichStage,"lastchallengeTime");

	char sql[4096] = {0};

	snprintf(sql, sizeof(sql), "SELECT chapterID from stage WHERE (PlayergUid = '%llu' AND chapterID = '%u')",playerUid,chapterID);

	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{		
		mDBInterface.freeResult(&rs);
		snprintf(sql, sizeof(sql), "insert into stage (playerguid,chapterId)values('%llu','%u')",playerUid,chapterID);	
		result = mDBInterface.execSql(sql);
	} 

	mDBInterface.freeResult(&rs);

	snprintf(sql, sizeof(sql), "UPDATE stage set iselite = '%u',treasure1getstate = '%u',treasure2getstate = '%u',treasure3getstate = '%u',chapterstarttime = '%llu',chapterfinishtime = '%llu' ,%s = '%u',%s = '%u',%s = '%u',%s = '%llu' WHERE (PlayergUid = '%llu' AND chapterID = '%u')",
		chapterData.m_nIselite,
		chapterData.m_nTreasure1getstate,
		chapterData.m_nTreasure2getstate,
		chapterData.m_nTreasure3getstate,
		chapterData.m_nChapterstarttime,
		chapterData.m_nChapterfinishtime,

		name1,
		stageData.m_nStageID ,
		name2,
		stageData.m_nGetStar,
		name3,
		stageData.m_nChallengTimes,
		name4,
		stageData.m_nLastChallengTime,		
		playerUid,
		chapterID
		);

	LOG_DEBUG("Sql:%s", sql);

	if (stageData.m_nGetStar %10 >5)
	{
		LOG_DEBUG("error star ----------------------- /n --------------------------------------- :%s", sql);
		assert(false);
	}

	result = mDBInterface.execSql(sql);
	if(!result)
	{
		return false;
	}
	else
	{
		return true;
	}

}


bool PersistWorker::updateRewardCounterData(UInt64 playerUid,UInt32 rewardID,UInt32 num)
{

	String strData;
	char dest[64]={};
	char name[64] = {};	
	char sql[4096] = {0};
	List<UInt32> rewardCounter;
	UInt32 i =0;
	num = 0;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player == NULL)
	{
		LOG_INFO("Can't find the player %llu",playerUid);
		return false;
	}

	
	rewardCounter = player->GetRewardCounter();	
	for (List<UInt32>::Iter * iter = rewardCounter.begin();iter!=NULL; iter = rewardCounter.next(iter))
	{
		if (i == rewardID)
		{
			num = iter->mValue;
			break;
		}
		i++;
	}
	sprintf(name,"%s%d","reward",rewardID);

	snprintf(sql, sizeof(sql), "SELECT chapterID from reward WHERE PlayergUid = '%llu'",playerUid);

	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{		
		mDBInterface.freeResult(&rs);
		snprintf(sql, sizeof(sql), "insert into reward playerguid values('%llu')",playerUid);	
		result = mDBInterface.execSql(sql);
	} 

	mDBInterface.freeResult(&rs);

	snprintf(sql, sizeof(sql), "UPDATE stage set %s = '%u' WHERE (PlayergUid = '%llu')",
		name,
		num ,		
		playerUid
		);

	LOG_DEBUG("Sql:%s", sql);

	result = mDBInterface.execSql(sql);
	return result;

}




UInt16 PersistWorker::loadPersonalRangking(PlayerData &playerData,UInt64 playerUid)
{
	MapRecord mapRecords;
	
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), 
		"CALL RankingPersonal_load(%llu)",playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rank table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		UInt32 type;
		UInt32 rank;
		Record record;
		type = lynxAtoi<UInt32>(row[0]);
		rank = lynxAtoi<UInt32>(row[1]);
		record.playerID = lynxAtoi<UInt32>(row[2]);
		record.name = row[3];	
		record.modleID = lynxAtoi<UInt32>(row[4]);
		record.val = lynxAtoi<UInt32>(row[5]);

		mapRecords.insert(rank,record);
		playerData.mRecordsData.m_TypeRecords.insert(type,mapRecords);


		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}


UInt16 PersistWorker::loadRankGameData(PlayerData &playerData,UInt64 playerUid)
{

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM rankgame WHERE playerID = %llu",playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rankgame table empty");
		return LynxErrno::None;
// 		return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		PlayerRankGameData rankGame;
	
		Guid playerID = lynxAtoi<UInt64>(row[0]);
		rankGame.m_Score = lynxAtoi<UInt32>(row[1]);
		rankGame.m_Point = lynxAtoi<UInt32>(row[2]);
		rankGame.m_Time = lynxAtoi<UInt32>(row[3]);
		rankGame.m_Sessions = lynxAtoi<UInt32>(row[4]);
		rankGame.m_WinSessions = lynxAtoi<UInt32>(row[5]);
		rankGame.m_MaskNum = lynxAtoi<UInt32>(row[6]);
		rankGame.m_LastIndex = lynxAtoi<UInt32>(row[7]);
		rankGame.m_LastTime = lynxAtoi<UInt32>(row[8]);
		rankGame.m_PlayerIds = row[9];
		rankGame.m_LastChallengeGuid = lynxAtoi<UInt64>(row[10]);
		rankGame.newReportFlag = lynxAtoi<UInt32>(row[11]);
		
		playerData.mRankGameData = rankGame;

		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}


UInt16 PersistWorker::loadOnlineDayData(PlayerData &playerData,UInt64 playerUid)
{

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM online_day WHERE playerUid = %llu",playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::None;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rankgame table empty");
		return LynxErrno::None;
		// 		return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		PlayerOnlineDay mPlayerOnlineDay ;

		Guid playerID = lynxAtoi<UInt64>(row[0]);
		mPlayerOnlineDay.day1 = lynxAtoi<UInt32>(row[1]);
		mPlayerOnlineDay.day2 = lynxAtoi<UInt32>(row[2]);
		mPlayerOnlineDay.day3 = lynxAtoi<UInt32>(row[3]);
		mPlayerOnlineDay.day4 = lynxAtoi<UInt32>(row[4]);
		mPlayerOnlineDay.day5 = lynxAtoi<UInt32>(row[5]);
		mPlayerOnlineDay.day6 = lynxAtoi<UInt32>(row[6]);
		mPlayerOnlineDay.day7 = lynxAtoi<UInt32>(row[7]);
		mPlayerOnlineDay.day8 = lynxAtoi<UInt32>(row[8]);
		mPlayerOnlineDay.day9 = lynxAtoi<UInt32>(row[9]);
		mPlayerOnlineDay.day10 = lynxAtoi<UInt32>(row[10]);

		mPlayerOnlineDay.day11 = lynxAtoi<UInt32>(row[11]);
		mPlayerOnlineDay.day12 = lynxAtoi<UInt32>(row[12]);
		mPlayerOnlineDay.day13 = lynxAtoi<UInt32>(row[13]);
		mPlayerOnlineDay.day14 = lynxAtoi<UInt32>(row[14]);
		mPlayerOnlineDay.day15 = lynxAtoi<UInt32>(row[15]);
		mPlayerOnlineDay.day16 = lynxAtoi<UInt32>(row[16]);
		mPlayerOnlineDay.day17 = lynxAtoi<UInt32>(row[17]);
		mPlayerOnlineDay.day18 = lynxAtoi<UInt32>(row[18]);
		mPlayerOnlineDay.day19 = lynxAtoi<UInt32>(row[19]);
		mPlayerOnlineDay.day20 = lynxAtoi<UInt32>(row[20]);

		mPlayerOnlineDay.day21 = lynxAtoi<UInt32>(row[21]);
		mPlayerOnlineDay.day22 = lynxAtoi<UInt32>(row[22]);
		mPlayerOnlineDay.day23 = lynxAtoi<UInt32>(row[23]);
		mPlayerOnlineDay.day24 = lynxAtoi<UInt32>(row[24]);
		mPlayerOnlineDay.day25 = lynxAtoi<UInt32>(row[25]);
		mPlayerOnlineDay.day26 = lynxAtoi<UInt32>(row[26]);
		mPlayerOnlineDay.day27 = lynxAtoi<UInt32>(row[27]);
		mPlayerOnlineDay.day28 = lynxAtoi<UInt32>(row[28]);
		mPlayerOnlineDay.day29 = lynxAtoi<UInt32>(row[29]);
		mPlayerOnlineDay.day30 = lynxAtoi<UInt32>(row[30]);
		
		playerData.mPlayerOnlineDay = mPlayerOnlineDay;


		
		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}


UInt16 PersistWorker::getRankGameData(PlayerRankGameData &rankGame,UInt64 playerUid)
{

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM rankgame WHERE playerID = %llu",playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rankgame table empty");
		return LynxErrno::None;
		// 		return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		Guid playerID = lynxAtoi<UInt64>(row[0]);
		rankGame.m_Score = lynxAtoi<UInt32>(row[1]);
		rankGame.m_Point = lynxAtoi<UInt32>(row[2]);
		rankGame.m_Time = lynxAtoi<UInt32>(row[3]);
		rankGame.m_Sessions = lynxAtoi<UInt32>(row[4]);
		rankGame.m_WinSessions = lynxAtoi<UInt32>(row[5]);
		rankGame.m_MaskNum = lynxAtoi<UInt32>(row[6]);
		rankGame.m_LastIndex = lynxAtoi<UInt32>(row[7]);
		rankGame.m_LastTime = lynxAtoi<UInt32>(row[8]);
		rankGame.m_PlayerIds = row[9];
		rankGame.m_LastChallengeGuid = lynxAtoi<UInt64>(row[10]);
		rankGame.newReportFlag = lynxAtoi<UInt32>(row[11]);

// 		playerData.mRankGameData = rankGame;

		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}




UInt16 PersistWorker::loadBuyCoinData(PlayerData &playerData,UInt64 playerUid)
{

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM buycoin WHERE playerID = %llu",playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("loadBuyCoinData table empty");
		return LynxErrno::None;
		// 		return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		
		PlayerBuyCoinData buyCoinData;

		Guid playerID = lynxAtoi<UInt64>(row[0]);
		buyCoinData.m_box1 = lynxAtoi<UInt32>(row[1]);
		buyCoinData.m_box2 = lynxAtoi<UInt32>(row[2]);
		buyCoinData.m_box3 = lynxAtoi<UInt32>(row[3]);
		buyCoinData.m_refreshTime = lynxAtoi<UInt32>(row[4]);
	

		playerData.mPlayerBuyCoinData = buyCoinData;

		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}

UInt16 PersistWorker::loadRankGameReport(PlayerData &playerData,UInt64 playerUid)
{
	char sql[2048] = {0};
	char sql1[2048] = {0};
	snprintf(sql, sizeof(sql), 	"SELECT * FROM rankgame_report WHERE playerID = %llu",playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rankgame table empty");
		return LynxErrno::None;
		// 		return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		ReportData reportData;
		reportData.id = lynxAtoi<UInt32>(row[1]);
		reportData.roleId = lynxAtoi<UInt64>(row[2]);
		reportData.modelID = lynxAtoi<UInt32>(row[3]);
		reportData.score = lynxAtoi<UInt32>(row[4]);
		reportData.name = row[5];
		reportData.level = lynxAtoi<UInt32>(row[6]);
		reportData.vipLv = lynxAtoi<UInt32>(row[7]);
		reportData.happenTime =  lynxAtoi<UInt32>(row[8]);
		reportData.power = lynxAtoi<UInt32>(row[9]);
		reportData.attackType =  lynxAtoi<UInt32>(row[10]);
		reportData.rank =  lynxAtoi<UInt32>(row[11]);
		reportData.flag =  lynxAtoi<UInt32>(row[12]);

		playerData.mRankGameData.m_ReportData.insertTail(reportData);

		row = mDBInterface.fetchRow(rs);
	}

	int size = playerData.mRankGameData.m_ReportData.size();
	if (size >10)
	{
		int i = 0;
		for (List<ReportData>::Iter *it = playerData.mRankGameData.m_ReportData.begin();it !=NULL;it = playerData.mRankGameData.m_ReportData.next(it) )
		{
			i++;
			if ( (size-10) >= i)
			{
				playerData.mRankGameData.m_ReportData.erase(it);

				snprintf(sql, sizeof(sql1), "DELETE FROM   rankgame_report WHERE playerID = %llu AND id = %u",playerUid,it->mValue.id);
				LOG_DEBUG("sql1:%s", sql1);
				result = mDBInterface.execSql(sql1);
				if(!result)
				{
					LOG_WARN("DELETE rankgame_report failed:%llu", playerUid);
				}
			}			
		}
	}

	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}


UInt16 PersistWorker::loadTowerData(PlayerData &playerData,UInt64 playerUid)
{
	PlayerTowerData towerData;
	
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), 
		"SELECT * FROM towerData WHERE playerguid = (%llu)",playerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("towerdata table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
// 		towerData. = lynxAtoi<UInt32>(row[0]);
		towerData.m_HighID = lynxAtoi<UInt32>(row[1]);
		towerData.m_Score = lynxAtoi<UInt32>(row[2]);		
		towerData.m_AP = lynxAtoi<UInt32>(row[3]);
		towerData.m_HP = lynxAtoi<UInt32>(row[4]);

		playerData.mTowerData = towerData;
		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}



UInt16 PersistWorker::loadInlineActivity(PlayerData &playerData,UInt64 playerUid)
{
	PlayerInlineActivityData inlineActivityData =  playerData.mInlineActivityData;

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM inline_activity WHERE playerguid = (%llu)",playerUid);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("towerdata table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}
		
	while(row)
	{
		// 		inlineActivityData. = lynxAtoi<UInt32>(row[0]);
		inlineActivityData.m_GrowFoundBuyFlag = lynxAtoi<UInt32>(row[1]);
		inlineActivityData.m_MonthSignCount = lynxAtoi<UInt32>(row[2]);		
		inlineActivityData.m_MonthSignTime = lynxAtoi<UInt32>(row[3]);
		inlineActivityData.m_OnlineWelFareOnlineTime = lynxAtoi<UInt32>(row[4]);
		inlineActivityData.m_PeopleWelfareRechargeNum = lynxAtoi<UInt32>(row[5]);
		inlineActivityData.m_FirstLoginTime = lynxAtoi<UInt32>(row[6]);
		inlineActivityData.m_SevenLoginGotCount = lynxAtoi<UInt32>(row[7]);
		inlineActivityData.m_LastGetTime = lynxAtoi<UInt32>(row[8]);
		inlineActivityData.m_LoginTime = lynxAtoi<UInt32>(row[9]);
		inlineActivityData.m_TimeAwardRefreshTime = lynxAtoi<UInt32>(row[10]);
		inlineActivityData.m_LogoutTime = lynxAtoi<UInt32>(row[11]);
		inlineActivityData.m_onlieFinishTime = lynxAtoi<UInt32>(row[12]);

		playerData.mInlineActivityData = inlineActivityData;
		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}



UInt16 PersistWorker::loadInlineActivityList(PlayerData &playerData,UInt64 playerUid)
{
	List<ActivityData>activityDataList;
	PersistWorker::initActivityData(playerData,activityDataList);

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM inline_activity_list WHERE playerguid = (%llu)",playerUid);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("towerdata table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		ActivityData activityData;
		activityData.playerID = lynxAtoi<UInt32>(row[0]);
		activityData.type = lynxAtoi<UInt32>(row[1]);
		activityData.count = lynxAtoi<UInt32>(row[2]);		
		activityData.key = lynxAtoi<UInt32>(row[3]);
		activityData.value = lynxAtoi<UInt32>(row[4]);

		activityDataList.insertTail(activityData);
		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	PersistWorker::setActivityData(playerData,activityDataList);
	return LynxErrno::None;

}


UInt16 PersistWorker::loadSevenDayTask(PlayerData &playerData,UInt64 playerUid)
{
	List<ActivityData>activityDataList;
	

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "SELECT * FROM sevenday_task WHERE playerguid = (%llu)",playerUid);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("towerdata table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		ActivityData activityData;
		activityData.playerID = lynxAtoi<UInt32>(row[0]);
		activityData.type = lynxAtoi<UInt32>(row[1]);
		activityData.count = lynxAtoi<UInt32>(row[2]);		
		activityData.key = lynxAtoi<UInt32>(row[3]);
		activityData.value = lynxAtoi<UInt32>(row[4]);
		activityData.gotIt = lynxAtoi<UInt32>(row[5]);

		activityDataList.insertTail(activityData);
		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	
	PersistWorker::setActivityData(playerData,activityDataList);
	return LynxErrno::None;

}
void PersistWorker::initActivityData(PlayerData &playerData,List<ActivityData>activityDataList)
{
	if (playerData.mInlineActivityData.m_SevenDayTaskGotList.size() == 0)
	{
		for (UInt32 i =0;i<7;i++)
		{
			KeyValue keyValue;
			playerData.mInlineActivityData.m_SevenDayTaskGotList.insertTail(keyValue);
		}
	}

	
	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_TASK )
		{
			KeyList keyList;
			keyList.key = it->mValue.id;
			playerData.mInlineActivityData.m_SevenDayTaskFinishList.insertTail(keyList);
		}
	}

}


void PersistWorker::setActivityData(PlayerData &playerData,List<ActivityData>activityDataList)
{
	UInt32 tmp = 0;

	PlayerInlineActivityData inlineActivityData = playerData.mInlineActivityData;

	for (List<ActivityData>::Iter *iter = activityDataList.begin();iter!= NULL;iter = activityDataList.next(iter))
	{
		if (iter->mValue.type == GROWTH_FOUND)
		{
			if (inlineActivityData.m_GrowFoundGotList.size() <= iter->mValue.count)
			{
				for (UInt32 i = inlineActivityData.m_GrowFoundGotList.size();i<= iter->mValue.count;i++)
				{
					inlineActivityData.m_GrowFoundGotList.insertTail(tmp);
				}
				
			}
			UInt32 count = 0;
			for(List<UInt32>::Iter*it = inlineActivityData.m_GrowFoundGotList.begin();it!=NULL;it = inlineActivityData.m_GrowFoundGotList.next(it) )
			{
				
				if (count == iter->mValue.count)
				{
					it->mValue = iter->mValue.key;
				}
				count ++;
			}
		}
		else if (iter->mValue.type == MONTH_SIGN)
		{

		}
		else if (iter->mValue.type == ONLINE_WELFARE)
		{
			if (inlineActivityData.m_OnlineWelFareGotList.size() <= iter->mValue.count)//todo geshu bu dui 
			{
				for (UInt32 i = inlineActivityData.m_OnlineWelFareGotList.size();i<= iter->mValue.count;i++)
				{
					inlineActivityData.m_OnlineWelFareGotList.insertTail(tmp);
				}
			}
			UInt32 count = 0;
			for(List<UInt32>::Iter*it = inlineActivityData.m_OnlineWelFareGotList.begin();it!=NULL;it = inlineActivityData.m_OnlineWelFareGotList.next(it) )
			{
				if (count == iter->mValue.count)
				{
					it->mValue = iter->mValue.key;
				}
				count ++;
			}
		}
		else if (iter->mValue.type == PEOPLE_WELFARE)
		{
			if (inlineActivityData.m_PeopleWelfareGotList.size() <= iter->mValue.count)
			{
				for (UInt32 i = inlineActivityData.m_PeopleWelfareGotList.size();i<= iter->mValue.count;i++)
				{
					inlineActivityData.m_PeopleWelfareGotList.insertTail(tmp);
				}
			}
			UInt32 count = 0;
			for(List<UInt32>::Iter*it = inlineActivityData.m_PeopleWelfareGotList.begin();it!=NULL;it = inlineActivityData.m_PeopleWelfareGotList.next(it) )
			{
				if (count == iter->mValue.count)
				{
					it->mValue = iter->mValue.key;
				}
				count ++;
			}

		}
		else if (iter->mValue.type == SEVENDAY_LOGIN_AWARD)
		{
			

		}
		else if (iter->mValue.type == SEVENDAY_TASK)
		{		
			UInt32 gotIt = 0;
			UInt32 count = 0;
			for(List<KeyValue>::Iter*it1 = inlineActivityData.m_SevenDayTaskGotList.begin();it1!=NULL;it1 = inlineActivityData.m_SevenDayTaskGotList.next(it1) )
			{
				if (count == iter->mValue.count)
				{
					KeyValue keyValue;
					keyValue.key = iter->mValue.key;
					keyValue.value = iter->mValue.value;
					it1->mValue = keyValue;
					gotIt = 1;
					break;
				}
				count ++;
			}
			if (gotIt == 0)
			{
				KeyValue keyValue;
				keyValue.key = iter->mValue.key;
				keyValue.value = iter->mValue.value;
				inlineActivityData.m_SevenDayTaskGotList.insertTail(keyValue);
			}

		}
		else if (iter->mValue.type == SEVENDAY_FINISHTASK)
		{
// 			if (inlineActivityData.m_SevenDayTaskFinishList.size() <= iter->mValue.count)
// 			{
// 				KeyList keyList11;
// 				for (UInt32 i = inlineActivityData.m_SevenDayTaskFinishList.size();i<= iter->mValue.count;i++)
// 				{
// 					inlineActivityData.m_SevenDayTaskFinishList.insertTail(keyList11);
// 				}
// 			}

			Goods goods;
			goods.resourcestype = iter->mValue.key;
			goods.subtype = iter->mValue.value;
			goods.num = iter->mValue.gotIt;

			UInt32 getIt = 0;
			UInt32 count = 0;
			for(List<KeyList>::Iter*it2 = inlineActivityData.m_SevenDayTaskFinishList.begin();it2!=NULL;it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2) )
			{				
				if (it2->mValue.key == iter->mValue.count)
				{	
					getIt = 1;
					it2->mValue.values.insertTail(goods);	
					break;
				}
			}
			if (getIt == 0)//没有key就先添加key，再添加values
			{
				KeyList keyList;
				keyList.key = iter->mValue.count;
				inlineActivityData.m_SevenDayTaskFinishList.insertTail(keyList);
				for(List<KeyList>::Iter*it2 = inlineActivityData.m_SevenDayTaskFinishList.begin();it2!=NULL;it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2) )
				{				
					if (it2->mValue.key == iter->mValue.count)
					{	
						it2->mValue.values.insertTail(goods);	
						break;
					}
				}
			}
			
		}
		else if (iter->mValue.type == TIME_AWARD)
		{
			if (inlineActivityData.m_TimeAwardGotList.size() <= iter->mValue.count)
			{
				for (UInt32 i = inlineActivityData.m_TimeAwardGotList.size();i<= iter->mValue.count;i++)
				{
					inlineActivityData.m_TimeAwardGotList.insertTail(tmp);
				}
			}
		
			UInt32 count = 0;
			for(List<UInt32>::Iter*it = inlineActivityData.m_TimeAwardGotList.begin();it!=NULL;it = inlineActivityData.m_TimeAwardGotList.next(it) )
			{
				if (count == iter->mValue.count)
				{
					it->mValue = iter->mValue.key;
				}
				count ++;
			}

		}
	}
	playerData.mInlineActivityData = inlineActivityData;
}


UInt16 PersistWorker::loadStageRangking()
{
	char sql[2048] = {0}; 
	snprintf(sql, sizeof(sql), 
		"call Ranking_Load()");

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rank table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		UInt32 type;
		UInt32 rank;
		Record record;
		type = lynxAtoi<UInt32>(row[0]);
		rank = lynxAtoi<UInt32>(row[1]);
		record.playerID = lynxAtoi<UInt32>(row[2]);
		record.name = row[3];	
		record.modleID = lynxAtoi<UInt32>(row[4]);
		record.val = lynxAtoi<UInt32>(row[5]);
		RankingManager::getSingleton().setRecord(type,rank,record,false);

		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}

UInt16 PersistWorker::loadServerData()
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	char sql[2048] = {0}; 
	ServerPublicData  serverPublicData;

	snprintf(sql, sizeof(sql),"SELECT * from server_data ");
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
// 		mDBInterface.freeResult(&rs);
		LOG_WARN("server_data table empty");
		
	}

	

	while(row)
	{
		serverPublicData.gServerID = lynxAtoi<UInt32>(row[0]);
		serverPublicData.gRechargeNum = lynxAtoi<UInt32>(row[1]);
		serverPublicData.gServerRobotRefreshFlag = lynxAtoi<UInt32>(row[2]);		
		serverPublicData.gRankGameRewardresetTime = lynxAtoi<UInt32>(row[3]);	
		serverPublicData.gServerFirstOpenTime = lynxAtoi<UInt32>(row[4]);
		serverPublicData.gServerSpare5 = lynxAtoi<UInt32>(row[5]);
		ServerData::getSingleton().setServerPublicData(serverPublicData,false);

		row = mDBInterface.fetchRow(rs);
	}
	if(serverPublicData.gServerFirstOpenTime == 0)
	{
		serverPublicData.gServerID = 0;
		serverPublicData.gRechargeNum = 0;
		serverPublicData.gServerRobotRefreshFlag = 0;		
		serverPublicData.gRankGameRewardresetTime = 0;	
		serverPublicData.gServerFirstOpenTime = nowTime;
		serverPublicData.gServerSpare5 = 0;
		ServerData::getSingleton().setServerPublicData(serverPublicData,true);	

	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}


UInt16 PersistWorker::loadServerDailyData()
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	char sql[2048] = {0}; 
	ServerDailyData  serverDailyData;

	snprintf(sql, sizeof(sql),"SELECT * from server_daily_data ");
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		// 		mDBInterface.freeResult(&rs);
	}

	while(row)
	{
		serverDailyData.gServerOnlineNum = lynxAtoi<UInt32>(row[0]);
		serverDailyData.gServerOnlineMaxNum = lynxAtoi<UInt32>(row[1]);
		serverDailyData.gServerOnlineNumSetTime = lynxAtoi<UInt32>(row[2]);		
		serverDailyData.gSSpare1 = lynxAtoi<UInt32>(row[3]);	
		serverDailyData.gSSpare2 = lynxAtoi<UInt32>(row[4]);
		serverDailyData.gSSpare3 = lynxAtoi<UInt32>(row[5]);
		ServerData::getSingleton().setServerDailyData(serverDailyData,false);

		row = mDBInterface.fetchRow(rs);
	}

	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}


void 
PersistWorker::onPersistUpdateStageRanking(PersistRankingSaveReq& msg )
{	

	if (msg.type == TWELVE_PALACE_MY_RECORD)
	{		
		char sql[2048];
		snprintf(sql, sizeof(sql), "call RankingPersonal_Update('%u','%u', '%llu','%u')",msg.type,msg.rank,msg.playerID,msg.val);

		LOG_DEBUG("Sql:%s", sql);

		bool result = mDBInterface.execSql(sql);

		MYSQL_RES* rs = mDBInterface.storeResult();

		if(!result)
		{
			mDBInterface.freeResult(&rs);
			return;
		}
		else
		{
			mDBInterface.freeResult(&rs);
			return;
		}  
	}
	else
	{
		char sql[2048];
		snprintf(sql, sizeof(sql), "call Ranking_Update('%u','%u', '%llu','%u')",msg.type,msg.rank,msg.playerID,msg.val);

		LOG_DEBUG("Sql:%s", sql);

		bool result = mDBInterface.execSql(sql);

		MYSQL_RES* rs = mDBInterface.storeResult();

		if(!result)
		{
			mDBInterface.freeResult(&rs);
			return;
		}
		else
		{
			mDBInterface.freeResult(&rs);
			return;
		}  
	}
}

UInt16 PersistWorker::loadShop(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql),"SELECT * FROM shop WHERE playerID = %llu",playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rank table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{
		ShopItems shopItems;
		Guid playerID = 0;		
		shopItems.shopType = lynxAtoi<UInt32>(row[0]);
		shopItems.refreshTimes = lynxAtoi<UInt32>(row[1]);
		playerID = lynxAtoi<UInt64>(row[2]);
		shopItems.refreshTime = lynxAtoi<UInt32>(row[3]);

		playerData.mPlayerShopData.m_ShopItems.insertTail(shopItems);

		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}

void PersistWorker::onCatCoinSaveReq(CatCoinSaveReq& msg )
{	
	char sql[2048];
	if (msg.num == 0)
	{
		snprintf(sql, sizeof(sql), "DELETE FROM cat_coin WHERE playerID = '%llu' AND catCoinID = '%u'",msg.playerID,msg.catCoinID);
	}
	else
	{
		snprintf(sql, sizeof(sql), "REPLACE INTO cat_coin VALUES('%u','%u', '%llu')",msg.catCoinID,msg.num,msg.playerID);
	}
	

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* rs = mDBInterface.storeResult();

	mDBInterface.freeResult(&rs);
	return;
}

void PersistWorker::onCoinGroupSaveReq(CoinGroupSaveReq& msg )
{	
	char sql[2048];
	snprintf(sql, sizeof(sql), "REPLACE INTO coin_group VALUES('%u', '%llu')",msg.groupID,msg.playerID);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* rs = mDBInterface.storeResult();

	mDBInterface.freeResult(&rs);
	return;
}

void 
PersistWorker::onPersistUpdateShop(PersistShopSaveReq& msg )
{	
	char sql[2048];
	snprintf(sql, sizeof(sql), "REPLACE INTO shop VALUES('%u','%u', '%llu','%u')",msg.shopType,msg.refreshTimes,msg.playerID,msg.refreshTime);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* rs = mDBInterface.storeResult();

	mDBInterface.freeResult(&rs);
	return;
}


UInt16 PersistWorker::loadCatCoin(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql),"SELECT * FROM cat_coin WHERE playerID = %llu",playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rank table empty");
		return LynxErrno::None;
	}

	while(row)
	{	
		CatCoin catCoin;
		Guid playerID;

		catCoin.catCoinID = lynxAtoi<UInt32>(row[0]);
		catCoin.num = lynxAtoi<UInt32>(row[1]);
		playerID = lynxAtoi<UInt64>(row[2]);
		if (catCoin.num != 0)
		{
			playerData.mCourageChallengeData.catCoins.insertTail(catCoin);
		}	
	
		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}

UInt16 PersistWorker::loadCoinGroup(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql),"SELECT * FROM coin_group WHERE playerID = %llu",playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rank table empty");
		return LynxErrno::None;
	}

	while(row)
	{	
		UInt32 coinGroupID;		

		coinGroupID = lynxAtoi<UInt32>(row[0]);
		
		playerData.mCourageChallengeData.coinGroup.insertTail(coinGroupID);

		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;
}


UInt16 PersistWorker::loadShopItem(PlayerData& playerData,UInt64 playerGuid)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql),"SELECT * FROM shop_item WHERE playerID = %llu",playerGuid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		LOG_WARN("rank table empty");
		return LynxErrno::None;
		//return LynxErrno::PlayerNotExist;
	}

	while(row)
	{	
	
	Item item;
	Guid playerID = 0;		
	item.shopType = lynxAtoi<UInt32>(row[0]);
	item.position = lynxAtoi<UInt32>(row[1]);
	playerID = lynxAtoi<UInt64>(row[2]);
	item.buyTime = lynxAtoi<UInt32>(row[3]);
	item.buyTimes = lynxAtoi<UInt32>(row[4]);
	item.goods.resourcestype = lynxAtoi<UInt32>(row[5]);
	item.goods.subtype = lynxAtoi<UInt32>(row[6]);
	item.goods.num = lynxAtoi<UInt32>(row[7]);

	for(List<ShopItems>::Iter*iter = playerData.mPlayerShopData.m_ShopItems.begin();iter != NULL;iter = playerData.mPlayerShopData.m_ShopItems.next(iter))
	{
		if (iter->mValue.shopType == item.shopType)
		{
			iter->mValue.items.insertTail(item);
			break;
		}
	}
		row = mDBInterface.fetchRow(rs);
	}
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}

void 
PersistWorker::onPersistUpdateShopItem(PersistShopItemSaveReq& msg )
{	

	char sql[2048];
	snprintf(sql, sizeof(sql), "REPLACE INTO shop_item VALUES('%u','%u', '%llu','%u','%u','%u','%u','%u')",msg.shopType,msg.position,msg.playerID,msg.buyTime,msg.buyTimes,msg.resType,msg.subType,msg.num);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* rs = mDBInterface.storeResult();

	if(!result)
	{
		mDBInterface.freeResult(&rs);
		return;
	}
	else
	{
		mDBInterface.freeResult(&rs);
		return;
	}
}



void PersistWorker::onPersistLoadOfflineData( PersistOfflineDataReq &msg)
{
	PlayerBaseData mBaseData;

	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call Player_Load(%llu)", msg.playerID);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return ;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return ;
	}

	mBaseData.m_nPlayerID = msg.playerID;
	mBaseData.m_nModelID = lynxAtoi<UInt32>(row[1]);
	mBaseData.m_strPlayerName = row[2];
	mBaseData.m_nLevel = lynxAtoi<UInt32>(row[3]);

	mBaseData.m_nLevelExp = lynxAtoi<Guid>(row[4]);
	mBaseData.m_nVipLevel = lynxAtoi<UInt32>(row[5]);
	mBaseData.m_nVipExp = lynxAtoi<Guid>(row[6]);
	mBaseData.m_nGold = lynxAtoi<Guid>(row[7]);
	mBaseData.m_nCoin = lynxAtoi<Guid>(row[8]);
	mBaseData.m_nStrength = lynxAtoi<UInt32>(row[9]);
	mBaseData.m_strMood = row[10];

	mBaseData.m_listSweepCount.insertTail(lynxAtoi<UInt32>(row[11]));
	mBaseData.m_listSweepCount.insertTail(lynxAtoi<UInt32>(row[12]));
	mBaseData.m_listSweepCount.insertTail(lynxAtoi<UInt32>(row[13]));
	mBaseData.m_accountID = lynxAtoi<Guid>(row[14]);
	mBaseData.m_nFame = lynxAtoi<Guid>(row[15]);
	mBaseData.m_nPower = lynxAtoi<Guid>(row[16]);

	LOG_INFO("Load playerBaseData %llu %s from DB successful!", 
		msg.playerID, mBaseData.m_strPlayerName.c_str());


	mDBInterface.freeResult(&rs); 


	return ;

}
void PersistWorker::onPersistServerDataReq( PersistServerDataReq &msg)
{

	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
	char sql[2048];
	
	snprintf(sql, sizeof(sql), "REPLACE INTO server_data VALUES('%u','%u','%u','%u','%u','%u')",serverPublicData.gServerID,serverPublicData.gRechargeNum,serverPublicData.gServerRobotRefreshFlag,
		serverPublicData.gRankGameRewardresetTime,serverPublicData.gServerFirstOpenTime,serverPublicData.gServerSpare5);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* rs = mDBInterface.storeResult();

	mDBInterface.freeResult(&rs);
	return;

}


void PersistWorker::onPersistServerDailyDataReq( PersistServerDailyDataReq &msg)
{

	ServerDailyData  serverDailyData = ServerData::getSingleton().getServerDailyData();
	char sql[2048];

	snprintf(sql, sizeof(sql), "REPLACE INTO server_daily_data VALUES('%u','%u','%u','%u','%u','%u')",serverDailyData.gServerOnlineNum,serverDailyData.gServerOnlineMaxNum,
		serverDailyData.gServerOnlineNumSetTime,serverDailyData.gSSpare1,serverDailyData.gSSpare2,serverDailyData.gSSpare3);




	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* rs = mDBInterface.storeResult();

	mDBInterface.freeResult(&rs);
	return;

}

void PersistWorker::onPersistRobotDataReq( PersistRobotDataReq &msg)
{

	char sql[1024] = {0};
	UInt32 time = TimeUtil::getTimeSec();
	UInt64 robotGuid = 0;
	bool result = 0;
	MYSQL_RES* rs =NULL;

	
	snprintf(sql, sizeof(sql), "DELETE FROM robot");
	LOG_INFO("Sql:%s", sql);
	result = mDBInterface.execSql(sql);
	rs = mDBInterface.storeResult();
	mDBInterface.freeResult(&rs);


	for(Map<UInt64, RobotTableTemplate>::Iter *iter = gRobotTable->mMap.begin();iter != NULL;iter = gRobotTable->mMap.next(iter))
	{
 		memset(sql,0,sizeof(sql));
		robotGuid = iter->mValue.roleID;

		snprintf(sql, sizeof(sql), " INSERT DELAYED INTO robot(uuid,uid,name,level,viplevel,power,time,score)VALUES(%llu,%u,'%s',%u,%u,%u,%u,%u)",
			robotGuid,iter->mValue.modelID,iter->mValue.name.c_str(),iter->mValue.level, iter->mValue.vipLv,iter->mValue.power,time, iter->mValue.score);
	
		LOG_INFO("Sql:%s", sql);

		result = mDBInterface.execSql(sql);
		 rs = mDBInterface.storeResult();
	}
	
	mDBInterface.freeResult(&rs);

	

	PersistInsertRobotResp resp;

	postMsgToOutputQueue(resp, 0);


}

void PersistWorker::onPersistFindSimilarPower( PersistFindSimilarPowerReq &msg)
{
	PlayerBaseData mBaseData;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(msg.playerGuid);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	char sql[1024] = {0};
	char sql2[2048] = {0};
	if (msg.times == 10001 ||msg.times == 2)
	{
		//leaveTime 有可能一直在登录没有时间的
		snprintf(sql, sizeof(sql), "SELECT uuid,level FROM playerBaseData WHERE level <=%u AND level >=%u AND uuid not in (%llu)", msg.high,msg.low,msg.playerGuid);//in ()不能为空所有填个0
	}
	else
	{
		snprintf(sql, sizeof(sql), "SELECT uuid,power FROM playerBaseData WHERE power <=%u AND power >=%u AND uuid not in (%llu) ", msg.high,msg.low,msg.playerGuid);//in ()不能为空所有填个0
	}
	
	strcat(sql," UNION ");

	strcat(sql2,sql);
	sql[0] = '\0';

	if (msg.times == 10001||msg.times == 2)
	{
		snprintf(sql, sizeof(sql), "SELECT uuid,level FROM robot WHERE level <=%u AND level >=%u AND uuid not in (%llu)", msg.high,msg.low,msg.playerGuid);//in ()不能为空所有填个0
	}
	else
	{
		snprintf(sql, sizeof(sql), "SELECT uuid,power FROM robot WHERE power <=%u AND power >=%u AND uuid not in (%llu)", msg.high,msg.low,msg.playerGuid);//in ()不能为空所有填个0
	}

	strcat(sql2,sql);

	LOG_DEBUG("Sql:%s", sql2);
	bool result = mDBInterface.execSql(sql2);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return ;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	List<GuidValue> keyValueList;
	while(row)
	{
		GuidValue keyvalue;
		keyvalue.guid = lynxAtoi<Guid>(row[0]);
		keyvalue.value = lynxAtoi<UInt32>(row[1]);
		keyValueList.insertTail(keyvalue);
		row = mDBInterface.fetchRow(rs);

	}

	mDBInterface.freeResult(&rs); 


	PersistFindSimilarPowerResp resp;
	resp.playerID = msg.playerGuid;
	resp.keyValueList = keyValueList;
	resp.times = msg.times;
	resp.initialValue = msg.initialValue;

	postMsgToOutputQueue(resp, 0);

	return ;

}

void PersistWorker::onPersistInlineActivityPower( PersistInlineActivityReq  &msg)
{
	UInt32 count = 0;

	char sql[2048] = {0};
	char sql1[2048] = {0};
	char tmp[2048] = {0};
	snprintf(sql, sizeof(sql), "REPLACE INTO inline_activity VALUES(%llu,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",msg.playerGuid, 
		msg.m_GrowFoundBuyFlag,
		msg.m_MonthSignCount,
		msg.m_MonthSignTime,
		msg.m_OnlineWelFareOnlineTime,
		msg.m_PeopleWelfareRechargeNum,
		msg.m_FirstLoginTime,
		msg.m_SevenLoginGotCount,
		msg.m_LastGetTime,
		msg.m_LoginTime,
		msg.m_TimeAwardRefreshTime,
		msg.m_LogoutTime,
		msg.m_onlieFinishTime);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	
	if (msg.type == GROWTH_FOUND)
	{
		count = 0;
		for (List<UInt32>::Iter * iter = msg.m_GrowFoundGotList.begin();iter != NULL;iter = msg.m_GrowFoundGotList.next(iter))
		{
			snprintf(sql1, sizeof(sql1), "REPLACE INTO inline_activity_list VALUES(%llu,%u,%u,%u,%u) ", msg.playerGuid,msg.type,count,iter->mValue,0);		

			LOG_DEBUG("Sql:%s", sql1);
			bool result = mDBInterface.execSql(sql1);
			count ++;
		}
	}
	else if (msg.type == MONTH_SIGN)
	{
		
	}
	else if (msg.type == ONLINE_WELFARE)
	{
		count = 0;
		for (List<UInt32>::Iter * iter = msg.m_OnlineWelFareGotList.begin();iter != NULL;iter = msg.m_OnlineWelFareGotList.next(iter))
		{
			snprintf(sql1, sizeof(sql1), "REPLACE INTO inline_activity_list VALUES(%llu,%u,%u,%u,%u) ", msg.playerGuid,msg.type,count,iter->mValue,0);		

			LOG_DEBUG("Sql:%s", sql1);
			bool result = mDBInterface.execSql(sql1);
			count ++;
		}
		
	}
	else if (msg.type == PEOPLE_WELFARE)
	{
		count = 0;
		for (List<UInt32>::Iter * iter = msg.m_PeopleWelfareGotList.begin();iter != NULL;iter = msg.m_PeopleWelfareGotList.next(iter))
		{
			snprintf(sql1, sizeof(sql1), "REPLACE INTO inline_activity_list VALUES(%llu,%u,%u,%u,%u) ", msg.playerGuid,msg.type,count,iter->mValue,0);		

			LOG_DEBUG("Sql:%s", sql1);
			bool result = mDBInterface.execSql(sql1);
			count ++;
		}
		
	}
	else if (msg.type == SEVENDAY_LOGIN_AWARD)
	{
		count = 0;
		for (List<UInt32>::Iter * iter = msg.m_GrowFoundGotList.begin();iter != NULL;iter = msg.m_GrowFoundGotList.next(iter))
		{
			snprintf(sql1, sizeof(sql1), "REPLACE INTO inline_activity_list VALUES(%llu,%u,%u,%u,%u) ", msg.playerGuid,msg.type,count,iter->mValue,0);		

			LOG_DEBUG("sql1:%s", sql1);
			bool result = mDBInterface.execSql(sql1);
			count ++;
		}
	}
	else if (msg.type == SEVENDAY_TASK)
	{
		count = 0;
		for (List<KeyValue>::Iter * it = msg.m_SevenDayTaskGotList.begin();it != NULL;it = msg.m_SevenDayTaskGotList.next(it))
		{
			snprintf(sql1, sizeof(sql1), "REPLACE INTO inline_activity_list VALUES(%llu,%u,%u,%u,%u) ", msg.playerGuid,msg.type,count,it->mValue.key,it->mValue.value);		

			LOG_DEBUG("sql1:%s", sql1);
			bool result = mDBInterface.execSql(sql1);
			count ++;
		}	
	}
	else if (msg.type == SEVENDAY_FINISHTASK)
	{		
		for(List<KeyList>::Iter * it2 = msg.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = msg.m_SevenDayTaskFinishList.next(it2))
		{
			for(List<Goods>::Iter *iter1 = it2->mValue.values.begin();iter1 != NULL;iter1 = it2->mValue.values.next(iter1))
			{
				snprintf(sql1, sizeof(sql1), "REPLACE INTO sevenday_task VALUES(%llu,%u,%u,%u,%u,%u) ", msg.playerGuid,SEVENDAY_FINISHTASK,it2->mValue.key,iter1->mValue.resourcestype,iter1->mValue.subtype,iter1->mValue.num);		

				LOG_DEBUG("sql1:%s", sql1);
				bool result = mDBInterface.execSql(sql1);
			}
		}
	}
	else if (msg.type == TIME_AWARD)
	{
		count = 0;
		for (List<UInt32>::Iter * iter = msg.m_TimeAwardGotList.begin();iter != NULL;iter = msg.m_TimeAwardGotList.next(iter))
		{
			snprintf(sql1, sizeof(sql1), "REPLACE INTO inline_activity_list VALUES(%llu,%u,%u,%u,%u) ", msg.playerGuid,msg.type,count,iter->mValue,0);		

			LOG_DEBUG("sql1:%s", sql1);
			bool result = mDBInterface.execSql(sql1);
			count ++;
		}
	}

	return ;

}

void PersistWorker::onPersistFriendOffLineUpdate(PersistUpdateFriendOffLineNotify & msg)
{
	UInt64 playerUid = msg.m_nPlayerUid;
	UInt64 myselfUid = msg.m_nMyselfUid;
		
	std::string playerStr;
	std::stringstream mystream;
	mystream << playerUid;
	mystream >> playerStr;

	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call friendandblack_load(%llu)", myselfUid);
	bool res = mDBInterface.execSql(sql);
	
	if(!res)
	{
		LOG_INFO("call friendandblack_load failed!! player: %llu",  myselfUid);
		return ;
	}

	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return ;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	std::string friendstr = row[1];
	std::string blackstr = row[2];
	UInt32 friendCount = lynxAtoi<UInt32>(row[3]);

	std::string ::size_type findIndex = friendstr.find(';');
	mDBInterface.freeResult(&rs); 
	//最新的字符串
	std::string newString;

	if(msg.m_nType == 1)
	{
		//删除情况
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			//123;456;789;
			//截取后friendEleStr变为123
			std::string friendEleStr = friendstr.substr(0,findIndex);
		
			if(friendEleStr.empty())
			{
				break;
			}

			//相等情况
			if(friendEleStr == playerStr)
			{
				friendstr = friendstr.substr(findIndex+1);
				//字符串不为空
				if(!friendstr.empty())
				{
					//加上剩余的，如果是末尾，那么不加
					newString += friendstr;
				}
				friendCount--;
				break;
			}
			else
			{
				newString += (friendEleStr + ';');
				friendstr = friendstr.substr(findIndex+1);
				findIndex = friendstr.find(';');
			}

			
		}
	}
	else
	{
		//添加的情况
		friendstr+= (playerStr + ';');
		newString = friendstr;
		friendCount++;
	}
	
	
	memset(sql, sizeof(sql), 0);
	snprintf(sql, sizeof(sql), "call friendandblack_update(%llu, '%s', '%s', %u)", myselfUid, newString.c_str(), blackstr.c_str(), friendCount);
	bool saveres = mDBInterface.execSql(sql);
	
	if(saveres)
	{
		LOG_INFO("call friendandblack_update success!! player: %llu",  myselfUid);
	}
	else
	{
		LOG_INFO("call friendandblack_update failed!! player: %llu",  myselfUid);
	}

	return ;

}


void PersistWorker::onPersistFriendOffLineAdd(PersistAddFriendOffLineNotify & msg)
{
	
	std::string friendstr =msg.friendStr.c_str();
	std::string blackstr = msg.blackStr.c_str();
	UInt32 friendCount = msg.friendCount;
	UInt64 myselfUid = msg.m_nMyselfUid;
	std::stringstream mystream;
	mystream << msg.m_nPlayerUid;
	std::string playerStr;
	mystream >> playerStr;

	
	//最新的字符串
	std::string newString;

		//添加的情况
		friendstr+= (playerStr + ';');
		newString = friendstr;
		friendCount++;
	


	char sql[4096];
	snprintf(sql, sizeof(sql), "call friendandblack_update(%llu, '%s', '%s', %u)", myselfUid, newString.c_str(), blackstr.c_str(), friendCount);
	bool saveres = mDBInterface.execSql(sql);

	if(saveres)
	{
		LOG_INFO("call friendandblack_update success!! player: %llu",  myselfUid);
	}
	else
	{
		LOG_INFO("call friendandblack_update failed!! player: %llu",  myselfUid);
	}

}

void PersistWorker::onPersistFriendOffLineDel(PersistDelFriendOffLineNotify &msg)
{
	std::string friendstr =msg.friendStr.c_str();
	std::string blackstr = msg.blackStr.c_str();
	UInt32 friendCount = msg.friendCount;
	UInt64 myselfUid = msg.m_nMyselfUid;
	std::stringstream mystream;
	mystream << msg.m_nPlayerUid;
	std::string playerStr;
	mystream >> playerStr;

	std::string ::size_type findIndex = friendstr.find(';');
	//最新的字符串
	std::string newString;

	
		//删除情况
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			//123;456;789;
			//截取后friendEleStr变为123
			std::string friendEleStr = friendstr.substr(0,findIndex);

			if(friendEleStr.empty())
			{
				break;
			}

			//相等情况
			if(friendEleStr == playerStr)
			{
				friendstr = friendstr.substr(findIndex+1);
				//字符串不为空
				if(!friendstr.empty())
				{
					//加上剩余的，如果是末尾，那么不加
					newString += friendstr;
				}
				friendCount--;
				break;
			}
			else
			{
				newString += (friendEleStr + ';');
				friendstr = friendstr.substr(findIndex+1);
				findIndex = friendstr.find(';');
			}


		}


		char sql[4096];
		snprintf(sql, sizeof(sql), "call friendandblack_update(%llu, '%s', '%s', %u)", myselfUid, newString.c_str(), blackstr.c_str(), friendCount);
		bool saveres = mDBInterface.execSql(sql);

		if(saveres)
		{
			LOG_INFO("call friendandblack_update success!! player: %llu",  myselfUid);
		}
		else
		{
			LOG_INFO("call friendandblack_update failed!! player: %llu",  myselfUid);
		}


}



void PersistWorker::onPersistFriendUpdate(PersistUpdateFriendNotify & msg)
{
	std::stringstream mystream;

	std::string friendstr;
	for(List<UInt64>::Iter * friendIter = msg.m_friendData.mFriendList.begin();  friendIter != msg.m_friendData.mFriendList.end();
		friendIter = msg.m_friendData.mFriendList.next(friendIter))
	{
		mystream.clear();
		mystream.str("");
		std::string uidstr;
		mystream << friendIter->mValue;
		mystream >> uidstr;

		friendstr += (uidstr + ';');
	}

	std::string blackstr;
	for(List<UInt64>::Iter * blackIter = msg.m_friendData.mBlackList.begin();  blackIter != msg.m_friendData.mBlackList.end();
		blackIter = msg.m_friendData.mBlackList.next(blackIter))
	{
		mystream.clear();
		mystream.str("");
		std::string uidstr;
		mystream << blackIter->mValue;
		mystream >> uidstr;

		blackstr += (uidstr + ';');
	}

	UInt32 size = msg.m_friendData.mFriendList.size();
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call friendandblack_update(%llu, '%s', '%s', %u)", msg.m_nPlayerUid, friendstr.c_str(), blackstr.c_str(),  size);
	bool res = mDBInterface.execSql(sql);
	
	if(res)
	{
		LOG_INFO("update friendandblack table player: %llu success!!",  msg.m_nPlayerUid);
	}
	else
	{
		LOG_INFO("update friendandblack table player: %llu failed!!",  msg.m_nPlayerUid);
	}

	

}

void PersistWorker::onPersistAllLeaveTime(PersistResetAllLeaveTime &msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call onlineallleave_update (%llu)", msg.m_nLeaveTime);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	
	if(!result)
	{
		LOG_WARN("update onlineallleave_update failed:%llu", msg.m_nLeaveTime);
	}
	
}



void PersistWorker::onPersistLeaveTime(PersistUpdateLeaveTime &msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call updateleavetime (%llu, %llu)", msg.m_nPlayerUid, msg.m_nLeaveTime);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	
	if(!result)
	{
		LOG_WARN("update leavetime failed:%llu", msg.m_nPlayerUid);
	}
	

}
//m_time >0 保存积分 m_lastTime 保存所有玩家的上次名次
void PersistWorker::onRankGameScoreSave(PersistRankGameScoreSave &msg)
{
	char sql[1024] = {0};
	char sql1[100*1024] = {0};
	UInt32 time = TimeUtil::getTimeSec();

	if (msg.m_time > 0)
	{
		if (msg.m_nPlayerUid  > RobotMinRoleID && msg.m_nPlayerUid  <RobotMaxRoleID)
		{
			snprintf(sql, sizeof(sql), "UPDATE  robot SET score = %u,time = %u WHERE uuid = %llu",  msg.m_score, msg.m_time,msg.m_nPlayerUid);

			LOG_DEBUG("Sql:%s", sql);
			bool result = mDBInterface.execSql(sql);

			if(!result)
			{
				LOG_WARN("update rankgame failed:%llu", msg.m_nPlayerUid);
			}
		}
		if( msg.m_nPlayerUid  >= RobotMaxRoleID)
		{
			//只保存不在线的情况

			PlayerRankGameData rankGame;
			UInt32 flag = getRankGameData(rankGame,msg.m_nPlayerUid);
			rankGame.m_Sessions ++;
			rankGame.m_WinSessions += msg.m_addWinCount;
			rankGame.m_Time =msg.m_time;
			rankGame.m_Score = msg.m_score;
			rankGame.newReportFlag = 1;

			snprintf(sql,sizeof(sql),"REPLACE INTO rankgame VALUES(%llu, %u,%u,%u,%u,%u,%u,%u,%u,'%s',%llu,%u)",
				msg.m_nPlayerUid,
				rankGame.m_Score,
				rankGame.m_Point,
				rankGame.m_Time,
				rankGame.m_Sessions,
				rankGame.m_WinSessions,
				rankGame.m_MaskNum,
				rankGame.m_LastIndex,
				rankGame.m_LastTime,
				rankGame.m_PlayerIds.c_str(),
				rankGame.m_LastChallengeGuid,
				rankGame.newReportFlag
				);

			LOG_DEBUG("Sql:%s", sql);
			bool result = mDBInterface.execSql(sql);

			if(!result)
			{
				LOG_WARN("update rankgame failed:%llu", msg.m_nPlayerUid);
			}
		}
	}

	UInt32 count = 0;
	if (msg.m_lastTime > 0)
	{			
		bool result = true;
		MYSQL_RES* res = NULL;
		Map<UInt32,RankData*>rankingIndexID = RankGameManager::getSingleton().getRankingIndexID();
		for (Map<UInt32,RankData*>::Iter *iter = rankingIndexID.begin();iter!=NULL;iter = rankingIndexID.next(iter) )
		{

// 			if (iter->mValue->playerUid  > RobotMinRoleID &&iter->mValue->playerUid   <RobotMaxRoleID)
// 			{
// 			}
			if(iter->mValue->playerUid   >= RobotMaxRoleID)
			{
				memset(sql, 0, sizeof(sql));
				snprintf(sql, sizeof(sql), "UPDATE  rankgame SET lastindex = %u WHERE playerID = %llu", iter->mValue->index ,iter->mValue->playerUid );

				strcat(sql1,sql);
				strcat(sql1,";");

				count ++;
				if (count%50 == 0)
				{	
					mDBInterface.execFreeSql(sql1,res);
					LOG_DEBUG("Sql:%s", sql1);

					memset(sql1, 0, sizeof(sql1));
				}			
			}
		}
		mDBInterface.execFreeSql(sql1,res);	
	}

}



void PersistWorker::onRankGameReportSave(PersistRankGameReportSave &msg)
{
	char sql[1024] = {0};
	char sql1[1024] = {0};
	int index = 1;
	bool result  = true;

	if (msg. deleteID > 0)
	{
		snprintf(sql, sizeof(sql), "DELETE FROM   rankgame_report WHERE playerID = %llu AND id = %u",msg.m_nPlayerUid,msg.deleteID);

		LOG_DEBUG("Sql:%s", sql);
		 result = mDBInterface.execSql(sql);

		if(!result)
		{
			LOG_WARN("DELETE rankgame_report failed:%llu", msg.m_nPlayerUid);
		}
	}

	if (msg.saveID > 0)
	{
		if(msg.saveID == 100000000)
		{
			do 
			{
				snprintf(sql1, sizeof(sql1), 	" SELECT max(id)+1  FROM rankgame_report  WHERE  playerID =  %llu",msg.m_nPlayerUid);
				LOG_DEBUG("sql1:%s", sql1);
				result = mDBInterface.execSql(sql1);
				MYSQL_RES* rs = mDBInterface.storeResult();		
				if(!rs)
				{
					// 执行失败
					mDBInterface.freeResult(&rs);
					break;	
				}
				
				MYSQL_ROW row = mDBInterface.fetchRow(rs);
				if(!row || !(row[0]))
				{
					// 角色不存在
					mDBInterface.freeResult(&rs);
					LOG_WARN("rankgame table empty");
					break;
				}
				while(row)
				{
			
					index = lynxAtoi<UInt32>(row[0]);
					row = mDBInterface.fetchRow(rs);
				}
			} while (false);
		
			snprintf(sql, sizeof(sql), "REPLACE INTO  rankgame_report values(%llu,%u,%llu,%u,%u,'%s',%u,%u,%u,%u,%u,%u,%u)", 
				msg.m_nPlayerUid,index,msg.reportData.roleId,msg.reportData.modelID,msg.reportData.score,msg.reportData.name.c_str(),msg.reportData.level,
				msg.reportData.vipLv,msg.reportData.happenTime,msg.reportData.power,msg.reportData.attackType,msg.reportData.rank,msg.reportData.flag);

			LOG_DEBUG("Sql:%s", sql);
			bool result = mDBInterface.execSql(sql);

			if(!result)
			{
				LOG_WARN("REPLACE rankgame_report failed:%llu", msg.m_nPlayerUid);
			}
		}
		else
		{
			snprintf(sql, sizeof(sql), "REPLACE INTO  rankgame_report values(%llu,%u,%llu,%u,%u,'%s',%u,%u,%u,%u,%u,%u,%u)", 
				msg.m_nPlayerUid,msg.reportData.id,msg.reportData.roleId,msg.reportData.modelID,msg.reportData.score,msg.reportData.name.c_str(),msg.reportData.level,
				msg.reportData.vipLv,msg.reportData.happenTime,msg.reportData.power,msg.reportData.attackType,msg.reportData.rank,msg.reportData.flag);

			LOG_DEBUG("Sql:%s", sql);
			bool result = mDBInterface.execSql(sql);

			if(!result)
			{
				LOG_WARN("REPLACE rankgame_report failed:%llu", msg.m_nPlayerUid);
			}
		}


	}	

	
}

void PersistWorker::onPersistBaseDateSave(PersistBaseDateSave & msg)
{
	char sql[4096] = {0};
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(msg.m_nPlayerUid);

	if(!mPlayer)
	{
		LOG_INFO("Can't find the player %llu",msg.m_nPlayerUid);
		return;
	}

	PlayerBaseData * pBaseData = mPlayer->getPlayerInfoManager().getPlayerBaseData();

	UInt32 sweepCountArry[3] = {0};
	int arryIndex = 0;
	for(List<UInt32>::Iter* iterList = pBaseData->m_listSweepCount.begin();
		iterList != NULL; iterList = pBaseData->m_listSweepCount.next(iterList))
	{
		sweepCountArry[arryIndex] = iterList->mValue;
		arryIndex++;
	}


	snprintf(sql, sizeof(sql), "call BaseData_Update(%llu,%u,'%s',%u,%llu,%u,%llu,%llu,%llu,%u,'%s',%u,%u,%u,%llu,%llu,%u,'%s',%u,%u)",
		msg.m_nPlayerUid,pBaseData->m_nModelID,
		pBaseData->m_strPlayerName.c_str(),pBaseData->m_nLevel,
		pBaseData->m_nLevelExp, pBaseData->m_nVipLevel,
		pBaseData->m_nVipExp,pBaseData->m_nGold,
		pBaseData->m_nCoin, pBaseData->m_nStrength,
		pBaseData->m_strMood.c_str(),sweepCountArry[0],sweepCountArry[1],sweepCountArry[2],
		pBaseData->m_accountID,
		pBaseData->m_nFame,pBaseData->m_nPower, pBaseData->m_strOldName.c_str(),
		pBaseData->m_nFirstLoinTime,pBaseData->m_nLoginTime);

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		

		return ;
	}
	else
	{
		return ;
	}
}

void PersistWorker::onPersistOnlineDaySave(PersistOnlineDaySave & msg)
{
	char sql[4096] = {0};
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(msg.m_nPlayerUid);
	if (player == NULL)
	{
		return;
	}
	PlayerOnlineDay mPlayerOnlineDay =	mPlayerOnlineDay = player->getPlayerOnlineDay();


	snprintf(sql,sizeof(sql),"REPLACE INTO  online_day values(%llu,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u, %u,%u,%u,%u,%u,%u,%u,%u,%u,%u, %u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",
		msg.m_nPlayerUid, 
		mPlayerOnlineDay.day1,
		mPlayerOnlineDay.day2,
		mPlayerOnlineDay.day3,
		mPlayerOnlineDay.day4,
		mPlayerOnlineDay.day5,
		mPlayerOnlineDay.day6,
		mPlayerOnlineDay.day7,
		mPlayerOnlineDay.day8,
		mPlayerOnlineDay.day9,
		mPlayerOnlineDay.day10,
		
		mPlayerOnlineDay.day11,
		mPlayerOnlineDay.day12,
		mPlayerOnlineDay.day13,
		mPlayerOnlineDay.day14,
		mPlayerOnlineDay.day15,
		mPlayerOnlineDay.day16,
		mPlayerOnlineDay.day17,
		mPlayerOnlineDay.day18,
		mPlayerOnlineDay.day19,
		mPlayerOnlineDay.day20,

		mPlayerOnlineDay.day21,
		mPlayerOnlineDay.day22,
		mPlayerOnlineDay.day23,
		mPlayerOnlineDay.day24,
		mPlayerOnlineDay.day25,
		mPlayerOnlineDay.day26,
		mPlayerOnlineDay.day27,
		mPlayerOnlineDay.day28,
		mPlayerOnlineDay.day29,
		mPlayerOnlineDay.day30
		);

	LOG_DEBUG("Sql:%s", sql);

	bool achRes = mDBInterface.execSql(sql);

	if (!achRes)
	{
		LOG_INFO("online_day failed playerUid: %llu",msg.m_nPlayerUid);

		return;

	}
	else
	{
		//创建玩家成功，需要初始化装备列表和装备信息

		return ;

	}
}

void PersistWorker::onLoadPlayerOnlineByLv(PerisistUpdateOnlineLvRank & msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call playeronlinebylv_load()" );
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("load player online by lv failed");
		return;
	}

	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return ;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	List<BaseInfoData> playerInfos;
	while(row)
	{
		//select uuid, uid, name , power, viplevel, level ,leaveTime
		BaseInfoData baseInfoData;

		baseInfoData.playerUid = lynxAtoi<Guid>(row[0]);
		baseInfoData.modelId = lynxAtoi<UInt32>(row[1]);
		baseInfoData.name = row[2];
		baseInfoData.power = lynxAtoi<UInt32>(row[3]);
		baseInfoData.vipLv = lynxAtoi<UInt32>(row[4]);
		baseInfoData.level = lynxAtoi<UInt32>(row[5]);
		baseInfoData.leaveTime = lynxAtoi<UInt64>(row[6]);

		playerInfos.insertTail(baseInfoData);
		LogicSystem::getSingleton().updateBaseInfo(baseInfoData);
		row = mDBInterface.fetchRow(rs);

	}

	RankSystem::getSingleton().createOnlineLvs(playerInfos);
	
	mDBInterface.freeResult(&rs); 
	
}

void PersistWorker::onLoadPlayerPowerRank(PersistGetPowerRank & msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call playerpowerrank_load()" );
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("load player power rank  failed");
		return;
	}

	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return ;		
	}

	RankSystem::getSingleton().clearPowerRank();
	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	List<BaseInfoData> playerInfos;
	UInt32 rank = 1;
	while(row)
	{
		//select uuid, uid, name , power, viplevel, level ,leaveTime
		BaseInfoData baseInfoData;

		baseInfoData.playerUid = lynxAtoi<Guid>(row[0]);
		baseInfoData.modelId = lynxAtoi<UInt32>(row[1]);
		baseInfoData.name = row[2];
		baseInfoData.power = lynxAtoi<UInt32>(row[3]);
		baseInfoData.vipLv = lynxAtoi<UInt32>(row[4]);
		baseInfoData.level = lynxAtoi<UInt32>(row[5]);
		baseInfoData.leaveTime = lynxAtoi<UInt64>(row[6]);

		playerInfos.insertTail(baseInfoData);
		LogicSystem::getSingleton().updateBaseInfo(baseInfoData);
		RankSystem::getSingleton().creatPowerRank(baseInfoData.playerUid,rank);
		row = mDBInterface.fetchRow(rs);
		rank ++;

	}

	mDBInterface.freeResult(&rs); 
}


void PersistWorker::onLoadPlayerScoreRank(PersistGetScoreRank & msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call playerscorerank_load()" );
	LOG_DEBUG("sql:%s", sql);
	UInt32 systemOpenLevel = 0;

	SystemInfoTemplate *systemInfoTemplate  = SYSTEMINFO_TABLE().get(SYSTEM_FOOD);
	if (systemInfoTemplate != NULL)
	{
		systemOpenLevel = systemInfoTemplate->openConditionValue;
	}



	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("load player score rank  failed");
		return;
	}

	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return ;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	UInt32 rank = 1;
	while(row)
	{

		RankData rankData;

		rankData.playerUid = lynxAtoi<Guid>(row[0]);
		rankData.modelId = lynxAtoi<UInt32>(row[1]);
		rankData.name = row[2];
		rankData.score = lynxAtoi<UInt32>(row[3]);
		rankData.vipLv = lynxAtoi<UInt32>(row[4]);
		rankData.level = lynxAtoi<UInt32>(row[5]);
		rankData.time = lynxAtoi<UInt32>(row[6]);
		rankData.power =  lynxAtoi<UInt32>(row[7]);
		rankData.index = rank;
		if (rankData.level >= systemOpenLevel&&rankData.score > 0)
		{
			RankGameManager::getSingleton().addNewRankGamePlayer(rankData);
			rank ++;
		}

		row = mDBInterface.fetchRow(rs);
	}

	mDBInterface.freeResult(&rs); 
	
	RankGameManager::getSingleton().getNullPlayerID();
}





void PersistWorker::onLoadPlayerConsortInfo(PersistPlayerConsortUpdate &msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call playerconsortall_load()");
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("load player consort  failed");
		return;
	}

	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return ;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	
	while(row)
	{
		//select uuid, uid, name , power, viplevel, level ,leaveTime
		ConsortInfoData consortInfoData;
		consortInfoData.playerUid = lynxAtoi<UInt64>(row[0]);
		consortInfoData.consortId = lynxAtoi<UInt64>(row[1]);
		consortInfoData.curContribute = lynxAtoi<UInt32>(row[2]);
		consortInfoData.totalContribute = lynxAtoi<UInt32>(row[3]);
		consortInfoData.consortJob = lynxAtoi<UInt32>(row[4]);
		
	
		LogicSystem::getSingleton().updateConsortInfo(consortInfoData.playerUid,consortInfoData);
	
		row = mDBInterface.fetchRow(rs);
	
	}
	
	mDBInterface.freeResult(&rs); 
}

void PersistWorker::onSavePlayerConsort(PersistPlayerConsortSave &msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call playerconsort_update(%llu, %llu, %u, %u, %u, %llu)", msg.m_nPlayerUid,
		msg.m_nConsortId, msg.m_nCurContribute, msg.m_nTotalContribute, msg.m_nConsortJob, msg.m_nLeaveTime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("playerconsort_update  failed");
		return;
	}

}

void PersistWorker::onSaveConsort(PersistConsortSave & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consort_update (%llu, %u, '%s', %u, %llu, '%s', %u, %u, %u, %llu)", msg.m_nConsortId,
		msg.m_nConsortLv, msg.m_strConsortName.c_str(), msg.m_nCount, msg.m_nPower, msg.m_strDescs.c_str(),
		msg.m_nRes, msg.m_nExp, msg.m_nCheck, msg.m_nPowerLimit);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("playerconsort_update  failed");
		return;
	}

}

void PersistWorker::onCreateConsort(PersistConsortCreate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consort_insert (%llu, %u, '%s', %u, %llu, '%s', %u, %llu, %u, %u, %llu)", msg.m_nConsortId,
		msg.m_nConsortLv, msg.m_strConsortName.c_str(), msg.m_nCount, msg.m_nPower, msg.m_strDescs.c_str(),
		msg.m_nRes, msg.m_nLeader,   msg.m_nExp, msg.m_nCheck, msg.m_nPowerLimit);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consort insert  failed");
		return;
	}

}

void PersistWorker::onConsortApplyInsert(PersistConsortApplyInsert & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortapply_insert (%llu, %llu,  %llu)", msg.m_nPlayerUid, msg.m_nConsortId, msg.m_nTime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortapply insert  failed");
		return;
	}
}

void PersistWorker::onConsortApplyDel(PersistConsortApplyDel & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortapply_del (%llu, %llu)", msg.m_nPlayerUid,
		msg.m_nConsortId);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortapply del  failed");
		return;
	}
}

void PersistWorker::onConsortApplyClear(PersistConsortApplyClear & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortapply_clear (%llu)", msg.m_nPlayerUid);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortapply clear  failed");
		return;
	}
}

void PersistWorker::onConsortApplyAllClear(PersistConsortApplyAllClear & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortapply_allclear(%llu)", msg.m_nConsortId);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortapply clear  failed");
		return;
	}
    

}

void PersistWorker::onConsortDel(PersistConsortDel & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consort_del(%llu)", msg.m_nConsortId);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consort_del   failed");
		return;
	}
}

void PersistWorker::onConsortLogInsert(PersistConsortLogInsert & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortlog_insert(%llu, %llu, %llu,'%s', '%s', '%s', %llu)", msg.m_nTempId, msg.m_nConsortId,
		msg.m_nPlayerUid, msg.m_strPlayerName.c_str(), msg.m_strParam1.c_str(), msg.m_strParam2.c_str(), msg.m_nTime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortlog isnertfailed   failed");
		return;
	}
}

void PersistWorker::onConsortSignReset(PersistConsortSignReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call activereset_update(%llu)", msg.m_nResetTime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("activereset update   failed");
		return;
	}
}

void PersistWorker::onConsortSignUpdate(PersistConsortSignUpdate & msg)
{
	Vector<UInt32>  awardvec;
	for(List<UInt32>::Iter * awardIter = msg.m_nSignAwards.begin();  awardIter != NULL; 
		awardIter = msg.m_nSignAwards.next(awardIter))
	{
		awardvec.push_back(awardIter->mValue);
	}

	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortsign_update(%llu,%u,%u,%u,%u,%u)", msg.m_nPlayerUid, msg.m_nSign, awardvec[0],
		awardvec[1], awardvec[2], awardvec[3]);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortsign_update   failed");
		return;
	}
}

void PersistWorker::onConsortActiveUpdate(PersistConsortActiveUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortactive_update(%llu,%u)", msg.m_nConsortId, msg.m_nActive);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortactive_update   failed");
		return;
	}
}

void PersistWorker::onConsortLoyalUpdate(PersistConsortLoyalUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortloyal_update(%llu,%u,  %u, %llu,  %u, %llu,  %u, %llu,  %u, %llu)", msg.m_nPlayerUid, msg.m_nRefreshTimes,
		msg.m_listXingxiaTasks.getn(0)->mValue.m_nTaskId,   msg.m_listXingxiaTasks.getn(0)->mValue.m_nGettime,
			msg.m_listXingxiaTasks.getn(1)->mValue.m_nTaskId,   msg.m_listXingxiaTasks.getn(1)->mValue.m_nGettime,
			msg.m_listXingxiaTasks.getn(2)->mValue.m_nTaskId,   msg.m_listXingxiaTasks.getn(2)->mValue.m_nGettime,
			msg.m_listXingxiaTasks.getn(3)->mValue.m_nTaskId,   msg.m_listXingxiaTasks.getn(3)->mValue.m_nGettime
		
		);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortloyal_update   failed");
		return;
	}
}

void PersistWorker::onKitQueTimesUpdate(PersistConsortKitQueUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call kitchentimes_update(%llu,%u)", msg.m_nPlayerUid, msg.m_nRefreshTimes);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("kitchentimes_update   failed");
		return;
	}
}

//更新公会商店刷新次数
void PersistWorker::onBusinessTimeReset(PersistBusinessCatTimeReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call businesscatreset_update(%llu)",  msg.m_nResetTime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("businesscatreset_update   failed");
		return;
	}

}

//重置公会商店刷新次数和商店计数等级
void PersistWorker::onBusinessTimesUpdate(PersistBusinessCatUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call businesscattimes_update(%llu, %u, %u, %u, %u, %u, %u, %u, %u, %u)", msg.m_nPlayerUid, msg.m_nRefreshTimes,
		msg.m_nBuyList.getn(0),  msg.m_nBuyList.getn(1),  msg.m_nBuyList.getn(2),  msg.m_nBuyList.getn(3), 
		msg.m_nBuyList.getn(4),  msg.m_nBuyList.getn(5),  msg.m_nBuyList.getn(6),  msg.m_nBuyList.getn(7) );
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("businesscattimes_update   failed");
		return;
	}
}

void PersistWorker::onEyeTimesUpdate(PersistEyeTimesUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call eyetimesupdate(%llu, %u)", msg.m_nPlayeruid, msg.m_nEyeTimes );
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("eyetimesupdate   failed");
		return;
	}
}

void PersistWorker::onTicketTimeReset(PersistTicketTimeReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call tickettimereset(%llu)", msg.m_nResetTime );
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("tickettimereset   failed");
		return;
	}
}

void PersistWorker::onTicketFriendUpdate(PersistTicketFriendUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call ticketfriends_update('%s', %llu, %u )", msg.m_strData.c_str(), 
		msg.m_nConsortuid, msg.m_nTicketid);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("ticketfriends_update   failed");
		return;
	}
}

void PersistWorker::onTicketQualityUpdate(PersistTicketQualityUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call ticketquality_update(%u, %llu, %u )", msg.m_nQuality,
		msg.m_nConsortuid, msg.m_nTicketid);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("ticketquality_update   failed");
		return;
	}
}

void PersistWorker::onTicketFriendInit(PersistTicketFriendInit & msg)
{
	MYSQL_RES* res = NULL;
	LOG_DEBUG("Sql:%s", msg.m_strExe.c_str());
	char  sqlchar[8096]={0};
	strcpy(sqlchar,msg.m_strExe.c_str());
	mDBInterface.execFreeSql(sqlchar, res);
	
}

void PersistWorker::onTicketAllDel(PersistTicketAllDel & msg)
{
	char sql[1024]={0};
	snprintf(sql,sizeof(sql),"truncate table ticketfriend");
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("truncate table ticketfriend failed.");
	}
}

void PersistWorker::onTicketDataUpdate(PersistTicketDataUpdate & msg)
{
	char sql[1024]={0};
	snprintf(sql,sizeof(sql),"call ticketfriends_update('%s', %llu, %u)",msg.m_strData.c_str(), msg.m_nConsortId, msg.m_nTicketId);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("ticketfriends_update failed.");
	}
}

void PersistWorker::onTicketSupportUpdate(PersistTicketSupport & msg)
{
	char sql[1024]={0};
	snprintf(sql,sizeof(sql),"call ticketquality_update(%u, %llu, %u, '%s')",msg.m_nQuality, msg.m_nConsortId, msg.m_nTicketId,
		msg.m_strFriends.c_str() );
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("ticketquality_update failed.");
	}
}

void PersistWorker::onTicketAwardAdd(PersistTicketAwardAdd & msg)
{
	char sql[1024]={0};
	snprintf(sql,sizeof(sql),"call addticketaward(%u, %llu, %u, %llu, %llu)",msg.m_nTicketId, msg.m_nAwardId, msg.m_nPeapleCnt, msg.m_nEndTime,
		msg.m_nPlayeruid);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("addticketaward failed.");
	}
}


void PersistWorker::onTicketTimesUpdate(PersistTicketTimesUpdate & msg)
{
	char sql[1024]={0};
	snprintf(sql,sizeof(sql),"call tickettimesupdate( %llu, %u)",msg.m_nPlayeruid,msg.m_nTimes );
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("tickettimesupdate failed.");
	}
}

void PersistWorker::onTicketAwardDel(PersistTicketAwardDel & msg)
{
	char sql[1024]={0};
	snprintf(sql,sizeof(sql),"call ticketawarddel( %llu)",msg.m_nPlayeruid );
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("tickettimesupdate failed.");
	}
}

void PersistWorker::onTicketTimesReset(PersistTicketTimesReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consorttickettimesreset(%llu)", msg.resettime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consorttickettimesreset    failed");
		return;
	}
}

void PersistWorker::onLoyalTimesReset(PersistLoyalTimesReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortloyalreset(%llu)", msg.resettime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortloyalreset    failed");
		return;
	}
}

void PersistWorker::onKitchenTimesReset(PersistKitchenTimesReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortkitchenreset(%llu)", msg.resettime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortkitchenreset    failed");
		return;
	}
}
	
void PersistWorker::onEyeTimesReset(PersistEyeTimesReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consorteyetimereset(%llu)", msg.resettime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consorteyetimereset    failed");
		return;
	}
}

void PersistWorker::onEloquenceUpdate(PersistEloquenceTimesUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call eloquencetimesupdate(%llu, %u)", msg.playeruid, msg.times);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("eloquencetimesupdate    failed");
		return;
	}
}

void PersistWorker::onEloquenceReset(PersistEloquenceTimesReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consorteloquencereset(%llu)", msg.resettime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("eloquencetimesupdate    failed");
		return;
	}
}

void PersistWorker::onWoodTimesReset(PersistWoodTimesReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortwoodcatreset(%llu)", msg.resettime);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortwoodcatreset    failed");
		return;
	}
}

void PersistWorker::onWoodTotalUpdate(PersistWoodTotalUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortwoodtotal_update(%llu, %u, %u, %u,%u)", msg.consortid, msg.woodleft, msg.enhance1,
		msg.enhance2, msg.enhance3);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortwoodtotal_update    failed");
		return;
	}
}

void PersistWorker::onWoodTotalReset(PersistWoodTotalReset & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortwoodtotal_reset()");
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortwoodtotal_reset    failed");
		return;
	}
}

void PersistWorker::onWoodSelfUpdate(PersistWoodSelfUpdate & msg)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call consortwoodself_update(%llu, %u, %u)", msg.playeruid, 
		msg.times, msg.awardflag);
	LOG_DEBUG("sql:%s", sql);

	bool result = mDBInterface.execSql(sql);
	if(!result)
	{
		LOG_WARN("consortwoodself_update    failed");
		return;
	}
}

void PersistWorker::onTicketAwardUpdate(PersistTicketAwardUpdate & msg)
{
	char sql[1024]={0};
	snprintf(sql,sizeof(sql),"call updateticketaward(%u, %llu, %u, %llu, %llu)",msg.m_nTicketId, msg.m_nAwardId, msg.m_nPeapleCnt, msg.m_nEndTime,
		msg.m_nPlayeruid );
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("updateticketaward failed.");
	}
}


void  PersistWorker::onAddEmail(PersistAddEmail &msg)
{
	
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call email_insert (%llu, %llu, '%s', '%s', %llu, '%s', '%s', %llu)", msg.mEmailData.m_nEmailUid, 
		msg.mEmailData.m_nTempId, msg.mEmailData.m_strDes.c_str(), msg.mEmailData.m_strContent.c_str(), msg.mEmailData.m_nPlayerUid,
		msg.mEmailData.m_strFrom.c_str(), msg.mEmailData.m_strTittle.c_str(), msg.mEmailData.m_nTime);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("GM add email failed:%llu", msg.mEmailData.m_nPlayerUid);
	}
}

void PersistWorker::onDelEmail(PersistDelEmail &msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call email_delete (%llu)", msg.mEmailUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("delete email failed:%llu", msg.mEmailUid);
	}
}

void PersistWorker::onUpdateEmail(PersistUpdateEmail &msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call email_update (%llu, %llu, '%s', '%s', %llu , '%s', '%s', %llu, %u, %u)", msg.mEmailData.m_nEmailUid, 
		msg.mEmailData.m_nTempId, msg.mEmailData.m_strDes.c_str(), msg.mEmailData.m_strContent.c_str(), msg.mEmailData.m_nPlayerUid,
		msg.mEmailData.m_strFrom.c_str(), msg.mEmailData.m_strTittle.c_str(), msg.mEmailData.m_nTime, msg.mEmailData.m_nOpenState,
		msg.mEmailData.m_nGetState);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("email_update failed:%llu", msg.mEmailData.m_nPlayerUid);
	}
}


void PersistWorker::onClearEmail(PersistClearEmail &msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call email_clear (%llu)", msg.mPlayerUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("email_update failed:%llu", msg.mPlayerUid);
	}
}

void PersistWorker::onFashionAdd(FashionAdd &msg)
{
	char sql[2048] = {0};
	snprintf(sql, sizeof(sql), "call fashions_insert(%llu,  %llu)", msg.mPlayerUid, msg.mFashionId);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);

	if(!result)
	{
		LOG_WARN("fashions_insert:%llu  failed", msg.mPlayerUid);
	}

	
}


void PersistWorker::loadAllPlayerTest()
{
	char allPlayerSql[4096] = {0};
	snprintf(allPlayerSql,sizeof(allPlayerSql),"call allplayer_load()");
	LOG_DEBUG("Sql:%s", allPlayerSql);
	bool playerloadres = mDBInterface.execSql(allPlayerSql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		return;
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	/*struct tag{
		UInt64 playerUid;
		UInt64 modelId;
	};*/
	/*List<struct tag> tagList;
	while(row)
	{
		struct tag elementnode;
		elementnode.playerUid = lynxAtoi<Guid>(row[0]);
		elementnode.modelId = lynxAtoi<Guid>(row[1]);
		tagList.insertTail(elementnode);
		row = mDBInterface.fetchRow(rs);
	}*/
	
	mDBInterface.freeResult(&rs);

	/*for(List<struct tag>::Iter * tagIter = tagList.begin(); tagIter != NULL; tagIter = tagList.next(tagIter))
	{
		
		char herofashionSql[4096] = {0};
		HeroFashionTemplate *  heroFashionTemp = HEROFASHION_TABLE().get(tagIter->mValue.modelId);
		

		UInt64 fashionUid = LogicSystem::getSingleton().generateItemGuid();
		FashionAdd fashionAdd;
		fashionAdd.mFashionId = heroFashionTemp->mFashionId;
		fashionAdd.mFashionUid = fashionUid;
		fashionAdd.mPlayerUid = tagIter->mValue.playerUid;
		PersistSystem::getSingleton().postThreadMsg(fashionAdd, fashionUid);
	

	}*/


}

void PersistWorker::onCharactorInsert(CharactorAdd & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call charactor_insert( %llu, %llu, %llu)",  msg.fashionId,
		msg.charactorId, msg.playerUid);
	LOG_DEBUG("Sql:%s", charactorsql);
	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("player charactor_insert failed %llu",  msg.playerUid);
	}


}

void PersistWorker::onCharactorUpdate(CharactorUpdate & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call charactor_update(%llu, %llu,%llu)",  msg.charactorid, msg.playeruid,msg.fashionId);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("charactor_update failed, characteruid: %llu",   msg.charactorid);
	}
}

void PersistWorker::onPersistGMResetTongbaoReq(PersistGMResetTongbao & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresettongbao(%llu)",  msg.playeruid);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresettongbao failed, playeruid: %llu",  msg.playeruid);
	}

}

void PersistWorker::onPersistGMResetMengchuReq(PersistGMResetMengchu & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetmengchu(%llu)",  msg.playeruid);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetmengchu failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetYushiGFReq(PersistGMResetYushiGF & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetyushigongfang(%llu)",  msg.playeruid);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetyushigongfang failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetJishiABReq(PersistGMResetJiShiAB & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetjishianbao(%llu)",  msg.playeruid);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetjishianbao failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetBashanSLReq(PersistGMResetBashanSL & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetbashansl(%llu)",  msg.playeruid);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetbashansl failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetWuxianTZReq(PersistGMResetWXTZ & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetwuxiantz(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetwuxiantz failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetCourage(PersistGMCourage & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetcourage(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetcourage failed, playeruid: %llu",  msg.playeruid);
	}

}

void PersistWorker::onPersistGMResetPaiWei(PersistGMPaiWei& msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetpaiwei(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetpaiwei failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetPaiWeiBuy(PersistGMPaiWeiBuy& msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetpaiweibuy(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetpaiweibuy failed, playeruid: %llu",  msg.playeruid);
	}

}

void PersistWorker::onPersistGMReset12HaoJiao(PersistGM12HaoJiao & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmreset12haojiao(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmreset12haojiao failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetFoodCook(PersistGMCookFood & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetfoodcook(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetfoodcook failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetServantCall1(PersistGMServantOnce & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetservant1(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetservant1 failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetServantCall10(PersistGMServantTen & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetservant10(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetservant10 failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetDailyTask(PersistGMResetDailyTask & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetalldailytask(%llu, %u)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetalldailytask failed, playeruid: %llu",  msg.playeruid);
	}
}


void PersistWorker::onPersistGMMonthSign(PersistGMResetMonthSign & msg )
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetmonthsign(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetmonthsign failed, playeruid: %llu",  msg.playeruid);
	}

}

void PersistWorker::onPersistGMSevenDay(PersistGMResetSevenday & msg )
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetsevenday(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetsevenday failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMSevenTrain(PersistGMResetSeventrain & msg )
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetseventrain(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetseventrain failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetGrowFound(PersistGMResetGrowfound & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetgrowfound(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetgrowfound failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetFenshi(PersistGMResetFenshi &msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetfenshi(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetfenshi failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetOnlineAward(PersistGMResetOnlineAward &msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetonlineaward(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetonlineaward failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetBaiCaiShen(PersistGMResetBaiCaiShen &msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetbaicaishen(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetbaicaishen failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetStrengthBuy(PersistGMResetBuyStrength & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetbuystrength(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetbuystrength failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetLianPu(PersistGMResetLianPu & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresetlianpu(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresetlianpu failed, playeruid: %llu",  msg.playeruid);
	}
}

void PersistWorker::onPersistGMResetTB(PersistGMResetTB & msg)
{
	char charactorsql[4096] = {0};
	snprintf(charactorsql,sizeof(charactorsql),"call gmresettongbaos(%llu, %llu)",  msg.playeruid, msg.reset);
	LOG_DEBUG("Sql:%s", charactorsql);

	bool charactorRes = mDBInterface.execSql(charactorsql);

	if(!charactorRes)
	{
		LOG_WARN("gmresettongbaos failed, playeruid: %llu",  msg.playeruid);
	}
}