#ifndef __LYNX_COMMON_LIB_REWARD_LOTTERY_TABLE_H__
#define __LYNX_COMMON_LIB_REWARD_LOTTERY_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct LotteryActivityTemplate 
	{

		UInt32 id;

		UInt32 starttime;

		UInt32 closetime;

		UInt32 lotteryid;	

		LYNX_S11N_4(LotteryActivityTemplate,UInt32, id,UInt32, starttime,UInt32, closetime,UInt32, lotteryid);

	};

	typedef Map<UInt32, LotteryActivityTemplate> LotteryActivityTemplateMap;


	struct LotteryActivityTable
	{
		LotteryActivityTemplateMap mMap;


		LYNX_S11N_1(LotteryActivityTable, LotteryActivityTemplateMap, mMap);

		LotteryActivityTemplate* get(UInt32 id)
		{
			LotteryActivityTemplateMap::Iter* iter = mMap.find(id);

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

	struct RewardLotteryTemplate 
	{

		UInt32 id;

		UInt32 type;

		UInt32 refreshtime;

		UInt32 opentime;

		UInt32 cost;

		UInt32 vipneed;

		UInt32 nextbestneedtimes;

		UInt32 normalaward;

		UInt32 bestaward;

		List<KeyValue> keyValues;

		LYNX_S11N_10(RewardLotteryTemplate,UInt32, id,UInt32, type,UInt32, refreshtime,UInt32, opentime,UInt32, cost,
			UInt32, vipneed,UInt32, nextbestneedtimes,UInt32, normalaward,UInt32, bestaward,List<KeyValue>, keyValues
			);

	};

	typedef Map<UInt32, RewardLotteryTemplate> RewardLotteryTemplateMap;


	struct RewardLotteryTable
	{
		RewardLotteryTemplateMap mMap;


		LYNX_S11N_1(RewardLotteryTable, RewardLotteryTemplateMap, mMap);

		RewardLotteryTemplate* get(UInt32 id)
		{
			RewardLotteryTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_REWARD_LOTTERY_TABLE_H__