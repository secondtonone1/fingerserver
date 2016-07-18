#ifndef __LYNX_COMMON_LIB_MSG_BUFFDATA_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_BUFFDATA_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"


namespace Lynx
{

	
	struct MsgBuffDataToJson
	{
		MsgBuffDataToJson()
		{

		}


		std::string convertDataToJson()
		{
			Json::Value root;
			
			std::stringstream mystream;

			for(Map<UInt64, BufferData*>::Iter* iter = m_mapIDToBuffData.begin(); iter != NULL; iter = m_mapIDToBuffData.next(iter))
			{			
				mystream.clear();
				mystream.str("");
				std::string buffIDStr;
				mystream << iter->mValue->m_nBufferID;
				mystream >> buffIDStr;
				
				root[buffIDStr]["id"] = iter->mValue->m_nBufferID;
				root[buffIDStr]["gettime"] = iter->mValue->m_nGetTime;				
			}
		
			Json::FastWriter writer;
			std::string strWrite = writer.write(root);	
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			
		}

		Map<UInt64, BufferData*> m_mapIDToBuffData;
	};



} // namespace Lynx



#endif //__LYNX_COMMON_LIB_MSG_BUFFDATA_TO_JSON_H__