#ifndef __LYNX_COMMON_LIB_MSG_SKILLDATA_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_SKILLDATA_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"

namespace Lynx
{

	struct MsgSkillDataToJson
	{
		MsgSkillDataToJson()
		{

		}

		
		std::string convertDataToJson()
		{
			Json::Value root;
			
			for(Map<UInt64, SkillData*>::Iter * iter = m_mapIDToSkillData.begin(); iter != NULL; iter = m_mapIDToSkillData.next(iter))
			{
				Json::Value skillNode;
				skillNode["id"] = Json::Value(iter->mValue->m_nID);
				skillNode["cd"] = Json::Value(iter->mValue->m_nCD);
				skillNode["level"] = Json::Value(iter->mValue->m_nLevel);
				skillNode["equip"] = Json::Value(iter->mValue->m_nEquipPos);
				root.append(skillNode);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			
		}

		Map<UInt64, SkillData*> m_mapIDToSkillData;
	};



} // namespace Lynx



#endif //__LYNX_COMMON_LIB_MSG_SKILLDATA_TO_JSON_H__