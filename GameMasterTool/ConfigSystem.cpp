#include "ConfigSystem.h"

using namespace Lynx;


ConfigSystem::ConfigSystem()
{

}

ConfigSystem::~ConfigSystem()
{

}

bool 
ConfigSystem::initial(const String& filePath)
{
    mConfigFilePath = filePath;

    FILE* fp = fopen(mConfigFilePath.c_str(), "rb");
    if (fp == NULL)
    {
        LOG_WARN("Failed to open %s file", mConfigFilePath.c_str());
        return false;
    }

    TinyXmlDocument doc;
    if (!doc.loadFile(fp))
    {
        LOG_WARN("Failed to load config file %s", mConfigFilePath.c_str());
        return false;
    }
    
    fclose(fp);

    TinyXmlNode* rootNode = doc.getFirstChild("gmtconfig");
    if (!rootNode)
    {
        LOG_WARN("Not found <gmtconfig> node.");
        return false;
    }

    TinyXmlElement* elem = rootNode->toElement();
    mAssetsFilePath = elem->getAttribute("assets");
    mRecordFilePath = elem->getAttribute("record");

    TinyXmlNode* node = rootNode->getFirstChild("serverinfo");
    if (!node)
    {
        LOG_WARN("Not found <serverinfo> node.");
        return false;
    }
    elem = node->toElement();
    do
    {
        GMServerInfo serverInfo;
        serverInfo.mServerName = elem->getAttribute("name");
        serverInfo.mServerIp = elem->getAttribute("ip");
        serverInfo.mServerPort = lynxAtoi<UInt16>(elem->getAttribute("port"));

        mServerInfoMap.insert(serverInfo.mServerName, serverInfo);
    } while ((elem = elem->nextSiblingElement("serverinfo")) != 0);

    return true;
}

void 
ConfigSystem::release()
{

}
