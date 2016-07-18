#ifndef __LYNX_GAME_ROBOT_CONFIG_PARSER_H__
#define __LYNX_GAME_ROBOT_CONFIG_PARSER_H__

//<?xml version="1.0" encoding="utf-8"?>
//<gamerobot account="1-100" password="123456" loginip="127.0.0.1", loginport="9999" log="debug" >
//  <strategy type="chat" duration="1000" />
//  <strategy type="battle" duration="5000" />
//</gamerobot>

namespace Lynx
{
	class ConfigParser : public Singleton<ConfigParser>
	{
	public:
		ConfigParser() {}
		virtual ~ConfigParser() {}

		bool initial(const char* filePath = "gamerobot.conf")
		{
			return parser(filePath);
		}

		void release() 
		{

		}

		

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

			return true;
		}



	private:
        String mAccountP
		UInt32 mServerId;
		String mAssetsPath;
		UInt8  mLogLevel;
		String mMainListenIp;
		UInt16 mMainListenPort;
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
		String mShutdownPassword;
	};
} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_CONFIG_PARSER_H__


