#ifndef __LYNX_COMMON_LIB_AWARDADDRATE_TABLE_H__
#define __LYNX_COMMON_LIB_AWARDADDRATE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct RateOfGood
	{
		UInt32 resourcesType;
		UInt32	subtype;
		UInt32	rate;
		LYNX_S11N_3(RateOfGood, UInt32, resourcesType,UInt32, subtype,UInt32 ,rate);

	};
	



	struct AwardAddRateTemplate 
	{
		UInt32 awardaddtype;
		
		UInt32 needcondition;

		List<RateOfGood> rateOfGoods;
		
		LYNX_S11N_3(AwardAddRateTemplate, UInt32, awardaddtype,UInt32, needcondition,List<RateOfGood> ,rateOfGoods);

	};

	typedef Map<UInt32, AwardAddRateTemplate> AwardAddRateTemplateMap;


	struct AwardAddRateTable
	{
		AwardAddRateTemplateMap mMap;


		LYNX_S11N_1(AwardAddRateTable, AwardAddRateTemplateMap, mMap);

		AwardAddRateTemplate* get(UInt32 id)
		{
			AwardAddRateTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_AWARDADDRATE_TABLE_H__