#ifndef __LYNX_GAME_MASTER_TOOL_LOGIC_SYSTEM_H__
#define __LYNX_GAME_MASTER_TOOL_LOGIC_SYSTEM_H__

#include "CommonLib.h"
#include "GMClient.h"

namespace Lynx
{
    class LogicSystem : public Singleton<LogicSystem>
    {
    public:
        LogicSystem();
        virtual ~LogicSystem();

        bool initial();
        void release();
        void update();

        void setAccountName(const char* accountName) { mAccountName = accountName; }
        void setAccountPassword(const char* accountPassword) { mAccountPassword = accountPassword; }

        const String& getAccountName() const { return mAccountName; }
        const String& getAccountPassword() const { return mAccountPassword; }

        GMClient* createGMClient(const GMServerInfo& serverInfo);
        void destroyGMClient(const String& serverName);
        GMClient* getGMClient(const String& serverName);

        const Map<String, GMClient*>& getGMClientMap() const { return mGMClientMap; }

        void loginAll();
        void logoutAll();
        void login(const char* serverName);
        void logout(const char* serverName);

        UInt32 getPlayerTotalCount() const { return mPlayerTotalCount; }
        void setCurrentGMClient(const String& serverName);
        GMClient* getCurrentGMClient() { return mCurrentGMClient; }

    private:
        bool onIOTimer(void* args);

    private:
        IOService mIOService;
        IOTimer mIOTimer;
        Map<String, GMClient*> mGMClientMap;
        String mAccountName;
        String mAccountPassword;
        UInt32 mPlayerTotalCount;
        GMClient* mCurrentGMClient;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_LOGIC_SYSTEM_H__

