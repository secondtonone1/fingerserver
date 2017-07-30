#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_DETAILINFO_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_DETAILINFO_MANAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class DetailInfoManager : public MainThread ,public Singleton<DetailInfoManager>
	{
	public:
		DetailInfoManager();
		virtual ~DetailInfoManager();

		bool initial(Player* player);
		void release();
		void setDetailInfo(std::string detailInfo);
		std::string getDetailInfoSelf(); 
		std::string getDetailInfo(std::string playerUidStr); 
	private:
	  Player * m_pPlayer;
	  std::string m_playerIdStr;
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_DETAILINFO_MANAGER_H__