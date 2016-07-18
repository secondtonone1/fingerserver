#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_CHARACTER_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_CHARACTER_MANAGER_H__

#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "../BuffManager.h"

static double GravityDeclineRate = 0.01;

namespace Lynx
{

	struct Area
	{			
		Area():x(0),y(0),w(0),h(0){}
		UInt32 x;//x坐标
		UInt32 y;//y坐标
		UInt32 w;//宽度
		UInt32 h;//高度
	};

	struct AttrSkillAdd
	{
		AttrSkillAdd():affectType(0),affectValue(0){}
		UInt32 affectType;
		UInt32 affectValue;

	};
	struct Damage
	{
		Damage():dType(0),hurtType(0),dValue(0){}
		//Type: 0:法术伤害, 1:物理伤害, hurtType: GameDefine.HURT_TYPE
		UInt32 dType;
		UInt32 hurtType;
		UInt32 dValue;
	};
	struct Damages
	{
		Damages():spcost(0),push(0),raise(0),stiff(0),damageval(0),hurtAni(0),hurtType(0),fall(false),fly(false),isCrit(false){}
		bool fall;
		bool fly;
		bool isCrit;

		UInt32 spcost;
		UInt32 push;
		UInt32 raise;
		UInt32 stiff;		
		UInt32	damageval;
		UInt32 hurtAni;
		UInt32 hurtType;
		Pos hurtPoint;		
		Damage damage;
		List<UInt32> addBuff;
		List<UInt32> buff;
	};
	
	
	struct AttParams
	{
		AttParams():notstand(0),isAttacksBegin(0){}
		UInt32 notstand;		//0:无效果, 1:浮空, 2:击倒, 3:击飞 
		UInt32 isAttacksBegin;
		Damage	damage;
		Area attackArea;		
		Map<UInt32, AttrSkillAdd*> attrSkillAdd;//{["GameDefine.CHAR_ATTR.AttPush"] = {affectType=1, affectValue=100}, ["GameDefine.CHAR_ATTR.AttRaise"] = {affectType=1, affectValue=100}, ...}
		List<UInt32> addBuff ;//{buffid1, buffid2}

	};
	
	struct HurtArea
	{
		Area stand;//站立时受伤害范围
		Area fall;//下落时受伤害范围
	};
	struct TotalRecordList
	{
		UInt32 maxhit;//最大连击次数
	};
// 	struct Targets
// 	{
// 	
// 		Map<Guid,Player*> keyObj;
// 		Map<Guid,Player> uuidPlayer;
// 	};

	struct ResID
	{
		ResID():charid(0),weapon(0),clothes(0),effect(0){}
		UInt32 charid;
		UInt32 weapon;
		UInt32 clothes;
		UInt32 effect;

	};
	struct LastP
	{
		LastP():hp(0),mp(0),sp(0),maxhp(0),maxmp(0),maxsp(0){}
		UInt32 hp;//血量
		UInt32 mp;//蓝量
		UInt32 sp;//霸体
		UInt32 maxhp;//最大血量
		UInt32 maxmp;//最高血量
		UInt32 maxsp;//最高霸体
	};

	struct SkillMap
	{
		Map<UInt32, SkillBaseTemplate*> skillMap;
	};

	struct BuffMap
	{
		Map<UInt32, BuffTemplate*> buffMap;
	};

	struct CharacterData
	{
		CharacterData(): hp(0),mp(0),sp(0),hitCnt(0),attr_buffEffect(0),camp(0),nowSpeed(0),moveStartTime(0), jumpStartTime(0), jump_speed(0), flying_state(0), keepFallCnt(0), 
			whichOneInBg(0), currentSkillId(0), currentMultiIdx(0),		 radio(0), killEnerNum(0), joystick_updown(0), hit(0), explodeNum(0), deadTimes(0), bulletStartTime(0),			
			skillBeginTime(0), flag_fall(false), flag_skillMoveY(false), flag_onGround(false), flag_haveAttachMaxY(false), bGravity(false), flag_canBeAttack(false), onAttack(false),		
			alertLowHp(false), effectLowHp(false), isOnQte(false), faceR(false), allowTurnFaceCommand(false), haveRecvTurnFaceCommand(false), canRealMove(false), canSkillControl(false),		
			flagAllowInterrupt(false), bulletStop(false), bulletDirection(false){}

		Guid playerID;	//玩家ID
		UInt32 level;	//等级
		UInt32 hp;	//血量
		UInt32 mp;	//蓝量
		UInt32 sp;	//霸体
		UInt32 attr_skillPassive;	////客户端弃用
		UInt32 attr_buffEffect;	//buff效果

		LastP lastP;	// 状态最高值

		//action state
		UInt32 eNowState;	//现在状态
		UInt32 eLastState;	//上次状态

		Pos pos;	//现在位置
		Pos lastPos;	//上次位置
		Int32 nowSpeed;	//现在速度
		UInt64 moveStartTime;	//移动开始时间
		UInt64 jumpStartTime;	//跳跃开始时间
		//area
		Area attackArea;	//	攻击范围
		HurtArea hurtArea;	//伤害范围

		UInt32 hitCnt;	//攻击个数
 
		//jump status
		bool flag_fall;	//下落状态
		bool flag_skillMoveY;	//技能移动上下状态
		bool flag_onGround;	//在地面状态
		bool flag_haveAttachMaxY;//
		Int32 jump_speed;	//跳跃速度
		bool bGravity;	//重力状态
		bool flag_canBeAttack;	//能否攻击状态
		UInt32 flying_state;	//飞行状态
		UInt32 keepFallCnt;	//连续浮空次数

		//阵营
		UInt32 camp;	//阵营

		UInt32 whichOneInBg;	//?隐藏

		//res
		//ResID resID;	//资源

		//异常状态
		bool flag_buffState[BUFF_STATE_SUB_TYPE_MAX];	//buff列表状态

		//self.targets = {}
		//Targets targets;//
		UInt32 currentSkillId;		//当前使用技能id
		UInt32 currentMultiIdx;	//客户端未用
		bool onAttack;	//攻击状态

		UInt32 radio;	//速度比率

		UInt32 killEnerNum;		//客户端未用
		UInt32 joystick_updown;		//  0:什么都没有，1:按住了上，2:按住了下
		// 			self.skillTab = "current"       -- "current", "back"
		UInt32 hit;		//连击
		bool alertLowHp;		//血条状态 雾
		bool effectLowHp ;		//血条状态 闪烁
		bool isOnQte;		//韵纹特效
		UInt32 explodeNum;		//客户端未用
		UInt32 deadTimes ;		//死亡次数
		bool faceR;		//右朝向标识
		bool allowTurnFaceCommand;		//能改变方向标识
		bool haveRecvTurnFaceCommand;		//接收到改变方向命令
		bool canRealMove;		////from scene 能移动标识
		bool canSkillControl;		//能使用技能
		bool flagAllowInterrupt;		//能打断标识

		//bullet
		bool bulletStop;		//子弹停止标识
		bool bulletDirection;		//子弹方向标识
		UInt64 bulletStartTime;		//子弹开始时间
		Pos bulletLastPos;		//子弹上次位置
		Pos bulletPos;		//	子弹当前位置
		Pos bulletSpeed;		//子弹速度

		UInt64 skillBeginTime;		//技能开始时间
		
		TotalRecordList totalRecordList;		//世界记录

		HeroTemplate heroTemplate;		////attr_base 角色基础属性值
// 		LevelGrowTemplate *levelGrowTemplate;		////attr_level成长属性

		SkillMap skillBaseData;		//技能基础信息

		PlayerSkillListData  skillData;		//玩家的技能数据//名字不对
		PlayerSkillListData  skillDatas;

		BuffMap buffData;		//玩家的buff数据

		PlayerBuffListData buffDataList;
	};


	class Character: public Singleton<Character>
	{
	public:
		Character();
		virtual ~Character();

		bool skillAttack(AttParams attParams, UInt32 isAttacks);

		const UInt32 getPlayerCoin() const { return m_character.keepFallCnt; }

		const UInt32&  GetKeepFallCnt() const	{return m_character.keepFallCnt;}

		void setKeepfallCnt(UInt32 num)
		{
			m_character.keepFallCnt = num;
		}

		bool getallowTurnFaceCommand()
		{
			return m_character.allowTurnFaceCommand;
		}

		void setallowTurnFaceCommand(bool num)
		{
			m_character.allowTurnFaceCommand = num;
		}

		bool gethaveRecvTurnFaceCommand()
		{
			return m_character.haveRecvTurnFaceCommand;
		}

		void sethaveRecvTurnFaceCommand(bool num)
		{
			m_character.haveRecvTurnFaceCommand = num;
		}

		UInt32 getCampID()
		{
			return m_character.camp;
		}

		void setCampID(UInt32 num)
		{
			m_character.camp = num;
		}

		bool getFlagAllowInterrupt()
		{
			return m_character.flagAllowInterrupt;
		}

		void setflagAllowInterrupt(bool num)
		{
			m_character.flagAllowInterrupt = num;
		}

		

		UInt32 getWhichOneInBg()
		{
			return m_character.whichOneInBg;
		}

		void setWhichOneInBg(UInt32 num)
		{
			m_character.whichOneInBg = num;
		}


		bool getCanSkillControl()
		{
			return m_character.canSkillControl;
		}

		void setCanSkillControl(bool num)
		{
			m_character.canSkillControl = num;
		}

		HeroTemplate  getHeroTemplate()
		{
			return m_character.heroTemplate;
		}

		void setHeroTemplate(HeroTemplate  num)
		{
			m_character.heroTemplate = num;
		}

		bool getCanRealMove()
		{
			return m_character.canRealMove;
		}

		void setCanRealMove(bool num)
		{
			m_character.canRealMove = num;
		}

		UInt32 getjoystick_updown()
		{
			return m_character.joystick_updown;
		}
		void setjoystick_updown(UInt32 num)
		{
			m_character.joystick_updown = num;
		}

		SkillMap getSkillMap()
		{
			return m_character.skillBaseData;
		}
		void setSkillMap(SkillMap num)
		{
			m_character.skillBaseData = num;
		}

		bool getBuffStateByIdx(UInt32 idx)
		{
			if(idx <= 0)
			{
				return false;

			}
			if (idx >= BUFF_STATE_SUB_TYPE_MAX)
			{
				return false;
			}
			return m_character.flag_buffState[idx];		
		}
		void setBuffStateByIdx(UInt32 idx,bool num)
		{
			m_character.flag_buffState[idx] = num;		
		}
		
		UInt32 getResIDByIdx(UInt32 idx)
		{
			return 0;
		}
		void setKeepfallCnt(UInt32 idx,UInt32 id)
		{
			// 		m_character.keepFallCnt = num;
		}

		UInt32 getSkillList()
		{
			return 0;
		}

		UInt32 getBuffList()
		{
			return 0;
		}

		UInt32 getServer()
		{
			return 0;
		}

		UInt32 getClient()
		{
			return 0;
		}

		UInt32 getHP()
		{
			return m_character.hp;
		}
		void setHP(UInt32 num)
		{
			m_character.hp = num;
		}
		UInt32 getLevel()
		{
			return m_character.level;
		}
		void setLevel(UInt32 num)
		{
			m_character.level = num;
		}

		UInt32 getMP()
		{
			return m_character.mp;
		}
		void setMP(UInt32 num)
		{
			m_character.mp = num;
		}

		UInt32 getSP()
		{
			return m_character.sp;
		}
		void setSP(UInt32 num)
		{
			m_character.sp = num;
		}

		UInt32 getNowState()
		{
			return m_character.eNowState;
		}
		void setNowState(UInt32 num)
		{
			setLastState();
			m_character.eNowState = num;
		}

		UInt32 getLastState()
		{
			return m_character.eLastState;
		}
		void setLastState(UInt32 num =0)
		{
			m_character.eLastState = getLastState();
		}

		Pos getPos()
		{
			return m_character.pos;
		}
		void setPos(Pos pos)
		{
			m_character.pos = pos;
		}
		UInt32 getPosX()
		{
			return m_character.pos.x;
		}
		void setPosX(UInt32 num)
		{
			m_character.pos.x  = num;
		}
		UInt32 getPosY()
		{
			return m_character.pos.y;
		}
		void setPosY(UInt32 num)
		{
			m_character.pos.y = num;
		}

		Pos getLastPos()
		{
			return m_character.lastPos;
		}

		void setLastPos(Pos pos)
		{
			m_character.lastPos = pos;
		}
		UInt32 getLastPosX()
		{
			return m_character.lastPos.x;
		}
		void setLastPosX(UInt32 num)
		{
			m_character.lastPos.x  = num;
		}
		UInt32 getLastPosY()
		{
			return m_character.lastPos.y;
		}
		void setLastPosY(UInt32 num)
		{
			m_character.lastPos.y = num;
		}

		Area getAttactArea()
		{
			return m_character.attackArea;
		}
		void setAttactArea(Area area)
		{
			m_character.attackArea = area;
		}

		Area getDefenceAreaStand()
		{
			return m_character.hurtArea.stand;
		}
		void setDefenceAreaStand(Area num)
		{
			m_character.hurtArea.stand = num;
		}

		Area getDefenceAreaFall()
		{
			return m_character.hurtArea.fall;
		}
		void setDefenceAreaFall(Area num)
		{
			m_character.hurtArea.fall = num;
		}

		UInt32 getCurSkillID()
		{
			return m_character.currentSkillId;
		}
		void setCurSkillID(UInt32 num)
		{
			m_character.currentSkillId = num;
		}
		UInt64 getSkillBeginTime()
		{
			return m_character.skillBeginTime;
		}
		void setSkillBeginTime(UInt64 num)
		{
			m_character.skillBeginTime = num;
		}

		bool getFaceR()
		{
			return m_character.faceR;
		}
		void setFaceR(bool num)
		{
			m_character.faceR = num;
		}
		
		UInt32 getCurMultiIdx()
		{
			return m_character.currentMultiIdx;
		}
		void setCurMultiIdx(UInt32 num)
		{
			m_character.currentMultiIdx = num;
		}

		bool getAttackFlag()
		{
			return m_character.onAttack;
		}
		void setAttackFlag(bool num)
		{
			m_character.onAttack = num;
		}

		UInt32 getSkillMoveYFlag()
		{
			return m_character.flag_skillMoveY;
		}
		void setSkillMoveYFlag(UInt32 num)
		{
			m_character.flag_skillMoveY = num;
		}

		bool getFallFlag()
		{
			return m_character.flag_fall;
		}
		void setFallFlag(UInt32 num)
		{
			m_character.flag_fall = num;
		}

		bool getOnGroundFlag()
		{
			return m_character.flag_onGround;
		}
		void setOnGroundFlag(UInt32 num)
		{
			m_character.flag_onGround = num;
		}

		UInt32 getHaveAttachMaxYFlag()
		{
			return m_character.flag_haveAttachMaxY;
		}
		void setHaveAttachMaxYFlag(UInt32 num)
		{
			m_character.flag_haveAttachMaxY = num;
		}

		UInt32 getJumpSpeed()
		{
			return m_character.jump_speed;
		}
		void setJumpSpeed(UInt32 num)
		{
			m_character.jump_speed = num;
		}

		bool getGravityFlag()
		{
			return m_character.bGravity;
		}
		void setGravityFlag(bool num)
		{
			m_character.bGravity = num;
		}

		UInt32 getCanBeAttack()
		{
			return m_character.flag_canBeAttack;
		}
		void setCanBeAttack(UInt32 num)
		{
			m_character.flag_canBeAttack = num;
		}

		UInt32 getFlyingState()
		{
			return m_character.flying_state;
		}
		void setFlyingState(UInt32 num)
		{
			m_character.flying_state = num;
		}
		CharacterData getCharacterData()
		{
			return m_character;
		}


	public:
		//关卡中角色属性初始化
		void initCharacter(Guid playerID);
		//角色初始属性与等级增加属性
		void initLevelAttr(Guid playerID,UInt32 level);

		//饰品
		void initLevelAttr(Guid playerID);

		//战力计算
		UInt32 calcPower(Guid playerID,UInt32 which);

	








		void update_skill();
		void updateHeight(UInt64 t);
		void release();
		void initAttr(UInt32 playerID);
		void initSkill(UInt32 playerID);
		void initBuff(UInt32 playerID);	

		void initEquipAttr(UInt32 playerID);
		void initFashionAttr(UInt32 playerID);

		void update_recover(UInt32 t);
		
		void changeMP(Int32 val);
		void changeSP(Int32 val);

		void playHurtEffect(Damages damages);

		void onHurt(Guid attacker,Damages damages,Pos p);

		void stopPreferPushMoveX();

		UInt32 getKeepFallCnt();

		void setKeepFallCnt(UInt32 val);

		void flyToWall(UInt32 dirIsRight);

		void turnFace(bool faceR);

		UInt32 fall(UInt32 height,UInt32 delay);

		void die(bool flag);

		void addBuff(UInt32 val);

		void victory();

		UInt32 getSkillMP(UInt32 skillId);

		SkillBaseTemplate* getSkillBaseById(UInt32 skillId);

		UInt32 getSkillIdByType(UInt32 skillType);

		UInt32 getSkillCnt();

		bool addSkill(UInt32 skillId,SkillData skillData);

		bool removeSkill(UInt32 skillId);

		void onChangeSkill(UInt32 skillId,UInt32 bIsAdd);

		void onSkillInterrupt(UInt32 skillID);

		void stopAnimation(UInt32 skillID);

		void onSkillFinish(UInt32 skillID);

		void skillStart(UInt32 skillId);

		void skillEnd(Guid playerID);

// 		bool checkInTargets(Guid uid );

		UInt32 getTargetsCnt();

		void addToTargets(Guid playerID);

		void cleanTargets();

		Area getScreenAttackArea(Area rectData);

		Area getScreenDefenceArea();

		BuffTemplate* getBuffByID(UInt32 buffId );
		UInt32 getBuffCnt();

		bool skillDamage(Character *defencer, AttParams attParams);

		UInt32 getBuffIdByType(UInt32 buffType,UInt32 buffSubType);

		BuffType getBuffTypeByID(UInt32 buffId);

		bool addBuff(UInt32 buffId,BufferData buffObj,Int32 keepTime);

		void removeBuff(UInt32 buffId );

		void removeAllBuff();

		void onChangeBuff(UInt32 buffId,UInt32 bIsAdd);

		void onJumpStart(UInt32 height);

		void onJumpEnd();

		void fallGroundDelay( UInt32 t);

		void revive();

		bool checkCanRun();

		bool checkCanTurn();

		bool checkCanJump();

		bool checkIsDie();		//是否死亡

		bool checkNeedPlayHurtAni();		//sp

		bool checkNeedExplode();		//玩家不会被击碎

		bool checkSkill(UInt32 skillId);

		void changeHP(Int32 val);

		UInt32 getMoveSpeed();

		void toIdle();

		bool heroLeftMove(UInt32 speed);

		bool heroRightMove(UInt32 speed);

		bool updateMove(UInt64 t);

// 		bool checkJumpUpdate();

		void onDead();

		void stopAllAction(bool isStop);

		void playAnimation(UInt32 aniType,UInt32 delaytime);

		void refreshPosition(UInt32 val);

		bool EventAttack(AttParams params,bool isKeep);

		bool playSkill();

		bool beforePlaySkill();

		bool procSkill();

		bool isNowAllowPlayerInterruptSkill(UInt32 skillID);

		bool isNeedCD(UInt32 skillID);

		bool skillEventMove(Pos moveDistance,Pos moveSpeed);

		bool bulletUpdateMove(UInt64 t);

		bool bulletStart(UInt64 t);

		bool startFrameEvent(UInt32 eventType,AttParams eventParams);

		bool updateSkill(UInt64 t);

		bool onSkillStart();

		SkillLevelTemplate* getSkillLevelTemplate (UInt32 skillId);

		bool getSkillCD(UInt32 skillID);

		void setSkillCD(UInt32 skillID,UInt64 startTime);

		SkillData getskillDatabyID(UInt32 skillID);

		void AniComplete(UInt32 aniType,UInt32 id);

		void addHit();


	private:
		CharacterData m_character;
		
	};

	
}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_CHARACTER_MANAGER_H__
