#ifndef __LYNX_COMMON_LIB_SERVANTSTAR_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTSTAR_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ServantStarTemplate 
	{
		UInt64 mId;
		UInt64 mServantId;
		UInt32 mStar;

		UInt32 mCost;
		UInt32 mCount;
	
		UInt32 mPower;
		UInt32 mAttrType;
		UInt32 mAttrValue;
		

		LYNX_S11N_8(ServantStarTemplate, UInt64, mId,UInt64, mServantId,UInt32, mStar,
			UInt32, mCost,UInt32, mCount,UInt32, mPower,
			UInt32, mAttrType, UInt32, mAttrValue);

	};

	typedef Map<UInt64, ServantStarTemplate> ServantStarTemplateMap;
	typedef Map<UInt64, Map<UInt32, ServantStarTemplate> > ServantStarReverseMap;


	struct ServantStarTable
	{
		ServantStarTemplateMap mMap;
		ServantStarReverseMap mReverseMap;
		LYNX_S11N_2(ServantStarTable, ServantStarTemplateMap, mMap, ServantStarReverseMap, mReverseMap);

		ServantStarTemplate* get(UInt64 id)
		{
			ServantStarTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		ServantStarTemplate* reverseGet(UInt64 servantId, UInt32 star)
		{
			ServantStarReverseMap::Iter* reverseIter =  mReverseMap.find(servantId);
			
			if(reverseIter)
			{
				Map<UInt32, ServantStarTemplate>::Iter* reverseTempIter =  reverseIter->mValue.find(star);
				if(reverseTempIter)
				{
					return &reverseTempIter->mValue;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}

		void clear()
		{
			mMap.clear();
			mReverseMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_SERVANTSTAR_TABLE_H__