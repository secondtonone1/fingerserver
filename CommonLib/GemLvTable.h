#ifndef __LYNX_COMMON_LIB_GEMLV_TABLE_H__
#define __LYNX_COMMON_LIB_GEMLV_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct GemLvTemplate 
	{
		UInt32 mGemLv;
		
		UInt32 mPlayerLv;

		UInt32 mLv1Count;
		
		LYNX_S11N_3(GemLvTemplate, UInt32, mGemLv,UInt32, mPlayerLv,UInt32 ,mLv1Count);

	};

	typedef Map<UInt32, GemLvTemplate> GemLvTemplateMap;


	struct GemLvTable
	{
		GemLvTemplateMap mMap;


		LYNX_S11N_1(GemLvTable, GemLvTemplateMap, mMap);

		GemLvTemplate* get(UInt32 id)
		{
			GemLvTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_GEMLV_TABLE_H__