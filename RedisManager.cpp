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
	String   redisPassword = ConfigParser::getSingleton().getRedisPassword().c_str();

	m_pConnect = redisConnect(redisIp, redisPort);               

	LOG_INFO("redisPassword = %s  ",redisPassword.c_str());

	if (m_pConnect != NULL && m_pConnect->err) {
		LOG_INFO(" Initial RedisSystem failed!!!!");
		return false;
	}

	this->m_pReply =(redisReply*) redisCommand(m_pConnect, "AUTH %s", redisPassword.c_str()); 

	if (this->m_pReply->type == REDIS_REPLY_ERROR) 
	{ 
		LOG_INFO(" redis auth failed!!!!");
		return false;
	} 
	freeReplyObject(this->m_pReply );



#endif

	LOG_INFO("Initial RedisSystem success!!!!");
    
	RedisManager::getSingleton().set("forbidlogin","0");

	

	return true;
}


void RedisManager::set(std::string key, std::string value)
{
#ifdef __linux__

	if (value == "")
	{
			LOG_INFO("set redis key = %s,value = %s", key.c_str(),value.c_str());
	}
	else
	{
		redisReply* r =(redisReply*)redisCommand(this->m_pConnect, "SET %s %s", key.c_str(), value.c_str());   
		if(!r)
		{
			LOG_INFO("set redis faliled -------------------------------------------------");
			LOG_INFO("set redis faliled -------------------------------------------------");
			LOG_INFO("set redis faliled -------------------------------------------------");
		}
	}

	
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
	LOG_INFO("Release RedisManager");
}


