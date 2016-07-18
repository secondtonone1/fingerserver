#include "ServerData.h"
#include "PersistSystem.h"
using namespace Lynx;

ServerPublicData &ServerData::getServerPublicData()
{
	return mServerPublicData;
}
void ServerData::setServerPublicData(ServerPublicData &serverPublicData)
{
	mServerPublicData = serverPublicData;

	PersistServerDataReq req;
	req.gRechargeNum = serverPublicData.gRechargeNum;
	req.gServerRobotRefreshFlag = serverPublicData.gServerRobotRefreshFlag;
	req.gServerSpare2 = serverPublicData.gServerSpare2;
	req.gServerSpare3 = serverPublicData.gServerSpare3;
	req.gServerSpare4 = serverPublicData.gServerSpare4;
	req.gServerSpare5 = serverPublicData.gServerSpare5;	

	PersistSystem::getSingleton().postThreadMsg(req,0);

}