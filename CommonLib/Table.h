#ifndef __LYNX_GAME_COMMON_LIB_TABLE_H__
#define __LYNX_GAME_COMMON_LIB_TABLE_H__

#include "PlatformLib.h"
#include <string.h>
#include <assert.h>
#include "../PlatformLib/Container.h"
using namespace Lynx;
//目前使用的表
//-----------------------------------------------------------------------------------------------------------------------------------
#include "ItemTable.h"
#include "AttrEnhanceTable.h"
#include "AttrRandomTable.h"
#include "BuffTable.h"
#include "ChapterTable.h"
#include "EffectTable.h"
#include "EquipTable.h"
#include "EquipSetTable.h"
#include "FashionTable.h"
#include "GemTable.h"
#include "HeroTable.h"
#include "LevelGrowTable.h"
#include "PowerValueTable.h"
#include "PlayerExpTable.h"
#include "StageTable.h"
#include "CopyFinishTime.h"
#include "globalvar.h"
#include "Award.h"

#include "AttrPolishTable.h"

#include "PolishPointsTable.h"
#include "MaterialPointsTable.h"
#include "GemCombineTable.h"
#include "GemCombineOnceTable.h"
#include "RhymeLevelTable.h"
#include "RhymeAcupointTable.h"
#include "RhymeSkillTable.h"
#include "SkillLevelTable.h"
#include "SkillBaseTable.h"
#include "HoarstoneBaseTable.h"
#include "HoarstoneRuneTable.h"
#include "HoarstoneLvTable.h"
#include "HoarstoneStarTable.h"
#include "NewEquipLvTable.h"
#include "HeroEquipRuleTable.h"
#include "EquipGemRuleTable.h"
#include "MaterialTable.h"
#include "NewGemTable.h"
#include "NewJewelryTable.h"

#include "TypeConditionTable.h"
#include "AwardAddRateTable.h"
#include "ServantBattleOpenTable.h"
#include "ServantTable.h"
#include "ServantStarTable.h"
#include "ServantLvUpMaterialTable.h"
#include "ServantLvUpTable.h"
#include "ServantFloorTable.h"
#include "ServantTreasureTable.h"
#include "ServantAssistBattleTable.h"
#include "ServantLuckTable.h"

#include "MysteryShopTable.h"
#include "TowersTable.h"
#include "RewardLottery.h"

#include "AttrPowerTable.h"
#include "ShopTable.h"
#include "HeroFashionTable.h"
#include "CourageContentTable.h"
#include "InlineActivityTable.h"

#include "AchievementTable.h"
#include "EventTable.h"
#include "DailyTaskTable.h"
#include "DailyActiveTable.h"

#include "MonsterAward.h"

#include "RobotTable.h"
#include "ConsortMsgTable.h"

#include "RankGameTable.h"
#include "SystemInfoTable.h"
#include "ConsortLvTable.h"
#include "ConsortSignTable.h"
#include "CatTaskTable.h"
#include "KitchenQuestionTable.h"
#include "BusinessCatTable.h"
#include "SkillConvertTable.h"
#include "Eyequestiongroup.h"
#include "TicketFriendTable.h"
#include "ConsortWoodTable.h"
//-----------========================================================================================================================

namespace Lynx
{
	class TableManager : public Singleton<TableManager>
	{

	public:
		TableManager() {}
		virtual ~TableManager() {}

		bool load();
		void unload();
		bool reload(const String& fileName);
		bool reloadAll();

	public:
		//目前使用的表
		//======================================================================================================

		ItemTable mItemTable;
		AttrEnhanceTable mAttrEnhanceTable;
		AttrRandomTable mAttrRandomTable;
		BuffTable mBuffTable;
		ChapterTable mChapterTable;
		EffectTable mEffectTable;
		EquipTable mEquipTable;
		EquipSetTable mEquipSetTable;
		FashionTable mFashionTable;

		GemTable mGemTable;
		HeroTable mHeroTable;
		LevelGrowTable mLevelGrowTable;
		PowerValueTable mPowerValueTable;
		PlayerExpTable mPlayerExpTable;	
		StageTable mStageTable;
		StageTable mPalacesStageTable;

		TypeConditionTable mTypeConditionTable;
		TowerTypeTable mTowerTypeTable;
		TowerMonsterTable mTowerMonsterTable;
		TowerEventTable mTowerEventTable;
		TowerBuffTable mTowerBuffTable;		
		TowerAttrTable mTowerAttrTable;
		ShopsTable mShopsTable;
		TowerBoxTable mTowerBoxTable;
		TowersTable mTowersTable;

		AwardAddRateTable mAwardAddRateTable;
		CopyFinishTable mCopyFinishTable;
		GlobalVarTable mGlobalVarTable;
		AwardTable mAwardTable;
		AwardRouletteTable mAwardRuoletteTable;
		AwardCardTable mAwardCardTable;
		AttrPolishTable mAttrPolishTable;

		PolishPointsTable mPolishPointsTable;
		MaterialPointsTable mMaterialPointsTable;
		AwardContentTable mAwardContentTable;
		AwardContentTable mAwardContentRuoletteTable;
		LotteryTable mLotteryTable;
		RewardLotteryTable mRewardLotteryTable;
		LotteryActivityTable mLotteryActivityTable;

		CommodityTable mCommodityTable;
		GiftTable mGiftTable;
		GiftShopTable mGiftShopTable;
		MysticalshopTable mMysticalshopTable;
		ServantshopTable mServantshopTable;
		CourageshopTable mCourageshopTable;
		CoinshopTable mCoinshopTable;
		AwardContentShopTable mAwardContentShopTable;

		GainStrengthTable mGainStrengthTable;

		GainWealthTable mGainWealthTable;

		PremiumsTable mPremiumsTable;

		CoinTable mCoinTable;
		CoinGroupTable mCoinGroupTable;
		CourageContentTable mCourageContentTable;
		TerritoryTable mTerritoryTable;

		RobotTable mRobotTable;

		InlineActivityTable mInlineActivityTable;

		BoxCounterTable mBoxCounterTable;
		SkeySvalueTable mSkeySvalueTable;
		GemCombineTable mGemCombineTable;
		GemCombineOnceTable mGemCombineOnceTable;
		VipTable mVipTable;
		WelfareMarketTable mWelfareMarketTable;
		RhymeLevelTable mRhymeLevelTable;
		RhymeAcupointTable mRhymeAcupointTable;
		RhymeSkillTable mRhymeSkillTable;
		SkillLevelTable mSkillLevelTable;
		SkillBaseTable mSkillBaseTable;
		HoarstoneBaseTable mHoarStoneBaseTable;
		HoarstoneRuneTable mHoarStoneRuneTable;
		HoarstoneLvTable mHoarStoneLvTable;
		HoarstoneStarTable mHoarStoneStarTable;
		NewEquipLvTable mNewEquipLvTable;
		HeroEquipRuleTable mHeroEquipRuleTable;
		EquipGemRuleTable mEquipGemRuleTable;
		MaterialTable mMaterialTable;
		NewGemTable mNewGemTable;
		NewJewelryTable mNewJewelryTable;
		ServantBattleOpenTable mServantBattleOpenTable;
		ServantTable mServantTable;
		ServantStarTable mServantStarTable;
		ServantLvUpMaterialTable mServantMaterialTable;
		ServantLvUpTable mServantLvUpTable;
		ServantFloorTable mServantFloorTable;
		ServantTreasureTable mServantTreasureTable;
		ServantAssistBattleTable mServantAssistBattleTable;
		ServantLuckTable mServantLuckTable;
		AttrPowerTable mAttrPowerTable;
		HeroFashionTable mHeroFashionTable;
		AchievementTable mAchievementTable;
		EventTable mEventTable;
		DailyTaskTable mDailyTaskTable;
		DailyActiveTable mDailyActiveTable;
		MonsterAwardTable mMonsterAwardTable;	
		SevenDayTrainningTable mSevenDayTrainningTable;
		ConsortMsgTable mConsortMsgTable;
		RankedShopTable mRankedShopTable;
		RankRewardTable mRankRewardTable;
		SystemInfoTable mSystemInfoTable;
		ConsortLvTable mConsortLvTable;
		ConsortSignTable mConsortSignTable;
		CatTaskTable mCatTaskTable;
		KitchenQuestionTable mKitchenQueTable;
		BusinessCatTable  mBusinessCatTable;
		SkillConvertTable mSkillConvertTable;
		EyeQuestionTable mEyeQueTable;
		TicketFriendTable mTicketFriendTable;
		ConsortWoodTable mConsortWoodTable;
		//=======================================================================================================

	};


	//copy的表结构，用于动态替换dbc， 王戊辰
	class TableManagerCopy : public Singleton<TableManagerCopy>
	{

	public:
		TableManagerCopy() {}
		virtual ~TableManagerCopy() {}

		bool load();
		void unload();
		bool reload(const String& fileName);
		bool reloadAll();

	public:
		//目前使用的表
		//======================================================================================================

		ItemTable mItemTableCopy;
		AttrEnhanceTable mAttrEnhanceTableCopy;
		AttrRandomTable mAttrRandomTableCopy;
		BuffTable mBuffTableCopy;
		ChapterTable mChapterTableCopy;
		EffectTable mEffectTableCopy;
		EquipTable mEquipTableCopy;
		EquipSetTable mEquipSetTableCopy;
		FashionTable mFashionTableCopy;

		GemTable mGemTableCopy;
		HeroTable mHeroTableCopy;
		LevelGrowTable mLevelGrowTableCopy;
		PowerValueTable mPowerValueTableCopy;
		PlayerExpTable mPlayerExpTableCopy;	
		StageTable mStageTableCopy;
		StageTable mPalacesStageTableCopy;

		TypeConditionTable mTypeConditionTableCopy;
		TowerTypeTable mTowerTypeTableCopy;
		TowerMonsterTable mTowerMonsterTableCopy;
		TowerEventTable mTowerEventTableCopy;
		TowerBuffTable mTowerBuffTableCopy;		
		TowerAttrTable mTowerAttrTableCopy;
		ShopsTable mShopsTableCopy;
		TowerBoxTable mTowerBoxTableCopy;
		TowersTable mTowersTableCopy;

		AwardAddRateTable mAwardAddRateTableCopy;
		CopyFinishTable mCopyFinishTableCopy;
		GlobalVarTable mGlobalVarTableCopy;
		AwardTable mAwardTableCopy;
		AwardRouletteTable mAwardRuoletteTableCopy;
		AwardCardTable mAwardCardTableCopy;
		AttrPolishTable mAttrPolishTableCopy;

		PolishPointsTable mPolishPointsTableCopy;
		MaterialPointsTable mMaterialPointsTableCopy;
		AwardContentTable mAwardContentTableCopy;
		AwardContentTable mAwardContentRuoletteTableCopy;
		LotteryTable mLotteryTableCopy;
		RewardLotteryTable mRewardLotteryTableCopy;
		LotteryActivityTable mLotteryActivityTableCopy;

		CommodityTable mCommodityTableCopy;
		GiftTable mGiftTableCopy;
		GiftShopTable mGiftShopTableCopy;
		MysticalshopTable mMysticalshopTableCopy;
		ServantshopTable mServantshopTableCopy;
		CourageshopTable mCourageshopTableCopy;
		CoinshopTable mCoinshopTableCopy;
		AwardContentShopTable mAwardContentShopTableCopy;

		GainStrengthTable mGainStrengthTableCopy;

		GainWealthTable mGainWealthTableCopy;

		PremiumsTable mPremiumsTableCopy;

		CoinTable mCoinTableCopy;
		CoinGroupTable mCoinGroupTableCopy;
		CourageContentTable mCourageContentTableCopy;
		TerritoryTable mTerritoryTableCopy;

		RobotTable mRobotTableCopy;

		InlineActivityTable mInlineActivityTableCopy;

		BoxCounterTable mBoxCounterTableCopy;
		SkeySvalueTable mSkeySvalueTableCopy;
		GemCombineTable mGemCombineTableCopy;
		GemCombineOnceTable mGemCombineOnceTableCopy;
		VipTable mVipTableCopy;
		WelfareMarketTable mWelfareMarketTableCopy;
		RhymeLevelTable mRhymeLevelTableCopy;
		RhymeAcupointTable mRhymeAcupointTableCopy;
		RhymeSkillTable mRhymeSkillTableCopy;
		SkillConvertTable mSkillConvertTableCopy;
		EyeQuestionTable mEyeQueTableCopy;
		SkillLevelTable mSkillLevelTableCopy;
		SkillBaseTable mSkillBaseTableCopy;
		HoarstoneBaseTable mHoarStoneBaseTableCopy;
		HoarstoneRuneTable mHoarStoneRuneTableCopy;
		HoarstoneLvTable mHoarStoneLvTableCopy;
		HoarstoneStarTable mHoarStoneStarTableCopy;
		NewEquipLvTable mNewEquipLvTableCopy;
		HeroEquipRuleTable mHeroEquipRuleTableCopy;
		EquipGemRuleTable mEquipGemRuleTableCopy;
		MaterialTable mMaterialTableCopy;
		NewGemTable mNewGemTableCopy;
		NewJewelryTable mNewJewelryTableCopy;
		ServantBattleOpenTable mServantBattleOpenTableCopy;
		ServantTable mServantTableCopy;
		ServantStarTable mServantStarTableCopy;
		ServantLvUpMaterialTable mServantMaterialTableCopy;
		ServantLvUpTable mServantLvUpTableCopy;
		ServantFloorTable mServantFloorTableCopy;
		ServantTreasureTable mServantTreasureTableCopy;
		ServantAssistBattleTable mServantAssistBattleTableCopy;
		ServantLuckTable mServantLuckTableCopy;
		AttrPowerTable mAttrPowerTableCopy;
		HeroFashionTable mHeroFashionTableCopy;
		AchievementTable mAchievementTableCopy;
		EventTable mEventTableCopy;
		DailyTaskTable mDailyTaskTableCopy;
		DailyActiveTable mDailyActiveTableCopy;
		MonsterAwardTable mMonsterAwardTableCopy;
		SevenDayTrainningTable mSevenDayTrainningTableCopy;
		ConsortMsgTable mConsortMsgTableCopy;
		RankedShopTable mRankedShopTableCopy;
		RankRewardTable mRankRewardTableCopy;
		SystemInfoTable mSystemInfoTableCopy;
		ConsortLvTable mConsortLvTableCopy;
		ConsortSignTable mConsortSignTableCopy;
		CatTaskTable mCatTaskTableCopy;
		KitchenQuestionTable mKitchenQueTableCopy;
		BusinessCatTable  mBusinessCatTableCopy;
		TicketFriendTable mTicketFriendTableCopy;
		ConsortWoodTable mConsortWoodTableCopy;
		//=======================================================================================================

	};

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本体全局变量  王戊辰
extern Lynx::ItemTable* gItemTable;
extern Lynx::AttrEnhanceTable * gAttrEnhanceTable;
extern Lynx::AttrRandomTable * gAttrRandomTable;
extern Lynx::BuffTable * gBuffTable;
extern Lynx::ChapterTable * gChapterTable;
extern Lynx::EffectTable * gEffectTable;
extern Lynx::EquipTable * gEquipTable;
extern Lynx::EquipSetTable * gEquipSetTable;
extern Lynx::FashionTable * gFashionTable;
extern Lynx::GemTable * gGemTable;
extern Lynx::HeroTable * gHeroTable;
extern Lynx::LevelGrowTable * gLevelGrowTable;
extern Lynx::PowerValueTable * gPowerValueTable;
extern Lynx::PlayerExpTable * gPlayerExpTable;
extern Lynx::StageTable *gStageTable;
extern Lynx::StageTable *gPalacesStageTable;
extern Lynx::CopyFinishTable * gCopyFinishTable;
extern Lynx::TypeConditionTable * gTypeConditionTable;
extern Lynx::TowerTypeTable * gTowerTypeTable;
extern Lynx::TowerMonsterTable * gTowerMonsterTable;
extern Lynx::TowerEventTable * gTowerEventTable;
extern Lynx::TowerBuffTable * gTowerBuffTable;
extern Lynx::TowerAttrTable * gTowerAttrTable;
extern Lynx::ShopsTable * gShopsTable;
extern Lynx::TowerBoxTable * gTowerBoxTable;
extern Lynx::TowersTable * gTowersTable;
extern Lynx::RewardLotteryTable * gRewardLotteryTable;
extern Lynx::LotteryActivityTable * gLotteryActivityTable;

extern Lynx::CommodityTable * gCommodityTable;
extern Lynx::GiftTable * gGiftTable;
extern Lynx::GiftShopTable * gGiftShopTable;
extern Lynx::MysticalshopTable * gMysticalshopTable;
extern Lynx::ServantshopTable * gServantshopTable;
extern Lynx::CourageshopTable * gCourageshopTable;
extern Lynx::CoinshopTable * gCoinshopTable;
extern Lynx::AwardContentShopTable * gAwardContentShopTable;

extern Lynx::GainStrengthTable * gGainStrengthTable;

extern Lynx::GainWealthTable * gGainWealthTable;

extern Lynx::PremiumsTable * gPremiumsTable;

extern Lynx::InlineActivityTable * gInlineActivityTable;

extern Lynx::CoinTable * gCoinTable;
extern Lynx::CoinGroupTable * gCoinGroupTable;
extern Lynx::CourageContentTable * gCourageContentTable;
extern Lynx::TerritoryTable * gTerritoryTable;

extern Lynx::RobotTable * gRobotTable;



extern Lynx::AwardAddRateTable * gAwardAddRateTable;
extern Lynx::GlobalVarTable * gGlobalVarTable;
extern Lynx::AwardTable * gAwardTable;
extern Lynx::AwardRouletteTable * gAwardRuoletteTable;
extern Lynx::AttrPolishTable * gAttrPolishTable;
extern Lynx::PolishPointsTable * gPolishPointsTable;
extern Lynx::MaterialPointsTable * gMaterialPointsTable;
extern Lynx::AwardCardTable * gAwardCardTable;
extern Lynx::AwardContentTable * gAwardContentTable;
extern Lynx::AwardContentTable * gAwardContentRuoletteTable;
extern Lynx::LotteryTable * gLotteryTable;
extern Lynx::BoxCounterTable * gBoxCounterTable;
extern Lynx::SkeySvalueTable * gSkeySvalueTable;
extern Lynx::GemCombineTable * gGemCombineTable;
extern Lynx::GemCombineOnceTable * gGemCombineOnceTable;
extern Lynx::VipTable * gVipTable;
extern Lynx::WelfareMarketTable * gWelfareMarketTable;
extern Lynx::RhymeLevelTable * gRhymeLevelTable;
extern Lynx::RhymeAcupointTable * gRhymeAcupointTable;
extern Lynx::RhymeSkillTable * gRhymeSkillTable;
extern Lynx::SkillLevelTable * gSkillLevelTable;
extern Lynx::SkillBaseTable * gSkillBaseTable;
extern Lynx::SkillConvertTable * gSkillConvertTable;
extern Lynx::EyeQuestionTable * gEyeQueTable;
extern Lynx::HoarstoneBaseTable * gHoarstoneBaseTable;
extern Lynx::HoarstoneRuneTable * gHoarstoneRuneTable;
extern Lynx::HoarstoneLvTable * gHoarstoneLvTable;
extern Lynx::HoarstoneStarTable * gHoarstoneStarTable;
extern Lynx::NewEquipLvTable * gNewEquipLvTable;
extern Lynx::HeroEquipRuleTable * gHeroEquipRuleTable;
extern Lynx::EquipGemRuleTable * gEquipGemRuleTable;
extern Lynx::MaterialTable * gMaterialTable;
extern Lynx::NewGemTable * gNewGemTable;
extern Lynx::NewJewelryTable * gNewJewelryTable;
extern Lynx::ServantBattleOpenTable * gServantBattleOpenTable;
extern Lynx::ServantTable * gServantTable;
extern Lynx::ServantStarTable * gServantStarTable;
extern Lynx::ServantLvUpMaterialTable *gServantMaterialTable;
extern Lynx::ServantLvUpTable * gServantLvUpTable;
extern Lynx::ServantFloorTable * gServantFloorTable;
extern Lynx::ServantTreasureTable * gServantTreasureTable;
extern Lynx::ServantAssistBattleTable * gServantAssistBattleTable;
extern Lynx::ServantLuckTable * gServantLuckTable;
extern Lynx::AttrPowerTable * gAttrPowerTable;
extern Lynx::HeroFashionTable * gHeroFashionTable;
extern Lynx::AchievementTable * gAchievementTable;
extern Lynx::EventTable * gEventTable;
extern Lynx::DailyTaskTable * gDailyTaskTable;

extern Lynx::DailyActiveTable * gDailyActiveTable;
extern Lynx::MonsterAwardTable * gMonsterAwardTable;

extern Lynx::SevenDayTrainningTable * gSevenDayTrainningTable;
extern Lynx::ConsortMsgTable * gConsortMsgTable;

extern Lynx::RankedShopTable * gRankedShopTable;
extern Lynx::RankRewardTable * gRankRewardTable;
extern Lynx::SystemInfoTable * gSystemInfoTable;
extern Lynx::ConsortLvTable * gConsortLvTable;
extern Lynx::ConsortSignTable * gConsortSignTable;
extern Lynx::CatTaskTable * gCatTaskTable;
extern Lynx::KitchenQuestionTable * gKitchenQueTable;
extern Lynx::BusinessCatTable* gBusinessCatTable;
extern Lynx::TicketFriendTable * gTicketFriendTable;
extern Lynx::ConsortWoodTable * gConsortWoodTable;
 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copy全局变量 王戊辰

extern Lynx::ItemTable* gItemTableCopy;
extern Lynx::AttrEnhanceTable * gAttrEnhanceTableCopy;
extern Lynx::AttrRandomTable * gAttrRandomTableCopy;
extern Lynx::BuffTable * gBuffTableCopy;
extern Lynx::ChapterTable * gChapterTableCopy;
extern Lynx::EffectTable * gEffectTableCopy;
extern Lynx::EquipTable * gEquipTableCopy;
extern Lynx::EquipSetTable * gEquipSetTableCopy;
extern Lynx::FashionTable * gFashionTableCopy;
extern Lynx::GemTable * gGemTableCopy;
extern Lynx::HeroTable * gHeroTableCopy;
extern Lynx::LevelGrowTable * gLevelGrowTableCopy;
extern Lynx::PowerValueTable * gPowerValueTableCopy;
extern Lynx::PlayerExpTable * gPlayerExpTableCopy;
extern Lynx::StageTable *gStageTableCopy;
extern Lynx::StageTable *gPalacesStageTableCopy;
extern Lynx::CopyFinishTable * gCopyFinishTableCopy;
extern Lynx::TypeConditionTable * gTypeConditionTableCopy;
extern Lynx::TowerTypeTable * gTowerTypeTableCopy;
extern Lynx::TowerMonsterTable * gTowerMonsterTableCopy;
extern Lynx::TowerEventTable * gTowerEventTableCopy;
extern Lynx::TowerBuffTable * gTowerBuffTableCopy;
extern Lynx::TowerAttrTable * gTowerAttrTableCopy;
extern Lynx::ShopsTable * gShopsTableCopy;
extern Lynx::TowerBoxTable * gTowerBoxTableCopy;
extern Lynx::TowersTable * gTowersTableCopy;
extern Lynx::RewardLotteryTable * gRewardLotteryTableCopy;
extern Lynx::LotteryActivityTable * gLotteryActivityTableCopy;

extern Lynx::CommodityTable * gCommodityTableCopy;
extern Lynx::GiftTable * gGiftTableCopy;
extern Lynx::GiftShopTable * gGiftShopTableCopy;
extern Lynx::MysticalshopTable * gMysticalshopTableCopy;
extern Lynx::ServantshopTable * gServantshopTableCopy;
extern Lynx::CourageshopTable * gCourageshopTableCopy;
extern Lynx::CoinshopTable * gCoinshopTableCopy;
extern Lynx::AwardContentShopTable * gAwardContentShopTableCopy;

extern Lynx::GainStrengthTable * gGainStrengthTableCopy;

extern Lynx::GainWealthTable * gGainWealthTableCopy;

extern Lynx::PremiumsTable * gPremiumsTableCopy;

extern Lynx::InlineActivityTable * gInlineActivityTableCopy;

extern Lynx::CoinTable * gCoinTableCopy;
extern Lynx::CoinGroupTable * gCoinGroupTableCopy;
extern Lynx::CourageContentTable * gCourageContentTableCopy;
extern Lynx::TerritoryTable * gTerritoryTableCopy;

extern Lynx::RobotTable * gRobotTableCopy;



extern Lynx::AwardAddRateTable * gAwardAddRateTableCopy;
extern Lynx::GlobalVarTable * gGlobalVarTableCopy;
extern Lynx::AwardTable * gAwardTableCopy;
extern Lynx::AwardRouletteTable * gAwardRuoletteTableCopy;
extern Lynx::AttrPolishTable * gAttrPolishTableCopy;
extern Lynx::PolishPointsTable * gPolishPointsTableCopy;
extern Lynx::MaterialPointsTable * gMaterialPointsTableCopy;
extern Lynx::AwardCardTable * gAwardCardTableCopy;
extern Lynx::AwardContentTable * gAwardContentTableCopy;
extern Lynx::AwardContentTable * gAwardContentRuoletteTableCopy;
extern Lynx::LotteryTable * gLotteryTableCopy;
extern Lynx::BoxCounterTable * gBoxCounterTableCopy;
extern Lynx::SkeySvalueTable * gSkeySvalueTableCopy;
extern Lynx::GemCombineTable * gGemCombineTableCopy;
extern Lynx::GemCombineOnceTable * gGemCombineOnceTableCopy;
extern Lynx::VipTable * gVipTableCopy;
extern Lynx::WelfareMarketTable * gWelfareMarketTableCopy;
extern Lynx::RhymeLevelTable * gRhymeLevelTableCopy;
extern Lynx::RhymeAcupointTable * gRhymeAcupointTableCopy;
extern Lynx::RhymeSkillTable * gRhymeSkillTableCopy;
extern Lynx::SkillLevelTable * gSkillLevelTableCopy;
extern Lynx::SkillBaseTable * gSkillBaseTableCopy;
extern Lynx::SkillConvertTable * gSkillConvertTableCopy;
extern Lynx::EyeQuestionTable * gEyeQueTableCopy;
extern Lynx::HoarstoneBaseTable * gHoarstoneBaseTableCopy;
extern Lynx::HoarstoneRuneTable * gHoarstoneRuneTableCopy;
extern Lynx::HoarstoneLvTable * gHoarstoneLvTableCopy;
extern Lynx::HoarstoneStarTable * gHoarstoneStarTableCopy;
extern Lynx::NewEquipLvTable * gNewEquipLvTableCopy;
extern Lynx::HeroEquipRuleTable * gHeroEquipRuleTableCopy;
extern Lynx::EquipGemRuleTable * gEquipGemRuleTableCopy;
extern Lynx::MaterialTable * gMaterialTableCopy;
extern Lynx::NewGemTable * gNewGemTableCopy;
extern Lynx::NewJewelryTable * gNewJewelryTableCopy;
extern Lynx::ServantBattleOpenTable * gServantBattleOpenTableCopy;
extern Lynx::ServantTable * gServantTableCopy;
extern Lynx::ServantStarTable * gServantStarTableCopy;
extern Lynx::ServantLvUpMaterialTable *gServantMaterialTableCopy;
extern Lynx::ServantLvUpTable * gServantLvUpTableCopy;
extern Lynx::ServantFloorTable * gServantFloorTableCopy;
extern Lynx::ServantTreasureTable * gServantTreasureTableCopy;
extern Lynx::ServantAssistBattleTable * gServantAssistBattleTableCopy;
extern Lynx::ServantLuckTable * gServantLuckTableCopy;
extern Lynx::AttrPowerTable * gAttrPowerTableCopy;
extern Lynx::HeroFashionTable * gHeroFashionTableCopy;
extern Lynx::AchievementTable * gAchievementTableCopy;
extern Lynx::EventTable * gEventTableCopy;
extern Lynx::DailyTaskTable * gDailyTaskTableCopy;

extern Lynx::DailyActiveTable * gDailyActiveTableCopy;
extern Lynx::MonsterAwardTable * gMonsterAwardTableCopy;

extern Lynx::SevenDayTrainningTable * gSevenDayTrainningTableCopy;
extern Lynx::ConsortMsgTable * gConsortMsgTableCopy;
extern Lynx::RankedShopTable * gRankedShopTableCopy;
extern Lynx::RankRewardTable * gRankRewardTableCopy;
extern Lynx::SystemInfoTable * gSystemInfoTableCopy;
extern Lynx::ConsortLvTable * gConsortLvTableCopy;
extern Lynx::ConsortSignTable * gConsortSignTableCopy;
extern Lynx::CatTaskTable * gCatTaskTableCopy;
extern Lynx::KitchenQuestionTable * gKitchenQueTableCopy;
extern Lynx::BusinessCatTable* gBusinessCatTableCopy;
extern Lynx::TicketFriendTable * gTicketFriendTableCopy;
extern Lynx::ConsortWoodTable * gConsortWoodTableCopy;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//全局标记位
extern Lynx::Map<std::string,  Lynx::UInt32> m_mapGlobleFlag;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ITEM_TABLE() (*((m_mapGlobleFlag.find("item")->mValue == 1) ? (gItemTableCopy) : (gItemTable)) )

#define ATTRENHANCE_TABLE() (*((m_mapGlobleFlag.find("attrenhance")->mValue == 1) ? (gAttrEnhanceTableCopy) : (gAttrEnhanceTable)) )

#define ATTRRANDOM_TABLE() (*((m_mapGlobleFlag.find("attrrandom")->mValue == 1) ? (gAttrRandomTableCopy) : (gAttrRandomTable)) )

#define BUFF_TABLE() (*((m_mapGlobleFlag.find("buff")->mValue == 1) ? (gBuffTableCopy) : (gBuffTable)) )

#define CHAPTER_TABLE() (*((m_mapGlobleFlag.find("chapter")->mValue == 1) ? (gChapterTableCopy) : (gChapterTable)) )

#define EFFECT_TABLE() (*((m_mapGlobleFlag.find("effect")->mValue == 1) ? (gEffectTableCopy) : (gEffectTable)) )


#define EQUIP_TABLE() (*((m_mapGlobleFlag.find("equip")->mValue == 1) ? (gEquipTableCopy) : (gEquipTable)) )

#define EQUIPSET_TABLE() (*((m_mapGlobleFlag.find("equipset")->mValue == 1) ? (gEquipSetTableCopy) : (gEquipSetTable)) )

#define FASHION_TABLE() (*((m_mapGlobleFlag.find("fashion")->mValue == 1) ? (gFashionTableCopy) : (gFashionTable)) )

#define GEM_TABLE() (*((m_mapGlobleFlag.find("gem")->mValue == 1) ? (gGemTableCopy) : (gGemTable)) )

#define HERO_TABLE() (*((m_mapGlobleFlag.find("hero")->mValue == 1) ? (gHeroTableCopy) : (gHeroTable)) )

#define LEVELGROW_TABLE() (*((m_mapGlobleFlag.find("levelgrow")->mValue == 1) ? (gLevelGrowTableCopy) : (gLevelGrowTable)) )

#define POWERVALUE_TABLE() (*((m_mapGlobleFlag.find("powervalue")->mValue == 1) ? (gPowerValueTableCopy) : (gPowerValueTable)) )

#define PLAYEREXP_TABLE() (*((m_mapGlobleFlag.find("playerexp")->mValue == 1) ? (gPlayerExpTableCopy) : (gPlayerExpTable)) )

#define STAGE_TABLE() (*((m_mapGlobleFlag.find("stage")->mValue == 1) ? (gStageTableCopy) : (gStageTable)) )

#define COPY_FINISH_TABLE() (*((m_mapGlobleFlag.find("copyfinishtime")->mValue == 1) ? (gCopyFinishTableCopy) : (gCopyFinishTable)) )

#define GLOBAL_VAR_TABLE() (*((m_mapGlobleFlag.find("globalvar")->mValue == 1) ? (gGlobalVarTableCopy) : (gGlobalVarTable)) )

#define AWARD_TABLE() (*((m_mapGlobleFlag.find("award")->mValue == 1) ? (gAwardTableCopy) : (gAwardTable)) )
#define AWARD_CARD_TABLE() (*((m_mapGlobleFlag.find("awardcard")->mValue == 1) ? (gAwardCardTableCopy) : (gAwardCardTable)) )
#define ATTRPOLISH_TABLE() (*((m_mapGlobleFlag.find("equipattrpolish")->mValue == 1) ? (gAttrPolishTableCopy) : (gAttrPolishTable)) )

#define POLISHPOINTS_TABLE() (*((m_mapGlobleFlag.find("equippolishpoints")->mValue == 1) ? (gPolishPointsTableCopy) : (gPolishPointsTable)) )

#define MATERIALPOINTS_TABLE() (*((m_mapGlobleFlag.find("equipmaterialpoints")->mValue == 1) ? (gMaterialPointsTableCopy) : (gMaterialPointsTable)) )

#define AWARD_CONTENT_TABLE() (*((m_mapGlobleFlag.find("awardcontent")->mValue == 1) ? (gAwardContentTableCopy) : (gAwardContentTable)) )
#define LOTTERY_TABLE() (*((m_mapGlobleFlag.find("lottery")->mValue == 1) ? (gLotteryTableCopy) : (gLotteryTable)) )
#define BOX_COUNTER_TABLE() (*((m_mapGlobleFlag.find("boxcounter")->mValue == 1) ? (gBoxCounterTableCopy) : (gBoxCounterTable)) )

#define SKEY_SVALUE_TABLE() (*((m_mapGlobleFlag.find("skeysvalue")->mValue == 1) ? (gSkeySvalueTableCopy) : (gSkeySvalueTable)) )

#define GEMCOMBINE_TABLE() (*((m_mapGlobleFlag.find("gemcombine")->mValue == 1) ? (gGemCombineTableCopy) : (gGemCombineTable)) )

#define GEMCOMBINEONCE_TABLE() (*((m_mapGlobleFlag.find("gemcombineonce")->mValue == 1) ? (gGemCombineOnceTableCopy) : (gGemCombineOnceTable)) )

#define VIP_TABLE() (*((m_mapGlobleFlag.find("vip")->mValue == 1) ? (gVipTableCopy) : (gVipTable)) )

#define WELFAREMARKET_TABLE() (*((m_mapGlobleFlag.find("welfaremarket")->mValue == 1) ? (gWelfareMarketTableCopy) : (gWelfareMarketTable)) )

#define RHYMELEVEL_TABLE()(*((m_mapGlobleFlag.find("rhymelevel")->mValue == 1) ? (gRhymeLevelTableCopy) : (gRhymeLevelTable)) )

#define RHYMEACUPOINT_TABLE()(*((m_mapGlobleFlag.find("rhymeacupoint")->mValue == 1) ? (gRhymeAcupointTableCopy) : (gRhymeAcupointTable)) )

#define RHYMESKILL_TABLE()(*((m_mapGlobleFlag.find("rhymeskill")->mValue == 1) ? (gRhymeSkillTableCopy) : (gRhymeSkillTable)) )

#define SKILLLEVEL_TABLE()(*((m_mapGlobleFlag.find("skilllevel")->mValue == 1) ? (gSkillLevelTableCopy) : (gSkillLevelTable)) )
#define SKILLBASE_TABLE()(*((m_mapGlobleFlag.find("skillbase")->mValue == 1) ? (gSkillBaseTableCopy) : (gSkillBaseTable)) )


#define SKILLCONVERT_TABLE()(*((m_mapGlobleFlag.find("skillcompare")->mValue == 1) ? (gSkillConvertTableCopy) : (gSkillConvertTable)) )
#define EYEQUESTION_TABLE()(*((m_mapGlobleFlag.find("memoryquestionsgroup")->mValue == 1) ? (gEyeQueTableCopy) : (gEyeQueTable)) )

#define HOARSTONEBASE_TABLE()(*((m_mapGlobleFlag.find("hoarstonebase")->mValue == 1) ? (gHoarstoneBaseTableCopy) : (gHoarstoneBaseTable)) )
#define HOARSTONERUNE_TABLE()(*((m_mapGlobleFlag.find("hoarstoneequip")->mValue == 1) ? (gHoarstoneRuneTableCopy) : (gHoarstoneRuneTable)) )
#define HOARSTONELV_TABLE()(*((m_mapGlobleFlag.find("hoarstonelevel")->mValue == 1) ? (gHoarstoneLvTableCopy) : (gHoarstoneLvTable)) )

#define HOARSTONESTAR_TABLE()(*((m_mapGlobleFlag.find("hoarstonestar")->mValue == 1) ? (gHoarstoneStarTableCopy) : (gHoarstoneStarTable)) )

#define NEWEQUIPLV_TABLE()(*((m_mapGlobleFlag.find("new_equip_level")->mValue == 1) ? (gNewEquipLvTableCopy) : (gNewEquipLvTable)) )
#define HEROEQUIPRULE_TABLE()(*((m_mapGlobleFlag.find("hero_equip_rule")->mValue == 1) ? (gHeroEquipRuleTableCopy) : (gHeroEquipRuleTable)) )

#define EQUIPGEMRULE_TABLE()(*((m_mapGlobleFlag.find("equip_gem_rule")->mValue == 1) ? (gEquipGemRuleTableCopy) : (gEquipGemRuleTable)) )
#define MATERIAL_TABLE()(*((m_mapGlobleFlag.find("material")->mValue == 1) ? (gMaterialTableCopy) : (gMaterialTable)) )

#define NEWGEM_TABLE()(*((m_mapGlobleFlag.find("new_gem")->mValue == 1) ? (gNewGemTableCopy) : (gNewGemTable)) )
#define NEWJEWELRY_TABLE()(*((m_mapGlobleFlag.find("new_jewelry")->mValue == 1) ? (gNewJewelryTableCopy) : (gNewJewelryTable)) )

#define SERVANTBATTLEOPEN_TABLE()(*((m_mapGlobleFlag.find("servantbattleopen")->mValue == 1) ? (gServantBattleOpenTableCopy) : (gServantBattleOpenTable)) )
#define SERVANT_TABLE() (*((m_mapGlobleFlag.find("servant")->mValue == 1) ? (gServantTableCopy) : (gServantTable)) )
#define SERVANTSTAR_TABLE()(*((m_mapGlobleFlag.find("servantstar")->mValue == 1) ? (gServantStarTableCopy) : (gServantStarTable)) )

#define SERVANTMATERIAL_TABLE()(*((m_mapGlobleFlag.find("servantlvupmaterial")->mValue == 1) ? (gServantMaterialTableCopy) : (gServantMaterialTable)) )

#define SERVANTLVUP_TABLE()(*((m_mapGlobleFlag.find("servantlvup")->mValue == 1) ? (gServantLvUpTableCopy) : (gServantLvUpTable)) )

#define SERVANTFLOOR_TABLE()(*((m_mapGlobleFlag.find("servantfloor")->mValue == 1) ? (gServantFloorTableCopy) : (gServantFloorTable)) )
#define SERVANTTREASURE_TABLE()(*((m_mapGlobleFlag.find("servanttreasure")->mValue == 1) ? (gServantTreasureTableCopy) : (gServantTreasureTable)) )

#define SERVANTASSISTBATTLE_TABLE()(*((m_mapGlobleFlag.find("servantassisbattle")->mValue == 1) ? (gServantAssistBattleTableCopy) : (gServantAssistBattleTable)) )

#define SERVANTLUCK_TABLE()(*((m_mapGlobleFlag.find("servantlucky")->mValue == 1) ? (gServantLuckTableCopy) : (gServantLuckTable)) )

#define ATTRPOWER_TABLE()(*((m_mapGlobleFlag.find("attrpower")->mValue == 1) ? (gAttrPowerTableCopy) : (gAttrPowerTable)) )

#define HEROFASHION_TABLE()(*((m_mapGlobleFlag.find("herofashion")->mValue == 1) ? (gHeroFashionTableCopy) : (gHeroFashionTable)) )

#define ACHIEVEMENT_TABLE()(*((m_mapGlobleFlag.find("achievement")->mValue == 1) ? (gAchievementTableCopy) : (gAchievementTable)) )

#define EVENT_TABLE()(*((m_mapGlobleFlag.find("event")->mValue == 1) ? (gEventTableCopy) : (gEventTable)) )

#define DAILYTASK_TABLE()(*((m_mapGlobleFlag.find("dailytask")->mValue == 1) ? (gDailyTaskTableCopy) : (gDailyTaskTable)) )

#define DAILYACTIVE_TABLE()(*((m_mapGlobleFlag.find("active")->mValue == 1) ? (gDailyActiveTableCopy) : (gDailyActiveTable)) )

#define MONSTERAWARD_TABLE()(*((m_mapGlobleFlag.find("monsteraward")->mValue == 1) ? (gMonsterAwardTableCopy) : (gMonsterAwardTable)) )

#define SEVENDAYTRAINING_TABLE()(*((m_mapGlobleFlag.find("sevendaystrainning")->mValue == 1) ? (gSevenDayTrainningTableCopy) : (gSevenDayTrainningTable)) )

#define ROULETTE_TABLE()(*((m_mapGlobleFlag.find("roulette")->mValue == 1) ? (gAwardRuoletteTableCopy) : (gAwardRuoletteTable)) )

#define ROULETTECONTENT_TABLE()(*((m_mapGlobleFlag.find("roulettecontent")->mValue == 1) ? (gAwardContentRuoletteTableCopy) : (gAwardContentRuoletteTable)) )

#define MONSTERAWARD_TABLE()(*((m_mapGlobleFlag.find("monsteraward")->mValue == 1) ? (gMonsterAwardTableCopy) : (gMonsterAwardTable)) )

#define CONSORTMSG_TABLE()(*((m_mapGlobleFlag.find("consortmsg")->mValue == 1) ? (gConsortMsgTableCopy) : (gConsortMsgTable)) )

#define RANKENSHOP_TABLE()(*((m_mapGlobleFlag.find("rankedshop")->mValue == 1) ? (gRankedShopTableCopy) : (gRankedShopTable)) )
#define RANKREWARD_TABLE()(*((m_mapGlobleFlag.find("rankreward")->mValue == 1) ? (gRankRewardTableCopy) : (gRankRewardTable)) )
#define SYSTEMINFO_TABLE()(*((m_mapGlobleFlag.find("systeminfo")->mValue == 1) ? (gSystemInfoTableCopy) : (gSystemInfoTable)) )

#define CONSORTLV_TABLE()( *((m_mapGlobleFlag.find("guildlevel")->mValue == 1) ? (gConsortLvTableCopy) : (gConsortLvTable)))
#define CONSORTSIGN_TABLE()( *((m_mapGlobleFlag.find("guildsign")->mValue == 1) ? (gConsortSignTableCopy) : (gConsortSignTable))   )
#define CATTASK_TABLE()( *((m_mapGlobleFlag.find("cattask")->mValue == 1) ? (gCatTaskTableCopy) : (gCatTaskTable))   )
#define KITCHENQUE_TABLE()( *((m_mapGlobleFlag.find("kitchenquestions")->mValue == 1) ? (gKitchenQueTableCopy) : (gKitchenQueTable) )   )
#define BUSINESSCAT_TABLE()( *((m_mapGlobleFlag.find("businesscat")->mValue == 1) ? (gBusinessCatTableCopy) : (gBusinessCatTable) )   )
#define TICKETFRIEND_TABLE()( *((m_mapGlobleFlag.find("ticketfriend")->mValue == 1) ? (gTicketFriendTableCopy) : (gTicketFriendTable) )   )

#define wCoinShopTable() ( *((m_mapGlobleFlag.find("coinshop")->mValue == 1) ? (*gCoinShopTableCopy) : (*gCoinShopTableCopy) )   )
#define CONSORTWOOD_TABLE()( *((m_mapGlobleFlag.find("woodencat")->mValue == 1) ? (gConsortWoodTableCopy) : (gConsortWoodTable))   )


#endif 