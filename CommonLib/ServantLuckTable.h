#ifndef __LYNX_COMMON_LIB_SERVANTLUCK_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTLUCK_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ServantLuckTemplate 
	{
		UInt64 mId;
		UInt32 mAttrType1;
		UInt32 mAttrValue1;
		UInt32 mAttrType2;
		UInt32 mAttrValue2;
		UInt32 mAttrType3;
		UInt32 mAttrValue3;

		UInt64 mServant1;
		UInt64 mServant2;
		UInt64 mServant3;
		UInt64 mServant4;
		UInt64 mServant5;

		

		LYNX_S11N_12(ServantLuckTemplate, UInt64, mId,UInt32, mAttrType1,UInt32, mAttrValue1, UInt32, mAttrType2, UInt32, mAttrValue2,
			UInt32, mAttrType3, UInt32, mAttrValue3, UInt64, mServant1, UInt64, mServant2,UInt64, mServant3, UInt64, mServant4, UInt64, mServant5
			);

	};

	typedef Map<UInt64, ServantLuckTemplate> ServantLuckTemplateMap;


	struct ServantLuckTable
	{
		ServantLuckTemplateMap mMap;

		LYNX_S11N_1(ServantLuckTable, ServantLuckTemplateMap, mMap);

		ServantLuckTemplate* get(UInt64 id)
		{
			ServantLuckTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_SERVANTLUCK_TABLE_H__