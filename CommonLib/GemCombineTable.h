#ifndef __LYNX_COMMON_LIB_GEMCOMBINE_TABLE_H__
#define __LYNX_COMMON_LIB_GEMCOMBINE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct GemCombineTemplate 
	{
		UInt64 mID;

		//合成下一级宝石，所需本级宝石的数量
		UInt32 mAmount;

		UInt64 mNextGem;

		//合成本级宝石，所需前一级宝石的数量
		UInt32 mPreAmount;

		UInt64 mPreGem;

		LYNX_S11N_5(GemCombineTemplate, UInt64, mID,UInt32, mAmount,UInt64, mPreGem, UInt64, mNextGem, UInt32, mPreAmount);

	};

	typedef Map<UInt64, GemCombineTemplate> GemCombineTemplateMap;


	struct GemCombineTable
	{
		GemCombineTemplateMap mMap;


		LYNX_S11N_1(GemCombineTable, GemCombineTemplateMap, mMap);

		GemCombineTemplate* get(UInt32 id)
		{
			GemCombineTemplateMap::Iter* iter = mMap.find(id);

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


#endif //__LYNX_COMMON_LIB_GEMCOMBINE_TABLE_H__