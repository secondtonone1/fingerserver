#include "PVPMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"
#include "./PKSystem/PVPSystemWC.h"

using namespace Lynx;

void 
PVPMsgHandler::onPVPReq(const ConnId& connId, CGPVPReq& msg)
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
		UInt32 type = root["packetType"].asUInt();
		UInt64 uuid = root["uuid"].asUInt64();
		std::string battleData = root["data"].asString();
		
		switch(type)
		{
		case 1:
			PVPSystemWC::getSingleton().addWaitForPVP(uuid, battleData.c_str());
			break;
		case 3:
			PVPSystemWC::getSingleton().cancelWaitForPVP(uuid);
			break;
		case 6:
			PVPSystemWC::getSingleton().readyForPVP(uuid);
			break;
		case 7:
			PVPSystemWC::getSingleton().cancelPVPIng(uuid);
			break;
		case 101:
			PVPSystemWC::getSingleton().sendMsgToOther(uuid, battleData);
			break;
		default:

			break;
		
		}

	}
	else
	{
		LOG_INFO("empty message gemCombineReq!");
	}


}