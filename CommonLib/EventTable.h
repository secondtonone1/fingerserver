#ifndef __LYNX_COMMON_LIB_EVENT_TABLE_H__
#define __LYNX_COMMON_LIB_EVENT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct EventTemplate 
	{
		UInt64 mId;
		
		LYNX_S11N_1(EventTemplate, UInt64, mId);

	};

	typedef Map<UInt64, EventTemplate> EventTemplateMap;



	struct EventTable
	{
		EventTemplateMap mMap;


		LYNX_S11N_1(EventTable, EventTemplateMap, mMap);

		EventTemplate* get(UInt64 id)
		{
			EventTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_EVENT_TABLE_H__