#ifndef __LYNX_COMMON_LIB_ITEM_DATA_H__
#define __LYNX_COMMON_LIB_ITEM_DATA_H__

#include "Definition.h"

namespace Lynx
{
    

//==========================================================================================================
// BOC ItemData 数据结构 sec

	struct RandomAttribute
	{
		RandomAttribute():m_nKey(0),m_nValue(0),m_nQuality(0){}
		//装备带有的随机属性的种类，物攻，物防，魔防等等sec
		UInt32 m_nKey;
		//相应的种类对应的数值，注意，如果是暴击率，数值为万分比
		UInt32 m_nValue;

		UInt32 m_nQuality;

		LYNX_S11N_3(RandomAttribute,UInt32,m_nKey,UInt32,m_nValue,UInt32, m_nQuality);
	};

	struct IntrinsicAttribute
	{
		IntrinsicAttribute():m_nKey(0),m_nValue(0){}

		//固有属性对应的类型ID
		UInt32 m_nKey;
		//固有属性的数值
		UInt32 m_nValue;
		LYNX_S11N_2(IntrinsicAttribute,UInt32,m_nKey,UInt32,m_nValue);
	};


	//一些道具的属性字段，如装备初始化生成的时候会有属性sec
	struct EquipAttribute
	{
		EquipAttribute():m_nEquipState(0),m_nEnhanceLevel(0),m_nEnhanceExp(0),
			m_nStarLevel(0),m_nStarExp(0),m_nRefineTimes(0),m_nPurplePoints(0),m_nPurpleTimes(0),m_nOrangePoints(0),m_nOrangeTimes(0){}

		//装备状态
		UInt32 m_nEquipState;

		//强化等级
		UInt32 m_nEnhanceLevel;

		//强化经验
		UInt64 m_nEnhanceExp;  

		//星级
		UInt32 m_nStarLevel;

		//星级经验
		UInt64 m_nStarExp;

		//洗练次数
		UInt32 m_nRefineTimes;

		//每个装备的镶嵌的宝石，没有则为0，最多五个槽位
		List<Guid> m_listGems;

		//每个装备携带的固有属性列表
		List<IntrinsicAttribute> m_listIntrinsicAttrs;

		//每个装备携带的随机属性列表
		List<RandomAttribute> m_listRandomAttrs;

		UInt32 m_nPurplePoints;

		UInt32 m_nPurpleTimes;

		UInt32 m_nOrangePoints;

		UInt32 m_nOrangeTimes;

		RandomAttribute m_nOldAttribute;

		UInt32 m_nOldAttributeIndex;

		LYNX_S11N_15(EquipAttribute, UInt32, m_nEquipState,UInt32,m_nEnhanceLevel,
			UInt64, m_nEnhanceExp, UInt32, m_nStarLevel,UInt64, m_nStarExp,
			UInt32, m_nRefineTimes,List<Guid>, m_listGems,List<IntrinsicAttribute>, m_listIntrinsicAttrs,
			List<RandomAttribute>, m_listRandomAttrs,UInt32, m_nPurplePoints,UInt32, m_nPurpleTimes,
			UInt32, m_nOrangePoints,UInt32, m_nOrangeTimes, RandomAttribute, m_nOldAttribute,
			UInt32, m_nOldAttributeIndex
			);
	};

	struct ItemData 
	{
	
		ItemData() : m_nUid(0), m_nItemId(0), m_nCurStackAmount(0),m_nItemGetTime(0),m_nDirtyType(0),m_nInvalid(0) {

		}

		// 全局唯一的uuid
		Guid m_nUid;

		//道具在道具表中的ID
		Guid m_nItemId;

		// 当前叠加数量
		UInt32 m_nCurStackAmount;

		// 物品获得的时间
		UInt32 m_nItemGetTime;

		//一个装备属性的结构体
		EquipAttribute m_itemAttribute;

		List<UInt32> m_listParam;

		UInt32 m_nDirtyType;

		UInt32 m_nInvalid;

		

		LYNX_S11N_8(ItemData, Guid, m_nUid, Guid ,m_nItemId,
			UInt32 ,m_nCurStackAmount,UInt32, m_nItemGetTime, EquipAttribute, m_itemAttribute, List<UInt32>, m_listParam,
			UInt32,m_nDirtyType,UInt32, m_nInvalid);	

	};

	struct IdAndCount
	{
		UInt64 mItemID;
		UInt32 mCount;

		LYNX_S11N_2(IdAndCount, UInt64, mItemID, UInt32 ,mCount);	

	};

	struct ItemsChangeEle
	{
		UInt64 mItemUid;
		UInt32 mCurStackAmount;
		OperationType mOption;
		
	};

	//所有物品通用
	struct AllItemEle
	{
		AllItemEle():resType(0),  subType(0), count(0){}
		UInt64 resType;
		UInt64 subType;
		UInt32 count;
		
		LYNX_S11N_3(AllItemEle, UInt64, resType, UInt64 ,subType, UInt32, count);
	};

	//回传参数表示增加的物品数量
	
	//新增字段
//addCount1,(int)(表示增加的数量，当玩家没有用佣兵A，获取到A时此字段为1，表示获取一个佣兵) 
//addCount2,(int)(扩充字段，表示增加的数量，当玩家拥有佣兵A时，此时获取佣兵A，那么A按照配置表转化为碎片，
	//此字段表示转化后增加的碎片数量)

//客户端判断resType为佣兵时，两个参数addCount1表示增加的佣兵数量，addCount表示增加的碎片数量。
//当客户端判断resType为碎片时，addCount1必为0，addCount2表示新增碎片数量
//其余类型，addCount1表示该类型物品新增的数量，addCount2为0
   class JewelryData;
	struct ReturnItemEle
	{
		ReturnItemEle():resType(0),  subType(0), addCount1(0), addCount2(0), jewelryData(NULL){}
		UInt64 resType;
		UInt64 subType;
		//增加的个数，低于佣兵表示增加的佣兵数
		UInt32 addCount1;
		//扩展字段, 对于佣兵表示增加的碎片数
		UInt32 addCount2;
		
		//以下为扩充字段

		//饰品字段
		JewelryData * jewelryData;

		LYNX_S11N_5(ReturnItemEle, UInt64, resType, UInt64 ,subType, UInt32 ,addCount1,
			UInt32, addCount2, JewelryData *, jewelryData);
	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ITEM_DATA_H__