#ifndef __LYNX_GAME_SERVER_LOGGER_THREAD_MSG_H__
#define __LYNX_GAME_SERVER_LOGGER_THREAD_MSG_H__

#include "CommonLib.h"

namespace Lynx
{
	enum LoggerMsg
	{
// 		LOGGER_SERVERDATA_REQ				   	   = 1,		//公共数据
		LOGGER_PLAYER_LOGIN_REQ				   	   = 2,		//玩家登陆log
		LOGGER_PLAYER_LOGOUT_REQ				   = 3,		//玩家离线log

	};

	enum LogTyep
	{
	
		LOG_LOGIN			=1,//登录
		LOG_LOGOUT			=2,//离线
	};


	struct LoggerPlayerTypeLogReq
	{
		UInt32 serverID ;//服务器编号
		UInt64 playerID ;//角色id
		std::string name ;//角色名字	
		UInt32 type ;//LogType 类型
		std::string explain ;//说明
		UInt64 accountID ;//账号id
		std::string tableName;// player_login_20160720

		LoggerPlayerTypeLogReq():accountID(1),playerID(0),name(""),type(0),explain(""),serverID(0),tableName(""){}

		LYNX_MESSAGE_7(LOGGER_PLAYER_LOGIN_REQ, LoggerPlayerTypeLogReq,UInt64,accountID,UInt64, playerID,std::string, name,UInt32, type,std::string, explain,UInt32, serverID,std::string, tableName);
	};

	struct LoggerPlayerLogoutReq
	{
		UInt32 serverID ;//服务器编号
		UInt64 playerID ;//角色id
		std::string name ;//角色名字		
		UInt64 accountID ;//账号id
		std::string tableName;// player_login_20160720

		LoggerPlayerLogoutReq():accountID(1),playerID(0),name(""),serverID(0),tableName(""){}

		LYNX_MESSAGE_5(LOGGER_PLAYER_LOGOUT_REQ, LoggerPlayerLogoutReq,UInt64,accountID,UInt64, playerID,std::string, name,UInt32, serverID,std::string, tableName);
	};

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_LOGGER_THREAD_MSG_H__