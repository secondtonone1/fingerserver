#ifndef __LYNX_COMMON_LIB_EQUIP_SET_TABLE_H__
#define __LYNX_COMMON_LIB_EQUIP_SET_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct EquipSetTemplate 
	{
		UInt64 mSetId;
		String mName;
		String mDesc;
		UInt32 mList1;
		UInt32 mList2;
		UInt32 mList3;
		UInt32 mList4;
		UInt32 mList5;
		UInt32 mList6;
		UInt32 mList7;
		UInt32 mList8;
		UInt32 mList9;

		UInt32 mAttrRequireEquipCount1;
		UInt32 mAttrType1;
		UInt32 mAttrValue1;

		UInt32 mAttrRequireEquipCount2;
		UInt32 mAttrType2;
		UInt32 mAttrValue2;

		UInt32 mAttrRequireEquipCount3;
		UInt32 mAttrType3;
		UInt32 mAttrValue3;

		LYNX_S11N_21(EquipSetTemplate, UInt64, mSetId,String, mName,String ,mDesc,
			UInt32, mList1,UInt32, mList2,UInt32, mList3,
			UInt32, mList4,UInt32, mList5,UInt32, mList6,
			UInt32, mList7,UInt32, mList8,UInt32, mList9,
			UInt32, mAttrRequireEquipCount1,UInt32, mAttrType1,UInt32, mAttrValue1,
			UInt32, mAttrRequireEquipCount2,UInt32, mAttrType2,UInt32, mAttrValue2,
			UInt32, mAttrRequireEquipCount3,UInt32, mAttrType3,UInt32, mAttrValue3
			);

	};

	typedef Map<UInt64, EquipSetTemplate> EquipSetTemplateMap;


	struct EquipSetTable
	{
		EquipSetTemplateMap mMap;


		LYNX_S11N_1(EquipSetTable, EquipSetTemplateMap, mMap);

		EquipSetTemplate* get(UInt32 id)
		{
			EquipSetTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_EQUIP_SET_TABLE_H__