#include "ClientConnection.h"
#include "CommonMsgHandler.h"
#include "LoginMsgHandler.h"

#include "ItemMsgHandler.h"

#include "DebugMsgHandler.h"
#include "RhymeMsgHandler.h"
#include "SkillMsgHandler.h"
#include "HoarStoneMsgHandler.h"
#include "HeroEquipMsgHandler.h"
#include "GemMsgHandler.h"
#include "JewelryMsgHandler.h"
#include "ServantMsgHandler.h"
#include "ChatMsgHandler.h"
#include "DetailInfoMsgHandler.h"
#include "GMMessageHandler.h"
#include "FriendMsgHandler.h"
#include "EmailMsgHandler.h"
#include "FashionMsgHandler.h"
#include "CharactorMsgHandler.h"
#include "AchieveMsgHandler.h"

#include "FireConfirm/FireConfirmManager.h"
#include "FireConfirm/Gift.h"
#include "FireConfirm/Box.h"
#include "FireConfirm/PVPSystem.h"
#include "Ranking/Ranking.h"
#include "FireConfirm/StageCalc.h"
#include "FireConfirm/ClimbTower.h"
#include "FireConfirm/Shop.h"
#include "FireConfirm/CourageChallenge.h"
#include "FireConfirm/Code.h"
#include "FireConfirm/InlineActivity.h"

using namespace Lynx;

ClientConnection::HandlerWrapper* ClientConnection::sHandlerWrapperArray[LYNX_MAX_MSG_ID + 1];

ClientConnection::ClientConnection() : mConnId(0), mReceivePending(false), mBrokenFlag(false),
mIStream(1024 - sizeof(StreamBuffer::Node), 1), mOStream(1024 - sizeof(StreamBuffer::Node), 1),
mFromIp(""), mFromPort(0)	
{
    mKeepaliveTime = TimeUtil::getTimeMilliSec();
}

ClientConnection::~ClientConnection()
{

}

bool 
ClientConnection::initial()
{
	
	mConnId = 0;
	return true;
}

void 
ClientConnection::release()
{
    mConnId = 0;
}

void 
ClientConnection::onReceived()
{
    //UInt32 packageCount = 10;

	while (1)
	{
		if (mReceivePending == false)
		{
			if (!mMsgHdr.unserialize(mIStream))
			{
				break;
			}

			LOG_INFO("[ RECEIVE ] MessageID %u length %u", mMsgHdr.mId, mMsgHdr.mBodyLen);

			if (mMsgHdr.mBodyLen > LYNX_MAX_MSG_LENGTH)
			{
				LOG_WARN("Msg length overflow id %u length %u", mMsgHdr.mId, mMsgHdr.mBodyLen);
				pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, mConnId);
				break;
			}
			
		}

		if (mMsgHdr.mBodyLen > mIStream.length())
		{
			mReceivePending = true;
			break;
		}

		mReceivePending = false;

		if (mMsgHdr.mId >= LYNX_MAX_MSG_ID)
		{
			LOG_WARN("Msg id overflow id %u", mMsgHdr.mId);
			pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, mConnId);
			break;
		}

		HandlerWrapper* handler = sHandlerWrapperArray[mMsgHdr.mId];
		if (handler)
		{
            if (isGMConnection())
            {
                if (mMsgHdr.mId < 1000)
                {
                    if (!handler->mRecvWrapper(*this, handler->mFunc))
                    {
                        LOG_WARN("Failed to unserialize msg id %u length %u", mMsgHdr.mId, mMsgHdr.mBodyLen);
                        pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, mConnId);
                        break;
                    }
                }
                else
                {
                    LOG_WARN("Connection type don't match.");
                    pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, mConnId);
                    break;
                }
            }
            else
            {
				if (NetworkSystem::getSingleton().getCanMsgRecived() == false)
				{
					LOG_WARN("all player msg can not recived");
					return;
				}

                if (!handler->mRecvWrapper(*this, handler->mFunc))
                {
                    LOG_WARN("Failed to unserialize msg id %u length %u", mMsgHdr.mId, mMsgHdr.mBodyLen);
                    pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, mConnId);
                    break;
                }
            }
		}
		else
		{
			LOG_WARN("Don't match msg id %u", mMsgHdr.mId);
			pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, mConnId);
			break;
		}
	}
}

void 
ClientConnection::registerMsgs()
{
	for (UInt32 i = 0; i < LYNX_MAX_MSG_ID; ++i)
	{
		sHandlerWrapperArray[i] = NULL;
	}

   
    registerCommonMsgs();
   
    registerLoginMsgs();
    
  
    registerItemMsgs();

 	registerFireMsgs();
   
	registerSkillMsgs();

	registerGemMsgs();

	registerHeroEquipMsgs();

	registerPVPMsgs();

	registerJewelryMsgs();

	registerRhymeMsgs();

	registerHoarStoneMsgs();

	registerServantMsgs();

	registerChatMsgs();

	registerDetailMsgs();

	registerGMMsgs();

	registerFriendsMsgs();

	registerEmailMsgs();

	registerFashionMsgs();

	registerCharactorMsgs();

	registerAchieveMsgs();

#ifdef DEBUG
    registerDebugMsgs();
#endif
}

void 
ClientConnection::deregisterMsgs()
{
	for (UInt32 i = 0; i < LYNX_MAX_MSG_ID; ++i)
	{
		if (sHandlerWrapperArray[i])
		{
			XDELETE(sHandlerWrapperArray[i]);
			sHandlerWrapperArray[i] = NULL;
		}
	}
}


void 
ClientConnection::registerCommonMsgs()
{
    registerMsgHandler<MsgAckNotify>(&CommonMsgHandler::onMsgAckNotify);
    registerMsgHandler<SyncTimeReq>(&CommonMsgHandler::onSyncTimeReq);
    registerMsgHandler<KeepalivePingReq>(&CommonMsgHandler::onKeepalivePingReq);

	
}

void 
ClientConnection::registerLoginMsgs()
{
    
    registerMsgHandler<ReconnectReq>(&LoginMsgHandler::onReconnectReq);
    registerMsgHandler<CGGameCreatePlayerReq>(&LoginMsgHandler::onCGCreatePlayerReq);
	registerMsgHandler<CGGameLoginReq>(&LoginMsgHandler::onCGGameLoginReq);
	registerMsgHandler<CGAccountLoginReq>(&LoginMsgHandler::onCGAccountLoginReq);
	
}

void 
ClientConnection::registerItemMsgs()
{
    registerMsgHandler<UseItemReq>(&ItemMsgHandler::onUseItemReq);
	registerMsgHandler<SellItemReq>(&ItemMsgHandler::onSellItemReq);

	registerMsgHandler<EquipEhanceReq>(&ItemMsgHandler::onEhanceEquipReq);
	registerMsgHandler<EquipPolishReq>(&ItemMsgHandler::onPolishEquipReq);
	registerMsgHandler<EquipPolishResetReq>(&ItemMsgHandler::onPolishResetEquipReq);

	registerMsgHandler<GemCombineReq>(&ItemMsgHandler::onGemCombineReq);
	registerMsgHandler<GemCombineOnceReq>(&ItemMsgHandler::onGemCombineOnceReq);
	registerMsgHandler<GemSetReq>(&ItemMsgHandler::onGemSetReq);
	registerMsgHandler<EquipGemCombineReq>(&ItemMsgHandler::onEquipGemCombineReq);

	registerMsgHandler<GemUnloadReq>(&ItemMsgHandler::onGemUnloadReq);
}

void ClientConnection::registerSkillMsgs()
{
	registerMsgHandler<CGSkillLevelUpReq>(&SkillMsgHandler::onSkillLevelUpReq);
	registerMsgHandler<CGSkillEquipSetReq>(&SkillMsgHandler::onSkillPositionSetReq);
}

void 
ClientConnection::registerFireMsgs()
{
	registerMsgHandler<CGConfirmDataReq>(&FireConfirmManager::onConfirmDataReq);
	registerMsgHandler<GiftReq>(&GiftManager::onOpenGiftReq);
 	registerMsgHandler<BoxReq>(&GiftManager::onOpenBoxReq);
	registerMsgHandler<CGPropertyChange>(&GiftManager::onPropertyChang);

	registerMsgHandler<RewardLotteryMsgReq>(&GiftManager::onRewardLotteryMsgReq);
	registerMsgHandler<RewardLotteryReq>(&GiftManager::onRewardLotteryReq);


	registerMsgHandler<CGChapterStart>(&FireConfirmManager::onStartCopy);
	registerMsgHandler<CGChapterLeave>(&FireConfirmManager::onLeaveCopy);
	registerMsgHandler<CGChapterEnd>(&FireConfirmManager::onEndCopy);
	registerMsgHandler<CGChapterAwardCard>(&FireConfirmManager::onChapterAwardCard);

	registerMsgHandler<CGStageMopUp>(&FireConfirmManager::onStageMopUp);

	registerMsgHandler<CGOpenChapterBox>(&StageManager::onOpenChapterBox);

	registerMsgHandler<CGFoods>(&FireConfirmManager::onFoodsReq);

	registerMsgHandler<CGStrength>(&FireConfirmManager::onStrengthReq);

	registerMsgHandler<CGRelive>(&FireConfirmManager::onRelive);

	registerMsgHandler<CGisRecievedReq>(&FireConfirmManager::onisRecieve);


	registerMsgHandler<CGWelfareBegin>(&FireConfirmManager::onWelfaleBeginReq);

	registerMsgHandler<CGWelfareEnd>(&FireConfirmManager::onWelfaleEndReq);

	registerMsgHandler<CGRecord>(&RankingManager::onRecordReq);

	registerMsgHandler<CGTwelvePalaceRecord>(&RankingManager::onTwelvePalaceRecordReq);

	registerMsgHandler<CGTwelvePalaceUnlock>(&StageCalcManager::onTwelvePalaceUnlock);

	registerMsgHandler<CGTwelvePalaceStart>(&StageCalcManager::onTwelvePalaceStartReq);

	registerMsgHandler<CGTwelvePalaceEnd>(&StageCalcManager::onTwelvePalaceEndReq);

	registerMsgHandler<CGBattleStart>(&StageCalcManager::onBattleStart);



	registerMsgHandler<InTowerChoiseReq>(&ClimbTowerManager::onInTowerChoiseReq);

	registerMsgHandler<TowerBeginReq>(&ClimbTowerManager::onTowerBeginReq);

	registerMsgHandler<TowerEndReq>(&ClimbTowerManager::onTowerEndReq);

	registerMsgHandler<TowerReliveReq>(&ClimbTowerManager::onTowerReliveReq);

	registerMsgHandler<TowerMopUpReq>(&ClimbTowerManager::onTowerMopUpReq);

	registerMsgHandler<TowerReq>(&ClimbTowerManager::onTowerReq);


	registerMsgHandler<ShopReq>(&ShopManager::onShopReq);

	registerMsgHandler<ShopBuyReq>(&ShopManager::onShopBuyReq);

	registerMsgHandler<CoinBuyReq>(&ShopManager::onCoinBuyReq);

	registerMsgHandler<CourageChallengeReq>(&CourageChallengeManager::onCourageChallengeReq);
	registerMsgHandler<CourageChallengeBeginReq>(&CourageChallengeManager::onCourageChallengeBeginReq);
	registerMsgHandler<CourageChallengeEndReq>(&CourageChallengeManager::onCourageChallengeEndReq);
	registerMsgHandler<CatCoinReq>(&CourageChallengeManager::onCatCoinReq);

	registerMsgHandler<CodeReq>(&CodeManager::onCodeReq);


	registerMsgHandler<GrowFoundReq>(&InlineActivityManager::onGrowFoundReq);
	registerMsgHandler<MonthSingnReq>(&InlineActivityManager::onMonthSingnReq);
	registerMsgHandler<OnlineWelFareReq>(&InlineActivityManager::onOnlineWelFareReq);
	registerMsgHandler<PeopleWelfareReq>(&InlineActivityManager::onPeopleWelfareReq);
	registerMsgHandler<SevenLoginAwardReq>(&InlineActivityManager::onSevenLoginAwardReq);
	registerMsgHandler<SevenDayTaskReq>(&InlineActivityManager::onSevenDayTaskReq);
	registerMsgHandler<TimeAwardReq>(&InlineActivityManager::onTimeAwardReq);


	return;
}

void 
ClientConnection::registerPVPMsgs()
{
	registerMsgHandler<PVPRoomReq>(&PVPSystem::onPVPRoomReq);

	registerMsgHandler<PVPBattleReq>(&PVPSystem::onPVPBattleReq);

	return;
}

void ClientConnection::registerRhymeMsgs()
{
	registerMsgHandler<CGRhymeEnhanceReq>(&RhymeMsgHandler::onRhymeEnhanceReq);
	
	registerMsgHandler<CGRhymeSkillSetReq>(&RhymeMsgHandler::onRhymeSkillSetReq);
	registerMsgHandler<CGRhymeAcupointActiveReq>(&RhymeMsgHandler::onRhymeAcupointActiveReq);
	registerMsgHandler<CGRhymeStepReq>(&RhymeMsgHandler::onRhymeStepReq);
	

	return;
}

void ClientConnection:: registerHoarStoneMsgs()
{
	registerMsgHandler<CGHoarStoneActiveReq>(&HoarStoneMsgHandler::onHoarStoneActiveReq);
	registerMsgHandler<CGHoarStoneLvUpReq>(&HoarStoneMsgHandler::onHoarStoneLvUpReq);
	registerMsgHandler<CGHoarStoneAddPieceReq>(&HoarStoneMsgHandler::onHoarStoneAddPieceReq);
	registerMsgHandler<CGHoarStoneAddRuneReq>(&HoarStoneMsgHandler::onHoarStoneAddRuneReq);


	registerMsgHandler<CGHoarStoneRuneEquipReq>(&HoarStoneMsgHandler::onHoarStoneRuneEquipReq);

	registerMsgHandler<CGHoarStoneRuneCombReq>(&HoarStoneMsgHandler::onHoarStoneRuneCombineReq);
	
	registerMsgHandler<CGHoarStoneStarUpReq>(&HoarStoneMsgHandler::onHoarStoneStarUpReq);
	
}

void ClientConnection::registerHeroEquipMsgs()
{
	registerMsgHandler<CGHeroEquipEnhanceReq>(&HeroEquipMsgHandler::onHeroEquipLvUp);
	registerMsgHandler<CGHeroEquipGemSetReq>(&HeroEquipMsgHandler::onHeroEquipGemSet);
	registerMsgHandler<CGHeroEquipGemSetOnceReq>(&HeroEquipMsgHandler::onHeroEquipGemSetOnce);
	registerMsgHandler<CGHeroEquipGemManualActiveReq>(&HeroEquipMsgHandler::onHeroEquipGemManualActive);
	registerMsgHandler<CGHeroEquipGemLvUpReq>(&HeroEquipMsgHandler::onHeroEquipGemLvUp);
	registerMsgHandler<CGHeroEquipGemUnloadReq>(&HeroEquipMsgHandler::onHeroEquipGemUnload);

}

void ClientConnection::registerJewelryMsgs()
{
	registerMsgHandler<CGJewelryPolishReq>(&JewelryMsgHandler::onJewelryPolish);
	registerMsgHandler<CGJewelryResetReq>(&JewelryMsgHandler::onJewelryReset);
	registerMsgHandler<CGJewelrySetReq>(&JewelryMsgHandler::onJewelrySet);
	registerMsgHandler<CGJewelryUnloadReq>(&JewelryMsgHandler::onJewelryUnload);
	registerMsgHandler<CGJewelrySellReq>(&JewelryMsgHandler::onJewelrySell);
	registerMsgHandler<CGJewelryUpdateReq>(&JewelryMsgHandler::onJewelryUpdate);
	registerMsgHandler<CGJewelrySetOnceReq>(&JewelryMsgHandler::onJewelrySetOnce);
}

void ClientConnection::registerGemMsgs()
{
	registerMsgHandler<CGGemCombineReq>(&GemMsgHandler::onGemCombineReq);
	registerMsgHandler<CGGemCombineOnceReq>(&GemMsgHandler::onGemCombineOnce);
	
}

void ClientConnection::registerServantMsgs()
{
	registerMsgHandler<CGServantCombineReq>(&ServantMsgHandler::onServantCombine);
	registerMsgHandler<CGServantLvUpReq>(&ServantMsgHandler::onServantLvUp);
	registerMsgHandler<CGServantStarUpReq>(&ServantMsgHandler::onServantStarUp);
	registerMsgHandler<CGServantFloorUpReq>(&ServantMsgHandler::onServantFloorUp);
	registerMsgHandler<CGTreasureCombineReq>(&ServantMsgHandler::onServantTreasureCombine);
	registerMsgHandler<CGTreasureEquipReq>(&ServantMsgHandler::onServantTreasureEquip);
	registerMsgHandler<CGServantBattleOpenReq>(&ServantMsgHandler::onServantBattleOpen);
	registerMsgHandler<CGAssistBattleEnhanceReq>(&ServantMsgHandler::onAssistBattleEnhance);
	registerMsgHandler<CGServantBattleSetReq>(&ServantMsgHandler::onServantBattleSet);
	registerMsgHandler<CGAssistBattleOnSetReq>(&ServantMsgHandler::onAssistBattleOneset);
	registerMsgHandler<CGServantSwitchReq>(&ServantMsgHandler::onServantSwitch);
	
}

void ClientConnection::registerChatMsgs()
{
	registerMsgHandler<CGSetBlackListReq>(&ChatMsgHandler::onMsgSetBlack);
	registerMsgHandler<CGAddFriendReq>(&ChatMsgHandler::onMsgAddFriend);
	registerMsgHandler<CGShowDetailReq>(&ChatMsgHandler::onMsgShowDetail);
	registerMsgHandler<CGClientChatReq>(&ChatMsgHandler::onMsgClientChat);
	registerMsgHandler<CGForbidChatReq>(&ChatMsgHandler::onMsgClientForbid);
	registerMsgHandler<CGResetChatReq>(&ChatMsgHandler::onMsgClientReset);
	registerMsgHandler<CGChatOnlineReq>(&ChatMsgHandler::onMsgClientOnline);
}

void ClientConnection::registerDetailMsgs()
{
	registerMsgHandler<CGClientDetailInfoReq>(&DetailInfoMsgHandler::onClientDetailInfoReq);
	registerMsgHandler<CGClientUpdateInfoReq>(&DetailInfoMsgHandler::onClientUpdateInfoReq);
	registerMsgHandler<CGPlayerRenameReq>(&DetailInfoMsgHandler::onRenameReq);
}

void ClientConnection::registerGMMsgs()
{
	registerMsgHandler<CGGMReq>(&GMMessageHandler::onMsgGMReq);
}

void ClientConnection:: registerFriendsMsgs()
{
	registerMsgHandler<CGFriendAddReq>(&FriendMsgHandler::onAddFriendReq);

	registerMsgHandler<CGFriendAddBackReq>(&FriendMsgHandler::onAddFriendBack);

	registerMsgHandler<CGFriendDelReq>(&FriendMsgHandler::onDelFriendReq);

	registerMsgHandler<CGBlackAddReq>(&FriendMsgHandler::onAddBlackReq);

	registerMsgHandler<CGBlackDelReq>(&FriendMsgHandler::onDelBlackReq);

	registerMsgHandler<CGRecommendListReq>(&FriendMsgHandler::onRecommendListReq);

	registerMsgHandler<CGFindPlayerReq>(&FriendMsgHandler::onFindPlayerReq);

	
}

void ClientConnection::registerEmailMsgs()
{
		registerMsgHandler<GMSendReq>(&EmailMsgHandler::onGMSendEmail);
	    registerMsgHandler<DelEmailReq>(&EmailMsgHandler::onDelEmailReq);
		registerMsgHandler<EmailDelOnceReq>(&EmailMsgHandler::onDelEmailOnce);
		registerMsgHandler<EmailOpenReq>(&EmailMsgHandler::onReadEmailReq);
		registerMsgHandler<EmailGetContantReq>(&EmailMsgHandler::onGetContantReq);
		registerMsgHandler<EmailGetOnceReq>(&EmailMsgHandler::onGetContantListReq);
	

}

void ClientConnection::registerFashionMsgs()
{
	registerMsgHandler<CGFashionBuyReq>(&FashionMsgHandler::onFashionBuy);
}

void ClientConnection::registerCharactorMsgs()
{
	registerMsgHandler<CGCharactorBuyReq>(&CharactorMsgHandler::onCharactorBuy);
	registerMsgHandler<CGCharactorSwitchReq>(&CharactorMsgHandler::onCharactorChange);
	registerMsgHandler<CGFashionEquipReq>(&CharactorMsgHandler::onCharactorEquipFashion);
	registerMsgHandler<CGPowerRankReq>(&CharactorMsgHandler::onPowerRank);
	
	
}


void ClientConnection::registerAchieveMsgs()
{
	registerMsgHandler<CGAchieveFinishReq>(&AchieveMsgHandler::onAchieveFinishReq);
	registerMsgHandler<CGDailyTaskAddReq>(&AchieveMsgHandler::onDailyTaskAddReq);
	registerMsgHandler<CGDailyTaskFinishReq>(&AchieveMsgHandler::onDailyTaskFinishReq);
	registerMsgHandler<CGActiveAwardGetReq>(&AchieveMsgHandler::onActiveAwardGetReq);
	

}

#ifdef DEBUG
void
ClientConnection::registerDebugMsgs()
{
    registerMsgHandler<DebugRemoteLogNotify>(&DebugMsgHandler::onDebugRemoteLogNotify);
}
#endif
