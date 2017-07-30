#ifndef __LYNX_COMMON_LIB_HOARSTONELV_TABLE_H__
#define __LYNX_COMMON_LIB_HOARSTONELV_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HoarstoneLvTemplate 
	{
		// 界石表id
		UInt64 mId;

		//界石id
		UInt64 mIdx;

		//界石等级

		UInt32 mLevel;

		//界石power
		UInt32 mPower;

		//界石nextLevelAddPower
		UInt32 mNextLvAddPower;

		//花费的金币
		UInt32 mCostCoin;

		//花费的碎片
		UInt32 mCostPiece;

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
		
		UInt32 mToMax;

		LYNX_S11N_18(HoarstoneLvTemplate,UInt64, mId, UInt64, mIdx,UInt32, mLevel,UInt32, mPower,
				UInt32, mNextLvAddPower,UInt32, mCostCoin,UInt32, mCostPiece,UInt32, mAttrType1,UInt32, mAttrValue1,
				UInt32, mAttrType2,UInt32, mAttrValue2,UInt32, mAttrType3,UInt32, mAttrValue3,
				UInt32, mAttrType4,UInt32, mAttrValue4,UInt32, mAttrType5,UInt32, mAttrValue5,	UInt32, mToMax
			);

	};

	typedef Map<UInt64, HoarstoneLvTemplate> HoarstoneLvTemplateMap;

	typedef Map<UInt64, Map<UInt32 , HoarstoneLvTemplate> > HoarstoneLvReversMap; 

	struct HoarstoneLvTable
	{
		HoarstoneLvTemplateMap mMap;

		HoarstoneLvReversMap mReversMap;

		LYNX_S11N_2(HoarstoneLvTable, HoarstoneLvTemplateMap, mMap,HoarstoneLvReversMap, mReversMap);

		HoarstoneLvTemplate* get(UInt64 id)
		{
			HoarstoneLvTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		HoarstoneLvTemplate* reverseGet(UInt64 hoarStoneId, UInt32 hoarStoneLv)
		{
			HoarstoneLvReversMap::Iter *reverseMapIter = mReversMap.find(hoarStoneId);
			
			if(reverseMapIter)
			{
				Map<UInt32 , HoarstoneLvTemplate>::Iter * hoarStoneTempIter = reverseMapIter->mValue.find(hoarStoneLv);
				
				if(hoarStoneTempIter)
				{
					return &hoarStoneTempIter->mValue;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}

		void clear()
		{
			mMap.clear();
			mReversMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_HOARSTONEBASE_TABLE_H__


