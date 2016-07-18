#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EMAIL_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EMAIL_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class EmailManager : public MainThread,public Singleton<EmailManager>
	{
	public:
		EmailManager();
		virtual ~EmailManager();

		bool initial(Player* player);

		void release();

		void addEmail(const EmailData & emailData);

		void delEmail(UInt64 emailUid);

		void updateEmailState(const EmailData & emailData);

		void openEmail(UInt64 emailUid);

		void getContant(UInt64 emailUid);

		void getContant(List<UInt64> emailUid);

		void dealSubStr(std::string &contantStr, std::string ::size_type  findIndex,Map<UInt64, List<UInt64> > &mapRes2Sub, List<JewelryData *> &lsitJewelryData);

		void clearAllEmail();

		std::string convertDataToJson();

		std::string convertGetToJson(Map<UInt64, List<UInt64> > mapRes2Sub, List<JewelryData *> lsitJewelryData);

	private:
		

		Player* m_pPlayer;

		List<EmailData> *m_pEmailList;

		Map<UInt64, EmailData*> m_emailMap;

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_EMAIL_MAMAGER_H__