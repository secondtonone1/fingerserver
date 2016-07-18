#ifndef __LYNX_COMMON_LIB_PLAYER_DATA_2_CLIENT_H__
#define __LYNX_COMMON_LIB_PLAYER_DATA_2_CLIENT_H__

#include "Definition.h"
#include "ItemData.h"
#include "HeroData.h"
#include "PetData.h"
#include "TollgateData.h"
#include "SecretStoreData.h"
#include "EmailData.h"
#include "RankData.h"
#include "BuddyData.h"
#include "AchievementData.h"
#include "TowerData.h"
#include "SkillData2Client.h"
#include "BufferData.h"
#include "BufferData2Client.h"
#include "ItemData2Client.h"

namespace Lynx
{
	struct PlayerBaseData2Client
	{
	
		PlayerBaseData2Client() : m_nPlayerID(0), m_nModelID(0),m_strPlayerName(""),
			m_nLevel(0), m_nLevelExp(0),
			m_nVipLevel(0), m_nVipExp(0), m_nRMB(0), m_nGold(0),m_nStrength(0),m_strMood(""){}

		//玩家角色ID 
		Guid m_nPlayerID;

		//模型ID
		UInt32 m_nModelID;

		//玩家姓名
		std::string  m_strPlayerName;

		//玩家等级
		UInt32 m_nLevel;

		//玩家等级经验
		Guid m_nLevelExp;

		//玩家vip等级
		UInt32 m_nVipLevel;

		//玩家vip经验
		Guid	m_nVipExp;

		//玩家RMB个数
		Guid	m_nRMB;

		//玩家金币
		Guid	m_nGold;

		UInt32  m_nStrength;

		std::string m_strMood;

		LYNX_S11N_11(PlayerBaseData2Client, Guid, m_nPlayerID, UInt32 ,m_nModelID,
			std::string ,m_strPlayerName, UInt32,m_nLevel, 
			Guid, m_nLevelExp, UInt32, m_nVipLevel, Guid,	m_nVipExp,
			Guid,m_nRMB,  Guid,m_nGold, UInt32,  m_nStrength,std::string ,m_strMood);

	};

	
	

	struct PlayerSkillListData2Client
	{
		PlayerSkillListData2Client(){


		}
	
		std::string constructString(int num = 0)
		{
			m_strSendMsg.clear();
			for (List<SkillItem2Client>::Iter* it = m_listSkillItems.begin();
					            it != NULL; it = m_listSkillItems.next(it))
			{
				if(m_listSkillItems.size() == 1)
				{
					m_strSendMsg += it->mValue.constructString(num);
					return m_strSendMsg;
				}

				if(m_listSkillItems.next(it)!=NULL)
				{
					m_strSendMsg += (it->mValue.constructString(num+1) + connectSymbols[num]);
				}
				else
				{
					m_strSendMsg += it->mValue.constructString(num+1);
				}
			}
		  return m_strSendMsg;
		}
		
		List<SkillItem2Client> m_listSkillItems;
		std::string m_strSendMsg;
		LYNX_S11N_1(PlayerSkillListData2Client,std::string ,m_strSendMsg);
	};


	struct PlayerBuffListData2Client
	{
		PlayerBuffListData2Client(){}

		List<BufferData2Client> m_listBuffers;

		std::string constructString(int num = 0)
		{
			m_strSendMsg.clear();
			for(List<BufferData2Client>::Iter *it = m_listBuffers.begin(); it != NULL; it = m_listBuffers.next(it))
			{
				if(m_listBuffers.size() == 1)
				{
					m_strSendMsg += it->mValue.constructString(num);
				}

				if(m_listBuffers.next(it) != NULL)
				{
					m_strSendMsg += (it->mValue.constructString(num+1) + connectSymbols[num]);
				}
				else
				{
					m_strSendMsg += it->mValue.constructString(num+1);
				}
			}

			return m_strSendMsg;
		}

		std::string m_strSendMsg;
		LYNX_S11N_1(PlayerBuffListData2Client,std::string, m_strSendMsg);

	};

	
	struct PlayerFashionListS2C
	{
		PlayerFashionListS2C():m_strSendMsg(""),m_nEquipFashionID(0){}
		UInt64 m_nEquipFashionID;
		//玩家的时装列表，获得与没获得的，都加入list，没获得的时间为0，获得的时间为获取时间戳
		List<Guid> m_listFashionGetTimes;
		
		std::string constructString(int num = 0)
		{
			m_strSendMsg.clear();
			std::stringstream mystream;
			

			for(List<Guid>::Iter *it = m_listFashionGetTimes.begin(); it != NULL; it = m_listFashionGetTimes.next(it))
			{
				mystream.clear();
				mystream.str("");
				mystream << it->mValue;
				std::string timeStr;
				mystream >> timeStr;
			

				if(m_listFashionGetTimes.next(it) != NULL)
				{
					m_strSendMsg += timeStr + connectSymbols[num];
				}
				else
				{
					m_strSendMsg += timeStr;
				}
				
			}

			return m_strSendMsg;
		}

		std::string m_strSendMsg; 

		LYNX_S11N_1(PlayerFashionListS2C,std::string, m_strSendMsg);
	};

	struct PlayerData2Client 
	{
		PlayerData2Client(){}
		
		PlayerBaseData2Client mBaseData;
		PlayerSkillListData2Client mSkillListData;	
		PlayerBuffListData2Client	  mBuffListData;
		PlayerFashionListS2C		mFashionListData;
		PlayerItemListS2C			mItemListData;   

		LYNX_S11N_5(PlayerData2Client,PlayerBaseData2Client,mBaseData,  
			PlayerSkillListData2Client,mSkillListData,
			 PlayerBuffListData2Client , mBuffListData,
			 PlayerFashionListS2C, mFashionListData,
			 PlayerItemListS2C, mItemListData);

	};




} // namespace Lynx

#endif // __LYNX_COMMON_LIB_PLAYER_DATA_2_CLIENT_H__