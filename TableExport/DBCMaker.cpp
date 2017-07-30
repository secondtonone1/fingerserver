#include "DBCMaker.h"
#include "CommonLib.h"

using namespace Lynx;

//static StringTable s_StringTable;

DBCMaker::DBCMaker() : m_Path("")
{

}

DBCMaker::~DBCMaker()
{

}

bool 
DBCMaker::Initial(const String& filePath)
{
    m_Path = filePath;
    return true;
}

void 
DBCMaker::Release()
{

}

bool 
DBCMaker::Convert(const String& fileName)
{
    String filePath = m_Path + "/" + fileName;
    String dbcFileName = fileName;
    lynxStrReplace(dbcFileName, ".csv", ".dbc");
    String dbcFilePath = m_Path + "/" + dbcFileName;

	if (fileName == "item.csv")
	{
		ItemTable table;
		if (!table.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load item.csv file");
			return false;
		}
		if (!table.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to itemTable.dbc file");
			return false;
		}
	}
	else if(fileName == "attrenhance.csv")
	{
		AttrEnhanceTable attrEnhanceTable;
		
		if(!attrEnhanceTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load attrenhance.csv file");
			return false;
		}

		if(!attrEnhanceTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to attrenhance.dbc file");
			return false;
		}

	}
	else if(fileName == "attrrandom.csv")
	{
		AttrRandomTable attrRandomTable;

		if(!attrRandomTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load attrrandom.csv file");
			return false;
		}

		if(!attrRandomTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to attrrandom.dbc file");
			return false;
		}

	}
	else if(fileName == "buff.csv")
	{
		BuffTable buffTable;
		if(!buffTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load buff.csv file");
			return false;
		}
		
		if(!buffTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to buff.dbc file");
			return false;
		}

	}
	else if(fileName == "chapter.csv")
	{
		ChapterTable chapterTable;
		if(!chapterTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load chapter.csv file");
			return false;
		}

		if(!chapterTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to chapter.dbc file");
			return false;
		}
	}
	else if(fileName == "effect.csv")
	{
		EffectTable effectTable;
		if(!effectTable.loadFromCsv(filePath))
		{	
			LOG_WARN("Failed to load effect.csv file");
			return false;
		}

		if(!effectTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to effect.dbc file");
			return false;
		}
	}
	else if(fileName == "equip.csv")
	{
		EquipTable equipTable;
		if(!equipTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load equip.csv file");
			return false;
		}

		if(!equipTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to equip.dbc file");
			return false;
		}
	}
	else if(fileName == "equipset.csv")
	{
		EquipSetTable equipSetTable;
		if(!equipSetTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load equipset.csv file");
			return false;
		}

		if(!equipSetTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to equipset.dbc file");
			return false;
		}

	}
	else if(fileName == "fashion.csv")
	{
		FashionTable fashionTable;
		if(!fashionTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load fashion.csv file");
			return false;
		}

		if(!fashionTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to fashion.dbc file");
			return false;
		}

	}
	else if(fileName == "gem.csv")
	{
		GemTable gemTable;
		if(!gemTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load gemTable.csv file");
			return false;
		}

		if(!gemTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to gemTable.dbc file");
			return false;
		}

	}
	else if(fileName == "hero.csv")
	{
		HeroTable heroTable;
		if(!heroTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load heroTable.csv file");
			return false;
		}

		if(!heroTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to heroTable.dbc file");
			return false;
		}

	}
	else if(fileName == "levelgrow.csv")
	{
		LevelGrowTable levelGrowTable;
		if(!levelGrowTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load levelgrow.csv file");
			return false;
		}

		if(!levelGrowTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to levelgrow.dbc file");
			return false;
		}
	}
	else if(fileName == "powervalue.csv")
	{
		PowerValueTable powerValueTable;
		if(!powerValueTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load powervalue.csv file");
			return false;
		}

		if(!powerValueTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to powervalue.dbc file");
			return false;
		}

	}
	else if(fileName == "playerexp.csv")
	{
		PlayerExpTable playerExpTable;
		if(!playerExpTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load playerexp.csv file");
			return false;
		}

		if(!playerExpTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to playerexp.dbc file");
			return false;
		}

	}
	else if(fileName == "stage.csv")
	{
		StageTable stageTable;
		if(!stageTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load stage.csv file");
			return false;
		}

		if(!stageTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to stage.dbc file");
			return false;
		}
	}

	else if(fileName == "twelvepalacestage.csv")
	{
		StageTable stageTable;
		if(!stageTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load twelvepalacestage.csv file");
			return false;
		}

		if(!stageTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to twelvepalacestage.dbc file");
			return false;
		}
	}

	else if(fileName == "typecondition.csv")
	{
		TypeConditionTable typeConditionTable;
		if(!typeConditionTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load typecondition.csv file");
			return false;
		}

		if(!typeConditionTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to typecondition.dbc file");
			return false;
		}
	}
	else if(fileName == "towertype.csv")
	{
		TowerTypeTable towerTypeTable;
		if(!towerTypeTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load towertype.csv file");
			return false;
		}

		if(!towerTypeTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to towertype.dbc file");
			return false;
		}
	}

	else if(fileName == "towermonster.csv")
	{
		TowerMonsterTable towerMonsterTable;
		if(!towerMonsterTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load towermonster.csv file");
			return false;
		}

		if(!towerMonsterTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to towermonster.dbc file");
			return false;
		}
	}

	else if(fileName == "towerevent.csv")
	{
		TowerEventTable towerEventTable;
		if(!towerEventTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load towerevent.csv file");
			return false;
		}

		if(!towerEventTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to towerevent.dbc file");
			return false;
		}
	}

	else if(fileName == "towerbuff.csv")
	{
		TowerBuffTable towerBuffTable;
		if(!towerBuffTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load towerbuff.csv file");
			return false;
		}

		if(!towerBuffTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to towerbuff.dbc file");
			return false;
		}
	}

	else if(fileName == "mysteryshop.csv")
	{
		ShopsTable shopsTable;
		if(!shopsTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load mysteryshop.csv file");
			return false;
		}

		if(!shopsTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to mysteryshop.dbc file");
			return false;
		}
	}

	else if(fileName == "towerbox.csv")
	{
		TowerBoxTable towerBoxTable;
		if(!towerBoxTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load towerbox.csv file");
			return false;
		}

		if(!towerBoxTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to towerbox.dbc file");
			return false;
		}
	}

	else if(fileName == "towers.csv")
	{
		TowersTable towersTable;
		if(!towersTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load towers.csv file");
			return false;
		}

		if(!towersTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to towers.dbc file");
			return false;
		}
	}

	else if(fileName == "rewardlottery.csv")
	{
		RewardLotteryTable rewardLotteryTable;
		if(!rewardLotteryTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load rewardlottery.csv file");
			return false;
		}

		if(!rewardLotteryTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to rewardlottery.dbc file");
			return false;
		}
	}

	else if(fileName == "lotteryactivity.csv")
	{
		LotteryActivityTable lotteryActivityTable;
		if(!lotteryActivityTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load rewardlottery.csv file");
			return false;
		}

		if(!lotteryActivityTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to rewardlottery.dbc file");
			return false;
		}
	}




	else if(fileName == "commodity.csv")
	{
		CommodityTable commodityTable;
		if(!commodityTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load commodity.csv file");
			return false;
		}

		if(!commodityTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to commodity.dbc file");
			return false;
		}
	}

	else if(fileName == "gift.csv")
	{
		GiftTable giftTable;
		if(!giftTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load gift.csv file");
			return false;
		}

		if(!giftTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to gift.dbc file");
			return false;
		}
	}

	else if(fileName == "giftshop.csv")
	{
		GiftShopTable giftShopTable;
		if(!giftShopTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load giftshop.csv file");
			return false;
		}

		if(!giftShopTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to giftshop.dbc file");
			return false;
		}
	}

	

	else if(fileName == "mysticalshop.csv")
	{
		MysticalshopTable mysticalshopTable;
		if(!mysticalshopTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load mysticalshop.csv file");
			return false;
		}

		if(!mysticalshopTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to mysticalshop.dbc file");
			return false;
		}
	}

	else if(fileName == "servantshop.csv")
	{
		ServantshopTable servantshopTable;
		if(!servantshopTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantshop.csv file");
			return false;
		}

		if(!servantshopTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantshop.dbc file");
			return false;
		}
	}
	else if(fileName == "courageshop.csv")
	{
		CourageshopTable courageshopTable;
		if(!courageshopTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load courageshop.csv file");
			return false;
		}

		if(!courageshopTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to courageshop.dbc file");
			return false;
		}
	}
	else if(fileName == "coinshop.csv")
	{
		CoinshopTable coinshopTable;
		if(!coinshopTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load coinshop.csv file");
			return false;
		}

		if(!coinshopTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to coinshop.dbc file");
			return false;
		}
	}

	else if(fileName == "awardcontentshop.csv")
	{
		AwardContentShopTable awardContentShopTable;
		if(!awardContentShopTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load awardcontentshop.csv file");
			return false;
		}

		if(!awardContentShopTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to awardcontentshop.dbc file");
			return false;
		}
	}


	else if(fileName == "gainstrength.csv")
	{
		GainStrengthTable gainStrengthTable;
		if(!gainStrengthTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load gainstrength.csv file");
			return false;
		}

		if(!gainStrengthTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to gainstrength.dbc file");
			return false;
		}
	}


	else if(fileName == "gainwealth.csv")
	{
		GainWealthTable gainWealthTable;
		if(!gainWealthTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load gainwealth.csv file");
			return false;
		}

		if(!gainWealthTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to gainwealth.dbc file");
			return false;
		}
	}


	else if(fileName == "premiums.csv")
	{
		PremiumsTable premiumsTable;
		if(!premiumsTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load premiums.csv file");
			return false;
		}

		if(!premiumsTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to premiums.dbc file");
			return false;
		}
	}


	else if(fileName == "insideactivities.csv")
	{
		InlineActivityTable inlineActivityTable;
		if(!inlineActivityTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load insideactivities.csv file");
			return false;
		}

		if(!inlineActivityTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to insideactivities.dbc file");
			return false;
		}
	}



	else if(fileName == "coin.csv")
	{
		CoinTable coinTable;
		if(!coinTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load coin.csv file");
			return false;
		}

		if(!coinTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to coin.dbc file");
			return false;
		}
	}

	else if(fileName == "coingroup.csv")
	{
		CoinGroupTable coinGroupTable;
		if(!coinGroupTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load coingroup.csv file");
			return false;
		}

		if(!coinGroupTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to coingroup.dbc file");
			return false;
		}
	}

	else if(fileName == "couragecontent.csv")
	{
		CourageContentTable courageContentTable;
		if(!courageContentTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load couragecontent.csv file");
			return false;
		}

		if(!courageContentTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to couragecontent.dbc file");
			return false;
		}
	}

	else if(fileName == "territory.csv")
	{
		TerritoryTable territoryTable;
		if(!territoryTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load territory.csv file");
			return false;
		}

		if(!territoryTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to territory.dbc file");
			return false;
		}
	}

	else if(fileName == "robot.csv")
	{
		RobotTable RobotTable;
		if(!RobotTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load robot.csv file");
			return false;
		}

		if(!RobotTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to robot.dbc file");
			return false;
		}
	}



	else if(fileName == "towerattr.csv")
	{
		TowerAttrTable towerAttrTable;
		if(!towerAttrTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load towerattr.csv file");
			return false;
		}

		if(!towerAttrTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to towerattr.dbc file");
			return false;
		}
	}


	else if(fileName == "awardaddrate.csv")
	{
		AwardAddRateTable awardAddRateTable;
		if(!awardAddRateTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load awardaddrate.csv file");
			return false;
		}

		if(!awardAddRateTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to awardaddrate.dbc file");
			return false;
		}
	}
	
	else if(fileName == "copyfinishtime.csv")
	{
		
		CopyFinishTable copyFinishTable;
		if(!copyFinishTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load copyfinishtime.csv file");
			return false;
		}

		if(!copyFinishTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to copyfinishtime.dbc file");
			return false;
		}
	}
	else if(fileName == "globalvar.csv")
	{

		GlobalVarTable copyFinishTable;
		if(!copyFinishTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load globalvar.csv file");
			return false;
		}

		if(!copyFinishTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to globalvar.dbc file");
			return false;
		}

	}
	else if(fileName == "equipattrpolish.csv")
	{
		AttrPolishTable attrPolishTable;
		if(!attrPolishTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load equipattrpolish.csv file");
			return false;
		}

		if(!attrPolishTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to equipattrpolish.dbc file");
			return false;
		}
	}
	
	else if(fileName == "award.csv")
	{

		AwardTable awardTable;
		if(!awardTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load award.csv file");
			return false;
		}

		if(!awardTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to award.dbc file");
			return false;
		}

	}
	else if(fileName == "roulette.csv")
	{

		AwardRouletteTable awardRuoletteTable;
		if(!awardRuoletteTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load roulette.csv file");
			return false;
		}

		if(!awardRuoletteTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to roulette.dbc file");
			return false;
		}

	}
	else if(fileName == "awardcard.csv")
	{

		AwardCardTable awardCardTable;
		if(!awardCardTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load awardcard.csv file");
			return false;
		}

		if(!awardCardTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to awardcard.dbc file");
			return false;
		}

	}
	else if(fileName == "awardcontent.csv")
	{

		AwardContentTable awardContentTable;
		if(!awardContentTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load awardcontent.csv file");
			return false;
		}

		if(!awardContentTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to awardcontent.dbc file");
			return false;
		}

	}
	else if(fileName == "roulettecontent.csv")
	{

		AwardContentTable awardContentRuoletteTable;
		if(!awardContentRuoletteTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load roulettecontent.csv file");
			return false;
		}

		if(!awardContentRuoletteTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to roulettecontent.dbc file");
			return false;
		}

	}
	
	else if(fileName == "boxcounter.csv")
	{

		BoxCounterTable boxCounterTable;
		if(!boxCounterTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load boxcounter.csv file");
			return false;
		}

		if(!boxCounterTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to boxcounter.dbc file");
			return false;
		}

	}
	else if(fileName == "lottery.csv")
	{

		LotteryTable lotteryTable;
		if(!lotteryTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load LotteryTable.csv file");
			return false;
		}

		if(!lotteryTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to LotteryTable.dbc file");
			return false;
		}
		
	}
	else if(fileName == "skeysvalue.csv")
	{

		SkeySvalueTable skeySvalueTable;
		if(!skeySvalueTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load skeysvalueTable.csv file");
			return false;
		}

		if(!skeySvalueTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to skeysvalueTable.dbc file");
			return false;
		}

	}
	else if(fileName == "equippolishpoints.csv")
	{
		PolishPointsTable polishPointsTable;
		if(!polishPointsTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load equippolishpoints.csv file");
			return false;
		}
		
		if(!polishPointsTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to equippolishpoints.dbc file");
			return false;
		}
	}
	else if(fileName == "equipmaterialpoints.csv")
	{
		MaterialPointsTable materialPointsTable;
		if(!materialPointsTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load equipmaterialpoints.csv file");
			return false;
		}

		if(!materialPointsTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to equipmaterialpoints.dbc file");
			return false;
		}
	}
	else if(fileName == "gemcombine.csv")
	{
		GemCombineTable gemCombineTable;
		if(!gemCombineTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load gemcombine.csv file");
			return false;
		}

		if(!gemCombineTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to gemcombine.dbc file");
			return false;
		}
	}
	else if(fileName == "gemcombineonce.csv")
	{
		GemCombineOnceTable gemCombineOnceTable;
		if(!gemCombineOnceTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load gemcombineonce.csv file");
			return false;
		}

		if(!gemCombineOnceTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to gemcombineonce.dbc file");
			return false;
		}
	}
	else if(fileName == "vip.csv")
	{
		VipTable vipTable;
		if(!vipTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load vip.csv file");
			return false;
		}

		if(!vipTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to vip.dbc file");
			return false;
		}
	}
	else if(fileName == "welfaremarket.csv")
	{
		WelfareMarketTable WelfareMarketTable;
		if(!WelfareMarketTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load WelfareMarket.csv file");
			return false;
		}

		if(!WelfareMarketTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to WelfareMarket.dbc file");
			return false;
		}
	}
	else if(fileName == "rhymelevel.csv")
	{
		RhymeLevelTable rhymeLevelTable;
		if(!rhymeLevelTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load rhymelevel.csv file");
			return false;
		}

		if(!rhymeLevelTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to rhymelevel.dbc file");
			return false;
		}
	}
	else if(fileName == "rhymeacupoint.csv")
	{
		RhymeAcupointTable rhymeAcupointTable;
		if(!rhymeAcupointTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load rhymeacupoint.csv file");
			return false;
		}

		if(!rhymeAcupointTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to rhymeacupoint.dbc file");
			return false;
		}
	
	}
	else if(fileName == "rhymeskill.csv")
	{
		RhymeSkillTable rhymeSkillTable;
		if(!rhymeSkillTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load rhymeskill.csv file");
			return false;
		}

		if(!rhymeSkillTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to rhymeskill.dbc file");
			return false;
		}

	}
	else if(fileName == "skilllevel.csv")
	{
		SkillLevelTable skillLevelTable;
		if(!skillLevelTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load skilllevel.csv file");
			return false;
		}

		if(!skillLevelTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to skilllevel.dbc file");
			return false;
		}

	}
	else if(fileName == "skillbase.csv")
	{
		SkillBaseTable skillBaseTable;
		if(!skillBaseTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load skillbase.csv file");
			return false;
		}

		if(!skillBaseTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to skillbase.dbc file");
			return false;
		}

	}
	else if(fileName == "hoarstonebase.csv")
	{
		HoarstoneBaseTable hoarStoneBaseTable;
		
		if(!hoarStoneBaseTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load hoarstonebase.csv file");
			return false;
		}

		if(!hoarStoneBaseTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to hoarstonebase.dbc file");
			return false;
		}
	}
	else if(fileName == "hoarstoneequip.csv")
	{
		HoarstoneRuneTable hoarStoneRuneTable;
		if(!hoarStoneRuneTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load hoarstoneequip.csv file");
			return false;
		}

		if(!hoarStoneRuneTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to hoarstoneequip.dbc file");
			return false;
		}
	
	}
	else if(fileName == "hoarstonelevel.csv")
	{
		HoarstoneLvTable hoarStoneLvTable;
		if(!hoarStoneLvTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load hoarstonelevel.csv file");
			return false;
		}

		if(!hoarStoneLvTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to hoarstonelevel.dbc file");
			return false;
		}
	
	}
	else if (fileName == "hoarstonestar.csv")
	{
		 HoarstoneStarTable hoarStoneStarTable;
		if(!hoarStoneStarTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load hoarstonestar.csv file");
			return false;
		}

		if(!hoarStoneStarTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to hoarstonestar.dbc file");
			return false;
		}
	}
	else if(fileName == "new_equip_level.csv")
	{
		NewEquipLvTable newEquipLvTable;
		if(!newEquipLvTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load new_equip_level.csv file");
			return false;
		}

		if(!newEquipLvTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to new_equip_level.dbc file");
			return false;
		}
	}
	else if(fileName == "hero_equip_rule.csv")
	{
		HeroEquipRuleTable heroEquipRuleTable;
		if(!heroEquipRuleTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load hero_equip_rule.csv file");
			return false;
		}

		if(!heroEquipRuleTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to hero_equip_rule.dbc file");
			return false;
		}
	}
	else if(fileName == "equip_gem_rule.csv")
	{
		EquipGemRuleTable equipGemRuleTable;
		if(!equipGemRuleTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv file");
			return false;
		}

		if(!equipGemRuleTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to equip_gem_rule.dbc file");
			return false;
		}
	}
	else if(fileName == "material.csv")
	{
		MaterialTable materialTable;
		if(!materialTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load material.csv file");
			return false;
		}

		if(!materialTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to material.dbc file");
			return false;
		}
	}
	else if(fileName == "new_gem.csv")
	{
		NewGemTable newGemTable;
		if(!newGemTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load new_gem.csv file");
			return false;
		}

		if(!newGemTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to new_gem.dbc file");
			return false;
		}
	}
	else if(fileName == "new_jewelry.csv")
	{
		NewJewelryTable newJewelryTable;
		if(!newJewelryTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load new_jewelry.csv file");
			return false;
		}

		if(!newJewelryTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to new_jewelry.dbc file");
			return false;
		}
	}
	else if(fileName == "servantbattleopen.csv")
	{
		ServantBattleOpenTable servantBattleOpenTable;
		if(!servantBattleOpenTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantbattleopen.csv file");
			return false;
		}

		if(!servantBattleOpenTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantbattleopen.dbc file");
			return false;
		}
	}
	else if(fileName == "servant.csv")
	{
		ServantTable servantTable;
		if(!servantTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantTable.csv file");
			return false;
		}

		if(!servantTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantTable.dbc file");
			return false;
		}
	}
	else if(fileName == "servantstar.csv")
	{
		ServantStarTable servantstarTable;
		if(!servantstarTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantstar.csv file");
			return false;
		}

		if(!servantstarTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantstar.dbc file");
			return false;
		}
	}
	else if(fileName == "servantlvupmaterial.csv")
	{
		ServantLvUpMaterialTable servantMaterialTable;
		if(!servantMaterialTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantlvupmaterial.csv file");
			return false;
		}

		if(!servantMaterialTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantlvupmaterial.dbc file");
			return false;
		}
	}
	else if(fileName == "servantlvup.csv")
	{
		ServantLvUpTable servantLvUpTable;
		if(!servantLvUpTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantlvup.csv file");
			return false;
		}

		if(!servantLvUpTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantlvup.dbc file");
			return false;
		}
	}
	else if(fileName == "servantfloor.csv")
	{
		ServantFloorTable servantFloorTable;
		if(!servantFloorTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantfloor.csv file");
			return false;
		}

		if(!servantFloorTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantfloor.dbc file");
			return false;
		}
	}
	else if(fileName == "servanttreasure.csv")
	{
		ServantTreasureTable servantTreasureTable;
		if(!servantTreasureTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servanttreasure.csv file");
			return false;
		}

		if(!servantTreasureTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servanttreasure.dbc file");
			return false;
		}
	}
	else if(fileName == "servantassisbattle.csv")
	{
		ServantAssistBattleTable servantTreasureTable;
		if(!servantTreasureTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantassisbattle.csv file");
			return false;
		}

		if(!servantTreasureTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantassisbattle.dbc file");
			return false;
		}
	}
	else if(fileName == "servantlucky.csv")
	{
		ServantLuckTable servantLuckTable;
		if(!servantLuckTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load servantlucky.csv file");
			return false;
		}

		if(!servantLuckTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to servantlucky.dbc file");
			return false;
		}
	}
	else if(fileName == "attrpower.csv")
	{
		AttrPowerTable attrPowerTable;
		if(!attrPowerTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load attrpower.csv file");
			return false;
		}

		if(!attrPowerTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to attrpower.dbc file");
			return false;
		}
	}
	else if(fileName == "herofashion.csv")
	{
		HeroFashionTable heroFashionTable;
		if(!heroFashionTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load herofashion.csv file");
			return false;
		}

		if(!heroFashionTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to herofashion.dbc file");
			return false;
		}
	}
	else if(fileName == "achievement.csv")
	{
		AchievementTable achievementTable;
		if(!achievementTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load achievement.csv file");
			return false;
		}

		if(!achievementTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to achievement.dbc file");
			return false;
		}
	}
	else if(fileName == "event.csv")
	{
		EventTable eventTable;
		if(!eventTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load event.csv file");
			return false;
		}

		if(!eventTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to event.dbc file");
			return false;
		}
	}
	else if(fileName == "dailytask.csv")
	{
		DailyTaskTable dailyTaskTable;
		if(!dailyTaskTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load dailytask.csv file");
			return false;
		}

		if(!dailyTaskTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to dailytask.dbc file");
			return false;
		}
	}
	else if(fileName == "active.csv")
	{
		DailyActiveTable dailyActiveTable;
		if(!dailyActiveTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load dailyactive.csv file");
			return false;
		}

		if(!dailyActiveTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to dailyactive.dbc file");
			return false;
		}
	}
	else if(fileName == "monsteraward.csv")
	{
		MonsterAwardTable monsterAwardTable;
		if(!monsterAwardTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load monsteraward.csv file");
			return false;
		}

		if(!monsterAwardTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to monsteraward.dbc file");
			return false;
		}
	}
	else if(fileName == "sevendaystrainning.csv")
	{
		SevenDayTrainningTable sevenDayTrainningTable;
		if(!sevenDayTrainningTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load sevendaystrainning.csv file");
			return false;
		}

		if(!sevenDayTrainningTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to sevendaystrainning.dbc file");
			return false;
		}
	}
	else if(fileName == "consortmsg.csv")
	{
		ConsortMsgTable consortMsgTable;
		if(!consortMsgTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load consortmsg.csv file");
			return false;
		}

		if(!consortMsgTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to consortmsg.dbc file");
			return false;
		}
	}
	else if(fileName == "guildlevel.csv")
	{
		ConsortLvTable consortlvTable;
		if(!consortlvTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load guildlevel.csv file");
			return false;
		}

		if(!consortlvTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to guildlevel.dbc file");
			return false;
		}
	}
	else if(fileName == "rankedshop.csv")
	{
		RankedShopTable rankedShopTable;
		if(!rankedShopTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load rankedshop.csv file");
			return false;
		}

		if(!rankedShopTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to rankedshop.dbc file");
			return false;
		}
	}
	else if(fileName == "rankreward.csv")
	{
		RankRewardTable rankRewardTable;
		if(!rankRewardTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load rankreward.csv file");
			return false;
		}

		if(!rankRewardTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to rankreward.dbc file");
			return false;
		}
	}
	else if(fileName == "systeminfo.csv")
	{
		SystemInfoTable systemInfoTable;
		if(!systemInfoTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load systeminfo.csv file");
			return false;
		}

		if(!systemInfoTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to systeminfo.dbc file");
			return false;
		}
	}
	else if(fileName == "guildsign.csv")
	{
		ConsortSignTable consortSignTable;
		if(!consortSignTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load guildsign.csv file");
			return false;
		}

		if(!consortSignTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to systeminfo.dbc file");
			return false;
		}
	}
	else if(fileName == "cattask.csv")
	{
		CatTaskTable cattaskTable;
		if(!cattaskTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load cattask.csv file");
			return false;
		}

		if(!cattaskTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to cattask.dbc file");
			return false;
		}
	}
	else if(fileName == "kitchenquestions.csv")
	{
		KitchenQuestionTable kitchenquestionTable;
		if(!kitchenquestionTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load kitchenquestions.csv file");
			return false;
		}

		if(!kitchenquestionTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to kitchenquestions.dbc file");
			return false;
		}
	}
	else if(fileName == "businesscat.csv")
	{
		BusinessCatTable businesscatTable;
		if(!businesscatTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load businesscat.csv file");
			return false;
		}

		if(!businesscatTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to businesscat.dbc file");
			return false;
		}
	}
	else if(fileName == "skillcompare.csv")
	{
		SkillConvertTable skillconvertTable;
		if(!skillconvertTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load skillcompare.csv file");
			return false;
		}

		if(!skillconvertTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to skillcompare.dbc file");
			return false;
		}
	}
	else if(fileName == "memoryquestionsgroup.csv")
	{
		EyeQuestionTable eyeQueTable;
		if(!eyeQueTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load memoryquestionsgroup.csv file");
			return false;
		}

		if(!eyeQueTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to memoryquestionsgroup.dbc file");
			return false;
		}
	}
	else if(fileName == "ticketfriend.csv")
	{
		TicketFriendTable ticketFriendTable;
		if(!ticketFriendTable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load ticketfriend.csv file");
			return false;
		}

		if(!ticketFriendTable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to ticketfriend.dbc file");
			return false;
		}
	}
	else if(fileName == "woodencat.csv")
	{
		ConsortWoodTable woodtable;
		if(!woodtable.loadFromCsv(filePath))
		{
			LOG_WARN("Failed to load woodencat.csv file");
			return false;
		}

		if(!woodtable.saveToDbc(dbcFilePath))
		{
			LOG_WARN("Failed to save to woodencat.dbc file");
			return false;
		}
	}
    else 
    {
        return false;
    }

    return true;
}


bool
DBCMaker::ConvertString(const String& fileName, unsigned int no, bool isClearMap)
{
    char buf[30] = {};
    sprintf(buf, "%d%s", no, ".csv");

    String filePath = m_Path + "/" + fileName;
    String dbcFileName = fileName;
    lynxStrReplace(dbcFileName, buf, ".dbc");
    String dbcFilePath = m_Path + "/" + dbcFileName;

    sprintf(buf, "%d%s", no, ".csv");
    String csvName("StringTable");
    csvName += buf;

    if (fileName == csvName)
    {
        //string sec×¢ÊÍ±í
		/*if (!s_StringTable.loadFromCsv(filePath ))
        {
            LOG_WARN("Failed to load StringTable.csv file.");
            return false;
        }

        if (!s_StringTable.saveToDbc(dbcFilePath))
        {
            LOG_WARN("Failed to load StringTable.csv file.");
            return false;
        }*/
    }
    
    return true;
}

bool
DBCMaker::ConvertQuestInfo(const String& fileName, unsigned int no, bool isClearMap)
{
    char buf[30] = {};
    sprintf(buf, "%d%s", no, ".csv");
    String filePath = m_Path + "/" + fileName;
    String dbcFileName = fileName;
    lynxStrReplace(dbcFileName, buf, ".dbc");
    String dbcFilePath = m_Path + "/" + dbcFileName;
    sprintf(buf, "%d%s", no, ".csv");
    String csvName("QuestInfoTemplate");
    csvName += buf;

    if (fileName == csvName)
    {
        Int32 repeatKey = 0;
		// sec ×¢ÊÍ±í
       /* if (!m_QuestInfoTemplateTable.loadFromCsv(filePath))
        {
            LOG_WARN("Failed to load StringTable.csv file.");
            return false;
        }

        if (!m_QuestInfoTemplateTable.SaveToDbc(dbcFilePath))
        {
            LOG_WARN("Failed to load StringTable.csv file.");
            return false;
        }*/
    }
    return true;
}
