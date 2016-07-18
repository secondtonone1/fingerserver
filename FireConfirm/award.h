#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_FIRE_MANAGER_H__


{

	// module("GameDefine", package.seeall)
	// 
	// 
	// GRUOND_Y = display.bottom + 130 		//地面的Y坐标
	// FRAMES_PERSECOND = 30
	// 
	// PLAYER_TAG = 100
	// 
	// JOYSTICK_RADIUS = 35
	// JOYSTICK_LENGTH = 3 					//控制摇杆触摸长度 3*JOYSTICK_RADIUS
	// 
	// DEFAULT_PLAYER_CAMP = 2
	// 
	// BAG_TABLEVIEW_NUM = 3
	// 
	// CHAR_SHADOW = 1
enum	CHAR_SHADOW_CONFIG = {
		scaleY = 0.5,
		offsetX = 0,
		offsetY = 0,
		opacity = 75,
		skewX = 0,
		rotate = 15
	};

enum	CHAR_ATTR = {
		MaxHP 				= 1,	//max hp
		MaxMP 				= 2,	//max mp
		MaxSP				= 3,	//max sp
		PA 					= 4,	//物理攻击
		MA 					= 5,	//物理防御
		PF 					= 6,	//法术攻击
		MF 					= 7,	//法术防御
		HPRecover			= 8,    //hp回复速度
		MPRecover			= 9,    //mp回复速度
		SPRecover 			= 10,	//sp回复速度
		AttSpeed 			= 11,	//攻击速度
		CombinationAtt 		= 12,	//连招间隔
		MoveSpeed 			= 13,	//移动速度
		JumpHeight 			= 14,	//跳起高度
		FlyTimes 			= 15,	//可连续浮空次数
		SPDamage			= 16,	//sp消耗
		AttPush 			= 17,	//击退
		AttPushDef 			= 18,	//击退防御
		AttRaise 			= 19,	//挑起
		AttraiseDef 		= 20,	//挑起防御
		Stiff				= 21,	//基础僵直
		StiffDef 			= 22,	//僵直防御
		BaseCrit 			= 23,	//基础暴击率
		Critrate 			= 24,	//暴击倍率
		BaseToughness 		= 25,	//韧性
		VertigoDef 			= 26,	//眩晕防御(几率)
		VertigoTimeDef 		= 27,	//眩晕防御(时间)
		SlowDef 			= 28,	//减速防御(几率)
		SlowTimeDef 		= 29,	//减速防御(时间)
		PoisonDef 			= 30,	//中毒防御
		BloodDef 			= 31,	//易伤防御
		KODef 				= 32,	//秒杀防御
		FloatVal            = 33,   //击飞
		Floatdef            = 34,	//击飞抗性
		Fall                = 35,	//击倒
		Falldef             = 36	//击倒抗性
	};

enum	CHAR_ATTR_KEY = {
		MaxHP			= "MaxHP",		//max hp
		MaxMP 			= "MaxMP",		//max mp
		MaxSP 			= "MaxSP",		//max sp
		PA 				= "PA",			//物理攻击
		MA 				= "MA",			//物理防御
		PF 				= "PF",			//法术攻击
		MF 				= "MF",			//法术防御
		HPRecover 		= "HPRecover",  //hp回复速度
		MPRecover 		= "MPRecover",  //mp回复速度
		SPRecover 		= "SPRecover",	//sp回复速度
		AttSpeed 		= "AttSpeed",	//攻击速度
		CombinationAtt 	= "CombinationAtt",	//连招间隔
		MoveSpeed 		= "MoveSpeed",	//移动速度
		JumpHeight 		= "JumpHeight",	//跳起高度
		FlyTimes 		= "FlyTimes",	//可击飞次数
		SPDamage 		= "SPDamage",	//sp消耗
		AttPush 		= "AttPush",	//击退
		AttPushDef 		= "AttPushDef",	//击退防御
		AttRaise 		= "AttRaise",	//挑起
		AttraiseDef 	= "AttraiseDef",//挑起防御
		Stiff 			= "Stiff",		//基础僵直
		StiffDef 		= "StiffDef",	//僵直防御
		BaseCrit 		= "BaseCrit",	//基础暴击率
		Critrate 		= "Critrate",	//暴击倍率
		BaseToughness 	= "BaseToughness",	//韧性
		VertigoDef 		= "VertigoDef",		//眩晕防御(几率)
		VertigoTimeDef 	= "VertigoTimeDef",	//眩晕防御(时间)
		SlowDef 		= "SlowDef",	//减速防御(几率)
		SlowTimeDef 	= "SlowTimeDef",	//减速防御(时间)
		PoisonDef 		= "PoisonDef",	//中毒防御
		BloodDef 		= "BloodDef",	//易伤防御
		KODef 			= "KODef", 		//秒杀防御
		FloatVal 		= "FloatVal",   //击飞
		Floatdef 		= "Floatdef",	//击飞抗性
		Fall 			= "Fall",		//击倒
		Falldef 		= "Falldef",	//击倒抗性
		[1]		= "MaxHP",			//max hp
		[2]		= "MaxMP",			//max mp
		[3]		= "MaxSP",			//max sp
		[4]		= "PA",				//物理攻击
		[5]		= "MA",				//物理防御
		[6]		= "PF",				//法术攻击
		[7]		= "MF",				//法术防御
		[8]		= "HPRecover",    	//hp回复速度
		[9]		= "MPRecover",    	//mp回复速度
		[10]	= "SPRecover",		//sp回复速度
		[11]	= "AttSpeed",		//攻击速度
		[12]	= "CombinationAtt",	//连招间隔
		[13]	= "MoveSpeed",		//移动速度
		[14]	= "JumpHeight",		//跳起高度
		[15]	= "FlyTimes",		//可击飞次数
		[16]	= "SPDamage",		//sp消耗
		[17]	= "AttPush",		//击退
		[18]	= "AttPushDef",		//击退防御
		[19]	= "AttRaise",		//挑起
		[20]	= "AttraiseDef",	//挑起防御
		[21]	= "Stiff",			//基础僵直
		[22]	= "StiffDef",		//僵直防御
		[23]	= "BaseCrit",		//基础暴击率
		[24]	= "Critrate",		//暴击倍率
		[25]	= "BaseToughness",	//韧性
		[26]	= "VertigoDef",		//眩晕防御(几率)
		[27]	= "VertigoTimeDef",	//眩晕防御(时间)
		[28]	= "SlowDef",		//减速防御(几率)
		[29]	= "SlowTimeDef",	//减速防御(时间)
		[30]	= "PoisonDef",		//中毒防御
		[31]	= "BloodDef",		//易伤防御
		[32]	= "KODef", 		    //秒杀防御
		[33]	= "FloatVal",   	//击飞
		[34]	= "Floatdef",		//击飞抗性
		[35]	= "Fall",			//击倒
		[36]	= "Falldef",		//击倒抗性
	}

enum	CHAR_ATTR_CHS_STR = {
		[1]		= "生命上限",
		[2]		= "法力上限",
		[3]		= "霸体上限",
		[4]		= "物理攻击",
		[5]		= "物理防御",
		[6]		= "法术攻击",
		[7]		= "法术防御",
		[8]		= "hp回复速度",
		[9]		= "mp回复速度",
		[10]	= "sp回复速度",
		[11]	= "攻击速度",
		[12]	= "连招间隔",
		[13]	= "移动速度",
		[14]	= "跳起高度",
		[15]	= "可连续浮空次数",
		[16]	= "sp消耗",
		[17]	= "击退",
		[18]	= "击退防御",
		[19]	= "挑起",
		[20]	= "挑起防御",
		[21]	= "基础僵直",
		[22]	= "僵直防御",
		[23]	= "基础暴击率",
		[24]	= "暴击倍率",
		[25]	= "韧性",
		[26]	= "眩晕防御(几率)",
		[27]	= "眩晕防御(时间)",
		[28]	= "减速防御(几率)",
		[29]	= "减速防御(时间)",
		[30]	= "中毒防御",
		[31]	= "易伤防御",
		[32]	= "秒杀防御",
		[33]	= "击飞",
		[34]	= "击飞抗性",
		[35]	= "击倒",
		[36]	= "击倒抗性",
	};

enum	ATTRIBUTE_COUNT = 36	//属性数量

		//角色类型
		CHARACTER_TYPE = {
			Player 		= 1,	//玩家
			Monster 	= 2,	//怪
			NPC 		= 3,	//npc
	};

// 	//怪物类型
enum		MONSTER_TYPE = {
			Normal 		= 1,	//普通怪
			Elite 		= 2,	//精英怪
			Boss 		= 3,	//boss
			Goblin 		= 4		//哥布林
	};

// 	//技能类型
		SKILL_TYPE = {
			Normal 		= 1,	//普通
			Passive 	= 2,	//被动
			Multi 		= 3,	//多段
			Cumulate 	= 4,	//蓄力
			Continue 	= 5,	//连续
			Throw 		= 6,	//投掷
			RaiseUp 	= 7,	//上挑
			FallDown 	= 8,	//下压
	}

// 	//伤害类型
enum		HURT_TYPE = {
			Default 	= 1,	//默认
	};


// 	// buff大类型
enum		BUFF_TYPE = {
			Attr 		= 1,	//属性
			State 		= 2,	//异常状态
			Display 	= 3 	//表现
	};
// 	//异常状态类buff子类型
enum		BUFF_STATE_SUB_TYPE = {
			Vertigo 	= 1,	//眩晕
			Slow 		= 2,	//减速
			Poison 		= 3, 	//中毒
			Blood 		= 4,	//易伤
			God 		= 5,	//无敌
			Endure 		= 6,	//霸体
	};

// 	//加成方式
enum		VALUE_EFFECT_TYPE = {
			numval = 1,		//按值加成
			numrate = 2,	//按百分比加成
	};

	//buff 配置最大参数数量
#define 	MAX_BUFF_CONFIG_PARAM_NUM = 8

		//buff end

	enum	GROW_TYPE = {
			line = 1,			// y = ax + b
			parabolic = 2,		// y = ax^2 + bx + c
	}


enum	CHAR_NODE_PRIORITY = {
		CHAR 		= 1,
		WEAPON_L 	= 2,
		FASHION 	= 3,
		WEAPON_R 	= 4,
		EFFECT 		= 5,
	}
#define 	CHAR_NODE_PIC_CNT = 5	// CHAR_NODE_PRIORITY的数量

	enum	RES_PREFIX_TBL = {
			[1] = "_char",
			[2] = "_Lweapon",
			[3] = "_clothes",
			[4] = "_Rweapon", 
			[5] = "_effect", 
	};

	//击飞过程状态
	enum	CHARACTER_FLYING_STATE = {
			none 		= 0,		//没有击飞
			startfly 	= 1,		//开始飞
			touchwall 	= 2,		//撞墙反弹
			fallground 	= 3 		//反弹后朝地面下落
	};

	//子弹类型
enum		BULLET_TYPE = {
			line	= 1,	
			object	= 2,
	};
enum	BULLET_DESTORY_TYPE = {
		none = 0,
		outScreen = 1 , //超出屏幕， 
		stop = 2 ,      //不在移动, 
		after = 3 ,     //出现一段时间,
		trigger = 4,    //碰撞后
		stopAter = 5    //停止移动一段时间
	};


	//场景AI事件
enum		SCENEAI_CONDITION_TYPE = {
			//种怪相关
			monster_seed 			= 1,

			//time
			time_after 				= 101,  	//多久之后
			time_each 				= 102,		//每隔多久
			hero_time_use 			= 103, 		//多长时间之后
			//position
			pos_pass_id 			= 121, 		//某单位经过某个位置
			pos_pass_camp 			= 122,
			pos_between 			= 123,

			//attr
			attr_atValue_id 		= 141,
			attr_changeValue_id 	= 142,
			attr_atPercent_id 		= 143,
			attr_changePercent_id 	= 144,
			attr_val_camp 			= 145, 		//某单位某属性大于、等于、小于某值/百分比
			hero_hp_camp 			= 146,		//hero Hp低于某值时触发
			//state
			state_die_id 			= 161,
			state_buff_id 			= 162,
			unit_dead 				= 163, 		//某个单位死亡
			hero_relive_times 		= 164, 		//玩家复活次数超过某数
			//action
			action_start_id 		= 181,
			action_end_id 			= 182, 		//某个动作结束
			action_end_scene 		= 183,

			//num_camp
			monster_num_camp 		= 191, 		//monster数量大于、等于、小于某值

			// unit_distance
			unit_distance 			= 151, 		//两个单位之间距离

			//chapter_progress
			chapter_progress 		= 171, 		//关卡进度
	};

	enum SCENEAI_EVENT_TYPE = {
		change_camp			= 101, 		 	 			//1
		play_skill			= 102, 			//释放技能 1
		lock_attr			= 103,			//锁定属性 1
		show_talk			= 104,			//播放对白 1
		create_monster		= 105, 			//创建monster
		lock_scene			= 106, 			//锁定场景
		unlock_scene		= 107, 			//解锁场景
		play_ani			= 108, 			//播放动画 1
		add_buff			= 109, 			//添加buff 1
		change_scene_label  = 110, 			//改变场景进度
		boss_apear_alert 	= 111, 			//前方高能警告
		update_timer        = 112,			//改变计时器
		start_timer         = 113, 			//开始计时器
		stop_timer          = 114,			//停止计时器
		pause_timer         = 115, 			//暂停计时器
		star_unget 			= 117,			//失去一颗星（默认获得，达到一定条件失去，例：用的time太长）
		chapter_victory 	= 118, 			//关卡胜利
		unit_change_camp 	= 119, 			//特定单位转变阵营
		camp_to_camp 		= 120, 			//特定阵营转变为另一个阵营
		play_bg_animation 	= 121,			//播放背景层动画
		stop_bg_animation 	= 122, 			//停止背景动画
		unit_bg_to_map 		= 123, 			//monster从背景层进战斗层
	};


enum	SCENEAI_UNIT_ACTION_TYPE = {
		fall2up	    		=	3, //浮空起身
		airhurt	    		=	4, //空中受击
		airfall	    		=	5, //空中下落
		flyback2up			=	6, //反弹后起身
		hurt	    		= 	8, //多个受击动作（hurtbefore）
	};


	//角色状态
enum		CHAR_STATE = {
			idle		=	1,		//空闲站立
			run			=	2,		//移动
			jump		=	3,		//跳跃
			die			=	4,		//死亡
			hurt		=	5,		//受击
			fall		=	6,		//浮空
			victory		=	7,		//胜利
			attack		=	8,		//攻击
			show 		= 	9,		//展示
			showback 	= 	10, 	//展示结束
	};

	//角色ai状态
	enum	AI_STATE = {
			none        =   0,      //
			idle		=	1,		//空闲
			fan			=	2,		//休闲
			walk		=	3,		//寻路
			escape		=	4,		//逃跑
			follow		=	5,		//跟随
			patrol		=	6,		//巡逻
			attack		=	7,		//攻击
			wander		=	8,		//徘徊
			hide 		=   9,  	//躲藏
			show 		=   10,		//展示
			fall 		= 	11, 	//倒地
	};

	//技能帧事件
enum		FRAME_EVENT_TYPE = {
			move 		=	1,		//移动
			wait		=	2,		//等待
			shake		=	3,		//震动
			attack 		=	4,		//攻击（单一帧）
			attackKeep 			=	5,	//攻击（帧区间）
			mulitCheck_start	=	7,		//段技能连击判定区间开始
			mulitCheck_end		=	8,		//段技能连击判定区间结束并进行判定
			addBuff 	= 	9,		//增加buff
			addBullet	=	10,		//增加子弹
			addMusicEffect		=	11,		//增加音效
			addAniEffect		=	12,		//增加特效
			allowInterrupt		=	13,		//在此帧事件后允许玩家主动打断技能
	};

enum	JoystickDir = {
		up = "Joystick_up",
		down = "Joystick_down",
		right = "Joystick_right",
		left = "Joystick_left",
		stop = "Joystick_stop",
	};
enum	ButtonClick = {
		skill1 = "Btn_skill",
		skill2 = "Btn_skill2",
		soldierBtn = "Btn_soldier",
		attack = "Btn_attack",
		pause = "Btn_pause",
		jump = "Btn_jump",
	};

	//比较符号
	enum	CompareSign = {
			eq = 1,  // 等于
			rt = 2,  // 大于
			lt = 3,  // 小于
			rte = 4, // 大于等于
			lte = 5,  // 小于等于
			inc = "+", //递增
			dec = "-", //递减
	};

	//攻击时 被攻击者的状态
		//0:站立不变,1:挑到空中,2:打倒在地,3:击飞
enum		BeHurtedState = {
			stand = 0,
			float = 1, 
			fall = 2, 
			floatX = 3,
	};

enum	ANI_KEY_NAME = {
		idle = "idle",
		run = "run",
		airhurt = "airhurt",
		airfall = "airfall",
		fall = "fall",
		fall2up = "fall2up",
		victory = "victory",
		die = "die",
		flyback = "flyback",
		show = "show",
		flybackfall = "flybackfall",
		flyback2up = "flyback2up",
		jumpbefore = "jumpbefore", 
		jumpup = "jumpup", 
		jumpdown = "jumpdown", 
		jumpafter = "jumpafter",
		showback = "showback",
		showstart = "showstart",
		escapewin = "escapewin",
		escapelose = "escapelose",
	};

	//资源类型
enum		GAME_RES_TYPE = {
			coin 		= 1,
			rmb 		= 2,
			strength 	= 3,
			exp 		= 4,
			prestige 	= 5,
			item 		= 6,
	};

	//关卡激活条件
enum		STAGE_ACTIVE_CONDITION = {
			level 			= 1,
			preFinishStage 	= 2,
			mission 		= 3,
			preFinishStageStarCnt = 4,
			vipLevel		= 5,
			guildLevel		= 6,
	};

	//关卡类型
enum		STAGE_TYPE = {
			normal = 1,
			master = 2,
			activity = 3,

			[1] = "normal",
			[2] = "master",
			[3] = "activity",
	};

	//关卡得星条件
		STAGE_STAR_CONDITION = {
			useTime 			= 1,
			reviveTimes 		= 2,
			useMedicinetimes	= 3,
			leftHpValue			= 4,
			leftHpRate			= 5,
	};
	//关卡星数量
#define 		STAGE_STAR_NUMBER = 3

		//每章节最多关卡数量
#define 		CHAPTER_MAX_STAGE_NUMBER = 20

		//扫荡道具种类
#define 	SWEEP_ITEM_TYPE_NUM = 3

		//背包每行的背包格数
#define 	ITEM_CNT_EACH_ROW = 5

		//道具类型
enum		ITEM_TYPE = {
			equip 			= 1,	//装备
			gem 			= 2,	//宝石
			enhance		 	= 3,	//强化石
			cost 			= 4, 	//游戏消耗品
			symbol 			= 5,	//符文
			treasureBox 	= 6,	//宝箱
			presentpackage 	= 7,	//礼包
			medicine 		= 8,	//药品恢复类道具
			equip_piece 	= 9,	//装备碎片
			hoarstone_piece = 10,	//界石碎片
			other 			= 11,	//其它游戏道具
	}

	//宝石子类型
enum		ITEM_GEM_SUBTYPE = {
			red 	= 1,	//红
			yellow 	= 2,	//黄
			green 	= 3,	//绿
			blue 	= 4,	//蓝
			purple	= 5,	//紫
	};

	//强化石子类型
enum		ITEM_ENHANCE_SUBTYPE = {
			weapon 	= 1,	//武器强化石	
			defence = 2,	//防具强化石
			jewelry = 3, 	//饰品强化石
	};

	//游戏消耗品子类型
enum		ITEM_COST_SUBTYPE = {
			remeltStone 	= 1,		//装备洗炼石
			sweepPaper		= 2,		//扫荡卷
			rhymeSoul 		= 3,		//韵魂
			dungeonTicket	= 4,		//副本入场券
	};

	//药品恢复类道具子类型
		ITEM_MEDICINE_SUBTYPE = {
			1
	}

	//其它游戏道具子类型
enum		ITEM_OTHER_SUBTYPE = {
			key_gold	= 1,	//金钥匙
			key_silver	= 2,	//银钥匙
	};

	//道具品质
	enum	ITEM_QUALITY = {
			white 	= 1,	//白
			green 	= 2,	//绿
			blue 	= 3,	//蓝
			purple 	= 4,	//紫
			golden 	= 5,	//金
	};

	//装备点
enum		CHARACTER_EQUIP_POS = 
	{
		weapon 	= 1,	//武器
		head	= 2,	//头
		chest 	= 3,	//胸
		belt 	= 4,	//腰带
		leg 	= 5,	//腿
		foot 	= 6,	//脚
		jewelry = 7,	//首饰
		amulet 	= 8,	//护符
	};

	//装备最高星级
#define 		MAX_EQUIP_STAR_LEVEL = 5

		//道具的参数属性数量
#define 	ITEM_PARAM_NUM = 8

		//背包分类
enum		BAG_TAB_TYPE = 
	{
		equip = 1,
		item  = 2,
		other = 3,
		[1] = "equip",
		[2] = "item",
		[3] = "other",
	};

	// 关卡分类
enum		SELECT_TAG_TYPE = 
	{
		normal = 1,
		master = 2,
		[1] = "normal",
		[2] = "master",
	};
	//关卡扫荡道具
enum		SELECT_SWEEP_PROPERTY = 
	{
		cunzheng = 1,                //村正
		power    = 2,                //小体力药
		gem      = 3,                //宝石
		[1]      = "cunzheng",
		[2]      = "power",
		[3]      = "gem",
	};
	//时装最多生效的属性数量
#define 		MAX_FASHION_EFFECTIVE_ATTR_COUNT = 6

		//界石类型数量
#define 	HOARSTONE_TYPE_NUM = 12

// 		//对应资源名
// 	enum	colorRes = {
// 			ResourceDef.IMAGE_BAG_BTNFRAME_WHITE, 
// 			ResourceDef.IMAGE_BAG_BTNFRAME_GREEN,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_BLUE,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_PURPLE,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_GRAY, 
// 	};

	//effect type
enum		EFFECT_RES_TYPE = {
			particle = 1,
			frameAction = 2,
			blend = 3,
			other = 4,
	};

enum	EFFECT_PLAY_TYPE = {
		PLAY_ONCE = 1,
		PLAY_TIME = 2,
		PLAY_FOREVER = 3,
		SPECIAL = 4,
	};

enum	EFFECT_POS_REFERENCE_TYPE = {
		Camera = 1,
		MapPos = 2,
		AttackerPos = 3,
		DefencerPos = 4,
	};

enum	EFFECT_PARTICLE_POSITION_TYPE = {
		free = 1,
		grouped = 2,
		relative = 3,
	};

	//refresh event
enum		GAME_REFRESH_EVENT = {
			addItem 	= "addItem",
			removeItem	= "removeItem",
			updateItem	= "updateItem",
			equipResult		= "equipResult",
			updateFashion	= "updateFashion",
			equipFashion 	= "equipFashiion",
			coinChange	= "coinChange",
			rmbChange	= "rmbChange",
			strengthChange	= "strengthChange",
	};

	//佣兵装备槽数
#define 	MAX_LANSQUENET_EQUIP_POS_NUM = 4

		//MonsterType
	enum	MonsterType = {
			monster = 0,
			boss 	= 1,
			goblin 	= 2,
			elite   = 3, 		//精英怪
			lansquenet = 4,
	};

	//SoldierBtnScale
#define 	SoldierBtnScale = {
			[1] = 0.8,
			[2] = 0.5,
			[3] = 0.3, 
	};

	//Goblin_Escape_Type
	enum	Goblin_Escape_Type = {
			lose 	= 1,
			win 	= 2,
	};

	//infinite loop  ,cann't push cca.RepeatForver to sequence  in cocos2dx
#define 		REPEATFOREVER = 9999999999
}
#endif // __LYNX_GAME_SERVER_GAME_MANAGER_FIRE_MANAGER_H__
