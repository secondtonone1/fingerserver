#include "ConsortMsgHandler.h"
#include "LogicSystem.h"


using namespace Lynx;

void
ConsortMsgHandler::onClientConsortListReq(const ConnId& connId, CGConsortListReq& msg)
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
		

	   selfplayer->getConsortManager().syncConsortList();



	}
	else
	{
		LOG_INFO("empty message finish req!");
	}
}


void ConsortMsgHandler::onClientConsortFindReq(const ConnId & connId, CGConsortFindReq& msg)
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

		UInt64 consortId = root["consortid"].asUInt64();
		
		 selfplayer->getConsortManager().findConsortById(consortId);
	   
	}
	else
	{
		LOG_INFO("empty message finish req!");
	}
}


void ConsortMsgHandler::onQuickJoinConsortReq(const ConnId & connId, CGConsortQuickJoinReq& msg)
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
		
		selfplayer->getConsortManager().quickJoinConsort();
	   
	}
	else
	{
		LOG_INFO("empty message finish req!");
	}
}

void ConsortMsgHandler::onJoinConsortReq(const ConnId & connId, CGConsortJoinReq& msg)
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

		UInt64 consortid = root["consortid"].asUInt64();
		
		selfplayer->getConsortManager().joinConsort(consortid);
	   
	}
	else
	{
		LOG_INFO("empty message finish req!");
	}
}

void ConsortMsgHandler::onCreateConsortReq(const ConnId & connId, CGConsortCreateReq& msg)
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

		std::string  consortname = root["consortname"].asString();
		
		selfplayer->getConsortManager().createConsort(consortname);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
	
}

void ConsortMsgHandler::onSyncConsortMemberReq(const ConnId & connId, CGConsortMemberReq & msg)
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

		UInt64 consortId = root["consortid"].asUInt64();
		
		selfplayer->getConsortManager().consortMemberSync(consortId);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}

void ConsortMsgHandler::onCheckApplyReq(const ConnId & connId, CGApplyCheckReq& msg)
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

		UInt64 consortId = root["consortid"].asUInt64();
		
		selfplayer->getConsortManager().checkApply(consortId);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}

void ConsortMsgHandler::onDealApplyReq(const ConnId & connId, CGApplyDealReq&msg)
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

		UInt64 applyUid = root["applyuid"].asUInt64();
		UInt32 accept = root["accept"].asUInt();
		UInt64 consortId = root["consortid"].asUInt64();
		
		selfplayer->getConsortManager().dealApply(consortId, applyUid, accept);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}

void ConsortMsgHandler::onClearApplyReq(const ConnId & connId, CGApplyClearReq&msg)
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

		
		UInt64 consortId = root["consortid"].asUInt64();
		
		selfplayer->getConsortManager().dealApplyClear(consortId);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}


void ConsortMsgHandler::onConsortSetReq(const ConnId & connId, CGConsortSetReq & msg)
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

		
		UInt64 consortId = root["consortid"].asUInt64();
		UInt32 needCheck = root["needcheck"].asUInt();
		UInt64 powerLimit = root["condition"].asUInt64();
		

		selfplayer->getConsortManager().setConsortInfo(consortId, needCheck, powerLimit);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}

void ConsortMsgHandler::onSetDescsReq(const ConnId & connId, CGSetDescsReq & msg)
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

		
		UInt64 consortId = root["consortid"].asUInt64();
		std::string descs = root["desc"].asString();
		
		

		selfplayer->getConsortManager().setDescs(consortId, descs);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}


void ConsortMsgHandler::onKickMemberReq(const ConnId & connId, CGKickMemberReq& msg)
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

		
		UInt64 consortId = root["consortid"].asUInt64();
		UInt64 kickId = root["kickid"].asUInt64();
		
		

		selfplayer->getConsortManager().kickMember(consortId, kickId);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}

}

void ConsortMsgHandler::onDestroyConsortReq(const ConnId & connId, CGDestroyConsortReq& msg)
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

		
		UInt64 consortId = root["consortid"].asUInt64();
		
		
		

		selfplayer->getConsortManager().destroyConsort(consortId);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}

}

void ConsortMsgHandler::onLeaveConsortReq(const ConnId & connId, CGLeaveConsortReq & msg)
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
		UInt64 consortId = root["consortid"].asUInt64();
		selfplayer->getConsortManager().leaveConsort(consortId);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}

void ConsortMsgHandler::onSetViceLeaderReq(const ConnId & connId, CGSetViceLeaderReq & msg)
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
		UInt64 otheruid = root["otheruid"].asUInt64();
		UInt32 setup = root["setup"].asUInt();
		selfplayer->getConsortManager().setViceLeader(otheruid, setup);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}

void ConsortMsgHandler::onConsortLogReq(const ConnId & connId, CGConsortlogReq & msg)
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
		UInt64 logtime = root["logtime"].asUInt64();
		
		selfplayer->getConsortManager().logListSync(logtime);
	   
	}
	else
	{
		LOG_INFO("empty message create req!");
	}
}

void ConsortMsgHandler::onConsortDetailReq(const ConnId & connId, CGConsortDetailReq & msg)
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
		UInt64 consortId = root["consortid"].asUInt64();
		
		selfplayer->getConsortManager().consortDetailSync(consortId);
	   
	}
	else
	{
		LOG_INFO("empty consort detail  req!");
	}
}

void ConsortMsgHandler::onConsortSignReq(const ConnId & connId, CGConsortSignReq & msg)
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
		UInt32 signtype = root["signtype"].asUInt();
		
		selfplayer->getConsortManager().consortSign(signtype);
	   
	}
	else
	{
		LOG_INFO("empty consort sign  req!");
	}
}

void ConsortMsgHandler::onConsortSignAwardReq(const ConnId & connId, CGConsortSignAwardReq & msg)
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
		UInt32 awardIndex = root["index"].asUInt();
		
		selfplayer->getConsortManager().consortSignAward(awardIndex);
	   
	}
	else
	{
		LOG_INFO("empty consort sign  req!");
	}
}

void ConsortMsgHandler::onCatTaskGetReq(const ConnId & connId, CGCatTaskGetReq & msg)
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
		UInt32 taskId = root["taskid"].asUInt();
		
		selfplayer->getConsortManager().getCatTask(taskId);
	   
	}
	else
	{
		LOG_INFO(" empty cat task get  req!   CGCatTaskGetReq !");
	}
}

void ConsortMsgHandler::onCatTaskAwardGetReq(const ConnId & connId, CGCatTaskGetAwardReq & msg)
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
		UInt32 taskId = root["taskid"].asUInt();
		
		selfplayer->getConsortManager().getCatTaskAward(taskId);
	   
	}
	else
	{
		LOG_INFO(" empty cat task get award  req!   CGCatTaskGetAwardReq !");
	}
}

void ConsortMsgHandler::onCatTaskRefreshReq(const ConnId & connId, CGCatTaskRefreshReq & msg)
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
		
		selfplayer->getConsortManager().refreshCatTask();
	   
	}
	else
	{
		LOG_INFO(" empty cat task refresh req!   CGCatTaskRefreshReq !");
	}
}

//开始厨房特训
void ConsortMsgHandler::onKitchenBeginReq(const ConnId & connId, CGKitchenBeginReq & msg)
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
		
		selfplayer->getConsortManager().beginKitchenQue();
	   
	}
	else
	{
		LOG_INFO(" empty cat task refresh req!   kitchenbeginreq !");
	}
}

void ConsortMsgHandler::onKitchenEndReq(const ConnId & connId, CGKitchenEndReq & msg)
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
		UInt32 questionid  = root["questionid"].asUInt();
		UInt32 score  = root["score"].asUInt();
		
		selfplayer->getConsortManager().endKitchenQue(questionid, score,MiniLog80);
	   
	}
	else
	{
		LOG_INFO(" empty cat task refresh req!   kitchenbeginreq !");
	}
}

void ConsortMsgHandler::onBusinessCatResetReq(const ConnId & connId, CGBusinessCatResetReq & msg)
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
		UInt64 playeruid  = root["playeruid"].asUInt64();
	
		selfplayer->getConsortManager().businessCatReset();
	   
	}
	else
	{
		LOG_INFO(" empty business cat RESET req!    !");
	}

}

void ConsortMsgHandler::onBusinessCatBuyReq(const ConnId & connId, CGBusinessCatBuyReq & msg)
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
		UInt64 buyIndex  = root["index"].asUInt();
	
		selfplayer->getConsortManager().businessCatBuy(buyIndex);
	   
	}
	else
	{
		LOG_INFO(" empty business cat buy req!    !");
	}
}

void ConsortMsgHandler::onBusinessCatTimeReq(const ConnId & connId, CGBusinessCatTimeReq & msg)
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
		UInt64 playeruid  = root["playeruid"].asUInt64();
	
		selfplayer->getConsortManager().businessCatTime();
	   
	}
	else
	{
		LOG_INFO(" empty business cat time req!    !");
	}
}

void ConsortMsgHandler::onUpdateGuidReq(const ConnId & connId, CGPlayerNewGuidReq & msg)
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
		std::string guidStr  = root["guidStr"].asString();
	
		selfplayer->getConsortManager().updateGuidStr(guidStr);
	   
	}
	else
	{
		LOG_INFO(" empty business cat time req!    !");
	}
}

void ConsortMsgHandler::onEyeQueReq(const ConnId & connId, CGEyeQueReq & msg)
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
		
	
		selfplayer->getConsortManager().generateEyeQue();
	   
	}
	else
	{
		LOG_INFO(" empty onEyeQueReq time req!    !");
	}
 }

void ConsortMsgHandler::onEyeAwardReq(const ConnId & connId, CGEyeawardReq & msg)
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
		std::string  codeStr  = root["code"].asString();
		UInt32  score  = root["score"].asUInt();

		selfplayer->getConsortManager().getEyeAward(codeStr.c_str(), score);
	   
	}
	else
	{
		LOG_INFO(" empty onEyeAwardReq time req!    !");
	}
}

void ConsortMsgHandler::onTicketJoinReq(const ConnId & connId, CGTicketJoinReq& msg)
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
		UInt32 ticketId  = root["ticketid"].asUInt();
		UInt32  sceneIndex  = root["scene"].asUInt();
		UInt64  servantId = root["servantid"].asUInt64();
		
		selfplayer->getConsortManager().ticketJoin(ticketId, sceneIndex, servantId);
		
	   
	}
	else
	{
		LOG_INFO(" empty ticketJoin  req!    !");
	}
}

void ConsortMsgHandler::onTicketGetAwardReq(const ConnId & connId, CGTicketAwardReq& msg)
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
		
		selfplayer->getConsortManager().ticketGetAward();
		
	   
	}
	else
	{
		LOG_INFO(" empty ticketJoin  req!    !");
	}
}	

void ConsortMsgHandler::onSupportReq(const ConnId & connId, CGTicketSupportReq& msg)
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
	
		UInt32 ticketId  = root["ticketid"].asUInt();
		UInt32  grade  = root["grade"].asUInt();
		
		selfplayer->getConsortManager().ticketSupport(ticketId, grade);
	}
	else
	{
		LOG_INFO(" empty ticketJoin  req!    !");
	}
}

void ConsortMsgHandler::onTicketDataReq(const ConnId & connId, CGTicketDataReq& msg)
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
		selfplayer->getConsortManager().getTicketData();
	}
	else
	{
		LOG_INFO(" empty ticketJoin  req!    !");
	}
}

void ConsortMsgHandler::onTicketOneDataReq(const ConnId & connId, CGTicketOneDataReq& msg)
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
	
		UInt32 ticketId  = root["ticketid"].asUInt();
	
		selfplayer->getConsortManager().getTicketOneData(ticketId);
	}
	else
	{
		LOG_INFO(" empty ticketJoin  req!    !");
	}
}

void ConsortMsgHandler::onEloquenceAward(const ConnId & connId, CGEloquenceAwardReq& msg)
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
		UInt32 ticketId  = root["win"].asUInt();
		selfplayer->getConsortManager().getEloquenceAward(ticketId);
	}
	else
	{
		LOG_INFO(" eloquencebegin  req!    !");
	}
}

void ConsortMsgHandler::onEloquenceBegin(const ConnId & connId, CGEloquenceBeginReq& msg)
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
		selfplayer->getConsortManager().beginEloquence();
	}
	else
	{
		LOG_INFO(" eloquencebegin  req!    !");
	}
	
	
}


void ConsortMsgHandler::onWoodCatDataReq(const ConnId & connId, CGWoodDataReq& msg)
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
		selfplayer->getConsortManager().getWoodData();
	}
	else
	{
		LOG_INFO(" woodcatdata  req!    !");
	}
}

void ConsortMsgHandler::onWoodCatBeginReq(const ConnId & connId, CGWoodCatBeginReq& msg)
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
		UInt32 woodIndex  = root["woodindex"].asUInt();
		selfplayer->getConsortManager().beginWoodCat(woodIndex);
	}
	else
	{
		LOG_INFO(" woodcatdata  req!    !");
	}
}

void ConsortMsgHandler::onWoodCatEndReq(const ConnId & connId, CGWoodCatEndReq& msg)
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
		UInt32 woodIndex  = root["woodindex"].asUInt();
		UInt32 res  = root["win"].asUInt();
		
		selfplayer->getConsortManager().endWoodCat(woodIndex, res);
	}
	else
	{
		LOG_INFO(" woodcatdata  req!    !");
	}
}

void ConsortMsgHandler::onWoodCatEnhanceReq(const ConnId & connId, CGWoodEnhanceReq& msg)
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
		UInt32 type  = root["type"].asUInt();
		
		selfplayer->getConsortManager().enhanceWood(type);
	}
	else
	{
		LOG_INFO(" woodcatdata  req!    !");
	}
}

void ConsortMsgHandler::onWoodCatTotalAwardReq(const ConnId & connId, CGWoodTotalAwardReq& msg)
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
		selfplayer->getConsortManager().woodTotalAward();
	}
	else
	{
		LOG_INFO(" woodcatdata  req!    !");
	}
}
