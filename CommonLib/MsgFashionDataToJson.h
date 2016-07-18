#ifndef __LYNX_COMMON_LIB_MSG_FASHIONDATA_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_FASHIONDATA_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"

namespace Lynx
{

	struct MsgFashionDataToJson
	{
		MsgFashionDataToJson()
		{

		}


		std::string convertDataToJson()
		{
			Json::Value root;

			root["equiped"] = *m_pNowEquipId;
			
			Json::Value buyNode;

			for(Map<UInt64, FashionData*>::Iter * iter = m_mapIDToFashionData.begin(); iter != NULL; 
				iter = m_mapIDToFashionData.next(iter))
			{
				Json::Value fashionNode;
				fashionNode["id"] = iter->mKey;

				buyNode.append(fashionNode);
					
			}
			
			root["haveBuyList"] = buyNode;

			Json::FastWriter writer;
			std::string strWrite = writer.write(root);
			//cout << strWrite;
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			
		}

		Map<UInt64, FashionData*> m_mapIDToFashionData;
		UInt64* m_pNowEquipId;
	};



} // namespace Lynx



#endif //__LYNX_COMMON_LIB_MSG_FASHIONDATA_TO_JSON_H__