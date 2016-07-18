#ifndef __LYNX_COMMON_LIB_COURAGECHALLENGE_TABLE_H__
#define __LYNX_COMMON_LIB_COURAGECHALLENGE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"
#include "Award.h"

namespace Lynx
{
	struct CoinTemplate 
	{

		UInt32 id;

		UInt32 monsterid;

		UInt32 light;

		UInt32 quality;

		UInt32 territory;

		LYNX_S11N_5(CoinTemplate,UInt32, id,UInt32, monsterid,UInt32, light,UInt32, quality,UInt32, territory);

	};

	typedef Map<UInt64, CoinTemplate> CoinTemplateMap;


	struct CoinTable
	{
		CoinTemplateMap mMap;

		LYNX_S11N_1(CoinTable, CoinTemplateMap, mMap);

		CoinTemplate* get(UInt32 id)
		{
			CoinTemplateMap::Iter* iter = mMap.find(id);

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




	struct CoinGroupTemplate 
	{
		UInt32 id;

		UInt32 territoryID;

		String des;

		UInt32 attr1;

		UInt32 percent1;

		UInt32 coin1;

		UInt32 num1;

		UInt32 coin2;

		UInt32 num2;

		UInt32 coin3; 

		UInt32 num3;

		LYNX_S11N_11(CoinGroupTemplate,UInt32,id,UInt32,territoryID,String,des,UInt32,attr1,UInt32,	percent1,UInt32,coin1,UInt32,num1,UInt32,coin2,UInt32,num2,UInt32,coin3,UInt32,num3);

	};

	typedef Map<UInt64, CoinGroupTemplate> CoinGroupTemplateMap;


	struct CoinGroupTable
	{
		CoinGroupTemplateMap mMap;

		LYNX_S11N_1(CoinGroupTable, CoinGroupTemplateMap, mMap);

		CoinGroupTemplate* get(UInt32 id)
		{
			CoinGroupTemplateMap::Iter* iter = mMap.find(id);

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

	struct CourageContentTable
	{
		mAwardContentList mContentList;

		LYNX_S11N_1(CourageContentTable, mAwardContentList, mContentList);

		void clear()
		{
			mContentList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};




	struct TerritoryTemplate 
	{
		UInt32 id;

		String name;

		UInt32 condition1;

		UInt32 condition2;

		UInt32 couragemonster;

		List<KeyValue> conditionMonsters;

		LYNX_S11N_6(TerritoryTemplate,UInt32, id,String, name,UInt32, condition1,UInt32, condition2,UInt32, couragemonster,List<KeyValue>, conditionMonsters);

	};

	typedef Map<UInt64, TerritoryTemplate> TerritoryTemplateMap;


	struct TerritoryTable
	{
		TerritoryTemplateMap mMap;

		LYNX_S11N_1(TerritoryTable, TerritoryTemplateMap, mMap);

		TerritoryTemplate* get(UInt32 id)
		{
			TerritoryTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_COURAGECHALLENGE_TABLE_H__