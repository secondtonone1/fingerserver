#ifndef __LYNX_COMMON_LIB_MonsterAward_TABLE_H__
#define __LYNX_COMMON_LIB_MonsterAward_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	
	struct MonsterAwardTemplate 
	{
		UInt32 id;

		UInt32 stageid;

		UInt32 autoawardid;

		UInt32 killawardid;

		List<KeyValue> timesAward;
	
		
		LYNX_S11N_5(MonsterAwardTemplate, UInt32, id,UInt32, stageid,UInt32, autoawardid,UInt32, killawardid,List<KeyValue>, timesAward);

	};

	typedef Map<UInt32, MonsterAwardTemplate> MonsterAwardTemplateMap;


	struct MonsterAwardTable
	{
		MonsterAwardTemplateMap mMap;


		LYNX_S11N_1(MonsterAwardTable, MonsterAwardTemplateMap, mMap);

		MonsterAwardTemplate* get(UInt32 id)
		{
			MonsterAwardTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_MonsterAward_TABLE_H__