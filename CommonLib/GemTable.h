#ifndef __LYNX_COMMON_LIB_GEM_TABLE_H__
#define __LYNX_COMMON_LIB_GEM_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct GemTemplate 
	{
		UInt64 mGemId;

		UInt32 mLevel;

		UInt32 mPower;

		UInt32 mAttrType1;

		UInt32 mAttrValue1;

		UInt32 mAttrType2;

		UInt32 mAttrValue2;

		UInt32 mAttrType3;

		UInt32 mAttrValue3;

		UInt32 mAttrType4;

		UInt32 mAttrValue4;

		


		LYNX_S11N_11(GemTemplate, UInt64, mGemId,UInt32, mLevel,UInt32 ,mPower,
			UInt32, mAttrType1,UInt32, mAttrValue1,UInt32, mAttrType2,
			UInt32, mAttrValue2,UInt32, mAttrType3,UInt32, mAttrValue3,
			UInt32, mAttrType4,UInt32, mAttrValue4
			);

	};

	typedef Map<UInt64, GemTemplate> GemTemplateMap;


	struct GemTable
	{
		GemTemplateMap mMap;


		LYNX_S11N_1(GemTable, GemTemplateMap, mMap);

		GemTemplate* get(UInt32 id)
		{
			GemTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_GEM_TABLE_H__