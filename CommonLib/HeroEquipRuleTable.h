#ifndef __LYNX_COMMON_LIB_HEROEQUIPRULE_TABLE_H__
#define __LYNX_COMMON_LIB_HEROEQUIPRULE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HeroEquipRuleTemplate 
	{
		UInt64 mHeroId;

		UInt64 mEquipId1;
		UInt64 mEquipId2;
		UInt64 mEquipId3;
		UInt64 mEquipId4;
		UInt64 mEquipId5;
		UInt64 mEquipId6;

		UInt32 mEquip1UnlockLv;
		UInt32 mEquip2UnlockLv;
		UInt32 mEquip3UnlockLv;
		UInt32 mEquip4UnlockLv;
		UInt32 mEquip5UnlockLv;
		UInt32 mEquip6UnlockLv;

		UInt32 mJewelry1UnlockLv;
		UInt32 mJewelry2UnlockLv;
		UInt32 mJewelry3UnlockLv;
		UInt32 mJewelry4UnlockLv;
		UInt32 mJewelry5UnlockLv;
		UInt32 mJewelry6UnlockLv;

		LYNX_S11N_19(HeroEquipRuleTemplate, UInt64, mHeroId,UInt64, mEquipId1,UInt64, mEquipId2,UInt64, mEquipId3,UInt64, mEquipId4,UInt64, mEquipId5,UInt64, mEquipId6,
			UInt32, mEquip1UnlockLv,UInt32, mEquip2UnlockLv,UInt32, mEquip3UnlockLv,UInt32, mEquip4UnlockLv,UInt32, mEquip5UnlockLv,UInt32, mEquip6UnlockLv,
			UInt32, mJewelry1UnlockLv,UInt32, mJewelry2UnlockLv,UInt32, mJewelry3UnlockLv,UInt32, mJewelry4UnlockLv,UInt32, mJewelry5UnlockLv,UInt32, mJewelry6UnlockLv);

	};

	typedef Map<UInt64, HeroEquipRuleTemplate> HeroEquipRuleTemplateMap;
	
	struct HeroEquipRuleTable
	{
		HeroEquipRuleTemplateMap mMap;
		

		LYNX_S11N_1(HeroEquipRuleTable, HeroEquipRuleTemplateMap, mMap);

		HeroEquipRuleTemplate* get(UInt64 id)
		{
			HeroEquipRuleTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_HEROEQUIPRULE_TABLE_H__