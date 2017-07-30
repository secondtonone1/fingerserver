#ifndef __LYNX_COMMON_LIB_CONSORTSIGN_TABLE_H__
#define __LYNX_COMMON_LIB_CONSORTSIGN_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ConsortSignTemplate 
	{
		UInt32 mId;
		String mCost;
		UInt32 mActivity;
		UInt32 mConsortExp;
		UInt32 mHonor;
		
		
		LYNX_S11N_5(ConsortSignTemplate, UInt32, mId, String, mCost, UInt32, mActivity, UInt32, mConsortExp,
			UInt32, mHonor);

	};

	typedef Map<UInt32, ConsortSignTemplate> ConsortSignTemplateMap;
	


	struct ConsortSignTable
	{
		ConsortSignTemplateMap mMap;
		

		LYNX_S11N_1(ConsortSignTable, ConsortSignTemplateMap, mMap);

		ConsortSignTemplate* get(UInt32 id)
		{
			ConsortSignTemplateMap::Iter* iter = mMap.find(id);
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