#ifndef __LYNX_COMMON_LIB_ATTRENHANCE_TABLE_H__
#define __LYNX_COMMON_LIB_ATTRENHANCE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct AttrEnhanceTemplate 
	{
		UInt32 mLevel;
		float  mAddRate;
		UInt64 mCostCoin;
		UInt64 mExp;
		UInt64 mID;
		UInt32 mStar;

		LYNX_S11N_6(AttrEnhanceTemplate, UInt64, mID, UInt32, mStar,UInt32, mLevel, float, mAddRate,UInt64, mCostCoin,
			UInt64, mExp
			);

	};

	typedef Map<UInt64, AttrEnhanceTemplate> AttrEnhanceTemplateMap;
	typedef Map<UInt32, Map<UInt32, AttrEnhanceTemplate> > AttrEnhanceReverseMap;
	

	struct AttrEnhanceTable
	{
		AttrEnhanceTemplateMap mMap;
		AttrEnhanceReverseMap mReverseMap;

		LYNX_S11N_2(AttrEnhanceTable, AttrEnhanceTemplateMap, mMap,AttrEnhanceReverseMap, mReverseMap);

		AttrEnhanceTemplate* get(UInt64 id)
		{
			AttrEnhanceTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		AttrEnhanceTemplate * reverseGet(UInt32 starLv, UInt32 enhanceLv)
		{
			AttrEnhanceReverseMap::Iter * reversIter = mReverseMap.find(starLv);
			
			if(reversIter)
			{
				Map<UInt32, AttrEnhanceTemplate>::Iter * resultIter =  reversIter->mValue.find(enhanceLv);

				if(resultIter)
				{
					return &resultIter->mValue;
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

#endif // __LYNX_COMMON_LIB_ATTRENHANCE_TABLE_H__