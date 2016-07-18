#ifndef __LYNX_COMMON_LIB_SERVANTLVUP_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTLVUP_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ServantLvUpTemplate 
	{
		UInt32 mLv;
		UInt32 mExp;
		

		LYNX_S11N_2(ServantLvUpTemplate, UInt32, mLv, UInt32, mExp);

	};

	typedef Map<UInt32, ServantLvUpTemplate> ServantLvUpTemplateMap;


	struct ServantLvUpTable
	{
		ServantLvUpTemplateMap mMap;

		LYNX_S11N_1(ServantLvUpTable, ServantLvUpTemplateMap, mMap);

		ServantLvUpTemplate* get(UInt32 id)
		{
			ServantLvUpTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_SERVANTLVUP_TABLE_H__