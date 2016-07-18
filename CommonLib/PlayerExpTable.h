#ifndef __LYNX_COMMON_LIB_PLAYEREXP_TABLE_H__
#define __LYNX_COMMON_LIB_PLAYEREXP_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct PlayerExpTemplate 
	{
		UInt32 mLevel;

		UInt32 mExp;
		//每一级体力上限
		UInt32 mStrength;
		//每一级恢复体力
		UInt32 mStrengthAdd;

		LYNX_S11N_4(PlayerExpTemplate, UInt32, mLevel,UInt32, mExp,UInt32,mStrength, UInt32, mStrengthAdd);

	};

	typedef Map<UInt64, PlayerExpTemplate> PlayerExpTemplateMap;


	struct PlayerExpTable
	{
		PlayerExpTemplateMap mMap;


		LYNX_S11N_1(PlayerExpTable, PlayerExpTemplateMap, mMap);

		PlayerExpTemplate* get(UInt32 id)
		{
			PlayerExpTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_PLAYEREXP_TABLE_H__