#ifndef __LYNX_COMMON_LIB_CHAPTER_TABLE_H__
#define __LYNX_COMMON_LIB_CHAPTER_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ChapterTemplate 
	{
		UInt64 mId;
		String mName;
		String mDesc;
		String mBgPicture;
		
		UInt32 mStarAwardCount1;
		UInt32 mStarAwardType1;
		UInt32 mStarAwardContent1;
		
		UInt32 mStarAwardCount2;
		UInt32 mStarAwardType2;
		UInt32 mStarAwardContent2;

		UInt32 mStarAwardCount3;
		UInt32 mStarAwardType3;
		UInt32 mStarAwardContent3;

		String mStrStageList;

		UInt32 mStarTotalCount;

		LYNX_S11N_15(ChapterTemplate, UInt64, mId, String, mName,String, mDesc,
			String, mBgPicture, UInt32 ,mStarAwardCount1, UInt32 ,mStarAwardType1,
			UInt32 ,mStarAwardContent1,UInt32,mStarAwardCount2, UInt32,mStarAwardType2,
			UInt32,mStarAwardContent2, UInt32 ,mStarAwardCount3,UInt32, mStarAwardType3,
			UInt32, mStarAwardContent3, String, mStrStageList, UInt32 , mStarTotalCount
			);

	};

	typedef Map<UInt64, ChapterTemplate> ChapterTemplateMap;


	struct ChapterTable
	{
		ChapterTemplateMap mMap;


		LYNX_S11N_1(ChapterTable, ChapterTemplateMap, mMap);

		ChapterTemplate* get(UInt32 id)
		{
			ChapterTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_CHAPTER_TABLE_H__