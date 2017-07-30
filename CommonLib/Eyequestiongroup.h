#ifndef  __LYNX_COMMON_LIB_EYEQUESTION_TABLE_H__
#define __LYNX_COMMON_LIB_EYEQUESTION_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct EyeQuestionTemplate 
	{
		UInt32 mId;
		UInt32 mAward1;
		UInt32 mAward2;
		UInt32 mAward3;
		UInt32 mAward4;
		UInt32 mAward5;
		
		LYNX_S11N_6(EyeQuestionTemplate, UInt32, mId, 
			UInt32, mAward1,UInt32, mAward2, UInt32, mAward3, UInt32, mAward4,
			UInt32, mAward5
		);

	};

	typedef Map<UInt32, EyeQuestionTemplate> EyeQuestionTemMap;


	struct EyeQuestionTable
	{
		EyeQuestionTemMap mMap;

		LYNX_S11N_1(EyeQuestionTable, EyeQuestionTemMap, mMap);

		EyeQuestionTemplate* get(UInt32 id)
		{
			EyeQuestionTemMap::Iter* iter = mMap.find(id);
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




#endif //__LYNX_COMMON_LIB_EYEQUESTION_TABLE_H__