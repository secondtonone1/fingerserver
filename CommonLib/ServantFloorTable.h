#ifndef __LYNX_COMMON_LIB_SERVANTFLOOR_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTFLOOR_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ServantFloorTemplate 
	{
		UInt64 mId;
		UInt64 mServantId;
		UInt32 mFloor;

		UInt64 mSlot1;
		UInt64 mSlot2;
		UInt64 mSlot3;
		UInt64 mSlot4;
		
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

		LYNX_S11N_18(ServantFloorTemplate, UInt64, mId,UInt64, mServantId,UInt32, mFloor,
			UInt64, mSlot1, UInt64, mSlot2,  UInt64, mSlot3,  UInt64, mSlot4, 
			UInt32, mAttrType1, UInt32, mAttrValue1,
			UInt32, mAttrType2, UInt32, mAttrValue2,
			UInt32, mAttrType3, UInt32, mAttrValue3,
			UInt32, mAttrType4, UInt32, mAttrValue4,
			UInt32, mAttrType5, UInt32, mAttrValue5,
			UInt32, mPower);

	};

	typedef Map<UInt64, ServantFloorTemplate> ServantFloorTemplateMap;
	typedef Map<UInt64, Map<UInt32, ServantFloorTemplate> > ServantFloorReverseMap;


	struct ServantFloorTable
	{
		ServantFloorTemplateMap mMap;
		ServantFloorReverseMap mReverseMap;
		LYNX_S11N_2(ServantFloorTable, ServantFloorTemplateMap, mMap, ServantFloorReverseMap, mReverseMap);

		ServantFloorTemplate* get(UInt64 id)
		{
			ServantFloorTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		ServantFloorTemplate* reverseGet(UInt64 servantId, UInt32 floor)
		{
			ServantFloorReverseMap::Iter* reverseIter =  mReverseMap.find(servantId);

			if(reverseIter)
			{
				Map<UInt32, ServantFloorTemplate>::Iter* reverseTempIter =  reverseIter->mValue.find(floor);
				if(reverseTempIter)
				{
					return &reverseTempIter->mValue;
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

#endif // __LYNX_COMMON_LIB_SERVANTSTAR_TABLE_H__