#ifndef __LYNX_COMMON_LIB_CONSORTMSG_TABLE_H__
#define __LYNX_COMMON_LIB_CONSORTMSG_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ConsortMsgTemplate 
	{
		UInt32 mId;
		UInt32 mVisible;
		
		
		LYNX_S11N_2(ConsortMsgTemplate, UInt32, mId, UInt32, mVisible);

	};

	typedef Map<UInt32, ConsortMsgTemplate> ConsortMsgTemplateMap;
	


	struct ConsortMsgTable
	{
		ConsortMsgTemplateMap mMap;
		

		LYNX_S11N_1(ConsortMsgTable, ConsortMsgTemplateMap, mMap);

		ConsortMsgTemplate* get(UInt32 id)
		{
			ConsortMsgTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_CONSORTMSG_TABLE_H__