#ifndef __LYNX_COMMON_LIB_RANKGAME_TABLE_H__
#define __LYNX_COMMON_LIB_RANKGAME_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"


namespace Lynx
{
	struct RankedShopTemplate 
	{

		UInt32 id;

		UInt32 resType;

		UInt32 subType;

		UInt32 num;

		UInt32 costresType;

		UInt32 costsubType;

		UInt32 costnum;

		LYNX_S11N_7(RankedShopTemplate,UInt32, id,UInt32, resType,UInt32, subType,UInt32, num,UInt32, costresType,UInt32, costsubType,UInt32, costnum);

	};

	typedef Map<UInt64, RankedShopTemplate> RankedShopTemplateMap;


	struct RankedShopTable
	{
		RankedShopTemplateMap mMap;

		LYNX_S11N_1(RankedShopTable, RankedShopTemplateMap, mMap);

		RankedShopTemplate* get(UInt32 id)
		{
			RankedShopTemplateMap::Iter* iter = mMap.find(id);

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



	struct RankRewardTemplate 
	{

		UInt32 id;

		UInt32 coin;

		UInt32 gold;

		UInt32 score;

		UInt32 min;

		UInt32 max;

		UInt32 type;

		UInt32 descIndex;
		

		LYNX_S11N_8(RankRewardTemplate,UInt32, id,UInt32, coin,UInt32, gold,UInt32, score,UInt32, min,UInt32, max,UInt32, type,UInt32, descIndex);

	};

	typedef Map<UInt64, RankRewardTemplate> RankRewardTemplateMap;


	struct RankRewardTable
	{
		RankRewardTemplateMap mMap;

		LYNX_S11N_1(RankRewardTable, RankRewardTemplateMap, mMap);

		RankRewardTemplate* get(UInt32 id)
		{
			RankRewardTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_RANKGAME_TABLE_H__