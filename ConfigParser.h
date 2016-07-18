#ifndef __LYNX_GAME_SERVER_CONFIG_PARSER_H__
#define __LYNX_GAME_SERVER_CONFIG_PARSER_H__

#include "OptionParser.h"

//<?xml version="1.0" encoding="utf-8"?>
//<serverconfig id="326" assets="ResourceConfig.xml" log="debug" shutdownpwd="123456" >
//    <main_listen_address ip="127.0.0.1" port="9997" />
//    <gm_listen_address ip="127.0.0.1" port="8888" />
//    <web_listen_address ip="127.0.0.1" port="8080" />
//    <indulge_address ip="127.0.0.1" port="8769" />
//    <passport_address ip="127.0.0.1" port="6666" />
//    <maindb name="BOC_A" ip="192.168.1.38" port="3306" username="lbs" password="lbs"/>
//    <logdb name="BOC_A_LOG" ip="192.168.1.38" port="3306" username="lbs" password="lbs"/>
//    <accountdb name="BOC_ACCOUNT" ip="192.168.1.38" port="3306" username="lbs" password="lbs"/>
//    <redisserver_address ip="127.0.0.1" port="6379"/>

//</serverconfig>

namespace Lynx
{
	class ConfigParser : public Singleton<ConfigParser>
	{
	public:
		ConfigParser() {}
		virtual ~ConfigParser() {}

		bool initial(const char* filePath = "gameserver.conf")
		{
			return parser(filePath);
		}

		void release() 
		{

		}

		UInt32 getServerId() const { return mServerId; }
		const String& getAssetsPath() const { return mAssetsPath; }
		UInt8 getLogLevel() const { return mLogLevel; }
		const String& getMainListenIp() const { return mMainListenIp; }
		UInt16 getMainListenPort() const { return mMainListenPort; }
        const String& getGMListenIp() const { return mGMListenIp; }
        UInt16 getGMListenPort() const { return mGMListenPort; }
		const String& getWebListenIp() const { return mWebListenIp; }
		UInt16 getWebListenPort() const { return mWebListenPort; }
		const String& getIndulgeIp() const { return mIndulgeIp; }
		UInt16 getIndulgePort() const { return mIndulgePort; }
		const String& getPassportIp() const { return mPassportIp; }
		UInt16 getPassportPort() const { return mPassportPort; }
		const String& getMainDbName() const { return mMainDbName; }
		const String& getMainDbIp() const { return mMainDbIp; }
		UInt16 getMainDbPort() const { return mMainDbPort; }
		const String& getMainDbUsername() const { return mMainDbUsername; }
		const String& getMainDbPassword() const { return mMainDbPassword; }
		const String& getLogDbName() const { return mLogDbName; }
		const String& getLogDbIp() const { return mLogDbIp; }
		UInt16 getLogDbPort() const { return mLogDbPort; }
		const String& getLogDbUsername() const { return mLogDbUsername; }
		const String& getLogDbPassword() const { return mLogDbPassword; }
		const String& getAccountDbName() const { return mAccountDbName; }
		const String& getAccountDbIp() const { return mAccountDbIp; }
		UInt16 getAccountDbPort() const { return mAccountDbPort; }
		const String& getAccountDbUsername() const { return mAccountDbUsername; }
		const String& getAccountDbPassword() const { return mAccountDbPassword; }
		const String& getShutdownPassword() const { return mShutdownPassword; }
                const String& getRedisServerIp() const { return mRedisServerIp; }
		UInt16 getRedisServerPort() const { return mRedisPort; }                


		bool parser(const char* filePath)
		{
			FILE* fp = fopen(filePath, "rb");
			if (fp == NULL)
			{
				LOG_WARN("Failed to open %s file", filePath);
				return false;
			}

			TinyXmlDocument doc;
			if (!doc.loadFile(fp))
			{
				LOG_WARN("Failed to load config file %s", filePath);
				return false;
			}

			fclose(fp);

			TinyXmlNode* rootNode = doc.getFirstChild("serverconfig");
			if (!rootNode)
			{
				LOG_WARN("Not found <serverconfig> node");
				return false;
			}

			TinyXmlElement* elem = rootNode->toElement();
			mServerId = lynxAtoi<UInt32>(elem->getAttribute("id"));
			mAssetsPath = elem->getAttribute("assets");
			mShutdownPassword = elem->getAttribute("shutdownpwd");
            String logLevel = elem->getAttribute("log");

            lynxToLowerCase(logLevel);
            if (logLevel == "debug")
            {
                mLogLevel = 0;
            }
            else if (logLevel == "info")
            {
                mLogLevel = 1;
            }
            else if (logLevel == "warn")
            {
                mLogLevel = 2;
            }
            else if (logLevel == "error")
            {
                mLogLevel = 3;
            }
            else if (logLevel == "print")
            {
                mLogLevel = 4;
            }
            else
            {
                LOG_WARN("Invalid log level [%s], [debug|info|warn|error|print]", logLevel.c_str());
                return false;
            }

			// <main_listen_address>
			TinyXmlNode* node = rootNode->getFirstChild("main_listen_address");
			if (!node)
			{
				LOG_WARN("Not found <main_listen_address> node.");
				return false;
			}
			elem = node->toElement();
			mMainListenIp = elem->getAttribute("ip");
			mMainListenPort = lynxAtoi<UInt16>(elem->getAttribute("port"));

            // <gm_listen_address>
			node = rootNode->getFirstChild("gm_listen_address");
			if (!node)
			{
				LOG_WARN("Not found <gm_listen_address> node.");
				return false;
			}
			elem = node->toElement();
			mGMListenIp = elem->getAttribute("ip");
			mGMListenPort = lynxAtoi<UInt16>(elem->getAttribute("port"));

			// <web_listen_address>
			node = rootNode->getFirstChild("web_listen_address");
			if (!node)
			{
				LOG_WARN("Not found <web_listen_address> node.");
				return false;
			}
			elem = node->toElement();
			mWebListenIp = elem->getAttribute("ip");
			mWebListenPort = lynxAtoi<UInt16>(elem->getAttribute("port"));

			// <indulge_address>
			node = rootNode->getFirstChild("indulge_address");
			if (!node)
			{
				LOG_WARN("Not found <indulge_address> node.");
				return false;
			}
			elem = node->toElement();
			mIndulgeIp = elem->getAttribute("ip");
			mIndulgePort = lynxAtoi<UInt16>(elem->getAttribute("port"));

			// <passport_address>
			node = rootNode->getFirstChild("passport_address");
			if (!node)
			{
				LOG_WARN("Not found <passport_address> node.");
				return false;
			}
			elem = node->toElement();
			mPassportIp = elem->getAttribute("ip");
			mPassportPort = lynxAtoi<UInt16>(elem->getAttribute("port"));

			// <maindb>
			node = rootNode->getFirstChild("maindb");
			if (!node)
			{
				LOG_WARN("Not found <maindb> node.");
				return false;
			}
			elem = node->toElement();
			mMainDbName = elem->getAttribute("name");
			mMainDbIp = elem->getAttribute("ip");
			mMainDbPort = lynxAtoi<UInt16>(elem->getAttribute("port"));
			mMainDbUsername = elem->getAttribute("username");
			mMainDbPassword = elem->getAttribute("password");

			// <logdb>
			node = rootNode->getFirstChild("logdb");
			if (!node)
			{
				LOG_WARN("Not found <logdb> node.");
				return false;
			}
			elem = node->toElement();
			mLogDbName = elem->getAttribute("name");
			mLogDbIp = elem->getAttribute("ip");
			mLogDbPort = lynxAtoi<UInt16>(elem->getAttribute("port"));
			mLogDbUsername = elem->getAttribute("username");
			mLogDbPassword = elem->getAttribute("password");

			// <accountdb>
			node = rootNode->getFirstChild("accountdb");
			if (!node)
			{
				LOG_WARN("Not found <accountdb> node.");
				return false;
			}
			elem = node->toElement();
			mAccountDbName = elem->getAttribute("name");
			mAccountDbIp = elem->getAttribute("ip");
			mAccountDbPort = lynxAtoi<UInt16>(elem->getAttribute("port"));
			mAccountDbUsername = elem->getAttribute("username");
			mAccountDbPassword = elem->getAttribute("password");

			//<RedisServer>
			node = rootNode->getFirstChild("redisserver_address");
			if (!node)
			{
				LOG_WARN("Not found <redisserver_address>");
				return false;
			}
			elem = node->toElement();
			mRedisServerIp = elem->getAttribute("ip");
			mRedisPort = lynxAtoi<UInt16>(elem->getAttribute("port"));
			
			return true;
		}

	private:
		UInt32 mServerId;
		String mAssetsPath;
		UInt8  mLogLevel;
		String mMainListenIp;
		UInt16 mMainListenPort;
        String mGMListenIp;
        UInt16 mGMListenPort;
		String mWebListenIp;
		UInt16 mWebListenPort;
		String mIndulgeIp;
		UInt16 mIndulgePort;
		String mPassportIp;
		UInt16 mPassportPort;
		String mMainDbName;
		String mMainDbIp;
		UInt16 mMainDbPort;
		String mMainDbUsername;
		String mMainDbPassword;
		String mLogDbName;
		String mLogDbIp;
		UInt16 mLogDbPort;
		String mLogDbUsername;
		String mLogDbPassword;
		String mAccountDbName;
		String mAccountDbIp;
		UInt16 mAccountDbPort;
		String mAccountDbUsername;
		String mAccountDbPassword;
		String mShutdownPassword;
		String mRedisServerIp;
		UInt16 mRedisPort;
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_CONFIG_PARSER_H__


