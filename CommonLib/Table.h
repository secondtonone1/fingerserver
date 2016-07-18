#ifndef __LYNX_GAME_COMMON_LIB_TABLE_H__
#define __LYNX_GAME_COMMON_LIB_TABLE_H__

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

#include "RobotTable.h"

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
		AwardCardTable mAwardCardTable;
		AttrPolishTable mAttrPolishTable;

		PolishPointsTable mPolishPointsTable;
		MaterialPointsTable mMaterialPointsTable;
		AwardContentTable mAwardContentTable;
		LotteryTable mLotteryTable;
		RewardLotteryTable mRewardLotteryTable;
		LotteryActivityTable mLotteryActivityTable;

		CommodityTable mCommodityTable;
		GiftTable mGiftTable;
		GiftShopTable mGiftShopTable;
		MysticalshopTable mMysticalshopTable;
		ServantshopTable mServantshopTable;
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
		SevenDayTrainningTable mSevenDayTrainningTable;
		//=======================================================================================================

	};
}


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
extern Lynx::AttrPolishTable * gAttrPolishTable;
extern Lynx::PolishPointsTable * gPolishPointsTable;
extern Lynx::MaterialPointsTable * gMaterialPointsTable;
extern Lynx::AwardCardTable * gAwardCardTable;
extern Lynx::AwardContentTable * gAwardContentTable;
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

extern Lynx::SevenDayTrainningTable * gSevenDayTrainningTable;

#define ITEM_TABLE() (*gItemTable)
#define ATTRENHANCE_TABLE() (*gAttrEnhanceTable)
#define ATTRRANDOM_TABLE() (*gAttrRandomTable)
#define BUFF_TABLE() (*gBuffTable)
#define CHAPTER_TABLE() (*gChapterTable)
#define EFFECT_TABLE() (*gEffectTable)
#define EQUIP_TABLE() (*gEquipTable)
#define EQUIPSET_TABLE() (*gEquipSetTable)
#define FASHION_TABLE() (*gFashionTable)
#define GEM_TABLE() (*gGemTable)
#define HERO_TABLE() (*gHeroTable)
#define LEVELGROW_TABLE() (*gLevelGrowTable)
#define POWERVALUE_TABLE() (*gPowerValueTable)
#define PLAYEREXP_TABLE() (*gPlayerExpTable)
#define STAGE_TABLE() (*gStageTable)
#define COPY_FINISH_TABLE() (*gCopyFinishTable)
#define GLOBAL_VAR_TABLE() (*gGlobalVarTable)
#define AWARD_TABLE() (*gAwardTable)
#define AWARD_CARD_TABLE() (*gAwardCardTable)
#define ATTRPOLISH_TABLE() (*gAttrPolishTable)
#define POLISHPOINTS_TABLE() (*gPolishPointsTable)
#define MATERIALPOINTS_TABLE() (*gMaterialPointsTable)
#define AWARD_CONTENT_TABLE() (*gAwardContentTable)
#define LOTTERY_TABLE() (*gLotteryTable)
#define BOX_COUNTER_TABLE() (*gBoxCounterTable)
#define SKEY_SVALUE_TABLE() (*gSkeySvalueTable)
#define GEMCOMBINE_TABLE() (*gGemCombineTable)
#define GEMCOMBINEONCE_TABLE() (*gGemCombineOnceTable)
#define VIP_TABLE() (*gVipTable)
#define WELFAREMARKET_TABLE() (*gWelfareMarketTable)
#define RHYMELEVEL_TABLE()(*gRhymeLevelTable)
#define RHYMEACUPOINT_TABLE()(*gRhymeAcupointTable)
#define RHYMESKILL_TABLE()(*gRhymeSkillTable)
#define SKILLLEVEL_TABLE()(*gSkillLevelTable)
#define SKILLBASE_TABLE()(*gSkillBaseTable)
#define HOARSTONEBASE_TABLE()(*gHoarstoneBaseTable)
#define HOARSTONERUNE_TABLE()(*gHoarstoneRuneTable)
#define HOARSTONELV_TABLE()(*gHoarstoneLvTable)
#define HOARSTONESTAR_TABLE()(*gHoarstoneStarTable)
#define NEWEQUIPLV_TABLE()(*gNewEquipLvTable)
#define HEROEQUIPRULE_TABLE()(*gHeroEquipRuleTable)
#define EQUIPGEMRULE_TABLE()(*gEquipGemRuleTable)
#define MATERIAL_TABLE()(*gMaterialTable)
#define NEWGEM_TABLE()(*gNewGemTable)
#define NEWJEWELRY_TABLE()(*gNewJewelryTable)
#define SERVANTBATTLEOPEN_TABLE()(*gServantBattleOpenTable)
#define SERVANT_TABLE()(*gServantTable)
#define SERVANTSTAR_TABLE()(*gServantStarTable)
#define SERVANTMATERIAL_TABLE()(*gServantMaterialTable)
#define SERVANTLVUP_TABLE()(*gServantLvUpTable)
#define SERVANTFLOOR_TABLE()(*gServantFloorTable)
#define SERVANTTREASURE_TABLE()(*gServantTreasureTable)
#define SERVANTASSISTBATTLE_TABLE()(*gServantAssistBattleTable)
#define SERVANTLUCK_TABLE()(*gServantLuckTable)
#define ATTRPOWER_TABLE()(*gAttrPowerTable)
#define HEROFASHION_TABLE()(*gHeroFashionTable)
#define ACHIEVEMENT_TABLE()(*gAchievementTable)
#define EVENT_TABLE()(*gEventTable)
#define DAILYTASK_TABLE()(*gDailyTaskTable)
#define DAILYACTIVE_TABLE()(*gDailyActiveTable)

#define SEVENDAYTRAINING_TABLE()(*gSevenDayTrainningTable)
#endif