#ifndef __LYNX_SERVER_DATA_H__
#define __LYNX_SERVER_DATA_H__


#include "LogicSystem.h"



namespace Lynx
{
	struct ServerPublicData
	{
		ServerPublicData():gRechargeNum(0),gServerRobotRefreshFlag(0),gServerSpare2(0),gServerSpare3(0),gServerSpare4(0),gServerSpare5(0){}
		UInt32 gRechargeNum ;//充值人数
		UInt32 gServerRobotRefreshFlag ;//充值人数
		UInt32 gServerSpare2 ;//充值人数
		UInt32 gServerSpare3 ;//充值人数
		UInt32 gServerSpare4 ;//充值人数
		UInt32 gServerSpare5 ;//充值人数

		LYNX_S11N_6(ServerPublicData,UInt32,gRechargeNum,UInt32, gServerRobotRefreshFlag,UInt32, gServerSpare2,UInt32, gServerSpare3,UInt32, gServerSpare4,UInt32, gServerSpare5);
	};

    class ServerData : public MainThread, public Singleton<ServerData>
	{
	public:
	
		ServerPublicData &getServerPublicData();
		void setServerPublicData(ServerPublicData &serverPublicData);
	
	

	};
	static ServerPublicData mServerPublicData;

} // namespace Lynx

#endif // __LYNX_SERVER_DATA_H__

