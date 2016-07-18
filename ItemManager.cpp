#include "ItemManager.h"
#include "Player.h"
#include "GameServer.h"
#include "EquipManager.h"
#include "Table.h"
using namespace Lynx;

ItemManager::ItemManager() : mPlayer(NULL), m_pListItemDatas(NULL),m_nEquipOccupies(0),m_nItemOccupies(0),m_nOtherOccupies(0)
{

}


ItemManager::~ItemManager()
{

}

bool 
ItemManager::initial(Player* player)
{
    //钻石，金币等item的初始化
	if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
    m_pListItemDatas = &mPlayer->mPlayerData.mItemListData.m_listItems;
    
	for (List<ItemData>::Iter* it = m_pListItemDatas->begin();
        it != m_pListItemDatas->end();
        it = m_pListItemDatas->next(it))
    {
        //如果该物品已经被删除，那么跳过这个物品，继续统计下一个
		if(it->mValue.m_nInvalid)
		{
			continue;
		}

		//map 插入uid对应的 itemdata的指针 sec
		m_mapItemUuid.insert(it->mValue.m_nUid, &it->mValue);
		
		//通过id查找对应的Map<Guid,List<Guid> > 迭代器指针
		Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(it->mValue.m_nItemId);
		
		if(iterItemID2Uids)
		{
			//取出对应的uid List sec
			List<Guid> &uidlist = iterItemID2Uids->mValue;
			//在list 里查找对应的uid sec
			List<Guid>::Iter *iterIdList = uidlist.find(it->mValue.m_nUid,uidlist.begin(),uidlist.end());
			
			//没找到就插入到uidList里，找到就不做操作，只保留一份uid记录即可 sec
			if(!iterIdList)
			{
				uidlist.insertTail(it->mValue.m_nUid);
			}
		}
		else
		{
			//没找到itemID对应的 uid List,那么插入uid sec
			List<Guid> uidlist;
			uidlist.insertTail(it->mValue.m_nUid);
			m_mapItemId2Uids.insert(it->mValue.m_nItemId,uidlist);
		}

		//完成上面的uid map 和id2uid map,根据物品的类型将他放到不同类型的map里，
		//方便发送给客户端消息
		UInt64 itemTableID = it->mValue.m_nItemId;
		
		ItemTemplate * itemTemplate = ITEM_TABLE().get(itemTableID);
		
		if(itemTemplate->mBagTab == 1)
		{
			//判断是否是装备，如果是装备放入装备对应的map
			if(itemTemplate->mTypeId == EQUIPTYPEID)
			{
				//第一标签页下装备对应的map，为了保险，第一标签页下可能还有装备碎片之类的
				m_mapUidEquipType.insert(it->mValue.m_nUid, &it->mValue);
				assert(!it->mValue.m_nInvalid);
			}
			
			m_nEquipOccupies++;
		}
		else
		{
			if(itemTemplate->mBagTab == 2)
			{
				m_mapUidItemType.insert(it->mValue.m_nUid, &it->mValue);
				assert(!it->mValue.m_nInvalid);
				
				
				m_nItemOccupies++;
				
				
			}
			else
			{
				m_mapUidOtherType.insert(it->mValue.m_nUid, &it->mValue);
				assert(!it->mValue.m_nInvalid);
				m_nOtherOccupies++;
			}
		}
    }

    return true;
}

void 
ItemManager::release()
{
	//清除掉 m_mapItemId2Uids和m_mapItemUuid结构的iter sec
	
	m_mapItemUuid.clear();
	m_mapItemId2Uids.clear();
	m_pListItemDatas = NULL;
	mPlayer = NULL;

	m_mapUidEquipType.clear();
	m_mapUidItemType.clear();
	m_mapUidOtherType.clear();

	m_nEquipOccupies = 0;
	m_nItemOccupies = 0;
	m_nOtherOccupies = 0;
}


void 
ItemManager::destroyItem(Guid itemUid)
{
    delItem(itemUid, 0);
}

UInt32 
ItemManager::delItem(Guid itemUid, UInt32 count)
{    
	
	//新游戏删除道具 sec 优化删除功能

	Map<Guid, ItemData*>::Iter* it = m_mapItemUuid.find(itemUid);
	if (it->mValue->m_nUid == itemUid)
	{
		 // 减少数量不删除道具 sec
				
		 if (count && count < it->mValue->m_nCurStackAmount)
		 {
		     updateOneItem(it->mValue,it->mValue->m_nCurStackAmount - count);
								
		            return 1;
		 }

				//map里通过对应的uid删除itemdata的关联关系 sec
		 delOneItem(it->mValue);
		 return 1;
		        
	 }
	return 2;

}

void ItemManager::delOneItem(ItemData* itemData)
{
	itemData->m_nCurStackAmount = 0;
	m_mapItemUuid.erase(itemData->m_nUid);

	//通过itemID查找对应的uid List sec
	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(itemData->m_nItemId);
	assert(iterItemID2Uids);

	if(iterItemID2Uids)
	{
		//list 里 查找指定的uid sec
		List<Guid>& listItemUID = iterItemID2Uids->mValue;
		
		List<Guid>::Iter * iterUids = listItemUID.find(itemData->m_nUid,listItemUID.begin(),listItemUID.end());
		assert(iterUids);
		//找到后将对应的uid从list中删除 sec
		listItemUID.erase(iterUids);
		if(listItemUID.empty())
		{
			m_mapItemId2Uids.erase(iterItemID2Uids);
		}
	}

	//list删除uid对应的itemData数据 sec
	//m_pListItemDatas->erase(it); 已经改为标志位标记是否删除了
	
	delTypeMapEle(itemData);

	itemData->m_nInvalid = 1;
	if(itemData->m_nDirtyType == NODIRTY)
	{
		itemData->m_nDirtyType = DELDIRTY;
		mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);
	}
	
}

void ItemManager::delTypeMapEle(ItemData* itemData)
{
	//删除对应的类型存储map中的元素

	ItemTemplate * itemTemplate = ITEM_TABLE().get(itemData->m_nItemId);

	if(itemTemplate->mBagTab == 1)
	{
		if(itemTemplate->mTypeId == EQUIPTYPEID)
		{
			m_mapUidEquipType.erase(itemData->m_nUid);
		}
		
		m_nEquipOccupies--;
	}
	else
	{
		if(itemTemplate->mBagTab == 2)
		{
			m_mapUidItemType.erase(itemData->m_nUid);
			m_nItemOccupies--;
		}
		else
		{
			m_mapUidOtherType.erase(itemData->m_nUid);
			m_nOtherOccupies--;
		}
	}
}

bool 
ItemManager::isItemFull(UInt32 bagTab)
{
   if(bagTab == 1)
   {
		return (MAXITEMCOUNT <= m_nEquipOccupies);
   }
   else if(bagTab == 2)
   {
		return(MAXITEMCOUNT <= m_nItemOccupies);
   }
   else
   {
		return(MAXITEMCOUNT <= m_nOtherOccupies);
   }

}

UInt32 ItemManager::getEmputyNum(UInt32 bagTab)
{
	if(bagTab == 1)
	{
		return (MAXITEMCOUNT - m_nEquipOccupies);
	}
	else if(bagTab == 2)
	{
		return(MAXITEMCOUNT - m_nItemOccupies);
	}
	else
	{
		return(MAXITEMCOUNT - m_nOtherOccupies);
	}
}

errorId 
ItemManager::useItem(const Guid& itemUid, UInt32 count)
{
	//sec BOC 使用背包物品
	Map<Guid, ItemData*>::Iter * iterMap = m_mapItemUuid.find(itemUid);

	if(!iterMap)
	{
		LOG_WARN("not find this item");
		return LynxErrno::ItemNotExist;
	}

	assert(iterMap);

	//如果该物品已经被删除了，返回错误码
	if(iterMap->mValue->m_nInvalid)
	{
		return LynxErrno::ItemNotExist;
	}

	Guid itemID = iterMap->mValue->m_nItemId;

	ItemTemplate* itemTemplate = ITEM_TABLE().get(itemID); 

	if (!itemTemplate->mUseful)
	{
		LOG_WARN("Item can not use,templateid: %llu", itemID);
		return LynxErrno::InvalidParameter;
	}

	if (iterMap->mValue->m_nCurStackAmount < count)
	{
		return LynxErrno::InvalidParameter;
	}
	//添加逻辑效果
	switch(itemTemplate->mTypeId)
	{

	case EQUIPTYPEID:
		{

		}
		break;
	case COSTTYPEID:
		{

		}
		break;

	default:

		return LynxErrno::None;
	}

	delItem(itemUid, count);
	return LynxErrno::None;
}

UInt32 ItemManager::getNoneEmptyBags(UInt32 bagTab)
{
	if(bagTab == 1)
	{
		return  m_nEquipOccupies;
	}
	else if(bagTab == 2)
	{
		return m_nItemOccupies;
	}
	else
	{
		return  m_nOtherOccupies;
	}

	
}

void ItemManager::updateOneItem(ItemData* itemData, UInt32 StackAmount)
{
	itemData->m_nCurStackAmount = StackAmount;
	
	if(itemData->m_nDirtyType == NODIRTY)
	{
		itemData->m_nDirtyType = UPDATEDIRTY;
		mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);
	}
	
	
}


UInt64 ItemManager::addOneItem(Guid templateId, UInt32 stackAmount)
{
	UInt64 itemUid = LogicSystem::getSingleton().generateItemGuid();
	ItemData itemData;
	itemData.m_nUid = itemUid;
	itemData.m_nItemId = templateId;
	itemData.m_nItemGetTime = TimeUtil::getTimeSec();
	itemData.m_nCurStackAmount = stackAmount;
	for(unsigned int i = 0; i < PARAMSIZE; i++)
	{
		UInt64 ele = 0;
		itemData.m_listParam.insertTail(ele);
	}

	List<ItemData>::Iter *it = m_pListItemDatas->insertTail(itemData);
	//将物品uid和value 对应插入map里
	m_mapItemUuid.insert(itemUid, &it->mValue);

	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(it->mValue.m_nItemId);

	if(iterItemID2Uids)
	{
		//取出对应的uid List sec	
		iterItemID2Uids->mValue.insertTail(it->mValue.m_nUid);		
	}
	else
	{
		//没找到itemID对应的 uid List,那么插入uid sec
		List<Guid> uidlist;
		uidlist.insertTail(it->mValue.m_nUid);
		m_mapItemId2Uids.insert(it->mValue.m_nItemId,uidlist);
	}

	addTypeMapEle(&it->mValue);

	//添加dirty标记
	it->mValue.m_nDirtyType = ADDDIRTY;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);

	return itemUid;
}

void ItemManager::addTypeMapEle(ItemData* itemData)
{
	//将物品根据类型，插入对应的类型map里
	ItemTemplate * itemTemplate =  ITEM_TABLE().get(itemData->m_nItemId);

	if(itemTemplate->mBagTab == 1)
	{
		if(itemTemplate->mTypeId == EQUIPTYPEID)
		{
			m_mapUidEquipType.insert(itemData->m_nUid, itemData);
		}
		
		m_nEquipOccupies++;
		
		//如果是装备，添加装备属性的随机生成
		addEquipAttribute(itemData);
	}
	else
	{
		if(itemTemplate->mBagTab == 2)
		{
			m_mapUidItemType.insert(itemData->m_nUid, itemData);
			m_nItemOccupies++;
		}
		else
		{
			m_mapUidOtherType.insert(itemData->m_nUid, itemData);
			m_nOtherOccupies++;
		}
	}

}

UInt32 ItemManager::getRandomValue(UInt32 minValue, UInt32 maxValue)
{
	UInt32 diffValue = maxValue - minValue;

	if (diffValue ==0)//todo: 0 的bug
	{
		return minValue;
	}

	UInt32 result = rand()%diffValue;

	return minValue + result + 1;
	
}

List<UInt64> ItemManager::addItems(Guid templateId, UInt32 count)
{
	List<UInt64> listUids;
	ItemTemplate* itemTemplate = ITEM_TABLE().get(templateId);
	//物品最大叠加数量  sec
	UInt32 maxStackAmount = itemTemplate->mStackAmount;
	
	//全部添加到背包里，占用newbags个背包格

	while(count > maxStackAmount )
	{
		count -= maxStackAmount;
		UInt64 uid = addOneItem(templateId, maxStackAmount);
		listUids.insertTail(uid);
				
	}
			
	if(count > 0 )
	{
		UInt64 uid =addOneItem(templateId,count);
		listUids.insertTail(uid);
	}
	return listUids;	
}

//计算占用几个背包格
UInt32 ItemManager::calculateCount(Guid itemId, UInt32 count)
{
	ItemTemplate* itemTemplate = ITEM_TABLE().get(itemId);
	//物品最大叠加数量  sec
	UInt32 maxStackAmount = itemTemplate->mStackAmount;

	//将要新占用的背包格数量
	UInt32 newBags = 0;

	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(itemId);

		//没找到itemid 对应的uuid列表，说明背包里没有此类物品，生成新的itemdata,占用一个或多个背包格
	if(!iterItemID2Uids)
	{
		if(count%maxStackAmount)
		{
			newBags = count/maxStackAmount + 1;
		}
		else
		{
			newBags = count/maxStackAmount;
		}
	}
	else
	{
		//找到itemid对应的uuid列表，说明背包里有此类物品
		List<Guid> &uidList = iterItemID2Uids->mValue;
		for(List<Guid>::Iter * iterUids = uidList.begin(); iterUids != NULL; iterUids = uidList.next(iterUids))
		{
			Guid uid = iterUids->mValue;
	
			Map<Guid, ItemData*>::Iter *iterItemData =  m_mapItemUuid.find(uid);
			
			assert(!iterItemData->mValue->m_nInvalid);

			if(iterItemData->mValue->m_nInvalid)
			{
				continue;
			}
			
			UInt32 currStackAmount = iterItemData->mValue->m_nCurStackAmount;
			//找到未堆叠满的
			if(currStackAmount < maxStackAmount)
			{
				//数量超过此格子最大叠加数量，则填满
				if(count + currStackAmount > maxStackAmount)
				{
					count -=  (maxStackAmount - currStackAmount);
					break;
				}
				else 
				{					
					count = 0;
					break;
				}


			}

		}

		//将剩余的物品占有新的背包格	
		if(count > 0)
		{
			if(count%maxStackAmount)
			{
				newBags = count/maxStackAmount + 1;
			}
			else
			{
				newBags = count/maxStackAmount;
			}
		}

	}
	
	return newBags;
}

List<ItemData>* ItemManager::getItemDatasList(void)
{
	return m_pListItemDatas;
}

//服务器获取到新的物品
 // 新游戏获取物品,templateId 为itemid
List<UInt64> ItemManager::getNewItems(Guid itmeId, UInt32 count)
{
	//用于
	List<UInt64> itemUidLists;

	ItemTemplate* itemTemplate = ITEM_TABLE().get(itmeId);
	//物品最大叠加数量  sec
	UInt32 maxStackAmount = itemTemplate->mStackAmount;
	
	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(itmeId);
	
	//没找到itemid 对应的uuid列表，说明背包里没有此类物品，生成新的itemdata,占用一个或多个背包格
	if(!iterItemID2Uids)
	{
		itemUidLists = addItems(itmeId,count);
	}
	else
	{
		//找到itemid对应的uuid列表，说明背包里有此类物品
		List<Guid> &uidList = iterItemID2Uids->mValue;
		for(List<Guid>::Iter * iterUids = uidList.begin(); iterUids != NULL; iterUids = uidList.next(iterUids))
		{
			Guid uid = iterUids->mValue;
			Map<Guid, ItemData*>::Iter *iterItemData =  m_mapItemUuid.find(uid);
		
			//如果该物品已经被删除
			assert(!iterItemData->mValue->m_nInvalid);
			if(iterItemData->mValue->m_nInvalid)
			{
				continue;
			}

			UInt32 currStackAmount = iterItemData->mValue->m_nCurStackAmount;
			//找到未堆叠满的
			if(currStackAmount < maxStackAmount)
			{
				//数量超过此格子最大叠加数量，则填满
				if(count + currStackAmount > maxStackAmount)
				{
					count -=  (maxStackAmount - currStackAmount);
					updateOneItem(iterItemData->mValue, maxStackAmount);
					itemUidLists.insertTail(iterItemData->mValue->m_nUid);
					break;
				}
				else 
				{
					currStackAmount += count;
					updateOneItem(iterItemData->mValue,currStackAmount);
					count = 0;
					itemUidLists.insertTail(iterItemData->mValue->m_nUid);
					break;
				}
				
				
			}
			
		}

		//将剩余的物品占有新的背包格	
		if(count > 0)
		{
			itemUidLists += addItems(itmeId,count);

		}
		
	}

	return itemUidLists;
}


List<UInt64> ItemManager::getNewItemsDefrent(List<IdAndCount> itemsList)
{
	UInt32 newItemOccupies = 0;
	UInt32 newEquipOccupies = 0;
	UInt32 newOtherOccupies = 0;

	List<UInt64> newItemUids;
	for(List<IdAndCount>::Iter * itemsIter = itemsList.begin(); itemsIter != NULL; itemsIter = itemsList.next(itemsIter))
	{
		UInt64 &itemID = itemsIter->mValue.mItemID;
		UInt32 &itemCount = itemsIter->mValue.mCount;
		ItemTemplate *itemTemplate = ITEM_TABLE().get(itemID);

		if (itemTemplate == NULL)
		{
			return newItemUids;
		}
		
		if(itemTemplate->mBagTab == 1)
		{
			newEquipOccupies += calculateCount(itemID,itemCount);
		}
		else if(itemTemplate->mBagTab == 2)
		{
			newItemOccupies += calculateCount(itemID,itemCount);
		}
		else
		{
			newOtherOccupies += calculateCount(itemID,itemCount);
		}

	}

	if(m_nEquipOccupies + newEquipOccupies > MAXITEMCOUNT || m_nItemOccupies + newItemOccupies > MAXITEMCOUNT 
		|| m_nOtherOccupies + newOtherOccupies > MAXITEMCOUNT)
	{
		//
		newItemUids.clear();
		return newItemUids;
	}
	else
	{
		for(List<IdAndCount>::Iter * itemsIter = itemsList.begin(); itemsIter != NULL; itemsIter = itemsList.next(itemsIter))
		{
			UInt64 &itemID = itemsIter->mValue.mItemID;
			UInt32 &itemCount = itemsIter->mValue.mCount;
			ItemTemplate *itemTemplate = ITEM_TABLE().get(itemID);
			
			if(itemTemplate == NULL)
			{
				LOG_WARN("itemid isn't in table %llu", itemID);
				continue;
			}

			newItemUids += getNewItems(itemID, itemCount);

		}

		return newItemUids;
	}
	
}

List<UInt64> ItemManager::getNewItemsSame(Guid templateId, UInt32 count)
{
	ItemTemplate *itemTemplate = ITEM_TABLE().get(templateId);
	
	UInt32 newItemOccupies = 0;
	UInt32 newEquipOccupies = 0;
	UInt32 newOtherOccupies = 0;

	List<UInt64> newItemUids;

	if (itemTemplate == NULL)
	{
		return newItemUids;
	}
	if(itemTemplate->mBagTab == 1)
	{
		newEquipOccupies += calculateCount(templateId,count);
	}
	else if(itemTemplate->mBagTab == 2)
	{
		newItemOccupies += calculateCount(templateId,count);
	}
	else
	{
		newOtherOccupies += calculateCount(templateId,count);
	}

	if(m_nEquipOccupies + newEquipOccupies > MAXITEMCOUNT || m_nItemOccupies + newItemOccupies > MAXITEMCOUNT 
		|| m_nOtherOccupies + newOtherOccupies > MAXITEMCOUNT)
	{
		//
		newItemUids.clear();
		return newItemUids;
	}
	else
	{
		newItemUids += getNewItems(templateId, count);
		
		return newItemUids;
	}
}


 ItemData* 
ItemManager::getItemByGuid(const Guid& itemGuid)
{
	Map<Guid, ItemData*>::Iter* findIter =  m_mapItemUuid.find(itemGuid);
	
	if(findIter)
	{
		return findIter->mValue;
	}
	else
	{
		return NULL;
	}
}



errorId
ItemManager::sellItem(const List<Guid>& itemList, UInt32& getGold)
{
	for (List<Guid>::Iter* it = itemList.begin();
	    it != NULL; it = itemList.next(it))
	{
	    Map<Guid, ItemData*>::Iter* existIter = m_mapItemUuid.find(it->mValue);
	    if (!existIter)
	    {
	        LOG_WARN("Item %llu not exit", it->mValue);
	        return LynxErrno::InvalidParameter;
	    }

	    ItemTemplate* itemTemplate = ITEM_TABLE().get(existIter->mValue->m_nItemId);
	    if (!itemTemplate)
	    {
	        LOG_WARN("Failed to get ItemTemplate by Id :%u", existIter->mValue->m_nItemId);
			return LynxErrno::ItemNotExist;
	    }

	    if (!itemTemplate->mSale)
	    {
	        return LynxErrno::InvalidParameter;
	    }
	}
	
	for (List<Guid>::Iter* it = itemList.begin();
	    it != NULL; it = itemList.next(it))
	{
	    Map<Guid, ItemData*>::Iter* existIter = m_mapItemUuid.find(it->mValue);
		assert(!existIter->mValue->m_nInvalid);
	    ItemTemplate* itemTemplate = ITEM_TABLE().get(existIter->mValue->m_nItemId);
	    //加钱操作
	    //mPlayer->increasePlayerGold(itemTemplate->mPrice * existIter->mValue->m_nCurStackAmount);
	    getGold += itemTemplate->mPrice * existIter->mValue->m_nCurStackAmount;
	    delItem(existIter->mValue->m_nUid);
	}
	
	mPlayer->mPlayerData.mBaseData.m_nCoin += getGold;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);

	return LynxErrno::None;
}

std::string ItemManager::convertItemDataToJson()
{
	MsgItemDataToJson itemDataToJson;
	itemDataToJson.m_mapUidEquipType = m_mapUidEquipType;
	itemDataToJson.m_mapUidItemType = m_mapUidItemType;
	itemDataToJson.m_mapUidOtherType = m_mapUidOtherType;
	
	return itemDataToJson.convertDataToJson();

}

void ItemManager::addEquipAttribute(ItemData* itemData)
{
	//固有属性
	EquipTemplate * equipTemplate = EQUIP_TABLE().get(itemData->m_nItemId);

	UInt32 minValue1 = equipTemplate->mStaticAttrValMin1;
	UInt32 maxValue1 = equipTemplate->mStaticAttrValMax1;

	IntrinsicAttribute intrinsicAttr1;
	intrinsicAttr1.m_nKey = equipTemplate->mStaticAttrType1;
	UInt32 randomValue1 = getRandomValue(minValue1,maxValue1);
	intrinsicAttr1.m_nValue = randomValue1;

	itemData->m_itemAttribute.m_listIntrinsicAttrs.insertTail(intrinsicAttr1);


	UInt32 minValue2 = equipTemplate->mStaticAttrValMin2;
	UInt32 maxValue2 = equipTemplate->mStaticAttrValMax2;

	IntrinsicAttribute intrinsicAttr2;
	intrinsicAttr2.m_nKey = equipTemplate->mStaticAttrType2;
	UInt32 randomValue2 = getRandomValue(minValue2,maxValue2);
	intrinsicAttr2.m_nValue = randomValue2;

	itemData->m_itemAttribute.m_listIntrinsicAttrs.insertTail(intrinsicAttr2);


	UInt32 minValue3 = equipTemplate->mStaticAttrValMin3;
	UInt32 maxValue3 = equipTemplate->mStaticAttrValMax3;

	IntrinsicAttribute intrinsicAttr3;
	intrinsicAttr3.m_nKey = equipTemplate->mStaticAttrType3;
	UInt32 randomValue3 = getRandomValue(minValue3,maxValue3);
	intrinsicAttr3.m_nValue = randomValue3;
	itemData->m_itemAttribute.m_listIntrinsicAttrs.insertTail(intrinsicAttr3);

	if(equipTemplate->mRandomRuleId1)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId1);
	}

	if(equipTemplate->mRandomRuleId2)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId2);
	}

	if(equipTemplate->mRandomRuleId3)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId3);
	}

	if(equipTemplate->mRandomRuleId4)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId4);
	}

	if(equipTemplate->mRandomRuleId5)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId5);
	}
}

void ItemManager::addRandomAttribute(ItemData* itemData, UInt32 randomID)
{
	AttrRandomTemplate* attrRandom = ATTRRANDOM_TABLE().get(randomID);

	//PA 百分比的最大值
	UInt32 PAPercent = attrRandom->mPARate;

	//MA 百分比的最大值，判断时要看随机出来的值是否在 PA 和 MA之间
	UInt32 MAPercent = PAPercent + attrRandom->mMARate;

	UInt32 PFPercent = MAPercent + attrRandom->mPFRate;

	UInt32 MFPercent = PFPercent + attrRandom->mMFRate;

	UInt32 MaxHPPercent = MFPercent + attrRandom->mMaxHPRate;

	UInt32 MaxMPPercent = MaxHPPercent + attrRandom->mMaxMPRate;

	UInt32 MaxSPPercent = MaxMPPercent + attrRandom->mMaxSPRate;

	UInt32 CritRatePercent = MaxSPPercent + attrRandom->mCritRateRate;

	UInt32 CritDamagePercent = CritRatePercent + attrRandom->mCritDamageRate;

	UInt32 CritDefPercent =  CritDamagePercent + attrRandom->mCritDamageRate;

	UInt32 randomRes = rand()%100;

	RandomAttribute randomAttr;

	randomAttr.m_nQuality = attrRandom->mQuality;

	//如果随机出来的值大于等于critDamagePercent上限值，小于CritDefPercent上限值,那么随机出来的属性为critDef
	if(randomRes < CritDefPercent && randomRes >= CritDamagePercent)
	{
		randomAttr.m_nKey = BaseToughness;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDefMin,attrRandom->mCritDefMax);
		randomAttr.m_nValue = randomValue;
	}
	//属性为critdamage
	else	if(randomRes >= CritRatePercent)
	{
		randomAttr.m_nKey = BaseCrit;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDamageMin,attrRandom->mCritDamageMax);
		randomAttr.m_nValue = randomValue;
	}
	//大于maxSPPercent最大值，小于CritRate，属性为critrate
	else if(randomRes >= MaxSPPercent)
	{
		randomAttr.m_nKey = Critrate;
		UInt32 randomValue = getRandomValue(attrRandom->mCritRateMin,attrRandom->mCritRateMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MaxMPPercent)
	{
		//属性为MAXsp
		randomAttr.m_nKey = MaxSP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxSPMin,attrRandom->mMaxSPMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MaxHPPercent)
	{
		//属性为MaxMP
		randomAttr.m_nKey = MaxMP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxMPMin, attrRandom->mMaxMPMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MFPercent)
	{
		//属性为最大HP
		randomAttr.m_nKey = MaxHP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxHPMin, attrRandom->mMaxHPMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= PFPercent)
	{
		//属性为MFPercent
		randomAttr.m_nKey = MF;
		UInt32 randomValue = getRandomValue(attrRandom->mMFMin,attrRandom->mMFMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MAPercent)
	{
		//属性为PFPercent
		randomAttr.m_nKey = PF;
		UInt32 randomValue = getRandomValue(attrRandom->mPFMin, attrRandom->mPFMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= PAPercent)
	{
		//属性为MAPercent
		randomAttr.m_nKey = MA;
		UInt32 randomValue = getRandomValue(attrRandom->mMAMin,attrRandom->mMAMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= 0)
	{
		//属性为PAPercent
		randomAttr.m_nKey = PA;
		UInt32 randomValue = getRandomValue(attrRandom->mPAMin,attrRandom->mPAMax);
		randomAttr.m_nValue = randomValue;
	}
	else
	{
		assert(false);
	}

	itemData->m_itemAttribute.m_listRandomAttrs.insertTail(randomAttr);
}

UInt32 ItemManager::getEnhanceLevelLimit(UInt32 starLv)
{
	UInt32 lvLimit = 0;
	switch(starLv)
	{
	case 0:
		lvLimit = 10;
		break;
	case 1:
		lvLimit = 12;
		break;
	case 2:
		lvLimit = 14;
		break;
	case 3:
		lvLimit = 16;
		break;
	case 4:
		lvLimit = 18;
		break;
	case 5:
		lvLimit = 20;
		break;
	default:
		break;

	}
	return lvLimit;
}

UInt32 ItemManager::getTotalEnhanceLv(UInt32 starLv, UInt32 curEnhanceLv)
{
	UInt32 totalLv = 0;
	
	//统计当前星级之前，所有等级累加，如当前星级为2星1级，那么统计0星和1星满级对应的强化等级
	for(UInt32 i = 0; i < starLv; i++)
	{
		totalLv += getEnhanceLevelLimit(i)+1;
	}

	//再加上当前星级中的强化等级
	totalLv += curEnhanceLv;
	
	return totalLv;
}

void ItemManager::enhanceEquip(UInt64 equipID, List<UInt64> materialLists)
{
	Map<Guid, ItemData*>::Iter * iterFind = m_mapItemUuid.find(equipID);
	
	if(!iterFind)
	{
		LOG_INFO("NO Equip IDs!");
		//提示玩家装备失效，信息以后写
		return;
	}
		
	UInt32 enhanceLevel = iterFind->mValue->m_itemAttribute.m_nEnhanceLevel;
	UInt32 starLevel = iterFind->mValue->m_itemAttribute.m_nStarLevel;
	//要强化装备的配置表信息
	EquipTemplate * equipEnhance = EQUIP_TABLE().get(iterFind->mValue->m_nItemId);
	UInt32 starMax = equipEnhance->mMaxStar;
	UInt32 maxLvLimit = getEnhanceLevelLimit(starMax);
	if(starLevel == starMax && enhanceLevel == maxLvLimit)
	{
		LOG_INFO("yidadaozuidaqianghuadengji");
		//通知玩家
		return;
	}
	
	UInt64 enhanceExp = iterFind->mValue->m_itemAttribute.m_nEnhanceExp;
	//统计材料所有的经验
	UInt64 totalExp = 0;
	for(List<UInt64>::Iter * iterMaterial = materialLists.begin(); iterMaterial != NULL; iterMaterial = materialLists.next(iterMaterial))
	{
		Map<Guid, ItemData*>::Iter * materialFind = m_mapItemUuid.find(iterMaterial->mValue);
	
		if(!materialFind)
		{
			continue;
		}

		ItemTemplate * materialTemplate = ITEM_TABLE().get(materialFind->mValue->m_nItemId);

		//由于强化石代表的经验还没有给出表，所以先填写装备材料的处理逻辑
		if(materialTemplate->mTypeId == EQUIPTYPEID)
		{
			EquipTemplate * equipTemplate = EQUIP_TABLE().get(materialFind->mValue->m_nItemId);
			totalExp += equipTemplate->mMaterialExp;
			delItem(iterMaterial->mValue,1);
		}
		else if(materialTemplate->mTypeId == ENHANCETYPEID)
		{
			
			totalExp += materialTemplate->mPower;
			delItem(iterMaterial->mValue,1);
		}
		
	}

	enhanceExp += totalExp;

	//UInt32 totalEnhanceLv = getTotalEnhanceLv(starLevel,enhanceLevel);

	AttrEnhanceTemplate* curEnhanceTemp = ATTRENHANCE_TABLE().reverseGet(starLevel,enhanceLevel);
	
	//表中配置的id是从当前级别升级到下一个级别的属性，所以当前id +1找到对应的表项
	UInt64 nextID = curEnhanceTemp->mID + 1;
	AttrEnhanceTemplate * attrEnhanceTemp = ATTRENHANCE_TABLE().get(nextID);

	UInt64 enhanceExpLimit =attrEnhanceTemp ->mExp;

	UInt64 needCoin = attrEnhanceTemp ->mCostCoin;

	UInt64 playerCoin = mPlayer->getPlayerCoin();

	if(playerCoin < needCoin)
	{
		//提示玩家错误信息，金币不足
		LOG_INFO("Not enouph gold!");
		return ;
	}
	else
	{
		playerCoin = playerCoin - needCoin;
		mPlayer->setPlayerCoin(playerCoin);
	}
	
	//循环判断，如果物品此时获得经验值后总经验值大于强化经验值上限，那么进行升级操作
	while(enhanceExp >= enhanceExpLimit)
	{
		
			//获取该星级的强化等级上限
			UInt32 enhanceLvLimit = getEnhanceLevelLimit(starLevel);
			
			if(enhanceLevel < enhanceLvLimit)
			{
				enhanceLevel++;
				nextID++;
				//总经验值扣除本级上限值后
				enhanceExp = enhanceExp - enhanceExpLimit;
				//计算下一级别的经验值
				AttrEnhanceTemplate * attrEnhanceTemplate = ATTRENHANCE_TABLE().get(nextID);
				enhanceExpLimit = attrEnhanceTemplate->mExp;
			}
			else 
			{
				if(starLevel < equipEnhance->mMaxStar)
				{
					//星级等级增加
					starLevel++;
					//总的强化等级增加
					nextID++;
					//当前星级下的等级置0
					enhanceLevel = 0;

					//总经验值扣除本级上限值后
					enhanceExp = enhanceExp - enhanceExpLimit;
					//计算下一级别的经验值
					AttrEnhanceTemplate * attrEnhanceTemplate = ATTRENHANCE_TABLE().get(nextID);
					enhanceExpLimit = attrEnhanceTemplate->mExp;
					

				}
				else
				{
					//提示玩家满星，满级，不再溢出，消息以后写
					break;

				}
			}
	}

	//属性加成就不存盘了，只保存星级和强化等级，以及强化经验
	iterFind->mValue->m_itemAttribute.m_nStarLevel = starLevel;
	iterFind->mValue->m_itemAttribute.m_nEnhanceExp = enhanceExp;
	iterFind->mValue->m_itemAttribute.m_nEnhanceLevel = enhanceLevel;

	iterFind->mValue->m_nDirtyType = UPDATEDIRTY;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT|BASEDATABIT);

	//通知玩家强化成功，以及金币数的更新，消息以后写
	Json::Value root;
	root["errorId"] = 0;
	root["equipuid"] = iterFind->mValue->m_nUid;
	root["enhancelv"] = iterFind->mValue->m_itemAttribute.m_nEnhanceLevel;
	root["enhanceStar"] = iterFind->mValue->m_itemAttribute.m_nStarLevel;
	root["enhanceExp"] = iterFind->mValue->m_itemAttribute.m_nEnhanceExp;
	root["nowCoin"] = playerCoin;

	for(List<UInt64>::Iter * iterMaterial = materialLists.begin(); iterMaterial != NULL; iterMaterial = materialLists.next(iterMaterial))
	{
		Map<Guid, ItemData*>::Iter * materialFind = m_mapItemUuid.find(iterMaterial->mValue);

		if(!materialFind)
		{
			Json::Value itemEle;
			itemEle["uid"] = iterMaterial->mValue;
			itemEle["count"] = 0;
			
			root["itemList"].append(itemEle);
		}
		else
		{
			Json::Value itemEle;
			itemEle["uid"] = iterMaterial->mValue;
			itemEle["count"] = materialFind->mValue->m_nCurStackAmount;
			
			root["itemList"].append(itemEle);
		}

	}

	Json::FastWriter writer;
	std::string respJson = writer.write(root);

	EquipEhanceResp enhanceResp;
	enhanceResp.mPacketID = EQUIP_ENHANCE_RESP;
	enhanceResp.mRespJsonStr = respJson;
	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);


}

void  ItemManager::polishEquip(UInt64 objEquipUid, UInt64 materialEquipUid, UInt32 objRdAttrIndex)
{
	
	
	Map<Guid, ItemData*>::Iter * objEquipIter = m_mapItemUuid.find(objEquipUid);
	Map<Guid, ItemData*>::Iter * materialEquipIter = m_mapItemUuid.find(materialEquipUid);

	if(!objEquipUid || !materialEquipIter)
	{
		LOG_WARN("error uid ");

		EquipPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		root["equipuid"] = objEquipUid;
		root["newAttr"]["type"] = 0;
		root["newAttr"]["value"] = 0;
		root["newAttr"]["quality"] = 0;

		root["oldAttr"]["type"] = 0;
		root["oldAttr"]["value"] = 0;
		root["oldAttr"]["quality"] = 0;
		root["objAttrIndex"] = objRdAttrIndex;
		root["materialAttrIndex"] = 0;

		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
		

		return;
	}

	ItemData * objectItemData = objEquipIter->mValue;
	ItemData * materialItemData = materialEquipIter->mValue;

	EquipTemplate * materialTemplate = EQUIP_TABLE().get(materialEquipIter->mValue->m_nItemId);

	EquipTemplate * objectTemplate = EQUIP_TABLE().get(objEquipIter->mValue->m_nItemId);

	if(objectTemplate->mRefreshcoin > mPlayer->mPlayerData.mBaseData.m_nCoin)
	{
		EquipPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;
		root["equipuid"] = objEquipUid;
		root["newAttr"]["type"] = 0;
		root["newAttr"]["value"] = 0;
		root["newAttr"]["quality"] = 0;

		root["oldAttr"]["type"] = 0;
		root["oldAttr"]["value"] = 0;
		root["oldAttr"]["quality"] = 0;
		root["objAttrIndex"] = objRdAttrIndex;
		root["materialAttrIndex"] = 0;

		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
		return;
	}

	Map<Guid,List<Guid> >::Iter * polishStoneListIter = m_mapItemId2Uids.find(PolishStoneID);
	ItemTemplate * polishStoneTemplate = ITEM_TABLE().get(PolishStoneID);
	

	UInt32 totalCountPolishStone = 0;
	UInt32 notFullUid = 0;
	//标识已经堆叠满的洗练石uid
	List<UInt64> listPolishStoneUids;

	//遍历，构造堆叠满的洗练石uid List，并且找到未堆叠满的uid
	for(List<Guid>::Iter * polishStoneIter = polishStoneListIter->mValue.begin(); polishStoneIter != NULL; 
		polishStoneIter = polishStoneListIter->mValue.next(polishStoneIter))
	{
		Map<Guid, ItemData*>::Iter * polishDataIter =  m_mapItemUuid.find(polishStoneIter->mValue);
		totalCountPolishStone += polishDataIter->mValue->m_nCurStackAmount;

		if(polishDataIter->mValue->m_nCurStackAmount < polishStoneTemplate->mStackAmount)
		{
			notFullUid = polishStoneIter->mValue;
		}
		else
		{
			listPolishStoneUids.insertTail(polishStoneIter->mValue);
		}
	}

	//洗练石数量不够，提示玩家
	if(totalCountPolishStone < objectTemplate->mRefreshstone)
	{
		EquipPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::MaterialNotEnough;
		root["equipuid"] = objEquipUid;
		root["newAttr"]["type"] = 0;
		root["newAttr"]["value"] = 0;
		root["newAttr"]["quality"] = 0;

		root["oldAttr"]["type"] = 0;
		root["oldAttr"]["value"] = 0;
		root["oldAttr"]["quality"] = 0;
		root["objAttrIndex"] = objRdAttrIndex;
		root["materialAttrIndex"] = 0;

		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
		return;
	}

	//目前先不考虑金币消耗和材料洗练石消耗
	//以后添加洗练石和金币消耗逻辑(现已添加)

	List<polishStoneEle> polishStoneLists;

	UInt32 needPolishStoneCount = objectTemplate->mRefreshstone;
	if(notFullUid)
	{
		//删除未堆叠满的，并且更改需求洗练石数量
		Map<Guid, ItemData*>::Iter * itemDataIter = m_mapItemUuid.find(notFullUid);
		
		//needPolishStoneCount 小于当前叠加数量，并且不为0
		if(needPolishStoneCount < itemDataIter->mValue->m_nCurStackAmount )
		{
			if(needPolishStoneCount)
			{
				delItem(notFullUid, needPolishStoneCount);

				polishStoneEle stonesEle;
				stonesEle.count = itemDataIter->mValue->m_nCurStackAmount;
				stonesEle.uid = notFullUid;
				polishStoneLists.insertTail(stonesEle);
			}
					
		}
		else
		{
			//由于需要的洗练石数量大于背包中未堆叠满的那个格子里洗练石的数量，所以删除这个格子，并且更新洗练石数量
			needPolishStoneCount -= itemDataIter->mValue->m_nCurStackAmount;
			delItem(notFullUid);

			polishStoneEle stonesEle;
			stonesEle.count = 0;
			stonesEle.uid = notFullUid;
            polishStoneLists.insertTail(stonesEle);

			//处理堆叠满的格子
			for(List<UInt64>::Iter * stoneUidIter = listPolishStoneUids.begin(); stoneUidIter != NULL; 
				stoneUidIter = listPolishStoneUids.next(stoneUidIter))
			{
				if(needPolishStoneCount == 0)
				{
					break;
				}

				if(needPolishStoneCount > polishStoneTemplate->mStackAmount)
				{
					//删除那些堆叠满的
					needPolishStoneCount -= polishStoneTemplate->mStackAmount;
					delItem(stoneUidIter->mValue);

					polishStoneEle stonesEle;
					stonesEle.count = 0;
					stonesEle.uid = stoneUidIter->mValue;
					polishStoneLists.insertTail(stonesEle);

				}
				else
				{
					//清除一部分数据
					delItem(stoneUidIter->mValue,needPolishStoneCount);
					//不足以全部删除，堆叠满的-需要的，将剩余的数量和id放入list里

					polishStoneEle stonesEle;
					stonesEle.count = polishStoneTemplate->mStackAmount-needPolishStoneCount;
					stonesEle.uid = stoneUidIter->mValue;
					polishStoneLists.insertTail(stonesEle);

					break;
				}
			}
		}
		
	}
	else
	{
		//该类的洗练石都堆叠满了

		for(List<UInt64>::Iter * stoneUidIter = listPolishStoneUids.begin(); stoneUidIter != NULL; 
			stoneUidIter = listPolishStoneUids.next(stoneUidIter))
		{
			if(needPolishStoneCount == 0)
			{
				break;
			}
			
			if(needPolishStoneCount > polishStoneTemplate->mStackAmount)
			{
				needPolishStoneCount -= polishStoneTemplate->mStackAmount;
				delItem(stoneUidIter->mValue);

				polishStoneEle stonesEle;
				stonesEle.count = 0;
				stonesEle.uid = stoneUidIter->mValue;

				polishStoneLists.insertTail(stonesEle);
			}
			else
			{
				delItem(stoneUidIter->mValue,needPolishStoneCount);
				
				polishStoneEle stonesEle;
				stonesEle.count = polishStoneTemplate->mStackAmount-needPolishStoneCount;
				stonesEle.uid = stoneUidIter->mValue;

				polishStoneLists.insertTail(stonesEle);
				break;
			}
		}
	}

	//消耗金钱
	mPlayer->mPlayerData.mBaseData.m_nCoin -= objectTemplate->mRefreshcoin ;

	mPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

	AttrPolishTemplate *materialPolishTem = ATTRPOLISH_TABLE().get(materialTemplate->mLevelRequire);

	RandomAttribute *materialRdFind = NULL;

	//是否有橙色属性
	bool orangeFlag = false;
	//在材料装备中的橙色属性,随机概率对应的属性组成的map,主要用于目标装备中橙色洗练次数大于等于1，
	//并且材料装备中有橙色属性的情况
	Map<UInt32, RandomAttribute*> orangeRdMap;
	UInt32 orangeRate = 0;

	bool purpleFlag = false;
	//在材料装备中的紫色属性，随机概率对应的属性组成的map，主要用于目标装备中紫色洗练次数大于等于1，
	//并且材料装备中有紫色属性的情况
	Map<UInt32, RandomAttribute*> purpleRdMap;
	UInt32 purpleRate = 0;

	//材料装备中所有颜色的属性，随机概率对应的属性组成的map，主要用于目标装备中紫色和橙色积累次数为0，
	//或者材料装备中没有紫色或橙色属性的概率统计情况
	Map<UInt32, RandomAttribute*> totalRdMap;
	UInt32 totalRate = 0;
	
	Vector<RandomAttribute*> rdAttrVecs; 
	creatQualityMap(materialItemData,materialPolishTem,orangeFlag,orangeRdMap,orangeRate, purpleFlag,purpleRdMap,purpleRate,totalRdMap,totalRate);

	//先判断橙色，如果有橙色洗练积累的次数
	if(objectItemData->m_itemAttribute.m_nOrangeTimes >= 1)
	{
		
		//防止策划更改，将不同颜色逻辑分开
		if(orangeFlag)
		{
			orangeSuccess(orangeRdMap,objectItemData,materialRdFind,orangeRate);
		}
		else
		{
			//如果材料装备没有橙色属性，判断目标装备的紫色洗练累计次数是否为大于等于1
			if(objectItemData->m_itemAttribute.m_nPurpleTimes >= 1)
			{
				//目标装备的紫色洗练积累次数达到大于等于一,判断材料装备中是否有紫色属性
				if(purpleFlag)
				{
					//如果材料装备有紫色属性
					purpleSuccess(purpleRdMap,objectItemData,materialRdFind,purpleRate);
				}
				else
				{
					//如果材料装备中没有紫色属性，那么从所有属性map中随机
					totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);
				}
			}
			else
			{
				//如果目标装备的紫色洗练次数为0，橙色次数>= 1,并且材料装备没有橙色属性，那么就要采取材料装备全部属性的随机逻辑
				totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);

			}
		
		}
	}
	else
	{
		//目标装备的橙色洗练次数为0，判断目标装备紫色积累次数是否大于等于1
		if(objectItemData->m_itemAttribute.m_nPurpleTimes >= 1)
		{
			//目标装备的紫色洗练积累次数达到大于等于一,判断材料装备中是否有紫色属性
			if(purpleFlag)
			{
				//如果材料装备有紫色属性
				purpleSuccess(purpleRdMap,objectItemData,materialRdFind,purpleRate);
			}
			else
			{
				//如果材料装备中没有紫色属性，那么从所有属性map中随机
				totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);
			}
		}
		else
		{
			//目标装备既没有紫色洗练积累次数，也没有橙色洗练积累次数，那么从所有属性中随机选取
			totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);
		}
	}

	int a = 5;
	assert(materialRdFind);
	List<RandomAttribute>::Iter * objRdIter = objectItemData->m_itemAttribute.m_listRandomAttrs.getn(objRdAttrIndex);

	PolishPointsTemplate * findPolishPointsTemp = NULL;
	for(PolishPointsTemplateMap::Iter * polishPointsMapIter = POLISHPOINTS_TABLE().mMap.begin(); 
		polishPointsMapIter != NULL;
		polishPointsMapIter = POLISHPOINTS_TABLE().mMap.next(polishPointsMapIter))
	{
		if(mPlayer->mPlayerData.mBaseData.m_nLevel < polishPointsMapIter->mValue.mLevel)
		{
			break;
		}

		findPolishPointsTemp = &polishPointsMapIter->mValue;
	}

	//如果玩家高于或者等于40级，那么增加洗练积分
	if(findPolishPointsTemp)
	{
		//增加洗练积分，根据材料装备的颜色品质进行增加
		switch(materialTemplate->mQuality)
		{

		case Blue:
			{
				MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialTemplate->mLevelRequire);
				objectItemData->m_itemAttribute.m_nOrangePoints+= materialPointsTem->mBluePoints;
				objectItemData->m_itemAttribute.m_nPurplePoints+= materialPointsTem->mBluePoints;
			}
			break;

		case Purple:
			{
				MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialTemplate->mLevelRequire);
				objectItemData->m_itemAttribute.m_nOrangePoints+= materialPointsTem->mPurplePoints;
				objectItemData->m_itemAttribute.m_nPurplePoints+= materialPointsTem->mPurplePoints;
			}
			break;

		case Orange:
			{
				MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialTemplate->mLevelRequire);
				objectItemData->m_itemAttribute.m_nOrangePoints+= materialPointsTem->mOrangePoints;
				objectItemData->m_itemAttribute.m_nPurplePoints+= materialPointsTem->mOrangePoints;
			}
			break;

		default:
			break;

		}


		//重新统计目标装备的强化积分和橙色与紫色的累计次数

		
		//计算橙色属性的累计次数
		while(objectItemData->m_itemAttribute.m_nOrangePoints>=findPolishPointsTemp->mOrangePoints)
		{
			objectItemData->m_itemAttribute.m_nOrangePoints -= findPolishPointsTemp->mOrangePoints;
			objectItemData->m_itemAttribute.m_nOrangeTimes++;
		}

		//计算紫色属性的累计次数
		while(objectItemData->m_itemAttribute.m_nPurplePoints >= findPolishPointsTemp->mPurplePoints)
		{
			objectItemData->m_itemAttribute.m_nPurplePoints -= findPolishPointsTemp->mPurplePoints;
			objectItemData->m_itemAttribute.m_nPurpleTimes++;
		}

		if(objectItemData->m_itemAttribute.m_nOrangeTimes>3)
		{
			objectItemData->m_itemAttribute.m_nOrangeTimes = 3;
		}

		if(objectItemData->m_itemAttribute.m_nPurpleTimes>3)
		{
			objectItemData->m_itemAttribute.m_nPurpleTimes = 3;
		}

	}
	
	

	//将目标装备的原来属性保存在临时变量里，用于发给客户端,现在改为保存在装备字段里

	objectItemData ->m_itemAttribute.m_nOldAttribute = objRdIter->mValue;
	objectItemData->m_itemAttribute.m_nOldAttributeIndex = objRdAttrIndex;

	//洗练更改属性后，需重新计算新的属性在目标装备中的品质
	
	UInt32 attrQualityNew = getAttrQuality(*materialRdFind,objectTemplate);
	//属性替换，但是这个时候的品质是相对材料装备的，下面会进行更改为相对目标装备的
	objRdIter->mValue = *materialRdFind;
	//将计算好的新品质赋值给目标装备
	objRdIter->mValue.m_nQuality = attrQualityNew;
	
	//存盘标记
	objectItemData->m_nDirtyType = UPDATEDIRTY;
	
	//计算找到的那条随机属性的索引，从0开始
	UInt32 materialAttrIndexFind = 0;
	List<RandomAttribute>& materialRdAttrList =  materialItemData->m_itemAttribute.m_listRandomAttrs;
	for(List<RandomAttribute>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.m_nKey == materialRdFind->m_nKey && materialRdIter->mValue.m_nValue ==  materialRdFind->m_nValue &&
			materialRdIter->mValue.m_nQuality == materialRdFind->m_nQuality)
		{
			break;
		}
		materialAttrIndexFind++;
	}

	delItem(materialEquipUid,1);

	//发送消息给客户端
	EquipPolishResp polishResp;

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["equipuid"] = objEquipUid;
	root["newAttr"]["type"] = objRdIter->mValue.m_nKey;
	root["newAttr"]["value"] = objRdIter->mValue.m_nValue;
	root["newAttr"]["quality"] = objRdIter->mValue.m_nQuality;

	root["oldAttr"]["type"] = objectItemData ->m_itemAttribute.m_nOldAttribute.m_nKey;
	root["oldAttr"]["value"] = objectItemData ->m_itemAttribute.m_nOldAttribute.m_nValue;
	root["oldAttr"]["quality"] = objectItemData ->m_itemAttribute.m_nOldAttribute.m_nQuality;
	root["objAttrIndex"] = objRdAttrIndex;
	root["materialAttrIndex"] = materialAttrIndexFind;

	root["materialEquipid"] = materialEquipUid;
	
	for(List<polishStoneEle>::Iter * stoneEleIter = polishStoneLists.begin(); stoneEleIter!=NULL; stoneEleIter = polishStoneLists.next(stoneEleIter))
	{
		Json::Value stoneEle;
		stoneEle["uid"] = stoneEleIter->mValue.uid;
		stoneEle["count"] = stoneEleIter->mValue.count;

		root["polishStoneList"].append(stoneEle);
	}

	Json::FastWriter writer;
	std::string respJson = writer.write(root);

	polishResp.mRespJsonStr = respJson;
	polishResp.mPacketID = EQUIP_POLISH_RESP;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(polishResp,connId);
}

UInt32 ItemManager::getAttrQuality(const RandomAttribute &randomAttr, EquipTemplate * objectTemplate)
{
	//通过equip表找到装备对应的等级

	UInt32 level = objectTemplate->mLevelRequire;


	if(randomAttr.m_nKey == PA)
	{
		return getPAQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MA)
	{
		return getMAQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == PF)
	{
		return getPFQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MF)
	{
		return getMFQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxHP)
	{
		return getMaxHPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxMP)
	{
		return getMaxMPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxSP)
	{
		return getMaxSPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == Critrate)
	{
		return getCritRateQuality(level,randomAttr);
	}
	else if(randomAttr.m_nKey == BaseCrit)
	{
		return getCirtDamageQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == BaseToughness)
	{
		//韧性
		return getBaseToughnessQuality(level, randomAttr);

	}
	else
	{
		return 0;
	}
}


UInt32 ItemManager::getPAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr )
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mPAMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mPAMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mPAMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mPAMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getMAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr )
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMAMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMAMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMAMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMAMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getPFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mPFMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mPFMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mPFMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mPFMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getMFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMFMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMFMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMFMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMFMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getMaxHPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxHPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxHPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxHPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxHPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getMaxMPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxMPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxMPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxMPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxMPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getMaxSPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxSPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxSPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxSPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxSPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getCritRateQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritRateMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritRateMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritRateMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritRateMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getCirtDamageQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritDamageMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritDamageMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritDamageMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritDamageMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getBaseToughnessQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//从attrRandom表格里查找属性值在那个范围 sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritDefMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritDefMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritDefMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritDefMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}



void  ItemManager::polishResetEquip(UInt64 objEquipUid)
{
	Map<Guid, ItemData*>::Iter * objEquipIter = m_mapItemUuid.find(objEquipUid);
	if(!objEquipUid)
	{
		LOG_INFO("equip not find polishResetEquip!!");

		Json::Value root;
		root["errorId"] = LynxErrno::ItemNotExist;
		root["objRdAttrIndex"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;

		root["oldAttr"]["type"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nKey;
		root["oldAttr"]["value"] =  objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nValue;
		root["oldAttr"]["quality"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nQuality;

		Json::FastWriter writer;

		EquipPolishResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = EQUIP_POLISH_RESET_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}

	if(mPlayer->mPlayerData.mBaseData.m_nCoin < PolishNeedCoin)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;

		root["objRdAttrIndex"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;

		root["oldAttr"]["type"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nKey;
		root["oldAttr"]["value"] =  objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nValue;
		root["oldAttr"]["quality"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nQuality;

		Json::FastWriter writer;

		EquipPolishResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = EQUIP_POLISH_RESET_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}
	

	UInt32 & rdIndex = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;
	RandomAttribute & rdAttribute = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute;

	List<RandomAttribute>::Iter * rdAttrIter = objEquipIter->mValue->m_itemAttribute.m_listRandomAttrs.getn(rdIndex);
	assert(rdAttrIter);
	rdAttrIter->mValue = rdAttribute;
	
	objEquipIter->mValue->m_nDirtyType = UPDATEDIRTY;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT|BASEDATABIT);
	mPlayer->mPlayerData.mBaseData.m_nCoin -= PolishNeedCoin;
	
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["objRdAttrIndex"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;

	root["oldAttr"]["type"] = rdAttrIter->mValue.m_nKey;
	root["oldAttr"]["value"] =  rdAttrIter->mValue.m_nValue;
	root["oldAttr"]["quality"] = rdAttrIter->mValue.m_nQuality;
    root["equipUid"] = objEquipUid;

	Json::FastWriter writer;
	
	EquipPolishResetResp resetResp;
	resetResp.mRespJsonStr = writer.write(root);
	resetResp.mPacketID = EQUIP_POLISH_RESET_RESP;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resetResp,connId);
}

void ItemManager::creatQualityMap(ItemData * materialItemData,AttrPolishTemplate *materialPolishTem,bool& orangeFlag,
								  Map<UInt32, RandomAttribute*> &orangeRdMap,
								 UInt32& orangeRate, bool& purpleFlag,Map<UInt32, RandomAttribute*> &purpleRdMap,
								 UInt32& purpleRate,
								 Map<UInt32, RandomAttribute*> &totalRdMap,UInt32& totalRate)
{
	List<RandomAttribute>& materialRdAttrList =  materialItemData->m_itemAttribute.m_listRandomAttrs;


	for(List<RandomAttribute>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.m_nKey == 0)
		{
			continue;
		}
		
		if(materialRdIter->mValue.m_nQuality == Orange)
		{
			orangeFlag = true;

			//橙色随机值分段增加
			orangeRate += materialPolishTem->mOrange;
			//全类型随机值分段增加
			totalRate += materialPolishTem->mOrange;
			//添加橙色,key 为当前属性随机的最大值上限
			orangeRdMap.insert(orangeRate,&materialRdIter->mValue);
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else if(materialRdIter->mValue.m_nQuality == Purple)
		{
			purpleFlag = true;
			//紫色随机值分段增加
			purpleRate += materialPolishTem->mPurple;
			//全类型的随机值分段增加
			totalRate += materialPolishTem->mPurple;
			//添加紫色
			purpleRdMap.insert(purpleRate,&materialRdIter->mValue);
			//将所有种类的插入map
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else
		{
			//将此类属性放入所有种类的map里
			if(materialRdIter->mValue.m_nQuality == White)
			{
				totalRate += materialPolishTem->mWhite;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(materialRdIter->mValue.m_nQuality == Green)
			{
				totalRate += materialPolishTem->mGreen;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(materialRdIter->mValue.m_nQuality == Blue)
			{
				totalRate += materialPolishTem->mBlue;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
		}

	}

	if(totalRdMap.empty())
	{
		Json::Value root;
		root["errorId"] = LynxErrno::MaterialRdEmpty;
		
		EquipPolishResp polishResp;
		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
	}
}

void ItemManager::orangeSuccess(Map<UInt32, RandomAttribute*> &orangeRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &orangeRate)
{
	//orangrate主要就是处理材料装备中有多条橙色属性的权重，随机出一个值
	UInt32 orangeValue = rand()%orangeRate;
	UInt32 orangeBase = 0;
	for(Map<UInt32, RandomAttribute*>::Iter* orangeRdIter =  orangeRdMap.begin(); orangeRdIter != NULL; 
		orangeRdIter = orangeRdMap.next(orangeRdIter))
	{
		
		//如果随机值在某条橙色属性的临界值里，那么取出这条属性，直到遍历结束，一定会找到一个这样的属性
		if(orangeValue < orangeRdIter->mKey)
		{
			materialRdFind = orangeRdIter->mValue;
			//消耗掉橙色一次洗练积累次数
			objectItemData->m_itemAttribute.m_nOrangeTimes--;
			break;
		}

	}

	assert(materialRdFind);
}

void ItemManager::purpleSuccess(Map<UInt32, RandomAttribute*> &purpleRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &purpleRate)
{
	UInt32 purpleValue = rand()%purpleRate;
	UInt32 purpleBase = 0;
	for(Map<UInt32, RandomAttribute*>::Iter* purpleRdIter =  purpleRdMap.begin(); purpleRdIter != NULL; 
		purpleRdIter = purpleRdMap.next(purpleRdIter))
	{

		//如果随机值在某条紫色属性的临界值里，那么取出这条属性，直到遍历结束，一定会找到一个这样的属性
		if(purpleValue < purpleRdIter->mKey)
		{
			materialRdFind = purpleRdIter->mValue;
			//消耗掉一次紫色洗练积累次数
			objectItemData->m_itemAttribute.m_nPurpleTimes--;
			break;
		}

	}
	//断言一定能找到这个属性
	assert(materialRdFind);
}

void ItemManager::totalAttrRandom(Map<UInt32, RandomAttribute*> &totalRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &totalRate)
{
	UInt32 totalValue = rand()%totalRate;
	UInt32 totalBase = 0;
	for(Map<UInt32, RandomAttribute*>::Iter * totalRdIter  = totalRdMap.begin(); totalRdIter != NULL;
		totalRdIter = totalRdMap.next(totalRdIter))
	{
		
		//如果随机值在某条属性的临界值里，那么取出这条属性，直到遍历结束，一定会找到这个属性
		if(totalValue < totalRdIter->mKey)
		{
			materialRdFind = totalRdIter->mValue;
			break;
		}
	}

	assert(materialRdFind);
}

void ItemManager::gemCombine(UInt64 gem1Uid, UInt64 gem2Uid)
{
	Map<Guid, ItemData*>::Iter * gem1Iter = m_mapItemUuid.find(gem1Uid);
	Map<Guid, ItemData*>::Iter * gem2Iter =m_mapItemUuid.find(gem2Uid);
	
	UInt64 gem1ItemId = gem1Iter->mValue->m_nItemId;
	UInt64 gem2ItemId = gem2Iter->mValue->m_nItemId;
	
	if(gem1ItemId != gem2ItemId)
	{
		//添加错误回复
		GemCombineResp resp;
		resp.mPacketID = GEM_COMBINE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidIdentify;
		root["gems"] = "";
		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		resp.mRespJsonStr = jsonStr;
		
		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);
		return;
	}
	else
	{
		//合成的逻辑处理
		GemCombineTemplate * gem1Template = GEMCOMBINE_TABLE().get(gem1ItemId);
			
		if(gem1Template->mNextGem == 0)
		{
			//添加错误回复
			GemCombineResp resp;
			resp.mPacketID = GEM_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidIdentify;
			root["gems"] = "";
			Json::FastWriter writer;
			std::string jsonStr = writer.write(root);

			resp.mRespJsonStr = jsonStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(resp,connId);
			return;
		}

		if(gem1Uid == gem2Uid)
		{
			//堆叠uid相同
			//判断背包格是否够用
			if(m_nItemOccupies < MAXITEMCOUNT)
			{
				//当前有空白的背包格
				delItem(gem1Uid,2);
			}
			else
			{
				//当前没有空白的背包格
				//首先考虑新生成的宝石是否在背包中存在，
				Map<Guid,List<Guid> >::Iter * itemId2UidsIter = m_mapItemId2Uids.find(gem1Template->mNextGem);

				ItemTemplate * newGemTemplate = ITEM_TABLE().get(gem1Template->mNextGem);

				//此类宝石在背包中存在
				if(itemId2UidsIter)
				{
					//用于标记是否需要占用一个新的背包格的bool
					bool isNeedNewBag = true;
					//如果存在，找到未堆叠满的
					List<Guid> & uidsList = itemId2UidsIter->mValue;
					for(List<Guid>::Iter * uidsIter = uidsList.begin(); uidsIter != NULL; uidsIter = uidsList.next(uidsIter))
					{
						Map<Guid, ItemData*>::Iter * dataIter = m_mapItemUuid.find(uidsIter->mValue);
						assert(dataIter);
						if(dataIter->mValue->m_nCurStackAmount <= newGemTemplate->mStackAmount-1)
						{
							//背包中存在此类物品的堆叠数量未满，而且放入新的物品不会导致占用新的背包格子
							isNeedNewBag = false;
						}
					}

					if(!isNeedNewBag)
					{
						//当前的背包格够用
						delItem(gem1Uid,2);
					}
					else
					{
						//当前背包格不够用
						//判断删除现有宝石是否会造成背包格清空一个
						Map<Guid, ItemData*> ::Iter * oldGemIter = m_mapItemUuid.find(gem1Uid);
						
						//删除当前宝石会导致清空一个背包格
						if(oldGemIter->mValue->m_nCurStackAmount <= 2)
						{
							delItem(gem1Uid,2);
						}
						else
						{
							//删除当前宝石不会导致清空一个背包格，并且会有新的背包格被占用，而且当前被曝已经满了
							//提示玩家背包格子不足
							Json::Value root;
							GemCombineResp resp;
							resp.mPacketID = GEM_COMBINE_RESP;
							root["gems"] = "";
									

							root["errorId"] = LynxErrno::BagIsFull;


							Json::FastWriter writer;
							std::string jsonStr = writer.write(root);

							resp.mRespJsonStr = jsonStr;

							const ConnId& connId = mPlayer->getConnId();

							NetworkSystem::getSingleton().sendMsg(resp,connId);

							return ;
						}

					}
				}
				else
				{
					//此类宝石在背包中不存在

					//当前背包格不够用
					//判断删除现有宝石是否会造成背包格清空一个
					Map<Guid, ItemData*> ::Iter * oldGemIter = m_mapItemUuid.find(gem1Uid);

					//删除当前宝石会导致清空一个背包格
					if(oldGemIter->mValue->m_nCurStackAmount <= 2)
					{
						delItem(gem1Uid,2);
					}
					else
					{
						//删除当前宝石不会导致清空一个背包格，并且会有新的背包格被占用，而且当前被曝已经满了
						//提示玩家背包格子不足

						GemCombineResp resp;
						resp.mPacketID = GEM_COMBINE_RESP;
						Json::Value root;
						root["gems"] = "";


						root["errorId"] = LynxErrno::BagIsFull;


						Json::FastWriter writer;
						std::string jsonStr = writer.write(root);

						resp.mRespJsonStr = jsonStr;

						const ConnId& connId = mPlayer->getConnId();

						NetworkSystem::getSingleton().sendMsg(resp,connId);

						return ;
					}

				}

				
			}
		}
		else
		{
			//堆叠uid不相同, 如果两个背包格合成一个宝石，说明有一个背包格只有一个宝石，这样背包格子肯定是够用的
		
			
				delItem(gem1Uid,1);
				delItem(gem2Uid,1);
			
			
		}

		Json::Value root;
		List<UInt64> gemList = getNewItems(gem1Template->mNextGem,1);
		for(List<UInt64>::Iter* iterList = gemList.begin(); iterList != NULL; iterList = gemList.next(iterList))
		{
			Map<Guid, ItemData*>::Iter* mapIter = m_mapItemUuid.find(iterList->mValue);
			
			Json::Value gemEle;
			gemEle["gemUid"] = iterList->mValue;
			gemEle["stackAmount"] = mapIter->mValue->m_nCurStackAmount;

			root["gems"].append(gemEle);
		}

		root["errorId"] = LynxErrno::None;

		GemCombineResp resp;
		resp.mPacketID = GEM_COMBINE_RESP;
		
		
		
		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		resp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);

	}
	
}

void ItemManager::gemCombineOnce()
{
	//数组，每个颜色的折合成一级宝石的数量
	UInt32 gemLv1Color[6] = {0};
	//用于获取新的宝石uid列表
	List<UInt64> newGemUidLists;
	
	Vector<UInt64> oldGemUidVecs;
	Map<UInt64, UInt32 > newGemId2Count;
	
	//因为一键合成需要把所有的宝石折算成一级的宝石，然后重新计算，判定背包格数量可以把当前背包中宝石所占的背包格全被视为空的
	for(Map<Guid, ItemData*>::Iter * mapItemDataIter = m_mapUidItemType.begin(); mapItemDataIter!= NULL; 
		mapItemDataIter = m_mapUidItemType.next(mapItemDataIter))
	{
		//找到itemid
		UInt64 itemID = mapItemDataIter->mValue->m_nItemId;

		ItemTemplate * itemTemplate = ITEM_TABLE().get(itemID);
		//判断是否为宝石类型
		if(itemTemplate->mTypeId == GEMTYPEID)
		{

			//当前堆叠数量
			UInt32 curStackAmount = mapItemDataIter->mValue->m_nCurStackAmount;
			GemCombineOnceTemplate * gemCombineOnceTemplate = GEMCOMBINEONCE_TABLE().get(itemID);

			//当前堆叠数量*每一个转化为一级宝石的数量得到总共的一级同类型的宝石数量
			gemLv1Color[itemTemplate->mSubTypeId] += ((gemCombineOnceTemplate->mCount)*curStackAmount);
			oldGemUidVecs.push_back(mapItemDataIter->mValue->m_nUid);

		}
	}

	//统计一次性合成新生成的宝石数量和id
	for(UInt32 i = 1; i < 6; i++)
	{
		//宝石子类型
		UInt32 &type = i;
		//宝石子类型对应一级宝石总共数量
		UInt32 totalCount = gemLv1Color[i];
		
		if(totalCount <= 0)
		{
			continue;
		}
		
		for(UInt32 level = 15; level > 0; level--)
		{
			//该类型和等级对应的数量和id结构体
			GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(type,level);

			if(gemIdAndCount.mID == 0)
			{
				continue;
			}

			int newGemCounts = 0;
			while(totalCount >= gemIdAndCount.mCount)
			{
				newGemCounts++;
				totalCount -= gemIdAndCount.mCount;

			}

			if(newGemCounts)
			{
				newGemId2Count.insert(gemIdAndCount.mID, newGemCounts);
			}
			
			
		}

	}

	UInt32 totalNewGemCount = 0;
	for(Map<UInt64, UInt32 >::Iter * newGemId2CountIter = newGemId2Count.begin(); newGemId2CountIter != NULL; 
		newGemId2CountIter = newGemId2Count.next(newGemId2CountIter) )
	{
		
		ItemTemplate * newGemItem = ITEM_TABLE().get(newGemId2CountIter->mKey);
		
		while(newGemId2CountIter->mValue >newGemItem->mStackAmount)
		{
			totalNewGemCount++;
			newGemId2CountIter->mValue -= newGemItem->mStackAmount;
		}

		totalNewGemCount++;
		
	}

	if(totalNewGemCount - oldGemUidVecs.size() + m_nItemOccupies > MAXITEMCOUNT)
	{
		//提示玩家背包已经满了，不能合成

		Json::Value root;

		root["gems"] = "";
		root["errorId"] = LynxErrno::BagIsFull;

		Json::FastWriter writer;
		std::string jsongStr = writer.write(root);
		GemCombineResp resp;
		resp.mPacketID = GEM_COMBINE_RESP;
		resp.mRespJsonStr = jsongStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);

		return ;

	}
	
	for(UInt32 i = 0; i < oldGemUidVecs.size(); i++)
	{
		delItem(oldGemUidVecs[i],0);
	}

	

	for(UInt32 i = 1; i < 6; i++)
	{
		//宝石子类型
		UInt32 &type = i;
		//宝石子类型对应一级宝石总共数量
		UInt32 totalCount = gemLv1Color[i];
		
		if(totalCount <= 0)
		{
			continue;
		}

		for(UInt32 level = 15; level > 0; level--)
		{
			//该类型和等级对应的数量和id结构体
			GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(type,level);
			
			if(gemIdAndCount.mID == 0)
			{
				continue;
			}
				
			int newGemCounts = 0;
			while(totalCount >= gemIdAndCount.mCount)
			{
				newGemCounts++;
				totalCount -= gemIdAndCount.mCount;
				
			}
			
			if(newGemCounts)
			{
				newGemUidLists += getNewItems(gemIdAndCount.mID,newGemCounts);
			}
			
		}
		
	}

	Json::Value root;

	for(List<UInt64>::Iter* newUidIter = newGemUidLists.begin(); newUidIter != NULL; newUidIter = newGemUidLists.next(newUidIter))
	{
		Json::Value gemEle;
		Map<Guid, ItemData*>::Iter * itemDataIter = m_mapUidItemType.find(newUidIter->mValue);
		gemEle["stackAmount"] = itemDataIter->mValue->m_nCurStackAmount;
		gemEle["gemUid"] = newUidIter->mValue;
		gemEle["itemId"] = itemDataIter->mValue->m_nItemId;
		gemEle["gettime"] = itemDataIter->mValue->m_nItemGetTime;
		
		for(List<UInt32>::Iter * paramIter = itemDataIter->mValue->m_listParam.begin(); paramIter != NULL; 
			paramIter = itemDataIter->mValue->m_listParam.next(paramIter))
		{
			gemEle["param"].append(paramIter->mValue);
		}

		
		root["gems"].append(gemEle);
	}

	
	for(UInt32 i = 0; i < oldGemUidVecs.size(); i++)
		
	{
		Json::Value gemEle;
		
		gemEle["stackAmount"] = 0;
		gemEle["gemUid"] = oldGemUidVecs[i];
		gemEle["itemId"] = 0;
		gemEle["gettime"] = 0;
        gemEle["param"].append(0);
		root["gems"].append(gemEle);
	}
	
	root["errorId"] = LynxErrno::None;
	Json::FastWriter writer;
	std::string jsongStr = writer.write(root);
	GemCombineResp resp;
	resp.mPacketID = GEM_COMBINE_RESP;
	resp.mRespJsonStr = jsongStr;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resp,connId);


}

//宝石镶嵌在装备上
void ItemManager::gemSetEquip(UInt64 equipUid, UInt32 equipGemSlot,UInt64 gemUid)
{
	Map<UInt64, ItemData*>::Iter * equipDataIter = m_mapUidEquipType.find(equipUid);
	Map<UInt64, ItemData*>::Iter * gemDataIter = m_mapUidItemType.find(gemUid);
	
	if(equipDataIter == NULL || gemDataIter == NULL)
	{
		GemSetResp gemSetResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidIdentify;

		root["gemUid"] = 0;
		root["gemCount"] = 0;
		root["equipUid"] = 0;
		root["equipGemList"].append(0);

		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		gemSetResp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);

		return;
	}
	else
	{
		ItemData* equipData = equipDataIter->mValue;
		ItemData* gemData = gemDataIter->mValue;

		List<Guid>::Iter * gemIdIter = equipData->m_itemAttribute.m_listGems.getn(equipGemSlot);
		
		if(gemIdIter->mValue)
		{
			
			//判断装备上的槽位已经镶嵌了宝石,先卸下宝石，判断能否卸载成功
			if(unload(equipData,equipGemSlot) == 0)
			{
				//卸载失败，提示用户更换宝石失败
				GemSetResp gemSetResp;

				Json::Value root;
				root["errorId"] = LynxErrno::BagIsFull;

				root["gemUid"] = 0;
				root["gemCount"] = 0;
				root["equipUid"] = 0;
				root["equipGemList"].append(0);

				Json::FastWriter writer;
				std::string jsonStr = writer.write(root);

				gemSetResp.mRespJsonStr = jsonStr;

				const ConnId& connId = mPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);

				return;
			}
			else
			{
				//将该槽位的宝石换成新的,装备上镶嵌的是宝石的itemid
				gemIdIter->mValue = gemData->m_nItemId;
				equipData->m_nDirtyType = UPDATEDIRTY;

				//删除一个宝石
				delItem(gemUid,1);
			
			}
			
		}
		else
		{
			
			//装备上的槽位没有宝石
			//装备槽位上设置为宝石的itemId
			gemIdIter->mValue = gemData->m_nItemId;
			equipData->m_nDirtyType = UPDATEDIRTY;


			//删除一个宝石
			delItem(gemUid,1);
		}

		GemSetResp gemSetResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidIdentify;
		
		root["gemUid"] = gemUid;
		Map<UInt64, ItemData*>::Iter *gemSetIter = m_mapUidItemType.find(gemUid);
		
		if(gemSetIter)
		{
			root["gemCount"] = gemSetIter->mValue->m_nCurStackAmount;
		}
		else
		{
			root["gemCount"] = 0;
		}
		
		root["equipUid"] = equipUid;

		for(List<Guid>::Iter * gemListIter = equipData->m_itemAttribute.m_listGems.begin();gemListIter != NULL; 
			gemListIter = equipData->m_itemAttribute.m_listGems.next(gemListIter))
		{
			root["equipGemList"].append(gemListIter->mValue);
		}
		

		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		gemSetResp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);
	}
}


//宝石卸下, 返回卸下后新的宝石uid，如果为0，则表示卸下失败
UInt64 ItemManager::unload(ItemData * equipData, UInt32 gemIndex)
{
	List<Guid>::Iter * gemIter = equipData->m_itemAttribute.m_listGems.getn(gemIndex);
	List<UInt64> gemUids = getNewItemsSame(gemIter->mValue,1);

	if(!gemUids.empty())
	{
		//装备对应的宝石槽设置为空
		gemIter->mValue = 0;
		return gemUids.first()->mValue;
	}
	else
	{
		return 0;
	}
}


void ItemManager::equipGemCombine(UInt64 equipUid, UInt32 gemSlot)
{
	Map<Guid, ItemData*>::Iter * equipIter = m_mapItemUuid.find(equipUid);
	List<Guid>::Iter * gemIter = equipIter->mValue->m_itemAttribute.m_listGems.getn(gemSlot);
	GemCombineTemplate * gemCombineTemplate = GEMCOMBINE_TABLE().get(gemIter->mValue);
	
	if(gemCombineTemplate->mNextGem)
	{
		Map<Guid,List<Guid> >::Iter * uidListIter = m_mapItemId2Uids.find(gemIter->mValue);
		
		//是否找到背包中同类的物品
		if(uidListIter)
		{
			
			//如果找到
			List<UInt64> &uidList = uidListIter->mValue;

			//用于查找未堆叠满的背包格
			Map<Guid, ItemData*>::Iter * gemDataFind = NULL;
			for(List<UInt64>::Iter * uidIter = uidList.begin(); uidIter != NULL; uidIter = uidList.next(uidIter))
			{
				ItemTemplate * gemTemplate = ITEM_TABLE().get(gemIter->mValue);

				Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(uidIter->mValue);
				gemDataFind = gemDataIter;
				if(gemDataIter->mValue->m_nCurStackAmount < gemTemplate->mStackAmount)
				{
					break;
				}
			}

			//将当前的堆叠数量-1，并且将装备的宝石槽换为高级宝石
			
			delItem(gemDataFind->mValue->m_nUid,1);	
			

			gemIter->mValue = gemCombineTemplate->mNextGem;
			equipIter->mValue->m_nDirtyType = UPDATEDIRTY;	
		
			
			EquipGemCombineResp combineResp;
			combineResp.mPacketID = EQUIP_GEM_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["gemUid"] = gemDataFind->mValue->m_nUid;
			root["gemCount"] = gemDataFind->mValue->m_nCurStackAmount;
			root["gemItemID"] = gemDataFind->mValue->m_nItemId;
            root["equipUid"] = equipIter->mValue->m_nUid;

			for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin();gemListIter != NULL; 
				gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
			{
				root["equipGemList"].append(gemListIter->mValue);
			}

			Json::FastWriter writer;
			std::string jsonStr = writer.write(root); 

			combineResp.mRespJsonStr = jsonStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);

		}
		else
		{
			//提示玩家背包中没有此类宝石，无法合成

			EquipGemCombineResp combineResp;
			combineResp.mPacketID = EQUIP_GEM_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::ItemNotExist;
			root["gemUid"] = 0;
			Json::FastWriter writer;
			std::string jsonStr = writer.write(root); 
			
			combineResp.mRespJsonStr = jsonStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);

			return;

		}
		
		
	}
	else
	{
		//宝石等级达到上限，提示玩家

		EquipGemCombineResp combineResp;
		combineResp.mPacketID = EQUIP_GEM_COMBINE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::GemLevelLimit;
		root["gemUid"] = 0;
		Json::FastWriter writer;
		std::string jsonStr = writer.write(root); 

		combineResp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(combineResp,connId);

		return;
	}
	
}

List<UInt64> ItemManager::combineGemsList(UInt64 gemItemID,UInt32 count)
{
	List<UInt64> newGemList;
	UInt32 newGemCount = count/2;

	GemCombineTemplate * gemCombineTemplate = GEMCOMBINE_TABLE().get(gemItemID);
	
	if(gemCombineTemplate->mNextGem)
	{
		Map<Guid,List<Guid> >::Iter*  uidListIter = m_mapItemId2Uids.find(gemItemID);

		//是否找到背包中同类的物品
		if(uidListIter)
		{
			//如果找到
			List<UInt64> &uidList = uidListIter->mValue;
			ItemTemplate * gemTemplate = ITEM_TABLE().get(gemItemID);

			//先判断剩余的空格子是否至少有一个
			if(m_nItemOccupies < MAXITEMCOUNT)
			{
				//有空出的格子，那么这种情况下背包一定不会溢出的

				//用于存储的背包格uid，堆叠数量由少到多，主要是用这个数据结构进行删除和更新，因为删除操作会造成原来结构迭代器失效
				List<UInt64> gemUidList;
				constructGemList(uidList, gemTemplate, gemUidList);
				combineGems( gemUidList, newGemList, gemCombineTemplate,newGemCount,count);
				//合成成功，发送给客户端
				
				sendCombineRes(newGemList);

				return newGemList;
			}
			else
			{
				//用于存储的背包格uid，堆叠数量由少到多，主要是用这个数据结构进行删除和更新，因为删除操作会造成原来结构迭代器失效
				List<UInt64> gemUidList;
				constructGemList(uidList, gemTemplate, gemUidList);
				UInt64 firstGemUid  = gemUidList.first()->mValue;
				Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(firstGemUid);
				
				//当消耗的宝石数量不足以腾出原有的一个格子，考虑新生成的是否会占有一个额外的单元格
				if(gemDataIter ->mValue->m_nCurStackAmount > count)
				{
					UInt32 newOccupies = calculateCount(gemCombineTemplate->mNextGem,newGemCount);
					if(newOccupies >= 1)
					{
						//新生成的物品需要额外占用一个单元格，所以会导致背包溢出，提示玩家背包不足无法合成
						sendCombineRes(newGemList,false,LynxErrno::BagIsFull);
						return newGemList;
					}
					else
					{
						//合成
						combineGems( gemUidList, newGemList, gemCombineTemplate,newGemCount,count);

						sendCombineRes(newGemList);
						return newGemList;
					}
				}
				else
				{
					combineGems( gemUidList, newGemList, gemCombineTemplate,newGemCount,count);
					sendCombineRes(newGemList);
					return newGemList;
				}
			}
		}
		else
		{
			//提示玩家背包中没有此宝石，无法合成
			sendCombineRes(newGemList,false,LynxErrno::ItemNotExist);
			return newGemList;
		}

		
	
	}
	else
	{
		sendCombineRes(newGemList,false,LynxErrno::GemLevelLimit);
		
		return newGemList;
	}
}


void ItemManager::combineGems(List<UInt64> &gemUidList, List<UInt64> &newGemList,GemCombineTemplate * gemCombineTemplate,const UInt32 &newGemCount,UInt32 & count)
{

	for(List<UInt64>::Iter * gemUidIter = gemUidList.begin(); gemUidIter != NULL; 
		gemUidIter = gemUidList.next(gemUidIter))
	{
		Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(gemUidIter->mValue);

		//要消耗的宝石比单元格里的多，所以删除单元格里的宝石
		if(gemDataIter->mValue->m_nCurStackAmount <= count)
		{
			count -= gemDataIter->mValue->m_nCurStackAmount;
			delOneItem(gemDataIter->mValue);
			newGemList.insertTail(gemDataIter->mValue->m_nUid);
		}
		else
		{
			//单元格里的比要删除的宝石数量多，所以更新单元格里的宝石
			updateOneItem(gemDataIter->mValue,gemDataIter->mValue->m_nCurStackAmount - count);
			count = 0;
			newGemList.insertTail(gemDataIter->mValue->m_nUid);
			break;
		}
	}

	newGemList += getNewItems(gemCombineTemplate->mNextGem,newGemCount);
}


void ItemManager::constructGemList(List<UInt64> &uidList,ItemTemplate * gemTemplate, List<UInt64> &gemUidList)
{
	for(List<UInt64>::Iter * uidIter = uidList.begin(); uidIter != NULL; uidIter = uidList.next(uidIter))
	{
		Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(uidIter->mValue);


		if(gemDataIter->mValue->m_nCurStackAmount < gemTemplate->mStackAmount)
		{
			gemUidList.insertHead(uidIter->mValue);
		}
		else
		{
			gemUidList.insertTail(uidIter->mValue);
		}
	}
}

void ItemManager::sendCombineRes(List<UInt64> &newGemList, bool success, errorId  errorId)
{
	Json::Value root;
	if(success)
	{
		for(List<UInt64>::Iter* newUidIter = newGemList.begin(); newUidIter != NULL; newUidIter = newGemList.next(newUidIter))
		{
			Json::Value gemEle;
			Map<Guid, ItemData*>::Iter * itemDataIter = m_mapUidItemType.find(newUidIter->mValue);
			
			if(itemDataIter)
			{
				gemEle["stackAmount"] = itemDataIter->mValue->m_nCurStackAmount;
				gemEle["gemUid"] = newUidIter->mValue;
				gemEle["itemId"] = itemDataIter->mValue->m_nItemId;
				gemEle["gettime"] = itemDataIter->mValue->m_nItemGetTime;
				

				for(List<UInt32>::Iter * paramIt = itemDataIter->mValue->m_listParam.begin(); paramIt != NULL; 
					paramIt = itemDataIter->mValue->m_listParam.next(paramIt))
				{
					gemEle["param"].append(paramIt->mValue);
				}

				root["gems"].append(gemEle);
			}
			else
			{
				gemEle["stackAmount"] = 0;
				gemEle["gemUid"] = newUidIter->mValue;
				gemEle["itemId"] = 0;
				gemEle["gettime"] = 0;
				gemEle["param"].append(0);

				root["gems"].append(gemEle);
			}
		
		}

		root["errorId"] = LynxErrno::None;
	}
	else
	{
		root["errorId"] = errorId;
		root["gems"] = "";
	}
	
	Json::FastWriter writer;
	std::string jsongStr = writer.write(root);
	GemCombineResp resp;
	resp.mPacketID = GEM_COMBINE_RESP;
	resp.mRespJsonStr = jsongStr;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resp,connId);
}

void ItemManager::equipGemUnload(UInt64 equipUid, UInt32 gemSlot)
{
	Map<Guid, ItemData*>::Iter * equipIter = m_mapItemUuid.find(equipUid);
	
	if(equipIter)
	{
		UInt64 newGemUid = unload(equipIter->mValue,gemSlot);
		if(newGemUid)
		{
			Map<Guid, ItemData*>::Iter * newGemIter = m_mapItemUuid.find(newGemUid);
			assert(newGemIter);
			Json::Value root;

			root["errorId"] = LynxErrno::None;
			root["equipUid"] = equipUid;

			for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin(); gemListIter != NULL; 
				gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
			{
				root["equipGemList"].append(gemListIter->mValue);
			}

			root["newGem"]["id"] = newGemUid;
			root["newGem"]["itemTypeId"] = newGemIter->mValue->m_nItemId;
			root["newGem"]["gettime"] = newGemIter->mValue->m_nItemGetTime;
			root["newGem"]["count"] = newGemIter->mValue->m_nCurStackAmount;

			for(List<UInt32>::Iter * paramIter = newGemIter->mValue->m_listParam.begin(); paramIter!=NULL;
				paramIter = newGemIter->mValue->m_listParam.next(paramIter))
			{
				root["newGem"]["param"].append(paramIter->mValue);
			}

			Json::FastWriter writer;
			std::string jsongStr = writer.write(root);

			GemUnloadResp resp;
			resp.mPacketID = GEM_UNLOAD_RESP;
			resp.mRespJsonStr = jsongStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(resp,connId);

		}
		else
		{
			Json::Value root;
			//提示拆卸失败
			root["errorId"] = LynxErrno::BagIsFull;
			root["equipUid"] = equipUid;

			for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin(); gemListIter != NULL; 
				gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
			{
				root["equipGemList"].append(gemListIter->mValue);
			}

			root["newGem"]["id"] = 0;
			root["newGem"]["itemTypeId"] = 0;
			root["newGem"]["gettime"] = 0;
			root["newGem"]["count"] = 0;

		    root["newGem"]["param"].append(0);
			

			Json::FastWriter writer;
			std::string jsongStr = writer.write(root);

			GemUnloadResp resp;
			resp.mPacketID = GEM_UNLOAD_RESP;
			resp.mRespJsonStr = jsongStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(resp,connId);

		}
		
	}
	else
	{
		//提示玩家装备的id不正确

		Json::Value root;
		//提示拆卸失败
		root["errorId"] = LynxErrno::InvalidIdentify;
		root["equipUid"] = equipUid;

		for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin(); gemListIter != NULL; 
			gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
		{
			root["equipGemList"].append(gemListIter->mValue);
		}

		root["newGem"]["id"] = 0;
		root["newGem"]["itemTypeId"] = 0;
		root["newGem"]["gettime"] = 0;
		root["newGem"]["count"] = 0;

		root["newGem"]["param"].append(0);


		Json::FastWriter writer;
		std::string jsongStr = writer.write(root);

		GemUnloadResp resp;
		resp.mPacketID = GEM_UNLOAD_RESP;
		resp.mRespJsonStr = jsongStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);
	}
}