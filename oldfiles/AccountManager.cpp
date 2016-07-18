#include "AccountManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

AccountManager::AccountManager() : mPlayer(NULL), mPlayerAccountData(NULL)
{

}


AccountManager::~AccountManager()
{

}

bool 
AccountManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
	//账户字段被注释了，sec，以后根据需求添加
   /* mPlayerAccountData = &player->mPlayerData.mAccountData;*/
   
    return true;
}

void 
AccountManager::release()
{
    mPlayer = NULL;
    mPlayerAccountData = NULL;
}

void 
AccountManager::setLoginClientIp(const String& ip)
{
    if (mPlayerAccountData)
    {
        mPlayerAccountData->mLoginClientIp = ip.c_str();
    }
}

void 
AccountManager::setIdCard(const String& id)
{
    if (mPlayerAccountData)
    {
        mPlayerAccountData->mIdCard = id.c_str();
    }
}

void 
AccountManager::setLoginTime(UInt32 loginTime)
{
    if (mPlayerAccountData)
    {
        mPlayerAccountData->mLoginTime = loginTime;
    }
}

void 
AccountManager::setLogoutTime(UInt32 logoutTime)
{
    if (mPlayerAccountData)
    {
        mPlayerAccountData->mLogoutTime = logoutTime;
    }
}

void 
AccountManager::increaseRmb(UInt32 rmb)
{
    if (mPlayerAccountData)
    {
        mPlayerAccountData->mRmb += rmb;
        LOG_INFO("IncreaseRmb %u [%llu:%llu]", rmb, 
            mPlayerAccountData->mAccountId, mPlayer->getPlayerGuid());
    }
}

void 
AccountManager::reduceRmb(UInt32 rmb)
{
    if (mPlayerAccountData)
    {
        mPlayerAccountData->mRmb -= rmb;
        LOG_INFO("ReduceRmb %u [%llu:%llu]", rmb, 
            mPlayerAccountData->mAccountId, mPlayer->getPlayerGuid());
    }
}
