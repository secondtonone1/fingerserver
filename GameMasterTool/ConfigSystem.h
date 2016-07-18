#ifndef __LYNX_GAME_MASTER_TOOL_CONFIG_SYSTEM_H__
#define __LYNX_GAME_MASTER_TOOL_CONFIG_SYSTEM_H__

#include "CommonLib.h"

//<?xml version="1.0" encoding="utf-8"?>
//<gmtconfig assets="resourceconfig.xml" record="gmtrecords">
//  <serverinfo name="aaa" ip="127.0.0.1" port="9999" />
//  <serverinfo name="bbb" ip="192.168.1.112" port="9999" />
//</gmtconfig>

namespace Lynx
{

    class ConfigSystem : public Singleton<ConfigSystem>
    {
    public:
        ConfigSystem();
        virtual ~ConfigSystem();

        bool initial(const String& filePath = "GameMasterTool.conf");
        void release();

        const String& getConfigFilePath() const { return mConfigFilePath; }
        const String& getAssetsFilePath() const { return mAssetsFilePath; }
        const String& getRecordFilePath() const { return mRecordFilePath; }

        const Map<String, GMServerInfo>& getServerInfoMap() const { return mServerInfoMap; }

    private:
        String mConfigFilePath;
        String mAssetsFilePath;
        String mRecordFilePath;
        Map<String, GMServerInfo> mServerInfoMap;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_CONFIG_SYSTEM_H__

