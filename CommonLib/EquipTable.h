#ifndef __LYNX_COMMON_LIB_EQUIP_TABLE_H__
#define __LYNX_COMMON_LIB_EQUIP_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct EquipTemplate 
	{
		UInt64 mId;
		
		String mRes;

		UInt32 mLevelRequire;
	
		UInt32 mEquipType;

		UInt32 mEquipPos;

		UInt32 mEquipHole;

		UInt32 mMaxStar;

		UInt32 mEffect;

		UInt32 mEquipSet;

		UInt32 mMaterialExp;

		UInt32 mResID;

		UInt32 mStaticAttrType1;

		UInt32 mStaticAttrValMin1;

		UInt32 mStaticAttrValMax1;

		UInt32 mStaticAttrType2;

		UInt32 mStaticAttrValMin2;

		UInt32 mStaticAttrValMax2;

		UInt32 mStaticAttrType3;

		UInt32 mStaticAttrValMin3;

		UInt32 mStaticAttrValMax3;

		UInt32 mRandomRuleId1;

		UInt32 mRandomRuleId2;

		UInt32 mRandomRuleId3;

		UInt32 mRandomRuleId4;

		UInt32 mRandomRuleId5;

		UInt32 mRefreshcoin;
		UInt32 mRefreshstone;
		UInt32 mQuality;
		

		LYNX_S11N_28(EquipTemplate, UInt64, mId,String, mRes,UInt32, mLevelRequire,
			UInt32 ,mEquipType,UInt32, mEquipPos,UInt32, mEquipHole,
			UInt32, mMaxStar,UInt32, mEffect,UInt32, mEquipSet,
			UInt32, mMaterialExp,UInt32, mResID,UInt32, mStaticAttrType1,
			UInt32, mStaticAttrValMin1,UInt32, mStaticAttrValMax1,UInt32, mStaticAttrType2,
			UInt32, mStaticAttrValMin2,UInt32, mStaticAttrValMax2,UInt32, mStaticAttrType3,
			UInt32, mStaticAttrValMin3,UInt32, mStaticAttrValMax3,UInt32, mRandomRuleId1,
			UInt32, mRandomRuleId2,UInt32, mRandomRuleId3,UInt32, mRandomRuleId4,
			UInt32, mRandomRuleId5,UInt32, mRefreshcoin,UInt32, mRefreshstone,UInt32, mQuality
			);

	};

	typedef Map<UInt64, EquipTemplate> EquipTemplateMap;


	struct EquipTable
	{
		EquipTemplateMap mMap;


		LYNX_S11N_1(EquipTable, EquipTemplateMap, mMap);

		EquipTemplate* get(UInt32 id)
		{
			EquipTemplateMap::Iter* iter = mMap.find(id);
			
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

#endif // __LYNX_COMMON_LIB_EQUIP_TABLE_H__