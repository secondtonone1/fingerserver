#ifndef __LYNX_COMMON_LIB_ATTRPOWER_TABLE_H__
#define __LYNX_COMMON_LIB_ATTRPOWER_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{

	struct AttrPowerTemplate 
	{
		UInt32 mAttrType;
		float mAttrValue;

		LYNX_S11N_2(AttrPowerTemplate,UInt32, mAttrType, float, mAttrValue);

	};

	typedef Map<UInt32, AttrPowerTemplate> AttrPowerTemplateMap;
	

	struct AttrPowerTable
	{
		AttrPowerTemplateMap mMap;
		
		LYNX_S11N_1(AttrPowerTable, AttrPowerTemplateMap, mMap);

		AttrPowerTemplate* get(UInt32 id)
		{
			AttrPowerTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_ATTRPOWER_TABLE_H__