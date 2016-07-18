#ifndef __LYNX_COMMON_LIB_NEWJEWELRY_TABLE_H__
#define __LYNX_COMMON_LIB_NEWJEWELRY_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct NewJewelryTemplate 
	{
		UInt64 mJewelryId;

		UInt32 mQuality;

		UInt32 mEquipSlotIdx;

		
		UInt32 mLvRequire;

		UInt64 mRandomAttr1;

		UInt64 mRandomAttr2;

		UInt64 mRandomAttr3;

		UInt64 mRandomAttr4;

		UInt64 mRandomAttr5;

		UInt32 mStaticAttr1Key;
		
		UInt32 mStaticAttr1ValueMin;

		UInt32 mStaticAttr1ValueMax;

		UInt32 mPolishCount;

		UInt32 mPrice;

		UInt32 mRefreshCoin;

		LYNX_S11N_15(NewJewelryTemplate, UInt64, mJewelryId,UInt32, mQuality,UInt32 ,mEquipSlotIdx,UInt32,mLvRequire,UInt64, mRandomAttr1,
			UInt64, mRandomAttr2, UInt64, mRandomAttr3, UInt64, mRandomAttr4, UInt64, mRandomAttr5, UInt32, mStaticAttr1Key,
			UInt32, mStaticAttr1ValueMin, UInt32, mStaticAttr1ValueMax,UInt32, mPolishCount, UInt32, mPrice,UInt32 ,mRefreshCoin
			);

	};

	typedef Map<UInt64, NewJewelryTemplate> NewJewelryTemplateMap;


	struct NewJewelryTable
	{
		NewJewelryTemplateMap mMap;


		LYNX_S11N_1(NewJewelryTable, NewJewelryTemplateMap, mMap);

		NewJewelryTemplate* get(UInt64 id)
		{
			NewJewelryTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_NEWJEWELRY_TABLE_H__