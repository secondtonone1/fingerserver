#ifndef __LYNX_COMMON_LIB_CONSORT_DATA_H__
#define __LYNX_COMMON_LIB_CONSORT_DATA_H__

#include "Definition.h"

//王戊辰
//公会基础数据
//2016 10 31


namespace Lynx
{

	//玩家申请的数据结构
	    struct ApplyData
		{
			ApplyData():m_nApply(0), m_nTime(0){}
			//玩家id
		   UInt64 m_nApply;
		   //玩家申请时间
		   UInt64 m_nTime;
		   LYNX_S11N_2(ApplyData, UInt64, m_nApply, UInt64, m_nTime);
		};

		struct LogData
		{
			LogData(): m_nConsortId(0), m_nTime(0), m_nTempId(0),
				m_strParam1(""), m_strParam2(""), m_nPlayerUid(0), m_strPlayername(""){}
		
			UInt64 m_nTime;
			UInt64 m_nTempId;
			UInt64 m_nConsortId;
			String m_strParam1;
			String m_strParam2;
			UInt64 m_nPlayerUid;
			String m_strPlayername;
			LYNX_S11N_7(LogData,  UInt64, m_nTime, UInt64 ,m_nTempId,
				String, m_strParam1, String ,m_strParam2, UInt64 ,m_nPlayerUid, String, m_strPlayername, UInt64, m_nConsortId);

		};

		struct TicketFriend
		{	
			TicketFriend():m_nPlayeruid(0), m_nServantid(0){}
			UInt64 m_nPlayeruid;
			UInt64 m_nServantid;
		    UInt32 m_nOrderIndex;
		   LYNX_S11N_3(TicketFriend, UInt64, m_nPlayeruid, UInt64, m_nServantid, UInt32 , m_nOrderIndex);
		};

		struct GivenData
		{
			GivenData():m_strName(""), m_nGrade(0){}
			String m_strName;
			UInt32 m_nGrade;
		  
		   LYNX_S11N_2(GivenData, String, m_strName, UInt32, m_nGrade);
		};

		 struct TicketData
		{
			TicketData():m_nTicketId(0),m_nQuality(0){}
			//票友问题id
		   UInt32 m_nTicketId;
		   List<TicketFriend> m_ticketFriends;
		   List<GivenData> m_givenList;
		   UInt32 m_nQuality;
		   LYNX_S11N_4(TicketData, UInt32, m_nTicketId,  List<TicketFriend>, m_ticketFriends,
			   UInt32, m_nQuality ,List<GivenData> ,m_givenList);
		};


	//军团信息，全服军团结构体
	struct ConsortData
	{

		ConsortData() : m_nId(0), m_nLv(0),  m_strName(""), m_nCount(0), m_nPower(0), 
		m_strDesc(""), m_nRes(0), m_nLeader(0),m_nExp(0), m_nCheck(0), m_nPowerLimit(0), m_nSignLv(0), m_nActive(0),m_nBusinessLv(0),
		m_nWoodBloodLeft(0), m_nEnhance1(0), m_nEnhance2(0), m_nEnhance3(0){}

		//consortid
		Guid m_nId;
		//consortlv
		UInt32 m_nLv;
		//consortname
		String m_strName;
		//count
		UInt32 m_nCount;
		//power
		UInt64 m_nPower;
        //公会宣言
		String m_strDesc;
		//保留字段，公会资源，目前没用
		UInt32 m_nRes;
		//公会的会长
		UInt64  m_nLeader;
		//公会的经验
		UInt32 m_nExp;
		//公会审核标记
		UInt32 m_nCheck;
		//公会战力限制
		UInt32 m_nPowerLimit;
		
		//玩家申请列表
		List<ApplyData> m_applyList;
		//日志列表
		List<LogData> m_logList;
        //成员列表
		List<UInt64> m_nMemberList;


		//公会当天等级,防止升级影响签到逻辑
		UInt32 m_nSignLv;
		//公会活跃度
		UInt32 m_nActive;
		//公会行商猫半天等级
		UInt32 m_nBusinessLv;
		//公会票友集会
		Map<UInt32, TicketData*> m_ticketDataMap;
		List<TicketData> m_ticketList;

		//当前玩家攻击造成怪物损失的总血量

		UInt32 m_nWoodBloodLeft;
		//三种强化标准
		UInt32 m_nEnhance1;
		UInt32 m_nEnhance2;
		UInt32 m_nEnhance3;

		List<UInt64> m_listWoodPlayers1;
		List<UInt64> m_listWoodPlayers2;
		List<UInt64> m_listWoodPlayers3;
		List<UInt64> m_listWoodPlayers4;
		List<UInt64> m_listWoodPlayers5;


		LYNX_S11N_27(ConsortData, Guid ,m_nId, UInt32, m_nLv, String, m_strName, UInt32, m_nCount,
					UInt64, m_nPower,  String , m_strDesc,  UInt32,  m_nRes, UInt64, m_nLeader,
					List<ApplyData>, m_applyList, List<LogData>, m_logList, List<UInt64>, m_nMemberList,
					UInt32, m_nExp,UInt32, m_nCheck, UInt32 ,m_nPowerLimit,UInt32, m_nSignLv, UInt32 ,m_nActive,
					UInt32, m_nBusinessLv, List<TicketData>, m_ticketList,UInt32, m_nWoodBloodLeft,UInt32 ,m_nEnhance1,
					UInt32, m_nEnhance2, UInt32, m_nEnhance3, List<UInt64> ,m_listWoodPlayers1, List<UInt64>, m_listWoodPlayers2,
					List<UInt64>, m_listWoodPlayers3,List<UInt64>, m_listWoodPlayers4,List<UInt64>, m_listWoodPlayers5
			);


	};

	//军团概要同步信息
	struct ConsortRankData
	{
		ConsortRankData():m_nId(0), m_nPower(0), m_nRank(0){}
		//consortid
		Guid m_nId;
		//power
		UInt64 m_nPower;
		//rank
		UInt32 m_nRank;

		LYNX_S11N_3(ConsortRankData, Guid ,m_nId, UInt64, m_nPower,
				UInt32, m_nRank
			);
	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_CONSORT_DATA_H__