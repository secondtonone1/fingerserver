#ifndef __LYNX_COMMON_LIB_BUFF_TABLE_H__
#define __LYNX_COMMON_LIB_BUFF_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct BuffTemplate 
	{
		UInt64 mId;
		String mName;
		String mDesc;
		UInt32 mType;
		UInt32 mSubType;
		UInt32 mKeepTime;
		String mResPrefix;

		String mParam1;
		String mParam2;
		String mParam3;
		String mParam4;
		String mParam5;
		String mParam6;
		String mParam7;
		String mParam8;
	

		LYNX_S11N_15(BuffTemplate, UInt64, mId, String, mName,String, mDesc,
						UInt32, mType,UInt32,mSubType,UInt32, mKeepTime,
						String,mResPrefix,String, mParam1,String, mParam2,
						String, mParam3,String, mParam4,String, mParam5,
						String, mParam6, String, mParam7,String, mParam8
			
			);

	};

	typedef Map<UInt64, BuffTemplate> BuffTemplateMap;


	struct BuffTable
	{
		BuffTemplateMap mMap;


		LYNX_S11N_1(BuffTable, BuffTemplateMap, mMap);

		BuffTemplate* get(UInt32 id)
		{
			BuffTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_BUFF_TABLE_H__