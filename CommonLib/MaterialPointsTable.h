#ifndef __LYNX_COMMON_LIB_MATERIALPOINTS_TABLE_H__
#define __LYNX_COMMON_LIB_MATERIALPOINTS_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct MaterialPointsTemplate 
	{
		UInt32 mLevel;
		
		UInt32 mPurplePoints;

		UInt32 mBluePoints;

		UInt32 mOrangePoints;


		LYNX_S11N_4(MaterialPointsTemplate, UInt32, mLevel,UInt32, mPurplePoints,UInt32, mBluePoints,UInt32, mOrangePoints );

	};

	typedef Map<UInt32, MaterialPointsTemplate> MaterialPointsTemplateMap;
	

	struct MaterialPointsTable
	{
		MaterialPointsTemplateMap mMap;


		LYNX_S11N_1(MaterialPointsTable, MaterialPointsTemplateMap, mMap);

		MaterialPointsTemplate* get(UInt32 id)
		{
			MaterialPointsTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_MATERIALPOINTS_TABLE_H__