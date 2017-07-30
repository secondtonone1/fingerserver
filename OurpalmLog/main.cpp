#include "test.h"
#include "logsender.h"

using namespace std;
using OurpalmLog::Log;
using OurpalmLog::LogRolePropUpdate;
using OurpalmLog::PalmLogSender;

/*编译的时候请指定 -lpthread -luuid -lcurl 并确保是否已安装这些类库，且需要 -std=c++11，请确保g++版本大于等于4.8 */

int main()
{
	/* serviceCode是需要客户端接入掌趣ServiceCode SDK后方可获取的，下面所示serviceCode仅仅为了测试 */
	string serviceCode = "MTExMTExMTEzMTAyMzAwMDAwMDMxMDIzMDAwMzEwMjMwMDAwMDAyMDF8LXxGNEREOUJGOS0yM0JBLTQ1NUItQjNFQS1GNDc3MkU5RDgzRkJ8LXxpUGhvbmU3LDJ8MTAuMC4yfDc1MHgxMzM0fC18YXBwbGV8LXwyfC18MS4wLjF8LXwtfC18LXwxLjAuMQ%3D%3D";

 	//testSendLog10006(serviceCode);
	//testSendLog10007(serviceCode);
	//testSendLog10008(serviceCode);
	//testSendLog10091(serviceCode);
	//testSendLog10009(serviceCode);
	//testSendLog10010(serviceCode);
	//testSendLog10010_multi(serviceCode);
	//testSendLog10013(serviceCode);
	//testSendLog10014(serviceCode);
	//testSendLog11000(serviceCode);
	//testSendLog11101(serviceCode);
	//testSendLog11102(serviceCode);
	//testSendLog11103(serviceCode);
	//testSendLog12001(serviceCode);

    return 0;
}

/**
* 12001 服务端玩家自定义交互日志
*/
void testSendLog12001(string serviceCode) {
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("interactId", "addfriend"));
		map.insert(pair<string, string>("interactName", "添加好友"));
		map.insert(pair<string, string>("targetType", "1"));
		map.insert(pair<string, string>("targetCode", "0010000067543"));
		map.insert(pair<string, string>("targetInfo", "珍妮.爱荷华"));
		map.insert(pair<string, string>("detail", "SUCC"));

		Log log;
		log.setId("12001");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 11103 服务端玩家自定义行为日志
*/
void testSendLog11103(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("actId", "fight-0001"));
		map.insert(pair<string, string>("actName", "进入战斗-克尔苏加德"));
		map.insert(pair<string, string>("detail", "begin"));

		Log log;
		log.setId("11103");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 11102 服务端玩家副本关卡场景日志
*/
void testSendLog11102(string serviceCode)
{
	try {
		std::map<string, string> map1;
		map1.insert(pair<string, string>("stageId", "deadMine"));
		map1.insert(pair<string, string>("stageName", "副本-死亡矿井"));
		map1.insert(pair<string, string>("detail", "begin"));

		std::map<string, string> map2;
		map2.insert(pair<string, string>("stageId", "deadMine"));
		map2.insert(pair<string, string>("stageName", "副本-死亡矿井"));
		map2.insert(pair<string, string>("detail", "end"));

		Log log;
		log.setId("11102");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("112.23.22.22");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map1);

		Log log2;
		log2.setId("11102");
		log2.putServer("1");
		log2.putRoleId("11111111");
		log2.putRoleName("张宇");
		log2.putRoleLevel("1");
		log2.putRoleVipLevel("0");
		log2.setServiceCode(serviceCode);
		log2.putUserId("0100010000000000000000000000000090019780");
		log2.setClientIp("112.23.22.22");
		log2.setCreateLogTime(time(NULL) * 1000);
		log2.setLogInfo(map2);

		vector<Log> logArr;
		logArr.push_back(log);
		logArr.push_back(log2);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 11101 服务端玩家任务日志
*/
void testSendLog11101(string serviceCode)
{
	try {
		std::map<string, string> map1;
		map1.insert(pair<string, string>("taskId", "guide1"));
		map1.insert(pair<string, string>("taskName", "引导流程1"));
		map1.insert(pair<string, string>("detail", "accept"));

		std::map<string, string> map2;
		map2.insert(pair<string, string>("taskId", "guide1"));
		map2.insert(pair<string, string>("taskName", "引导流程"));
		map2.insert(pair<string, string>("detail", "complete"));

		Log log;
		log.setId("11101");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("112.23.22.22");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map1);

		Log log2;
		log2.setId("11101");
		log2.putServer("1");
		log2.putRoleId("11111111");
		log2.putRoleName("张宇");
		log2.putRoleLevel("1");
		log2.putRoleVipLevel("0");
		log2.setServiceCode(serviceCode);
		log2.putUserId("0100010000000000000000000000000090019780");
		log2.setClientIp("112.23.22.22");
		log2.setCreateLogTime(time(NULL) * 1000);
		log2.setLogInfo(map2);

		vector<Log> logArr;
		logArr.push_back(log);
		logArr.push_back(log2);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 发送完全自定义日志
* 11000 完全自定义日志
*/
void testSendLog11000(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("Time", "2016-05-31 12:01:00"));
		map.insert(pair<string, string>("roleid", "123000"));
		map.insert(pair<string, string>("rolename", "艾亚诺"));
		map.insert(pair<string, string>("occupation", "战士"));
		map.insert(pair<string, string>("lev", "10"));
		map.insert(pair<string, string>("fightvalue", "5"));
		map.insert(pair<string, string>("logintime", "2016-05-31 11:01:00"));
		map.insert(pair<string, string>("Vip", "1"));
		map.insert(pair<string, string>("Coin", "50000"));
		map.insert(pair<string, string>("Diamond", "10"));
		map.insert(pair<string, string>("Gold", "200"));
		map.insert(pair<string, string>("Itemsn", "79001"));
		map.insert(pair<string, string>("Price", "300"));
		map.insert(pair<string, string>("Tax", "45"));

		Log log;
		log.setCustomId("tbl_moneyflow");
		log.setId("11000");
		log.setServiceCode(serviceCode);
		log.setClientIp("112.32.112.22");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 10014 服务端玩家卡牌属性变更日志
*/
void testSendLog10014(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("cardId", "blr_man_001"));
		map.insert(pair<string, string>("cardName", "李星云"));
		map.insert(pair<string, string>("propKey", "level"));
		map.insert(pair<string, string>("newValue", "蓝+1"));
		map.insert(pair<string, string>("oldValue", "绿"));

		Log log;
		log.setId("10014");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 10013 服务端玩家商品属性变更日志
*/
void testSendLog10013(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("itemId", "MGH_Weapon_001"));
		map.insert(pair<string, string>("itemName", "玛格汉铁锤"));
		map.insert(pair<string, string>("propKey", "level"));
		map.insert(pair<string, string>("propValue", "2"));
		map.insert(pair<string, string>("rangeability", "1"));

		Log log;
		log.setId("10013");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/*
* 发送多属性变更日志
* 10010 服务端玩家属性变更日志
*/
void testSendLog10010_multi(string serviceCode)
{
	try {
		std::map<string, string> propMapOld;
		propMapOld.insert(pair<string, string>("level", "3"));
		propMapOld.insert(pair<string, string>("vipLevel", "1"));
		propMapOld.insert(pair<string, string>("hp", "1000"));
		propMapOld.insert(pair<string, string>("mp", "1000"));

		std::map<string, string> propMapNew;
		propMapNew.insert(pair<string, string>("level", "4"));
		propMapNew.insert(pair<string, string>("vipLevel", "1"));
		propMapNew.insert(pair<string, string>("hp", "1500"));
		propMapNew.insert(pair<string, string>("mp", "1500"));

		LogRolePropUpdate log;
		log.setId("10010");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("马克.吐温");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("112.23.22.22");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setPropMapOld(propMapOld);
		log.setPropMapNew(propMapNew);

		vector<LogRolePropUpdate> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 10010 服务端玩家属性变更日志
*/
void testSendLog10010(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("propKey", "level"));
		map.insert(pair<string, string>("propValue", "1"));
		map.insert(pair<string, string>("rangeability", "1"));

		Log log;
		log.setId("10010");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 10009 服务端玩家物品变更日志
*/	
void testSendLog10009(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("updateType", "0"));
		map.insert(pair<string, string>("itemId", "diamond"));
		map.insert(pair<string, string>("itemName", "钻石"));
		map.insert(pair<string, string>("isPrecious", "1"));
		map.insert(pair<string, string>("itemCount", "100"));
		map.insert(pair<string, string>("custom", "购买道具"));

		Log log;
		log.setId("10009");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 10091 服务端玩家货币变更日志
*/
void testSendLog10091(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("updateType", "0"));
		map.insert(pair<string, string>("itemId", "diamond"));
		map.insert(pair<string, string>("itemName", "钻石"));
		map.insert(pair<string, string>("isPrecious", "1"));
		map.insert(pair<string, string>("itemCount", "100"));
		map.insert(pair<string, string>("remains", "200"));
		map.insert(pair<string, string>("custom", "购买道具"));

		Log log;
		log.setId("10091");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 10008 服务端玩家充值日志
*/
void testSendLog10008(string serviceCode)
{
	try {
		std::map<string, string> map;
		map.insert(pair<string, string>("payChannelId", "3100052033100012"));
		map.insert(pair<string, string>("orderNo", "0012014090110143611500"));
		map.insert(pair<string, string>("currencyType", "1"));
		map.insert(pair<string, string>("currencyAmount", "100"));
		map.insert(pair<string, string>("itemId", "diamond"));
		map.insert(pair<string, string>("itemName", "钻石"));
		map.insert(pair<string, string>("itemCount", "100"));

		Log log;
		log.setId("10008");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 10007 服务端玩家登录日志
*/
void testSendLog10007(string serviceCode)
{
	try {
		std::map<string, string> map;

		Log log;
		log.setId("10007");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}

/**
* 发送常规日志
* 10006 服务端玩家注册日志
*/
void testSendLog10006(string serviceCode)
{
	try {
		std::map<string, string> map;

		Log log;
		log.setId("10006");
		log.putServer("1");
		log.putRoleId("11111111");
		log.putRoleName("张宇");
		log.putRoleLevel("1");
		log.putRoleVipLevel("0");
		log.setServiceCode(serviceCode);
		log.putUserId("0100010000000000000000000000000090019780");
		log.setClientIp("111.111.111.111");
		log.setCreateLogTime(time(NULL) * 1000);
		log.setLogInfo(map);

		vector<Log> logArr;
		logArr.push_back(log);

		PalmLogSender::setDebugMode(true);  //仅仅测试的时候打开debug模式，生产环境建议关闭
		PalmLogSender::sendLogList(logArr);
	}
	catch (exception &e) {
		cout << "catch exception:" << e.what() << endl;
	}
}
