#ifndef __LYNX_COMMON_LIB_DAILYACTIVE_TABLE_H__
#define __LYNX_COMMON_LIB_DAILYACTIVE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct DailyActiveTemplate 
	{
		UInt64 mId;

		UInt32 mActive;
		
		String mAward;
	
		List<AllItemEle> mItemList;

		LYNX_S11N_4(DailyActiveTemplate, String, mAward,List<AllItemEle>, mItemList, UInt32, mActive, UInt64, mId
			);

	};

	typedef Map<UInt64, DailyActiveTemplate> DailyActiveTemplateMap;



	struct DailyActiveTable
	{
		DailyActiveTemplateMap mMap;


		LYNX_S11N_1(DailyActiveTable, DailyActiveTemplateMap, mMap);

		DailyActiveTemplate* get(UInt64 id)
		{
			DailyActiveTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_DAILYACTIVE_TABLE_H__