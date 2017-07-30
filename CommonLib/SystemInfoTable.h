#ifndef __LYNX_COMMON_LIB_SYSTEMINFO_TABLE_H__
#define __LYNX_COMMON_LIB_SYSTEMINFO_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"


namespace Lynx
{
	struct SystemInfoTemplate 
	{
		UInt32 systemID;
		UInt32 funId;
		UInt32 subFunId;
		UInt32 openConditionType;
		UInt32 openConditionValue;
		UInt32 haveGuide;
		UInt32 preNoticeLevel;
		
		LYNX_S11N_7(SystemInfoTemplate, UInt32, systemID,	UInt32, funId,UInt32, subFunId,UInt32, openConditionType,
				UInt32, openConditionValue,UInt32, haveGuide,UInt32, preNoticeLevel);
	};

	typedef Map<UInt32, SystemInfoTemplate> SystemInfoTemplateMap;


	struct SystemInfoTable
	{
		SystemInfoTemplateMap mMap;


		LYNX_S11N_1(SystemInfoTable, SystemInfoTemplateMap, mMap);

		SystemInfoTemplate* get(UInt32 id)
		{
			SystemInfoTemplateMap::Iter* iter = mMap.find(id);

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


#endif //__LYNX_COMMON_LIB_SYSTEMINFO_TABLE_H__