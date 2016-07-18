#ifndef __LYNX_COMMON_LIB_GEMCOMBINEONCE_TABLE_H__
#define __LYNX_COMMON_LIB_GEMCOMBINEONCE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct GemIdAndCount
	{
		UInt64 mID;
		UInt32 mCount;
		LYNX_S11N_2(GemIdAndCount, UInt64, mID,UInt32, mCount);

	};
	
	struct GemCombineOnceTemplate 
	{
		
		UInt64 mID;

		UInt32 mType;

		UInt32 mLevel;

		UInt32 mCount;

		LYNX_S11N_4(GemCombineOnceTemplate, UInt64, mID,UInt32, mType,UInt32, mLevel,UInt32, mCount);

	};

	typedef Map<UInt64, GemCombineOnceTemplate> GemCombineOnceTemplateMap;
	typedef Map<UInt32,Map<UInt32,GemIdAndCount> > GemCombineOnceTypeLvMap;

	struct GemCombineOnceTable
	{
		GemCombineOnceTemplateMap mMap;
		GemCombineOnceTypeLvMap mTypeLvMap;

		LYNX_S11N_2(GemCombineOnceTable, GemCombineOnceTemplateMap, mMap,GemCombineOnceTypeLvMap,mTypeLvMap);

		GemCombineOnceTemplate* get(UInt32 id)
		{
			GemCombineOnceTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		GemIdAndCount getIdAndCount(UInt32 type, UInt32 level)
		{
			GemCombineOnceTypeLvMap::Iter * typeLvIter = mTypeLvMap.find(type);
			if(typeLvIter)
			{
				Map<UInt32,GemIdAndCount>::Iter * gemIdCountIter = typeLvIter->mValue.find(level);
				
				if(gemIdCountIter)
				{
					return gemIdCountIter->mValue;
				}
				else
				{
					LOG_WARN("not found id and count");
					GemIdAndCount gemIdAndCount;
					gemIdAndCount.mCount = 0;
					gemIdAndCount.mID = 0;
					return gemIdAndCount; 
				}
				
			}
			else
			{
				LOG_WARN("not found id and count");
				GemIdAndCount gemIdAndCount;
				gemIdAndCount.mCount = 0;
				gemIdAndCount.mID = 0;
				return gemIdAndCount;
			}
		}

		void clear()
		{
			mMap.clear();
			mTypeLvMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx


#endif //__LYNX_COMMON_LIB_GEMCOMBINEONCE_TABLE_H__