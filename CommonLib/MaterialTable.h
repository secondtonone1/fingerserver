#ifndef __LYNX_COMMON_LIB_MATERIAL_TABLE_H__
#define __LYNX_COMMON_LIB_MATERIAL_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct MaterialTemplate 
	{
		UInt64 mId;
		UInt32 mQuality;
		UInt32 mOrder;
		UInt32 mPrice;
		String mProduced;

		LYNX_S11N_5(MaterialTemplate, UInt64, mId,UInt32, mQuality,UInt32, mOrder, UInt32, mPrice,String, mProduced);

	};

	typedef Map<UInt64, MaterialTemplate> MaterialTemplateMap;
	

	struct MaterialTable
	{
		MaterialTemplateMap mMap;
		
		LYNX_S11N_1(MaterialTable, MaterialTemplateMap, mMap);

		MaterialTemplate* get(UInt64 id)
		{
			MaterialTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_MATERIAL_TABLE_H__