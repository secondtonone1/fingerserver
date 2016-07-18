#ifndef __LYNX_COMMON_LIB_HOARSTONESTAR_TABLE_H__
#define __LYNX_COMMON_LIB_HOARSTONESTAR_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HoarstoneStarTemplate 
	{
		// 界石星级表id
		UInt64 mId;
		// 界石id
		UInt64 mIdx;
		//界石星级
		UInt32     mStarlevel;
		//不同星级的界石需要的符文不同，总共有六个符文
		UInt64     needEquipId1;

		UInt64     needEquipId2;

		UInt64     needEquipId3;

		UInt64     needEquipId4;

		UInt64     needEquipId5;

		UInt64     needEquipId6;

		UInt32     mAttrType1;

		UInt32     mAttrValue1;

		UInt32     mAttrType2;

		UInt32     mAttrValue2;

		UInt32     mAttrType3;

		UInt32     mAttrValue3;

		UInt32     mAttrType4;

		UInt32     mAttrValue4;

		UInt32     mAttrType5;

		UInt32     mAttrValue5;

		LYNX_S11N_19(HoarstoneStarTemplate,UInt64, mId, UInt64, mIdx,UInt32, mStarlevel,
			UInt64,needEquipId1,UInt64,needEquipId2,UInt64,needEquipId3,UInt64,needEquipId4,UInt64, needEquipId5,
			UInt64, needEquipId6,UInt32,mAttrType1,UInt32,mAttrValue1,UInt32,mAttrType2,UInt32,mAttrValue2,
			UInt32,mAttrType3,UInt32,mAttrValue3,UInt32,mAttrType4,UInt32,mAttrValue4,
			UInt32,mAttrType5,UInt32,mAttrValue5

			);

	};

	typedef Map<UInt64, HoarstoneStarTemplate> HoarstoneStarTemplateMap;
	typedef Map<UInt64, Map<UInt32, HoarstoneStarTemplate > > HoarStoneReverseMap;

	struct HoarstoneStarTable
	{
		HoarstoneStarTemplateMap mMap;
		HoarStoneReverseMap mReverseMap;

		LYNX_S11N_2(HoarstoneStarTable, HoarstoneStarTemplateMap, mMap, HoarStoneReverseMap, mReverseMap);

		HoarstoneStarTemplate* get(UInt64 id)
		{
			HoarstoneStarTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		HoarstoneStarTemplate * reverseGet(UInt64 id, UInt32 level)
		{
			HoarStoneReverseMap::Iter * reverseMapIter = mReverseMap.find(id);
			
			if(reverseMapIter)
			{
				Map<UInt32, HoarstoneStarTemplate > ::Iter * findRes = reverseMapIter->mValue.find(level);
				
				if(findRes)
				{
					return &findRes->mValue;
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
			mReverseMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_HOARSTONERUNE_TABLE_H__


