#include "EmailMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;


void EmailMsgHandler::onGMSendEmail(const ConnId&connId, GMSendReq&msg)
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
		UInt64 sendType = root["sendtype"].asUInt64();
		UInt64 tempId = root["tempid"].asUInt64();
		UInt32 openState = root["openstate"].asUInt();
		UInt32 getState = root["getstate"].asUInt();
		string des = root["des"].asString();
		string contant = root["contant"].asString();
		string from = root["from"].asString();
		string title = root["title"].asString();
		UInt64 times = time(0);

		//判断是否为发送给单个人
		if(sendType)
		{
			PersistAddEmail addEmail;
			EmailData emailData;
			
			emailData.m_nEmailUid = LogicSystem::getSingleton().generateEmailGuid();
			emailData.m_nGetState = getState;
			emailData.m_nOpenState = openState;
			emailData.m_nPlayerUid = sendType;
			emailData.m_nTempId = tempId;
			emailData.m_nTime = times;
			emailData.m_strContent = contant.c_str();
			emailData.m_strDes = des.c_str();
			emailData.m_strFrom = from.c_str();
			emailData.m_strTittle = title.c_str();

			addEmail.mEmailData = emailData;
			PersistSystem::getSingleton().postThreadMsg(addEmail, sendType);
			Player * sendToPlayer = LogicSystem::getSingleton().getPlayerByAccountGuid(sendType);
			
			if(sendToPlayer)
			{
				ConnId sendToCon = sendToPlayer->getConnId();
				sendToPlayer->getEmailManager().addEmail(emailData);

				if(sendToCon)
				{
					//发送消息给玩家
					NewEmailNotify emailNotify;
					emailNotify.mPacketID = EMAIL_NOTIFY;
					Json::Value emailRoot;
					emailRoot["emailuid"] = addEmail.mEmailData.m_nEmailUid;
					emailRoot["tempid"] = tempId;
					emailRoot["openState"] = openState;
					emailRoot["getstate"] = getState;
					emailRoot["des"] = des;
					emailRoot["contant"] = contant;
					emailRoot["from"] = from;
					emailRoot["title"] = title;
					emailRoot["time"] = times;

					Json::StyledWriter writer;

					emailNotify.mRespJsonStr = writer.write(emailRoot);

					NetworkSystem::getSingleton().sendMsg(emailNotify, sendToCon);
				}
				
			}


			return;
		}


		Map<UInt64, BaseInfoData> baseInfoMap = LogicSystem::getSingleton().getBaseInfoMap();
		
		for(Map<UInt64, BaseInfoData>::Iter * baseInfoIter = baseInfoMap.begin();
			baseInfoIter != NULL;  baseInfoIter = baseInfoMap.next(baseInfoIter))
		{
			PersistAddEmail addEmail;
			EmailData emailData;

			emailData.m_nEmailUid = LogicSystem::getSingleton().generateEmailGuid();
			emailData.m_nGetState = getState;
			emailData.m_nOpenState = openState;
			emailData.m_nPlayerUid = baseInfoIter->mValue.playerUid;
			emailData.m_nTempId = tempId;
			emailData.m_nTime = times;
			emailData.m_strContent = contant.c_str();
			emailData.m_strDes = des.c_str();
			emailData.m_strFrom = from.c_str();
			emailData.m_strTittle = title.c_str();
			addEmail.mEmailData = emailData;

			PersistSystem::getSingleton().postThreadMsg(addEmail, addEmail.mEmailData.m_nPlayerUid);
			Player * sendToPlayer = LogicSystem::getSingleton().getPlayerByAccountGuid(addEmail.mEmailData.m_nPlayerUid);
			
			//判断玩家是否在线
			if(sendToPlayer)
			{
				ConnId sendToCon = sendToPlayer->getConnId();
				sendToPlayer->getEmailManager().addEmail(emailData);

				if(sendToCon)
				{
					//发送消息给玩家
					NewEmailNotify emailNotify;
					emailNotify.mPacketID = EMAIL_NOTIFY;
					Json::Value emailRoot;
					emailRoot["emailuid"] = addEmail.mEmailData.m_nEmailUid;
					emailRoot["tempid"] = tempId;
					emailRoot["openState"] = openState;
					emailRoot["getstate"] = getState;
					emailRoot["des"] = des;
					emailRoot["contant"] = contant;
					emailRoot["from"] = from;
					emailRoot["title"] = title;
					emailRoot["time"] = times;

					Json::StyledWriter writer;

					emailNotify.mRespJsonStr = writer.write(emailRoot);

					NetworkSystem::getSingleton().sendMsg(emailNotify, sendToCon);
				}
				
			}
			
	
		}
	

	}
	else
	{
		LOG_INFO("empty message GM send Email!");
	}
}


void EmailMsgHandler::onDelEmailReq(const ConnId & connId, DelEmailReq& msg)
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
		UInt64 emailUid = root["emailuid"].asUInt64();
		selfplayer->getEmailManager().delEmail(emailUid);
		
	}
	else
	{
		LOG_INFO("empty message Email del!");
	}
}

void EmailMsgHandler::onDelEmailOnce(const ConnId & connId, EmailDelOnceReq& msg)
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
			selfplayer->getEmailManager().clearAllEmail();
		
	}
	else
	{
		LOG_INFO("empty message Email del once!");
	}
}

void EmailMsgHandler::onReadEmailReq(const ConnId & connId, EmailOpenReq& msg)
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
		UInt64 emailUid = root["emailuid"].asUInt64();
		selfplayer->getEmailManager().openEmail(emailUid);

	}
	else
	{
		LOG_INFO("empty message Email open once!");
	}
}

void EmailMsgHandler::onGetContantReq(const ConnId & connId, EmailGetContantReq& msg)
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
		UInt64 emailUid = root["emailuid"].asUInt64();
		selfplayer->getEmailManager().getContant(emailUid);
		
	}
	else
	{
		LOG_INFO("empty message Email get once!");
	}
}


void EmailMsgHandler::onGetContantListReq(const ConnId & connId, EmailGetOnceReq& msg)
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
	
		List<UInt64> emailList;
		for(UInt32 i = 0; i < root["emailList"].size(); i++)
		{
			emailList.insertTail(root["emailList"][i].asUInt64());
			
		}
		
		selfplayer->getEmailManager().getContant(emailList);
	}
	else
	{
		LOG_INFO("empty message Email get once!");
	}
}