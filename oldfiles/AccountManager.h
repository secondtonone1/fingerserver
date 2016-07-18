#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_ACCOUNT_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_ACCOUNT_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class AccountManager : public MainThread
    {
    public:
        AccountManager();
        virtual ~AccountManager();

        bool initial(Player* player);
        void release();

        void setLoginClientIp(const String& ip);
        void setIdCard(const String& id);
        void setLoginTime(UInt32 loginTime);
        void setLogoutTime(UInt32 logoutTime);

        void increaseRmb(UInt32 rmb);
        void reduceRmb(UInt32 rmb);

    private:
        Player* mPlayer;
        AccountData* mPlayerAccountData;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_ACCOUNT_MAMAGER_H__
