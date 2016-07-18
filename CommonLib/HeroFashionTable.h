#ifndef __LYNX_COMMON_LIB_HEROFASHION_TABLE_H__
#define __LYNX_COMMON_LIB_HEROFASHION_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HeroFashionTemplate 
	{
		UInt64 mHeroId;

		UInt64 mFashionId;

		LYNX_S11N_2(HeroFashionTemplate, UInt64, mHeroId,UInt64, mFashionId);

	};

	typedef Map<UInt64, HeroFashionTemplate> HeroFashionMap;

	struct HeroFashionTable
	{
		HeroFashionMap mMap;


		LYNX_S11N_1(HeroFashionTable, HeroFashionMap, mMap);

		HeroFashionTemplate* get(UInt64 id)
		{
			HeroFashionMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_HEROFASHION_TABLE_H__