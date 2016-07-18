#ifndef __LYNX_COMMON_LIB_SERVANTTREASURE_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTTREASURE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ServantTreasureTemplate 
	{
		UInt64 mId;
	
		UInt32 mQuality;
		
		UInt32 mAttrType1;

		UInt32 mAttrValue1;

		UInt32 mAttrType2;

		UInt32 mAttrValue2;

		UInt32 mAttrType3;

		UInt32 mAttrValue3;

		UInt32 mPower;

		UInt32 mCombineId1;

		UInt32 mCombineCount1;

		UInt32 mCombineId2;

		UInt32 mCombineCount2;

		UInt32 mCombineId3;

		UInt32 mCombineCount3;

		LYNX_S11N_15(ServantTreasureTemplate, UInt64, mId,UInt32, mQuality, UInt32, mAttrType1, UInt32, mAttrValue1,
			UInt32, mAttrType2, UInt32, mAttrValue2,
			UInt32, mAttrType3, UInt32, mAttrValue3,
			UInt32, mPower,
			UInt32, mCombineId1,UInt32, mCombineCount1,
			UInt32, mCombineId2,UInt32, mCombineCount2,
			UInt32, mCombineId3,UInt32, mCombineCount3

			);

	};

	typedef Map<UInt64, ServantTreasureTemplate> ServantTreasureTemplateMap;


	struct ServantTreasureTable
	{
		ServantTreasureTemplateMap mMap;

		LYNX_S11N_1(ServantTreasureTable, ServantTreasureTemplateMap, mMap);

		ServantTreasureTemplate* get(UInt64 id)
		{
			ServantTreasureTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_SERVANTTREASURE_TABLE_H__