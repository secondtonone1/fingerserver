#include "Table.h"

using namespace Lynx;

ItemTable* gItemTable = NULL;
AttrEnhanceTable * gAttrEnhanceTable = NULL;
AttrRandomTable * gAttrRandomTable = NULL;
BuffTable * gBuffTable = NULL;
ChapterTable * gChapterTable = NULL;
EffectTable * gEffectTable = NULL;
EquipTable * gEquipTable = NULL;
EquipSetTable * gEquipSetTable = NULL;
FashionTable * gFashionTable = NULL;
GemTable * gGemTable = NULL;
HeroTable * gHeroTable = NULL;
LevelGrowTable * gLevelGrowTable = NULL;
PowerValueTable * gPowerValueTable = NULL;
PlayerExpTable * gPlayerExpTable = NULL;
StageTable * gStageTable = NULL;
StageTable * gPalacesStageTable = NULL;
TypeConditionTable * gTypeConditionTable = NULL;
TowerTypeTable * gTowerTypeTable = NULL;
TowerMonsterTable * gTowerMonsterTable = NULL;
TowerEventTable * gTowerEventTable = NULL;
TowerBuffTable * gTowerBuffTable = NULL;
TowerAttrTable * gTowerAttrTable = NULL;
ShopsTable * gShopsTable = NULL;
TowerBoxTable * gTowerBoxTable = NULL;
TowersTable * gTowersTable = NULL;
RewardLotteryTable * gRewardLotteryTable= NULL;
LotteryActivityTable * gLotteryActivityTable= NULL;

CommodityTable * gCommodityTable= NULL;
GiftTable * gGiftTable= NULL;
GiftShopTable * gGiftShopTable= NULL;
MysticalshopTable * gMysticalshopTable= NULL;
ServantshopTable * gServantshopTable= NULL;
AwardContentShopTable * gAwardContentShopTable= NULL;

GainStrengthTable * gGainStrengthTable= NULL;
GainWealthTable * gGainWealthTable= NULL;
PremiumsTable * gPremiumsTable= NULL;

CoinTable * gCoinTable= NULL;
CoinGroupTable * gCoinGroupTable= NULL;
CourageContentTable * gCourageContentTable= NULL;
TerritoryTable * gTerritoryTable= NULL;

RobotTable * gRobotTable= NULL;

InlineActivityTable * gInlineActivityTable= NULL;

AwardAddRateTable * gAwardAddRateTable = NULL;
AttrPolishTable * gAttrPolishTable = NULL;
PolishPointsTable * gPolishPointsTable = NULL;
MaterialPointsTable * gMaterialPointsTable = NULL;
CopyFinishTable * gCopyFinishTable = NULL;
GlobalVarTable * gGlobalVarTable = NULL;
AwardTable * gAwardTable = NULL;
AwardCardTable * gAwardCardTable = NULL;
AwardContentTable * gAwardContentTable = NULL;
LotteryTable * gLotteryTable = NULL;
BoxCounterTable * gBoxCounterTable = NULL;
SkeySvalueTable * gSkeySvalueTable = NULL;
GemCombineTable * gGemCombineTable = NULL;
GemCombineOnceTable * gGemCombineOnceTable = NULL;

VipTable * gVipTable = NULL;
WelfareMarketTable * gWelfareMarketTable = NULL;
RhymeLevelTable * gRhymeLevelTable = NULL;
RhymeAcupointTable * gRhymeAcupointTable = NULL;
RhymeSkillTable * gRhymeSkillTable = NULL;

SkillLevelTable * gSkillLevelTable = NULL;
SkillBaseTable * gSkillBaseTable = NULL;

HoarstoneBaseTable * gHoarstoneBaseTable = NULL;
HoarstoneRuneTable * gHoarstoneRuneTable = NULL;

HoarstoneLvTable * gHoarstoneLvTable = NULL;
HoarstoneStarTable * gHoarstoneStarTable = NULL;

NewEquipLvTable * gNewEquipLvTable = NULL;
HeroEquipRuleTable * gHeroEquipRuleTable = NULL;
EquipGemRuleTable * gEquipGemRuleTable = NULL;

MaterialTable * gMaterialTable = NULL;
NewGemTable * gNewGemTable = NULL;
NewJewelryTable * gNewJewelryTable = NULL;
ServantBattleOpenTable * gServantBattleOpenTable = NULL;
ServantTable * gServantTable = NULL;
ServantStarTable * gServantStarTable = NULL;
ServantLvUpMaterialTable * gServantMaterialTable = NULL;
ServantLvUpTable * gServantLvUpTable = NULL;
ServantFloorTable * gServantFloorTable = NULL;
ServantTreasureTable * gServantTreasureTable = NULL;
ServantAssistBattleTable * gServantAssistBattleTable = NULL;
ServantLuckTable * gServantLuckTable  = NULL;
AttrPowerTable * gAttrPowerTable = NULL;
HeroFashionTable * gHeroFashionTable = NULL;
AchievementTable * gAchievementTable = NULL;
EventTable * gEventTable = NULL;
DailyTaskTable * gDailyTaskTable = NULL;
DailyActiveTable * gDailyActiveTable = NULL;

SevenDayTrainningTable * gSevenDayTrainningTable = NULL;

bool
TableManager::load()
{
	if (!mItemTable.loadFromDbc("item.dbc"))
	{
		LOG_WARN("Failed to load ItemTable table.");
		return false;
	}
	gItemTable = &mItemTable;

	if(!mAttrEnhanceTable.loadFromDbc("attrenhance.dbc"))
	{
		LOG_WARN("Failed to load AttrenhanceTable table.");
		return false;
	}

	gAttrEnhanceTable = &mAttrEnhanceTable;


	if(!mAttrRandomTable.loadFromDbc("attrrandom.dbc"))
	{
		LOG_WARN("Failed to load AttrrandomTable table.");
		return false;
	}

	gAttrRandomTable = &mAttrRandomTable;


	if(!mBuffTable.loadFromDbc("buff.dbc"))
	{
		LOG_WARN("Failed to load BuffTable table.");
		return false;

	}

	gBuffTable = &mBuffTable;

	if(!mChapterTable.loadFromDbc("chapter.dbc"))
	{
		LOG_WARN("Failed to load ChapterTable table");
		return false;
	}

	gChapterTable = &mChapterTable;

	if(!mEffectTable.loadFromDbc("effect.dbc"))
	{
		LOG_WARN("Failed to load EffectTable table");
		return false;
	}

	gEffectTable = &mEffectTable;

	if(!mEquipTable.loadFromDbc("equip.dbc"))
	{
		LOG_WARN("Failed to load EquipTable table");
		return false ;
	}

	gEquipTable = &mEquipTable;

	if(!mEquipSetTable.loadFromDbc("equipset.dbc"))
	{
		LOG_WARN("Failed to load equipset table");
		return false;
	}

	gEquipSetTable = &mEquipSetTable;

	if(!mFashionTable.loadFromDbc("fashion.dbc"))
	{
		LOG_WARN("Failed to load FashionTable table");
		return false;
	}

	gFashionTable = &mFashionTable;

	if(!mGemTable.loadFromDbc("gem.dbc"))
	{
		LOG_WARN("Failed to load GemTable table");
		return false;
	}

	gGemTable = &mGemTable;

	if(!mHeroTable.loadFromDbc("hero.dbc"))
	{
		LOG_WARN("Failed to load HeroTable table");
		return false;
	}

	gHeroTable = &mHeroTable;

	if(!mLevelGrowTable.loadFromDbc("levelgrow.dbc"))
	{
		LOG_WARN("Failed to load levelgrow table");
		return false;
	}

	gLevelGrowTable = &mLevelGrowTable;


	if(!mPowerValueTable.loadFromDbc("powervalue.dbc"))
	{
		LOG_WARN("Failed to load powervalue table");
		return false;
	}

	gPowerValueTable = &mPowerValueTable;

	if(!mPlayerExpTable.loadFromDbc("playerexp.dbc"))
	{
		LOG_WARN("Failed to load playerexp table");
		return false;
	}

	gPlayerExpTable = &mPlayerExpTable;

	if(!mPalacesStageTable.loadFromDbc("twelvepalacestage.dbc"))
	{
		LOG_WARN("Failed to load twelvepalacestage table");
		return false;
	}
	gPalacesStageTable = &mPalacesStageTable;	

	if(!mStageTable.loadFromDbc("stage.dbc"))
	{
		LOG_WARN("Failed to load stage table");
		return false;
	}
	gStageTable = &mStageTable;	


	if(!mTypeConditionTable.loadFromDbc("typecondition.dbc"))
	{
		LOG_WARN("Failed to load typecondition table");
		return false;
	}
	gTypeConditionTable = &mTypeConditionTable;	

	if(!mTowerTypeTable.loadFromDbc("towertype.dbc"))
	{
		LOG_WARN("Failed to load towertype table");
		return false;
	}
	gTowerTypeTable = &mTowerTypeTable;	

	if(!mTowerMonsterTable.loadFromDbc("towermonster.dbc"))
	{
		LOG_WARN("Failed to load towermonster table");
		return false;
	}
	gTowerMonsterTable = &mTowerMonsterTable;	


	if(!mTowerEventTable.loadFromDbc("towerevent.dbc"))
	{
		LOG_WARN("Failed to load towerevent table");
		return false;
	}
	gTowerEventTable = &mTowerEventTable;	

	if(!mTowerBuffTable.loadFromDbc("towerbuff.dbc"))
	{
		LOG_WARN("Failed to load towerbuff table");
		return false;
	}
	gTowerBuffTable = &mTowerBuffTable;	

	if(!mTowerAttrTable.loadFromDbc("towerattr.dbc"))
	{
		LOG_WARN("Failed to load towerattr table");
		return false;
	}
	gTowerAttrTable = &mTowerAttrTable;	

	if(!mShopsTable.loadFromDbc("mysteryshop.dbc"))
	{
		LOG_WARN("Failed to load mysteryshop table");
		return false;
	}
	gShopsTable = &mShopsTable;	

	if(!mTowerBoxTable.loadFromDbc("towerbox.dbc"))
	{
		LOG_WARN("Failed to load towerbox table");
		return false;
	}
	gTowerBoxTable = &mTowerBoxTable;	

	if(!mTowersTable.loadFromDbc("towers.dbc"))
	{
		LOG_WARN("Failed to load towers table");
		return false;
	}
	gTowersTable = &mTowersTable;	


	if(!mRewardLotteryTable.loadFromDbc("rewardlottery.dbc"))
	{
		LOG_WARN("Failed to load rewardlottery table");
		return false;
	}
	gRewardLotteryTable = &mRewardLotteryTable;	

	if(!mLotteryActivityTable.loadFromDbc("lotteryactivity.dbc"))
	{
		LOG_WARN("Failed to load lotteryactivity table");
		return false;
	}
	gLotteryActivityTable = &mLotteryActivityTable;	


	if(!mCommodityTable.loadFromDbc("commodity.dbc"))
	{
		LOG_WARN("Failed to load commodity table");
		return false;
	}
	gCommodityTable = &mCommodityTable;	

	if(!mGiftTable.loadFromDbc("gift.dbc"))
	{
		LOG_WARN("Failed to load gift table");
		return false;
	}
	gGiftTable = &mGiftTable;	

	if(!mGiftShopTable.loadFromDbc("giftshop.dbc"))
	{
		LOG_WARN("Failed to load giftshop table");
		return false;
	}
	gGiftShopTable = &mGiftShopTable;	

	if(!mMysticalshopTable.loadFromDbc("mysticalshop.dbc"))
	{
		LOG_WARN("Failed to load mysticalshop table");
		return false;
	}

	gMysticalshopTable = &mMysticalshopTable;

	if(!mServantshopTable.loadFromDbc("servantshop.dbc"))
	{
		LOG_WARN("Failed to load servantshop table");
		return false;
	}
	gServantshopTable = &mServantshopTable;	

	if(!mAwardContentShopTable.loadFromDbc("awardcontentshop.dbc"))
	{
		LOG_WARN("Failed to load awardcontentshop table");
		return false;
	}
	gAwardContentShopTable = &mAwardContentShopTable;	



	if(!mGainStrengthTable.loadFromDbc("gainstrength.dbc"))
	{
		LOG_WARN("Failed to load gainstrength table");
		return false;
	}
	gGainStrengthTable = &mGainStrengthTable;	

	if(!mGainWealthTable.loadFromDbc("gainwealth.dbc"))
	{
		LOG_WARN("Failed to load gainwealth table");
		return false;
	}
	gGainWealthTable = &mGainWealthTable;	

	if(!mPremiumsTable.loadFromDbc("premiums.dbc"))
	{
		LOG_WARN("Failed to load premiums table");
		return false;
	}
	gPremiumsTable = &mPremiumsTable;	




	if(!mInlineActivityTable.loadFromDbc("insideactivities.dbc"))
	{
		LOG_WARN("Failed to load insideactivities table");
		return false;
	}
	gInlineActivityTable = &mInlineActivityTable;	

	if(!mCoinTable.loadFromDbc("coin.dbc"))
	{
		LOG_WARN("Failed to load coin table");
		return false;
	}
	gCoinTable = &mCoinTable;	

	if(!mCoinGroupTable.loadFromDbc("coingroup.dbc"))
	{
		LOG_WARN("Failed to load coingroup table");
		return false;
	}
	gCoinGroupTable = &mCoinGroupTable;	

	if(!mCourageContentTable.loadFromDbc("couragecontent.dbc"))
	{
		LOG_WARN("Failed to load couragecontent table");
		return false;
	}
	gCourageContentTable = &mCourageContentTable;	

	if(!mTerritoryTable.loadFromDbc("territory.dbc"))
	{
		LOG_WARN("Failed to load territory table");
		return false;
	}
	gTerritoryTable = &mTerritoryTable;	

	if(!mRobotTable.loadFromDbc("robot.dbc"))
	{
		LOG_WARN("Failed to load robot table");
		return false;
	}
	gRobotTable = &mRobotTable;	



	if(!mAwardAddRateTable.loadFromDbc("awardaddrate.dbc"))
	{
		LOG_WARN("Failed to load awardaddrate table");
		return false;
	}
	gAwardAddRateTable= &mAwardAddRateTable;	


	if(!mCopyFinishTable.loadFromDbc("copyfinishtime.dbc"))
	{
		LOG_WARN("Failed to load copyfinishtime table");
		return false;
	}

	gCopyFinishTable = &mCopyFinishTable;

	if(!mGlobalVarTable.loadFromDbc("globalvar.dbc"))
	{
		LOG_WARN("Failed to load GlobalVarTable table");
		return false;
	}

	gGlobalVarTable = &mGlobalVarTable;
	if(!mAttrPolishTable.loadFromDbc("equipattrpolish.dbc"))
	{
		LOG_WARN("Failed to load attrpolish table");
		return false;
	}
	gAttrPolishTable = &mAttrPolishTable;

	if(!mPolishPointsTable.loadFromDbc("equippolishpoints.dbc"))
	{
		LOG_WARN("Failed to load equippolishpoints table");
		return false;
	}
	gPolishPointsTable = &mPolishPointsTable;

	if(!mMaterialPointsTable.loadFromDbc("equipmaterialpoints.dbc"))
	{
		LOG_WARN("Failed to load equipmaterialpoints table");
		return false;
	}
	gMaterialPointsTable = &mMaterialPointsTable;

	if(!mAwardTable.loadFromDbc("award.dbc"))
	{
		LOG_WARN("Failed to load AwardTable table");
		return false;
	}

	gAwardTable = &mAwardTable;

	if(!mAwardCardTable.loadFromDbc("awardcard.dbc"))
	{
		LOG_WARN("Failed to load AwardCardTable table");
		return false;
	}

	gAwardCardTable = &mAwardCardTable;

	if(!mAwardContentTable.loadFromDbc("awardcontent.dbc"))
	{
		LOG_WARN("Failed to load AwardContentTable table");
		return false;
	}

	gAwardContentTable = &mAwardContentTable;

	if(!mLotteryTable.loadFromDbc("lottery.dbc"))
	{
		LOG_WARN("Failed to load lottery table");
		return false;
	}

	gLotteryTable  = &mLotteryTable ;

	if(!mBoxCounterTable.loadFromDbc("boxcounter.dbc"))
	{
		LOG_WARN("Failed to load boxcounter table");
		return false;
	}

	gBoxCounterTable  = &mBoxCounterTable ;

	if(!mSkeySvalueTable.loadFromDbc("skeysvalue.dbc"))
	{
		LOG_WARN("Failed to load skeysvalue table");
		return false;
	}

	gSkeySvalueTable  = &mSkeySvalueTable ;

	if(!mGemCombineTable.loadFromDbc("gemcombine.dbc"))
	{
		LOG_WARN("Failed to load gemcombine table");
		return false;
	}

	gGemCombineTable  = &mGemCombineTable ;

	if(!mGemCombineOnceTable.loadFromDbc("gemcombineonce.dbc"))
	{
		LOG_WARN("Failed to load gemcombineonce table");
		return false;
	}

	gGemCombineOnceTable  = &mGemCombineOnceTable ;

	if(!mVipTable.loadFromDbc("vip.dbc"))
	{
		LOG_WARN("Failed to load vip table");
		return false;
	}

	gVipTable  = &mVipTable ;

	if(!mWelfareMarketTable.loadFromDbc("welfaremarket.dbc"))
	{
		LOG_WARN("Failed to load WelfareMarket table");
		return false;
	}

	gWelfareMarketTable = &mWelfareMarketTable;

	if(!mRhymeLevelTable.loadFromDbc("rhymelevel.dbc"))
	{
		LOG_WARN("Failed to load rhymelevel table");
		return false;
	}
	gRhymeLevelTable = &mRhymeLevelTable;

	if(!mRhymeAcupointTable.loadFromDbc("rhymeacupoint.dbc"))
	{
		LOG_WARN("Failed to load rhymeacupoint table");
		return false;
	}
	gRhymeAcupointTable = &mRhymeAcupointTable;

	if(!mRhymeSkillTable.loadFromDbc("rhymeskill.dbc"))
	{
		LOG_WARN("Failed to load rhymeskill table");
		return false;
	}
	gRhymeSkillTable = &mRhymeSkillTable;

	if(!mSkillLevelTable.loadFromDbc("skilllevel.dbc"))
	{
		LOG_WARN("Failed to load skilllevel table");
		return false;
	}
	gSkillLevelTable = &mSkillLevelTable;

	if(!mSkillBaseTable.loadFromDbc("skillbase.dbc"))
	{
		LOG_WARN("Failed to load skillbase table");
		return false;
	}
	gSkillBaseTable = &mSkillBaseTable;

	if(!mHoarStoneBaseTable.loadFromDbc("hoarstonebase.dbc"))
	{
		LOG_WARN("Failed to load hoarstonebase table");
		return false;
	}
	gHoarstoneBaseTable = &mHoarStoneBaseTable;

	if(!mHoarStoneRuneTable.loadFromDbc("hoarstoneequip.dbc"))
	{
		LOG_WARN("Failed to load hoarstoneequip table");
		return false;
	}
	gHoarstoneRuneTable = &mHoarStoneRuneTable;

	if(!mHoarStoneLvTable.loadFromDbc("hoarstonelevel.dbc"))
	{
		LOG_WARN("Failed to load hoarstonelevel table");
		return false;
	}

	gHoarstoneLvTable = &mHoarStoneLvTable;

	if(!mHoarStoneStarTable.loadFromDbc("hoarstonestar.dbc"))
	{
		LOG_WARN("Failed to load hoarstonestar table");
		return false;
	}

	gHoarstoneStarTable = &mHoarStoneStarTable;

	if(!mNewEquipLvTable.loadFromDbc("new_equip_level.dbc"))
	{
		LOG_WARN("Failed to load new_equip_level table");
		return false;
	}

	gNewEquipLvTable = &mNewEquipLvTable;

	if(!mHeroEquipRuleTable.loadFromDbc("hero_equip_rule.dbc"))
	{
		LOG_WARN("Failed to load hero_equip_rule table");
		return false;
	}

	gHeroEquipRuleTable = &mHeroEquipRuleTable;

	if(!mEquipGemRuleTable.loadFromDbc("equip_gem_rule.dbc"))
	{
		LOG_WARN("Failed to load equip_gem_rule table");
		return false;
	}

	gEquipGemRuleTable = &mEquipGemRuleTable;

	if(!mMaterialTable.loadFromDbc("material.dbc"))
	{
		LOG_WARN("Failed to load material table");
		return false;
	}

	gMaterialTable = &mMaterialTable;


	if(!mNewGemTable.loadFromDbc("new_gem.dbc"))
	{
		LOG_WARN("Failed to load new_gem table");
		return false;
	}

	gNewGemTable = &mNewGemTable;

	if(!mNewJewelryTable.loadFromDbc("new_jewelry.dbc"))
	{
		LOG_WARN("Failed to load new_jewelry table");
		return false;
	}

	gNewJewelryTable = &mNewJewelryTable;

	if(!mServantBattleOpenTable.loadFromDbc("servantbattleopen.dbc"))
	{
		LOG_WARN("Failed to load servantbattleopen table");
		return false;
	}

	gServantBattleOpenTable = &mServantBattleOpenTable;


	if(!mServantTable.loadFromDbc("servant.dbc"))
	{
		LOG_WARN("Failed to load servant table");
		return false;
	}

	gServantTable = &mServantTable;


	if(!mServantStarTable.loadFromDbc("servantstar.dbc"))
	{
		LOG_WARN("Failed to load servantstar table");
		return false;
	}

	gServantStarTable = &mServantStarTable;

	if(!mServantMaterialTable.loadFromDbc("servantlvupmaterial.dbc"))
	{
		LOG_WARN("Failed to load servantlvupmaterial table");
		return false;
	}

	gServantMaterialTable = &mServantMaterialTable;

	if(!mServantLvUpTable.loadFromDbc("servantlvup.dbc"))
	{
		LOG_WARN("Failed to load servantlvup table");
		return false;
	}

	gServantLvUpTable = &mServantLvUpTable;

	if(!mServantFloorTable.loadFromDbc("servantfloor.dbc"))
	{
		LOG_WARN("Failed to load servantfloor table");
		return false;
	}

	gServantFloorTable = &mServantFloorTable;

	if(!mServantTreasureTable.loadFromDbc("servanttreasure.dbc"))
	{
		LOG_WARN("Failed to load servanttreasure table");
		return false;
	}

	gServantTreasureTable = &mServantTreasureTable;

	if(!mServantAssistBattleTable.loadFromDbc("servantassisbattle.dbc"))
	{
		LOG_WARN("Failed to load servantassisbattle table");
		return false;
	}

	gServantAssistBattleTable = &mServantAssistBattleTable;

	if(!mServantLuckTable.loadFromDbc("servantlucky.dbc"))
	{
		LOG_WARN("Failed to load servantlucky table");
		return false;
	}

	gServantLuckTable = &mServantLuckTable;

	if(!mAttrPowerTable.loadFromDbc("attrpower.dbc"))
	{
		LOG_WARN("Failed to load attrpower table");
		return false;
	}

	gAttrPowerTable = &mAttrPowerTable;

	if(!mHeroFashionTable.loadFromDbc("herofashion.dbc"))
	{
		LOG_WARN("Failed to load herofashion table");
		return false;
	}
	gHeroFashionTable = &mHeroFashionTable;

	if(!mAchievementTable.loadFromDbc("achievement.dbc"))
	{
		LOG_WARN("Failed to load achievement table");
		return false;
	}
	gAchievementTable = &mAchievementTable;

	if(!mEventTable.loadFromDbc("event.dbc"))
	{
		LOG_WARN("Failed to load event table");
		return false;
	}
	gEventTable = &mEventTable;
	
	if(!mDailyTaskTable.loadFromDbc("dailytask.dbc"))
	{
		LOG_WARN("Failed to load dailytask table");
		return false;
	}
	gDailyTaskTable = &mDailyTaskTable;

	if(!mDailyActiveTable.loadFromDbc("active.dbc"))
	{
		LOG_WARN("Failed to load dailyactive table");
		return false;
	}
	gDailyActiveTable = &mDailyActiveTable;

	if(!mSevenDayTrainningTable.loadFromDbc("sevendaystrainning.dbc"))
	{
		LOG_WARN("Failed to load dailytask table");
		return false;
	}
	gSevenDayTrainningTable = &mSevenDayTrainningTable;
	return true;
}

void
TableManager::unload()
{

	mItemTable.clear();
	gItemTable = NULL;

	mAttrEnhanceTable.clear();
	gAttrEnhanceTable = NULL;

	mAttrRandomTable.clear();
	gAttrRandomTable = NULL;

	mBuffTable.clear();
	gBuffTable = NULL;

	mChapterTable.clear();
	gChapterTable = NULL;

	mEffectTable.clear();
	gEffectTable = NULL;

	mEquipTable.clear();
	gEquipTable = NULL;

	mEquipSetTable.clear();
	gEquipSetTable = NULL;

	mFashionTable.clear();
	gFashionTable = NULL;

	mGemTable.clear();
	gGemTable = NULL;

	mHeroTable.clear();
	gHeroTable = NULL;

	mLevelGrowTable.clear();
	gLevelGrowTable = NULL;

	mPowerValueTable.clear();
	gPowerValueTable = NULL;

	mPlayerExpTable.clear();
	gPlayerExpTable = NULL;

	mStageTable.clear();
	gStageTable = NULL;

	mPalacesStageTable.clear();
	gPalacesStageTable = NULL;

	mTypeConditionTable.clear();
	gTypeConditionTable = NULL;

	mTowerTypeTable.clear();
	gTowerTypeTable = NULL;

	mTowerMonsterTable.clear();
	gTowerMonsterTable = NULL;

	mTowerEventTable.clear();
	gTowerEventTable = NULL;

	mTowerBuffTable.clear();
	gTowerBuffTable = NULL;

	mTowerAttrTable.clear();
	gTowerAttrTable = NULL;

	mShopsTable.clear();
	gShopsTable = NULL;

	mTowerBoxTable.clear();
	gTowerBoxTable = NULL;

	mTowersTable.clear();
	gTowersTable = NULL;

	mRewardLotteryTable.clear();
	gRewardLotteryTable = NULL;

	mLotteryActivityTable.clear();
	gLotteryActivityTable = NULL;



	mCommodityTable.clear();
	gCommodityTable = NULL;

	mGiftTable.clear();
	gGiftTable = NULL;


	mGiftShopTable.clear();
	gGiftShopTable = NULL;


	mMysticalshopTable.clear();
	gMysticalshopTable = NULL;


	mServantshopTable.clear();
	gServantshopTable = NULL;

	mAwardContentShopTable.clear();
	gAwardContentShopTable = NULL;


	mGainStrengthTable.clear();
	gGainStrengthTable = NULL;

	mGainWealthTable.clear();
	gGainWealthTable = NULL;

	mPremiumsTable.clear();
	gPremiumsTable = NULL;


	mInlineActivityTable.clear();
	gInlineActivityTable = NULL;

	mCoinTable.clear();
	gCoinTable = NULL;

	mCoinGroupTable.clear();
	gCoinGroupTable = NULL;


	mCourageContentTable.clear();
	gCourageContentTable = NULL;


	mTerritoryTable.clear();
	gTerritoryTable = NULL;

	mRobotTable.clear();
	gRobotTable = NULL;

	mAwardAddRateTable.clear();
	gAwardAddRateTable = NULL;

	mCopyFinishTable.clear();
	gCopyFinishTable = NULL;

	mGlobalVarTable.clear();
	gGlobalVarTable = NULL;

	mAwardTable.clear();
	gAwardTable = NULL;

	mAttrPolishTable.clear();
	gAttrPolishTable = NULL;


	mPolishPointsTable.clear();
	gPolishPointsTable = NULL;

	mMaterialPointsTable.clear();
	gMaterialPointsTable = NULL;

	mAwardCardTable.clear();
	gAwardCardTable = NULL;

	mAwardContentTable.clear();
	gAwardContentTable = NULL;

	mLotteryTable .clear();
	gLotteryTable  = NULL;

	mBoxCounterTable .clear();
	gBoxCounterTable  = NULL;

	mSkeySvalueTable.clear();
	gSkeySvalueTable = NULL;

	mGemCombineTable.clear();
	gGemCombineTable = NULL;

	mGemCombineOnceTable.clear();
	gGemCombineOnceTable = NULL;

	mVipTable.clear();
	gVipTable = NULL;

	mWelfareMarketTable.clear();
	gWelfareMarketTable = NULL;

	mRhymeLevelTable.clear();
	gRhymeLevelTable = NULL;

	mRhymeAcupointTable.clear();
	gRhymeAcupointTable = NULL;

	mRhymeSkillTable.clear();
	gRhymeSkillTable = NULL;

	mSkillLevelTable.clear();
	gSkillLevelTable = NULL;

	mSkillBaseTable.clear();
	gSkillBaseTable = NULL;

	mHoarStoneBaseTable.clear();
	gHoarstoneBaseTable = NULL;

	mHoarStoneRuneTable.clear();
	gHoarstoneRuneTable = NULL;

	mHoarStoneLvTable.clear();
	gHoarstoneLvTable = NULL;

	mHoarStoneStarTable.clear();
	gHoarstoneStarTable = NULL;

	mNewEquipLvTable.clear();
	gNewEquipLvTable = NULL;

	mHeroEquipRuleTable.clear();
	gHeroEquipRuleTable = NULL;

	mEquipGemRuleTable.clear();
	gEquipGemRuleTable = NULL;

	mMaterialTable.clear();
	gMaterialTable = NULL;

	mNewGemTable.clear();
	gNewGemTable = NULL;

	mNewJewelryTable.clear();
	gNewJewelryTable = NULL;

	mServantBattleOpenTable.clear();
	gServantBattleOpenTable = NULL;

	mServantTable.clear();
	gServantTable = NULL;

	mServantStarTable.clear();
	gServantTable = NULL;

	mServantMaterialTable.clear();
	gServantTable = NULL;

	mServantLvUpTable.clear();
	gServantLvUpTable = NULL;

	mServantFloorTable.clear();
	gServantFloorTable = NULL;

	mServantTreasureTable.clear();
	gServantTreasureTable = NULL;

	mServantAssistBattleTable.clear();
	gServantAssistBattleTable = NULL;

	mServantLuckTable.clear();
	gServantLuckTable = NULL;

	mAttrPowerTable.clear();
	gAttrPowerTable = NULL;

	mHeroFashionTable.clear();
	gHeroFashionTable = NULL;

	mAchievementTable.clear();
	gAchievementTable = NULL;

	mEventTable.clear();
	gEventTable = NULL;

	mDailyTaskTable.clear();
	gDailyTaskTable = NULL;

	mDailyActiveTable.clear();
	gDailyActiveTable = NULL;

	mSevenDayTrainningTable.clear();
	gSevenDayTrainningTable = NULL;	

}

bool
TableManager::reload(const String& fileName)
{
	String fileNameFix = fileName;
	lynxToLowerCase(fileNameFix);

	if (fileNameFix == "item.dbc")
	{
		if (!mItemTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload ItemTable table.");
			return false;
		}
		return true;
	}

	if(fileNameFix == "attrenhance.dbc")
	{
		if (!mAttrEnhanceTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload AttrEnhanceTable table.");
			return false;
		}
		return true;
	}

	if(fileNameFix == "attrrandom.dbc")
	{
		if(!mAttrRandomTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload AttrRandomTable table.");
			return false;
		}

		return true;
	}

	if(fileNameFix == "buff.dbc")
	{
		if(!mBuffTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload buffTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "chapter.dbc")
	{
		if(!mChapterTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload chapterTable table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "effect.dbc")
	{
		if(!mEffectTable.reloadFromDbc(fileName))
		{

			LOG_WARN("Failed to reload effectTable table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "equip.dbc")
	{
		if(!mEquipTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equipTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "equipset.dbc")
	{
		if(!mEquipSetTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equipsetTable table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "fashion.dbc")
	{
		if(!mFashionTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod fashionTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "gem.dbc")
	{
		if(!mGemTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod gemTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "hero.dbc")
	{
		if(!mHeroTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod heroTable table");
			return false;
		}

		return true;
	}


	if(fileNameFix == "levelgrow.dbc")
	{
		if(!mLevelGrowTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod levelgrow table");
			return false;
		}

		return true;
	}
	if(fileNameFix == "powervalue.dbc")
	{
		if(!mPowerValueTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod powervalue table");
			return false;
		}

		return true;
	}


	if(fileNameFix == "playerexp.dbc")
	{
		if(!mPlayerExpTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod playerexp table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "stage.dbc")
	{
		if(!mStageTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod stage table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "twelvepalacestage.dbc")
	{
		if(!mPalacesStageTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod twelvepalacestage table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "copyfinishtime.dbc")
	{
		if(!mCopyFinishTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod copyfinishtime table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "globalvar.dbc")
	{
		if(!mGlobalVarTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod GlobalVarTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "award.dbc")
	{
		if(!mAwardTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardTable  table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "awardcard.dbc")
	{
		if(!mAwardCardTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardCardTable  table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "awardcontent.dbc")
	{
		if(!mAwardContentTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardContentTable  table");
			return false;
		}
		return true;
	}


	if(fileNameFix == "equipattrpolish.dbc")
	{
		if(!mAttrPolishTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload attrpolish table");
			return false;
		}
		return true;
	}



	if(fileNameFix == "lottery.dbc")
	{
		if(!mLotteryTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload lottery table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "equippolishpoints.dbc")
	{
		if(!mPolishPointsTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equippolishpoints table");
			return false;
		}


		return true;
	}

	if(fileNameFix == "boxcounter.dbc")
	{
		if(!mBoxCounterTable .reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload boxcounter table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "skeysvalue.dbc")
	{
		if(!mSkeySvalueTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload skeysvalue table");
			return false;
		}
		return true;
	}



	if(fileNameFix == "equipmaterialpoints.dbc")
	{
		if(!mPolishPointsTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equipmaterialpoints table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "gemcombine.dbc")
	{
		if(!mGemCombineTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload gemcombine table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "gemcombineonce.dbc")
	{
		if(!mGemCombineOnceTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload gemcombineonce table");
			return false;
		}
		return true;
	}
	if(fileNameFix == "vip.dbc")
	{
		if(!mVipTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload vip table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "welfaremarket.dbc")
	{
		if(!mWelfareMarketTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload WelfareMarket table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rhymelevel.dbc")
	{
		if(!mRhymeLevelTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rhymelevel table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rhymeacupoint.dbc")
	{
		if(!mRhymeAcupointTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rhymeacupoint table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rhymeskill.dbc")
	{
		if(!mRhymeSkillTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rhymeskill table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "skilllevel.dbc")
	{
		if(!mSkillLevelTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload skilllevel table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "skillbase.dbc")
	{
		if(!mSkillBaseTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload skillbase table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hoarstonebase.dbc")
	{
		if(!mHoarStoneBaseTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstonebase table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hoarstoneequip.dbc")
	{
		if(!mHoarStoneRuneTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstoneequip table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hoarstonelevel.dbc")
	{
		if(!mHoarStoneLvTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstonelevel table");
			return false;
		}
		return true;
	}


	if(fileNameFix == "hoarstonestar.dbc")
	{
		if(!mHoarStoneStarTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstonestar table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "new_equip_level.dbc")
	{
		if(!mNewEquipLvTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload new_equip_level table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hero_equip_rule.dbc")
	{
		if(!mHeroEquipRuleTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hero_equip_rule table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "equip_gem_rule.dbc")
	{
		if(!mEquipGemRuleTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equip_gem_rule table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "material.dbc")
	{
		if(!mMaterialTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload material table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "new_gem.dbc")
	{
		if(!mNewGemTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload new_gem table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "new_jewelry.dbc")
	{
		if(!mNewJewelryTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload new_jewelry table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantbattleopen.dbc")
	{
		if(!mServantBattleOpenTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantbattleopen table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servant.dbc")
	{
		if(!mServantTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servant table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantstar.dbc")
	{
		if(!mServantStarTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantstar table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantlvupmaterial.dbc")
	{
		if(!mServantMaterialTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantlvupmaterial table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantlvup.dbc")
	{
		if(!mServantLvUpTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantlvup table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantfloor.dbc")
	{
		if(!mServantFloorTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantfloor table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servanttreasure.dbc")
	{
		if(!mServantTreasureTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servanttreasure table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantassisbattle.dbc")
	{
		if(!mServantAssistBattleTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantassisbattle table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantlucky.dbc")
	{
		if(!mServantLuckTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantlucky table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "attrpower.dbc")
	{
		if(!mAttrPowerTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload attrpower table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "herofashion.dbc")
	{
		if(!mHeroFashionTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload herofashion table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "achievement.dbc")
	{
		if(!mAchievementTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload achievement table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "event.dbc")
	{
		if(!mEventTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload event table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "dailytask.dbc")
	{
		if(!mDailyTaskTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload dailytask table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "active.dbc")
	{
		if(!mDailyActiveTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload dailyactive table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "sevendaystrainning.dbc")
	{
		if(!mSevenDayTrainningTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload sevendaystrainning table");
			return false;
		}
		return true;
	}

	return false;
}

bool
TableManager::reloadAll()
{

	if (!mItemTable.reloadFromDbc("item.dbc"))
	{
		LOG_WARN("Failed to reload item table.");
		return false;
	}	

	if (!mAttrEnhanceTable.reloadFromDbc("attrenhance.dbc"))
	{
		LOG_WARN("Failed to reload AttrEnhanceTable table.");
		return false;
	}

	if(!mAttrRandomTable.reloadFromDbc("attrrandom.dbc"))
	{
		LOG_WARN("Failed to reload AttrRandomTable table.");
		return false;
	}

	if(!mBuffTable.reloadFromDbc("buff.dbc"))
	{
		LOG_WARN("Failed to reload BuffTable table.");
		return false;
	}

	if(!mChapterTable.reloadFromDbc("chapter.dbc"))
	{
		LOG_WARN("Failed to reload ChapterTable table.");
		return false;
	}

	if(!mEffectTable.reloadFromDbc("effect.dbc"))
	{
		LOG_WARN("Failed to reload EffectTable table");
		return false;
	}

	if(!mEquipTable.reloadFromDbc("equip.dbc"))
	{
		LOG_WARN("Failed to reload EquipTable table");
		return false;
	}

	if(!mEquipSetTable.reloadFromDbc("equipset.dbc"))
	{
		LOG_WARN("Failed to reload equipset table");
		return false;
	}

	if(!mFashionTable.reloadFromDbc("fashion.dbc"))
	{
		LOG_WARN("Failed to reload fashionTable table");
		return false;
	}

	if(!mGemTable.reloadFromDbc("gem.dbc"))
	{
		LOG_WARN("Failed to reload gemTable table");
		return false;
	}

	if(!mHeroTable.reloadFromDbc("hero.dbc"))
	{
		LOG_WARN("Failed to reload heroTable table");
		return false;
	}

	if(!mLevelGrowTable.reloadFromDbc("levelgrow.dbc"))
	{
		LOG_WARN("Failed to reload levelgrow table");
		return false;
	}

	if(!mPowerValueTable.reloadFromDbc("powervalue.dbc"))
	{
		LOG_WARN("Failed to reload powervalue table");
		return false;
	}

	if(!mPlayerExpTable.reloadFromDbc("playerexp.dbc"))
	{
		LOG_WARN("Failed to reload playerexp table");
		return false;
	}


	if(!mStageTable.reloadFromDbc("stage.dbc"))
	{
		LOG_WARN("Failed to reload stage table");
		return false;
	}

	if(!mPalacesStageTable.reloadFromDbc("twelvepalacestage.dbc"))
	{
		LOG_WARN("Failed to reload twelvepalacestage table");
		return false;
	}


	if(!mCopyFinishTable.reloadFromDbc("copyfinishtime.dbc"))
	{
		LOG_WARN("Failed to relaod copyfinishtime table");
		return false;
	}


	if(!mGlobalVarTable.reloadFromDbc("globalvar.dbc"))
	{
		LOG_WARN("Failed to relaod globalvar table");
		return false;
	}
	if(!mAwardTable.reloadFromDbc("award.dbc"))
	{
		LOG_WARN("Failed to relaod AwardTable table");
		return false;
	}
	if(!mAwardCardTable.reloadFromDbc("awardcard.dbc"))
	{
		LOG_WARN("Failed to relaod AwardCardTable table");
		return false;
	}
	if(!mAwardContentTable.reloadFromDbc("awardcontent.dbc"))
	{
		LOG_WARN("Failed to relaod AwardContentTable table");
		return false;
	}

	if(!mAttrPolishTable.reloadFromDbc("equipattrpolish.dbc"))
	{
		LOG_WARN("Failed to reload attrpolish table");
		return false;
	}



	if(!mPolishPointsTable.reloadFromDbc("equippolishpoints.dbc"))
	{
		LOG_WARN("Failed to reload equippolishpoints table");
		return false;
	}

	if(!mMaterialPointsTable.reloadFromDbc("equipmaterialpoints.dbc"))
	{
		LOG_WARN("Failed to reload equipmaterialpoints table");
		return false;
	}

	if(!mGlobalVarTable.reloadFromDbc("globalvar.dbc"))
	{
		LOG_WARN("Failed to relaod GlobalVarTable table");
		return false;
	}

	if(!mGemCombineTable.reloadFromDbc("gemcombine.dbc"))
	{
		LOG_WARN("Failed to reload gemcombine table");
		return false;
	}

	if(!mGemCombineOnceTable.reloadFromDbc("gemcombineonce.dbc"))
	{
		LOG_WARN("Failed to reload gemcombineonce table");
		return false;
	}

	if(!mAwardCardTable.reloadFromDbc("awardcard.dbc"))
	{
		LOG_WARN("Failed to AwardCardTable  table");
		return false;
	}

	if(!mAwardContentTable.reloadFromDbc("awardcontent.dbc"))
	{
		LOG_WARN("Failed to AwardContentTable  table");
		return false;
	}

	if(!mAttrPolishTable.reloadFromDbc("equipattrpolish.dbc"))
	{
		LOG_WARN("Failed to reload attrpolish table");
		return false;
	}	

	if(!mLotteryTable.reloadFromDbc("lottery.dbc"))
	{
		LOG_WARN("Failed to reload lottery table");
		return false;
	}

	if(!mBoxCounterTable .reloadFromDbc("boxcounter.dbc"))
	{
		LOG_WARN("Failed to reload boxcounter table");
		return false;
	}

	if(!mSkeySvalueTable.reloadFromDbc("skeysvalue.dbc"))
	{
		LOG_WARN("Failed to reload skeysvalue table");
		return false;
	}

	if(!mVipTable.reloadFromDbc("vip.dbc"))
	{
		LOG_WARN("Failed to reload vip table");
		return false;
	}
	if(!mWelfareMarketTable.reloadFromDbc("welfaremarket.dbc"))
	{
		LOG_WARN("Failed to reload WelfareMarket table");
		return false;
	}


	if(!mRhymeLevelTable.reloadFromDbc("rhymelevel.dbc"))
	{
		LOG_WARN("Failed to reload rhymelevel table");
		return false;
	}

	if(!mRhymeAcupointTable.reloadFromDbc("rhymeacupoint.dbc"))
	{
		LOG_WARN("Failed to reload rhymeacupoint table");
		return false;
	}

	if(!mRhymeSkillTable.reloadFromDbc("rhymeskill.dbc"))
	{
		LOG_WARN("Failed to reload rhymeskill table");
		return false;
	}

	if(!mSkillLevelTable.reloadFromDbc("skilllevel.dbc"))
	{
		LOG_WARN("Failed to reload skilllevel table");
		return false;
	}

	if(!mSkillBaseTable.reloadFromDbc("skillbase.dbc"))
	{
		LOG_WARN("Failed to reload skillBase table");
		return false;
	}

	if(!mHoarStoneBaseTable.reloadFromDbc("hoarstonebase.dbc"))
	{
		LOG_WARN("Failed to reload hoarstonebase table");
		return false;
	}

	if(!mHoarStoneRuneTable.reloadFromDbc("hoarstoneequip.dbc"))
	{
		LOG_WARN("Failed to reload hoarstoneequip table");
		return false;
	}

	if(!mHoarStoneRuneTable.reloadFromDbc("hoarstonelevel.dbc"))
	{
		LOG_WARN("Failed to reload hoarstonelevel table");
		return false;
	}

	if(!mHoarStoneStarTable.reloadFromDbc("hoarstonestar.dbc"))
	{
		LOG_WARN("Failed to reload hoarstonestar table");
		return false;
	}

	if(!mNewEquipLvTable.reloadFromDbc("new_equip_level.dbc"))
	{
		LOG_WARN("Failed to reload new_equip_level table");
		return false;
	}

	if(!mHeroEquipRuleTable.reloadFromDbc("hero_equip_rule.dbc"))
	{
		LOG_WARN("Failed to reload hero_equip_rule table");
		return false;
	}

	if(!mEquipGemRuleTable.reloadFromDbc("equip_gem_rule.dbc"))
	{
		LOG_WARN("Failed to reload hero_equip_rule table");
		return false;
	}

	if(!mMaterialTable.reloadFromDbc("material.dbc"))
	{
		LOG_WARN("Failed to reload material table");
		return false;
	}

	if(!mNewGemTable.reloadFromDbc("new_gem.dbc"))
	{
		LOG_WARN("Failed to reload new_gem table");
		return false;
	}

	if(!mNewJewelryTable.reloadFromDbc("new_jewelry.dbc"))
	{
		LOG_WARN("Failed to reload new_jewelry table");
		return false;
	}

	if(!mServantBattleOpenTable.reloadFromDbc("servantbattleopen.dbc"))
	{
		LOG_WARN("Failed to reload servantbattleopen table");
		return false;
	}

	if(!mServantTable.reloadFromDbc("servant.dbc"))
	{
		LOG_WARN("Failed to reload servant table");
		return false;
	}

	if(!mServantStarTable.reloadFromDbc("servantstar.dbc"))
	{
		LOG_WARN("Failed to reload servantstar table");
		return false;
	}

	if(!mServantMaterialTable.reloadFromDbc("servantlvupmaterial.dbc"))
	{
		LOG_WARN("Failed to reload servantlvupmaterial table");
		return false;
	}

	if(!mServantLvUpTable.reloadFromDbc("servantlvup.dbc"))
	{
		LOG_WARN("Failed to reload servantlvup table");
		return false;
	}

	if(!mServantFloorTable.reloadFromDbc("servantfloor.dbc"))
	{
		LOG_WARN("Failed to reload servantfloor table");
		return false;
	}

	if(!mServantTreasureTable.reloadFromDbc("servanttreasure.dbc"))
	{
		LOG_WARN("Failed to reload servanttreasure table");
		return false;
	}

	if(!mServantAssistBattleTable.reloadFromDbc("servantassisbattle.dbc"))
	{
		LOG_WARN("Failed to reload servantassisbattle table");
		return false;
	}

	if(!mServantLuckTable.reloadFromDbc("servantlucky.dbc"))
	{
		LOG_WARN("Failed to reload servantlucky table");
		return false;
	}

	if(!mTowerTypeTable.reloadFromDbc("towertype.dbc"))
	{
		LOG_WARN("Failed to reload towertype table");
		return false;
	}

	if(!mTowerMonsterTable.reloadFromDbc("towermonster.dbc"))
	{
		LOG_WARN("Failed to reload towermonster table");
		return false;
	}

	if(!mTowerEventTable.reloadFromDbc("towerevent.dbc"))
	{
		LOG_WARN("Failed to reload towerevent table");
		return false;
	}

	if(!mTowerBuffTable.reloadFromDbc("towerbuff.dbc"))
	{
		LOG_WARN("Failed to reload towerbuff table");
		return false;
	}

	if(!mTowerAttrTable.reloadFromDbc("towerattr.dbc"))
	{
		LOG_WARN("Failed to reload towerattr table");
		return false;
	}

	if(!mShopsTable.reloadFromDbc("mysteryshop.dbc"))
	{
		LOG_WARN("Failed to reload mysteryshop table");
		return false;
	}

	if(!mTowerBoxTable.reloadFromDbc("towerbox.dbc"))
	{
		LOG_WARN("Failed to reload towerbox table");
		return false;
	}

	if(!mTowersTable.reloadFromDbc("towers.dbc"))
	{
		LOG_WARN("Failed to reload towers table");
		return false;
	}

	if(!mRewardLotteryTable.reloadFromDbc("rewardlottery.dbc"))
	{
		LOG_WARN("Failed to reload rewardlottery table");
		return false;
	}

	if(!mLotteryActivityTable.reloadFromDbc("lotteryactivity.dbc"))
	{
		LOG_WARN("Failed to reload lotteryactivity table");
		return false;
	}

	if(!mAttrPowerTable.reloadFromDbc("attrpower.dbc"))
	{
		LOG_WARN("Failed to reload attrpower table");
		return false;
	}



	if(!mCommodityTable.reloadFromDbc("commodity.dbc"))
	{
		LOG_WARN("Failed to reload commodity table");
		return false;
	}
	if(!mGiftTable.reloadFromDbc("gift.dbc"))
	{
		LOG_WARN("Failed to reload gift table");
		return false;
	}
	if(!mGiftShopTable.reloadFromDbc("giftshop.dbc"))
	{
		LOG_WARN("Failed to reload giftshop table");
		return false;
	}
	if(!mMysticalshopTable.reloadFromDbc("mysticalshop.dbc"))
	{
		LOG_WARN("Failed to reload mysticalshop table");
		return false;
	}
	if(!mServantshopTable.reloadFromDbc("servantshop.dbc"))
	{
		LOG_WARN("Failed to reload servantshop table");
		return false;
	}
	if(!mAwardContentShopTable.reloadFromDbc("awardcontentshop.dbc"))
	{
		LOG_WARN("Failed to reload awardcontentshop table");
		return false;
	}

	if(!mHeroFashionTable.reloadFromDbc("herofashion.dbc"))
	{
		LOG_WARN("Failed to reload herofashion table");
		return false;
	}

	if(!mAchievementTable.reloadFromDbc("achievement.dbc"))
	{
		LOG_WARN("Failed to reload achievement table");
		return false;
	}

	if(!mEventTable.reloadFromDbc("event.dbc"))
	{
		LOG_WARN("Failed to reload event table");
		return false;
	}

	
	if(!mDailyTaskTable.reloadFromDbc("dailytask.dbc"))
	{
		LOG_WARN("Failed to reload dailytask table");
		return false;
	}

	if(!mDailyActiveTable.reloadFromDbc("active.dbc"))
	{
		LOG_WARN("Failed to reload dailyactive table");
		return false;
	}

	if(!mSevenDayTrainningTable.reloadFromDbc("sevendaystrainning.dbc"))
	{
		LOG_WARN("Failed to reload sevendaystrainning table");
		return false;
	}
	
	return true;
}

