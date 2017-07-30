#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_ROBOT_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_ROBOT_MANAGER_H__



namespace Lynx
{


	class RobotManager: public Singleton<RobotManager>
	{
	
	public:
		bool initial();
		void insertRobotDetail();
	};



}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_ROBOT_MANAGER_H__
