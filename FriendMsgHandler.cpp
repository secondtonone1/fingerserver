#include "FriendMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"
#include "RankSystem.h"
using namespace Lynx;

void FriendMsgHandler::onAddFriendReq(const ConnId& connId, CGFriendAddReq& msg)
{
	GCFriendAddNotifyA addFriendNotifyA;

	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 playerUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();
	}

// 		//对方玩家指针
// 		Player * otherplayer = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
// 		if(!otherplayer)
// 		{
// 
// // 
// // 			FriendBlackInfo otherFriendInfo;
// // 
// // 			bool success =  ConsortSystem::getSingleton().getOtherFriendInfo(playerUid, otherFriendInfo);
// // 			if(!success)
// // 			{
// // 				GCFriendDelResp delResp;
// // 				delResp.mPacketID = BOC_FRIENDDEL_RESP;
// // 				Json::Value root;
// // 				root["errorId"] = LynxErrno::PlayerNotExist;
// // 				Json::StyledWriter writer;
// // 				delResp.mRespJsonStr = writer.write(root);
// // 				cout << delResp.mRespJsonStr;
// // 				NetworkSystem::getSingleton().sendMsg(delResp, connId);
// // 				return ;
// // 			}
// // 
// // 			otherFriendInfo
// // 
// // 			//自己先从好友列表中删除对方
// // 
// // 			bool successdel = selfplayer->getFriendBlackManager().delFriend(otherUid);
// // 			if(!successdel)
// // 			{
// // 				GCFriendDelResp delResp;
// // 				delResp.mPacketID = BOC_FRIENDDEL_RESP;
// // 				Json::Value root;
// // 				root["errorId"] = LynxErrno::PlayerNotExist;
// // 				Json::StyledWriter writer;
// // 				delResp.mRespJsonStr = writer.write(root);
// // 				cout << delResp.mRespJsonStr;
// // 				NetworkSystem::getSingleton().sendMsg(delResp, connId);
// // 				return ;
// // 			}
// // 
// // 			//通知客户端删除成功
// // 			GCFriendDelResp delResp;
// // 			delResp.mPacketID = BOC_FRIENDDEL_RESP;
// // 			Json::Value root;
// // 			root["errorId"] = LynxErrno::None;
// // 			root["playeruid"] = otherUid;
// // 			Json::StyledWriter writer;
// // 			delResp.mRespJsonStr = writer.write(root);
// // 			cout << delResp.mRespJsonStr;
// // 			NetworkSystem::getSingleton().sendMsg(delResp, connId);
// // 
// // 			//要删除的对象不在内存中，需要通知数据库删除
// // 			PersistDelFriendOffLineNotify delOffLine;
// // 			delOffLine.m_nMyselfUid = otherUid;
// // 			delOffLine.m_nPlayerUid = selfUid;
// // 			delOffLine.blackStr = otherFriendInfo.blackstr;
// // 			delOffLine.friendStr = otherFriendInfo.friendstr;
// // 			delOffLine.friendCount = otherFriendInfo.friendCount;
// // 
// // 			PersistSystem::getSingleton().postThreadMsg(delOffLine, otherUid);
// // 
// // 		}
// 
// 
// 
// 
// 			Json::Value sendroot;	
// 			sendroot["errorId"] =  LynxErrno::NotFound;
// 			Json::StyledWriter writer;
// 			addFriendNotifyA.mRespJsonStr = writer.write(sendroot);
// 			NetworkSystem::getSingleton().sendMsg(addFriendNotifyA,connId);
// 			return;
// 		}
// 
// 
// 
// 
// 
// 
// 		//对方玩家是否在线
// 		const ConnId & otherconnId = otherplayer->getConnId();
// 		if(!otherconnId)
// 		{
// 			Json::Value sendroot;	
// 			sendroot["errorId"] =  LynxErrno::NotOnLine;
// 			Json::StyledWriter writer;
// 			addFriendNotifyA.mRespJsonStr = writer.write(sendroot);
// 			NetworkSystem::getSingleton().sendMsg(addFriendNotifyA,connId);
// 			return ;
// 		}
// 
// 		//对方玩家是否在黑名单中
// // 		bool inblack = selfplayer->getFriendBlackManager().judgeBlack(playerUid);
// 	
// 		FriendBlackInfo otherFriendInfo;
// 		bool success =  ConsortSystem::getSingleton().getOtherFriendInfo(playerUid, otherFriendInfo);
// 
// 		bool inblack = FriendMsgHandler::getSingleton().judgeBlackBack(playerUid,selfUid,otherFriendInfo);
// 
// 		if(inblack)
// 		{
// 			Json::Value sendroot;			
// 			sendroot["errorId"] =  LynxErrno::InSelfBlack;
// 			Json::StyledWriter writer;
// 			addFriendNotifyA.mRespJsonStr = writer.write(sendroot);
// 			NetworkSystem::getSingleton().sendMsg(addFriendNotifyA,connId);
// 			return;
// 		}
// 
// 		//自己是否在对方玩家黑名单中
// 		bool inblack2 = otherplayer->getFriendBlackManager().judgeBlack(selfUid);
// 		if(inblack2)
// 		{
// 			Json::Value sendroot;			
// 			Json::StyledWriter writer;
// 			sendroot["errorId"] =  LynxErrno::InOtherBlack;
// 			addFriendNotifyA.mRespJsonStr = writer.write(sendroot);
// 			NetworkSystem::getSingleton().sendMsg(addFriendNotifyA,connId);
// 			return;
// 		}
// 
// 		//自己是否在对方玩家申请列表，已经申请过
// 		bool inBeApply = otherplayer->getFriendBlackManager().judgeOtherBeApply(selfUid);
// 		if(inBeApply)	
// 		{
// 			Json::Value sendroot;			
// 			Json::StyledWriter writer;
// 			sendroot["errorId"] =  LynxErrno::InOhterBeApply;
// 			addFriendNotifyA.mRespJsonStr = writer.write(sendroot);
// 			NetworkSystem::getSingleton().sendMsg(addFriendNotifyA,connId);
// 			return;
// 		}
// 
// 		//消息通知对方玩家
// 		GCFriendAddNotify addFriendNotify;
// 		addFriendNotify.mPacketID = BOC_FRIENDADD_NOTIFY;
// 
// 		Json::Value sendroot;
// 		sendroot["fromuid"] = selfUid;
// 		sendroot["modelid"] = selfplayer->getPlayerModelID();
// 		sendroot["name"] = selfplayer->getPlayerName();
// 		sendroot["power"] = selfplayer->getPlayerPower();
// 		sendroot["viplv"] = selfplayer->getVipLevel();
// 		sendroot["level"] = selfplayer->getPlayerLeval();
// 		sendroot["errorId"] =  LynxErrno::None;
// 		Json::StyledWriter writer;
// 
// 		addFriendNotify.mRespJsonStr = writer.write(sendroot);
// 
// 		cout << addFriendNotify.mRespJsonStr;
// 		//对方玩家的connid，发给对方玩家
// 		NetworkSystem::getSingleton().sendMsg(addFriendNotify,otherconnId);
// 
// 		Json::Value sendrootA;
// 		sendrootA["errorId"] =  LynxErrno::None;
// 		sendrootA["playeruid"] = playerUid;
// 
// 		addFriendNotifyA.mRespJsonStr = writer.write(sendrootA);
// 
// 		NetworkSystem::getSingleton().sendMsg(addFriendNotifyA,connId);
// 		BaseInfoData baseInfoData;
// 		baseInfoData.playerUid =selfUid;
// 		baseInfoData.modelId = selfplayer->getPlayerModelID();
// 		baseInfoData.name = selfplayer->getPlayerName().c_str();
// 		baseInfoData.power = selfplayer->getPlayerPower();
// 		baseInfoData.vipLv = selfplayer->getVipLevel();
// 		baseInfoData.level  = selfplayer->getPlayerLeval();
// 		baseInfoData.leaveTime  = selfplayer->getPlayerLeaveTime()/1000;
// 
// 
// 		otherplayer->getFriendBlackManager().addFriendBeApply(baseInfoData);
// 
// 
// 	}
// 	else
// 	{
// 		LOG_INFO("empty message friend add!");
// 	}
// 
// 	Json::StyledWriter writer;
// 
// 	addFriendNotify.mRespJsonStr = writer.write(sendroot);
// 


	Json::Value sendroot;

	Json::StyledWriter writer;
	FriendMsgHandler::getSingleton().dealAddFriendReq(connId, msg,sendroot);
	addFriendNotifyA.mRespJsonStr = writer.write(sendroot);
	NetworkSystem::getSingleton().sendMsg(addFriendNotifyA,connId);

}



void FriendMsgHandler::dealAddFriendReq(const ConnId& connId, CGFriendAddReq& msg,	Json::Value &sendroot)
{
	UInt64 playerUid =0;
	UInt64 selfUid = 0;
	GCFriendAddNotifyA addFriendNotifyA;
	FriendBlackInfo friendInfo;
	FriendBlackInfo otherFriendInfo;

	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;
	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		playerUid = root["playeruid"].asUInt64();
		selfUid = root["selfuid"].asUInt64();
	}
	else
	{
		return;
	}

	Player * otherplayer = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(!otherplayer)
	{		
		bool success =  ConsortSystem::getSingleton().getOtherFriendInfo(playerUid, otherFriendInfo);
		if(!success)
		{
			return;
		}

		UInt32 ret = ConsortSystem::getSingleton().getOtherBeApplyGuidList(playerUid,otherFriendInfo.friendBeApplyGuidList);
		if(ret != LynxErrno::None)
		{
			return;
		}

	}


	sendroot["errorId"] =  LynxErrno::None;
	sendroot["playeruid"] = playerUid;

	//对方玩家是否在黑名单中
	bool inblack = FriendMsgHandler::getSingleton().judgeBlackBack(selfUid,playerUid,friendInfo);
	if(inblack)
	{
		sendroot["errorId"] =  LynxErrno::InSelfBlack;
		return;
	}

	//自己是否在对方玩家黑名单中
	bool inblack2 =  FriendMsgHandler::getSingleton().judgeBlackBack(playerUid,selfUid,otherFriendInfo);
	if(inblack2)
	{
		sendroot["errorId"] =  LynxErrno::InOtherBlack;
		return;
	}

	bool overNum = FriendMsgHandler::getSingleton().judgeFriendNumBack(selfUid,friendInfo);
	if(overNum)
	{
		sendroot["errorId"] =  LynxErrno::SelfFriendFull;
		return;
	}

		if (sendroot["errorId"]  == LynxErrno::None)
		{
			//消息通知对方玩家
			GCFriendAddNotify addFriendNotify;
			addFriendNotify.mPacketID = BOC_FRIENDADD_NOTIFY;

			Json::Value sendrootA;
			sendrootA["fromuid"] = selfUid;
			sendrootA["modelid"] = selfplayer->getPlayerModelID();
			sendrootA["name"] = selfplayer->getPlayerName();
			sendrootA["power"] = selfplayer->getPlayerPower();
			sendrootA["viplv"] = selfplayer->getVipLevel();
			sendrootA["level"] = selfplayer->getPlayerLeval();
			sendrootA["errorId"] =  LynxErrno::None;
			
			if (otherplayer != NULL)
			{
				Json::StyledWriter writer;
				addFriendNotify.mRespJsonStr = writer.write(sendrootA);
				const ConnId & connId = otherplayer->getConnId();
				if(connId)
				{
						NetworkSystem::getSingleton().sendMsg(addFriendNotify,connId);
				}
				
			}
		

			BaseInfoData baseInfoData;
			baseInfoData.playerUid =selfUid;
			baseInfoData.modelId = selfplayer->getPlayerModelID();
			baseInfoData.name = selfplayer->getPlayerName().c_str();
			baseInfoData.power = selfplayer->getPlayerPower();
			baseInfoData.vipLv = selfplayer->getVipLevel();
			baseInfoData.level  = selfplayer->getPlayerLeval();
			baseInfoData.leaveTime  = selfplayer->getPlayerLeaveTime();

			FriendMsgHandler::getSingleton().addFriendBeApplyBack(playerUid,baseInfoData,otherFriendInfo.friendBeApplyGuidList);
		}




}

void FriendMsgHandler::onAddFriendBack(const ConnId& connId, CGFriendAddBackReq& msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	BaseInfoData baseInfoData;
	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 otherUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();
		UInt32 agree = root["agree"].asUInt();

		//A发给B加好友请求，服务器收到B拒绝添加好友的回复，那么双方都不通知，直接返回
		if(agree==0)
		{
			GCFriendAddBackResp  addBackResp;
			addBackResp.mPacketID = BOC_ADDAGRU_RESP;
			Json::Value root;
			//0拒绝1同意2已满3对方已满 10一键拒绝
			root["agree"] = 0;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] =otherUid;
			Json::StyledWriter writer;
			addBackResp.mRespJsonStr = writer.write(root);
			cout << addBackResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(addBackResp, connId);
			selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);
			return;
		}

		if(agree==10)
		{
			GCFriendAddBackResp  addBackResp;
			addBackResp.mPacketID = BOC_ADDAGRU_RESP;
			Json::Value root;
			//0拒绝1同意2已满3对方已满
			root["agree"] = 10;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] =otherUid;
			Json::StyledWriter writer;
			addBackResp.mRespJsonStr = writer.write(root);
			cout << addBackResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(addBackResp, connId);
			selfplayer->getFriendBlackManager().delFriendBeAllApply();
			return;
		}

		//判断自己好友是否加满了
		UInt32 selfFriendCount = selfplayer->getFriendBlackManager().getFriendCount();

		if(selfFriendCount >= 50)
		{
			GCFriendAddBackResp  addBackResp;
			addBackResp.mPacketID = BOC_ADDAGRU_RESP;
			Json::Value root;
			//0拒绝1同意2已满3对方已满
			root["agree"] = 2;
			root["errorId"] = LynxErrno::None;
			Json::StyledWriter writer;
			addBackResp.mRespJsonStr = writer.write(root);
			cout << addBackResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

			return;
		}


		//如果B同意了，那么审核A的好友是否满了
		Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherUid);
		//内存中有该玩家数据
		if(otherPlayer)
		{
			UInt32 otherFriendCount = otherPlayer->getFriendBlackManager().getFriendCount();
			//判断对方好友是否满了
			if(otherFriendCount >= 50)
			{
				//对方好友达到50人，好友已满了
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				//0拒绝1同意2已满3对方已满
				root["agree"] = 3;
				root["errorId"] = LynxErrno::None;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);
				selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);

				return;
			}

			bool inblack = selfplayer->getFriendBlackManager().judgeBlack(otherUid);
			if(inblack)
			{
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				//0拒绝1同意2已满3对方已满4对方在自己的黑名单里5自己在对方的黑名单里
				root["agree"] = 4;
				root["errorId"] = LynxErrno::None;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);
				selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);

				return;
			}
			bool inOtherblack = otherPlayer->getFriendBlackManager().judgeBlack(selfUid);
			if(inOtherblack)
			{
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				//0拒绝1同意2已满3对方已满4对方在自己的黑名单里5自己在对方的黑名单里
				root["agree"] = 5;
				root["errorId"] = LynxErrno::None;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);
				selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);

				return;
			}

			//添加双方为好友
			selfplayer ->getFriendBlackManager().addFriend(otherUid);
			otherPlayer ->getFriendBlackManager().addFriend(selfUid);
			selfplayer->getAchieveManager().updateAchieveData(FRIENDCNT,1);
			otherPlayer->getAchieveManager().updateAchieveData(FRIENDCNT,1);
			selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);
			otherPlayer->getFriendBlackManager().delFriendBeApply(selfUid);


			//给B,也就是同意的一方回包，告诉同意成功

			GCFriendAddBackResp  addBackResp;
			addBackResp.mPacketID = BOC_ADDAGRU_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherUid;

			//详细信息
			root["modelid"] = otherPlayer->getPlayerModelID();
			root["name"] = otherPlayer->getPlayerName();
			root["power"] = otherPlayer->getPlayerPower();
			root["viplv"] = otherPlayer->getVipLevel();
			root["level"] = otherPlayer->getPlayerLeval();
			root["leaveTime"] = otherPlayer->getPlayerLeaveTime()/1000;
			//0拒绝1同意2已满3对方已满
			root["agree"] = 1;


			//////////////////////////////////////////
			Json::StyledWriter writer;
			addBackResp.mRespJsonStr = writer.write(root);
			cout << addBackResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

			//判断A是否在线，通知最开始请求添加好友的一方添加好友成功信息
			const ConnId&  otherconnId = otherPlayer->getConnId();
			if(otherconnId)
			{
				GCFriendAddBackRespA friendAddRespA;	
				friendAddRespA.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value addRespRoot;
				addRespRoot["playeruid"] =  selfUid;
				addRespRoot["errorId"] = LynxErrno::None;
				//告诉对方自己的详细信息
				addRespRoot["modelid"] = selfplayer->getPlayerModelID();
				addRespRoot["name"] = selfplayer->getPlayerName();
				addRespRoot["power"] = selfplayer->getPlayerPower();
				addRespRoot["viplv"] = selfplayer->getVipLevel();
				addRespRoot["level"] = selfplayer->getPlayerLeval();
				addRespRoot["leaveTime"] = 0;
				//0拒绝1同意2已满3对方已满
				addRespRoot["agree"] = 1;
				


				Json::StyledWriter writer2;

				friendAddRespA.mRespJsonStr = writer2.write(addRespRoot);
				cout << friendAddRespA.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(friendAddRespA, otherconnId);
			}

			return;
		}
		else
		{
			//内存中没有该玩家的数据
			//从数据库读该玩家的数据

			FriendBlackInfo otherFriendInfo;
			bool success =  ConsortSystem::getSingleton().getOtherFriendInfo(otherUid, otherFriendInfo);
			if(!success)
			{
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);
				selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);
				return;
			}

			if(otherFriendInfo.friendCount >= 50)
			{
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				//0拒绝1同意2已满3对方已满
				root["agree"] = 3;
				root["errorId"] = LynxErrno::None;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);
				selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);
				return;
			}

			//该玩家好友没有满

			//双方添加好友
			//B，同意的一方添加好友
			selfplayer ->getFriendBlackManager().addFriend(otherUid);			
			//给B,也就是同意的一方回包，告诉同意成功

			GCFriendAddBackResp  addBackResp;
			addBackResp.mPacketID = BOC_ADDAGRU_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherUid;
			BaseInfoData otherbaseInfo;
			bool getRes = LogicSystem::getSingleton().getBaseInfo(otherUid, otherbaseInfo);
			selfplayer->getFriendBlackManager().delFriendBeApply(otherUid);
// 			otherPlayer->getFriendBlackManager().delFriendBeApply(selfUid);


			if(!getRes)
			{
				//bool resultDb = selfplayer->getFriendBlackManager().getBaseInfoFromDb(otherUid,  otherbaseInfo);
				bool resultDb = ConsortSystem::getSingleton().getBaseInfoFromDb(otherUid, otherbaseInfo);
				if(resultDb)
				{
					LogicSystem::getSingleton().insertBaseInfo(otherbaseInfo);
				}
			}

			root["errorId"] = LynxErrno::None;
			root["modelid"] = otherbaseInfo.modelId;
			root["name"] = otherbaseInfo.name.c_str();
			root["power"] = otherbaseInfo.power;
			root["viplv"] = otherbaseInfo.vipLv;
			root["level"] = otherbaseInfo.level;
			root["leaveTime"] = otherbaseInfo.leaveTime/1000;
			//0拒绝1同意2已满3对方已满
			root["agree"] = 1;

			Json::StyledWriter writer;
			addBackResp.mRespJsonStr = writer.write(root);
			cout << addBackResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

			//由于开始申请的一方，A没有在线，离线更新数据库
			PersistAddFriendOffLineNotify addOffLine;
			addOffLine.m_nMyselfUid = otherUid;
			addOffLine.m_nPlayerUid = selfUid;
			addOffLine.blackStr = otherFriendInfo.blackstr;
			addOffLine.friendStr = otherFriendInfo.friendstr;
			addOffLine.friendCount = otherFriendInfo.friendCount;

			PersistSystem::getSingleton().postThreadMsg(addOffLine, otherUid);

			//更新离线玩家好友数量成就
			OffLineFriendUpdateMsg achieveUpdate;
			achieveUpdate.playerUid = otherUid;
			achieveUpdate.count = otherFriendInfo.friendCount;
			PersistSystem::getSingleton().postThreadMsg(achieveUpdate, otherUid);

		}
	}
	else
	{
		LOG_INFO("empty message friend addback!");
	}

}


void FriendMsgHandler::onDelFriendReq(const ConnId&connId, CGFriendDelReq&msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Reader reader;
	Json::Value root;

	if(reader.parse(msg.mReqJsonStr, root))
	{
		UInt64 otherUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();

		//判断对方是否在内存中
		Player* otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherUid);
		//在内存中
		if(otherPlayer)
		{

			bool selfDel = selfplayer->getFriendBlackManager().delFriend(otherUid);
			if(!selfDel)
			{
				GCFriendDelResp delResp;
				delResp.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				delResp.mRespJsonStr = writer.write(root);
				cout << delResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delResp, connId);
				return ;
			}

			bool otherDel = otherPlayer->getFriendBlackManager().delFriend(selfUid);

			if(!otherDel)
			{	
				GCFriendDelResp delResp;
				delResp.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				delResp.mRespJsonStr = writer.write(root);
				cout << delResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delResp, connId);
				return ;
			}

			//发送删除成功的回复
			GCFriendDelResp delResp;
			delResp.mPacketID = BOC_FRIENDDEL_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherUid;
			Json::StyledWriter writer;
			delResp.mRespJsonStr = writer.write(root);
			cout << delResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(delResp, connId);


			//此时判断对方是否在线，在线则通知对方，对方从好友列表中将自己移除了
			const ConnId & otherconnId = otherPlayer->getConnId();
			if(otherconnId)
			{
				GCFriendDelResp delNotify;
				delNotify.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["playeruid"] = selfUid;
				root["errorId"] = LynxErrno::None;

				Json::StyledWriter writer;

				delNotify.mRespJsonStr = writer.write(root);
				cout << delNotify.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delNotify, otherconnId);
			}

		}
		else
		{
			//不在内存中

			FriendBlackInfo otherFriendInfo;

			bool success =  ConsortSystem::getSingleton().getOtherFriendInfo(otherUid, otherFriendInfo);
			if(!success)
			{
				GCFriendDelResp delResp;
				delResp.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				delResp.mRespJsonStr = writer.write(root);
				cout << delResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delResp, connId);
				return ;
			}

			//自己先从好友列表中删除对方

			bool successdel = selfplayer->getFriendBlackManager().delFriend(otherUid);
			if(!successdel)
			{
				GCFriendDelResp delResp;
				delResp.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				delResp.mRespJsonStr = writer.write(root);
				cout << delResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delResp, connId);
				return ;
			}

			//通知客户端删除成功
			GCFriendDelResp delResp;
			delResp.mPacketID = BOC_FRIENDDEL_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherUid;
			Json::StyledWriter writer;
			delResp.mRespJsonStr = writer.write(root);
			cout << delResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(delResp, connId);

			//要删除的对象不在内存中，需要通知数据库删除
			PersistDelFriendOffLineNotify delOffLine;
			delOffLine.m_nMyselfUid = otherUid;
			delOffLine.m_nPlayerUid = selfUid;
			delOffLine.blackStr = otherFriendInfo.blackstr;
			delOffLine.friendStr = otherFriendInfo.friendstr;
			delOffLine.friendCount = otherFriendInfo.friendCount;

			PersistSystem::getSingleton().postThreadMsg(delOffLine, otherUid);

		}

	}
	else
	{
		LOG_INFO("empty message friend del!");
	}

}


void FriendMsgHandler::onAddBlackReq(const ConnId& connId, CGBlackAddReq& msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		//对方uid
		UInt64 otherplayerUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();

		bool success = selfplayer->getFriendBlackManager().addBlack(otherplayerUid);

		if(success)
		{
			//判断对方是否在好友列表中
			bool infriend = selfplayer->getFriendBlackManager().judgeFriend(otherplayerUid);

			//移除彼此的好友关系
			if(infriend)
			{
				//判断对方是否在内存中
				Player* otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherplayerUid);
				if(otherPlayer)
				{
					//在内存中
					bool selfDel = selfplayer->getFriendBlackManager().delFriend(otherplayerUid);
					bool otherDel = otherPlayer->getFriendBlackManager().delFriend(selfUid);
					selfplayer->getFriendBlackManager().delFriendBeApply(otherplayerUid);
					otherPlayer->getFriendBlackManager().delFriendBeApply(selfUid);


					//此时判断对方是否在线，在线则通知对方，对方从好友列表中将自己移除了
					const ConnId & otherconnId = otherPlayer->getConnId();
					if(otherconnId)
					{
						GCFriendDelResp delNotify;
						delNotify.mPacketID = BOC_FRIENDDEL_RESP;
						Json::Value root;
						root["playeruid"] = selfUid;
						root["errorId"] = LynxErrno::None;

						Json::StyledWriter writer;

						delNotify.mRespJsonStr = writer.write(root);
						cout << delNotify.mRespJsonStr;
						NetworkSystem::getSingleton().sendMsg(delNotify, otherconnId);
					}

				}
				else
				{
					//不在内存中
					FriendBlackInfo otherFriendInfo;

					bool success =  ConsortSystem::getSingleton().getOtherFriendInfo(otherplayerUid, otherFriendInfo);
					//自己先从好友列表中删除对方

					bool successdel = selfplayer->getFriendBlackManager().delFriend(otherplayerUid);

					if(success)
					{
						//要删除的对象不在内存中，需要通知数据库删除
						PersistDelFriendOffLineNotify delOffLine;
						delOffLine.m_nMyselfUid = otherplayerUid;
						delOffLine.m_nPlayerUid = selfUid;
						delOffLine.blackStr = otherFriendInfo.blackstr;
						delOffLine.friendStr = otherFriendInfo.friendstr;
						delOffLine.friendCount = otherFriendInfo.friendCount;

						PersistSystem::getSingleton().postThreadMsg(delOffLine, otherplayerUid);
					}


				}

			}

			GCBlackAddResp blackAddResp;
			blackAddResp.mPacketID = BOC_ADDBLACK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherplayerUid;

			////////////////////////////////////////////////////
			BaseInfoData otherbaseInfo;
			bool getRes = LogicSystem::getSingleton().getBaseInfo(otherplayerUid, otherbaseInfo);

			if(!getRes)
			{
				//bool resultDb = selfplayer->getFriendBlackManager().getBaseInfoFromDb(otherplayerUid,  otherbaseInfo);
				bool resultDb = ConsortSystem::getSingleton().getBaseInfoFromDb(otherplayerUid, otherbaseInfo);
				if(resultDb)
				{
					LogicSystem::getSingleton().insertBaseInfo(otherbaseInfo);
				}
			}



			///////////////////////////////////////////////////////////
			root["modelid"] = otherbaseInfo.modelId;
			root["name"] = otherbaseInfo.name.c_str();
			root["power"] = otherbaseInfo.power;
			root["viplv"] = otherbaseInfo.vipLv;
			root["level"] = otherbaseInfo.level;
			root["leaveTime"] = otherbaseInfo.leaveTime/1000;
			root["errorId"] = LynxErrno::None;

			Json::StyledWriter writer;

			blackAddResp.mRespJsonStr = writer.write(root);

			cout <<blackAddResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackAddResp, connId);
		}
		else
		{

			GCBlackAddResp blackAddResp;
			blackAddResp.mPacketID = BOC_ADDBLACK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::PlayerNotExist;
			Json::StyledWriter writer;

			blackAddResp.mRespJsonStr = writer.write(root);

			cout <<blackAddResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackAddResp, connId);
		}


	}
	else
	{
		LOG_INFO("empty message black add!");
	}

}

void FriendMsgHandler::onDelBlackReq(const ConnId& connId, CGBlackDelReq&msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		//对方uid
		UInt64 playerUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();

		bool success = player->getFriendBlackManager().delBlack(playerUid);

		if(success)
		{
			GCBlackDelResp blackDelResp;
			blackDelResp.mPacketID = BOC_DELBALCK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = playerUid;

			Json::StyledWriter writer;

			blackDelResp.mRespJsonStr = writer.write(root);

			cout <<blackDelResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackDelResp, connId);
		}
		else
		{
			GCBlackDelResp blackDelResp;
			blackDelResp.mPacketID = BOC_DELBALCK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::PlayerNotExist;


			Json::StyledWriter writer;

			blackDelResp.mRespJsonStr = writer.write(root);

			cout <<blackDelResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackDelResp, connId);
		}


	}
	else
	{
		LOG_INFO("empty message black del!");
	}
}


void FriendMsgHandler::onRecommendListReq(const ConnId&connId, CGRecommendListReq&msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		//原来的结构
		/*const Set<Player > &playerSet = LogicSystem::getSingleton().getPlayerLvSet(player);

		Json::Value root;
		root["errorId"] = LynxErrno::None;

		for(const Set<Player >::Iter * setIter = playerSet.begin();  setIter != NULL;
		setIter = playerSet.next(setIter))
		{
		Json::Value playerRoot;
		playerRoot["playeruid"] = setIter->mValue.getPlayerGuid();
		playerRoot["modelid"] = setIter->mValue.getPlayerModelID();
		playerRoot["name"] = setIter->mValue.getPlayerName();
		playerRoot["power"] = setIter->mValue.getPlayerPower();
		playerRoot["viplv"] = setIter->mValue.getVipLevel();
		playerRoot["level"] = setIter->mValue.getPlayerLeval();
		root["playerlist"].append(playerRoot);
		}*/

		List<BaseInfoData> playerList;
		// 		RankSystem::getSingleton().getOnlinePlayers(player->getPlayerLeval(), player, playerList);
		RankSystem::getSingleton().getLvSimilarPlayers(player->getPlayerLeval(), player, playerList);

		for(List<BaseInfoData>::Iter * setIter = playerList.begin();  setIter != NULL;
			setIter = playerList.next(setIter))
		{
			Json::Value playerRoot;
			playerRoot["playeruid"] = setIter->mValue.playerUid;
			playerRoot["modelid"] = setIter->mValue.modelId;
			playerRoot["name"] = setIter->mValue.name.c_str();
			playerRoot["power"] = setIter->mValue.power;
			playerRoot["viplv"] = setIter->mValue.vipLv;
			playerRoot["level"] = setIter->mValue.level;
			root["playerlist"].append(playerRoot);
		}

		Json::StyledWriter writer;
		GCRecommendListResp commendListResp;
		commendListResp.mPacketID = BOC_RECOMMENDLIST_RESP;
		commendListResp.mRespJsonStr = writer.write(root);
		cout << commendListResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(commendListResp, connId);
	}
	else
	{
		LOG_INFO("empty message commend List Req");
	}
}


void FriendMsgHandler::onFindPlayerReq(const ConnId &connId,  CGFindPlayerReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		std::string keyStr = root["keystr"].asString();
		UInt32 type = root["type"].asUInt();

		Player * findPlayer = NULL;

		//1 uuid
		if(type)
		{
			std::stringstream mystream;
			mystream << keyStr;
			UInt64 playerUid = 0;
			mystream >> playerUid;

			findPlayer = LogicSystem::getSingleton().getPlayerByGuid(playerUid);

			if(!findPlayer)
			{
				Json::Value playerRoot;
				playerRoot["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				GCFindPlayerResp findResp;
				findResp.mRespJsonStr = writer.write(playerRoot);
				cout << findResp.mRespJsonStr;
				findResp.mPacketID = BOC_FINDPLAYER_RESP;
				NetworkSystem::getSingleton().sendMsg(findResp, connId);
				return;
			}


		}
		else
		{
			findPlayer = LogicSystem::getSingleton().getPlayerByName(keyStr.c_str());
			if(!findPlayer)
			{
				Json::Value playerRoot;
				playerRoot["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				GCFindPlayerResp findResp;
				findResp.mRespJsonStr = writer.write(playerRoot);
				cout << findResp.mRespJsonStr;
				findResp.mPacketID = BOC_FINDPLAYER_RESP;
				NetworkSystem::getSingleton().sendMsg(findResp, connId);

				return;
			}
		}


		Json::Value playerRoot;
		playerRoot["playeruid"] = findPlayer->getPlayerGuid();
		playerRoot["modelid"] = findPlayer->getPlayerModelID();
		playerRoot["name"] = findPlayer->getPlayerName();
		playerRoot["power"] = findPlayer->getPlayerPower();
		playerRoot["viplv"] = findPlayer->getVipLevel();
		playerRoot["level"] = findPlayer->getPlayerLeval();
		playerRoot["errorId"] = LynxErrno::None;

		Json::StyledWriter writer;
		GCFindPlayerResp findResp;
		findResp.mRespJsonStr = writer.write(playerRoot);
		cout << findResp.mRespJsonStr;
		findResp.mPacketID = BOC_FINDPLAYER_RESP;
		NetworkSystem::getSingleton().sendMsg(findResp, connId);


	}
	else
	{
		LOG_INFO("empty message findPlayer  Req");
	}
}	



void FriendMsgHandler::onFriendReq(const ConnId& connId, CGFriendBlackListReq& msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 playerUid = root["playeruid"].asUInt64();
		// 		UInt64 selfUid = root["selfuid"].asUInt64();

		//对方玩家指针
		Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
		if(!player)
		{
			GCFriendAddNotify addFriendNotify;
			Json::Value sendroot;			
			Json::StyledWriter writer;
			addFriendNotify.mRespJsonStr = writer.write(sendroot);
			NetworkSystem::getSingleton().sendMsg(addFriendNotify,selfplayer->getConnId());
			return;
		}
		GCFriendBlackListResp friendblackListResp;
		friendblackListResp.mPacketID = BOC_FRIENDBLACKLIST_RESP;
		friendblackListResp.mRespJsonStr = player->getFriendBlackManager().convertDataToJson();
		//cout << friendblackListResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(friendblackListResp, connId);

	}
}



bool FriendMsgHandler::judgeFriendBack(UInt64 playerUid,UInt64 otherUid,FriendBlackInfo &otherFriendInfo)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		return player->getFriendBlackManager().judgeFriend(otherUid);
	}
	else
	{

		Vector<String> strVector;
		lynxStrSplit(otherFriendInfo.friendstr, ";", strVector, true);

		for (UInt32 i =0;i<strVector.size();i++)
		{
			UInt64 uid = lynxAtoi<UInt64>(strVector[i].c_str());
	
			if ( uid== otherUid)
			{
				return true;
			}
		}


	}
	return false;

}

bool FriendMsgHandler::judgeBlackBack(UInt64 playerUid,UInt64 otherUid,FriendBlackInfo &otherFriendInfo)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		return player->getFriendBlackManager().judgeBlack(otherUid);
	}
	else
	{

		Vector<String> strVector;
		lynxStrSplit(otherFriendInfo.blackstr, ";", strVector, true);

		for (UInt32 i =0;i<strVector.size();i++)
		{
			UInt64 uid = lynxAtoi<UInt64>(strVector[i].c_str());
			if ( uid== otherUid)
			{
				return true;
			}
		}

	

		return false;

	}
	return false;
}

bool FriendMsgHandler::addFriendBeApplyBack(UInt64 playerUid,BaseInfoData baseInfoData,List<UInt64> &friendBeApplyGuidList)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		player->getFriendBlackManager().addFriendBeApply(baseInfoData);
		return true;
	}
	else
	{
		for (List<UInt64>::Iter* iter = friendBeApplyGuidList.begin();iter!=NULL;iter = friendBeApplyGuidList.next(iter))
		{
			if (iter->mValue == 0)
			{
				iter->mValue = baseInfoData.playerUid;

				PersistUpdateFriendBeApplyOffLineDataToDb req;
				req.m_nPlayerUid = playerUid;
				req.friendBeApplyGuidList = friendBeApplyGuidList;	
				PersistSystem::getSingleton().postThreadMsg(req, 0);

				return true;
			}
		}
	}
	return false;
}

bool FriendMsgHandler::delFriendBeApplyBack(UInt64 playerUid,List<UInt64> &friendBeApplyGuidList)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{		
		player->getFriendBlackManager().delFriendBeApply(playerUid);		
	}
	else
	{
		for (List<UInt64>::Iter * iter = friendBeApplyGuidList.begin();iter!=NULL;iter = friendBeApplyGuidList.next(iter))
		{	
			if (iter->mValue == playerUid )
			{
				iter->mValue = 0;
				break;
			}			
		}

		
		
		PersistUpdateFriendBeApplyOffLineDataToDb req;
		req.m_nPlayerUid = playerUid;
		req.friendBeApplyGuidList = friendBeApplyGuidList;	

		PersistSystem::getSingleton().postThreadMsg(req, 0);

	}
return false;
}

bool FriendMsgHandler::delFriendBeAllApplyBack(UInt64 playerUid,List<UInt64> &friendBeApplyGuidList)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		player->getFriendBlackManager().delFriendBeAllApply();
	}
	else
	{
		for (List<UInt64>::Iter * iter = friendBeApplyGuidList.begin();iter!=NULL;iter = friendBeApplyGuidList.next(iter))
		{			
			iter->mValue = 0;
		}

		PersistUpdateFriendBeApplyOffLineDataToDb req;
		req.m_nPlayerUid = playerUid;
		req.friendBeApplyGuidList = friendBeApplyGuidList;	

		PersistSystem::getSingleton().postThreadMsg(req, 0);

	}
	return false;
}

bool FriendMsgHandler::getFriendApplyListBack(UInt64 playerUid,List<BaseInfoData>& baseInfoDataList)
{
	Player * otherPlayer  = NULL;
	UInt32 nowTime = TimeUtil::getTimeSec(); 

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		baseInfoDataList =  player->getFriendBlackManager().getFriendApplyList();
		for (List<BaseInfoData>::Iter * iter = baseInfoDataList.begin();iter!=NULL;iter = baseInfoDataList.next(iter))
		{
			otherPlayer =  LogicSystem::getSingleton().getPlayerByGuid(iter->mValue.playerUid);
			if (player == NULL)
			{
				if (iter->mValue.leaveTime == 0)
				{
					iter->mValue.leaveTime = nowTime;
				}				
			}
			else
			{
				iter->mValue.leaveTime = 0;
			}
		}

		return true;
	}
	else
	{
		//离线是Lis<uint64>

	}
return false;
}

bool FriendMsgHandler::judgeOtherBeApplyBack(UInt64 playerUid,UInt64 otherUid,List<UInt64> &friendBeApplyGuidList)
{
	UInt32 getIt = 0;
	Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherUid);
	if(otherPlayer)
	{
		return otherPlayer->getFriendBlackManager().judgeOtherBeApply(playerUid);
	}
	else
	{
	
		for (List<UInt64>::Iter * iter = friendBeApplyGuidList.begin();iter!=NULL; iter = friendBeApplyGuidList.next(iter))
		{
			if (iter->mValue == playerUid)
			{
				return true;
			}
			if (iter->mValue == 0)
			{
				getIt = 1;
			}

		}
		if (getIt == 1)//还可以添加好友申请
		{
			return true;
		}

		return false;

	}
return false;
}



void FriendMsgHandler::saveFriendDataBack(UInt64 playerUid,FriendBlackInfo &otherFriendInfo)
{
	UInt32 getIt = 0;
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		player->getAchieveManager().updateAchieveData(FRIENDCNT,1);
	}
	else
	{
		
		//由于开始申请的一方，A没有在线，离线更新数据库
		PersistAddFriendOffLineNotify addOffLine;
		addOffLine.m_nMyselfUid = playerUid;
		addOffLine.m_nPlayerUid = 0;
		addOffLine.blackStr = otherFriendInfo.blackstr;
		addOffLine.friendStr = otherFriendInfo.friendstr;
		addOffLine.friendCount = otherFriendInfo.friendCount;

		PersistSystem::getSingleton().postThreadMsg(addOffLine, playerUid);

	}
}



bool FriendMsgHandler::delFriendBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo)
{
	char tmp[64]={0};
	char dest[8192] = {0};

	
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		player->getFriendBlackManager().delFriend(delUid);
	}
	else
	{

		Vector<String> strVector;
		lynxStrSplit(otherFriendInfo.friendstr, ";", strVector, true);

		for (UInt32 i =0;i<strVector.size();i++)
		{
			UInt64 uid = lynxAtoi<UInt64>(strVector[i].c_str());

			if ( uid== delUid)
			{
				continue;
			}
			memset(tmp, 0, sizeof(tmp));
			snprintf(tmp, sizeof(tmp), "%llu;",uid);	
			strcat(dest,tmp);
		}
		otherFriendInfo.friendstr = (String)dest;


	}

	return false;

}

bool FriendMsgHandler::addFriendBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo)
{
	char tmp[64]={0};
	char dest[8192] = {0};
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		player->getFriendBlackManager().addFriend(delUid);
	}
	else
	{
		Vector<String> strVector;
		lynxStrSplit(otherFriendInfo.friendstr, ";", strVector, true);

		for (UInt32 i =0;i<strVector.size();i++)
		{
			UInt64 uid = lynxAtoi<UInt64>(strVector[i].c_str());

			memset(tmp, 0, sizeof(tmp));
			snprintf(tmp, sizeof(tmp), "%llu;",uid);	
			strcat(dest,tmp);
		}
		memset(tmp, 0, sizeof(tmp));
		snprintf(tmp, sizeof(tmp), "%llu;",delUid);	
		strcat(dest,tmp);
		otherFriendInfo.friendstr = (String)dest;
	}
	return false;
}

bool FriendMsgHandler::delBlackBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo)
{
	char tmp[64]={0};
	char dest[8192] = {0};
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		player->getFriendBlackManager().delBlack(delUid);
	}
	else
	{

		Vector<String> strVector;
		lynxStrSplit(otherFriendInfo.blackstr, ";", strVector, true);

		for (UInt32 i =0;i<strVector.size();i++)
		{
			UInt64 uid = lynxAtoi<UInt64>(strVector[i].c_str());

			if ( uid== delUid)
			{
				continue;
			}
			memset(tmp, 0, sizeof(tmp));
			snprintf(tmp, sizeof(tmp), "%llu;",uid);	
			strcat(dest,tmp);
		}
		otherFriendInfo.blackstr = (String)dest;
	}
	return false;
}

bool FriendMsgHandler::addBlackBack(UInt64 playerUid,UInt64 delUid,FriendBlackInfo &otherFriendInfo)
{
	char tmp[64]={0};
	char dest[8192] = {0};
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		player->getFriendBlackManager().addBlack(delUid);
	}
	else
	{
		Vector<String> strVector;
		lynxStrSplit(otherFriendInfo.blackstr, ";", strVector, true);

		for (UInt32 i =0;i<strVector.size();i++)
		{
			UInt64 uid = lynxAtoi<UInt64>(strVector[i].c_str());

			memset(tmp, 0, sizeof(tmp));
			snprintf(tmp, sizeof(tmp), "%llu;",uid);	
			strcat(dest,tmp);
		}
		memset(tmp, 0, sizeof(tmp));
		snprintf(tmp, sizeof(tmp), "%llu;",delUid);	
		strcat(dest,tmp);
		otherFriendInfo.blackstr = (String)dest;
	}
	return false;
}


bool FriendMsgHandler::judgeFriendNumBack(UInt64 playerUid,FriendBlackInfo &friendInfo)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(player)
	{
		List<UInt64>friendList =  player->getFriendBlackManager().getFriendList();
		if (friendList.size()>= 50 )
		{
			return true;
		}
	}
	else
	{

		Vector<String> strVector;
		lynxStrSplit(friendInfo.friendstr, ";", strVector, true);

	if (strVector.size() >= 50)
	{
		return true;
	}


	}
	return false;

}