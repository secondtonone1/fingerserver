#ifndef __LYNX_COMMON_LIB_HOARSTONERUNE_TABLE_H__
#define __LYNX_COMMON_LIB_HOARSTONERUNE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HoarstoneRuneTemplate 
	{
		// 符文id
		UInt64 mId;

		// 符文名称
		String	   mName;
		// 符文资源
		String     mRes;

		UInt32     mAttrType1;

		UInt32     mAttrValue1;

		UInt32     mAttrType2;

		UInt32     mAttrValue2;

		UInt32     mAttrType3;

		UInt32     mAttrValue3;

		UInt32     mAttrType4;

		UInt32     mAttrValue4;

		UInt32     mAttrType5;

		UInt32     mAttrValue5;

		UInt32     mQuality;

		UInt64     mCombine1;

		UInt32     mAmount1;

		UInt64     mCombine2;

		UInt32     mAmount2;

		UInt64     mCombine3;

		UInt32     mAmount3;

		UInt64     mCombine4;

		UInt32     mAmount4;

		

		LYNX_S11N_22(HoarstoneRuneTemplate,UInt64, mId, String,mName,String,mRes,
			UInt32,mAttrType1,UInt32,mAttrValue1,UInt32,mAttrType2,UInt32,mAttrValue2,
			UInt32,mAttrType3,UInt32,mAttrValue3,UInt32,mAttrType4,UInt32,mAttrValue4,
			UInt32,mAttrType5,UInt32,mAttrValue5,UInt32,mQuality,UInt64,mCombine1,
			UInt32,mAmount1,UInt64,mCombine2,UInt32,mAmount2,UInt64,mCombine3,UInt32,mAmount3,
			UInt64,mCombine4,UInt32,mAmount4
			);

	};

	typedef Map<UInt64, HoarstoneRuneTemplate> HoarstoneRuneTemplateMap;


	struct HoarstoneRuneTable
	{
		HoarstoneRuneTemplateMap mMap;


		LYNX_S11N_1(HoarstoneRuneTable, HoarstoneRuneTemplateMap, mMap);

		HoarstoneRuneTemplate* get(UInt64 id)
		{
			HoarstoneRuneTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_HOARSTONERUNE_TABLE_H__


