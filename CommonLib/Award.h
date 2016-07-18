#ifndef __LYNX_COMMON_LIB_COPY_AWARD_TABLE_H__
#define __LYNX_COMMON_LIB_COPY_AWARD_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct AwardItem
	{
		UInt32 bigtype;
		UInt32 weight;
		UInt32 awardcontent;
		LYNX_S11N_3(AwardItem,UInt32 ,bigtype,UInt32, weight,UInt32, awardcontent);
	};
	struct AwardTemplate
	{
		UInt32 id;		
		List<AwardItem> AwardItems;				
		
		AwardTemplate():id(0){}
		LYNX_S11N_2(AwardTemplate,UInt32,id,List<AwardItem>, AwardItems);
	};

	typedef List<AwardTemplate> mAwardList;

	struct AwardTable
	{	
		mAwardList mList;

		LYNX_S11N_1(AwardTable, mAwardList, mList);

		void clear()
		{
			mList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

	struct Resource
	{
		UInt32 weight;
		UInt32 resourcestype;
		UInt32 subtype;
		UInt32 num;
		LYNX_S11N_4(Resource,UInt32 ,weight,UInt32, resourcestype,UInt32, subtype,UInt32, num);
	};
	struct AwardCardTemplate
	{
		UInt32 id;		
		List<Resource> resourcesList;				

		AwardCardTemplate():id(0){}
		LYNX_S11N_2(AwardCardTemplate,UInt32,id,List<Resource>, resourcesList);
	};

	typedef List<AwardCardTemplate> mAwardCardList;

	struct AwardCardTable
	{	
		mAwardCardList mCardList;

		LYNX_S11N_1(AwardCardTable, mAwardCardList, mCardList);

		void clear()
		{
			mCardList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

	struct Content
	{
		UInt32 contenttype;
		UInt32 weight;
		UInt32 resourcestype;
		UInt32 subtype;
		UInt32 num;
		LYNX_S11N_5(Content,UInt32 ,contenttype,UInt32, weight,UInt32, resourcestype,UInt32, subtype,UInt32, num);
	};	

	struct AwardContentTemplate
	{
		UInt32 id;		
		List<Content> ContentList;				

		AwardContentTemplate():id(0){}
		LYNX_S11N_2(AwardContentTemplate,UInt32,id,List<Content>, ContentList);
	};

	typedef List<AwardContentTemplate> mAwardContentList;

	struct AwardContentTable
	{	
		mAwardContentList mContentList;

		LYNX_S11N_1(AwardContentTable, mAwardContentList, mContentList);

		void clear()
		{
			mContentList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

// 	conditions  1，每开启xx次数
// 				2，开启xx次数后
// 				3，每在特定消费上花费xx元宝
// 				4，在特定时间段、特定消费上累积花费xx元宝
// 				5，每充值xx元宝
// 				6，每充值xxRMB

	struct TypeNumValue
	{
		UInt32 uType;
		UInt32 num;
		UInt32 uValue;
		LYNX_S11N_3(TypeNumValue,UInt32, uType,UInt32,num,UInt32, uValue);
	};	
	struct LotteryTemplate
	{
		UInt32 id;		
		UInt32 firstAward;
		List<TypeNumValue> ContentList;				

		LotteryTemplate():id(0),firstAward(0){}
		LYNX_S11N_3(LotteryTemplate,UInt32,id,UInt32, firstAward,List<TypeNumValue>, ContentList);
	};
	struct LotteryTable
	{	
		List<LotteryTemplate> mContentList;

		LYNX_S11N_1(LotteryTable, List<LotteryTemplate>, mContentList);

		void clear()
		{
			mContentList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

	struct BoxTemplate
	{
		UInt32 id;		
		UInt32 firstAward;
		List<KeyValue> ContentList;				

		BoxTemplate():id(0),firstAward(0){}
		LYNX_S11N_3(BoxTemplate,UInt32,id,UInt32, firstAward,List<KeyValue>, ContentList);
	};
	
	struct BoxCounterTable
	{	
		List<BoxTemplate> mContentList;

		LYNX_S11N_1(BoxCounterTable, List<BoxTemplate>, mContentList);


		void clear()
		{
			mContentList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

	struct SkeySvalue
	{
		String sKey;
		String sValue;
		LYNX_S11N_2(SkeySvalue,String, sKey,String, sValue);
	};	
	struct SkeySvalueTemplate
	{
		String name;		
		List<SkeySvalue> SkeySvalueList;				

		SkeySvalueTemplate():name(""){}
		LYNX_S11N_2(SkeySvalueTemplate,String ,name,List<SkeySvalue>, SkeySvalueList);
	};
	struct SkeySvalueTable
	{	
		List<SkeySvalueTemplate> mSkeySvalue;

		LYNX_S11N_1(SkeySvalueTable, List<SkeySvalueTemplate>, mSkeySvalue);

		void clear()
		{
			mSkeySvalue.clear();
		}

		SkeySvalueTemplate* get(String name)
		{
			for (List<SkeySvalueTemplate>::Iter * iter = mSkeySvalue.begin();iter != NULL;iter = mSkeySvalue.next(iter))
			{
				if (iter->mValue.name == name)
				{
					return &iter->mValue;
				}
				else if (iter == mSkeySvalue.end())
				{
					return NULL;
				}
			}
			return NULL;
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

	struct VipTemplate
	{
		
		UInt32 id;
		UInt32 charge;
		UInt32 stageCard1;
		UInt32 stageCard2;
		UInt32 stageCard3;
		UInt32 stageCard4;	
		UInt32 strengthPurchase;
		UInt32 stageReset;
		UInt32 stageCleanOut10;		
		UInt32 friends;
		UInt32 love;
		UInt32 foodBuy;
		UInt32 welfaremarket;
		UInt32 trailtimes;
		UInt32 trailopenrate;
		UInt32 climtowertimes;
		UInt32 twelvePalaceBuy;
		UInt32 twelvePalaceBossBuy;
		UInt32 territorybuytimes;



		LYNX_S11N_19(VipTemplate, UInt32,id, UInt32, charge, UInt32, 	stageCard1, UInt32, 	stageCard2, UInt32, 	stageCard3,
			UInt32, stageCard4, UInt32, 	strengthPurchase, UInt32, 	stageReset, UInt32, 	stageCleanOut10, 
			 UInt32, 	friends, UInt32,love,UInt32, foodBuy,UInt32, welfaremarket,UInt32, trailtimes ,UInt32, trailopenrate,
			 UInt32, climtowertimes,UInt32, twelvePalaceBuy,UInt32, twelvePalaceBossBuy,UInt32, territorybuytimes);
	};


	struct VipTable
	{	
		List<VipTemplate> mVip;

		LYNX_S11N_1(VipTable, List<VipTemplate>, mVip);

		VipTemplate* get(UInt32 id)
		{
			for (List<VipTemplate>::Iter * iter = mVip.begin();iter != NULL;iter = mVip.next(iter))
			{
				if (iter->mValue.id == id)
				{
					return &iter->mValue;
				}
				else if (iter == mVip.end())
				{
					return NULL;
				}
			}
			return NULL;
		}



		void clear()
		{
			mVip.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};
	struct Difficulty
	{
		UInt32 levelRequireDifficulty;
		UInt32 awardDifficulty;		
		float awardChangeDifficulty;
		LYNX_S11N_3(Difficulty, UInt32,levelRequireDifficulty,UInt32,awardDifficulty, float,awardChangeDifficulty);
	};

	struct  WelfareMarketTemplate
	{
		
		UInt32 id;
		UInt32 dailyBaseTimes;
		UInt32 sweepCostRmbEachTimes;
		UInt32 Monday;
		UInt32 Tuesday;
		UInt32 Wednesday;
		UInt32 Thursday;
		UInt32 Friday;
		UInt32 Saturday;
		UInt32 Sunday;
		Difficulty difficulty1;
		Difficulty difficulty2;
		Difficulty difficulty3;
		Difficulty difficulty4;
		Difficulty difficulty5;
		Difficulty difficulty6;

		LYNX_S11N_16(WelfareMarketTemplate, UInt32,id, UInt32, dailyBaseTimes, UInt32, 	sweepCostRmbEachTimes, Difficulty, 	difficulty1, Difficulty, 	difficulty2,
			Difficulty, difficulty3, Difficulty, 	difficulty4, Difficulty, 	difficulty5, Difficulty, 	difficulty6, UInt32, 	Monday,
			UInt32, 	Tuesday, UInt32, 	Wednesday, UInt32,Thursday,UInt32, Friday,UInt32, Saturday,UInt32, Sunday  );
	};

	struct WelfareMarketTable
	{	
		List<WelfareMarketTemplate> mWelfareMarket;

		LYNX_S11N_1(WelfareMarketTable, List<WelfareMarketTemplate>, mWelfareMarket);

		void clear()
		{
			mWelfareMarket.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};


} // namespace Lynx

#endif // __LYNX_COMMON_LIB_COPY_AWARD_TABLE_H__


