#ifndef __LYNX_COMMON_LIB_MSG_BASEINFO_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_BASEINFO_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"
namespace Lynx
{

	struct MsgBaseInfoToJson
	{
		MsgBaseInfoToJson():playerUID(0),modelID(0),playerName(""),mood(""),level(0),
		vipLevel(0),power(0),weaponResID(0),fashionResID(0){};

		UInt64 playerUID;
		UInt32 modelID;
		std::string playerName;
		std::string mood;
		UInt32 level;	
		UInt32 vipLevel;
		UInt32 power;
		UInt32 weaponResID;
		UInt32 fashionResID;
		
		std::string convertDataToJson()
		{
			Json::Value root;      //临时对象，供如下代码使用
			root["uuid"] = Json::Value(playerUID);
			root["uid"] = Json::Value(modelID);
			root["name"] = Json::Value(playerName);
			root["mood"] = Json::Value(mood);
			root["level"] = Json::Value(level);
			root["viplevel"] = Json::Value(vipLevel);
			root["power"] = Json::Value(power);
			root["weaponResId"] = Json::Value(weaponResID);
			root["fashionResID"] = Json::Value(fashionResID);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			
		}
	};



} // namespace Lynx

#endif//__LYNX_COMMON_LIB_MSG_BASEINFO_TO_JSON_H__
