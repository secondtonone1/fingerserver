#ifndef __LYNX_COMMON_LIB_POLISHPOINTS_TABLE_H__
#define __LYNX_COMMON_LIB_POLISHPOINTS_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"


namespace Lynx
{
	struct PolishPointsTemplate 
	{
		UInt32 mLevel;

		UInt32 mPurplePoints;

		UInt32 mOrangePoints;

		LYNX_S11N_3(PolishPointsTemplate, UInt32, mLevel,UInt32, mPurplePoints,UInt32, mOrangePoints);

	};

	typedef Map<UInt32, PolishPointsTemplate> PolishPointsTemplateMap;


	struct PolishPointsTable
	{
		PolishPointsTemplateMap mMap;


		LYNX_S11N_1(PolishPointsTable, PolishPointsTemplateMap, mMap);

		PolishPointsTemplate* get(UInt32 id)
		{
			PolishPointsTemplateMap::Iter* iter = mMap.find(id);

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


#endif //__LYNX_COMMON_LIB_POLISHPOINTS_TABLE_H__