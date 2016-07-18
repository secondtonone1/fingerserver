#ifndef __LYNX_COMMON_LIB_SERVANTBATTLEOPEN_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTBATTLEOPEN_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{

	struct ServantBattleOpenTemplate 
	{
		UInt32 mPos;
		UInt32 mLevel;
		UInt32 mCost;

		LYNX_S11N_3(ServantBattleOpenTemplate, UInt32, mPos,UInt32, mLevel, UInt32, mCost
			);

	};

	typedef Map<UInt64, ServantBattleOpenTemplate> ServantBattleOpenTemplateMap;

	struct ServantBattleOpenTable
	{
		ServantBattleOpenTemplateMap mMap;

		LYNX_S11N_1(ServantBattleOpenTable, ServantBattleOpenTemplateMap, mMap);

		ServantBattleOpenTemplate* get(UInt32 id)
		{
			ServantBattleOpenTemplateMap::Iter* iter = mMap.find(id);

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

#endif //__LYNX_COMMON_LIB_SERVANTBATTLEOPEN_TABLE_H__