#include "Table.h"

using namespace Lynx;

Lynx::Map<std::string,  Lynx::UInt32> m_mapGlobleFlag;

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
CourageshopTable * gCourageshopTable= NULL;
CoinshopTable * gCoinshopTable= NULL;

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
AwardRouletteTable * gAwardRuoletteTable = NULL;
AwardContentTable * gAwardContentRuoletteTable = NULL;

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
SkillConvertTable * gSkillConvertTable = NULL;
EyeQuestionTable * gEyeQueTable = NULL;

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
MonsterAwardTable * gMonsterAwardTable = NULL;

SevenDayTrainningTable * gSevenDayTrainningTable = NULL;
ConsortMsgTable * gConsortMsgTable = NULL;
RankedShopTable * gRankedShopTable = NULL;
RankRewardTable * gRankRewardTable = NULL;
ConsortLvTable * gConsortLvTable = NULL;
SystemInfoTable * gSystemInfoTable = NULL;
ConsortSignTable * gConsortSignTable = NULL;
CatTaskTable * gCatTaskTable = NULL;
KitchenQuestionTable * gKitchenQueTable = NULL;
BusinessCatTable * gBusinessCatTable = NULL;
TicketFriendTable * gTicketFriendTable = NULL;
ConsortWoodTable * gConsortWoodTable = NULL;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//copy 版本的全局变量
ItemTable* gItemTableCopy = NULL;
AttrEnhanceTable * gAttrEnhanceTableCopy = NULL;
AttrRandomTable * gAttrRandomTableCopy = NULL;
BuffTable * gBuffTableCopy = NULL;
ChapterTable * gChapterTableCopy = NULL;
EffectTable * gEffectTableCopy = NULL;
EquipTable * gEquipTableCopy = NULL;
EquipSetTable * gEquipSetTableCopy = NULL;
FashionTable * gFashionTableCopy = NULL;
GemTable * gGemTableCopy = NULL;
HeroTable * gHeroTableCopy = NULL;
LevelGrowTable * gLevelGrowTableCopy = NULL;
PowerValueTable * gPowerValueTableCopy = NULL;
PlayerExpTable * gPlayerExpTableCopy = NULL;
StageTable *gStageTableCopy = NULL;
StageTable *gPalacesStageTableCopy = NULL;
CopyFinishTable * gCopyFinishTableCopy = NULL;
TypeConditionTable * gTypeConditionTableCopy = NULL;
TowerTypeTable * gTowerTypeTableCopy = NULL;
TowerMonsterTable * gTowerMonsterTableCopy = NULL;
TowerEventTable * gTowerEventTableCopy = NULL;
TowerBuffTable * gTowerBuffTableCopy = NULL;
TowerAttrTable * gTowerAttrTableCopy = NULL;
ShopsTable * gShopsTableCopy = NULL;
TowerBoxTable * gTowerBoxTableCopy = NULL;
TowersTable * gTowersTableCopy=NULL;
RewardLotteryTable * gRewardLotteryTableCopy = NULL;
LotteryActivityTable * gLotteryActivityTableCopy=NULL;
CommodityTable * gCommodityTableCopy = NULL;
GiftTable * gGiftTableCopy = NULL;
GiftShopTable * gGiftShopTableCopy = NULL;
MysticalshopTable * gMysticalshopTableCopy = NULL;
ServantshopTable * gServantshopTableCopy = NULL;
CourageshopTable * gCourageshopTableCopy = NULL;
CoinshopTable * gCoinshopTableCopy = NULL;

AwardContentShopTable * gAwardContentShopTableCopy = NULL;
GainStrengthTable * gGainStrengthTableCopy = NULL;
GainWealthTable * gGainWealthTableCopy = NULL;
PremiumsTable * gPremiumsTableCopy= NULL;
InlineActivityTable * gInlineActivityTableCopy  = NULL;
CoinTable * gCoinTableCopy  = NULL;
CoinGroupTable * gCoinGroupTableCopy  = NULL;
CourageContentTable * gCourageContentTableCopy  = NULL;
TerritoryTable * gTerritoryTableCopy  = NULL;
RobotTable * gRobotTableCopy  = NULL;
AwardAddRateTable * gAwardAddRateTableCopy  = NULL;
GlobalVarTable * gGlobalVarTableCopy = NULL;
AwardTable * gAwardTableCopy = NULL;
AwardRouletteTable * gAwardRuoletteTableCopy = NULL;
AttrPolishTable * gAttrPolishTableCopy = NULL;
PolishPointsTable * gPolishPointsTableCopy = NULL;
MaterialPointsTable * gMaterialPointsTableCopy = NULL;
AwardCardTable * gAwardCardTableCopy = NULL;
AwardContentTable * gAwardContentTableCopy = NULL;
AwardContentTable * gAwardContentRuoletteTableCopy = NULL;
LotteryTable * gLotteryTableCopy = NULL;
BoxCounterTable * gBoxCounterTableCopy = NULL;
SkeySvalueTable * gSkeySvalueTableCopy = NULL;
GemCombineTable * gGemCombineTableCopy = NULL;
GemCombineOnceTable * gGemCombineOnceTableCopy = NULL;
VipTable * gVipTableCopy = NULL;
WelfareMarketTable * gWelfareMarketTableCopy = NULL;
RhymeLevelTable * gRhymeLevelTableCopy = NULL;
RhymeAcupointTable * gRhymeAcupointTableCopy = NULL;
RhymeSkillTable * gRhymeSkillTableCopy = NULL;
SkillLevelTable * gSkillLevelTableCopy = NULL;
SkillBaseTable * gSkillBaseTableCopy = NULL;
SkillConvertTable * gSkillConvertTableCopy = NULL;
EyeQuestionTable * gEyeQueTableCopy = NULL;
HoarstoneBaseTable * gHoarstoneBaseTableCopy = NULL;
HoarstoneRuneTable * gHoarstoneRuneTableCopy = NULL;
HoarstoneLvTable * gHoarstoneLvTableCopy = NULL;
HoarstoneStarTable * gHoarstoneStarTableCopy = NULL;
NewEquipLvTable * gNewEquipLvTableCopy = NULL;
HeroEquipRuleTable * gHeroEquipRuleTableCopy = NULL;
EquipGemRuleTable * gEquipGemRuleTableCopy = NULL;
MaterialTable * gMaterialTableCopy = NULL;
NewGemTable * gNewGemTableCopy = NULL;
NewJewelryTable * gNewJewelryTableCopy = NULL;
ServantBattleOpenTable * gServantBattleOpenTableCopy = NULL;
ServantTable * gServantTableCopy = NULL;
ServantStarTable * gServantStarTableCopy = NULL;
ServantLvUpMaterialTable *gServantMaterialTableCopy = NULL;
ServantLvUpTable * gServantLvUpTableCopy = NULL;
ServantFloorTable * gServantFloorTableCopy = NULL;
ServantTreasureTable * gServantTreasureTableCopy = NULL;
ServantAssistBattleTable * gServantAssistBattleTableCopy = NULL;
ServantLuckTable * gServantLuckTableCopy = NULL;
AttrPowerTable * gAttrPowerTableCopy = NULL;
HeroFashionTable * gHeroFashionTableCopy = NULL;
AchievementTable * gAchievementTableCopy = NULL;
EventTable * gEventTableCopy = NULL;
DailyTaskTable * gDailyTaskTableCopy = NULL;
DailyActiveTable * gDailyActiveTableCopy = NULL;
MonsterAwardTable * gMonsterAwardTableCopy = NULL;
SevenDayTrainningTable * gSevenDayTrainningTableCopy = NULL;

ConsortMsgTable * gConsortMsgTableCopy = NULL;

RankedShopTable * gRankedShopTableCopy = NULL;

RankRewardTable * gRankRewardTableCopy = NULL;
SystemInfoTable * gSystemInfoTableCopy = NULL;

ConsortLvTable * gConsortLvTableCopy = NULL;
ConsortSignTable * gConsortSignTableCopy = NULL;
CatTaskTable * gCatTaskTableCopy = NULL;
KitchenQuestionTable * gKitchenQueTableCopy = NULL;
BusinessCatTable * gBusinessCatTableCopy = NULL;
TicketFriendTable * gTicketFriendTableCopy = NULL;
ConsortWoodTable * gConsortWoodTableCopy = NULL;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool
TableManager::load()
{
	if (!mItemTable.loadFromDbc("item.dbc"))
	{
		LOG_WARN("Failed to load ItemTable table.");
		return false;
	}
	gItemTable = &mItemTable;
	m_mapGlobleFlag.insert("item", 0);

	if(!mAttrEnhanceTable.loadFromDbc("attrenhance.dbc"))
	{
		LOG_WARN("Failed to load AttrenhanceTable table.");
		return false;
	}

	gAttrEnhanceTable = &mAttrEnhanceTable;
	m_mapGlobleFlag.insert("attrenhance", 0);

	if(!mAttrRandomTable.loadFromDbc("attrrandom.dbc"))
	{
		LOG_WARN("Failed to load AttrrandomTable table.");
		return false;
	}

	gAttrRandomTable = &mAttrRandomTable;
	m_mapGlobleFlag.insert("attrrandom", 0);

	if(!mBuffTable.loadFromDbc("buff.dbc"))
	{
		LOG_WARN("Failed to load BuffTable table.");
		return false;

	}

	gBuffTable = &mBuffTable;
	m_mapGlobleFlag.insert("buff", 0);

	if(!mChapterTable.loadFromDbc("chapter.dbc"))
	{
		LOG_WARN("Failed to load ChapterTable table");
		return false;
	}

	gChapterTable = &mChapterTable;
		m_mapGlobleFlag.insert("chapter", 0);

	if(!mEffectTable.loadFromDbc("effect.dbc"))
	{
		LOG_WARN("Failed to load EffectTable table");
		return false;
	}

	gEffectTable = &mEffectTable;
	m_mapGlobleFlag.insert("effect", 0);

	if(!mEquipTable.loadFromDbc("equip.dbc"))
	{
		LOG_WARN("Failed to load EquipTable table");
		return false ;
	}

	gEquipTable = &mEquipTable;
		m_mapGlobleFlag.insert("equip", 0);

	if(!mEquipSetTable.loadFromDbc("equipset.dbc"))
	{
		LOG_WARN("Failed to load equipset table");
		return false;
	}

	gEquipSetTable = &mEquipSetTable;
		m_mapGlobleFlag.insert("equipset", 0);

	if(!mFashionTable.loadFromDbc("fashion.dbc"))
	{
		LOG_WARN("Failed to load FashionTable table");
		return false;
	}

	gFashionTable = &mFashionTable;
		m_mapGlobleFlag.insert("fashion", 0);

	if(!mGemTable.loadFromDbc("gem.dbc"))
	{
		LOG_WARN("Failed to load GemTable table");
		return false;
	}

	gGemTable = &mGemTable;
	m_mapGlobleFlag.insert("gem", 0);

	if(!mHeroTable.loadFromDbc("hero.dbc"))
	{
		LOG_WARN("Failed to load HeroTable table");
		return false;
	}

	gHeroTable = &mHeroTable;
		m_mapGlobleFlag.insert("hero", 0);

	if(!mLevelGrowTable.loadFromDbc("levelgrow.dbc"))
	{
		LOG_WARN("Failed to load levelgrow table");
		return false;
	}

	gLevelGrowTable = &mLevelGrowTable;
	m_mapGlobleFlag.insert("levelgrow", 0);

	if(!mPowerValueTable.loadFromDbc("powervalue.dbc"))
	{
		LOG_WARN("Failed to load powervalue table");
		return false;
	}

	gPowerValueTable = &mPowerValueTable;
	m_mapGlobleFlag.insert("powervalue", 0);

	if(!mPlayerExpTable.loadFromDbc("playerexp.dbc"))
	{
		LOG_WARN("Failed to load playerexp table");
		return false;
	}

	gPlayerExpTable = &mPlayerExpTable;
	m_mapGlobleFlag.insert("playerexp", 0);

	if(!mPalacesStageTable.loadFromDbc("twelvepalacestage.dbc"))
	{
		LOG_WARN("Failed to load twelvepalacestage table");
		return false;
	}

	gPalacesStageTable = &mPalacesStageTable;	
	m_mapGlobleFlag.insert("twelvepalacestage", 0);


	if(!mStageTable.loadFromDbc("stage.dbc"))
	{
		LOG_WARN("Failed to load stage table");
		return false;
	}
	gStageTable = &mStageTable;	
	m_mapGlobleFlag.insert("stage", 0);

	if(!mTypeConditionTable.loadFromDbc("typecondition.dbc"))
	{
		LOG_WARN("Failed to load typecondition table");
		return false;
	}
	gTypeConditionTable = &mTypeConditionTable;	
		m_mapGlobleFlag.insert("typecondition", 0);


	if(!mTowerTypeTable.loadFromDbc("towertype.dbc"))
	{
		LOG_WARN("Failed to load towertype table");
		return false;
	}
	gTowerTypeTable = &mTowerTypeTable;	
	m_mapGlobleFlag.insert("towertype", 0);


	if(!mTowerMonsterTable.loadFromDbc("towermonster.dbc"))
	{
		LOG_WARN("Failed to load towermonster table");
		return false;
	}
	gTowerMonsterTable = &mTowerMonsterTable;	
	m_mapGlobleFlag.insert("towermonster", 0);


	if(!mTowerEventTable.loadFromDbc("towerevent.dbc"))
	{
		LOG_WARN("Failed to load towerevent table");
		return false;
	}
	gTowerEventTable = &mTowerEventTable;
	m_mapGlobleFlag.insert("towerevent", 0);


	if(!mTowerBuffTable.loadFromDbc("towerbuff.dbc"))
	{
		LOG_WARN("Failed to load towerbuff table");
		return false;
	}
	gTowerBuffTable = &mTowerBuffTable;	
	m_mapGlobleFlag.insert("towerbuff", 0);


	if(!mTowerAttrTable.loadFromDbc("towerattr.dbc"))
	{
		LOG_WARN("Failed to load towerattr table");
		return false;
	}
	gTowerAttrTable = &mTowerAttrTable;	
	m_mapGlobleFlag.insert("towerattr", 0);

	if(!mShopsTable.loadFromDbc("mysteryshop.dbc"))
	{
		LOG_WARN("Failed to load mysteryshop table");
		return false;
	}
	gShopsTable = &mShopsTable;	
	m_mapGlobleFlag.insert("mysteryshop", 0);


	if(!mTowerBoxTable.loadFromDbc("towerbox.dbc"))
	{
		LOG_WARN("Failed to load towerbox table");
		return false;
	}
	gTowerBoxTable = &mTowerBoxTable;
	m_mapGlobleFlag.insert("towerbox", 0);

	if(!mTowersTable.loadFromDbc("towers.dbc"))
	{
		LOG_WARN("Failed to load towers table");
		return false;
	}
	gTowersTable = &mTowersTable;	
	m_mapGlobleFlag.insert("towers", 0);

	if(!mRewardLotteryTable.loadFromDbc("rewardlottery.dbc"))
	{
		LOG_WARN("Failed to load rewardlottery table");
		return false;
	}
	gRewardLotteryTable = &mRewardLotteryTable;	
	m_mapGlobleFlag.insert("rewardlottery", 0);


	if(!mLotteryActivityTable.loadFromDbc("lotteryactivity.dbc"))
	{
		LOG_WARN("Failed to load lotteryactivity table");
		return false;
	}
	gLotteryActivityTable = &mLotteryActivityTable;	
	m_mapGlobleFlag.insert("lotteryactivity", 0);


	if(!mCommodityTable.loadFromDbc("commodity.dbc"))
	{
		LOG_WARN("Failed to load commodity table");
		return false;
	}
	gCommodityTable = &mCommodityTable;
	m_mapGlobleFlag.insert("commodity", 0);

	if(!mGiftTable.loadFromDbc("gift.dbc"))
	{
		LOG_WARN("Failed to load gift table");
		return false;
	}
	gGiftTable = &mGiftTable;	
	m_mapGlobleFlag.insert("gift", 0);


	if(!mGiftShopTable.loadFromDbc("giftshop.dbc"))
	{
		LOG_WARN("Failed to load giftshop table");
		return false;
	}
	gGiftShopTable = &mGiftShopTable;
	m_mapGlobleFlag.insert("giftshop", 0);

	if(!mMysticalshopTable.loadFromDbc("mysticalshop.dbc"))
	{
		LOG_WARN("Failed to load mysticalshop table");
		return false;
	}

	gMysticalshopTable = &mMysticalshopTable;
	m_mapGlobleFlag.insert("mysticalshop", 0);

	if(!mServantshopTable.loadFromDbc("servantshop.dbc"))
	{
		LOG_WARN("Failed to load servantshop table");
		return false;
	}
	gServantshopTable = &mServantshopTable;	
	m_mapGlobleFlag.insert("servantshop", 0);

	if(!mAwardContentShopTable.loadFromDbc("awardcontentshop.dbc"))
	{
		LOG_WARN("Failed to load awardcontentshop table");
		return false;
	}
	gAwardContentShopTable = &mAwardContentShopTable;	
	m_mapGlobleFlag.insert("awardcontentshop", 0);


	if(!mCourageshopTable.loadFromDbc("courageshop.dbc"))
	{
		LOG_WARN("Failed to load courageshop table");
		return false;
	}
	gCourageshopTable = &mCourageshopTable;	
	m_mapGlobleFlag.insert("courageshop", 0);

	if(!mCoinshopTable.loadFromDbc("coinshop.dbc"))
	{
		LOG_WARN("Failed to load coinshop table");
		return false;
	}
	gCoinshopTable = &mCoinshopTable;	
	m_mapGlobleFlag.insert("coinshop", 0);



	if(!mGainStrengthTable.loadFromDbc("gainstrength.dbc"))
	{
		LOG_WARN("Failed to load gainstrength table");
		return false;
	}
	gGainStrengthTable = &mGainStrengthTable;
	m_mapGlobleFlag.insert("gainstrength", 0);

	if(!mGainWealthTable.loadFromDbc("gainwealth.dbc"))
	{
		LOG_WARN("Failed to load gainwealth table");
		return false;
	}
	gGainWealthTable = &mGainWealthTable;
	m_mapGlobleFlag.insert("gainwealth", 0);


	if(!mPremiumsTable.loadFromDbc("premiums.dbc"))
	{
		LOG_WARN("Failed to load premiums table");
		return false;
	}
	gPremiumsTable = &mPremiumsTable;	
	m_mapGlobleFlag.insert("premiums", 0);



	if(!mInlineActivityTable.loadFromDbc("insideactivities.dbc"))
	{
		LOG_WARN("Failed to load insideactivities table");
		return false;
	}
	gInlineActivityTable = &mInlineActivityTable;
	m_mapGlobleFlag.insert("insideactivities", 0);

	if(!mCoinTable.loadFromDbc("coin.dbc"))
	{
		LOG_WARN("Failed to load coin table");
		return false;
	}
	gCoinTable = &mCoinTable;
	m_mapGlobleFlag.insert("coin", 0);

	if(!mCoinGroupTable.loadFromDbc("coingroup.dbc"))
	{
		LOG_WARN("Failed to load coingroup table");
		return false;
	}
	gCoinGroupTable = &mCoinGroupTable;	
	m_mapGlobleFlag.insert("coingroup", 0);

	if(!mCourageContentTable.loadFromDbc("couragecontent.dbc"))
	{
		LOG_WARN("Failed to load couragecontent table");
		return false;
	}
	gCourageContentTable = &mCourageContentTable;
		m_mapGlobleFlag.insert("couragecontent", 0);

	if(!mTerritoryTable.loadFromDbc("territory.dbc"))
	{
		LOG_WARN("Failed to load territory table");
		return false;
	}
	gTerritoryTable = &mTerritoryTable;
	m_mapGlobleFlag.insert("territory", 0);

	if(!mRobotTable.loadFromDbc("robot.dbc"))
	{
		LOG_WARN("Failed to load robot table");
		return false;
	}
	gRobotTable = &mRobotTable;	
	m_mapGlobleFlag.insert("robot", 0);


	if(!mAwardAddRateTable.loadFromDbc("awardaddrate.dbc"))
	{
		LOG_WARN("Failed to load awardaddrate table");
		return false;
	}
	gAwardAddRateTable= &mAwardAddRateTable;	
	m_mapGlobleFlag.insert("awardaddrate", 0);

	if(!mCopyFinishTable.loadFromDbc("copyfinishtime.dbc"))
	{
		LOG_WARN("Failed to load copyfinishtime table");
		return false;
	}

	gCopyFinishTable = &mCopyFinishTable;
	m_mapGlobleFlag.insert("copyfinishtime", 0);

	if(!mGlobalVarTable.loadFromDbc("globalvar.dbc"))
	{
		LOG_WARN("Failed to load GlobalVarTable table");
		return false;
	}

	gGlobalVarTable = &mGlobalVarTable;
	m_mapGlobleFlag.insert("globalvar", 0);


	if(!mAttrPolishTable.loadFromDbc("equipattrpolish.dbc"))
	{
		LOG_WARN("Failed to load attrpolish table");
		return false;
	}
	gAttrPolishTable = &mAttrPolishTable;
	m_mapGlobleFlag.insert("equipattrpolish", 0);


	if(!mPolishPointsTable.loadFromDbc("equippolishpoints.dbc"))
	{
		LOG_WARN("Failed to load equippolishpoints table");
		return false;
	}
	gPolishPointsTable = &mPolishPointsTable;
		m_mapGlobleFlag.insert("equippolishpoints", 0);

	if(!mMaterialPointsTable.loadFromDbc("equipmaterialpoints.dbc"))
	{
		LOG_WARN("Failed to load equipmaterialpoints table");
		return false;
	}
	gMaterialPointsTable = &mMaterialPointsTable;
	m_mapGlobleFlag.insert("equipmaterialpoints", 0);

	if(!mAwardTable.loadFromDbc("award.dbc"))
	{
		LOG_WARN("Failed to load AwardTable table");
		return false;
	}

	gAwardTable = &mAwardTable;
	m_mapGlobleFlag.insert("roulette", 0);

	if(!mAwardRuoletteTable.loadFromDbc("roulette.dbc"))
	{
		LOG_WARN("Failed to load gAwardRuoletteTable table");
		return false;
	}

	gAwardRuoletteTable = &mAwardRuoletteTable;
	m_mapGlobleFlag.insert("roulette", 0);

	if(!mAwardCardTable.loadFromDbc("awardcard.dbc"))
	{
		LOG_WARN("Failed to load AwardCardTable table");
		return false;
	}

	gAwardCardTable = &mAwardCardTable;
	m_mapGlobleFlag.insert("awardcard", 0);

	if(!mAwardContentTable.loadFromDbc("awardcontent.dbc"))
	{
		LOG_WARN("Failed to load AwardContentTable table");
		return false;
	}

	gAwardContentTable = &mAwardContentTable;
	m_mapGlobleFlag.insert("awardcontent", 0);

	if(!mAwardContentRuoletteTable.loadFromDbc("roulettecontent.dbc"))
	{
		LOG_WARN("Failed to load mAwardContentRuoletteTable table");
		return false;
	}

	gAwardContentRuoletteTable = &mAwardContentRuoletteTable;
	m_mapGlobleFlag.insert("roulettecontent", 0);

	if(!mLotteryTable.loadFromDbc("lottery.dbc"))
	{
		LOG_WARN("Failed to load lottery table");
		return false;
	}

	gLotteryTable  = &mLotteryTable ;
	m_mapGlobleFlag.insert("lottery", 0);

	if(!mBoxCounterTable.loadFromDbc("boxcounter.dbc"))
	{
		LOG_WARN("Failed to load boxcounter table");
		return false;
	}

	gBoxCounterTable  = &mBoxCounterTable ;
	m_mapGlobleFlag.insert("boxcounter", 0);

	if(!mSkeySvalueTable.loadFromDbc("skeysvalue.dbc"))
	{
		LOG_WARN("Failed to load skeysvalue table");
		return false;
	}

	gSkeySvalueTable  = &mSkeySvalueTable ;
		m_mapGlobleFlag.insert("skeysvalue", 0);

	if(!mGemCombineTable.loadFromDbc("gemcombine.dbc"))
	{
		LOG_WARN("Failed to load gemcombine table");
		return false;
	}

	gGemCombineTable  = &mGemCombineTable ;
		m_mapGlobleFlag.insert("gemcombine", 0);

	if(!mGemCombineOnceTable.loadFromDbc("gemcombineonce.dbc"))
	{
		LOG_WARN("Failed to load gemcombineonce table");
		return false;
	}

	gGemCombineOnceTable  = &mGemCombineOnceTable ;
	m_mapGlobleFlag.insert("gemcombineonce", 0);

	if(!mVipTable.loadFromDbc("vip.dbc"))
	{
		LOG_WARN("Failed to load vip table");
		return false;
	}

	gVipTable  = &mVipTable ;
	m_mapGlobleFlag.insert("vip", 0);

	if(!mWelfareMarketTable.loadFromDbc("welfaremarket.dbc"))
	{
		LOG_WARN("Failed to load WelfareMarket table");
		return false;
	}

	gWelfareMarketTable = &mWelfareMarketTable;
	m_mapGlobleFlag.insert("welfaremarket", 0);

	if(!mRhymeLevelTable.loadFromDbc("rhymelevel.dbc"))
	{
		LOG_WARN("Failed to load rhymelevel table");
		return false;
	}
	gRhymeLevelTable = &mRhymeLevelTable;
	m_mapGlobleFlag.insert("rhymelevel", 0);


	if(!mRhymeAcupointTable.loadFromDbc("rhymeacupoint.dbc"))
	{
		LOG_WARN("Failed to load rhymeacupoint table");
		return false;
	}
	gRhymeAcupointTable = &mRhymeAcupointTable;
	m_mapGlobleFlag.insert("rhymeacupoint", 0);

	if(!mRhymeSkillTable.loadFromDbc("rhymeskill.dbc"))
	{
		LOG_WARN("Failed to load rhymeskill table");
		return false;
	}
	gRhymeSkillTable = &mRhymeSkillTable;
		m_mapGlobleFlag.insert("rhymeskill", 0);

	if(!mSkillLevelTable.loadFromDbc("skilllevel.dbc"))
	{
		LOG_WARN("Failed to load skilllevel table");
		return false;
	}
	gSkillLevelTable = &mSkillLevelTable;
		m_mapGlobleFlag.insert("skilllevel", 0);

	if(!mSkillBaseTable.loadFromDbc("skillbase.dbc"))
	{
		LOG_WARN("Failed to load skillbase table");
		return false;
	}
	gSkillBaseTable = &mSkillBaseTable;
	m_mapGlobleFlag.insert("skillbase", 0);

	if(!mSkillConvertTable.loadFromDbc("skillcompare.dbc"))
	{
		LOG_WARN("Failed to load skillcompare table");
		return false;
	}
	gSkillConvertTable = &mSkillConvertTable;
	m_mapGlobleFlag.insert("skillcompare", 0);

	if(!mEyeQueTable.loadFromDbc("memoryquestionsgroup.dbc"))
	{
		LOG_WARN("Failed to load memoryquestionsgroup table");
		return false;
	}
	gEyeQueTable = &mEyeQueTable;
	m_mapGlobleFlag.insert("memoryquestionsgroup", 0);


	if(!mHoarStoneBaseTable.loadFromDbc("hoarstonebase.dbc"))
	{
		LOG_WARN("Failed to load hoarstonebase table");
		return false;
	}
	gHoarstoneBaseTable = &mHoarStoneBaseTable;
	m_mapGlobleFlag.insert("hoarstonebase", 0);

	if(!mHoarStoneRuneTable.loadFromDbc("hoarstoneequip.dbc"))
	{
		LOG_WARN("Failed to load hoarstoneequip table");
		return false;
	}
	gHoarstoneRuneTable = &mHoarStoneRuneTable;
	m_mapGlobleFlag.insert("hoarstoneequip", 0);

	if(!mHoarStoneLvTable.loadFromDbc("hoarstonelevel.dbc"))
	{
		LOG_WARN("Failed to load hoarstonelevel table");
		return false;
	}

	gHoarstoneLvTable = &mHoarStoneLvTable;
		m_mapGlobleFlag.insert("hoarstonelevel", 0);

	if(!mHoarStoneStarTable.loadFromDbc("hoarstonestar.dbc"))
	{
		LOG_WARN("Failed to load hoarstonestar table");
		return false;
	}

	gHoarstoneStarTable = &mHoarStoneStarTable;
		m_mapGlobleFlag.insert("hoarstonestar", 0);

	if(!mNewEquipLvTable.loadFromDbc("new_equip_level.dbc"))
	{
		LOG_WARN("Failed to load new_equip_level table");
		return false;
	}

	gNewEquipLvTable = &mNewEquipLvTable;
	m_mapGlobleFlag.insert("new_equip_level", 0);

	if(!mHeroEquipRuleTable.loadFromDbc("hero_equip_rule.dbc"))
	{
		LOG_WARN("Failed to load hero_equip_rule table");
		return false;
	}

	gHeroEquipRuleTable = &mHeroEquipRuleTable;
	m_mapGlobleFlag.insert("hero_equip_rule", 0);

	if(!mEquipGemRuleTable.loadFromDbc("equip_gem_rule.dbc"))
	{
		LOG_WARN("Failed to load equip_gem_rule table");
		return false;
	}

	gEquipGemRuleTable = &mEquipGemRuleTable;
		m_mapGlobleFlag.insert("equip_gem_rule", 0);

	if(!mMaterialTable.loadFromDbc("material.dbc"))
	{
		LOG_WARN("Failed to load material table");
		return false;
	}

	gMaterialTable = &mMaterialTable;
	m_mapGlobleFlag.insert("material", 0);


	if(!mNewGemTable.loadFromDbc("new_gem.dbc"))
	{
		LOG_WARN("Failed to load new_gem table");
		return false;
	}

	gNewGemTable = &mNewGemTable;
	m_mapGlobleFlag.insert("new_gem", 0);

	if(!mNewJewelryTable.loadFromDbc("new_jewelry.dbc"))
	{
		LOG_WARN("Failed to load new_jewelry table");
		return false;
	}

	gNewJewelryTable = &mNewJewelryTable;
	m_mapGlobleFlag.insert("new_jewelry", 0);

	if(!mServantBattleOpenTable.loadFromDbc("servantbattleopen.dbc"))
	{
		LOG_WARN("Failed to load servantbattleopen table");
		return false;
	}

	gServantBattleOpenTable = &mServantBattleOpenTable;
	m_mapGlobleFlag.insert("servantbattleopen", 0);


	if(!mServantTable.loadFromDbc("servant.dbc"))
	{
		LOG_WARN("Failed to load servant table");
		return false;
	}

	gServantTable = &mServantTable;
	m_mapGlobleFlag.insert("servant", 0);


	if(!mServantStarTable.loadFromDbc("servantstar.dbc"))
	{
		LOG_WARN("Failed to load servantstar table");
		return false;
	}

	gServantStarTable = &mServantStarTable;
	m_mapGlobleFlag.insert("servantstar", 0);

	if(!mServantMaterialTable.loadFromDbc("servantlvupmaterial.dbc"))
	{
		LOG_WARN("Failed to load servantlvupmaterial table");
		return false;
	}

	gServantMaterialTable = &mServantMaterialTable;
		m_mapGlobleFlag.insert("servantlvupmaterial", 0);

	if(!mServantLvUpTable.loadFromDbc("servantlvup.dbc"))
	{
		LOG_WARN("Failed to load servantlvup table");
		return false;
	}

	gServantLvUpTable = &mServantLvUpTable;
		m_mapGlobleFlag.insert("servantlvup", 0);

	if(!mServantFloorTable.loadFromDbc("servantfloor.dbc"))
	{
		LOG_WARN("Failed to load servantfloor table");
		return false;
	}

	gServantFloorTable = &mServantFloorTable;
		m_mapGlobleFlag.insert("servantfloor", 0);

	if(!mServantTreasureTable.loadFromDbc("servanttreasure.dbc"))
	{
		LOG_WARN("Failed to load servanttreasure table");
		return false;
	}

	gServantTreasureTable = &mServantTreasureTable;
	m_mapGlobleFlag.insert("servanttreasure", 0);

	if(!mServantAssistBattleTable.loadFromDbc("servantassisbattle.dbc"))
	{
		LOG_WARN("Failed to load servantassisbattle table");
		return false;
	}

	gServantAssistBattleTable = &mServantAssistBattleTable;
	m_mapGlobleFlag.insert("servantassisbattle", 0);

	if(!mServantLuckTable.loadFromDbc("servantlucky.dbc"))
	{
		LOG_WARN("Failed to load servantlucky table");
		return false;
	}

	gServantLuckTable = &mServantLuckTable;
	m_mapGlobleFlag.insert("servantlucky", 0);

	if(!mAttrPowerTable.loadFromDbc("attrpower.dbc"))
	{
		LOG_WARN("Failed to load attrpower table");
		return false;
	}

	gAttrPowerTable = &mAttrPowerTable;
	m_mapGlobleFlag.insert("attrpower", 0);

	if(!mHeroFashionTable.loadFromDbc("herofashion.dbc"))
	{
		LOG_WARN("Failed to load herofashion table");
		return false;
	}
	gHeroFashionTable = &mHeroFashionTable;
		m_mapGlobleFlag.insert("herofashion", 0);

	if(!mAchievementTable.loadFromDbc("achievement.dbc"))
	{
		LOG_WARN("Failed to load achievement table");
		return false;
	}
	gAchievementTable = &mAchievementTable;
	m_mapGlobleFlag.insert("achievement", 0);

	if(!mEventTable.loadFromDbc("event.dbc"))
	{
		LOG_WARN("Failed to load event table");
		return false;
	}
	gEventTable = &mEventTable;
	m_mapGlobleFlag.insert("event", 0);
	
	if(!mDailyTaskTable.loadFromDbc("dailytask.dbc"))
	{
		LOG_WARN("Failed to load dailytask table");
		return false;
	}
	gDailyTaskTable = &mDailyTaskTable;
	m_mapGlobleFlag.insert("dailytask", 0);

	if(!mDailyActiveTable.loadFromDbc("active.dbc"))
	{
		LOG_WARN("Failed to load dailyactive table");
		return false;
	}
	gDailyActiveTable = &mDailyActiveTable;
	m_mapGlobleFlag.insert("active", 0);

	if(!mMonsterAwardTable.loadFromDbc("monsteraward.dbc"))
	{
		LOG_WARN("Failed to load monsteraward table");
		return false;
	}
	gMonsterAwardTable = &mMonsterAwardTable;
	m_mapGlobleFlag.insert("monsteraward", 0);


	if(!mSevenDayTrainningTable.loadFromDbc("sevendaystrainning.dbc"))
	{
		LOG_WARN("Failed to load sevendaystrainning table");
		return false;
	}
	gSevenDayTrainningTable = &mSevenDayTrainningTable;
	m_mapGlobleFlag.insert("sevendaystrainning", 0);

	if(!mConsortMsgTable.loadFromDbc("consortmsg.dbc"))
	{
		LOG_WARN("Failed to load consortmsg table");
		return false;
	}
	gConsortMsgTable = &mConsortMsgTable;
	m_mapGlobleFlag.insert("consortmsg", 0);

	if(!mRankedShopTable.loadFromDbc("rankedshop.dbc"))
	{
		LOG_WARN("Failed to load rankedshop table");
		return false;
	}
	gRankedShopTable = &mRankedShopTable;
	m_mapGlobleFlag.insert("rankedshop", 0);


	if(!mRankRewardTable.loadFromDbc("rankreward.dbc"))
	{
		LOG_WARN("Failed to load rankreward table");
		return false;
	}
	gRankRewardTable = &mRankRewardTable;
	m_mapGlobleFlag.insert("rankreward", 0);

	if(!mSystemInfoTable.loadFromDbc("systeminfo.dbc"))
	{
		LOG_WARN("Failed to load systeminfo table");
		return false;
	}
	gSystemInfoTable = &mSystemInfoTable;
	m_mapGlobleFlag.insert("systeminfo", 0);

	if(!mConsortLvTable.loadFromDbc("guildlevel.dbc"))
	{
		LOG_WARN("Failed to load guildlevel table");
		return false;
	}
	gConsortLvTable = &mConsortLvTable;
	m_mapGlobleFlag.insert("guildlevel", 0);

	if(!mConsortSignTable.loadFromDbc("guildsign.dbc"))
	{
		LOG_WARN("Failed to load guildsign table");
		return false;
	}
	gConsortSignTable = &mConsortSignTable;
	m_mapGlobleFlag.insert("guildsign", 0);

	if(!mCatTaskTable.loadFromDbc("cattask.dbc"))
	{
		LOG_WARN("Failed to load cattask table");
		return false;
	}
	gCatTaskTable = &mCatTaskTable;
	m_mapGlobleFlag.insert("cattask", 0);

	if(!mKitchenQueTable.loadFromDbc("kitchenquestions.dbc"))
	{
		LOG_WARN("Failed to load kitchenquestions table");
		return false;
	}
	gKitchenQueTable = &mKitchenQueTable;
	m_mapGlobleFlag.insert("kitchenquestions", 0);

	if(!mBusinessCatTable.loadFromDbc("businesscat.dbc"))
	{
		LOG_WARN("Failed to load businesscat table");
		return false;
	}
	gBusinessCatTable = &mBusinessCatTable;
	m_mapGlobleFlag.insert("businesscat", 0);

	if(!mTicketFriendTable.loadFromDbc("ticketfriend.dbc"))
	{
		LOG_WARN("Failed to load ticketfriend table");
		return false;
	}
	gTicketFriendTable = &mTicketFriendTable;
	m_mapGlobleFlag.insert("ticketfriend", 0);

	if(!mConsortWoodTable.loadFromDbc("woodencat.dbc"))
	{
		LOG_WARN("Failed to load woodencat table");
		return false;
	}
	gConsortWoodTable = &mConsortWoodTable;
	m_mapGlobleFlag.insert("woodencat", 0);



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

	mCourageshopTable.clear();
	gCourageshopTable = NULL;

	mCoinshopTable.clear();
	gCoinshopTable = NULL;


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

	mSkillConvertTable.clear();
	gSkillConvertTable = NULL;

	mEyeQueTable.clear();
	gEyeQueTable = NULL;

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

	mMonsterAwardTable.clear();
	gMonsterAwardTable = NULL;

	mSevenDayTrainningTable.clear();
	gSevenDayTrainningTable = NULL;	
	
	mConsortMsgTable.clear();
	gConsortMsgTable = NULL;

	mRankedShopTable.clear();
	gRankedShopTable = NULL;

	mRankRewardTable.clear();
	gRankRewardTable = NULL;

	mSystemInfoTable.clear();
	gSystemInfoTable = NULL;


	mConsortLvTable.clear();
	gConsortLvTable = NULL;

	mConsortSignTable.clear();
	gConsortSignTable = NULL;

	mCatTaskTable.clear();
	gCatTaskTable = NULL;

	mKitchenQueTable.clear();
	gKitchenQueTable = NULL;

	mBusinessCatTable.clear();
	gBusinessCatTable = NULL;

	mTicketFriendTable.clear();
	gTicketFriendTable = NULL;

	mConsortWoodTable.clear();
	gConsortWoodTable = NULL;

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
	if(fileNameFix == "roulette.dbc")
	{
		if(!mAwardRuoletteTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardRuoletteTable  table");
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

	

	if(fileNameFix == "roulettecontent.dbc")
	{
		if(!mAwardContentRuoletteTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to gAwardContentRuoletteTable  table");
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

	if(fileNameFix == "monsteraward.dbc")
	{
		if(!mMonsterAwardTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload monsteraward table");
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

	if(fileNameFix == "consortmsg.dbc")
	{
		if(!mConsortMsgTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload consortmsg table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rankedshop.dbc")
	{
		if(!mRankedShopTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rankedshop table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rankreward.dbc")
	{
		if(!mRankRewardTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rankreward table");
			return false;
		}
		return true;
	}
	if(fileNameFix == "systeminfo.dbc")
	{
		if(!mSystemInfoTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload systeminfo table");
			return false;
		}
		return true;
	}
	if(fileNameFix == "guildlevel.dbc")
	{
		if(!mConsortLvTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload guildlevel table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "guildsign.dbc")
	{
		if(!mConsortSignTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload guildsign table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "cattask.dbc")
	{
		if(!mCatTaskTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload cattask table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "ticketfriend.dbc")
	{
		if(!mTicketFriendTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload ticketfriend table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "woodencat.dbc")
	{
		if(!mConsortWoodTable.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload woodencat table");
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
	if(!mAwardRuoletteTable.reloadFromDbc("roulette.dbc"))
	{
		LOG_WARN("Failed to relaod mAwardRuoletteTable table");
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

	if(!mAwardContentRuoletteTable.reloadFromDbc("roulettecontent.dbc"))
	{
		LOG_WARN("Failed to relaod mAwardContentRuoletteTable table");
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

	if(!mAwardContentRuoletteTable.reloadFromDbc("roulettecontent.dbc"))
	{
		LOG_WARN("Failed to mAwardContentRuoletteTable  table");
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
	if(!mCourageshopTable.reloadFromDbc("courageshop.dbc"))
	{
		LOG_WARN("Failed to reload courageshop table");
		return false;
	}
	if(!mCoinshopTable.reloadFromDbc("coinshop.dbc"))
	{
		LOG_WARN("Failed to reload coinshop table");
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

	if(!mMonsterAwardTable.reloadFromDbc("monsteraward.dbc"))
	{
		LOG_WARN("Failed to reload monsteraward table");
		return false;
	}

	if(!mSevenDayTrainningTable.reloadFromDbc("sevendaystrainning.dbc"))
	{
		LOG_WARN("Failed to reload sevendaystrainning table");
		return false;
	}
	
	if(!mConsortMsgTable.reloadFromDbc("consortmsg.dbc"))
	{
		LOG_WARN("Failed to reload consortmsg table");
		return false;
	}

	if(!mRankedShopTable.reloadFromDbc("rankedshop.dbc"))
	{
		LOG_WARN("Failed to reload rankedshop table");
		return false;
	}

	if(!mRankRewardTable.reloadFromDbc("rankreward.dbc"))
	{
		LOG_WARN("Failed to reload rankreward table");
		return false;
	}
	if(!mSystemInfoTable.reloadFromDbc("systeminfo.dbc"))
	{
		LOG_WARN("Failed to reload systeminfo table");
		return false;
	}

	if(!mConsortLvTable.reloadFromDbc("guildlevel.dbc"))
	{
		LOG_WARN("Failed to reload guildlevel table");
		return false;
	}

	if(!mConsortSignTable.reloadFromDbc("guildsign.dbc"))
	{
		LOG_WARN("Failed to reload guildsign table");
		return false;
	}

	if(!mCatTaskTable.reloadFromDbc("cattask.dbc"))
	{
		LOG_WARN("Failed to reload cattask table");
		return false;
	}

	if(!mTicketFriendTable.reloadFromDbc("ticketfriend.dbc"))
	{
		LOG_WARN("Failed to reload ticketfriend table");
		return false;
	}

	if(!mConsortWoodTable.reloadFromDbc("woodencat.dbc"))
	{
		LOG_WARN("Failed to reload woodencat table");
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//copy版本的tablemanager

bool
TableManagerCopy::load()
{
	if (!mItemTableCopy.loadFromDbc("item.dbc"))
	{
		LOG_WARN("Failed to load ItemTable table.");
		return false;
	}
	gItemTableCopy = &mItemTableCopy;

	if(!mAttrEnhanceTableCopy.loadFromDbc("attrenhance.dbc"))
	{
		LOG_WARN("Failed to load AttrenhanceTable table.");
		return false;
	}

	gAttrEnhanceTableCopy = &mAttrEnhanceTableCopy;


	if(!mAttrRandomTableCopy.loadFromDbc("attrrandom.dbc"))
	{
		LOG_WARN("Failed to load AttrrandomTable table.");
		return false;
	}

	gAttrRandomTableCopy = &mAttrRandomTableCopy;


	if(!mBuffTableCopy.loadFromDbc("buff.dbc"))
	{
		LOG_WARN("Failed to load BuffTable table.");
		return false;

	}

	gBuffTableCopy = &mBuffTableCopy;

	if(!mChapterTableCopy.loadFromDbc("chapter.dbc"))
	{
		LOG_WARN("Failed to load ChapterTable table");
		return false;
	}

	gChapterTableCopy = &mChapterTableCopy;

	if(!mEffectTableCopy.loadFromDbc("effect.dbc"))
	{
		LOG_WARN("Failed to load EffectTable table");
		return false;
	}

	gEffectTableCopy = &mEffectTableCopy;

	if(!mEquipTableCopy.loadFromDbc("equip.dbc"))
	{
		LOG_WARN("Failed to load EquipTable table");
		return false ;
	}

	gEquipTableCopy = &mEquipTableCopy;

	if(!mEquipSetTableCopy.loadFromDbc("equipset.dbc"))
	{
		LOG_WARN("Failed to load equipset table");
		return false;
	}

	gEquipSetTableCopy = &mEquipSetTableCopy;

	if(!mFashionTableCopy.loadFromDbc("fashion.dbc"))
	{
		LOG_WARN("Failed to load FashionTable table");
		return false;
	}

	gFashionTableCopy = &mFashionTableCopy;

	if(!mGemTableCopy.loadFromDbc("gem.dbc"))
	{
		LOG_WARN("Failed to load GemTable table");
		return false;
	}

	gGemTableCopy = &mGemTableCopy;

	if(!mHeroTableCopy.loadFromDbc("hero.dbc"))
	{
		LOG_WARN("Failed to load HeroTable table");
		return false;
	}

	gHeroTableCopy = &mHeroTableCopy;

	if(!mLevelGrowTableCopy.loadFromDbc("levelgrow.dbc"))
	{
		LOG_WARN("Failed to load levelgrow table");
		return false;
	}

	gLevelGrowTableCopy = &mLevelGrowTableCopy;


	if(!mPowerValueTableCopy.loadFromDbc("powervalue.dbc"))
	{
		LOG_WARN("Failed to load powervalue table");
		return false;
	}

	gPowerValueTableCopy = &mPowerValueTableCopy;

	if(!mPlayerExpTableCopy.loadFromDbc("playerexp.dbc"))
	{
		LOG_WARN("Failed to load playerexp table");
		return false;
	}

	gPlayerExpTableCopy = &mPlayerExpTableCopy;

	if(!mPalacesStageTableCopy.loadFromDbc("twelvepalacestage.dbc"))
	{
		LOG_WARN("Failed to load twelvepalacestage table");
		return false;
	}
	gPalacesStageTableCopy = &mPalacesStageTableCopy;	

	if(!mStageTableCopy.loadFromDbc("stage.dbc"))
	{
		LOG_WARN("Failed to load stage table");
		return false;
	}
	gStageTableCopy = &mStageTableCopy;	


	if(!mTypeConditionTableCopy.loadFromDbc("typecondition.dbc"))
	{
		LOG_WARN("Failed to load typecondition table");
		return false;
	}
	gTypeConditionTableCopy = &mTypeConditionTableCopy;	

	if(!mTowerTypeTableCopy.loadFromDbc("towertype.dbc"))
	{
		LOG_WARN("Failed to load towertype table");
		return false;
	}
	gTowerTypeTableCopy = &mTowerTypeTableCopy;	

	if(!mTowerMonsterTableCopy.loadFromDbc("towermonster.dbc"))
	{
		LOG_WARN("Failed to load towermonster table");
		return false;
	}
	gTowerMonsterTableCopy = &mTowerMonsterTableCopy;	


	if(!mTowerEventTableCopy.loadFromDbc("towerevent.dbc"))
	{
		LOG_WARN("Failed to load towerevent table");
		return false;
	}
	gTowerEventTableCopy = &mTowerEventTableCopy;	

	if(!mTowerBuffTableCopy.loadFromDbc("towerbuff.dbc"))
	{
		LOG_WARN("Failed to load towerbuff table");
		return false;
	}
	gTowerBuffTableCopy = &mTowerBuffTableCopy;	

	if(!mTowerAttrTableCopy.loadFromDbc("towerattr.dbc"))
	{
		LOG_WARN("Failed to load towerattr table");
		return false;
	}
	gTowerAttrTableCopy = &mTowerAttrTableCopy;	

	if(!mShopsTableCopy.loadFromDbc("mysteryshop.dbc"))
	{
		LOG_WARN("Failed to load mysteryshop table");
		return false;
	}
	gShopsTableCopy = &mShopsTableCopy;	

	if(!mTowerBoxTableCopy.loadFromDbc("towerbox.dbc"))
	{
		LOG_WARN("Failed to load towerbox table");
		return false;
	}
	gTowerBoxTableCopy = &mTowerBoxTableCopy;	

	if(!mTowersTableCopy.loadFromDbc("towers.dbc"))
	{
		LOG_WARN("Failed to load towers table");
		return false;
	}
	gTowersTableCopy = &mTowersTableCopy;	


	if(!mRewardLotteryTableCopy.loadFromDbc("rewardlottery.dbc"))
	{
		LOG_WARN("Failed to load rewardlottery table");
		return false;
	}
	gRewardLotteryTableCopy = &mRewardLotteryTableCopy;	

	if(!mLotteryActivityTableCopy.loadFromDbc("lotteryactivity.dbc"))
	{
		LOG_WARN("Failed to load lotteryactivity table");
		return false;
	}
	gLotteryActivityTableCopy = &mLotteryActivityTableCopy;	


	if(!mCommodityTableCopy.loadFromDbc("commodity.dbc"))
	{
		LOG_WARN("Failed to load commodity table");
		return false;
	}
	gCommodityTableCopy = &mCommodityTableCopy;	

	if(!mGiftTableCopy.loadFromDbc("gift.dbc"))
	{
		LOG_WARN("Failed to load gift table");
		return false;
	}
	gGiftTableCopy = &mGiftTableCopy;	

	if(!mGiftShopTableCopy.loadFromDbc("giftshop.dbc"))
	{
		LOG_WARN("Failed to load giftshop table");
		return false;
	}
	gGiftShopTableCopy = &mGiftShopTableCopy;	

	if(!mMysticalshopTableCopy.loadFromDbc("mysticalshop.dbc"))
	{
		LOG_WARN("Failed to load mysticalshop table");
		return false;
	}

	gMysticalshopTableCopy = &mMysticalshopTableCopy;

	if(!mServantshopTableCopy.loadFromDbc("servantshop.dbc"))
	{
		LOG_WARN("Failed to load servantshop table");
		return false;
	}
	gServantshopTableCopy = &mServantshopTableCopy;	


	if(!mCourageshopTableCopy.loadFromDbc("courageshop.dbc"))
	{
		LOG_WARN("Failed to load servantshop table");
		return false;
	}
	gCourageshopTableCopy = &mCourageshopTableCopy;	

	if(!mCoinshopTableCopy.loadFromDbc("coinshop.dbc"))
	{
		LOG_WARN("Failed to load coinshop table");
		return false;
	}
	gCoinshopTableCopy = &mCoinshopTableCopy;	

	if(!mAwardContentShopTableCopy.loadFromDbc("awardcontentshop.dbc"))
	{
		LOG_WARN("Failed to load awardcontentshop table");
		return false;
	}
	gAwardContentShopTableCopy = &mAwardContentShopTableCopy;	



	if(!mGainStrengthTableCopy.loadFromDbc("gainstrength.dbc"))
	{
		LOG_WARN("Failed to load gainstrength table");
		return false;
	}
	gGainStrengthTableCopy = &mGainStrengthTableCopy;	

	if(!mGainWealthTableCopy.loadFromDbc("gainwealth.dbc"))
	{
		LOG_WARN("Failed to load gainwealth table");
		return false;
	}
	gGainWealthTableCopy = &mGainWealthTableCopy;	

	if(!mPremiumsTableCopy.loadFromDbc("premiums.dbc"))
	{
		LOG_WARN("Failed to load premiums table");
		return false;
	}
	gPremiumsTableCopy = &mPremiumsTableCopy;	




	if(!mInlineActivityTableCopy.loadFromDbc("insideactivities.dbc"))
	{
		LOG_WARN("Failed to load insideactivities table");
		return false;
	}
	gInlineActivityTableCopy = &mInlineActivityTableCopy;	

	if(!mCoinTableCopy.loadFromDbc("coin.dbc"))
	{
		LOG_WARN("Failed to load coin table");
		return false;
	}
	gCoinTableCopy = &mCoinTableCopy;	

	if(!mCoinGroupTableCopy.loadFromDbc("coingroup.dbc"))
	{
		LOG_WARN("Failed to load coingroup table");
		return false;
	}
	gCoinGroupTableCopy = &mCoinGroupTableCopy;	

	if(!mCourageContentTableCopy.loadFromDbc("couragecontent.dbc"))
	{
		LOG_WARN("Failed to load couragecontent table");
		return false;
	}
	gCourageContentTableCopy = &mCourageContentTableCopy;	

	if(!mTerritoryTableCopy.loadFromDbc("territory.dbc"))
	{
		LOG_WARN("Failed to load territory table");
		return false;
	}
	gTerritoryTableCopy = &mTerritoryTableCopy;	

	if(!mRobotTableCopy.loadFromDbc("robot.dbc"))
	{
		LOG_WARN("Failed to load robot table");
		return false;
	}
	gRobotTableCopy = &mRobotTableCopy;	



	if(!mAwardAddRateTableCopy.loadFromDbc("awardaddrate.dbc"))
	{
		LOG_WARN("Failed to load awardaddrate table");
		return false;
	}
	gAwardAddRateTableCopy= &mAwardAddRateTableCopy;	


	if(!mCopyFinishTableCopy.loadFromDbc("copyfinishtime.dbc"))
	{
		LOG_WARN("Failed to load copyfinishtime table");
		return false;
	}

	gCopyFinishTableCopy = &mCopyFinishTableCopy;

	if(!mGlobalVarTableCopy.loadFromDbc("globalvar.dbc"))
	{
		LOG_WARN("Failed to load GlobalVarTable table");
		return false;
	}

	gGlobalVarTableCopy = &mGlobalVarTableCopy;
	if(!mAttrPolishTableCopy.loadFromDbc("equipattrpolish.dbc"))
	{
		LOG_WARN("Failed to load attrpolish table");
		return false;
	}
	gAttrPolishTableCopy = &mAttrPolishTableCopy;

	if(!mPolishPointsTableCopy.loadFromDbc("equippolishpoints.dbc"))
	{
		LOG_WARN("Failed to load equippolishpoints table");
		return false;
	}
	gPolishPointsTableCopy = &mPolishPointsTableCopy;

	if(!mMaterialPointsTableCopy.loadFromDbc("equipmaterialpoints.dbc"))
	{
		LOG_WARN("Failed to load equipmaterialpoints table");
		return false;
	}
	gMaterialPointsTableCopy = &mMaterialPointsTableCopy;

	if(!mAwardTableCopy.loadFromDbc("award.dbc"))
	{
		LOG_WARN("Failed to load AwardTable table");
		return false;
	}

	gAwardTableCopy = &mAwardTableCopy;

	if(!mAwardRuoletteTableCopy.loadFromDbc("roulette.dbc"))
	{
		LOG_WARN("Failed to load gAwardRuoletteTable table");
		return false;
	}

	gAwardRuoletteTableCopy = &mAwardRuoletteTableCopy;

	if(!mAwardCardTableCopy.loadFromDbc("awardcard.dbc"))
	{
		LOG_WARN("Failed to load AwardCardTable table");
		return false;
	}

	gAwardCardTableCopy = &mAwardCardTableCopy;

	if(!mAwardContentTableCopy.loadFromDbc("awardcontent.dbc"))
	{
		LOG_WARN("Failed to load AwardContentTable table");
		return false;
	}

	gAwardContentTableCopy = &mAwardContentTableCopy;

	if(!mAwardContentRuoletteTableCopy.loadFromDbc("roulettecontent.dbc"))
	{
		LOG_WARN("Failed to load mAwardContentRuoletteTable table");
		return false;
	}


	gAwardContentRuoletteTableCopy = &mAwardContentRuoletteTableCopy;

	if(!mLotteryTableCopy.loadFromDbc("lottery.dbc"))
	{
		LOG_WARN("Failed to load lottery table");
		return false;
	}

	gLotteryTableCopy = &mLotteryTableCopy ;

	if(!mBoxCounterTableCopy.loadFromDbc("boxcounter.dbc"))
	{
		LOG_WARN("Failed to load boxcounter table");
		return false;
	}

	gBoxCounterTableCopy  = &mBoxCounterTableCopy ;

	if(!mSkeySvalueTableCopy.loadFromDbc("skeysvalue.dbc"))
	{
		LOG_WARN("Failed to load skeysvalue table");
		return false;
	}

	gSkeySvalueTableCopy  = &mSkeySvalueTableCopy ;

	if(!mGemCombineTableCopy.loadFromDbc("gemcombine.dbc"))
	{
		LOG_WARN("Failed to load gemcombine table");
		return false;
	}

	gGemCombineTableCopy  = &mGemCombineTableCopy ;

	if(!mGemCombineOnceTableCopy.loadFromDbc("gemcombineonce.dbc"))
	{
		LOG_WARN("Failed to load gemcombineonce table");
		return false;
	}

	gGemCombineOnceTableCopy  = &mGemCombineOnceTableCopy ;

	if(!mVipTableCopy.loadFromDbc("vip.dbc"))
	{
		LOG_WARN("Failed to load vip table");
		return false;
	}

	gVipTableCopy  = &mVipTableCopy ;

	if(!mWelfareMarketTableCopy.loadFromDbc("welfaremarket.dbc"))
	{
		LOG_WARN("Failed to load WelfareMarket table");
		return false;
	}

	gWelfareMarketTableCopy = &mWelfareMarketTableCopy;

	if(!mRhymeLevelTableCopy.loadFromDbc("rhymelevel.dbc"))
	{
		LOG_WARN("Failed to load rhymelevel table");
		return false;
	}
	gRhymeLevelTableCopy = &mRhymeLevelTableCopy;

	if(!mRhymeAcupointTableCopy.loadFromDbc("rhymeacupoint.dbc"))
	{
		LOG_WARN("Failed to load rhymeacupoint table");
		return false;
	}
	gRhymeAcupointTableCopy = &mRhymeAcupointTableCopy;

	if(!mRhymeSkillTableCopy.loadFromDbc("rhymeskill.dbc"))
	{
		LOG_WARN("Failed to load rhymeskill table");
		return false;
	}
	gRhymeSkillTableCopy = &mRhymeSkillTableCopy;

	if(!mSkillLevelTableCopy.loadFromDbc("skilllevel.dbc"))
	{
		LOG_WARN("Failed to load skilllevel table");
		return false;
	}
	gSkillLevelTableCopy = &mSkillLevelTableCopy;

	if(!mSkillBaseTableCopy.loadFromDbc("skillbase.dbc"))
	{
		LOG_WARN("Failed to load skillbase table");
		return false;
	}
	gSkillBaseTableCopy = &mSkillBaseTableCopy;


	if(!mSkillConvertTableCopy.loadFromDbc("skillcompare.dbc"))
	{
		LOG_WARN("Failed to load skillcompare table");
		return false;
	}
	gSkillConvertTableCopy = &mSkillConvertTableCopy;

	if(!mEyeQueTableCopy.loadFromDbc("memoryquestionsgroup.dbc"))
	{
		LOG_WARN("Failed to load memoryquestionsgroup table");
		return false;
	}
	gEyeQueTableCopy = &mEyeQueTableCopy;



	if(!mHoarStoneBaseTableCopy.loadFromDbc("hoarstonebase.dbc"))
	{
		LOG_WARN("Failed to load hoarstonebase table");
		return false;
	}
	gHoarstoneBaseTableCopy = &mHoarStoneBaseTableCopy;

	if(!mHoarStoneRuneTableCopy.loadFromDbc("hoarstoneequip.dbc"))
	{
		LOG_WARN("Failed to load hoarstoneequip table");
		return false;
	}
	gHoarstoneRuneTableCopy = &mHoarStoneRuneTableCopy;

	if(!mHoarStoneLvTableCopy.loadFromDbc("hoarstonelevel.dbc"))
	{
		LOG_WARN("Failed to load hoarstonelevel table");
		return false;
	}

	gHoarstoneLvTableCopy = &mHoarStoneLvTableCopy;

	if(!mHoarStoneStarTableCopy.loadFromDbc("hoarstonestar.dbc"))
	{
		LOG_WARN("Failed to load hoarstonestar table");
		return false;
	}

	gHoarstoneStarTableCopy = &mHoarStoneStarTableCopy;

	if(!mNewEquipLvTableCopy.loadFromDbc("new_equip_level.dbc"))
	{
		LOG_WARN("Failed to load new_equip_level table");
		return false;
	}

	gNewEquipLvTableCopy = &mNewEquipLvTableCopy;

	if(!mHeroEquipRuleTableCopy.loadFromDbc("hero_equip_rule.dbc"))
	{
		LOG_WARN("Failed to load hero_equip_rule table");
		return false;
	}

	gHeroEquipRuleTableCopy = &mHeroEquipRuleTableCopy;

	if(!mEquipGemRuleTableCopy.loadFromDbc("equip_gem_rule.dbc"))
	{
		LOG_WARN("Failed to load equip_gem_rule table");
		return false;
	}

	gEquipGemRuleTableCopy = &mEquipGemRuleTableCopy;

	if(!mMaterialTableCopy.loadFromDbc("material.dbc"))
	{
		LOG_WARN("Failed to load material table");
		return false;
	}

	gMaterialTableCopy = &mMaterialTableCopy;


	if(!mNewGemTableCopy.loadFromDbc("new_gem.dbc"))
	{
		LOG_WARN("Failed to load new_gem table");
		return false;
	}

	gNewGemTableCopy = &mNewGemTableCopy;

	if(!mNewJewelryTableCopy.loadFromDbc("new_jewelry.dbc"))
	{
		LOG_WARN("Failed to load new_jewelry table");
		return false;
	}

	gNewJewelryTableCopy = &mNewJewelryTableCopy;

	if(!mServantBattleOpenTableCopy.loadFromDbc("servantbattleopen.dbc"))
	{
		LOG_WARN("Failed to load servantbattleopen table");
		return false;
	}

	gServantBattleOpenTableCopy = &mServantBattleOpenTableCopy;


	if(!mServantTableCopy.loadFromDbc("servant.dbc"))
	{
		LOG_WARN("Failed to load servant table");
		return false;
	}

	gServantTableCopy = &mServantTableCopy;


	if(!mServantStarTableCopy.loadFromDbc("servantstar.dbc"))
	{
		LOG_WARN("Failed to load servantstar table");
		return false;
	}

	gServantStarTableCopy = &mServantStarTableCopy;

	if(!mServantMaterialTableCopy.loadFromDbc("servantlvupmaterial.dbc"))
	{
		LOG_WARN("Failed to load servantlvupmaterial table");
		return false;
	}

	gServantMaterialTableCopy = &mServantMaterialTableCopy;

	if(!mServantLvUpTableCopy.loadFromDbc("servantlvup.dbc"))
	{
		LOG_WARN("Failed to load servantlvup table");
		return false;
	}

	gServantLvUpTableCopy = &mServantLvUpTableCopy;

	if(!mServantFloorTableCopy.loadFromDbc("servantfloor.dbc"))
	{
		LOG_WARN("Failed to load servantfloor table");
		return false;
	}

	gServantFloorTableCopy = &mServantFloorTableCopy;

	if(!mServantTreasureTableCopy.loadFromDbc("servanttreasure.dbc"))
	{
		LOG_WARN("Failed to load servanttreasure table");
		return false;
	}

	gServantTreasureTableCopy = &mServantTreasureTableCopy;

	if(!mServantAssistBattleTableCopy.loadFromDbc("servantassisbattle.dbc"))
	{
		LOG_WARN("Failed to load servantassisbattle table");
		return false;
	}

	gServantAssistBattleTableCopy = &mServantAssistBattleTableCopy;

	if(!mServantLuckTableCopy.loadFromDbc("servantlucky.dbc"))
	{
		LOG_WARN("Failed to load servantlucky table");
		return false;
	}

	gServantLuckTableCopy = &mServantLuckTableCopy;

	if(!mAttrPowerTableCopy.loadFromDbc("attrpower.dbc"))
	{
		LOG_WARN("Failed to load attrpower table");
		return false;
	}

	gAttrPowerTableCopy = &mAttrPowerTableCopy;

	if(!mHeroFashionTableCopy.loadFromDbc("herofashion.dbc"))
	{
		LOG_WARN("Failed to load herofashion table");
		return false;
	}
	gHeroFashionTableCopy = &mHeroFashionTableCopy;

	if(!mAchievementTableCopy.loadFromDbc("achievement.dbc"))
	{
		LOG_WARN("Failed to load achievement table");
		return false;
	}
	gAchievementTableCopy = &mAchievementTableCopy;

	if(!mEventTableCopy.loadFromDbc("event.dbc"))
	{
		LOG_WARN("Failed to load event table");
		return false;
	}
	gEventTableCopy = &mEventTableCopy;

	if(!mDailyTaskTableCopy.loadFromDbc("dailytask.dbc"))
	{
		LOG_WARN("Failed to load dailytask table");
		return false;
	}

	gDailyTaskTableCopy = &mDailyTaskTableCopy;

	if(!mDailyActiveTableCopy.loadFromDbc("active.dbc"))
	{
		LOG_WARN("Failed to load dailyactive table");
		return false;
	}
	gDailyActiveTableCopy = &mDailyActiveTableCopy;

	if(!mMonsterAwardTableCopy.loadFromDbc("monsteraward.dbc"))
	{
		LOG_WARN("Failed to load monsteraward table");
		return false;
	}
	gMonsterAwardTableCopy = &mMonsterAwardTableCopy;

	if(!mSevenDayTrainningTableCopy.loadFromDbc("sevendaystrainning.dbc"))
	{
		LOG_WARN("Failed to load dailytask table");
		return false;
	}
	gSevenDayTrainningTableCopy = &mSevenDayTrainningTableCopy;


	if(!mConsortMsgTableCopy.loadFromDbc("consortmsg.dbc"))
	{
		LOG_WARN("Failed to load consortmsg table");
		return false;
	}
	gConsortMsgTableCopy = &mConsortMsgTableCopy;


	if(!mRankedShopTableCopy.loadFromDbc("rankedshop.dbc"))
	{
		LOG_WARN("Failed to load rankedshop table");
		return false;
	}
	gRankedShopTableCopy = &mRankedShopTableCopy;


	if(!mRankRewardTableCopy.loadFromDbc("rankreward.dbc"))
	{
		LOG_WARN("Failed to load rankreward table");
		return false;
	}
	gRankRewardTableCopy = &mRankRewardTableCopy;


	if(!mSystemInfoTableCopy.loadFromDbc("systeminfo.dbc"))
	{
		LOG_WARN("Failed to load systeminfo table");
		return false;
	}
	gSystemInfoTableCopy = &mSystemInfoTableCopy;

	if(!mConsortLvTableCopy.loadFromDbc("guildlevel.dbc"))
	{
		LOG_WARN("Failed to load guildlevel table");
		return false;
	}
	gConsortLvTableCopy = &mConsortLvTableCopy;

	if(!mConsortSignTableCopy.loadFromDbc("guildsign.dbc"))
	{
		LOG_WARN("Failed to load guildsign table");
		return false;
	}
	gConsortSignTableCopy = &mConsortSignTableCopy;

	if(!mCatTaskTableCopy.loadFromDbc("cattask.dbc"))
	{
		LOG_WARN("Failed to load cattask table");
		return false;
	}
	gCatTaskTableCopy = &mCatTaskTableCopy;

	if(!mKitchenQueTableCopy.loadFromDbc("kitchenquestions.dbc"))
	{
		LOG_WARN("Failed to load kitchenquestions table");
		return false;
	}
	gKitchenQueTableCopy = &mKitchenQueTableCopy;

	if(!mBusinessCatTableCopy.loadFromDbc("businesscat.dbc"))
	{
		LOG_WARN("Failed to load businesscat table");
		return false;
	}
	gBusinessCatTableCopy = &mBusinessCatTableCopy;

	if(!mTicketFriendTableCopy.loadFromDbc("ticketfriend.dbc"))
	{
		LOG_WARN("Failed to load ticketfriend table");
		return false;
	}
	gTicketFriendTableCopy = &mTicketFriendTableCopy;

	if(!mConsortWoodTableCopy.loadFromDbc("woodencat.dbc"))
	{
		LOG_WARN("Failed to load woodencat table");
		return false;
	}
	gConsortWoodTableCopy = &mConsortWoodTableCopy;


	return true;
}

void
TableManagerCopy::unload()
{

	mItemTableCopy.clear();
	gItemTableCopy = NULL;

	mAttrEnhanceTableCopy.clear();
	gAttrEnhanceTableCopy = NULL;

	mAttrRandomTableCopy.clear();
	gAttrRandomTableCopy = NULL;

	mBuffTableCopy.clear();
	gBuffTableCopy = NULL;

	mChapterTableCopy.clear();
	gChapterTableCopy = NULL;

	mEffectTableCopy.clear();
	gEffectTableCopy = NULL;

	mEquipTableCopy.clear();
	gEquipTableCopy = NULL;

	mEquipSetTableCopy.clear();
	gEquipSetTableCopy = NULL;

	mFashionTableCopy.clear();
	gFashionTableCopy = NULL;

	mGemTableCopy.clear();
	gGemTableCopy= NULL;

	mHeroTableCopy.clear();
	gHeroTableCopy = NULL;

	mLevelGrowTableCopy.clear();
	gLevelGrowTableCopy = NULL;

	mPowerValueTableCopy.clear();
	gPowerValueTableCopy = NULL;

	mPlayerExpTableCopy.clear();
	gPlayerExpTableCopy = NULL;

	mStageTableCopy.clear();
	gStageTableCopy = NULL;

	mPalacesStageTableCopy.clear();
	gPalacesStageTableCopy = NULL;

	mTypeConditionTableCopy.clear();
	gTypeConditionTableCopy = NULL;

	mTowerTypeTableCopy.clear();
	gTowerTypeTableCopy = NULL;

	mTowerMonsterTableCopy.clear();
	gTowerMonsterTableCopy = NULL;

	mTowerEventTableCopy.clear();
	gTowerEventTableCopy = NULL;

	mTowerBuffTableCopy.clear();
	gTowerBuffTableCopy = NULL;

	mTowerAttrTableCopy.clear();
	gTowerAttrTableCopy = NULL;

	mShopsTableCopy.clear();
	gShopsTableCopy = NULL;

	mTowerBoxTableCopy.clear();
	gTowerBoxTableCopy = NULL;

	mTowersTableCopy.clear();
	gTowersTableCopy = NULL;

	mRewardLotteryTableCopy.clear();
	gRewardLotteryTableCopy = NULL;

	mLotteryActivityTableCopy.clear();
	gLotteryActivityTableCopy = NULL;



	mCommodityTableCopy.clear();
	gCommodityTableCopy = NULL;

	mGiftTableCopy.clear();
	gGiftTableCopy = NULL;


	mGiftShopTableCopy.clear();
	gGiftShopTableCopy = NULL;


	mMysticalshopTableCopy.clear();
	gMysticalshopTableCopy = NULL;


	mServantshopTableCopy.clear();
	gServantshopTableCopy = NULL;

	mCourageshopTableCopy.clear();
	gCourageshopTableCopy = NULL;

	mCoinshopTableCopy.clear();
	gCoinshopTableCopy = NULL;

	mAwardContentShopTableCopy.clear();
	gAwardContentShopTableCopy = NULL;


	mGainStrengthTableCopy.clear();
	gGainStrengthTableCopy = NULL;

	mGainWealthTableCopy.clear();
	gGainWealthTableCopy = NULL;

	mPremiumsTableCopy.clear();
	gPremiumsTableCopy = NULL;


	mInlineActivityTableCopy.clear();
	gInlineActivityTableCopy = NULL;

	mCoinTableCopy.clear();
	gCoinTableCopy = NULL;

	mCoinGroupTableCopy.clear();
	gCoinGroupTableCopy = NULL;


	mCourageContentTableCopy.clear();
	gCourageContentTableCopy = NULL;


	mTerritoryTableCopy.clear();
	gTerritoryTableCopy = NULL;

	mRobotTableCopy.clear();
	gRobotTableCopy = NULL;

	mAwardAddRateTableCopy.clear();
	gAwardAddRateTableCopy = NULL;

	mCopyFinishTableCopy.clear();
	gCopyFinishTableCopy = NULL;

	mGlobalVarTableCopy.clear();
	gGlobalVarTableCopy = NULL;

	mAwardTableCopy.clear();
	gAwardTableCopy = NULL;

	mAwardRuoletteTableCopy.clear();
	gAwardRuoletteTableCopy = NULL;


	mAttrPolishTableCopy.clear();
	gAttrPolishTableCopy = NULL;


	mPolishPointsTableCopy.clear();
	gPolishPointsTableCopy = NULL;

	mMaterialPointsTableCopy.clear();
	gMaterialPointsTableCopy = NULL;

	mAwardCardTableCopy.clear();
	gAwardCardTableCopy = NULL;

	mAwardContentTableCopy.clear();
	gAwardContentTableCopy = NULL;

	mAwardContentRuoletteTableCopy.clear();
	gAwardContentRuoletteTableCopy = NULL;

	mLotteryTableCopy .clear();
	gLotteryTableCopy  = NULL;

	mBoxCounterTableCopy .clear();
	gBoxCounterTableCopy  = NULL;

	mSkeySvalueTableCopy.clear();
	gSkeySvalueTableCopy = NULL;

	mGemCombineTableCopy.clear();
	gGemCombineTableCopy = NULL;

	mGemCombineOnceTableCopy.clear();
	gGemCombineOnceTableCopy = NULL;

	mVipTableCopy.clear();
	gVipTableCopy = NULL;

	mWelfareMarketTableCopy.clear();
	gWelfareMarketTableCopy = NULL;

	mRhymeLevelTableCopy.clear();
	gRhymeLevelTableCopy = NULL;

	mRhymeAcupointTableCopy.clear();
	gRhymeAcupointTableCopy = NULL;

	mRhymeSkillTableCopy.clear();
	gRhymeSkillTableCopy = NULL;

	mSkillLevelTableCopy.clear();
	gSkillLevelTableCopy = NULL;

	mSkillBaseTableCopy.clear();
	gSkillBaseTableCopy = NULL;

	mSkillConvertTableCopy.clear();
	gSkillConvertTableCopy = NULL;

	mEyeQueTableCopy.clear();
	gEyeQueTableCopy = NULL;

	mHoarStoneBaseTableCopy.clear();
	gHoarstoneBaseTableCopy = NULL;

	mHoarStoneRuneTableCopy.clear();
	gHoarstoneRuneTableCopy = NULL;

	mHoarStoneLvTableCopy.clear();
	gHoarstoneLvTableCopy = NULL;

	mHoarStoneStarTableCopy.clear();
	gHoarstoneStarTableCopy = NULL;

	mNewEquipLvTableCopy.clear();
	gNewEquipLvTableCopy = NULL;

	mHeroEquipRuleTableCopy.clear();
	gHeroEquipRuleTableCopy = NULL;

	mEquipGemRuleTableCopy.clear();
	gEquipGemRuleTableCopy = NULL;

	mMaterialTableCopy.clear();
	gMaterialTableCopy = NULL;

	mNewGemTableCopy.clear();
	gNewGemTableCopy = NULL;

	mNewJewelryTableCopy.clear();
	gNewJewelryTableCopy = NULL;

	mServantBattleOpenTableCopy.clear();
	gServantBattleOpenTableCopy = NULL;

	mServantTableCopy.clear();
	gServantTableCopy = NULL;

	mServantStarTableCopy.clear();
	gServantTableCopy = NULL;

	mServantMaterialTableCopy.clear();
	gServantTableCopy = NULL;

	mServantLvUpTableCopy.clear();
	gServantLvUpTableCopy = NULL;

	mServantFloorTableCopy.clear();
	gServantFloorTableCopy = NULL;

	mServantTreasureTableCopy.clear();
	gServantTreasureTableCopy = NULL;

	mServantAssistBattleTableCopy.clear();
	gServantAssistBattleTableCopy = NULL;

	mServantLuckTableCopy.clear();
	gServantLuckTableCopy = NULL;

	mAttrPowerTableCopy.clear();
	gAttrPowerTableCopy = NULL;

	mHeroFashionTableCopy.clear();
	gHeroFashionTableCopy = NULL;

	mAchievementTableCopy.clear();
	gAchievementTableCopy = NULL;

	mEventTableCopy.clear();
	gEventTableCopy = NULL;

	mDailyTaskTableCopy.clear();
	gDailyTaskTableCopy = NULL;

	mDailyActiveTableCopy.clear();
	gDailyActiveTableCopy = NULL;

	mMonsterAwardTableCopy.clear();
	gMonsterAwardTableCopy = NULL;

	mSevenDayTrainningTableCopy.clear();
	gSevenDayTrainningTableCopy = NULL;	

	mConsortMsgTableCopy.clear();
	gConsortMsgTableCopy = NULL;

	mRankedShopTableCopy.clear();
	gRankedShopTableCopy = NULL;

	mRankRewardTableCopy.clear();
	gRankRewardTableCopy = NULL;

	mSystemInfoTableCopy.clear();
	gSystemInfoTableCopy = NULL;

	mConsortLvTableCopy.clear();
	gConsortLvTableCopy = NULL;

	mConsortSignTableCopy.clear();
	gConsortSignTableCopy = NULL;

	mCatTaskTableCopy.clear();
	gCatTaskTableCopy = NULL;

	mKitchenQueTableCopy.clear();
	gKitchenQueTableCopy = NULL;

	mBusinessCatTableCopy.clear();
	gBusinessCatTableCopy = NULL;

	mTicketFriendTableCopy.clear();
	gTicketFriendTableCopy = NULL;

	mConsortWoodTableCopy.clear();
	gConsortWoodTableCopy = NULL;

	
}

bool
TableManagerCopy::reload(const String& fileName)
{
	String fileNameFix = fileName;
	lynxToLowerCase(fileNameFix);

	if (fileNameFix == "item.dbc")
	{
		if (!mItemTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload ItemTable table.");
			return false;
		}
		return true;
	}

	if(fileNameFix == "attrenhance.dbc")
	{
		if (!mAttrEnhanceTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload AttrEnhanceTable table.");
			return false;
		}
		return true;
	}

	if(fileNameFix == "attrrandom.dbc")
	{
		if(!mAttrRandomTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload AttrRandomTable table.");
			return false;
		}

		return true;
	}

	if(fileNameFix == "buff.dbc")
	{
		if(!mBuffTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload buffTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "chapter.dbc")
	{
		if(!mChapterTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload chapterTable table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "effect.dbc")
	{
		if(!mEffectTableCopy.reloadFromDbc(fileName))
		{

			LOG_WARN("Failed to reload effectTable table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "equip.dbc")
	{
		if(!mEquipTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equipTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "equipset.dbc")
	{
		if(!mEquipSetTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equipsetTable table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "fashion.dbc")
	{
		if(!mFashionTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod fashionTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "gem.dbc")
	{
		if(!mGemTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod gemTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "hero.dbc")
	{
		if(!mHeroTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod heroTable table");
			return false;
		}

		return true;
	}


	if(fileNameFix == "levelgrow.dbc")
	{
		if(!mLevelGrowTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod levelgrow table");
			return false;
		}

		return true;
	}
	if(fileNameFix == "powervalue.dbc")
	{
		if(!mPowerValueTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod powervalue table");
			return false;
		}

		return true;
	}


	if(fileNameFix == "playerexp.dbc")
	{
		if(!mPlayerExpTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod playerexp table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "stage.dbc")
	{
		if(!mStageTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod stage table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "twelvepalacestage.dbc")
	{
		if(!mPalacesStageTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod twelvepalacestage table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "copyfinishtime.dbc")
	{
		if(!mCopyFinishTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod copyfinishtime table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "globalvar.dbc")
	{
		if(!mGlobalVarTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to relaod GlobalVarTable table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "award.dbc")
	{
		if(!mAwardTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardTable  table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "roulette.dbc")
	{
		if(!mAwardRuoletteTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to roulette  table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "awardcard.dbc")
	{
		if(!mAwardCardTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardCardTable  table");
			return false;
		}

		return true;
	}

	if(fileNameFix == "awardcontent.dbc")
	{
		if(!mAwardContentTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardContentTable  table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "roulettecontent.dbc")
	{
		if(!mAwardContentRuoletteTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to AwardContentRuoletteTable  table");
			return false;
		}
		return true;
	}


	if(fileNameFix == "equipattrpolish.dbc")
	{
		if(!mAttrPolishTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload attrpolish table");
			return false;
		}
		return true;
	}



	if(fileNameFix == "lottery.dbc")
	{
		if(!mLotteryTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload lottery table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "equippolishpoints.dbc")
	{
		if(!mPolishPointsTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equippolishpoints table");
			return false;
		}


		return true;
	}

	if(fileNameFix == "boxcounter.dbc")
	{
		if(!mBoxCounterTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload boxcounter table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "skeysvalue.dbc")
	{
		if(!mSkeySvalueTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload skeysvalue table");
			return false;
		}
		return true;
	}



	if(fileNameFix == "equipmaterialpoints.dbc")
	{
		if(!mPolishPointsTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equipmaterialpoints table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "gemcombine.dbc")
	{
		if(!mGemCombineTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload gemcombine table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "gemcombineonce.dbc")
	{
		if(!mGemCombineOnceTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload gemcombineonce table");
			return false;
		}
		return true;
	}
	if(fileNameFix == "vip.dbc")
	{
		if(!mVipTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload vip table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "welfaremarket.dbc")
	{
		if(!mWelfareMarketTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload WelfareMarket table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rhymelevel.dbc")
	{
		if(!mRhymeLevelTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rhymelevel table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rhymeacupoint.dbc")
	{
		if(!mRhymeAcupointTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rhymeacupoint table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rhymeskill.dbc")
	{
		if(!mRhymeSkillTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rhymeskill table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "skilllevel.dbc")
	{
		if(!mSkillLevelTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload skilllevel table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "skillbase.dbc")
	{
		if(!mSkillBaseTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload skillbase table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hoarstonebase.dbc")
	{
		if(!mHoarStoneBaseTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstonebase table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hoarstoneequip.dbc")
	{
		if(!mHoarStoneRuneTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstoneequip table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hoarstonelevel.dbc")
	{
		if(!mHoarStoneLvTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstonelevel table");
			return false;
		}
		return true;
	}


	if(fileNameFix == "hoarstonestar.dbc")
	{
		if(!mHoarStoneStarTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hoarstonestar table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "new_equip_level.dbc")
	{
		if(!mNewEquipLvTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload new_equip_level table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "hero_equip_rule.dbc")
	{
		if(!mHeroEquipRuleTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload hero_equip_rule table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "equip_gem_rule.dbc")
	{
		if(!mEquipGemRuleTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload equip_gem_rule table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "material.dbc")
	{
		if(!mMaterialTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload material table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "new_gem.dbc")
	{
		if(!mNewGemTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload new_gem table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "new_jewelry.dbc")
	{
		if(!mNewJewelryTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload new_jewelry table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantbattleopen.dbc")
	{
		if(!mServantBattleOpenTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantbattleopen table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servant.dbc")
	{
		if(!mServantTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servant table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantstar.dbc")
	{
		if(!mServantStarTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantstar table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantlvupmaterial.dbc")
	{
		if(!mServantMaterialTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantlvupmaterial table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantlvup.dbc")
	{
		if(!mServantLvUpTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantlvup table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantfloor.dbc")
	{
		if(!mServantFloorTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantfloor table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servanttreasure.dbc")
	{
		if(!mServantTreasureTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servanttreasure table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantassisbattle.dbc")
	{
		if(!mServantAssistBattleTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantassisbattle table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "servantlucky.dbc")
	{
		if(!mServantLuckTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload servantlucky table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "attrpower.dbc")
	{
		if(!mAttrPowerTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload attrpower table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "herofashion.dbc")
	{
		if(!mHeroFashionTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload herofashion table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "achievement.dbc")
	{
		if(!mAchievementTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload achievement table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "event.dbc")
	{
		if(!mEventTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload event table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "dailytask.dbc")
	{
		if(!mDailyTaskTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload dailytask table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "active.dbc")
	{
		if(!mDailyActiveTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload dailyactive table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "monsteraward.dbc")
	{
		if(!mMonsterAwardTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload monsteraward table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "sevendaystrainning.dbc")
	{
		if(!mSevenDayTrainningTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload sevendaystrainning table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "consortmsg.dbc")
	{
		if(!mConsortMsgTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload consortmsg table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rankedshop.dbc")
	{
		if(!mRankedShopTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rankedshop table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "rankreward.dbc")
	{
		if(!mRankRewardTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload rankreward table");
			return false;
		}
		return true;
	}
	if(fileNameFix == "systeminfo.dbc")
	{
		if(!mSystemInfoTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload systeminfo table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "guildlevel.dbc")
	{
		if(!mConsortLvTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload guildlevel table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "guildsign.dbc")
	{
		if(!mConsortSignTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload guildsign table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "cattask.dbc")
	{
		if(!mCatTaskTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload cattask table");
			return false;
		}
		return true;
	}
	
	if(fileNameFix == "ticketfriend.dbc")
	{
		if(!mTicketFriendTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload ticketfriend table");
			return false;
		}
		return true;
	}

	if(fileNameFix == "woodencat.dbc")
	{
		if(!mConsortWoodTableCopy.reloadFromDbc(fileName))
		{
			LOG_WARN("Failed to reload woodencat table");
			return false;
		}
		return true;
	}

	return false;
}

bool
TableManagerCopy::reloadAll()
{

	if (!mItemTableCopy.reloadFromDbc("item.dbc"))
	{
		LOG_WARN("Failed to reload item table.");
		return false;
	}	

	if (!mAttrEnhanceTableCopy.reloadFromDbc("attrenhance.dbc"))
	{
		LOG_WARN("Failed to reload AttrEnhanceTable table.");
		return false;
	}

	if(!mAttrRandomTableCopy.reloadFromDbc("attrrandom.dbc"))
	{
		LOG_WARN("Failed to reload AttrRandomTable table.");
		return false;
	}

	if(!mBuffTableCopy.reloadFromDbc("buff.dbc"))
	{
		LOG_WARN("Failed to reload BuffTable table.");
		return false;
	}

	if(!mChapterTableCopy.reloadFromDbc("chapter.dbc"))
	{
		LOG_WARN("Failed to reload ChapterTable table.");
		return false;
	}

	if(!mEffectTableCopy.reloadFromDbc("effect.dbc"))
	{
		LOG_WARN("Failed to reload EffectTable table");
		return false;
	}

	if(!mEquipTableCopy.reloadFromDbc("equip.dbc"))
	{
		LOG_WARN("Failed to reload EquipTable table");
		return false;
	}

	if(!mEquipSetTableCopy.reloadFromDbc("equipset.dbc"))
	{
		LOG_WARN("Failed to reload equipset table");
		return false;
	}

	if(!mFashionTableCopy.reloadFromDbc("fashion.dbc"))
	{
		LOG_WARN("Failed to reload fashionTable table");
		return false;
	}

	if(!mGemTableCopy.reloadFromDbc("gem.dbc"))
	{
		LOG_WARN("Failed to reload gemTable table");
		return false;
	}

	if(!mHeroTableCopy.reloadFromDbc("hero.dbc"))
	{
		LOG_WARN("Failed to reload heroTable table");
		return false;
	}

	if(!mLevelGrowTableCopy.reloadFromDbc("levelgrow.dbc"))
	{
		LOG_WARN("Failed to reload levelgrow table");
		return false;
	}

	if(!mPowerValueTableCopy.reloadFromDbc("powervalue.dbc"))
	{
		LOG_WARN("Failed to reload powervalue table");
		return false;
	}

	if(!mPlayerExpTableCopy.reloadFromDbc("playerexp.dbc"))
	{
		LOG_WARN("Failed to reload playerexp table");
		return false;
	}


	if(!mStageTableCopy.reloadFromDbc("stage.dbc"))
	{
		LOG_WARN("Failed to reload stage table");
		return false;
	}

	if(!mPalacesStageTableCopy.reloadFromDbc("twelvepalacestage.dbc"))
	{
		LOG_WARN("Failed to reload twelvepalacestage table");
		return false;
	}


	if(!mCopyFinishTableCopy.reloadFromDbc("copyfinishtime.dbc"))
	{
		LOG_WARN("Failed to relaod copyfinishtime table");
		return false;
	}


	if(!mGlobalVarTableCopy.reloadFromDbc("globalvar.dbc"))
	{
		LOG_WARN("Failed to relaod globalvar table");
		return false;
	}
	if(!mAwardTableCopy.reloadFromDbc("award.dbc"))
	{
		LOG_WARN("Failed to relaod AwardTable table");
		return false;
	}
	if(!mAwardRuoletteTableCopy.reloadFromDbc("roulette.dbc"))
	{
		LOG_WARN("Failed to relaod roulette table");
		return false;
	}
	if(!mAwardCardTableCopy.reloadFromDbc("awardcard.dbc"))
	{
		LOG_WARN("Failed to relaod AwardCardTable table");
		return false;
	}
	if(!mAwardContentTableCopy.reloadFromDbc("awardcontent.dbc"))
	{
		LOG_WARN("Failed to relaod AwardContentTable table");
		return false;
	}

	if(!mAwardContentRuoletteTableCopy.reloadFromDbc("roulettecontent.dbc"))
	{
		LOG_WARN("Failed to relaod mAwardContentRuoletteTable table");
		return false;
	}

	if(!mAttrPolishTableCopy.reloadFromDbc("equipattrpolish.dbc"))
	{
		LOG_WARN("Failed to reload attrpolish table");
		return false;
	}



	if(!mPolishPointsTableCopy.reloadFromDbc("equippolishpoints.dbc"))
	{
		LOG_WARN("Failed to reload equippolishpoints table");
		return false;
	}

	if(!mMaterialPointsTableCopy.reloadFromDbc("equipmaterialpoints.dbc"))
	{
		LOG_WARN("Failed to reload equipmaterialpoints table");
		return false;
	}

	if(!mGlobalVarTableCopy.reloadFromDbc("globalvar.dbc"))
	{
		LOG_WARN("Failed to relaod GlobalVarTable table");
		return false;
	}

	if(!mGemCombineTableCopy.reloadFromDbc("gemcombine.dbc"))
	{
		LOG_WARN("Failed to reload gemcombine table");
		return false;
	}

	if(!mGemCombineOnceTableCopy.reloadFromDbc("gemcombineonce.dbc"))
	{
		LOG_WARN("Failed to reload gemcombineonce table");
		return false;
	}

	if(!mAwardCardTableCopy.reloadFromDbc("awardcard.dbc"))
	{
		LOG_WARN("Failed to AwardCardTable  table");
		return false;
	}

	if(!mAwardContentTableCopy.reloadFromDbc("awardcontent.dbc"))
	{
		LOG_WARN("Failed to AwardContentTable  table");
		return false;
	}

	if(!mAwardContentRuoletteTableCopy.reloadFromDbc("roulettecontent.dbc"))
	{
		LOG_WARN("Failed to mAwardContentRuoletteTable  table");
		return false;
	}

	if(!mAttrPolishTableCopy.reloadFromDbc("equipattrpolish.dbc"))
	{
		LOG_WARN("Failed to reload attrpolish table");
		return false;
	}	

	if(!mLotteryTableCopy.reloadFromDbc("lottery.dbc"))
	{
		LOG_WARN("Failed to reload lottery table");
		return false;
	}

	if(!mBoxCounterTableCopy .reloadFromDbc("boxcounter.dbc"))
	{
		LOG_WARN("Failed to reload boxcounter table");
		return false;
	}

	if(!mSkeySvalueTableCopy.reloadFromDbc("skeysvalue.dbc"))
	{
		LOG_WARN("Failed to reload skeysvalue table");
		return false;
	}

	if(!mVipTableCopy.reloadFromDbc("vip.dbc"))
	{
		LOG_WARN("Failed to reload vip table");
		return false;
	}
	if(!mWelfareMarketTableCopy.reloadFromDbc("welfaremarket.dbc"))
	{
		LOG_WARN("Failed to reload WelfareMarket table");
		return false;
	}


	if(!mRhymeLevelTableCopy.reloadFromDbc("rhymelevel.dbc"))
	{
		LOG_WARN("Failed to reload rhymelevel table");
		return false;
	}

	if(!mRhymeAcupointTableCopy.reloadFromDbc("rhymeacupoint.dbc"))
	{
		LOG_WARN("Failed to reload rhymeacupoint table");
		return false;
	}

	if(!mRhymeSkillTableCopy.reloadFromDbc("rhymeskill.dbc"))
	{
		LOG_WARN("Failed to reload rhymeskill table");
		return false;
	}

	if(!mSkillLevelTableCopy.reloadFromDbc("skilllevel.dbc"))
	{
		LOG_WARN("Failed to reload skilllevel table");
		return false;
	}

	if(!mSkillBaseTableCopy.reloadFromDbc("skillbase.dbc"))
	{
		LOG_WARN("Failed to reload skillBase table");
		return false;
	}

	if(!mHoarStoneBaseTableCopy.reloadFromDbc("hoarstonebase.dbc"))
	{
		LOG_WARN("Failed to reload hoarstonebase table");
		return false;
	}

	if(!mHoarStoneRuneTableCopy.reloadFromDbc("hoarstoneequip.dbc"))
	{
		LOG_WARN("Failed to reload hoarstoneequip table");
		return false;
	}

	if(!mHoarStoneRuneTableCopy.reloadFromDbc("hoarstonelevel.dbc"))
	{
		LOG_WARN("Failed to reload hoarstonelevel table");
		return false;
	}

	if(!mHoarStoneStarTableCopy.reloadFromDbc("hoarstonestar.dbc"))
	{
		LOG_WARN("Failed to reload hoarstonestar table");
		return false;
	}

	if(!mNewEquipLvTableCopy.reloadFromDbc("new_equip_level.dbc"))
	{
		LOG_WARN("Failed to reload new_equip_level table");
		return false;
	}

	if(!mHeroEquipRuleTableCopy.reloadFromDbc("hero_equip_rule.dbc"))
	{
		LOG_WARN("Failed to reload hero_equip_rule table");
		return false;
	}

	if(!mEquipGemRuleTableCopy.reloadFromDbc("equip_gem_rule.dbc"))
	{
		LOG_WARN("Failed to reload hero_equip_rule table");
		return false;
	}

	if(!mMaterialTableCopy.reloadFromDbc("material.dbc"))
	{
		LOG_WARN("Failed to reload material table");
		return false;
	}

	if(!mNewGemTableCopy.reloadFromDbc("new_gem.dbc"))
	{
		LOG_WARN("Failed to reload new_gem table");
		return false;
	}

	if(!mNewJewelryTableCopy.reloadFromDbc("new_jewelry.dbc"))
	{
		LOG_WARN("Failed to reload new_jewelry table");
		return false;
	}

	if(!mServantBattleOpenTableCopy.reloadFromDbc("servantbattleopen.dbc"))
	{
		LOG_WARN("Failed to reload servantbattleopen table");
		return false;
	}

	if(!mServantTableCopy.reloadFromDbc("servant.dbc"))
	{
		LOG_WARN("Failed to reload servant table");
		return false;
	}

	if(!mServantStarTableCopy.reloadFromDbc("servantstar.dbc"))
	{
		LOG_WARN("Failed to reload servantstar table");
		return false;
	}

	if(!mServantMaterialTableCopy.reloadFromDbc("servantlvupmaterial.dbc"))
	{
		LOG_WARN("Failed to reload servantlvupmaterial table");
		return false;
	}

	if(!mServantLvUpTableCopy.reloadFromDbc("servantlvup.dbc"))
	{
		LOG_WARN("Failed to reload servantlvup table");
		return false;
	}

	if(!mServantFloorTableCopy.reloadFromDbc("servantfloor.dbc"))
	{
		LOG_WARN("Failed to reload servantfloor table");
		return false;
	}

	if(!mServantTreasureTableCopy.reloadFromDbc("servanttreasure.dbc"))
	{
		LOG_WARN("Failed to reload servanttreasure table");
		return false;
	}

	if(!mServantAssistBattleTableCopy.reloadFromDbc("servantassisbattle.dbc"))
	{
		LOG_WARN("Failed to reload servantassisbattle table");
		return false;
	}

	if(!mServantLuckTableCopy.reloadFromDbc("servantlucky.dbc"))
	{
		LOG_WARN("Failed to reload servantlucky table");
		return false;
	}

	if(!mTowerTypeTableCopy.reloadFromDbc("towertype.dbc"))
	{
		LOG_WARN("Failed to reload towertype table");
		return false;
	}

	if(!mTowerMonsterTableCopy.reloadFromDbc("towermonster.dbc"))
	{
		LOG_WARN("Failed to reload towermonster table");
		return false;
	}

	if(!mTowerEventTableCopy.reloadFromDbc("towerevent.dbc"))
	{
		LOG_WARN("Failed to reload towerevent table");
		return false;
	}

	if(!mTowerBuffTableCopy.reloadFromDbc("towerbuff.dbc"))
	{
		LOG_WARN("Failed to reload towerbuff table");
		return false;
	}

	if(!mTowerAttrTableCopy.reloadFromDbc("towerattr.dbc"))
	{
		LOG_WARN("Failed to reload towerattr table");
		return false;
	}

	if(!mShopsTableCopy.reloadFromDbc("mysteryshop.dbc"))
	{
		LOG_WARN("Failed to reload mysteryshop table");
		return false;
	}

	if(!mTowerBoxTableCopy.reloadFromDbc("towerbox.dbc"))
	{
		LOG_WARN("Failed to reload towerbox table");
		return false;
	}

	if(!mTowersTableCopy.reloadFromDbc("towers.dbc"))
	{
		LOG_WARN("Failed to reload towers table");
		return false;
	}

	if(!mRewardLotteryTableCopy.reloadFromDbc("rewardlottery.dbc"))
	{
		LOG_WARN("Failed to reload rewardlottery table");
		return false;
	}

	if(!mLotteryActivityTableCopy.reloadFromDbc("lotteryactivity.dbc"))
	{
		LOG_WARN("Failed to reload lotteryactivity table");
		return false;
	}

	if(!mAttrPowerTableCopy.reloadFromDbc("attrpower.dbc"))
	{
		LOG_WARN("Failed to reload attrpower table");
		return false;
	}



	if(!mCommodityTableCopy.reloadFromDbc("commodity.dbc"))
	{
		LOG_WARN("Failed to reload commodity table");
		return false;
	}
	if(!mGiftTableCopy.reloadFromDbc("gift.dbc"))
	{
		LOG_WARN("Failed to reload gift table");
		return false;
	}
	if(!mGiftShopTableCopy.reloadFromDbc("giftshop.dbc"))
	{
		LOG_WARN("Failed to reload giftshop table");
		return false;
	}
	if(!mMysticalshopTableCopy.reloadFromDbc("mysticalshop.dbc"))
	{
		LOG_WARN("Failed to reload mysticalshop table");
		return false;
	}
	if(!mServantshopTableCopy.reloadFromDbc("servantshop.dbc"))
	{
		LOG_WARN("Failed to reload servantshop table");
		return false;
	}

	if(!mCourageshopTableCopy.reloadFromDbc("courageshop.dbc"))
	{
		LOG_WARN("Failed to reload courageshop table");
		return false;
	}

	if(!mCoinshopTableCopy.reloadFromDbc("coinshop.dbc"))
	{
		LOG_WARN("Failed to reload coinshop table");
		return false;
	}

	if(!mAwardContentShopTableCopy.reloadFromDbc("awardcontentshop.dbc"))
	{
		LOG_WARN("Failed to reload awardcontentshop table");
		return false;
	}

	if(!mHeroFashionTableCopy.reloadFromDbc("herofashion.dbc"))
	{
		LOG_WARN("Failed to reload herofashion table");
		return false;
	}

	if(!mAchievementTableCopy.reloadFromDbc("achievement.dbc"))
	{
		LOG_WARN("Failed to reload achievement table");
		return false;
	}

	if(!mEventTableCopy.reloadFromDbc("event.dbc"))
	{
		LOG_WARN("Failed to reload event table");
		return false;
	}


	if(!mDailyTaskTableCopy.reloadFromDbc("dailytask.dbc"))
	{
		LOG_WARN("Failed to reload dailytask table");
		return false;
	}

	if(!mDailyActiveTableCopy.reloadFromDbc("active.dbc"))
	{
		LOG_WARN("Failed to reload dailyactive table");
		return false;
	}

	if(!mMonsterAwardTableCopy.reloadFromDbc("monsteraward.dbc"))
	{
		LOG_WARN("Failed to reload monsteraward table");
		return false;
	}

	if(!mSevenDayTrainningTableCopy.reloadFromDbc("sevendaystrainning.dbc"))
	{
		LOG_WARN("Failed to reload sevendaystrainning table");
		return false;
	}

	if(!mConsortMsgTableCopy.reloadFromDbc("consortmsg.dbc"))
	{
		LOG_WARN("Failed to reload consortmsg table");
		return false;
	}

	if(!mRankedShopTableCopy.reloadFromDbc("rankedshop.dbc"))
	{
		LOG_WARN("Failed to reload rankedshop table");
		return false;
	}

	if(!mRankRewardTableCopy.reloadFromDbc("systeminfo.dbc"))
	{
		LOG_WARN("Failed to reload systeminfo table");
		return false;
	}

	if(!mConsortLvTableCopy.reloadFromDbc("guildlevel.dbc"))
	{
		LOG_WARN("Failed to reload guildlevel table");
		return false;
	}

	if(!mConsortSignTableCopy.reloadFromDbc("guildsign.dbc"))
	{
		LOG_WARN("Failed to reload guildsign table");
		return false;
	}

	if(!mCatTaskTableCopy.reloadFromDbc("cattask.dbc"))
	{
		LOG_WARN("Failed to reload cattask table");
		return false;
	}

	if(!mTicketFriendTableCopy.reloadFromDbc("ticketfriend.dbc"))
	{
		LOG_WARN("Failed to reload ticketfriend table");
		return false;
	}

	if(!mConsortWoodTableCopy.reloadFromDbc("woodencat.dbc"))
	{
		LOG_WARN("Failed to reload woodencat table");
		return false;
	}

	return true;
}

