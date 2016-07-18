#include "RedisManager.h"
#include "GameServer.h"
#ifdef __linux__
#include "hiredis/hiredis.h"
#endif
using namespace Lynx;

RedisManager::RedisManager(){}

RedisManager::~RedisManager()
{}

bool RedisManager::initial()
{
	LOG_INFO("Initial RedisSystem begin:!!!!");	
#ifdef __linux__

	const char*  redisIp = ConfigParser::getSingleton().getRedisServerIp().c_str();
	UInt16  redisPort = ConfigParser::getSingleton().getRedisServerPort();
	m_pConnect = redisConnect(redisIp, redisPort);	                

	if (m_pConnect != NULL && m_pConnect->err) {
		LOG_INFO(" Initial RedisSystem failed!!!!");
		return false;
	}
#endif

	LOG_INFO("Initial RedisSystem success!!!!");
	return true;
}


void RedisManager::set(std::string key, std::string value)
{
#ifdef __linux__

	redisReply* r =(redisReply*)redisCommand(this->m_pConnect, "SET %s %s", key.c_str(), value.c_str());    
#endif

}

std::string RedisManager::get(std::string key)
{
#ifdef __linux__

	this->m_pReply = (redisReply*)redisCommand(this->m_pConnect, "GET %s", key.c_str());
	if (this->m_pReply != NULL)//add by cheng
	{
		if (this->m_pReply->type == REDIS_REPLY_STRING)
		{
			std::string str = this->m_pReply->str;
			freeReplyObject(this->m_pReply);
			return str; 
		}		
		freeReplyObject(this->m_pReply);	
	}
	 
	return "";
#endif

	return "";
}

void RedisManager::release()
{
#ifdef __linux__
	m_pConnect = NULL;
	m_pReply = NULL;
#endif
}


