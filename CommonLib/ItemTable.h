#ifndef __LYNX_COMMON_LIB_ITEM_TABLE_H__
#define __LYNX_COMMON_LIB_ITEM_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ItemTemplate 
	{
		// 模板id
		TemplateId mTemplateId;

		// 背包标签tag
		UInt32	   mBagTab;

		//类型id
		UInt32	   mTypeId;
		//小类型
		UInt32	   mSubTypeId;
		//名称
		String	   mName;
		//描述
		String	   mDes;
		//图标
		String	   mIcon;
		//品质
		UInt32	   mQuality;

		//开始时间
		UInt64	   mBeginTime;

		//失效时间
		UInt64	   mInvalid;
		//可否使用
		UInt32     mUseful;
		//使用条件
		String		mUseLimit1;
		//使用条件
		String		mUseLimit2;
		//使用条件
		String		mUseLimit3;
		//使用条件
		String		mUseLimit4;
		//使用条件
		String		mUseLimit5;


		//最大使用限制
		UInt32		mAmountLimit;
		//使用次数
		UInt32		mUseTime;
		//效果指数
		UInt32		mPower;
		//CD
		UInt32		mCD;
		//堆叠数量
		UInt32		mStackAmount;
		//排序
		UInt32		mOrder;
		//可否出售
		UInt32		mSale;
		//售卖价格
		UInt32		mPrice;
		//包含规则
		String		mContent;

		LYNX_S11N_25(ItemTemplate, TemplateId, mTemplateId, UInt32, mBagTab,UInt32, mTypeId, 
									UInt32, mSubTypeId,String, mName, String, mDes, 
									String , mIcon, UInt32, mQuality, UInt64,  mBeginTime,
									UInt64	,mInvalid, UInt32 ,mUseful, String, mUseLimit1,
									String, mUseLimit2, String, mUseLimit3,String, mUseLimit4,
									String, mUseLimit5,UInt32 ,mAmountLimit, UInt32 ,mUseTime,
									 UInt32 ,mPower, UInt32 ,mCD, UInt32 ,	mStackAmount, 
									 UInt32, mOrder, UInt32 ,mSale, UInt32	,mPrice,
									 String, mContent
			         );

	};

	typedef Map<UInt32, ItemTemplate> ItemTemplateMap;


	struct ItemTable
	{
		ItemTemplateMap mMap;


		LYNX_S11N_1(ItemTable, ItemTemplateMap, mMap);

		ItemTemplate* get(UInt32 id)
		{
			ItemTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_ITEM_TABLE_H__


