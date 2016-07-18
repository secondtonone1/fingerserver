#ifndef __LYNX_COMMON_LIB_SERVANTLVUPMATERIAL_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTLVUPMATERIAL_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{

	struct ServantLvUpMaterialTemplate 
	{
		UInt64 mId;
		UInt32 mExpProvide;
	

		LYNX_S11N_2(ServantLvUpMaterialTemplate, UInt64, mId,UInt32, mExpProvide
			);

	};

	typedef Map<UInt64, ServantLvUpMaterialTemplate> ServantLvUpMaterialTemplateMap;

	struct ServantLvUpMaterialTable
	{
		ServantLvUpMaterialTemplateMap mMap;

		LYNX_S11N_1(ServantLvUpMaterialTable, ServantLvUpMaterialTemplateMap, mMap);

		ServantLvUpMaterialTemplate* get(UInt32 id)
		{
			ServantLvUpMaterialTemplateMap::Iter* iter = mMap.find(id);

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

#endif //__LYNX_COMMON_LIB_SERVANTLVUPMATERIAL_TABLE_H__