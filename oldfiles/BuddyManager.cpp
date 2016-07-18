#include "BuddyManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

BuddyManager::BuddyManager() : mPlayer(NULL), mPlayerBuddyData(NULL)
{

}


BuddyManager::~BuddyManager()
{

}

bool 
BuddyManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
	//playerdata 的mBuddyData注释了 sec
    //mPlayerBuddyData = &mPlayer->mPlayerData.mBuddyData;

    // 从PlayerInfo中加载好友数据
    //LogicSystem& logicSystem = LogicSystem::getSingleton();
    //const PlayerInfo* playerInfo = logicSystem.getPlayerInfo(player->getPlayerGuid());
    //if (!playerInfo)
    //{
    //    LOG_WARN("Failed to get player info by guid %llu", player->getPlayerGuid());
    //    return false;
    //}

    //// 加载好友列表数据 sec playerinfo里没有好友的list，以后根据需求增加
    //for (UInt32 i = 0; i < BUDDY_MAX_COUNT; i++)
    //{
    //    Guid buddyGuid = playerInfo->mBuddyList[i];
    //    
    //    if (buddyGuid)
    //    {
    //        const PlayerInfo* playerInfo = logicSystem.getPlayerInfo(buddyGuid);
    //        if (playerInfo)
    //        {
    //            BuddyData buddyData(*playerInfo);
    //            List<BuddyData>::Iter* buddyDataIter = mPlayerBuddyData->mBuddyDataList.insertTail(buddyData);
    //            mBuddyDataMap.insert(buddyGuid, &buddyDataIter->mValue);           
    //        }
    //    }
    //}

    //// 获得Invited数据 由于buddyData的构造函数被注释了 以后添加对应的构造函数sec
    //list<guid>* invitebuddylist = logicsystem::getsingleton().getinvitedbuddylist(mplayer->getplayerguid());
    //if (invitebuddylist)
    //{
    //    for (list<guid>::iter* iter = invitebuddylist->begin();
    //        iter != null; iter = invitebuddylist->next(iter))
    //    {
    //        const playerinfo* buddyinfo = logicsystem.getplayerinfo(iter->mvalue);
    //        if (buddyinfo)
    //        {
    //            buddydata buddydata(*buddyinfo);
    //            list<buddydata>::iter* jter = mplayerbuddydata->minvitebuddydatalist.inserttail(buddydata);
    //            minvitebuddydatamap.insert(jter->mvalue.mbuddyguid, &jter->mvalue);
    //        }
    //    }
    //}
    // 
    return true;
}

void 
BuddyManager::release()
{
    mBuddyDataMap.clear();
    mInviteBuddyDataMap.clear();
}

ErrorId 
BuddyManager::inviteBuddy(const Guid& buddyGuid)
{
   /* const PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(buddyGuid);
    if (!playerInfo)
    {
        return LynxErrno::PlayerInfoNotExist;
    }

    if (!LogicSystem::getSingleton().insertInvitedBuddy(buddyGuid, mPlayer->getPlayerGuid()))
    {
        return LynxErrno::BuddyHasExist;
    }

    LogicSystem::getSingleton().insertInviteBuddy(mPlayer->getPlayerGuid(), buddyGuid);*/

    return LynxErrno::None;
}

ErrorId 
BuddyManager::addBuddy(const Guid& buddyGuid)
{
 //   // 先判断是否已经是我的好友了
 //   Map<Guid, BuddyData*>::Iter* iter = mBuddyDataMap.find(buddyGuid);
 //   if (iter)
 //   {
 //       return LynxErrno::BuddyHasExist;
 //   }

 //   // 判断对方好友列表是否满
 //   PlayerInfo* buddyPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(buddyGuid);
 //   if (!buddyPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }
 //   
 //   Guid* buddyGuidPointer = NULL;
	////以后在playerinfo里添加好友list再打开注释， sec
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (buddyPlayerInfo->mBuddyList[i] == 0)
 //       {
 //           buddyGuidPointer = &buddyPlayerInfo->mBuddyList[i];
 //           break;
 //       }
 //   }

 //   if (buddyGuidPointer == NULL)
 //   {
 //       return LynxErrno::BuddyBuddyListHasFull;
 //   }

 //   // 判断自己好友列表是否已经满了
 //   PlayerInfo* myPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(mPlayer->getPlayerGuid());
 //   if (!myPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }

	//
 //   Guid* playerGuidPointer = NULL;
	////以后在playerinfo里添加好友list再打开注释， sec
	//
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (myPlayerInfo->mBuddyList[i] == 0)
 //       {
 //           playerGuidPointer = &myPlayerInfo->mBuddyList[i];
 //           break;
 //       }
 //   }

 //   if (playerGuidPointer == NULL)
 //   {
 //       return LynxErrno::PlayerBuddyListHasFull;
 //   }

 //   *buddyGuidPointer = mPlayer->getPlayerGuid();
 //   *playerGuidPointer = buddyGuid;

 //   // 删除好友双方之前的邀请关系
 //   LogicSystem::getSingleton().removeInvitedBuddy(mPlayer->getPlayerGuid(), buddyGuid);
 //   LogicSystem::getSingleton().removeInviteBuddy(mPlayer->getPlayerGuid(), buddyGuid);
 //   LogicSystem::getSingleton().removeInvitedBuddy(buddyGuid, mPlayer->getPlayerGuid());
 //   LogicSystem::getSingleton().removeInviteBuddy(buddyGuid, mPlayer->getPlayerGuid());

 //   // 同步DB好友列表
 //   PersistUpdatePlayerInfoNotify dbNotify;
 //   dbNotify.mPlayerInfo = *myPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, myPlayerInfo->mPlayerGuid);

 //   dbNotify.mPlayerInfo = *buddyPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, buddyPlayerInfo->mPlayerGuid);

    return LynxErrno::None;
}

ErrorId 
BuddyManager::delBuddy(const Guid& buddyGuid)
{
    //从好友列表里删除玩家
 //   PlayerInfo* buddyPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(buddyGuid);
 //   if (!buddyPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }
 //  // 以后在playerinfo里添加好友list再打开注释， sec
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (buddyPlayerInfo->mBuddyList[i] == mPlayer->getPlayerGuid())
 //       {
 //           buddyPlayerInfo->mBuddyList[i] = 0;
 //           break;
 //       }
 //   }

 //   // 从自己列表里删除好友
 //   PlayerInfo* myPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(mPlayer->getPlayerGuid());
 //   if (!myPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }

	////以后在playerinfo里添加好友list再打开注释， sec
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (myPlayerInfo->mBuddyList[i] == buddyGuid)
 //       {
 //           myPlayerInfo->mBuddyList[i] = 0;
 //           break;
 //       }
 //   }

 //   // 同步DB好友列表
 //   PersistUpdatePlayerInfoNotify dbNotify;
 //   dbNotify.mPlayerInfo = *myPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, myPlayerInfo->mPlayerGuid);

 //   dbNotify.mPlayerInfo = *buddyPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, buddyPlayerInfo->mPlayerGuid);

    return LynxErrno::None;
}

ErrorId 
BuddyManager::getRecommendBuddyList(List<BuddyData>& recommendBuddyList)
{
    List<Guid> buddyList;
    LogicSystem::getSingleton().getRecommendBuddyList(mPlayer, buddyList);


	//以后添加好友的构造函数，构造函数的参数是playerinfo，目前先注释sec
   /* for (List<Guid>::Iter* iter = buddyList.begin();
        iter != NULL; iter = buddyList.next(iter))
    {
        const PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(iter->mValue);
        if (playerInfo)
        {
            BuddyData buddyData(*playerInfo);
            recommendBuddyList.insertTail(buddyData);
        }
    }*/

    return LynxErrno::None;
}

ErrorId 
BuddyManager::syncInviteBuddyList(List<BuddyData>& addBuddyList, 
                                   List<BuddyData>& changeBuddyList,
                                   List<Guid>& delBuddyList)
{
	//删除好友请求列表，结构变了需求没定义，目前先注释sec
    //LogicSystem& logicSystem = LogicSystem::getSingleton();
    //List<Guid>* inviteBuddyDataList = LogicSystem::getSingleton().getInvitedBuddyList(mPlayer->getPlayerGuid());
    //if (!inviteBuddyDataList)
    //{
    //    return LynxErrno::None;
    //}

    //// 计算增加的
    //for (List<Guid>::Iter* iter = inviteBuddyDataList->begin();
    //    iter != NULL; )
    //{
    //    const PlayerInfo* buddyInfo = logicSystem.getPlayerInfo(iter->mValue);
    //    if (!buddyInfo)
    //    {
    //        LOG_WARN("Failed to get player info %llu, del invite buddy", iter->mValue);
    //        iter = inviteBuddyDataList->erase(iter);
    //    }
    //    else
    //    {
    //        Map<Guid, BuddyData*>::Iter* jter = mInviteBuddyDataMap.find(iter->mValue);
    //        if (jter == NULL)
    //        {
    //            BuddyData buddyData(*buddyInfo);
    //            List<BuddyData>::Iter* buddyIter = mPlayerBuddyData->mInviteBuddyDataList.insertTail(buddyData);
    //            mInviteBuddyDataMap.insert(buddyIter->mValue.mBuddyGuid, &buddyIter->mValue);
    //            buddyIter->mValue.mDiffFlag = true;
    //            addBuddyList.insertTail(buddyData);
    //        }
    //        else // 存在， 判断战力是否变化
    //        {
    //            if (jter->mValue->mBuddyBattleValue != buddyInfo->mPlayerBattleValue)
    //            {
    //                jter->mValue->syncFromPlayerInfo(*buddyInfo);
    //                changeBuddyList.insertTail(*jter->mValue);
    //            }
    //            jter->mValue->mDiffFlag = true;
    //        }   
    //        iter = inviteBuddyDataList->next(iter);
    //    }
    //}

    //for (List<BuddyData>::Iter* buddyIter = mPlayerBuddyData->mInviteBuddyDataList.begin();
    //    buddyIter != NULL; )
    //{
    //    if (buddyIter->mValue.mDiffFlag == true)
    //    {
    //        buddyIter->mValue.mDiffFlag = false;
    //        buddyIter = mPlayerBuddyData->mInviteBuddyDataList.next(buddyIter);
    //    }
    //    else
    //    {
    //        delBuddyList.insertTail(buddyIter->mValue.mBuddyGuid);
    //        mInviteBuddyDataMap.erase(buddyIter->mValue.mBuddyGuid);
    //        buddyIter = mPlayerBuddyData->mInviteBuddyDataList.erase(buddyIter);
    //    }
    //}
    return LynxErrno::None;
}

ErrorId 
BuddyManager::syncBuddyList(List<BuddyData>& addBuddyList, 
                            List<BuddyData>& changeBuddyList,
                            List<Guid>& delBuddyList)
{
	//同步好友列表，包括好友信息的变更，好友的增加或减少等等 ， 好友结构没定义，所以先注释了sec
//    LogicSystem& logicSystem = LogicSystem::getSingleton();
//    PlayerInfo* playerInfo = logicSystem.getPlayerInfo(mPlayer->getPlayerGuid());
//    if (!playerInfo)
//    {
//        return LynxErrno::PlayerInfoNotExist;
//    }
//
//    for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
//    {
//        if (playerInfo->mBuddyList[i] != 0)
//        {
//            PlayerInfo* buddyPlayerInfo = logicSystem.getPlayerInfo(playerInfo->mBuddyList[i]);
//            if (!buddyPlayerInfo)
//            {
//                LOG_WARN("Failed to get player info %llu, del buddy.", playerInfo->mBuddyList[i]);
//                playerInfo->mBuddyList[i] = 0;
//            }
//            else
//            {
//                Map<Guid, BuddyData*>::Iter* iter = mBuddyDataMap.find(playerInfo->mBuddyList[i]);
//                if (!iter)
//                {
//                    // 新增加的好友
//                    BuddyData buddyData(*buddyPlayerInfo);
//                    buddyData.mDiffFlag = true;
//                    List<BuddyData>::Iter* buddyDataIter = mPlayerBuddyData->mBuddyDataList.insertTail(buddyData);
//                    mBuddyDataMap.insert(playerInfo->mBuddyList[i], &buddyDataIter->mValue);
//
//                    addBuddyList.insertTail(buddyData);
//                }
//                else // 已经存在的好友， 判断战力是否变化
//                {
//                    if (iter->mValue->mBuddyBattleValue != buddyPlayerInfo->mPlayerBattleValue)
//                    {
//                        iter->mValue->syncFromPlayerInfo(*buddyPlayerInfo);
//                        changeBuddyList.insertTail(*iter->mValue);
//                    }
//                    iter->mValue->mDiffFlag = true;
//                }
//            }
//        }
//    }
//
//    for (List<BuddyData>::Iter* iter = mPlayerBuddyData->mBuddyDataList.begin();
//        iter != NULL; )
//    {
//        if (iter->mValue.mDiffFlag == true)
//        {
//            iter->mValue.mDiffFlag = false;
//            iter = mPlayerBuddyData->mBuddyDataList.next(iter);
//        }
//        else
//        {
//            delBuddyList.insertTail(iter->mValue.mBuddyGuid);
//            mBuddyDataMap.erase(iter->mValue.mBuddyGuid);
//            iter = mPlayerBuddyData->mBuddyDataList.erase(iter);
//        }
//    }
     return LynxErrno::None;
}

ErrorId 
BuddyManager::rejectBuddy(const Guid& buddyGuid) // 只删除公共数据， 玩家身上的好友数据， 靠同步diff删除
{
    if (buddyGuid == 0) // 删除全部邀请
    {
        //mInviteBuddyDataMap.clear();
        //mPlayerBuddyData->mInviteBuddyDataList.clear();
        LogicSystem::getSingleton().removeInvitedBuddy(mPlayer->getPlayerGuid(), 0);
    }
    else
    {
        //mInviteBuddyDataMap.erase(buddyGuid);

        //for (List<BuddyData>::Iter* iter = mPlayerBuddyData->mInviteBuddyDataList.begin();
        //    iter != NULL; iter = mPlayerBuddyData->mInviteBuddyDataList.next(iter))
        //{
        //    if (iter->mValue.mBuddyGuid == buddyGuid)
        //    {
        //        mPlayerBuddyData->mInviteBuddyDataList.erase(iter);
        //        break;
        //    }
        //}

        LogicSystem::getSingleton().removeInvitedBuddy(mPlayer->getPlayerGuid(), buddyGuid);
        LogicSystem::getSingleton().removeInviteBuddy(buddyGuid, mPlayer->getPlayerGuid());
    }
    return LynxErrno::None;
}

