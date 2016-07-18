#ifndef __LYNX_COMMON_LIB_MSG_LOGINRESP_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_LOGINRESP_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"
namespace Lynx
{
	
	struct MsgLoginRespToJson
	{
		MsgLoginRespToJson():result(0),loginToken(0),errMsg(""),loginMainKey(""),systemTime(0) {};

		UInt32 result;
		std::string errMsg;
		UInt64 loginToken;
		std::string loginMainKey;
		UInt32 systemTime;
		
		std::string convertDataToJson()
		{
			Json::Value root;      //临时对象，供如下代码使用
			root["result"] = Json::Value(result);
			root["errMsg"] = Json::Value(errMsg);
			root["logintoken"] = Json::Value(loginToken);
			root["loginMainKey"] = Json::Value(loginMainKey);
			root["systemTime"] = Json::Value(systemTime);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				result = root["result"].asUInt();
				
			}
		}
	};



} // namespace Lynx

#endif // __LYNX_COMMON_LIB_MSGBODY_TO_JSON_DEFINE_H__