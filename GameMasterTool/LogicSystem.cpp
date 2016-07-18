#include "LogicSystem.h"
#include "GameMasterTool.h"

using namespace Lynx;

LogicSystem::LogicSystem() : mIOService(IOSERVICE_SELECT_DRIVER),
mAccountName(""), mAccountPassword(""), mPlayerTotalCount(0),
mIOTimer(&mIOService), mCurrentGMClient(NULL)
{

}

LogicSystem::~LogicSystem()
{

}

bool 
LogicSystem::initial()
{
    mIOTimer.set(1000, IOTimerCallback(this, &LogicSystem::onIOTimer), NULL);
    const Map<String, GMServerInfo>& serverInfoMap = ConfigSystem::getSingleton().getServerInfoMap();

    for (Map<String, GMServerInfo>::Iter* iter = serverInfoMap.begin();
        iter != NULL; iter = serverInfoMap.next(iter))
    {
        createGMClient(iter->mValue);
    }

    return true;
}

void 
LogicSystem::release()
{
    for (Map<String, GMClient*>::Iter* iter = mGMClientMap.begin();
        iter != NULL; iter = mGMClientMap.next(iter))
    {
        GMClient* gmClient = iter->mValue;
        if (gmClient)
        {
            gmClient->release();
            XDELETE(gmClient);
        }
    }
    mGMClientMap.clear();
}

void 
LogicSystem::update()
{
    UInt64 accTime = TimeUtil::getTimeMilliSec();
   
    for (Map<String, GMClient*>::Iter* iter = mGMClientMap.begin();
        iter != NULL; iter = mGMClientMap.next(iter))
    {
        GMClient* gmClient = iter->mValue;
        if (gmClient)
        {
            gmClient->update(accTime);
        }
    }

    mIOService.poll();
}

bool 
LogicSystem::onIOTimer(void* args)
{
    return true;
}

GMClient* 
LogicSystem::createGMClient(const GMServerInfo& serverInfo)
{
    Map<String, GMClient*>::Iter* iter = mGMClientMap.find(serverInfo.mServerName);
    if (iter)
    {
        LOG_WARN("Failed to create GMClient for server name %s repeat", serverInfo.mServerName.c_str());
        return NULL;
    }

    GMClient* gmClient = XNEW(GMClient)(&mIOService);
    if (!gmClient->initial(serverInfo))
    {
        LOG_WARN("Failed to initial GMClient for server name %s", serverInfo.mServerName.c_str());
        return NULL;
    }

    mGMClientMap.insert(serverInfo.mServerName, gmClient);
    return gmClient;
}

void 
LogicSystem::destroyGMClient(const String& serverName)
{
    Map<String, GMClient*>::Iter* iter = mGMClientMap.find(serverName);
    if (iter)
    {
        LOG_WARN("Failed to find GMClient %s", serverName.c_str());
        return;
    }

    GMClient* gmClient = iter->mValue;
    mGMClientMap.erase(iter);
    XDELETE(gmClient);
}

GMClient* 
LogicSystem::getGMClient(const String& serverName)
{
    Map<String, GMClient*>::Iter* iter = mGMClientMap.find(serverName);
    if (!iter)
    {
        return NULL;
    }
    return iter->mValue;
}

void 
LogicSystem::loginAll()
{
    for (Map<String, GMClient*>::Iter* iter = mGMClientMap.begin();
        iter != NULL; iter = mGMClientMap.next(iter))
    {
        GMClient* gmClient = iter->mValue;
        if (gmClient)
        {
            if (gmClient->getLoginState() == GMClient::UNLOGIN_STATE)
            {
                gmClient->connectServer();
            }
        }
    }
}

void 
LogicSystem::logoutAll()
{
    for (Map<String, GMClient*>::Iter* iter = mGMClientMap.begin();
        iter != NULL; iter = mGMClientMap.next(iter))
    {
        GMClient* gmClient = iter->mValue;
        if (gmClient)
        {
            gmClient->disconnectServer();
        }
    }
}

void 
LogicSystem::login(const char* serverName)
{
    GMClient* gmClient = getGMClient(serverName);
    if (!gmClient)
    {
        LOG_WARN("Failed to connect server %s for gmClient isn't exist", serverName);
        return;
    }

    if (gmClient->getLoginState() == GMClient::UNLOGIN_STATE)
    {
        gmClient->connectServer();
    }
}

void 
LogicSystem::logout(const char* serverName)
{
    GMClient* gmClient = getGMClient(serverName);
    if (!gmClient)
    {
        LOG_WARN("Failed to connect server %s for gmClient isn't exist", serverName);
        return;
    }

    gmClient->disconnectServer();

}

void 
LogicSystem::setCurrentGMClient(const String& serverName)
{
    GMClient* gmClient = getGMClient(serverName);
    if (gmClient)
    {
        mCurrentGMClient = gmClient;
        ServerSelectEvent serverSelectEvent;
        serverSelectEvent.mSelected = true;
        serverSelectEvent.mServerName = serverName;
        EventSystem::getSingleton().postEvent(serverSelectEvent);
    }
    else
    {
        mCurrentGMClient = NULL;
    }
}
