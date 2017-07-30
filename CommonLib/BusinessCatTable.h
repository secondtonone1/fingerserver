#ifndef __LYNX_COMMON_LIB_BUSINESSCAT_TABLE_H__
#define __LYNX_COMMON_LIB_BUSINESSCAT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct BusinessCatTemplate 
	{
		UInt32 mId;
		String mAwardStr1;
		String mCostStr1;

		String mAwardStr2;
		String mCostStr2;

		String mAwardStr3;
		String mCostStr3;

		String mAwardStr4;
		String mCostStr4;

		String mAwardStr5;
		String mCostStr5;

		String mAwardStr6;
		String mCostStr6;

		String mAwardStr7;
		String mCostStr7;

		String mAwardStr8;
		String mCostStr8;

		String mAwardStr9;
		String mCostStr9;

		String mAwardStr10;
		String mCostStr10;

		String mAwardStr11;
		String mCostStr11;

		String mAwardStr12;
		String mCostStr12;

		String mAwardStr13;
		String mCostStr13;

		String mAwardStr14;
		String mCostStr14;

		String mAwardStr15;
		String mCostStr15;

		String mAwardStr16;
		String mCostStr16;

		List<AllItemEle> mAwardList;
		List<AllItemEle> mCostList;


		LYNX_S11N_35(BusinessCatTemplate, UInt32, mId, String, mAwardStr1, String, mCostStr1,
			String, mAwardStr2, String, mCostStr2, String, mAwardStr3, String, mCostStr3,
			String, mAwardStr4, String, mCostStr4, String, mAwardStr5, String ,mCostStr5,
			String, mAwardStr6, String, mCostStr6, String, mAwardStr7, String ,mCostStr7,
			String, mAwardStr8, String, mCostStr8, String, mAwardStr9, String, mCostStr9,
			String, mAwardStr10, String, mCostStr10, String, mAwardStr11, String, mCostStr11,
			String, mAwardStr12, String ,mCostStr12, String, mAwardStr13, String, mCostStr13,
			String ,mAwardStr14, String ,mCostStr14, String ,mAwardStr15, String, mCostStr15,
			String ,mAwardStr16, String, mCostStr16, List<AllItemEle>, mAwardList,List<AllItemEle>, mCostList );

	};

	typedef Map<UInt32, BusinessCatTemplate> BusinessCatTemplateMap;
	


	struct BusinessCatTable
	{
		BusinessCatTemplateMap mMap;
		

		LYNX_S11N_1(BusinessCatTable, BusinessCatTemplateMap, mMap);

		BusinessCatTemplate* get(UInt32 id)
		{
			BusinessCatTemplateMap::Iter* iter = mMap.find(id);
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

		void createList(BusinessCatTemplate * pTemp, std::string awardStr,  std::string costStr);

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx




#endif