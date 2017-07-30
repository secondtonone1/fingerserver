#ifndef  __LYNX_COMMON_LIB_CATTASK_TABLE_H__
#define __LYNX_COMMON_LIB_CATTASK_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct CatTaskTemplate 
	{
		UInt32 mId;
		UInt32 mGuildLv;
		UInt32 mTime;
		UInt32 mRate;
		String mAward1;
		String mAward2;

		UInt64 mType1;
		UInt64 mSubType1;
		UInt32 mCount1;

		UInt64 mType2;
		UInt64 mSubType2;
		UInt32 mCount2;
		
		LYNX_S11N_12(CatTaskTemplate, UInt32, mId, UInt32 ,mGuildLv, UInt32 ,mTime,
			UInt32, mRate,String, mAward1, String, mAward2, 
			UInt64, mType1, UInt64 ,mSubType1, UInt32, mCount1,
			UInt64, mType2,  UInt64,  mSubType2, UInt32, mCount2);

	};

	typedef Map<UInt32, CatTaskTemplate> CatTaskTemplateMap;
	typedef Map<UInt32, List<CatTaskTemplate > > LvTaskTemplateMap;


	struct CatTaskTable
	{
		CatTaskTemplateMap mMap;
		LvTaskTemplateMap mLvMap;

		LYNX_S11N_2(CatTaskTable, CatTaskTemplateMap, mMap, LvTaskTemplateMap ,mLvMap);

		CatTaskTemplate* get(UInt32 id)
		{
			CatTaskTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		List<CatTaskTemplate > *getTaskList(UInt32 level)
		{
			Map<UInt32, List<CatTaskTemplate > >::Iter * findList = mLvMap.find(level);
			if(!findList)
			{
				return NULL;
			}

			return &findList->mValue;
		}
		

		void clear()
		{
			mMap.clear();
			mLvMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx




#endif //__LYNX_COMMON_LIB_CATTASK_TABLE_H__