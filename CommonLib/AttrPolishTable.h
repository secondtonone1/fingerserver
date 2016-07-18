#ifndef __LYNX_COMMON_LIB_ATTRPOLISH_TABLE_H__
#define __LYNX_COMMON_LIB_ATTRPOLISH_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"
#include <string>
namespace Lynx
{
	struct AttrPolishTemplate 
	{

		UInt32 mLevel;
		UInt32 mWhite;
		UInt32 mGreen;
		UInt32 mBlue;
		UInt32 mPurple;
		UInt32 mOrange;
		


		LYNX_S11N_6(AttrPolishTemplate, UInt32, mLevel,UInt32, mWhite,UInt32, mGreen,UInt32, mBlue,
			UInt32, mPurple,UInt32, mOrange);

	};

	typedef Map<UInt32, AttrPolishTemplate> AttrPolishTemplateMap;


	struct AttrPolishTable
	{
		AttrPolishTemplateMap mMap;


		LYNX_S11N_1(AttrPolishTable, AttrPolishTemplateMap, mMap);

		AttrPolishTemplate* get(UInt32 key)
		{
			AttrPolishTemplateMap::Iter* iter = mMap.find(key);
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

#endif // __LYNX_COMMON_LIB_ATTRPOLISH_TABLE_H__