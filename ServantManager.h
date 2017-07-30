#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SERVANT_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SERVANT_MAMAGER_H__


#include "MainThread.h"
#include "PlayerAttrData.h"
namespace Lynx
{
	class Player;

	class ServantManager : public MainThread,  public Singleton<ServantManager>
	{
	public:
		ServantManager();
		virtual ~ServantManager();

		bool initial(Player* player);
		void release();

		void servantSwitch(UInt64 servantId, UInt32 count);

		void servantSwitch(UInt64 servantId);

		ServantData* addServant(UInt64 servantId);

		ServantData * getServantById(UInt64 servantId);

		ServantData* addServantPiece(UInt64 servantId, UInt32 pieceCount);

		ServantData * GMsetServantPiece(UInt64 servantId, UInt32 pieceCount);

		ServantData* combineServant(UInt64 servantId);

		UInt32 addServantBattleStone(UInt32 count);

		UInt32 addServantFood1(UInt32 count);

		UInt32 addServantFood2(UInt32 count);

		UInt32 addServantFood3(UInt32 count);

		UInt32 addServantFood4(UInt32 count);
				
		UInt32 addServantFood5(UInt32 count);

		UInt32 getServantFood(UInt32 ServantType);

		void servantLvUp(UInt64 servantId, const List<UInt32>& foodList);

		std::string convertDataToJson();

		void checkActiveCondition();

		void checkBattleOpen(UInt32 &battleIndex, const Vector<UInt32> &bitVec, UInt32 & playerlv);

		bool getOpenState(UInt32 battlePos);

		bool getMainOpenState(UInt32 battlePos);

		bool getAssistOpenState(UInt32 battlePos);

		ServantData* servantStarUp(UInt64 servantId);

		void servantFloorUp(UInt64 servantId);

		ServantTreasure * getTreasure(UInt64 treasureId);

		ServantTreasure* addTreasure(UInt64 treasureId, UInt32 count);

		ServantTreasure * GMsetTreasure(UInt64 treasureId, UInt32 count);

		bool costTreasure(UInt64 treasureId, UInt32 count);

		void addBattleStone(UInt32 count);

		bool costBattleStone(UInt32 count);

		void addServantSwitch(UInt32 count);

		UInt32 getServantSwitch();

		bool costServantSwitch(UInt32 count);

		ServantTreasure* combineTreasure(UInt64 treasureId, UInt32 count);

		//自动合成目标宝物，只合成一个
		bool  autoCombineTreasure(UInt64 treasureId, List<UInt64 >& changeList);
		
		bool treasureEquip(UInt64 servantId, const Vector<UInt64>& equipVec);

		bool treasureEquipOnce(UInt64 servantId);

		//助阵位手动开启
		bool assistbattleOpen(UInt32 battleIndex);

		bool assistbattleEnhance(UInt32 battleIndex);

		bool servantBattleSet(const Vector<UInt64> &battleIndex);

        bool assistBattleOneSet(const Vector<UInt64> &totalBattle);

		//计算上阵英雄出厂实际概率
		void calculateServantRate();

		//计算总战力
		UInt32 calculateFightCapacity();
		
		
		//计算佣兵给角色提供的所有属性
		void calculteAttrAdd(PlayerAttrData & playerAttrData);

		void getServantListJson(List<ServantData> * m_pServantList,Json::Value &root);
        //GM命令
		void GMsetServantFood(UInt64 foodType, UInt64 foodCount);



 
		bool addServants(UInt32 subTypeID,UInt32 count);

		bool isServantExist(UInt32 subTypeID);

		UInt32 getServantPieceCount(UInt64 servantId);

		bool getServantDataJson(UInt32 subTypeID,Json::Value &jsonValue);

		void infolock(UInt64 servantid, UInt32 index);



	private:

		UInt32 calculateAssistCapacity();

		UInt32 calculateLuckCapacity();

		UInt32 calculateServantCapacity(ServantData * servantData);

		UInt32 calculateServantsCapacity();

		void calculateFinalRate();
		//更改阵容后计算缘分
		void calculateLuck();

		void judgeMainBattleLuck();

		void judgeAssistBattleLuck();

		
		void calculateServantsAddAttr(PlayerAttrData & playerAttrData);

		void calculateServantAddAttr(PlayerAttrData & playerAttrData,ServantData * servantData);

		bool luckServant(ServantLuckTemplate * servantLuckTemp, UInt64 baseServant);

		bool isActiveLuck(UInt64 servantId);

		void attrConverToCapacity(UInt32 & fightCapacity, UInt32 attrType, UInt32 attrValue);

		

		Player * m_pPlayer;

		ServantBattleData * m_pServantBattleData;
		List<ServantData> * m_pServantList;
		List<ServantTreasure> * m_pTreasureList;
		PlayerServantData * m_pServantData;
		Map<UInt64 ,ServantData*> m_mapIdServant;
		Map<UInt64, ServantTreasure *> m_mapIdTreasure;
		UInt64 m_nPlayerUid;
		//总大小为13，第一个元素为vec总大小
		Vector<UInt32> m_bitVec;
		Vector<UInt64> m_luckVec;
		//rateVec中的是百分比，比如15，表示15%
		Vector<UInt32> m_rateVec;
		Map<UInt32, double> m_attrPercentMap;


	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_SERVANT_MAMAGER_H__
