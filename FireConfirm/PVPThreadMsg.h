#ifndef __LYNX_GAME_SERVER_PVP_THREAD_MSG_H__
#define __LYNX_GAME_SERVER_PVP_THREAD_MSG_H__

#include "../CommonLib/CommonLib.h"
#include "Character.h"
#include<cstring>
namespace Lynx
{
	enum
	{
		PVP_CREATE_ROOM_REQ                  = sPVPMsgBase + 0,
		PVP_CREATE_ROOM_RESP				 = sPVPMsgBase + 1,

		PVP_JOIN_ROOM_REQ					 = sPVPMsgBase + 2,
		PVP_JOIN_ROOM_RESP					 = sPVPMsgBase + 3,

		PVP_LEAVE_ROOM_REQ					 = sPVPMsgBase + 4,
		PVP_LEAVE_ROOM_RESP					 = sPVPMsgBase + 5,

		PVP_START_REQ						 = sPVPMsgBase + 6,
		PVP_START_RESP						 = sPVPMsgBase + 7,

		PVP_ROOM_REQ						 = sPVPMsgBase + 10,//pvp房间请求
		PVP_ROOM_RESP						 = sPVPMsgBase + 11,//pvp房间回复


		PVP_BATTLE_REQ						 = sPVPMsgBase + 12,//pvp请求
		PVP_BATTLE_RESP						 = sPVPMsgBase + 13,//pvp请求回复

		PVP_SYNC_RESP						 = sPVPMsgBase + 15,//pvp数据同步

	};

	enum 
	{
		PVPSYSTEM_PVP					 = 309, //PVP请求

	};


	struct PVPTreadReq
	{
		UInt32 typeID;//0创建房间，1加入房间，2开始PVP，3离开房间，4改变房间状态5获取房间列表，6左右移动 ，7技能，8离开关卡，9其他
		UInt32 value;
		UInt64 connId;
	
		PVPTreadReq():typeID(0),value(0),connId(0){}
		
		LYNX_MESSAGE_3(PVPSYSTEM_PVP, PVPTreadReq,  UInt32, typeID,UInt32, value,UInt64, connId);

	};

	struct RoomPlayer
	{
		Guid playerID;//玩家ID
		std::string name;//名称
		UInt32 level;//
		UInt32 vipLevel;//
		std::string title;//称号		
		UInt32 camp;//阵营
		UInt32 state;//0未准备 1准备
		UInt32 modID;//模型
		UInt32 combatCapability;//战力
		UInt32 delay;//延迟（3种）
		UInt32 score;//个人积分		
		List<Goods> equipList;

	};

	struct RoomData
	{
		UInt32 roomID;//房间号
		UInt32 stageID;	//关卡ID
		UInt32 sceneID;//场景ID	
		UInt32 roomState;//0准备 1 已开始
		UInt32 multiple;//倍数
		List<RoomPlayer> playerList;//玩家信息
	};
	struct PVPRoomReq
	{
		PVPRoomReq() : typeID(0),stageID(0),value(0) {}

		UInt32 typeID;//1.获取房间列表2.创建房间3.加入房间4.开始PVP 5.离开房间6.改变房间状态7.排队

		UInt32 stageID;
		UInt32 value;//倍数

		std::string jsonStr;
		
		LYNX_MESSAGE_1(PVP_ROOM_REQ, PVPRoomReq,std::string, jsonStr);

		
		void convertJsonToData(std::string jsonStr)
		{
			
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				typeID = root["typeID"].asUInt();
				stageID = root["stageID"].asUInt();
				value = root["value"].asUInt();	
			}
		}
	};
	struct PVPRoomResp
	{
		PVPRoomResp() : typeID(0),value(0),result(0),otherPlayerID(0),isRobot(0),otherPlayer("") {}

		UInt32 typeID;//1.获取房间列表2.创建房间3.加入房间4.开始PVP5.离开房间6.改变房间状态7.获取房间列表8.排队
		UInt32 value;
		UInt32 result;
		Guid otherPlayerID;
		UInt32 isRobot;
		std::string otherPlayer;
		List<RoomData> roomDatas;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["typeID"] = Json::Value(typeID);
			root["value"] = Json::Value(value);
			root["result"] = Json::Value(result);
			root["isRobot"] = Json::Value(isRobot);
			root["otherPlayerID"] = Json::Value(otherPlayerID);
			for (List<RoomData>::Iter *iter = roomDatas.begin();iter!= NULL;iter = roomDatas.next(iter))
			{
				Json::Value son;

				son["roomID"] = Json::Value(iter->mValue.roomID);
				son["sceneID"] = Json::Value(iter->mValue.sceneID);
				son["multiple"] = Json::Value(iter->mValue.multiple);
				son["stageID"] = Json::Value(iter->mValue.stageID);
				son["roomState"] = Json::Value(iter->mValue.roomState);
				son["otherPlayer"] = Json::Value(otherPlayer);
				for(List<RoomPlayer>::Iter *it = iter->mValue.playerList.begin();it!= NULL;it = iter->mValue.playerList.next(it))
				{

					Json::Value node;
					node["playerID"] = Json::Value(it->mValue.playerID);
					node["name"] = Json::Value(it->mValue.name);
					node["level"] = Json::Value(it->mValue.level);
					node["vipLevel"] = Json::Value(it->mValue.vipLevel);
					node["title"] = Json::Value(it->mValue.title);
					node["camp"] = Json::Value(it->mValue.camp);
					node["state"] = Json::Value(it->mValue.state);
					node["modID"] = Json::Value(it->mValue.modID);
					node["combatCapability"] = Json::Value(it->mValue.combatCapability);
					node["delay"] = Json::Value(it->mValue.delay);
					node["score"] = Json::Value(it->mValue.score);

					for(List<Goods>::Iter * it1 =it->mValue.equipList.begin();it1!=NULL;it1 = it->mValue.equipList.next(it1))
					{
						Json::Value leaf;

						leaf["resourcestype"] = Json::Value(it1->mValue.resourcestype);
						leaf["subtype"] = Json::Value(it1->mValue.subtype);
						leaf["num"] = Json::Value(it1->mValue.num);
						node["equipList"].append(leaf);
					}
					son["roomData"].append(node);
				}

				root["roomDatas"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct PVPBattleReq
	{
		PVPBattleReq() : typeID(0),value(0) {}

		UInt32 typeID;//1.左移动2.右移动3.技能4.离开关卡5.切换技能列表6.复活

		UInt32 value;

		std::string jsonStr;
		LYNX_MESSAGE_1(PVP_BATTLE_REQ, PVPBattleReq,std::string, jsonStr);

		
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				typeID = root["typeID"].asUInt();
				value = root["value"].asUInt();	
			}
		}
	};
	struct PVPBattleResp
	{
		PVPBattleResp() : typeID(0),value(0),result(0) {}

		UInt32 typeID;//0创建房间，1加入房间，3离开房间，4改变房间状态5获取房间列表，8离开关卡，9其他  6左右移动 ，7技能，2开始PVP，
		UInt32 value;
		UInt32 result;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["typeID"] = Json::Value(typeID);
			root["value"] = Json::Value(value);
			root["result"] = Json::Value(result);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	struct CharacterSyncData
	{
		CharacterSyncData(): hp(0),mp(0),sp(0),hitCnt(0),attr_buffEffect(0),jump_speed(0),camp(0),keepFallCnt(0),currentSkillId(0),currentMultiIdx(0),radio(0),nowSpeed(0),moveStartTime(0),deadTimes(0){}

		Guid playerID;
		UInt32 level;
		UInt32 hp;
		UInt32 mp;
		UInt32 sp;
		UInt32 attr_skillPassive;//客户端弃用
		UInt32 attr_buffEffect;

		LastP lastP;

		//action state
		UInt32 eNowState;
		UInt32 eLastState;

		Pos pos;
		Pos lastPos;
		Int32 nowSpeed;
		UInt64 moveStartTime;
		UInt64 jumpStartTime;
		//area
		Area attackArea;	
		HurtArea hurtArea;

		UInt32 hitCnt;

		//jump status
		bool flag_fall;
		bool flag_skillMoveY;
		bool flag_onGround;
		bool flag_haveAttachMaxY;
		Int32 jump_speed;
		bool bGravity;
		bool flag_canBeAttack;
		UInt32 flying_state;
		UInt32 keepFallCnt;

		//阵营
		UInt32 camp;

		UInt32 whichOneInBg;

		//res
		// 		ResID resID;

		//异常状态
		bool flag_buffState[32];

		//self.targets = {}
		//Targets targets;
		UInt32 currentSkillId;
		UInt32 currentMultiIdx;
		bool onAttack;

		UInt32 radio;

		UInt32 killEnerNum;//add
		UInt32 joystick_updown;      
		// 			self.skillTab = "current"       -- "current", "back"
		UInt32 hit;
		bool alertLowHp;
		bool effectLowHp ;
		bool isOnQte;
		UInt32 explodeNum;
		UInt32 deadTimes ;
		bool faceR;
		bool allowTurnFaceCommand;
		bool haveRecvTurnFaceCommand;

		bool canRealMove;//from scene
		bool canSkillControl;
		bool flagAllowInterrupt;

		//bullet
		bool bulletStop;
		bool bulletDirection;
		UInt64 bulletStartTime;
		Pos bulletLastPos;
		Pos bulletPos;		
		Pos bulletSpeed;

		UInt64 skillBeginTime;

		HeroTemplate *heroTemplate;//attr_base
		LevelGrowTemplate *levelGrowTemplate;//attr_level

		SkillMap skillBaseData;
		PlayerSkillListData  skillData;

		BuffMap buffData;

	};

	struct PVPSyncResp
	{
		PVPSyncResp() :typeID(0),value(0),result(0) {}
		UInt32 typeID;//0 状态 1技能 2 buff 3 jump 4 fall 5 伤害 5 死亡 6 胜利 7进入场景
		UInt32 value;
		UInt32 result;
		List<CharacterData> charDatas;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["typeID"] = Json::Value(typeID);
			root["value"] = Json::Value(value);		
			root["result"] = Json::Value(result);

			for(List<CharacterData>::Iter *iter = charDatas.begin();iter!= NULL;iter = charDatas.next(iter))
			{
				Json::Value son;

				if (iter->mValue.playerID != 0)
				{
					son["playerID"] = Json::Value(iter->mValue.playerID);
				}
				if (iter->mValue.level != 0)
				{
					son["level"] = Json::Value(iter->mValue.level);
				}
				if (iter->mValue.hp != 0)
				{
					son["hp"] = Json::Value(iter->mValue.hp);
				}
				if (iter->mValue.mp != -1)
				{
					son["mp"] = Json::Value(iter->mValue.mp);
				}
				if (iter->mValue.sp != 0)
				{
					son["sp"] = Json::Value(iter->mValue.sp);
				}				

				if (iter->mValue.attr_skillPassive != 0)
				{
					son["attr_skillPassive"] = Json::Value(iter->mValue.attr_skillPassive);
				}
				if (iter->mValue.attr_buffEffect != 0)
				{
					son["attr_buffEffect"] = Json::Value(iter->mValue.attr_buffEffect);
				}
				if (iter->mValue.lastP.maxhp != 0 ||iter->mValue.lastP.maxmp != 0 ||iter->mValue.lastP.maxsp != 0 ||iter->mValue.lastP.hp != 0 ||iter->mValue.lastP.mp != 0 ||iter->mValue.lastP.sp != 0 )
				{
					son["lastP"].append(iter->mValue.lastP.maxhp);
					son["lastP"].append(iter->mValue.lastP.maxmp);
					son["lastP"].append(iter->mValue.lastP.maxsp);
					son["lastP"].append(iter->mValue.lastP.hp);
					son["lastP"].append(iter->mValue.lastP.mp);
					son["lastP"].append(iter->mValue.lastP.sp);
				}
				if (iter->mValue.eNowState != 0)
				{
					son["eNowState"] = Json::Value(iter->mValue.eNowState);
				}
				if (iter->mValue.eLastState != 0)
				{
					son["eLastState"] = Json::Value(iter->mValue.eLastState);
				}
				if (iter->mValue.pos.x != 0 ||iter->mValue.pos.y != 0)
				{
					son["pos"].append(iter->mValue.pos.x);
					son["pos"].append(iter->mValue.pos.y);
				}
				if (iter->mValue.lastPos.x != 0 ||iter->mValue.lastPos.y != 0)
				{
					son["lastPos"].append(iter->mValue.lastPos.x);
					son["lastPos"].append(iter->mValue.lastPos.y);
				}
				if (iter->mValue.nowSpeed != 0)
				{
					son["nowSpeed"] = Json::Value(iter->mValue.nowSpeed);
				}
				if (iter->mValue.moveStartTime != 0)
				{
					son["moveStartTime"] = Json::Value(iter->mValue.moveStartTime);
				}
				if (iter->mValue.moveStartTime != 0)
				{
					son["moveStartTime"] = Json::Value(iter->mValue.moveStartTime);
				}
				if (iter->mValue.moveStartTime != 0)
				{
					son["moveStartTime"] = Json::Value(iter->mValue.moveStartTime);
				}
				if (iter->mValue.jumpStartTime != 0)
				{
					son["jumpStartTime"] = Json::Value(iter->mValue.jumpStartTime);
				}
				if (iter->mValue.attackArea.x != 0 ||iter->mValue.attackArea.y != 0 ||iter->mValue.attackArea.w != 0 ||iter->mValue.attackArea.h != 0)
				{
					son["attackArea"].append(iter->mValue.attackArea.x);
					son["attackArea"].append(iter->mValue.attackArea.y);
					son["attackArea"].append(iter->mValue.attackArea.w);
					son["attackArea"].append(iter->mValue.attackArea.h);
				}
				if (iter->mValue.hurtArea.stand.x != 0 ||iter->mValue.hurtArea.fall.x != 0 )//可能有漏洞
				{
					Json::Value leaf;
					leaf["stand"].append(iter->mValue.hurtArea.stand.x);
					leaf["stand"].append(iter->mValue.hurtArea.stand.y);
					leaf["stand"].append(iter->mValue.hurtArea.stand.w);
					leaf["stand"].append(iter->mValue.hurtArea.stand.h);
					leaf["fall"].append(iter->mValue.hurtArea.fall.x);
					leaf["fall"].append(iter->mValue.hurtArea.fall.y);
					leaf["fall"].append(iter->mValue.hurtArea.fall.w);
					leaf["fall"].append(iter->mValue.hurtArea.fall.h);
					son["hurtArea"] = Json::Value(leaf);
				}
				if (iter->mValue.hitCnt != 0)
				{
					son["hitCnt"] = Json::Value(iter->mValue.hitCnt);
				}
				if (iter->mValue.flag_fall != false)
				{
					son["flag_fall"] = Json::Value(iter->mValue.flag_fall);
				}
				if (iter->mValue.flag_skillMoveY != false)
				{
					son["flag_skillMoveY"] = Json::Value(iter->mValue.flag_skillMoveY);
				}
				if (iter->mValue.flag_onGround != false)
				{
					son["flag_onGround"] = Json::Value(iter->mValue.flag_onGround);
				}
				if (iter->mValue.flag_skillMoveY != false)
				{
					son["flag_skillMoveY"] = Json::Value(iter->mValue.flag_skillMoveY);
				}
				if (iter->mValue.flag_haveAttachMaxY != false)
				{
					son["flag_haveAttachMaxY"] = Json::Value(iter->mValue.flag_haveAttachMaxY);
				}

				if (iter->mValue.flag_skillMoveY != false)
				{
					son["flag_skillMoveY"] = Json::Value(iter->mValue.flag_skillMoveY);
				}
				if (iter->mValue.level != 0)
				{
					son["level"] = Json::Value(iter->mValue.level);
				}

				if (iter->mValue.flag_skillMoveY != false)
				{
					son["flag_skillMoveY"] = Json::Value(iter->mValue.flag_skillMoveY);
				}
				if (iter->mValue.jump_speed != 0)
				{
					son["jump_speed"] = Json::Value(iter->mValue.jump_speed);
				}
				if (iter->mValue.bGravity != false)
				{
					son["bGravity"] = Json::Value(iter->mValue.bGravity);
				}
				if (iter->mValue.flag_onGround != false)
				{
					son["flag_onGround"] = Json::Value(iter->mValue.flag_onGround);
				}
				if (iter->mValue.flag_haveAttachMaxY != false)
				{
					son["flag_haveAttachMaxY"] = Json::Value(iter->mValue.flag_haveAttachMaxY);
				}
				if (iter->mValue.jump_speed != 0)
				{
					son["jump_speed"] = Json::Value(iter->mValue.jump_speed);
				}
				if (iter->mValue.bGravity != false)
				{
					son["bGravity"] = Json::Value(iter->mValue.bGravity);
				}
				if (iter->mValue.flag_canBeAttack != false)
				{
					son["flag_canBeAttack"] = Json::Value(iter->mValue.flag_canBeAttack);
				}
				if (iter->mValue.flying_state != 0)
				{
					son["flying_state"] = Json::Value(iter->mValue.flying_state);
				}
				if (iter->mValue.keepFallCnt != 0)
				{
					son["keepFallCnt"] = Json::Value(iter->mValue.keepFallCnt);
				}
				if (iter->mValue.camp != 0)
				{
					son["camp"] = Json::Value(iter->mValue.camp);
				}
				if (iter->mValue.whichOneInBg != 0)
				{
					son["whichOneInBg"] = Json::Value(iter->mValue.whichOneInBg);
				}
				if (iter->mValue.currentSkillId != 0)
				{
					son["currentSkillId"] = Json::Value(iter->mValue.currentSkillId);
				}
				if (iter->mValue.currentMultiIdx != 0)
				{
					son["currentMultiIdx"] = Json::Value(iter->mValue.currentMultiIdx);
				}
				if (iter->mValue.onAttack != false)
				{
					son["onAttack"] = Json::Value(iter->mValue.onAttack);
				}
				if (iter->mValue.radio != 0)
				{
					son["radio"] = Json::Value(iter->mValue.radio);
				}
				if (iter->mValue.killEnerNum != 0)
				{
					son["killEnerNum"] = Json::Value(iter->mValue.killEnerNum);
				}
				if (iter->mValue.killEnerNum != 0)
				{
					son["killEnerNum"] = Json::Value(iter->mValue.killEnerNum);
				}
				if (iter->mValue.joystick_updown != 0)
				{
					son["joystick_updown"] = Json::Value(iter->mValue.joystick_updown);
				}
				if (iter->mValue.hit != 0)
				{
					son["hit"] = Json::Value(iter->mValue.hit);
				}
				if (iter->mValue.alertLowHp != false)
				{
					son["alertLowHp"] = Json::Value(iter->mValue.alertLowHp);
				}
				if (iter->mValue.effectLowHp != false)
				{
					son["effectLowHp"] = Json::Value(iter->mValue.effectLowHp);
				}
				if (iter->mValue.isOnQte != false)
				{
					son["isOnQte"] = Json::Value(iter->mValue.isOnQte);
				}
				if (iter->mValue.explodeNum != 0)
				{
					son["explodeNum"] = Json::Value(iter->mValue.explodeNum);
				}
				if (iter->mValue.deadTimes != 0)
				{
					son["deadTimes"] = Json::Value(iter->mValue.deadTimes);
				}
				if (iter->mValue.faceR != false)
				{
					son["faceR"] = Json::Value(iter->mValue.faceR);
				}
				if (iter->mValue.allowTurnFaceCommand != false)
				{
					son["allowTurnFaceCommand"] = Json::Value(iter->mValue.allowTurnFaceCommand);
				}
				if (iter->mValue.haveRecvTurnFaceCommand != 0)
				{
					son["haveRecvTurnFaceCommand"] = Json::Value(iter->mValue.haveRecvTurnFaceCommand);
				}
				if (iter->mValue.canRealMove != false)
				{
					son["canRealMove"] = Json::Value(iter->mValue.canRealMove);
				}
				if (iter->mValue.canSkillControl != 0)
				{
					son["canSkillControl"] = Json::Value(iter->mValue.canSkillControl);
				}
				if (iter->mValue.flagAllowInterrupt != 0)
				{
					son["flagAllowInterrupt"] = Json::Value(iter->mValue.flagAllowInterrupt);
				}
				if (iter->mValue.bulletStop != 0)
				{
					son["bulletStop"] = Json::Value(iter->mValue.bulletStop);
				}
				if (iter->mValue.bulletDirection != 0)
				{
					son["bulletDirection"] = Json::Value(iter->mValue.bulletDirection);
				}
				if (iter->mValue.bulletStartTime != 0)
				{
					son["bulletStartTime"] = Json::Value(iter->mValue.bulletStartTime);
				}
				if (iter->mValue.bulletStartTime != 0)
				{
					son["bulletStartTime"] = Json::Value(iter->mValue.bulletStartTime);
				}
				if (iter->mValue.skillBeginTime != 0)
				{
					son["skillBeginTime"] = Json::Value(iter->mValue.skillBeginTime);
				}
				if (iter->mValue.bulletLastPos.x != 0 ||iter->mValue.bulletLastPos.y != 0)
				{
					son["bulletLastPos"].append(iter->mValue.bulletLastPos.x);
					son["bulletLastPos"].append(iter->mValue.bulletLastPos.y);
				}
				if (iter->mValue.bulletPos.x != 0 ||iter->mValue.bulletPos.y != 0)
				{
					son["bulletPos"].append(iter->mValue.bulletPos.x);
					son["bulletPos"].append(iter->mValue.bulletPos.y);
				}
				if (iter->mValue.bulletSpeed.x != 0 ||iter->mValue.bulletSpeed.y != 0)
				{
					son["bulletSpeed"].append(iter->mValue.bulletSpeed.x);
					son["bulletSpeed"].append(iter->mValue.bulletSpeed.y);
				}
				if (iter->mValue.flag_buffState[0] != true)
				{
					for(UInt32 i = 0;i<BUFF_STATE_SUB_TYPE_MAX;i++)
					{
						son["flag_buffState"].append(iter->mValue.flag_buffState[i]);
					}
				}
// 				if (iter->mValue.heroTemplate !=NULL)				
				{
					Json::Value leaf;
					leaf["mName"] = Json::Value(iter->mValue.heroTemplate.mName.c_str());
					leaf["mDesc"] = Json::Value(iter->mValue.heroTemplate.mDesc.c_str());
					leaf["mRes"] = Json::Value(iter->mValue.heroTemplate.mRes.c_str());
					leaf["mStrSkillList"] = Json::Value(iter->mValue.heroTemplate.mStrSkillList.c_str());
					leaf["mStrSkillListOnCreate"] = Json::Value(iter->mValue.heroTemplate.mStrSkillListOnCreate.c_str());
					leaf["mPA"] = Json::Value(iter->mValue.heroTemplate.mPA);
					leaf["mPF"] = Json::Value(iter->mValue.heroTemplate.mPF);
					leaf["mMA"] = Json::Value(iter->mValue.heroTemplate.mMA);
					leaf["mMF"] = Json::Value(iter->mValue.heroTemplate.mMF);
					leaf["mMaxHP"] = Json::Value(iter->mValue.heroTemplate.mMaxHP);
					leaf["mMaxMP"] = Json::Value(iter->mValue.heroTemplate.mMaxMP);
					leaf["mMaxSP"] = Json::Value(iter->mValue.heroTemplate.mMaxSP);
					leaf["mMPRecover"] = Json::Value(iter->mValue.heroTemplate.mMPRecover);
					leaf["mSPRecover"] = Json::Value(iter->mValue.heroTemplate.mSPRecover);
					leaf["mAttackSpeed"] = Json::Value(iter->mValue.heroTemplate.mAttackSpeed);
					leaf["mCombinationAtt"] = Json::Value(iter->mValue.heroTemplate.mCombinationAtt);
					leaf["mMoveSpeed"] = Json::Value(iter->mValue.heroTemplate.mMoveSpeed);
					leaf["mJumpHeight"] = Json::Value(iter->mValue.heroTemplate.mJumpHeight);
					leaf["mFlyTimes"] = Json::Value(iter->mValue.heroTemplate.mFlyTimes);
					leaf["mSPDamage"] = Json::Value(iter->mValue.heroTemplate.mSPDamage);
					leaf["mAttPush"] = Json::Value(iter->mValue.heroTemplate.mAttPush);
					leaf["mAttPushDef"] = Json::Value(iter->mValue.heroTemplate.mAttPushDef);
					leaf["mAttRaise"] = Json::Value(iter->mValue.heroTemplate.mAttRaise);
					leaf["mAttRaiseDef"] = Json::Value(iter->mValue.heroTemplate.mAttRaiseDef);
					leaf["mStiff"] = Json::Value(iter->mValue.heroTemplate.mStiff);
					leaf["mStiffDef"] = Json::Value(iter->mValue.heroTemplate.mStiffDef);
					leaf["mBaseCrit"] = Json::Value(iter->mValue.heroTemplate.mBaseCrit);
					leaf["mCritRate"] = Json::Value(iter->mValue.heroTemplate.mCritRate);
					leaf["mCritDef"] = Json::Value(iter->mValue.heroTemplate.mCritDef);
					leaf["mStunDef"] = Json::Value(iter->mValue.heroTemplate.mStunDef);
					leaf["mStunTimeDef"] = Json::Value(iter->mValue.heroTemplate.mStunTimeDef);
					leaf["mSlowDef"] = Json::Value(iter->mValue.heroTemplate.mSlowDef);
					leaf["mSlowTimeDef"] = Json::Value(iter->mValue.heroTemplate.mSlowTimeDef);
					leaf["mPoisonDef"] = Json::Value(iter->mValue.heroTemplate.mPoisonDef);
					leaf["mBloodDef"] = Json::Value(iter->mValue.heroTemplate.mBloodDef);
					leaf["mKODef"] = Json::Value(iter->mValue.heroTemplate.mKODef);
					leaf["mFloatVal"] = Json::Value(iter->mValue.heroTemplate.mFloatVal);
					leaf["mFloatDef"] = Json::Value(iter->mValue.heroTemplate.mFloatDef);
					leaf["mFall"] = Json::Value(iter->mValue.heroTemplate.mFall);
					leaf["mFallDef"] = Json::Value(iter->mValue.heroTemplate.heroEx.mFallDef);
					leaf["mDefenceAreaStand"] = Json::Value(iter->mValue.heroTemplate.heroEx.mDefenceAreaStand.c_str());
					leaf["mDefenceAreaLie"] = Json::Value(iter->mValue.heroTemplate.heroEx.mDefenceAreaLie.c_str());
					for(List<UInt64>::Iter * it3 = iter->mValue.heroTemplate.m_listOriginSkills.begin();it3!=NULL;it3 = iter->mValue.heroTemplate.m_listOriginSkills.next(it3))
					{
						leaf["m_listOriginSkills"].append(it3->mValue);
					}
					for(List<UInt64>::Iter * it4 = iter->mValue.heroTemplate.m_listLearnSkills.begin();it4!=NULL;it4 = iter->mValue.heroTemplate.m_listLearnSkills.next(it4))
					{
						leaf["m_listLearnSkills"].append(it4->mValue);
					}
					
					Json::Value childLeaf;
					childLeaf["mVertigoDef"].append(iter->mValue.heroTemplate.heroEx.mVertigoDef);
					childLeaf["mVertigoTimeDef"].append(iter->mValue.heroTemplate.heroEx.mVertigoTimeDef);
					childLeaf["mBaseToughness"].append(iter->mValue.heroTemplate.heroEx.mBaseToughness);
					leaf["heroEx"].append(childLeaf);

					son["heroTemplate"]= leaf;
				}	
		
				

				for(Map<UInt32, SkillBaseTemplate*>::Iter *it = iter->mValue.skillBaseData.skillMap.begin();it!= NULL;it = iter->mValue.skillBaseData.skillMap.next(it))
				{
					Json::Value node;

					if(it->mKey == 0)
					{
						continue;
					}						
					node["skillID"] = Json::Value(it->mKey);

					if (it->mValue == NULL)
					{
						continue;
					}
					if (it->mValue->mName != "" )
					{
						node["mName"] = Json::Value(it->mValue->mName.c_str());
					}
					if (it->mValue->mDesc != "" )
					{
						node["mDesc"] = Json::Value(it->mValue->mDesc.c_str());
					}
					if (it->mValue->mType != 0 )
					{
						node["mType"] = Json::Value(it->mValue->mType);
					}
					if (it->mValue->mSubType != 0 )
					{
						node["mSubType"] = Json::Value(it->mValue->mSubType);
					}
					if (it->mValue->mTarget != 0 )
					{
						node["mTarget"] = Json::Value(it->mValue->mTarget);
					}
					if (it->mValue->mMaxLevel != 0 )
					{
						node["mMaxLevel"] = Json::Value(it->mValue->mMaxLevel);
					}
					if (it->mValue->mFrameStart != 0 )
					{
						node["mFrameStart"] = Json::Value(it->mValue->mFrameStart);
					}
					if (it->mValue->mFrameCount != 0 )
					{
						node["mFrameCount"] = Json::Value(it->mValue->mFrameCount);
					}
					if (it->mValue->mEffectId != 0 )
					{
						node["mEffectId"] = Json::Value(it->mValue->mEffectId);
					}
					if (it->mValue->mResPrefix != "" )
					{
						node["mResPrefix"] = Json::Value(it->mValue->mResPrefix.c_str());
					}
					if (it->mValue->mIcon != "" )
					{
						node["mIcon"] = Json::Value(it->mValue->mIcon.c_str());
					}
					if (it->mValue->mAnchorPoint != "" )
					{
						node["mAnchorPoint"] = Json::Value(it->mValue->mAnchorPoint.c_str());
					}
					if (it->mValue->mControl != 0 )
					{
						node["mControl"] = Json::Value(it->mValue->mControl);
					}

					if (it->mValue->mControlSpeed != 0 )
					{
						node["mControlSpeed"] = Json::Value(it->mValue->mControlSpeed);
					}
					if (it->mValue->mLoopNum != 0)
					{
						node["mLoopNum"] = Json::Value(it->mValue->mLoopNum);
					}
					if (it->mValue->mLoopFrameStart != 0)
					{
						node["mLoopFrameStart"] = Json::Value(it->mValue->mLoopFrameStart);
					}
					if (it->mValue->mLoopFrameEnd != 0)
					{
						node["mLoopFrameEnd"] = Json::Value(it->mValue->mLoopFrameEnd);
					}
					if (it->mValue->mCanTurnFace != 0)
					{
						node["mCanTurnFace"] = Json::Value(it->mValue->mCanTurnFace);
					}
					if (it->mValue->width != 0)
					{
						node["width"] = Json::Value(it->mValue->width);
					}
					if (it->mValue->height != 0)
					{
						node["height"] = Json::Value(it->mValue->height);
					}								
					son["skillBaseData"].append(node);
				}					
				for(List<SkillData>::Iter *it1 = iter->mValue.skillDatas.m_listSkills.begin();it1!= NULL;it1 = iter->mValue.skillDatas.m_listSkills.next(it1))
				{
					Json::Value node;						
					node["SkillData"].append(it1->mValue.m_nID);	
					node["SkillData"].append(it1->mValue.m_nCD);
					node["SkillData"].append(it1->mValue.m_nLevel);
					node["SkillData"].append(it1->mValue.m_nEquipPos);
					son["skillDatas"].append(node);
				}					
				for(List<BufferData>::Iter *it2 = iter->mValue.buffDataList.m_listBuffers.begin();it2!= NULL;it2 = iter->mValue.buffDataList.m_listBuffers.next(it2))
				{
					Json::Value node;

					node["BufferData"].append(it2->mValue.m_nBufferID);	
					node["BufferData"].append(it2->mValue.m_nGetTime);
					son["buffDataList"].append(node);
				}				
				root["charDatas"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};







} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PVP_THREAD_MSG_H__