#ifndef __LYNX_COMMON_LIB_NEWGEM_TABLE_H__
#define __LYNX_COMMON_LIB_NEWGEM_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct NewGemTemplate 
	{
		UInt64 mGemId;

		UInt32 mLevel;

		UInt32 mQuality;

		UInt32 mEquipType;

		UInt32 mLvRequire;

		UInt32 mAttrType1;

		UInt32 mAttrValue1;

		UInt32 mAttrType2;

		UInt32 mAttrValue2;

		UInt32 mAttrType3;

		UInt32 mAttrValue3;

		UInt32 mAttrType4;

		UInt32 mAttrValue4;

		UInt32 mAttrType5;

		UInt32 mAttrValue5;

		UInt32 mPower;

		LYNX_S11N_16(NewGemTemplate, UInt64, mGemId,UInt32, mLevel,UInt32 ,mQuality,UInt32,mEquipType,UInt32, mLvRequire,
			UInt32, mAttrType1,UInt32, mAttrValue1,UInt32, mAttrType2,
			UInt32, mAttrValue2,UInt32, mAttrType3,UInt32, mAttrValue3,
			UInt32, mAttrType4,UInt32, mAttrValue4,UInt32, mAttrType5,
			UInt32, mAttrValue5, UInt32, mPower
			);

	};

	typedef Map<UInt64, NewGemTemplate> NewGemTemplateMap;


	struct NewGemTable
	{
		NewGemTemplateMap mMap;


		LYNX_S11N_1(NewGemTable, NewGemTemplateMap, mMap);

		NewGemTemplate* get(UInt64 id)
		{
			NewGemTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_NEWGEM_TABLE_H__