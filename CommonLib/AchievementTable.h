#ifndef __LYNX_COMMON_LIB_ACHIEVEMENT_TABLE_H__
#define __LYNX_COMMON_LIB_ACHIEVEMENT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct AchievementTemplate 
	{
		UInt64 mId;
		UInt64 mEvent;
		UInt32 mArg;
		String mAward;
		UInt64 mNextId;
		UInt64 mPreId;
		List<AllItemEle> mItemList;
		
		LYNX_S11N_7(AchievementTemplate, UInt64, mId, UInt64, mEvent,UInt32, mArg,
				String, mAward,UInt64, mNextId,List<AllItemEle>, mItemList,UInt64, mPreId
			);

	};

	typedef Map<UInt64, AchievementTemplate> AchievementTemplateMap;
	typedef Map<UInt64,  AchievementTemplate> FirstAchievementMap;


	struct AchievementTable
	{
		AchievementTemplateMap mMap;
		FirstAchievementMap  mFirstAchMap;

		LYNX_S11N_2(AchievementTable, AchievementTemplateMap, mMap, FirstAchievementMap,  mFirstAchMap);

		AchievementTemplate* get(UInt64 id)
		{
			AchievementTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		AchievementTemplate* getFirstAch(UInt64 eventid)
		{
			FirstAchievementMap::Iter* iter = mFirstAchMap.find(eventid);
			if (iter == mFirstAchMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		const FirstAchievementMap& getFirstAchMap()
		{
			return mFirstAchMap;
		}

		void clear()
		{
			mMap.clear();
			mFirstAchMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ACHIEVEMENT_TABLE_H__