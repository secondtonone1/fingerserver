#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_RANKINGPERSONAL_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_RANKINGPERSONAL_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"

namespace Lynx
{

	class Player;


	
	class RankingPersonalManager :public MainThread , public Singleton<RankingPersonalManager>
	{
	
	public:
	
		RankingPersonalManager();
		virtual~RankingPersonalManager();

		UInt32 initial(Player* player);

		void release(); 		


		UInt32 setRecord(UInt32 type,UInt32 rank, Record record, bool needSave);//插入更高纪录


		MapRecord getRecords(UInt32 type);

		void setRecords(UInt32 type,MapRecord mapRecord);

		TypeRecords  *getTypeRecords();

		void setTypeRecords(TypeRecords val);

		Record getRecord(UInt32 type,UInt32 rank);

	private:
	
		Player* mPlayer;


	
		TypeRecords *mTypeRecords;
		
		
	};


}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_RANKINGPERSONAL_MANAGER_H__
