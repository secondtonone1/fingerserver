#ifndef __LYNX_COMMON_LIB_SKILLCONVERT_TABLE_H__
#define __LYNX_COMMON_LIB_SKILLCONVERT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{

	struct SkillListTemplate 
	{
        UInt32 skill1;
		UInt32 skill2;
		UInt32 skill3;
		UInt32 skill4;
		UInt32 key;
		List<UInt32> skillList;

		LYNX_S11N_6(SkillListTemplate, UInt32 ,key,UInt32, skill1, UInt32, skill2,
				UInt32, skill3, UInt32,  skill4, List<UInt32>, skillList
			);

	};

	typedef Map<UInt32, SkillListTemplate> SkillListMap;

	struct SkillConvertTable
	{
		SkillListMap mMap;

		LYNX_S11N_1(SkillConvertTable, SkillListMap, mMap);

		SkillListTemplate* get(UInt32 id)
		{
			SkillListMap::Iter* iter = mMap.find(id);

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

#endif //__LYNX_COMMON_LIB_SKILLCONVERT_TABLE_H__