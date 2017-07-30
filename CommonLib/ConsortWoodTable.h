#ifndef __LYNX_COMMON_LIB_CONSORTWOOD_TABLE_H__
#define __LYNX_COMMON_LIB_CONSORTWOOD_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ConsortWoodTemplate 
	{
		UInt32 mLv;
		UInt32 mHp;
		UInt32 mTimes;
		UInt64 mTargetAward;

		UInt32 mLimit1;
		UInt32 mLimit2;
		UInt32 mLimit3;
		UInt32 mLimit4;
		UInt32 mLimit5;

		UInt32 mDelHp1;
		UInt32 mDelHp2;
		UInt32 mDelHp3;
		UInt32 mDelHp4;
		UInt32 mDelHp5;

		UInt64 mWinAward1;
		UInt64 mWinAward2;
		UInt64 mWinAward3;
		UInt64 mWinAward4;
		UInt64 mWinAward5;

		UInt64 mLoseAward1;
		UInt64 mLoseAward2;
		UInt64 mLoseAward3;
		UInt64 mLoseAward4;
		UInt64 mLoseAward5;
	
		LYNX_S11N_24(ConsortWoodTemplate, UInt32, mLv,UInt32, mHp, UInt32,  mTimes, UInt64, mTargetAward,
					UInt32, mLimit1, UInt32, mLimit2, UInt32, mLimit3, UInt32 ,mLimit4,UInt32 ,mLimit5,
					UInt32, mDelHp1, UInt32, mDelHp2, UInt32, mDelHp3,UInt32, mDelHp4,UInt32 ,mDelHp5,
					UInt64, mWinAward1, UInt64 ,mWinAward2, UInt64, mWinAward3,UInt64, mWinAward4,UInt64, mWinAward5,
					UInt64 ,mLoseAward1, UInt64, mLoseAward2, UInt64, mLoseAward3,UInt64, mLoseAward4,UInt64, mLoseAward5
			);

	};

	typedef Map<UInt32, ConsortWoodTemplate> ConsortWoodTempMap;
	


	struct ConsortWoodTable
	{
		ConsortWoodTempMap mMap;
		
		LYNX_S11N_1(ConsortWoodTable, ConsortWoodTempMap, mMap);

		ConsortWoodTemplate* get(UInt32 id)
		{
			ConsortWoodTempMap::Iter* iter = mMap.find(id);
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




#endif