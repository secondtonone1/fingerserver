#ifndef __LYNX_COMMON_LIB_MSG_DETAILINFO_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_DETAILINFO_TO_JSON_H__

#include "../PlatformLib/StringConverter.h"
#include"../jsoncpp/include/json/json.h"
namespace Lynx
{

	struct MsgDetailInfoToJson
	{
		MsgDetailInfoToJson():playerUID(0),modelID(0),playerName(""),mood(""),level(0),exp(0),
			vipLevel(0),vipExp(0),power(0),coin(0),gold(0),strength(0),arenaEnterCount(0),
			amphitheaterEnterCount(0),amphitheaterWinCount(0),amphitheaterLoseCount(0),
			dailyChaiCount(0),dailyMiCount(0),dailyYouCount(0),dailyYanCount(0),
			twelvepalaceEnterCount(0),twelvepalaceData(""),couragetrialDailyPoint(0),
			courageData(""),activepoint(0),dailyMultipleCopyCount(0),
			twelvePalaceUnlockCount(0),twelvePalaceBuyTimes(0),twelvePalaceResetFlag(0),climbTowerTimes(0),towerBuyTimes(0),
			oldName(""){};

		UInt64 playerUID;
		UInt32 modelID;
		std::string playerName;
		std::string mood;
		UInt32 level;
		UInt64 exp;
		UInt32 vipLevel;
		UInt64 vipExp;

		UInt64 coin;
		UInt64 gold;

		UInt64 fame;

		UInt32 power;
		
		UInt32 strength;

		std::string oldName;

		List<UInt32> listSweepCount;

		UInt16 arenaEnterCount;

		UInt16 amphitheaterEnterCount;

		//斗技场胜利次数
		UInt16 amphitheaterWinCount;
		//斗技场失败次数
		UInt16 amphitheaterLoseCount;

		//日常副本柴次数
		UInt16 dailyChaiCount;

		//日常副本米次数
		UInt16 dailyMiCount;

		//日常副本油次数
		UInt16 dailyYouCount;

		//日常副本盐次数
		UInt16 dailyYanCount;

		//十二宫进入次数
		UInt16 twelvepalaceEnterCount;

		//十二宫信息
		std::string twelvepalaceData;

		//勇气试炼日常获得点数

		UInt16 couragetrialDailyPoint;

		std::string courageData;

		UInt32 activepoint;

		//多人副本
		UInt16 dailyMultipleCopyCount;

		UInt32 twelvePalaceUnlockCount;

		UInt32 twelvePalaceBuyTimes;

		UInt32 twelvePalaceResetFlag;

		UInt32 climbTowerTimes;

		UInt32 towerBuyTimes;
		UInt32 fishEatTimes;
		
		std::string guidStr;
		
		std::string convertDataToJson()
		{
			Json::Value root;      //临时对象，供如下代码使用
			root["uuid"] = Json::Value(playerUID);
			root["uid"] = Json::Value(modelID);
			root["name"] = Json::Value(playerName);
			root["mood"] = Json::Value(mood);
			root["level"] = Json::Value(level);
			root["exp"] = Json::Value(exp);
			root["vipLevel"] = Json::Value(vipLevel);
			root["vipExp"] = Json::Value(vipExp);
			root["coin"] = Json::Value(coin);
			root["gold"] = Json::Value(gold);
			root["fame"] = Json::Value(fame);
			root["power"] = Json::Value(power);
			root["strength"] = Json::Value(strength);
			root["oldname"] = Json::Value(oldName);
			root["guidstr"] = Json::Value(guidStr);
				
			for(List<UInt32>::Iter * iter = listSweepCount.begin(); iter != NULL; iter = listSweepCount.next(iter))
			{
				root["sweepItemNum"].append(Json::Value(iter->mValue));
				
			}
			
			root["arenaCountData"] = Json::Value(arenaEnterCount);

			Json::Value amphitheaterNode;
			amphitheaterNode["enter_count"] = Json::Value(amphitheaterEnterCount);
			amphitheaterNode["win_count"] = Json::Value(amphitheaterWinCount);
			amphitheaterNode["lose_count"] = Json::Value(amphitheaterLoseCount);
			root["amphitheaterData"] = amphitheaterNode;

			root["chaimiyouyanCountData"].append(Json::Value(dailyChaiCount));
			root["chaimiyouyanCountData"].append(Json::Value(dailyMiCount));
			root["chaimiyouyanCountData"].append(Json::Value(dailyYouCount));
			root["chaimiyouyanCountData"].append(Json::Value(dailyYanCount));

			root["twelvepalaceCount"] = Json::Value(twelvepalaceEnterCount);

			root["twelvepalaceInfo"] = Json::Value(twelvepalaceData);

			root["couragetrial_daily_point"] = Json::Value(couragetrialDailyPoint);

			root["couragetrialInfo"] = Json::Value(courageData);

			Json::Value totalValue;
			totalValue["totalNum"] = Json::Value(activepoint);

			root["activepoint"] = totalValue;

			root["dailyMultipleCopyCount"] = Json::Value(dailyMultipleCopyCount);

			root["twelvePalaceUnlockCount"] = Json::Value(twelvePalaceUnlockCount);

			root["climbTowerTimes"] = Json::Value(climbTowerTimes);

			root["towerBuyTimes"] = Json::Value(towerBuyTimes);

			root["twelvePalaceBuyTimes"] = Json::Value(twelvePalaceBuyTimes);

			root["fishEatTimes"] = Json::Value(fishEatTimes);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{

		}
	};



} // namespace Lynx

#endif//__LYNX_COMMON_LIB_MSG_DETAILINFO_TO_JSON_H__
