#ifndef __LYNX_COMMON_LIB_SHOP_TABLE_H__
#define __LYNX_COMMON_LIB_SHOP_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"
#include "Award.h"

namespace Lynx
{
	struct CommodityTemplate 
	{

		UInt32 id;

		UInt32 resourcesType1;

		UInt32 subtype1;

		UInt32 num1;	

		UInt32 costype1;

		UInt32 costsubtype1;

		UInt32 costnumber1;	

		LYNX_S11N_7(CommodityTemplate,	UInt32, id,	UInt32, resourcesType1,	UInt32, subtype1,UInt32, num1,UInt32, costype1,UInt32, costsubtype1,UInt32, costnumber1	);
	};

	typedef Map<UInt32, CommodityTemplate> CommodityTemplateMap;


	struct CommodityTable
	{
		CommodityTemplateMap mMap;


		LYNX_S11N_1(CommodityTable, CommodityTemplateMap, mMap);

		CommodityTemplate* get(UInt32 id)
		{
			CommodityTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};


	struct GiftTemplate 
	{
		UInt32 giftid;

		String name;

		UInt32 gifttype;

		List<UInt32> packageIDs;

		List<UInt32> awardcontentshopIDs;

		LYNX_S11N_5(GiftTemplate,UInt32, giftid,String, name,UInt32, gifttype,List<UInt32>, packageIDs,List<UInt32>, awardcontentshopIDs);

	};

	typedef Map<UInt32, GiftTemplate> GiftTemplateMap;


	struct GiftTable
	{
		GiftTemplateMap mMap;

		LYNX_S11N_1(GiftTable, GiftTemplateMap, mMap);

		GiftTemplate* get(UInt32 id)
		{
			GiftTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};


	struct GiftShopTemplate 
	{

		UInt32 ID;

		String name;

		UInt32 giftID;

		UInt32 giftquality;	
		UInt32 gifttype;
		UInt32 canbuyNum;
		UInt32 qualification;
		UInt32 program1;
		UInt32 costtype;
		UInt32 secondarytype;
		UInt32 costNum;
		String giftshow;
		UInt32 favorable;
		String whichday;
		UInt32 starttime;
		UInt32 endtime;


		LYNX_S11N_16(GiftShopTemplate,UInt32, ID,String, name,UInt32, giftID,UInt32, giftquality,UInt32, gifttype,UInt32, canbuyNum,
			UInt32, qualification,UInt32, program1,UInt32, costtype,UInt32, secondarytype,UInt32, costNum,String, giftshow,UInt32, favorable,String, whichday,
			UInt32, starttime,UInt32, endtime );

	};

	typedef Map<UInt32, GiftShopTemplate> GiftShopMap;


	struct GiftShopTable
	{
		GiftShopMap mMap;


		LYNX_S11N_1(GiftShopTable, GiftShopMap, mMap);

		GiftShopTemplate* get(UInt32 id)
		{
			GiftShopMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};

	struct MysticalshopTemplate 
	{
		UInt32 ID;

		UInt32 shopposition;

		UInt32 vipLv;

		UInt32 playerLv;	

		String refreshTime;

		UInt32 timeawardcontentshop;

		List<UInt32> awardcontentshops;

		LYNX_S11N_7(MysticalshopTemplate,UInt32, ID,	UInt32, shopposition,UInt32, vipLv,UInt32, playerLv,String, refreshTime,UInt32, timeawardcontentshop,List<UInt32>, awardcontentshops);

	};

	typedef Map<UInt32, MysticalshopTemplate> MysticalshopTemplateMap;


	struct MysticalshopTable
	{
		MysticalshopTemplateMap mMap;


		LYNX_S11N_1(MysticalshopTable, MysticalshopTemplateMap, mMap);

		MysticalshopTemplate* get(UInt32 id)
		{
			MysticalshopTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};


	struct ServantshopTemplate 
	{
		UInt32 ID;

		UInt32 shopposition;

		UInt32 vipLv;

		UInt32 playerLv;	

		String refreshTime;

		UInt32 timeawardcontentshop;

		List<UInt32> awardcontentshops;

		LYNX_S11N_7(ServantshopTemplate,UInt32, ID,	UInt32, shopposition,UInt32, vipLv,UInt32, playerLv,String, refreshTime,UInt32, timeawardcontentshop,List<UInt32>, awardcontentshops);

	};

	typedef Map<UInt32, ServantshopTemplate> ServantshopTemplateMap;


	struct ServantshopTable
	{
		ServantshopTemplateMap mMap;


		LYNX_S11N_1(ServantshopTable, ServantshopTemplateMap, mMap);

		ServantshopTemplate* get(UInt32 id)
		{
			ServantshopTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};


	struct AwardContentShopTable
	{	
		mAwardContentList mContentList;

		LYNX_S11N_1(AwardContentShopTable, mAwardContentList, mContentList);

		void clear()
		{
			mContentList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};

	struct GainStrengthTemplate 
	{

		UInt32 viplevel;

		UInt32 buytimes;

		UInt32 cost;

		UInt32 addstrength;	
	

		LYNX_S11N_4(GainStrengthTemplate,UInt32, viplevel,	UInt32, buytimes,UInt32, cost,UInt32, addstrength);

	};


	struct GainStrengthTable
	{
		List<GainStrengthTemplate> mMap;


		LYNX_S11N_1(GainStrengthTable, List<GainStrengthTemplate>, mMap);

		
		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};

	struct GainWealthTemplate 
	{
		UInt32 VIPlevel;

		UInt32 times;

		UInt32 cost;

		UInt32 gaincoin;	

		LYNX_S11N_4(GainWealthTemplate,UInt32, VIPlevel,	UInt32, times,UInt32, cost,UInt32, gaincoin);

	};


	struct GainWealthTable
	{
		List<GainWealthTemplate> mMap;


		LYNX_S11N_1(GainWealthTable, List<GainWealthTemplate>, mMap);

		

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};


	struct PremiumsTemplate 
	{

		UInt32 ID;

		UInt32 type;

		String contentID;

		LYNX_S11N_3(PremiumsTemplate,UInt32, ID,UInt32, type,String,contentID);

	};

	typedef Map<UInt32, PremiumsTemplate>PremiumsTemplateMap;


	struct PremiumsTable
	{
		PremiumsTemplateMap mMap;


		LYNX_S11N_1(PremiumsTable, PremiumsTemplateMap, mMap);

		PremiumsTemplate* get(UInt32 id)
		{
			PremiumsTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};




} // namespace Lynx

#endif // __LYNX_COMMON_LIB_SHOP_TABLE_H__