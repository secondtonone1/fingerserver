#ifndef __LYNX_COMMON_LIB_EQUIP_GEM_RULE_TABLE_H__
#define __LYNX_COMMON_LIB_EQUIP_GEM_RULE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct EquipGemRuleTemplate 
	{
		UInt64 mEquipId;
		
		UInt32 mGemLevel1;
		String mCondition1;

		UInt32 mGemLevel2;
		String mCondition2;

		UInt32 mGemLevel3;
		String mCondition3;

		UInt32 mGemLevel4;
		String mCondition4;

		UInt32 mGemLevel5;
		String mCondition5;

		UInt32 mGemLevel6;
		String mCondition6;

		UInt32 mCondKey1;
		UInt32 mCondValue1;

		UInt32 mCondKey2;
		UInt32 mCondValue2;

		UInt32 mCondKey3;
		UInt32 mCondValue3;

		UInt32 mCondKey4;
		UInt32 mCondValue4;

		UInt32 mCondKey5;
		UInt32 mCondValue5;

		UInt32 mCondKey6;
		UInt32 mCondValue6;

		LYNX_S11N_25(EquipGemRuleTemplate, UInt64, mEquipId,UInt32, mGemLevel1,
			String, mCondition1,UInt32, mGemLevel2,String, mCondition2,UInt32, mGemLevel3,String, mCondition3,
			UInt32, mGemLevel4, String, mCondition4, UInt32, mGemLevel5, String, mCondition5,UInt32, mGemLevel6, String, mCondition6,
			UInt32, mCondKey1,UInt32, mCondValue1,UInt32, mCondKey2,UInt32, mCondValue2,UInt32, mCondKey3,UInt32, mCondValue3,UInt32, mCondKey4,
			UInt32, mCondValue4, UInt32, mCondKey5, UInt32 ,mCondValue5, UInt32, mCondKey6, UInt32, mCondValue6
			);

	};

	typedef Map<UInt64, EquipGemRuleTemplate> EquipGemRuleTemplateMap;


	struct EquipGemRuleTable
	{
		EquipGemRuleTemplateMap mMap;


		LYNX_S11N_1(EquipGemRuleTable, EquipGemRuleTemplateMap, mMap);

		EquipGemRuleTemplate* get(UInt32 id)
		{
			EquipGemRuleTemplateMap::Iter* iter = mMap.find(id);

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

		void dealCondition(UInt32 & condKey, UInt32 & condValue, std::string condStr);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_EQUIP_GEM_RULE_TABLE_H__