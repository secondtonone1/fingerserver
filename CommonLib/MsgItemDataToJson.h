#ifndef __LYNX_COMMON_LIB_MSG_ITEMDATA_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_ITEMDATA_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"

namespace Lynx
{

	struct MsgItemDataToJson
	{
		MsgItemDataToJson()
		{

		}

		std::string convertDataToJson()
		{
			Json::Value root;
			Json::Value equipFeildNode;
			std::stringstream mystream;
			for(Map<UInt64, ItemData*>::Iter * iter = m_mapUidEquipType.begin(); iter != NULL; iter = m_mapUidEquipType.next(iter))
			{
				mystream.clear();
				mystream.str();
				mystream <<iter->mValue->m_nUid;
				std::string equipIdStr;
				mystream >> equipIdStr;
				equipFeildNode[equipIdStr]["id"] = iter->mValue->m_nUid;
				equipFeildNode[equipIdStr]["itemTypeId"] = iter->mValue->m_nItemId;
				equipFeildNode[equipIdStr]["gettime"] = iter->mValue->m_nItemGetTime;
				equipFeildNode[equipIdStr]["count"] = iter->mValue->m_nCurStackAmount;

				for(List<UInt32>::Iter * paramIt = iter->mValue->m_listParam.begin(); paramIt != NULL; 
					paramIt = iter->mValue->m_listParam.next(paramIt))
				{
					equipFeildNode[equipIdStr]["param"].append(paramIt->mValue);
				}

				Json::Value attrNode;
				for(List<IntrinsicAttribute>::Iter* intrinsicIter = iter->mValue->m_itemAttribute.m_listIntrinsicAttrs.begin();
					intrinsicIter != NULL;
					intrinsicIter = iter->mValue->m_itemAttribute.m_listIntrinsicAttrs.next(intrinsicIter))
				{
					if(intrinsicIter->mValue.m_nKey)
					{
						Json::Value intrinNode;
						intrinNode["key"]= intrinsicIter->mValue.m_nKey;
						intrinNode["value"]= intrinsicIter->mValue.m_nValue;
						attrNode["const"].append(intrinNode);
					}				
				}
				
				
				for(List<RandomAttribute>::Iter* randomIter = iter->mValue->m_itemAttribute.m_listRandomAttrs.begin();
					randomIter != NULL;
					randomIter = iter->mValue->m_itemAttribute.m_listRandomAttrs.next(randomIter))
				{
					if(randomIter->mValue.m_nKey)
					{
						Json::Value randomNode;
						randomNode["key"] = randomIter->mValue.m_nKey;
						randomNode["value"] = randomIter->mValue.m_nValue;

						attrNode["random"].append(randomNode);
					}
				}

				equipFeildNode[equipIdStr]["attr"] = attrNode;

				equipFeildNode[equipIdStr]["enhance"]["level"] = iter->mValue->m_itemAttribute.m_nEnhanceLevel;
				equipFeildNode[equipIdStr]["enhance"]["exp"] = iter->mValue->m_itemAttribute.m_nEnhanceExp;

				for(List<Guid>::Iter * iterGem = iter->mValue->m_itemAttribute.m_listGems.begin(); iterGem != NULL; 
					iterGem = iter->mValue->m_itemAttribute.m_listGems.next(iterGem))
				{
					equipFeildNode[equipIdStr]["gem"].append(iterGem->mValue);
				}

				equipFeildNode[equipIdStr]["star"]["level"] = iter->mValue->m_itemAttribute.m_nStarLevel;
				equipFeildNode[equipIdStr]["star"]["exp"] = iter->mValue->m_itemAttribute.m_nStarExp;

			}

			root["equip"] = equipFeildNode;

			Json::Value itemFeildNode;
			for(Map<UInt64, ItemData*>::Iter * iter = m_mapUidItemType.begin(); iter != NULL; iter = m_mapUidItemType.next(iter))
			{
				mystream.clear();
				mystream.str();
				mystream <<iter->mValue->m_nUid;
				std::string itemIdStr;
				mystream >> itemIdStr;
				itemFeildNode[itemIdStr]["id"] = iter->mValue->m_nUid;
				itemFeildNode[itemIdStr]["itemTypeId"] = iter->mValue->m_nItemId;
				itemFeildNode[itemIdStr]["gettime"] = iter->mValue->m_nItemGetTime;
				itemFeildNode[itemIdStr]["count"] = iter->mValue->m_nCurStackAmount;

				for(List<UInt32>::Iter * paramIt = iter->mValue->m_listParam.begin(); paramIt != NULL; 
					paramIt = iter->mValue->m_listParam.next(paramIt))
				{
					itemFeildNode[itemIdStr]["param"].append(paramIt->mValue);
				}
			}

			root["item"] = itemFeildNode;


			Json::Value otherFeildNode;
			for(Map<UInt64, ItemData*>::Iter * iter = m_mapUidOtherType.begin(); iter != NULL; iter = m_mapUidOtherType.next(iter))
			{
				mystream.clear();
				mystream.str();
				mystream <<iter->mValue->m_nUid;
				std::string otherIdStr;
				mystream >> otherIdStr;
				otherFeildNode[otherIdStr]["id"] = iter->mValue->m_nUid;
				otherFeildNode[otherIdStr]["itemTypeId"] = iter->mValue->m_nItemId;
				otherFeildNode[otherIdStr]["gettime"] = iter->mValue->m_nItemGetTime;
				otherFeildNode[otherIdStr]["count"] = iter->mValue->m_nCurStackAmount;

				for(List<UInt32>::Iter * paramIt = iter->mValue->m_listParam.begin(); paramIt != NULL; 
					paramIt = iter->mValue->m_listParam.next(paramIt))
				{
					otherFeildNode[otherIdStr]["param"].append(paramIt->mValue);
				}
			}

			root["other"] = otherFeildNode;

			
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);

			//cout << strWrite;
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			
		}

		std::string convertEquipStateData()
		{
			Map<UInt32, UInt64> mapEquipState;
			Json::Value root;
			
			//建立一个装备位置到uid的map
			for(unsigned int i = 1; i <= 8; i++)
			{
				mapEquipState.insert(i,0);
			}
			
			//根据数据设置map中pos对应的value
			for(Map<UInt64, ItemData*>::Iter * iter = m_mapUidEquipType.begin(); iter != NULL; iter = m_mapUidEquipType.next(iter))
			{
				UInt32 equipState = iter->mValue->m_itemAttribute.m_nEquipState;
				if(equipState != 0 && equipState <= 8 )
				{
					Map<UInt32, UInt64>::Iter * mapIter = mapEquipState.find(equipState);
					assert(mapIter);
					mapIter->mValue = iter->mValue->m_nUid;
				}
			}

			std::stringstream mystream;
			for(Map<UInt32, UInt64>::Iter *iterState = mapEquipState.begin(); iterState != NULL; iterState = mapEquipState.next(iterState))
			{
				mystream.clear();
				mystream.str("");
				mystream << iterState->mKey;
				std::string idStr;
				mystream >> idStr;
				
				Json::Value idNode;
				idNode["id"] = iterState->mValue;
				root[idStr] = idNode;
			}
			
			
			Json::FastWriter writer;
			std::string strWrite = writer.write(root);
			
			return strWrite;
		}
		

		//装备UID 的map 第一标签页下装备对应的map，为了保险，第一标签页下可能还有装备碎片之类的
		Map<UInt64, ItemData*> m_mapUidEquipType;

		//非装备的物品UID map ,第二标签页
		Map<UInt64, ItemData*> m_mapUidItemType;

		//其他类型的UID map 目前没用到 第三标签页
		Map<UInt64, ItemData*> m_mapUidOtherType;
	};



} // namespace Lynx



#endif //__LYNX_COMMON_LIB_MSG_ITEMDATA_TO_JSON_H__
