#ifndef __LYNX_COMMON_LIB_COPY_GLOBAL_VAR_TABLE_H__
#define __LYNX_COMMON_LIB_COPY_GLOBAL_VAR_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct GlobalTemplate
	{					
		String sKey;
		String sValue;
		LYNX_S11N_2(GlobalTemplate,String, sKey,String, sValue);
	};

	typedef List<GlobalTemplate> mGlobalList;

	struct GlobalVarTable
	{	
		mGlobalList mList;
		LYNX_S11N_1(GlobalVarTable, mGlobalList, mList);

		void clear()
 		{
			mList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

	struct MeiShiWuTemplate
	{	
		UInt32 weight1;
		UInt32 weight2;
		UInt32 weight3;
		UInt32 max;
		UInt32 recover;
		UInt32 eachBuy;
		UInt32 price;
		UInt32 time;
		UInt32 rate1;
		UInt32 rate2;
		UInt32 rate3;

		LYNX_S11N_11(MeiShiWuTemplate,UInt32, weight1,UInt32, weight2,UInt32, weight3,UInt32, max,UInt32, recover,UInt32, eachBuy,UInt32, price,UInt32, time,UInt32, rate1,UInt32, rate2,UInt32, rate3);
	};

	struct StrengthTemplate
	{	
	
		UInt32 max;
		UInt32 recover;
		UInt32 eachBuy;
		UInt32 time;
		UInt32 price1;
		UInt32 price2;
		UInt32 price3;
		UInt32 price4;
		UInt32 price5;
		UInt32 price6;
		UInt32 price7;
		UInt32 price8;
		UInt32 price9;
		UInt32 price10;

		LYNX_S11N_14(StrengthTemplate,UInt32, max,UInt32, recover,UInt32, eachBuy,UInt32,time,UInt32, price1,UInt32, price2,
		UInt32, price3,UInt32, price4,UInt32, price5,UInt32, price6,UInt32, price7,UInt32, price8,UInt32, price9,UInt32, price10);
	};

	struct SaoDangTemplate
	{	
		
		UInt32 rate;
		UInt32 sweeptimes;
		UInt32 sweepfreetimes;
		UInt32 amount;
		
		List<UInt32> prices;

		LYNX_S11N_5(SaoDangTemplate,UInt32, rate,UInt32, sweeptimes,UInt32, sweepfreetimes,UInt32, amount,List<UInt32>, prices);
	};

	struct EliteBuffTemplate
	{	
		List<UInt32> buffIDs;

		LYNX_S11N_1(EliteBuffTemplate,List<UInt32>, buffIDs);
	};
	struct NormalBuffTemplate
	{	
		List<UInt32> buffIDs;

		LYNX_S11N_1(NormalBuffTemplate,List<UInt32>, buffIDs);
	};
	struct SpecialBuffTemplate
	{	
		List<UInt32> buffIDs;

		LYNX_S11N_1(SpecialBuffTemplate,List<UInt32>, buffIDs);
	};


	struct BonusBuffTemplate
	{	
		List<UInt32> buffIDs;

		LYNX_S11N_1(BonusBuffTemplate,List<UInt32>, buffIDs);
	};
	struct MasterBuffTemplate
	{	
		List<UInt32> buffIDs;

		LYNX_S11N_1(MasterBuffTemplate,List<UInt32>, buffIDs);
	};
	struct TrailBuffTemplate
	{	
		List<UInt32> buffIDs;

		LYNX_S11N_1(TrailBuffTemplate,List<UInt32>, buffIDs);
	};

	struct NormalReliveTemplate
	{	
		UInt32 dayfreetimes;
		UInt32 maxtimes;
		List<UInt32> costs;

		LYNX_S11N_3(NormalReliveTemplate,UInt32, dayfreetimes,UInt32,maxtimes,List<UInt32>, costs);
	};
	struct EliteReliveTemplate
	{	
		UInt32 dayfreetimes;
		UInt32 maxtimes;
		List<UInt32> costs;

		LYNX_S11N_3(EliteReliveTemplate,UInt32, dayfreetimes,UInt32,maxtimes,List<UInt32>, costs);
	};
	struct SpecialReliveTemplate
	{	
		UInt32 dayfreetimes;
		UInt32 maxtimes;
		List<UInt32> costs;

		LYNX_S11N_3(SpecialReliveTemplate,UInt32, dayfreetimes,UInt32,maxtimes,List<UInt32>, costs);
	};
	struct BonusReliveTemplate
	{	
		UInt32 dayfreetimes;
		UInt32 maxtimes;
		List<UInt32> costs;

		LYNX_S11N_3(BonusReliveTemplate,UInt32, dayfreetimes,UInt32,maxtimes,List<UInt32>, costs);
	};
	
	struct MasterReliveTemplate
	{	
		UInt32 dayfreetimes;
		UInt32 maxtimes;
		List<UInt32> costs;

		LYNX_S11N_3(MasterReliveTemplate,UInt32, dayfreetimes,UInt32,maxtimes,List<UInt32>, costs);
	};


	struct TrialTemplate
	{	
		UInt32 dayfreetimes;
		UInt32 relivemaxtimes;
		Map<UInt32,UInt32> costs;		
		UInt32 player2dayfreetimes;
		UInt32 reliveplayer2maxtimes;
		Map<UInt32,UInt32> player2costs;
		Map<UInt32,UInt32> ratecost;

// 		LYNX_S11N_7(TrialTemplate,UInt32, totaltimes,UInt32, dayfreetimes,UInt32, relivemaxtimes,Map<UInt32,UInt32>, costs,	
// 		UInt32, player2dayfreetimes,UInt32, reliveplayer2maxtimes,Map<UInt32,UInt32>, player2cost);
	};

	struct TwelvePalaceTemplate
	{	
		UInt32 dailyreset;
		UInt32 needitemid;
		UInt32 canresetitem;
		UInt32 resetlowerlimit;
		UInt32 buycost;
		UInt32 buybosscost;

		LYNX_S11N_6(TwelvePalaceTemplate,UInt32, dailyreset,UInt32, needitemid,UInt32, canresetitem,UInt32, resetlowerlimit,UInt32, buycost,UInt32, buybosscost);
	};

	struct ClimbTowerTemplate
	{	
		UInt32 dailyreset;
		UInt32 needitemid;
		UInt32 canresetitem;
		UInt32 resetlowerlimit;
		UInt32 buycost;

		LYNX_S11N_5(ClimbTowerTemplate,UInt32, dailyreset,UInt32, needitemid,UInt32, canresetitem,UInt32, resetlowerlimit,UInt32,buycost);
	};

	struct MysticalShopCostTemplate
	{	
		
		List<UInt32> refreshTimeList;

		LYNX_S11N_1(MysticalShopCostTemplate,List<UInt32>, refreshTimeList);
	};

	struct ServantShopCostTemplate
	{	
		List<UInt32> refreshTimeList;

	
		LYNX_S11N_1(ServantShopCostTemplate,List<UInt32>, refreshTimeList);
	};

	struct GainWealth
	{	

		UInt32 freeTimes;

		LYNX_S11N_1(GainWealth,UInt32, freeTimes);
	};

	struct GrowthFoundTemplate
	{	

		UInt32 viplvNeed;
		UInt32 cost;

		LYNX_S11N_2(GrowthFoundTemplate,UInt32, viplvNeed,UInt32, cost);
	};

	struct TerritoryLuckyTemplate
	{	

		UInt32 maxtimes;
		UInt32 recovertime;
		UInt32 freerefresh;
		UInt32 freerefreshtimes;
		UInt32 challage;
		List<UInt32> refreshs;
		List<UInt32> costs;
		List<UInt32> buytimess;	
		UInt32 rmbrefresh;

		LYNX_S11N_9(TerritoryLuckyTemplate,UInt32, maxtimes,UInt32, recovertime,UInt32, freerefresh,List<UInt32>, refreshs,UInt32, challage,UInt32, freerefreshtimes,
		List<UInt32>, costs,List<UInt32>, buytimess,UInt32, rmbrefresh);
	};

	class GlobalVarManager: public Singleton<GlobalVarManager>
	{
	public:

		 UInt32 initial();

		 UInt32 initMeiShiWu();
		 UInt32 initStrength();
		 UInt32 initSaoDang();
		 UInt32 initnormalrelive();
		 UInt32 initeliterelive();
		 UInt32 initspecialrelive();
		 UInt32 initbonusrelive();
		 UInt32 initmasterrelive();
		 UInt32 inittrial();
		 UInt32 initbonusbuff();
		 UInt32 initmasterbuff();
		 UInt32 inittrailbuff();
		 UInt32 initnormalbuff();
		 UInt32 initelitebuff();
		 UInt32 initspecialbuff();
		 UInt32 inittwelvepalace();		 
		 UInt32 initclimbtower();		 
		 UInt32 initservantshopcost();		 
		 UInt32 initmysticalshopcost();		
		 UInt32 initgainwealth();		 
		 UInt32 initterritorylucky();
		 UInt32 initGrowthFound();
		 

		SaoDangTemplate getSaoDang()
		{
			return saoDangTemplate;
		}
		MeiShiWuTemplate getMeiShiWu()
		{
			return meiShiWuTemplate;
		}
		StrengthTemplate getStrength()
		{
			return strengthTemplate;
		}
		SpecialReliveTemplate getSpecialRelive()
		{
			return specialReliveTemplate;
		}
		EliteReliveTemplate getEliteRelive()
		{
			return eliteReliveTemplate;
		}
		NormalReliveTemplate getNormalRelive()
		{
			return normalReliveTemplate;
		}
		SpecialBuffTemplate getSpecialBuff()
		{
			return specialBuffTemplate;
		}
		EliteBuffTemplate getEliteBuff()
		{
			return eliteBuffTemplate;
		}
		NormalBuffTemplate getNormalBuff()
		{
			return normalBuffTemplate;
		}

		TrialTemplate& gettrial()
		{
			return trialTemplate;
		}

		BonusBuffTemplate getbonusBuff()
		{
			return bonusBuffTemplate;
		}

		MasterBuffTemplate getmasterBuff()
		{
			return masterBuffTemplate;
		}

		TrailBuffTemplate gettrailBuff()
		{
			return trailBuffTemplate;
		}
		BonusReliveTemplate getbonusRelive()
		{
			return bonusReliveTemplate;
		}
		MasterReliveTemplate getmasterRelive()
		{
			return masterReliveTemplate;
		}

		TwelvePalaceTemplate getTwelvePalace()
		{
			return twelvePalaceTemplate;
		}

		ClimbTowerTemplate getclimbtower()
		{
			return climbTowerTemplate;
		}

		MysticalShopCostTemplate getMysticalShopCost()
		{
			return mysticalShopCostTemplate;
		}

		ServantShopCostTemplate getServantShopCost()
		{
			return servantShopCostTemplate;
		}
		GainWealth getGainWealth()
		{
			return gainWealth;
		}

		TerritoryLuckyTemplate getTerritoryLucky()
		{
			return territoryLuckyTemplate;
		}

		GrowthFoundTemplate getGrowthFound()
		{
			return growthFoundTemplate;
		}

	private:
		MeiShiWuTemplate meiShiWuTemplate;

		StrengthTemplate strengthTemplate;
		
		SaoDangTemplate saoDangTemplate;

		SpecialReliveTemplate specialReliveTemplate;

		EliteReliveTemplate eliteReliveTemplate;

		NormalReliveTemplate normalReliveTemplate;

		SpecialBuffTemplate specialBuffTemplate;

		EliteBuffTemplate eliteBuffTemplate;

		NormalBuffTemplate normalBuffTemplate;

		TrialTemplate trialTemplate;

		BonusBuffTemplate bonusBuffTemplate;
		
		MasterBuffTemplate masterBuffTemplate;
			
		TrailBuffTemplate trailBuffTemplate;
		
		BonusReliveTemplate bonusReliveTemplate;
		
		MasterReliveTemplate masterReliveTemplate;

		TwelvePalaceTemplate twelvePalaceTemplate;

		ClimbTowerTemplate climbTowerTemplate;

		MysticalShopCostTemplate mysticalShopCostTemplate;

		ServantShopCostTemplate servantShopCostTemplate;

		GainWealth gainWealth;

		TerritoryLuckyTemplate territoryLuckyTemplate;

		GrowthFoundTemplate growthFoundTemplate;

	};


} // namespace Lynx

#endif // __LYNX_COMMON_LIB_COPY_GLOBAL_VAR_TABLE_H__


