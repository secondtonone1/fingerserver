#ifndef __LYNX_COMMON_LIB_CONSORTLV_TABLE_H__
#define __LYNX_COMMON_LIB_CONSORTLV_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ConsortLvTemplate 
	{
		UInt32 mId;
		UInt64 mExp;
		UInt32 mCount;
		UInt32 mActive;
		String mStrActiveAward1;
		String mStrActiveAward2;
		String mStrActiveAward3;
		String mStrActiveAward4;
		String mStrActiveAward5;
		//行侠仗义次数
		UInt32 mXingManDo;
		//行侠仗义开启的副本
		String mXingXiaStage;

		UInt32 mActivePoint1;
		UInt64 mActiveAward1;
		
		UInt32 mActivePoint2;
		UInt64 mActiveAward2;

		UInt32 mActivePoint3;
		UInt64 mActiveAward3;

		UInt32 mActivePoint4;
		UInt64 mActiveAward4;

		UInt32 mKitchentimes;
		String mStrKitchenQueStr;
		List<UInt32> kitchenQueList;

		UInt32 mBusinessId;
		UInt32 mBusinessTimes;

		String mStrEyeQuestr;
		List<UInt32 > mEyeQueList;

		UInt32 mTicketTimes;
		UInt32 mfriendlyTimes;

		UInt64 mEloquencewin;
		UInt64 mEloquencelose;
		UInt32 mEloquenTimes;
		UInt32 mEyeSightTimes;
		
		LYNX_S11N_32(ConsortLvTemplate, UInt32, mId,UInt64, mExp, UInt32, mCount,UInt32, mActive,
			String, mStrActiveAward1, String, mStrActiveAward2, String, mStrActiveAward3, String, mStrActiveAward4,
			String, mStrActiveAward5, UInt32, mXingManDo, String, mXingXiaStage, UInt32, mActivePoint1,UInt64, mActiveAward1,
			UInt32, mActivePoint2, UInt64, mActiveAward2, UInt32, mActivePoint3,UInt64, mActiveAward3, UInt32, mActivePoint4,
			UInt64, mActiveAward4, UInt32,  mKitchentimes, String,  mStrKitchenQueStr, List<UInt32>, kitchenQueList,
			UInt32, mBusinessId, UInt32, mBusinessTimes,	String ,mStrEyeQuestr,List<UInt32 >, mEyeQueList,UInt32 ,mTicketTimes,
			UInt32, mfriendlyTimes, 	UInt64, mEloquencewin,UInt64, mEloquencelose, UInt32, mEloquenTimes,UInt32 ,mEyeSightTimes
			);

	};

	typedef Map<UInt32, ConsortLvTemplate> ConsortLvTemplateMap;
	


	struct ConsortLvTable
	{
		ConsortLvTemplateMap mMap;
		

		LYNX_S11N_1(ConsortLvTable, ConsortLvTemplateMap, mMap);

		ConsortLvTemplate* get(UInt32 id)
		{
			ConsortLvTemplateMap::Iter* iter = mMap.find(id);
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