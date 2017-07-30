#ifndef __LYNX_COMMON_LIB_CONST_DEFINITION_DEFINITION_H__
#define __LYNX_COMMON_LIB_CONST_DEFINITION_DEFINITION_H__

#include "TypeDefinition.h"

#define TCP_MAX_CACHE_ISTREAM_LENGTH  2097152  /* 2 * 1024 * 1024 */
#define TCP_MAX_CACHE_OSTREAM_LENGTH  2097152  /* 2 * 1024 * 1024 */

// 邮件收件人最大数量
#define EMAIL_MULTICAST_MAX_COUNT 50
// 邮件携带道具最大数量
#define EMAIL_ITEM_MAX_COUNT 5
// 邮件文本内容最大长度
#define EMAIL_CONTENT_MAX_LENGTH 200

// 世界聊天最大历史数量
#define WORLD_CHAT_MAX_HISTORY_COUNT 200
// 公会聊天最大历史数量
#define GUILD_CHAT_MAX_HISTORY_COUNT 100
// 玩家聊天最大历史数量
#define PLAYER_CHAT_MAX_HISTORY_COUNT 50
// 客户端聊天历史行数
#define CLIENT_CHAT_HISOTRY_COUNT 24

// 最大好友数量
#define BUDDY_MAX_COUNT 50
// 最大邀请好友数量
#define INVITE_BUDDY_MAX_COUNT 50
// 最大推荐好友数量
#define RECOMMEND_BUDDY_MAX_COUNT 5

// 玩家钻石最大值 (7位)
#define PLAYER_RMB_MAX_VALUE  9999999 

// 玩家经验最大值 (8位整数)
#define PLAYER_EXP_MAX_VALUE 99999999
// 玩家等级最大值
#define PLAYER_LEVEL_MAX_VALUE 999
// 玩家体力值最大值
#define PLAYER_ENERGY_MAX_VALUE 999
// 玩家战力最大值 (8位数)
#define PLAYER_BATTLE_VALUE_MAX_VALUE 99999999
// 玩家金币最大值(42亿）
#define PLAYER_GOLD_MAX_VALUE 4294967295
// 英雄经验最大值
#define HERO_EXP_MAX_VALUE 99999999
// 宠物经验最大值
#define PET_EXP_MAX_VALUE 99999999
// 玩家最大魂力(42亿）
#define PLAYER_SOUL_MAX_VALUE 4294967295
// 玩家最大荣誉
#define PLAYER_HONOR_MAX_VALUE 4294967295
// 英雄等级最大值
#define HERO_LEVEL_MAX_VALUE 999
// 宠物等级最大值
#define PET_LEVEL_MAX_VALUE 999
// 英雄进阶最大值
#define HERO_ADVANCED_MAX_VALUE 9
// 英雄血量最大值（七位数)
#define HERO_HP_MAX_VALUE 9999999
// 英雄星级最大值
#define HERO_STAR_LEVEL_MAX_VALUE 9
// 宠物星级经验最大值
#define PET_STAR_LEVEL_EXP_MAX_VALUE 4294967295
// 英雄怒气最大值
#define HERO_ANGER_MAX_VALUE 999
// 英雄物理攻击最大值（六位）
#define HERO_ATTACK_MAX_VALUE 999999
// 英雄物理防御最大值（六位）
#define HERO_PHYSICS_DEFENSE_MAX_VALUE 999999
// 英雄魔法防御最大值（六位）
#define HERO_MAGIC_DEFENSE_MAX_VALUE 999999
// 英雄暴击最大值（5位）
#define HERO_CRITICAL_MAX_VALUE 99999
// 英雄命中最大值（5位）
#define HERO_HIT_MAX_VALUE 99999
// 英雄闪避最大值（5位）
#define HERO_DODGE_MAX_VALUE 99999
// 英雄血量上线最大值（7位）
#define HERO_HP_UPPER_MAX_VALUE 9999999
// 英雄怒气上线最大值
#define HERO_ANGER_UPPER_MAX_VALUE 999
// 英雄耐力最大值
#define HERO_ENDURANCE_MAX_VALUE 99999 
// 英雄智力最大值
#define HERO_INTELLIGNECE_MAX_VALUE 99999
// 英雄力量最大值
#define HERO_POWER_MAX_VALUE 99999
// 英雄抗暴最大值
#define HERO_RESILIENCE_MAX_VALUE 99999 
// 消耗记录英雄类型
#define CONSUME_RECORD_TYPE_HERO    1
// 消耗记录类型道具
#define CONSUME_RECORD_TYPE_ITEM    2

#define  HURT_AIR_FALL_SPEED  0

#define  SlowMotionFactor  0.5 // 相对于正常速度的系数 ，正常速度为 1 
#define SlowMotionDuration  2 * SlowMotionFactor // 慢动作持续时间，到时间恢复正常
//跳跃相关

#define JUMP_V0  200
#define JUMP_KEEP_V  -1
#define HURT_DELAY_TIME 500 // 0.05 空中受击停留时间
#define BOUNCE_MIN_SPEED  -150
#define BOUNCE_DECREASE_RATE  0.2

//连击
#define HIT_RECORD_MIN_TIME  3
#define HIT_CHANGE_MAX_TIME  0.1
#define HIT_TICK_TIME  0.01

//货币最大值
#define MAX_MONEY  999999
#define MAX_RMB  999999

//场景tick
#define SCENE_TICK_TIME  0.05

//掉落飞行时间
#define DROP_BEFORE_FLY_DELAY_BASE_TIME  0.75
#define DROP_FLY_TIME  0.5
//掉落飞行Y轴偏移
#define DROP_FLY_OFFSETY  20

//摇杆事件频率
#define JOYSTICK_TIME_INTERVAL  0.05 //摇杆发出事件的频率

//玩家移动速度
#define DEFAULT_HERO_MOVE_SPEED  750       //游戏中默认英雄移动的速度（当英雄移动属性为0时才找这里的值进行运算）
//击飞时飞行速度
#define HURT_FLY_SPEED  2200
#define HURT_FLY_WALL_BOUNCE_TIME  0.15
#define HURT_FLY_WALL_BOUNCE_OFFSETX  40

//死亡相关
#define DIE_TO_FADEOUT_DELAY_TIME  1.5
#define DIE_FADEOUT_TIME  1

//受打击时变色时间
#define HURT_FLASH_KEEPTIME  0.25
//受打击时push位移时间
#define HURT_PUSH_MOVE_TIME  0.01
//在空中受打击时给的空中下落速度
#define HURT_AIR_FALL_SPEED  0

//震动相关参数
#define SHAKE_CHAR_OFFSET  2
#define SHAKE_CHAR_TIME  0.01
#define SHAKE_SCREEN_OFFSET  3.5
#define SHAKE_SCREEN_TIME  0.12

//shuffle tiles 参数
#define SHUFFLE_CHAR_TIME  0.5
#define SHUFFLE_CHAR_SIZE  cc.size(200, 200)
#define SHUFFLE_CHAR_TO_FADEOUT_TIME  0.3

//hp低于此值,系统进行警告提示 (如果小于1，例如，0.2表示低于百分之二十， 如果大于1，例如20表示当前血量小于20)
#define SYSTEM_ALERT_LOW_HP  4000
//--怪物在无攻击目标时候 行为改变时间间隔
#define MONSTER_PATROL_INTERVAL  5

//--屏幕溅墨最随机最大值（有2个动画，最大值是5，则产生动画的几率是 2/5 ）
#define SCREEN_INK_TOTAL_NUMBER  5
//--屏幕溅墨消失时间
#define SCREEN_INK_FADEOUT_TIME  2
//--屏幕溅墨每次叠加效果数量
#define SCREEN_INK_EACH_USE_COUNT  5

//--摇杆下拉变换技能按钮内容时播放的特效id
#define JOYSTICK_CHANGE_SKILL_EFFECT_ID  20
//--怪物被击碎时播放的碎裂动画
#define MONSTER_EXPLODE_EFFECT_ID  21

//--背包整理cd时间
#define BAG_ORDER_TIME  5
//--背包格子的数目
#define BAG_ITEM_NUM  100 

#define ITEM_GOLD_TEMPLATEID        101
#define ITEM_DIAMOND_TEMPLATEID     102
#define ITEM_SOUL_TEMPLATEID        103
#define ITEM_ENERGY_TEMPLATEID      104
//服务器公共数据存档时间毫秒
#define SAVE_INV_TIME      5000
//服务器心跳检测重置，毫秒
#define RESET_INV_TIME    1000
//服务器在线人数计数，毫秒
#define SAVE_ONLINE_NUM_TIME      180000
//服务器在线奖励检测重置，毫秒
#define CHECK_ONLEWELFARE_TIME    60000

// 活动模板ID
#define Activity_Setting_Rune_TemplateID 1
#define Activity_Setting_Gold_TemplateID 2
#define Activity_Setting_Enchant_TemplateID 3
#define Activity_Setting_Pet_TemplateID 4
#define Activity_Setting_StoreHouse_TemplateID 5
#define Activity_Setting_TemplateID_Max 5

#define ITEM_EQUIP_TYPEID		 1
#define ITEM_ITEMS_TYPEID		 2
#define ITEM_DIAMOND_TYPEID		 3
#define ITEM_MATERIAL_TYPEID	 4
#define	ITEM_GIFT_TYPEID		 5
#define ITEM_TREASUREBOX_TYPEID	 6



namespace Lynx
{
    enum HeroPersentPos
    {
        HERO_PRESENT_POS_BAG                 = 0,
        HERO_PRESENT_POS_LEFT                = 1,
        HERO_PRESENT_POS_MIDDLE              = 2,
        HERO_PRESENT_POS_RIGHT               = 3,
        HERO_PRESENT_POS_ALTERNATE_1         = 4,
        HERO_PRESENT_POS_ALTERNATE_2         = 5,
    };

    enum PassportAuthStatus
    {
        PASSPORT_AUTH_INVALID           = 0,
        PASSPORT_AUTH_SUCCESS           = 1,
        PASSPORT_AUTH_ACCOUNT_NOT_EXIT  = 2,
        PASSPORT_AUTH_PASSWORD_ERROR    = 3,
    };

    //背包道具栏位
    enum BagItemField
    {
        BAG_ITEM_FIELD_NIVALID          = 0,
        BAG_COMMON_ITEM_FIELD           = 1,
        BAG_EQUIP_ITEM_FIELD            = 2,
        BAG_EQUIP_DEBRIS_ITEM_FIELD     = 3,
        BAG_HERO_DEBRIS_ITEM_FIELD      = 4,
    };

    // 道具使用效果类型
    enum ItemUseEffect
    {
        ITEM_USE_INVALID                    = 0,
        ITEM_USE_TRUASURE_CHESTS            = 1,    // 宝箱
        ITEM_USE_GIFT_BAG                   = 2,    // 礼包
        ITEM_USE_KEY                        = 3,    // 钥匙
        ITEM_USE_TREASURE_WITHOUT_KEY       = 4,    // 宝箱(不需要钥匙开启)
        ITEM_USE_PET_EXP                    = 5,    // 宠物饲料
        ITEM_USE_COMBINE_EQUIP              = 6,    // 合成装备
        ITEM_USE_ADD_ENERGY                 = 7,    // 获得体力
        ITEM_USE_ADD_DIAMOND                = 8,    // 获得钻石
        ITEM_USE_ADD_GOLD                   = 9,    // 获得金币
        ITEM_USE_ADD_SPRING_EXP             = 10,   // 获得经验泉水经验
        ITEM_USE_CHANGE_PLAYER_NAME         = 11,   // 改名
        ITEM_USE_ADD_SOUL                   = 12,   // 获得灵力
    };

    enum EnchantType
    {
        // 攻击力
        ENCHANT_ATTACK_TYPE               = 1,
        // 物理防御
        ENCHANT_PHYSICS_DEFENSE_TYPE      = 2,
        // 魔法防御
        ENCHANT_MAGIC_DEFENSE_TYPE        = 3,
        // 暴击
        ENCHANT_CRITICAL_TYPE             = 4,
        // 命中
        ENCHANT_HIT_TYPE                  = 5,
        // 闪避
        ENCHANT_DODGE_TYPE                = 6,
        // 血量上限
        ENCHANT_HP_TYPE                   = 7,
        // 怒气上限
        ENCHANT_ANGER_TYPE                = 8,
    };

    enum MonsterType
    {
        // 普通怪
        MONSTER_COMMON    = 1,
        // BOSS
        MONSTER_BOSS      = 2,

		MONSTER_NPC 	  = 3,	////npc
    };

    enum ItemSourceType
    {
        ITEM_SOURCE_INVALID             = 0,
        ITEM_SOURCE_GM_COMMAND          = 1,
        ITEM_SOURCE_SYSTEM_AWARD        = 2,
    };

    enum TollgateDifficulty
    {
        TOLLGATE_INVALID                = 0,
        TOLLGATE_EASY                   = 1,
        TOLLGATE_NORMAL                 = 2,
        TOLLGATE_HARD                   = 3,
    };

    enum ItemType
    {
        EQUIP_TYPE        = 1,
        ITEM_TYPE         = 2,
        HERO_TYPE         = 3,
        HERO_DEBRIS_TYPE  = 4,
        PET_TYPE          = 5,
        RUNE_TYPE         = 6,
        PET_DEBRIS_TYPE   = 7,
        EQUIP_DEBRIS_TYPE   = 8,
    };

    enum StoreItemLimitNumType
    {
        STORE_ITEM_LIMIT_NUM_NONE   = 0,
        STORE_ITEM_LIMIT_NUM_DAILY  = 1,
        STORE_ITEM_LIMIT_NUM_CAREER = 2,
    };

    enum StoreItemLimitTimeType
    {
        STORE_ITEM_LIMIT_TIME_NONE      = 0,
        STORE_ITEM_LIMIT_TIME_SELL      = 1,
        STORE_ITEM_LIMIT_TIME_DISCOUNT  = 2,
    };

    enum StorItemType
    {
        STORE_ITEM_TYPE_INVALID         = 0,
        STORE_ITEM_TYPE_ITEM            = 1,
        STORE_ITEM_TYPE_GIFTBAG         = 2,
    };

    enum SystemAwardType
    {
        SYSTEM_AWARD_TYPE_INVALID           = 0,
        SYSTEM_AWARD_TYPE_CONTINOUS_LOGIN   = 1,
        SYSTEM_AWARD_TYPE_LEVEL             = 2,
        SYSTEM_AWARD_TYPE_ACCUMULATE_LOGIN  = 3,
    };

    enum BattleCombatType
    {
        BATTLE_CLOSE    = 1,
        BATTLE_FAR      = 2,
    };

    enum BattleRelationship
    {
        BATTLE_FRIEND   = 1,
        BATTLE_FOE      = 2,
    };

    enum BattleCharacterType
    {
        BATTLE_MONSTER  = 1,
        BATTLE_HERO     = 2,
        BATTLE_PET      = 3,
    };

    // 技能攻击类型，对位，随机单体，随机2个，全体，血最多的，血最少的类型
    enum SkillAttackType
    {
        SKILL_ONLY_POSITION     = 1,
        SKILL_RANDOM_ONE        = 2,
        SKILL_RANDOM_TWO        = 3,
        SKILL_ALL               = 4,
        SKILL_MOST_BLOOD_ONE    = 5,
        SKILL_LEAST_BLOOD_ONE   = 6,
    };

    // 技能敌我
    enum SkillRelationship
    {
        SKILL_FOE       = 1,
        SKILL_FRIEND    = 2,
    };



    enum BattleActorPosType
    {
        BATTLE_ENEMY_LEFT       = 1,
        BATTLE_ENEMY_MIDDLE     = 2,
        BATTLE_ENEMY_RIGHT      = 3,
        BATTLE_ENEMY_PET        = 4,
        BATTLE_PLAYER_LEFT      = 5,
        BATTLE_PLAYER_MIDDLE    = 6,
        BATTLE_PLAYER_RIGHT     = 7,
        BATTLE_PLAYER_PET       = 8,
    };

    enum BufferType
    {
        BUFFER_TYPE_BUFF    = 1,
        BUFFER_TYPE_DEBUFF  = 2,
    };

    //0.无条件，适用于主动技能buff
    //1.自身闪避攻击触发
    //2.自身被击触发
    //3.自身单次攻击杀死目标触发
    //4.自身攻击暴击触发
    enum BufferTriggerType
    {
        BUFFER_TRIGGER_ALL            = 0,
        BUFFER_TRIGGER_DODGE          = 1,
        BUFFER_TRIGGER_BEHIT          = 2,
        BUFFER_TRIGGER_KILL_IN_ONCE   = 3,
        BUFFER_TRIGGER_CRITICAL       = 4,
    };
    /*
    1，生命
    2，攻击力
    3，物理防御
    4，魔法防御
    5，暴击率
    6，抗暴率
    7，命中
    8，闪避
    9，伤害减免
    10，吸血
    11，治疗
    12，暴击伤害
    */
    enum BufferPropertyType
    {
        BUFFER_PROPERTY_HP               = 1,
        BUFFER_PROPERTY_ATTACK           = 2,
        BUFFER_PROPERTY_PHYDEF           = 3,
        BUFFER_PROPERTY_MAGDEF           = 4,
        BUFFER_PROPERTY_CRITICAL         = 5,
        BUFFER_PROPERTY_RESCRIT          = 6,
        BUFFER_PROPERTY_HIT              = 7,
        BUFFER_PROPERTY_DODGE            = 8,
        BUFFER_PROPERTY_DAMAGE_RED       = 9,
        BUFFER_PROPERTY_SUCK_BLOOD       = 10,
        BUFFER_PROPERTY_CURE             = 11,
        BUFFER_PROPERTY_CRITICAL_DAMAGE  = 12,
    };

    /*
    1，燃烧（行动前，受到火属性魔法伤害，攻击力百分比）
    2，中毒（行动前，受到水属性魔法伤害，攻击力百分比）
    3，流血（行动前，受到物理伤害，攻击力百分比）
    4，治疗（行动前，受到生命恢复，攻击力百分比）
    5，麻痹（行动前，几率跳过行动流程，跳过行动的几率）
    6，致盲（攻击miss率，在普通的命中暴击前计算，miss几率）（攻击前）
    7，集火（对方单体攻击或随机目标攻击时，作为优先目标，无参数）（选目标前）
    8，沉默（无法使用自动技能，只能使用普攻，无参数）（攻击前）
    9，物理无敌（免疫物理攻击的伤害和流血伤害，无参数）（被击前）
    10，魔法无敌（免疫魔法攻击的伤害和燃烧，中毒的伤害，免疫麻痹，集火，沉默，致盲，封印效果）（被击前）
    11，封印，被封印者普攻无法获得怒气（攻击后）
    12，反弹攻击者伤害百分比的伤害（被动）（被击前）
    13，自身恢复自身生命上限百分比的生命（被动）（立即生效）
    14，净化（效果值对应buffer类型，删除可清楚的相应类型buffer）（立即生效）
    15，减怒气（立即生效）
    16，加怒气（立即生效）
    */
    enum BufferEffectType
    {
        BUFFER_EFFECT_BURN               = 1,
        BUFFER_EFFECT_POISONING          = 2,
        BUFFER_EFFECT_BLEED              = 3,
        BUFFER_EFFECT_CURE               = 4,
        BUFFER_EFFECT_PARALYSIS          = 5,
        BUFFER_EFFECT_BLIND              = 6,
        BUFFER_EFFECT_CONVERGED          = 7,
        BUFFER_EFFECT_SILENT             = 8,
        //DAMAGE_RED         = 9,
        BUFFER_EFFECT_PHY_INVINCIBLE     = 9,
        BUFFER_EFFECT_MAG_INVINCIBLE     = 10,
        BUFFER_EFFECT_SEAL               = 11,
        BUFFER_EFFECT_REBOUND            = 12,
        BUFFER_EFFECT_SELF_CURE          = 13,
        BUFFER_EFFECT_CLEAN              = 14,
        //SUCK_BLOOD         = 16,
        BUFFER_EFFECT_ANGER_RED          = 15,
        BUFFER_EFFECT_ANGER_INC          = 16,
    };

    // 目标类型，自己，目标
    enum BufferTargetType
    {
        BUFFER_TARGET_SELF   = 1,
        BUFFER_TARGET_TARGET = 2,
    };

    // 成就类型
    enum AchievementType
    {
        // 总战力
        ACHIEVEMENT_TYPE_BATTLE_VALUE               = 1,
        // 拥有4星英雄数
        ACHIEVEMENT_TYPE_OWN_STAR_FOUR_HERO_CNT     = 2,
        // 拥有5星英雄数
        ACHIEVEMENT_TYPE_OWN_STAR_FIVE_HERO_CNT     = 3,
        // 拥有6星英雄数
        ACHIEVEMENT_TYPE_OWN_STAR_SIX_HERO_CNT      = 4,
        // 英雄强化最高等级
        ACHIEVEMENT_TYPE_HERO_REFINE_LEVEL          = 5,
        // 一个英雄觉醒等级
        ACHIEVEMENT_TYPE_ONE_HERO_AWAKEN_LEVEL      = 6,
        // 两个英雄觉醒等级
        ACHIEVEMENT_TYPE_TWO_HERO_AWAKEN_LEVEL      = 7,
        // 三个英雄觉醒等级
        ACHIEVEMENT_TYPE_THREE_HERO_AWAKEN_LEVEL    = 8,
        // 四个英雄觉醒等级
        ACHIEVEMENT_TYPE_FOUR_HERO_AWAKEN_LEVEL     = 9,
        // 五个英雄觉醒等级
        ACHIEVEMENT_TYPE_FIVE_HERO_AWAKEN_LEVEL     = 10,
        // 合成英雄数量
        ACHIEVEMENT_TYPE_COMBINE_HERO_CNT           = 11,
        // 拥有蓝色装备数量
        ACHIEVEMENT_TYPE_OWN_EQUIP_BLUE_CNT         = 12,
        // 拥有紫色装备数量
        ACHIEVEMENT_TYPE_OWN_EQUIP_PURPLE_CNT       = 13,
        // 拥有橙色装备数量
        ACHIEVEMENT_TYPE_OWN_EQUIP_ORANGE_CNT       = 14,
        // 拥有红色装备数量
        ACHIEVEMENT_TYPE_OWN_EQUIP_RED_CNT          = 15,
        // 装备总强化次数
        ACHIEVEMENT_TYPE_REFINE_EQUIP_CNT           = 16,
        // 装备最高强化等级
        ACHIEVEMENT_TYPE_EQUIP_MAX_REFINE_LEVEL     = 17,
        // 装备最高附魔等级
        ACHIEVEMENT_TYPE_ONE_EQUIP_ENCHANT_LEVEL    = 18,
        // 两件装备达到附魔等级
        ACHIEVEMENT_TYPE_TWO_EQUIP_ENCHANT_LEVEL    = 19,
        // 五件装备达到附魔等级
        ACHIEVEMENT_TYPE_FIVE_EQUIP_ENCHANT_LEVEL   = 20,
        // 十件装备达到附魔等级
        ACHIEVEMENT_TYPE_TEN_EQUIP_ENCHANT_LEVEL    = 21,
        // 合成装备数量
        ACHIEVEMENT_TYPE_COMBINE_EQUIP_CNT          = 22,
        // 神兽达到最高等级
        ACHIEVEMENT_TYPE_PET_LEVEL                  = 23,
        // 神兽达到最高星级
        ACHIEVEMENT_TYPE_PET_STAR_LEVEL             = 24,
        // 神兽洗天赋船数
        ACHIEVEMENT_TYPE_PET_RESET_TALENT_CNT       = 25,
        
        // 普通关卡通关章节
        ACHIEVEMENT_TYPE_CLEAR_NORMAL_TOLLGATE_CNT  = 51,
        // 精英管卡通关章节
        ACHIEVEMENT_TYPE_CLEAR_HARD_TOLLGATE_CNT    = 52,
        // 获得星级评价总星数
        ACHIEVEMENT_TYPE_GET_TOLLGATE_STAR_CNT      = 53,
        // 竞技场总参与次数
        ACHIEVEMENT_TYPE_JION_ARENA_CNT             = 54,
        // 竞技场连胜次数
        ACHIEVEMENT_TYPE_ARENA_WIN_STREAK_CNT       = 55,
        // 黄金岛冒险总伤害
        ACHIEVEMENT_TYPE_GOLD_ISLAND_TOTAL_DAMAGE   = 56,
        // 附魔大师冒险首次通关
        ACHIEVEMENT_TYPE_DSMX_FIRST_CLEAR           = 57,
        // 神兽领地冒险首次通关
        ACHIEVEMENT_TYPE_SSLD_FIRST_CLEAR           = 58,
        // 埋符之地冒险首次通关
        ACHIEVEMENT_TYPE_MFZD_FIRST_CLEAR           = 59,
        // 巴别塔冒险通关层数
        ACHIEVEMENT_TYPE_BABEL_TOWER_CLEAR_FLOOR    = 60,
        // 宝藏封印通关章节
        ACHIEVEMENT_TYPE_BZFY_CLEAR_TOLLGATE_CNT    = 61,

        // 总抽卡次数
        ACHIEVEMENT_TYPE_DRAW_CARD_CNT              = 101,
        // 十连抽次数
        ACHIEVEMENT_TYPE_TEN_DRAW_CARD_CNT          = 102,
        // 金矿达到等级
        ACHIEVEMENT_TYPE_GOLD_MINE_LEVEL            = 103,
        // 钻石矿达到等级
        ACHIEVEMENT_TYPE_DIAMOND_MINE_LEVEL         = 104,
        // 魔力之泉打到等级
        ACHIEVEMENT_TYPE_EXP_SPRING_LEVEL           = 105,
        // 累计获得金币
        ACHIEVEMENT_TYPE_GET_TOTAL_GOLD             = 106,
        // 累计获得钻石
        ACHIEVEMENT_TYPE_GET_TOTAL_DIAMOND          = 107,
        // 累计获得灵力
        ACHIEVEMENT_TYPE_GET_TOTAL_SOUL             = 108,
        // 累计获得荣誉
        ACHIEVEMENT_TYPE_GET_TOTAL_HONOR            = 109,
        // 图鉴神圣属性英雄达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_HOLY_CNT   = 110,
        // 图鉴黑暗属性英雄达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_DARK_CNT   = 111,
        // 图鉴自然属性英雄达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_NATURE_CNT = 112,
        // 图鉴火属性英雄达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_FIRE_CNT   = 113,
        // 图鉴雷属性英雄达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_THUNDER_CNT    = 114,
        // 图鉴水属性英雄达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_WATER_CNT      = 115,
        // 图鉴武器装备达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_WEAPON_CNT    = 116,
        // 图鉴头盔装备达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_HAT_CNT       = 117,
        // 图鉴护甲装备达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_ARMOR_CNT     = 118,
        // 图鉴护腿装备达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_CUISH_CNT     = 119,
        // 图鉴饰品装备达到数量
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_ORNAMENT_CNT  = 120,
        // 拥有好友个数
        ACHIEVEMENT_TYPE_BUDDY_CNT                  = 121,
        // 赠送体力次数
        ACHIEVEMENT_TYPE_SENDOUT_ENERGY_CNT         = 122,
        // 加入或者创建公会
        ACHIEVEMENT_TYPE_OWN_GUILD                  = 123,
    };

    enum AchievementStatus
    {
        // 成就未完成
        ACHIEVEMENT_STATUS_UNDERWAY                 = 0,
        // 成就达成已领奖
        ACHIEVEMENT_STATUS_COMPLETE                 = 1,
        // 成就打成未领奖
        ACHIEVEMENT_STATUS_ACCEPT_AWARD             = 2,
    };

    enum TowerBufferType
    {
        TOWER_BUFFER_HP                  = 1,
        TOWER_BUFFER_ATTACK              = 2,
        TOWER_BUFFER_CRITICAL            = 3,
        TOWER_BUFFER_RESCRIT             = 4,
        TOWER_BUFFER_HIT                 = 5,
        TOWER_BUFFER_DODGE               = 6,
        TOWER_BUFFER_DAMAGE_RED          = 7,
    };

    enum TowerPassConditionType
    {
        TOWER_PASS_CONDITION_WIN             = 1,
        TOWER_PASS_CONDITION_WIN_IN_ROUND    = 2,
        TOWER_PASS_CONDITION_DEATH_COUNT     = 3,
        TOWER_PASS_CONDITION_REMAIN_HP       = 4,
    };

    // 金矿每秒产出
    const UInt32 MITERAL_GOLD_OUTPUT_PERSEC         = 100;
    // 钻石每秒产出
    const UInt32 MITERAL_DIAMOND_OUTPUT_PERSEC      = 100;
    // 泉水经验每秒产出
    const UInt32 SPRING_EXP_OUTPUT_PERSEC           = 100;
    // 酒馆体力每秒产出
    const UInt32 TAVERN_ENERGY_OUTPUT_PERSEC        = 100;
    // 金矿加速倍率
    const float GOLD_SPEED_UP_LEVEL                 = 1.0;
    // 钻石加速倍率
    const float DIAMOND_SPEED_UP_LEVEL              = 1.0;
    // 泉水经验加速倍率
    const float EXP_SPEED_UP_LEVEL                  = 1.0;
    // 酒馆体力加速倍率
    const float ENERGY_SPEED_UP_LEVEL               = 1.0;


    // 背包最大数量
    const UInt8 BAG_MAX_SIZE                        = 99;
    const UInt32 ITEM_MAX_COUNT                     = 999999;

    // 英雄装备最大位置
    const UInt8 PLAYER_EQUIP_MAX_POSITION           = 25;
    // 单个英雄装备最大数量 
    const UInt8 PLAYER_EQUIP_PER_HERO_MAX_COUNT     = 5;
    // 符文装备最大位置
    const UInt8 PLAYER_RUNE_MAX_POSITION            = 25;
    // 单个英雄装备符文最大数量
    const UInt8 PLAYER_RUNE_PER_HERO_MAX_COUNT      = 5;
    // 装备强化最大等级
    const UInt32 EQUIP_REFINE_MAX_LEVEL             = 160;
    // 玩家关卡数量最大值
    const UInt32 TOLLGATE_MAX_COUNT                 = 300;

    // 神秘商店一天刷新次数
    const UInt8 SECRET_STORE_RESET_TIMES_DAILY      = 12;
    // 神秘商店商品数量
    const UInt8 SECRET_STORE_ITEM_MAX_COUNT         = 6;

    // 排行版显示玩家个数
    const UInt32 PLAYER_COUNT_ON_RANK               = 50;

    // 竞技场最大玩家数量
    const UInt32 ARENA_PLAYER_MAX_COUNT             = 100000;
    // 竞技场最大机器人数量
    //const UInt32 ARENA_ROBOT_MAX_COUNT              = 10000;
    const UInt32 ARENA_ROBOT_MAX_COUNT              = 19;
    // 竞技场最大挑战次数
    const UInt32 ARENA_BATTLE_MAX_COUNT             = 10;

    // 玩家随机姓氏最大数量
    const UInt32 RANDOM_FIRST_NAME_MAX_COUNT        = 10;
    // 玩家随机名最大数量
    const UInt32 RANDOM_SECOND_NAME_MAX_COUNT       = 10;


    // 连续签到天数上限
    const UInt8 CONTINOUS_LOGIN_AWARD_MAX_DATE_NUM  = 7;

    // 关卡每日刷新时间
    const UInt32 TOLLGATE_BATTLE_COUNT_RESET_TIME   = 3600 * 5;
    // 竞技场每日刷新时间
    const UInt32 ARENA_BATTLE_COUNT_RESET_TIME      = 3600 * 22;
    // 同一类型成就最大数量
    const UInt8 ACHIEVEMENT_TYPE_MAX_COUNT          = 50;
    // 成就最大总量
    const UInt8 ACHIEVEMENT_MAX_COUNT               = 150;
    // 宝藏每日刷新时间
    const UInt32 STORE_HOUSE_BATTLE_COUNT_RESET_TIME   = 3600 * 5;

    // 爬塔活动最大重置次数
    const UInt8 TOWER_RESET_MAX_COUNT               = 10;
    // 爬塔活动重置刷新时间
    const UInt32 TOWER_RESET_TIME                   = 3600 * 5;
    // 爬塔功能开启等级限制
    const UInt8 TOWER_OPEN_LEVEL_LIMIT              = 1;
    // 爬塔最大层数
    const UInt8 TOWER_FLOOR_MAX_COUNT               = 30;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//玩家装备的最大技能数量 
#define MAXEQUIPSKILL 4
#define MAXSKILLLIST  10
#define MAXBUFFERCOUNT 20
//背包最大数量
#define MAXITEMCOUNT   200
#define MAXCONSYMBOLCOUNT 8
//玩家最大时装数量
#define MAXFASHIONCOUNT  10

//玩家装备携带的最大宝石数量

#define MAXGEMCOUNT  5

//装备最大的固有属性数量
#define MAXINTRINSICATTRCOUNT  4

//装备最大的随机属性数量
#define MAXRANDOMATTRCOUNT  5

//物品的参数数组大小

#define PARAMSIZE 8

//每一章节最大关卡数
#define CHAPTERSTAGECOUNT 20
//强制断开客户端次数，时间是 100*30s
#define KEEP_ALIVE_COUNT  40

enum ItemType
{
	EQUIPTYPEID = 1, 
	GEMTYPEID = 3,
	ENHANCETYPEID = 30003,
	COSTTYPEID = 4,
	SYMBOLTYPEID =5,
	TREASUREBOXTYPEID = 6,
	PRESENTPACKAGETYPEID = 7,
	MEDICINETYPEID = 8,
	EQUIPPIECETYPEID = 9,
	HOARSTONEPIECETYPEID = 10,
	OTHERTYPEID = 11
};

enum gemSubType
{
	SubTypeRed = 1,
	SubTypeYellow = 2,
	SubTypeGreen = 3,
	SubTypeBlue = 4,
	SubTypePurple = 5
};

enum CostSubType
{
	REFRESHSTONE = 1, //装备洗练石
	SWEEPPAPER   = 2//扫荡卷
};

#define PolishStoneID 30004401
#define PolishNeedCoin 10



enum AttributeType 
{
	////max hp
	MaxHP 				= 1,
	////max mp
	MaxMP 				= 2,
	////max sp
	MaxSP				= 3,
	////物理攻击
	PA 					= 4,
	////物理防御
	MA					= 5,
	////法术攻击
	PF 					= 6,
	////法术防御
	MF 					= 7,
	////hp回复速度
	HPRecover			= 8,
	////mp回复速度
	MPRecover			= 9,
	////sp回复速度
	SPRecover 			= 10,
	////攻击速度
	AttSpeed 			= 11,
	////连招间隔
	CombinationAtt 		= 12,
	////移动速度
	MoveSpeed 			= 13,
	////跳起高度
	JumpHeight 			= 14,
	////可连续浮空次数
	FlyTimes 			= 15,
	////sp消耗
	SPDamage			= 16,
	////击退
	AttPush 			= 17,
	////击退防御
	AttPushDef 			= 18,
	////挑起
	AttRaise 			= 19,
	////挑起防御
	AttraiseDef 		= 20,
	////基础僵直
	Stiff				= 21,
	////僵直防御
	StiffDef 			= 22,
	////基础暴击率
	BaseCrit 			= 23,
	////暴击倍率
	Critrate 			= 24,
	////韧性
	BaseToughness 		= 25,
	////眩晕防御(几率)
	VertigoDef 			= 26,
	////眩晕防御(时间)
	VertigoTimeDef 		= 27,

	////减速防御(几率)
	SlowDef 			= 28,
	////减速防御(时间)
	SlowTimeDef 		= 29,
	////中毒防御
	PoisonDef 			= 30,
	////易伤防御
	BloodDef 			= 31,
	////秒杀防御
	KODef 				= 32,
	////击飞
	FloatVal            = 33, 
	////击飞抗性
	Floatdef            = 34,
	////击倒
	Fall                = 35,
	////击倒抗性
	Falldef             = 36,

};

enum EquipGemSlotBit
{
	//装备宝石槽，第一个宝石的位
	Gem1SlotBit = 0x00000001,
	//装备宝石槽，第2个宝石的位
	Gem2SlotBit = 0x00000002,
	//装备宝石槽，第3个宝石的位
	Gem3SlotBit = 0x00000004,
	//装备宝石槽，第4个宝石的位
	Gem4SlotBit = 0x00000008,
	//装备宝石槽，第5个宝石的位
	Gem5SlotBit = 0x00000010,
	//装备宝石槽，第6个宝石的位
	Gem6SlotBit = 0x00000020,

	AllGemLock = 0x00000000

};

enum JewelryOpenBit
{
	//第一个饰品解锁的标志位，解锁0x00000001
	Jewelry1OpenBit = 0x00000001,
	//第二个饰品解锁的标志位
	Jewelry2OpenBit = 0x00000002,
	//第三个饰品解锁标志位
	Jewelry3OpenBit = 0x00000004,

	//第四个饰品解锁标志位
	Jewelry4OpenBit = 0x00000008,

	//第五个饰品解锁标志位
	Jewelry5OpenBit = 0x00000010,

	//第六个饰品解锁标志位
	Jewelry6OpenBit = 0x00000020,

	AllJewelryLock = 0x00000000


};

enum BOCConditionType
{
	NeedCoin = 1,
	PlayerLv = 7,
	HoarStoneLv = 8
};

enum QualityType
{
	White = 1,
	Green = 2,
	Blue  = 3,
	Purple = 4,
	Orange = 5,
	Gold = 6
};

enum DirtyType
{
	NODIRTY = 0,
	ADDDIRTY = 1,
	UPDATEDIRTY = 2,
	DELDIRTY = 3
};


enum OperationType
{
	ADDOPERATION = 0,
	DELOPERATION = 1
};

enum DataIndex
{
	BASEDATAINDEX = 0,
	DAILYRESETINDEX = 1,
	SKILLDATAINDEX = 2,
	BUFFDATAINDEX = 3,
	ITEMDATAINDEX = 4,
	FASHIONDATAINDEX = 5,
	STAGEDATAINDEX = 6,
	RHYMEDATAINDEX = 7,
	HOARSTONEDATAINDEX = 8
};

enum DataBit
{
	BASEDATABIT = 0x00000001,
	DAILYRESETBIT = 0x00000002,
	SKILLDATABIT = 0x00000004,
	BUFFDATABIT = 0x00000008,
	ITEMDATABIT = 0x00000010,
	FASHIONDATABIT = 0x00000020,
	STAGEDATABIT = 0x00000040,
	RHYMEDATABIT = 0x00000080,
	HOARSTONEDATABIT = 0x00000100,
	MATERIALDATABIT =  0x00000200,
	HEROEQUIPDATABIT = 0x00000400,
	GEMDATABIT       = 0x00000800,
	JEWELRYDATABIT   = 0x00001000,
	LOCALDATABIT   = 0x00002000,
	CHAPTERUNLOCKEDTABIT   = 0x00004000,
	BOXCOUNTER   = 0x00008000,
	SERVANTFOODBIT = 0x00010000,
	SERVANTBATTLEBIT = 0x00020000,
	TOWERDATABIT = 0x00040000,
	CHATDATABIT =  0x00080000,
	FOODDATABIT =  0x00100000,
	STRENGTHDATABIT =  0x00200000,
	COUNTERDATABIT =  0x00400000,
	COURAGECHALLENGEDATABIT =  0x00800000,
	RANKGAMEDATABIT =  0x01000000,
	BUYCOINDATABIT =  0x02000000,
	FRIENDBEAPPLYDATABIT =  0x04000000,


	MAXBIT =  0x80000000
};

enum ServantInfoLock
{
	FIRSTLOCK = 0x00000001,
	SECONDLOCK = 0x00000002,
	THIRDLOCK = 0x00000004
};

enum ProcessState
{
	PROCESSFREE = 0,
	PROCESSBUSY = 1
};

// // buff大类型
enum BUFF_TYPE 
{
	BUFF_TYPE_ATTR  		= 1,	//属性
	BUFF_TYPE_STATE  		= 2,	//异常状态
	BUFF_TYPE_DISPLAY   	= 3 	//表现
};
//异常状态类buff子类型
enum BUFF_STATE_SUB_TYPE
{
	BUFF_STATE_SUB_TYPE_VERTIGO  	= 1,	//眩晕
	BUFF_STATE_SUB_TYPE_SLOW  		= 2,	//减速
	BUFF_STATE_SUB_TYPE_POISON  	= 3, 	//中毒
	BUFF_STATE_SUB_TYPE_BLOOD  		= 4,	//易伤
	BUFF_STATE_SUB_TYPE_GOD  		= 5,	//无敌
	BUFF_STATE_SUB_TYPE_ENDURE  	= 6,	//霸体
	BUFF_STATE_SUB_TYPE_MAX			= 7	//最大值
};

enum CHAR_STATE 
{
	CHAR_STATE_IDLE		=	1,		//空闲站立
	CHAR_STATE_RUN		=	2,		//移动
	CHAR_STATE_JUMP		=	3,		//跳跃
	CHAR_STATE_DIE		=	4,		//死亡
	CHAR_STATE_HURT		=	5,		//受击
	CHAR_STATE_FALL		=	6,		//浮空
	CHAR_STATE_VICTORY	=	7,		//胜利
	CHAR_STATE_ATTACK	=	8,		//攻击
	CHAR_STATE_SHOW		= 	9,		//展示
	CHAR_STATE_SHOWBACK	= 	10 		//展示结束
};

enum GROW_TYPE
{
	GROW_TYPE_LINE		 = 1,			// y = ax + b
	GROW_TYPE_PARABOLIC  = 2		// y = ax^2 + bx + c
};



// #define  display_bottom = 0;
// #define GRUOND_Y = display_bottom + 130 		////地面的Y坐标
// #define  SCENE_EDGEX = 1000//关卡长度
// #define  SCENE_EDGEY = 1000//关卡高度
// #define GRAVITY  -2580
// #define GravityDeclineRate  0.01
enum SingleDefine
{
	display_bottom = 0,
	GRUOND_Y =display_bottom + 130 ,
	 FIRST_POSX =  10,
	CHAR_SHADOW = 0,
	SCENE_EDGEX = 10000,
	SCENE_EDGEY = 10000,
	GRAVITY = -2580,
};
#define  FRAMES_PERSECOND = 30

#define PLAYER_TAG = 100

#define JOYSTICK_RADIUS = 35
#define JOYSTICK_LENGTH = 3 					//控制摇杆触摸长度 3*JOYSTICK_RADIUS
#define TOPUI_ZORDER = 1000

#define DEFAULT_PLAYER_CAMP = 2

#define BAG_TABLEVIEW_NUM = 3

// #define CHAR_SHADOW = 0

enum CHAR_SHADOW_CONFIG 
{					////阴影设置
	CHAR_SHADOW_CONFIG_SCALEY  = 5000,            //           //Y轴缩放
	CHAR_SHADOW_CONFIG_OFFSETX  = 0,				//		//X偏移量
	CHAR_SHADOW_CONFIG_OFFSETY = 0,					//	//Y偏移量
	CHAR_SHADOW_CONFIG_OPACITY = 75,			//			//透明度
	CHAR_SHADOW_CONFIG_SKEWX = 0,				//			//倾斜
	CHAR_SHADOW_CONFIG_ROTATE = 15				//			//旋转角度
};
// 
// CHAR_ACTION_SHADOW_CONFIG {
// 	[1]		=	{	//idle			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[2]		=	{	//run			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[3]		=	{	//jump			
// 	offsetX = 0,
// 	offsetY = -200,
// 	rotate = 0
// 	};,
// 	[4]		=	{	//die			
// 	offsetX = 0,
// 	offsetY = -200,
// 	rotate = 0
// 	};,
// 	[5]		=	{	//hurt			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[6]		=	{	//fall		
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 0
// 	};,
// 	[7]		=	{	//victory			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[8]		=	{	//attack			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[9] 	= 	{	//show			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[10] 	= 	{	//showback			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};
// };



enum CHAR_ATTR 
{
	CHAR_ATTR_MaxHP 				= 1,	//max hp
	CHAR_ATTR_MaxMP 				= 2,	//max mp
	CHAR_ATTR_MaxSP				= 3,	//max sp
	CHAR_ATTR_PA 					= 4,	//物理攻击
	CHAR_ATTR_MA 					= 5,	//物理防御
	CHAR_ATTR_PF 					= 6,	//法术攻击
	CHAR_ATTR_MF 					= 7,	//法术防御
	CHAR_ATTR_HPRecover			= 8,    //hp回复速度
	CHAR_ATTR_MPRecover			= 9,    //mp回复速度
	CHAR_ATTR_SPRecover 			= 10,	//sp回复速度
	CHAR_ATTR_AttSpeed 			= 11,	//攻击速度
	CHAR_ATTR_CombinationAtt 		= 12,	//连招间隔
	CHAR_ATTR_speed 			= 13,	//移动速度
	CHAR_ATTR_JumpHeight 			= 14,	//跳起高度
	CHAR_ATTR_FlyTimes 			= 15,	//可连续浮空次数
	CHAR_ATTR_SPDamage			= 16,	//sp消耗
	CHAR_ATTR_AttPush 			= 17,	//击退
	CHAR_ATTR_AttPushDef 			= 18,	//击退防御
	CHAR_ATTR_AttRaise 			= 19,	//挑起
	CHAR_ATTR_AttraiseDef 		= 20,	//挑起防御
	CHAR_ATTR_Stiff				= 21,	//基础僵直
	CHAR_ATTR_StiffDef 			= 22,	//僵直防御
	CHAR_ATTR_BaseCrit 			= 23,	//基础暴击率
	CHAR_ATTR_Critrate 			= 24,	//暴击倍率
	CHAR_ATTR_BaseToughness 		= 25,	//韧性
	CHAR_ATTR_VertigoDef 			= 26,	//眩晕防御(几率)
	CHAR_ATTR_VertigoTimeDef 		= 27,	//眩晕防御(时间)
	CHAR_ATTR_SlowDef 			= 28,	//减速防御(几率)
	CHAR_ATTR_SlowTimeDef 		= 29,	//减速防御(时间)
	CHAR_ATTR_PoisonDef 			= 30,	//中毒防御
	CHAR_ATTR_BloodDef 			= 31,	//易伤防御
	CHAR_ATTR_KODef 				= 32,	//秒杀防御
	CHAR_ATTR_FloatVal            = 33,   //击飞
	CHAR_ATTR_Floatdef            = 34,	//击飞抗性
	CHAR_ATTR_Fall                = 35,	//击倒
	CHAR_ATTR_Falldef             = 36	//击倒抗性
};

enum ECHAR_ATTR_KEY 
{
// 	MaxHP			= "MaxHP",		//max hp
// 	MaxMP 			= "MaxMP",		//max mp
// 	MaxSP 			= "MaxSP",		//max sp
// 	PA 				= "PA",			//物理攻击
// 	MA 				= "MA",			//物理防御
// 	PF 				= "PF",			//法术攻击
// 	MF 				= "MF",			//法术防御
// 	HPRecover 		= "HPRecover",  //hp回复速度
// 	MPRecover 		= "MPRecover",  //mp回复速度
// 	SPRecover 		= "SPRecover",	//sp回复速度
// 	AttSpeed 		= "AttSpeed",	//攻击速度
// 	CombinationAtt 	= "CombinationAtt",	//连招间隔
// 	MoveSpeed 		= "MoveSpeed",	//移动速度
// 	JumpHeight 		= "JumpHeight",	//跳起高度
// 	FlyTimes 		= "FlyTimes",	//可击飞次数
// 	SPDamage 		= "SPDamage",	//sp消耗
// 	AttPush 		= "AttPush",	//击退
// 	AttPushDef 		= "AttPushDef",	//击退防御
// 	AttRaise 		= "AttRaise",	//挑起
// 	AttraiseDef 	= "AttraiseDef",//挑起防御
// 	Stiff 			= "Stiff",		//基础僵直
// 	StiffDef 		= "StiffDef",	//僵直防御
// 	BaseCrit 		= "BaseCrit",	//基础暴击率
// 	Critrate 		= "Critrate",	//暴击倍率
// 	BaseToughness 	= "BaseToughness",	//韧性
// 	VertigoDef 		= "VertigoDef",		//眩晕防御(几率)
// 	VertigoTimeDef 	= "VertigoTimeDef",	//眩晕防御(时间)
// 	SlowDef 		= "SlowDef",	//减速防御(几率)
// 	SlowTimeDef 	= "SlowTimeDef",	//减速防御(时间)
// 	PoisonDef 		= "PoisonDef",	//中毒防御
// 	BloodDef 		= "BloodDef",	//易伤防御
// 	KODef 			= "KODef", 		//秒杀防御
// 	FloatVal 		= "FloatVal",   //击飞
// 	Floatdef 		= "Floatdef",	//击飞抗性
// 	Fall 			= "Fall",		//击倒
// 	Falldef 		= "Falldef",	//击倒抗性
// 	[1]		= "MaxHP",			//max hp
// 	[2]		= "MaxMP",			//max mp
// 	[3]		= "MaxSP",			//max sp
// 	[4]		= "PA",				//物理攻击
// 	[5]		= "MA",				//物理防御
// 	[6]		= "PF",				//法术攻击
// 	[7]		= "MF",				//法术防御
// 	[8]		= "HPRecover",    	//hp回复速度
// 	[9]		= "MPRecover",    	//mp回复速度
// 	[10]	= "SPRecover",		//sp回复速度
// 	[11]	= "AttSpeed",		//攻击速度
// 	[12]	= "CombinationAtt",	//连招间隔
// 	[13]	= "MoveSpeed",		//移动速度
// 	[14]	= "JumpHeight",		//跳起高度
// 	[15]	= "FlyTimes",		//可击飞次数
// 	[16]	= "SPDamage",		//sp消耗
// 	[17]	= "AttPush",		//击退
// 	[18]	= "AttPushDef",		//击退防御
// 	[19]	= "AttRaise",		//挑起
// 	[20]	= "AttraiseDef",	//挑起防御
// 	[21]	= "Stiff",			//基础僵直
// 	[22]	= "StiffDef",		//僵直防御
// 	[23]	= "BaseCrit",		//基础暴击率
// 	[24]	= "Critrate",		//暴击倍率
// 	[25]	= "BaseToughness",	//韧性
// 	[26]	= "VertigoDef",		//眩晕防御(几率)
// 	[27]	= "VertigoTimeDef",	//眩晕防御(时间)
// 	[28]	= "SlowDef",		//减速防御(几率)
// 	[29]	= "SlowTimeDef",	//减速防御(时间)
// 	[30]	= "PoisonDef",		//中毒防御
// 	[31]	= "BloodDef",		//易伤防御
// 	[32]	= "KODef", 		    //秒杀防御
// 	[33]	= "FloatVal",   	//击飞
// 	[34]	= "Floatdef",		//击飞抗性
// 	[35]	= "Fall",			//击倒
// 	[36]	= "Falldef",		//击倒抗性
};

// CHAR_ATTR_CHS_STR {
// 	[1]		= "生命上限",
// 	[2]		= "法力上限",
// 	[3]		= "霸体上限",
// 	[4]		= "物理攻击",
// 	[5]		= "物理防御",
// 	[6]		= "法术攻击",
// 	[7]		= "法术防御",
// 	[8]		= "HP回复速度",
// 	[9]		= "MP回复速度",
// 	[10]	= "SP回复速度",
// 	[11]	= "攻击速度",
// 	[12]	= "连招间隔",
// 	[13]	= "移动速度",
// 	[14]	= "跳起高度",
// 	[15]	= "可连续浮空次数",
// 	[16]	= "SP消耗",
// 	[17]	= "击退",
// 	[18]	= "击退防御",
// 	[19]	= "挑起",
// 	[20]	= "挑起防御",
// 	[21]	= "基础僵直",
// 	[22]	= "僵直防御",
// 	[23]	= "基础暴击率",
// 	[24]	= "暴击倍率",
// 	[25]	= "韧性",
// 	[26]	= "眩晕防御(几率)",
// 	[27]	= "眩晕防御(时间)",
// 	[28]	= "减速防御(几率)",
// 	[29]	= "减速防御(时间)",
// 	[30]	= "中毒防御",
// 	[31]	= "易伤防御",
// 	[32]	= "秒杀防御",
// 	[33]	= "击飞",
// 	[34]	= "击飞抗性",
// 	[35]	= "击倒",
// 	[36]	= "击倒抗性",
// };

#define  ATTRIBUTE_COUNT = 36	//属性数量

//角色类型
enum CHARACTER_TYPE 
{
	CHARACTER_TYPE_Player 		= 1,	//玩家
	CHARACTER_TYPE_Monster 	= 2,	//怪
	CHARACTER_TYPE_NPC 		= 3,	//npc
};

//怪物类型
enum MONSTER_TYPE 
{
	MONSTER_TYPE_Normal 		= 1,	//普通怪
	MONSTER_TYPE_Elite 		= 2,	//精英怪
	MONSTER_TYPE_Boss 		= 3,	//boss
	MONSTER_TYPE_Goblin 		= 4		//哥布林
};

//技能类型
enum SKILL_TYPE
{
	SKILL_TYPE_Normal 		= 1,	//普通
	SKILL_TYPE_Passive 	= 2,	//被动
	SKILL_TYPE_Multi 		= 3,	//多段
	SKILL_TYPE_Cumulate 	= 4,	//蓄力
	SKILL_TYPE_Continue 	= 5,	//连续
	SKILL_TYPE_Throw 		= 6,	//投掷
	SKILL_TYPE_RaiseUp 	= 7,	//上挑
	SKILL_TYPE_FallDown 	= 8,	//下压
};

//伤害类型
enum HURT_TYPE 
{
	HURT_TYPE_Default 	= 1,	//默认
};





//加成方式
enum VALUE_EFFECT_TYPE 
{
	VALUE_EFFECT_TYPE_numval = 1,		//按值加成
	VALUE_EFFECT_TYPE_numrate = 2,	//按百分比加成
};

//buff 配置最大参数数量
#define  MAX_BUFF_CONFIG_PARAM_NUM = 8

//buff end

// enum  GROW_TYPE 
// {
// 	GROW_TYPE_line = 1,			// y = ax + b
// 	GROW_TYPE_parabolic = 2,		// y = ax^2 + bx + c
// };


enum CHAR_NODE_PRIORITY
{
	CHAR_NODE_PRIORITY_CHAR 		= 1,
	CHAR_NODE_PRIORITY_WEAPON_L 	= 2,
	CHAR_NODE_PRIORITY_FASHION 	= 3,
	CHAR_NODE_PRIORITY_WEAPON_R 	= 4,
	CHAR_NODE_PRIORITY_EFFECT 		= 5,
};
#define CHAR_NODE_PIC_CNT = 5	// CHAR_NODE_PRIORITY的数量

enum RES_PREFIX_TBL 
{
// 	[1] = "_char",
// 	[2] = "_Lweapon",
// 	[3] = "_clothes",
// 	[4] = "_Rweapon", 
// 	[5] = "_effect", 
};

//击飞过程状态
enum CHARACTER_FLYING_STATE 
{
	CHARACTER_FLYING_STATE_none 		= 0,		//没有击飞
	CHARACTER_FLYING_STATE_startfly 	= 1,		//开始飞
	CHARACTER_FLYING_STATE_touchwall 	= 2,		//撞墙反弹
	CHARACTER_FLYING_STATE_fallground 	= 3 		//反弹后朝地面下落
};

//子弹类型
enum BULLET_TYPE 
{
	BULLET_TYPE_line	= 1,	
	BULLET_TYPE_object	= 2,
};
enum BULLET_DESTORY_TYPE 
{
	BULLET_DESTORY_TYPE_none = 0,
	BULLET_DESTORY_TYPE_outScreen = 1 , //超出屏幕， 
	BULLET_DESTORY_TYPE_stop = 2 ,      //不在移动, 
	BULLET_DESTORY_TYPE_after = 3 ,     //出现一段时间,
	BULLET_DESTORY_TYPE_trigger = 4,    //碰撞后
	BULLET_DESTORY_TYPE_stopAter = 5,   //停止移动一段时间
	BULLET_DESTORY_TYPE_senderGone=6    //发射子弹的本体死亡
};


//场景AI事件
enum SCENEAI_CONDITION_TYPE 
{
	//种怪相关
	SCENEAI_CONDITION_TYPE_monster_seed 			= 1,

	//time
	SCENEAI_CONDITION_TYPEtime_after 				= 101,  	//多久之后
	SCENEAI_CONDITION_TYPEtime_each 				= 102,		//每隔多久
	SCENEAI_CONDITION_TYPEhero_time_use 			= 103, 		//多长时间之后
	//position
	SCENEAI_CONDITION_TYPEpos_pass_id 			= 121, 		//某单位经过某个位置
	SCENEAI_CONDITION_TYPEpos_pass_camp 			= 122,
	SCENEAI_CONDITION_TYPEpos_between 			= 123,

	//attr
	SCENEAI_CONDITION_TYPEattr_atValue_id 		= 141,
	SCENEAI_CONDITION_TYPEattr_changeValue_id 	= 142,
	SCENEAI_CONDITION_TYPEattr_atPercent_id 		= 143,
	SCENEAI_CONDITION_TYPEattr_changePercent_id 	= 144,
	SCENEAI_CONDITION_TYPEattr_val_camp 			= 145, 		//某单位某属性大于、等于、小于某值/百分比
	SCENEAI_CONDITION_TYPEhero_hp_camp 			= 146,		//hero Hp低于某值时触发
	//state
	SCENEAI_CONDITION_TYPEstate_die_id 			= 161,
	SCENEAI_CONDITION_TYPEstate_buff_id 			= 162,
	SCENEAI_CONDITION_TYPEunit_dead 				= 163, 		//某个单位死亡
	SCENEAI_CONDITION_TYPEhero_relive_times 		= 164, 		//玩家复活次数超过某数
	//action
	SCENEAI_CONDITION_TYPEaction_start_id 		= 181,
	SCENEAI_CONDITION_TYPEaction_end_id 			= 182, 		//某个动作结束
	SCENEAI_CONDITION_TYPEaction_end_scene 		= 183,

	//num_camp
	SCENEAI_CONDITION_TYPEmonster_num_camp 		= 191, 		//monster数量大于、等于、小于某值

	// unit_distance
	SCENEAI_CONDITION_TYPEunit_distance 			= 151, 		//两个单位之间距离

	//chapter_progress
	SCENEAI_CONDITION_TYPEchapter_progress 		= 171, 		//关卡进度
};

enum SCENEAI_EVENT_TYPE 
{
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
	bg_monster_move 	= 124, 			//monster在背景层移动
	bg_monster_animation = 125, 		//monster在背景层播放特定动作
	bg_create_monster 	= 126, 			//从背景层种怪
	active_monster 		= 127, 			//激活monster
};


enum SCENEAI_UNIT_ACTION_TYPE
{
	fall2up	    		=	3, //浮空起身
	airhurt	    		=	4, //空中受击
	airfall	    		=	5, //空中下落
	flyback2up			=	6, //反弹后起身
	hurt	    		= 	8, //多个受击动作（hurtbefore）
};


// 角色状态
// enum CHAR_STATE 
// {
// 	idle		=	1,		//空闲站立
// 	run			=	2,		//移动
// 	jump		=	3,		//跳跃
// 	die			=	4,		//死亡
// 	hurt		=	5,		//受击
// 	fall		=	6,		//浮空
// 	victory		=	7,		//胜利
// 	attack		=	8,		//攻击
// 	show 		= 	9,		//展示
// 	showback 	= 	10, 	//展示结束
// };

//角色ai状态
enum AI_STATE 
{
	AI_STATE_none        =   0,      //
	AI_STATE_idle		=	1,		//空闲
	AI_STATE_fan			=	2,		//休闲
	AI_STATE_walk		=	3,		//寻路
	AI_STATE_escape		=	4,		//逃跑
	AI_STATE_follow		=	5,		//跟随
	AI_STATE_patrol		=	6,		//巡逻
	AI_STATE_attack		=	7,		//攻击
	AI_STATE_wander		=	8,		//徘徊
	AI_STATE_hide 		=   9,  	//躲藏
	AI_STATE_show 		=   10,		//展示
	AI_STATE_fall 		= 	11, 	//倒地
	AI_STATE_fixedwalk 	= 	12, 	//向固定点move
};

//技能帧事件
enum FRAME_EVENT_TYPE
{
	FRAME_EVENT_TYPE_move 		=	1,		//移动
	FRAME_EVENT_TYPE_wait		=	2,		//等待
	FRAME_EVENT_TYPE_shake		=	3,		//震动
	FRAME_EVENT_TYPE_attack 		=	4,		//攻击（单一帧）
	FRAME_EVENT_TYPE_attackKeep 			=	5,	//攻击（帧区间）
	FRAME_EVENT_TYPE_mulitCheck_start	=	7,		//段技能连击判定区间开始
	FRAME_EVENT_TYPE_mulitCheck_end		=	8,		//段技能连击判定区间结束并进行判定
	FRAME_EVENT_TYPE_addBuff 	= 	9,		//增加buff
	FRAME_EVENT_TYPE_addBullet	=	10,		//增加子弹
	FRAME_EVENT_TYPE_addMusicEffect		=	11,		//增加音效
	FRAME_EVENT_TYPE_addAniEffect		=	12,		//增加特效
	FRAME_EVENT_TYPE_allowInterrupt		=	13,		//在此帧事件后允许玩家主动打断技能
	FRAME_EVENT_TYPE_ctrlDirect_start	=	14,		//允许控制技能方向开始
	FRAME_EVENT_TYPE_ctrlDirect_end 		=	15,		//允许控制技能方向结束
};

enum JoystickDir 
{
	JOYSTICKDIR_UP = 1,
	JOYSTICKDIR_down = 2,
	JOYSTICKDIR_right = 3,
	JOYSTICKDIR_left = 4,
	JOYSTICKDIR_stop = 5,

};
enum ButtonClick 
{
	ButtonClick_skill1 = 1,
	ButtonClick_skill2 = 2,
	ButtonClick_soldierBtn = 3,
	ButtonClick_attack = 4,
	ButtonClick_pause = 5,
	ButtonClick_jump = 6,
	ButtonClick_direction = 7,
};
enum SkillEquip
{
	SkillEquip_normal_attack = 1,
	SkillEquip_jump_attack =2,
	SkillEquip_up =3,
	SkillEquip_down = 4,

};

//比较符号
enum CompareSign 
{
	CompareSign_eq = 1, // -- 等于
	CompareSign_rt = 2, // -- 大于
	CompareSign_lt = 3,//-- 小于
	CompareSign_rte = 4, //-- 大于等于
	CompareSign_lte = 5, // -- 小于等于
// 	CompareSign_ = "+", //--递增
// 	CompareSign_dec = "-", //--递减
};

//攻击时 被攻击者的状态
//0:站立不变,1:挑到空中,2:打倒在地,3:击飞
enum BeHurtedState 
{
	BeHurtedState_stand = 0,
	BeHurtedState_float = 1, 
	BeHurtedState_fall = 2, 
	BeHurtedState_floatX = 3,
};

enum ANI_KEY_NAME 
{
// 	idle = "idle",
// 	run = "run",
// 	airhurt = "airhurt",
// 	airfall = "airfall",
// 	fall = "fall",
// 	fall2up = "fall2up",
// 	victory = "victory",
// 	die = "die",
// 	flyback = "flyback",
// 	show = "show",
// 	flybackfall = "flybackfall",
// 	flyback2up = "flyback2up",
// 	jumpbefore = "jumpbefore", 
// 	jumpup = "jumpup", 
// 	jumpdown = "jumpdown", 
// 	jumpafter = "jumpafter",
// 	showback = "showback",
// 	showstart = "showstart",
// 	escapewin = "escapewin",
// 	escapelose = "escapelose",
};

//资源类型
enum GAME_RES_TYPE 
{
	GAME_RES_TYPE_coin 		= 1,
	GAME_RES_TYPE_rmb 		= 2,
	GAME_RES_TYPE_strength 	= 3,
	GAME_RES_TYPE_exp 		= 4,
	GAME_RES_TYPE_prestige 	= 5,
	GAME_RES_TYPE_item 		= 6,
};

//关卡激活条件
enum STAGE_ACTIVE_CONDITION 
{
	STAGE_ACTIVE_CONDITION_level 			= 1,
	STAGE_ACTIVE_CONDITION_preFinishStage 	= 2,
	STAGE_ACTIVE_CONDITION_mission 		= 3,
	STAGE_ACTIVE_CONDITION_preFinishStageStarCnt = 4,
	STAGE_ACTIVE_CONDITION_vipLevel		= 5,
	STAGE_ACTIVE_CONDITION_guildLevel		= 6,
};

//关卡类型
enum STAGE_TYPE 
{
	STAGE_TYPE_normal = 1,
	STAGE_TYPE_master = 2,
	STAGE_TYPE_activity = 3,
// 
// 	[1] = "normal",
// 	[2] = "master",
// 	[3] = "activity",
};

//关卡得星条件
enum STAGE_STAR_CONDITION 
{
	STAGE_STAR_CONDITION_useTime 			= 1,
	STAGE_STAR_CONDITION_reviveTimes 		= 2,  //复活次数
	STAGE_STAR_CONDITION_useMedicinetimes	= 3,
	STAGE_STAR_CONDITION_leftHpValue			= 4,
	STAGE_STAR_CONDITION_leftHpRate			= 5,
};
//关卡星数量
#define STAGE_STAR_NUMBER = 3

//每章节最多关卡数量
#define CHAPTER_MAX_STAGE_NUMBER = 20

//扫荡道具种类
#define SWEEP_ITEM_TYPE_NUM = 3

//背包每行的背包格数
#define ITEM_CNT_EACH_ROW = 4

//道具类型
enum ITEM_TYPE 
{
	ITEM_TYPE_equip 			= 1,		//装备
	ITEM_TYPE_gem 			= 3,		//宝石
	ITEM_TYPE_enhance		 	= 30003,	//强化石
	ITEM_TYPE_cost 			= 4, 		//游戏消耗品
	ITEM_TYPE_treasureBox 	= 6,		//宝箱
	ITEM_TYPE_presentpackage 	= 7,		//礼包
	ITEM_TYPE_medicine 		= 8,		//药品恢复类道具
	ITEM_TYPE_equip_piece 	= 9,		//装备碎片
	ITEM_TYPE_material		= 10,		//材料
	ITEM_TYPE_other 			= 11,		//其它游戏道具
};

//宝石子类型
enum ITEM_GEM_SUBTYPE 
{
	ITEM_GEM_SUBTYPE_red 	= 1,	//红
	ITEM_GEM_SUBTYPE_yellow 	= 2,	//黄
	ITEM_GEM_SUBTYPE_green 	= 3,	//绿
	ITEM_GEM_SUBTYPE_blue 	= 4,	//蓝
	ITEM_GEM_SUBTYPE_purple	= 5,	//紫
};

//强化石子类型
enum ITEM_ENHANCE_MINITYPE {
	ITEM_ENHANCE_MINITYPE_weapon 	= 1,	//武器强化石	
	ITEM_ENHANCE_MINITYPE_defence = 2,	//防具强化石
	ITEM_ENHANCE_MINITYPE_jewelry = 3, 	//饰品强化石
};

//游戏消耗品子类型
enum ITEM_COST_SUBTYPE
{
	ITEM_COST_SUBTYPE_enhance 		= 5,		//装备强化石
	ITEM_COST_SUBTYPE_remeltStone 	= 1,		//装备洗炼石
	ITEM_COST_SUBTYPE_sweepPaper		= 2,		//扫荡卷
	// rhymeSoul 		= 3,		//韵魂
	ITEM_COST_SUBTYPE_dungeonTicket	= 4,		//副本入场券
};

//药品恢复类道具子类型
enum ITEM_MEDICINE_SUBTYPE
{
// 	1
};

//其它游戏道具子类型
enum ITEM_OTHER_SUBTYPE 
{
	key_gold	= 1,	//金钥匙
	key_silver	= 2,	//银钥匙
};

//宝箱子类型
enum ITEM_TREASURE_MINITYPE
{
	ITEM_TREASURE_MINITYPE_common_gold		= 100,
	ITEM_TREASURE_MINITYPE_common_silver	= 101,
	ITEM_TREASURE_MINITYPE_common_cu		= 102,
	ITEM_TREASURE_MINITYPE_common_fe		= 103,
	ITEM_TREASURE_MINITYPE_weapon_lv_first = 200,
	ITEM_TREASURE_MINITYPE_weapon_lv_end	= 209,
	ITEM_TREASURE_MINITYPE_defence 	= 300,
	ITEM_TREASURE_MINITYPE_jewelry 	= 400,
	ITEM_TREASURE_MINITYPE_gem 		= 500,
	ITEM_TREASURE_MINITYPE_random 		= 600,
};

//礼包子类型
enum ITEM_PACKAGE_MINITYPE 
{

};

//道具品质
enum ITEM_QUALITY 
{
	ITEM_QUALITY_white 	= 1,	//白
	ITEM_QUALITY_green 	= 2,	//绿
	ITEM_QUALITY_blue 	= 3,	//蓝
	ITEM_QUALITY_purple 	= 4,	//紫
	ITEM_QUALITY_golden 	= 5,	//金
};

//装备点
enum CHARACTER_EQUIP_POS 

{
	CHARACTER_EQUIP_POS_weapon 	= 1,	//武器
	CHARACTER_EQUIP_POS_head	= 2,	//头
	CHARACTER_EQUIP_POS_chest 	= 3,	//胸
	CHARACTER_EQUIP_POS_belt 	= 4,	//腰带
	CHARACTER_EQUIP_POS_leg 	= 5,	//腿
	CHARACTER_EQUIP_POS_foot 	= 6,	//脚
	CHARACTER_EQUIP_POS_jewelry = 7,	//首饰
	CHARACTER_EQUIP_POS_amulet 	= 8,	//护符
};

//装备类型
enum EQUIP_TYPE 
{
// 	[1] = 1, 		//武器
// 	[2] = 2,		//防具
// 	[3] = 2,
// 	[4] = 2,
// 	[5] = 2,
// 	[6] = 2,
// 	[7] = 3,		//饰品
// 	[8] = 3
};

//装备最高星级
#define MAX_EQUIP_STAR_LEVEL = 5

//道具的参数属性数量
#define ITEM_PARAM_NUM = 8

//背包分类
enum BAG_TAB_TYPE 

{
	BAG_TAB_TYPE_equip = 1,
	BAG_TAB_TYPE_item  = 2,
	BAG_TAB_TYPE_other = 3,
// 	[1] = "equip",
// 	[2] = "item",
// 	[3] = "other",
};

// 关卡分类
enum SELECT_TAG_TYPE

{
	SELECT_TAG_TYPE_normal = 1,
	SELECT_TAG_TYPE_master = 2,
// 	[1] = "normal",
// 	[2] = "master",
};
//关卡扫荡道具
enum SELECT_SWEEP_PROPERTY 
{
	SELECT_SWEEP_PROPERTY_cunzheng = 1,                //村正
	SELECT_SWEEP_PROPERTY_power    = 2,                //小体力药
	SELECT_SWEEP_PROPERTY_gem      = 3,                //宝石
// 	[1]      = "cunzheng",
// 	[2]      = "power",
// 	[3]      = "gem",
};
//美食屋开启状态
#define FoodOpenPosition = false
//时装最多生效的属性数量
#define MAX_FASHION_EFFECTIVE_ATTR_COUNT = 5

//界石类型数量
#define HOARSTONE_TYPE_NUM = 12
//界石最大星级
#define HOARSTONE_MAX_STAR = 10
//界石装备（符文）装备槽数
#define HOARSTONE_EQUIP_SLOT_NUM = 6

//韵纹最多阶数
#define RHYME_MAX_STEP_COUNT = 10
//每个游戏角色的韵功总数
#define GAMECHARACTER_RHYME_MAX_SKILL_COUNT = 3
//每阶韵纹最多穴道数
#define STEP_MAX_ACUPOINT_COUNT = 20


//日常关卡类型
enum DAILY_DUNGEON_TYPE
{
	DAILY_DUNGEON_TYPE_coin 	= 1,
	DAILY_DUNGEON_TYPE_exp		= 2,
	DAILY_DUNGEON_TYPE_gem 	= 3,
	DAILY_DUNGEON_TYPE_rhyme 	= 4
};
//日常关卡类型总数
#define DAILY_DUNGEON_TYPE_COUNT = table.nums(DAILY_DUNGEON_TYPE)
//日常关卡难度数量
#define DAILY_DUNGEON_MAX_DIFFICULT = 6

//对应资源名
enum colorRes 
{
// 	ResourceDef.IMAGE_BAG_BTNFRAME_WHITE, 
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_BLUE,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_PURPLE,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GRAY, 
};

enum colorRes_new
{
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE_new,
};

//effect type
enum EFFECT_RES_TYPE
{
	EFFECT_RES_TYPE_particle = 1,
	EFFECT_RES_TYPE_frameAction = 2,
	EFFECT_RES_TYPE_blend = 3,
	EFFECT_RES_TYPE_other = 4,
};

enum EFFECT_PLAY_TYPE
{
	EFFECT_PLAY_TYPE_PLAY_ONCE = 1,
	EFFECT_PLAY_TYPE_PLAY_TIME = 2,
	EFFECT_PLAY_TYPE_PLAY_FOREVER = 3,
	EFFECT_PLAY_TYPE_SPECIAL = 4,
};

enum EFFECT_POS_REFERENCE_TYPE 
{
	EFFECT_POS_REFERENCE_TYPE_Camera = 1,
	EFFECT_POS_REFERENCE_TYPE_MapPos = 2,
	EFFECT_POS_REFERENCE_TYPE_AttackerPos = 3,
	EFFECT_POS_REFERENCE_TYPE_DefencerPos = 4,
};

enum EFFECT_PARTICLE_POSITION_TYPE 
{
	EFFECT_PARTICLE_POSITION_TYPE_free = 1,
	EFFECT_PARTICLE_POSITION_TYPE_grouped = 2,
	EFFECT_PARTICLE_POSITION_TYPE_relative = 3,
};

//refresh event
enum GAME_REFRESH_EVENT 
{
// 	addItem 	= "addItem",
// 	removeItem	= "removeItem",
// 	updateItem	= "updateItem",
// 	equipResult		= "equipResult",
// 	updateFashion	= "updateFashion",
// 	equipFashion 	= "equipFashiion",
// 	coinChange	= "coinChange",
// 	rmbChange	= "rmbChange",
// 	strengthChange	= "strengthChange",
// 	stageStar  = "stageStar",
// 	stageEnd    = "stageEnd",
// 	returnAccounts = "returnAccounts",
// 	refreshItemData = "refreshItemData",
// 	foodData  = "foodData"
};

//佣兵装备槽数
#define MAX_LANSQUENET_EQUIP_POS_NUM = 4

//MonsterType
enum MonsterType 
{
	MonsterType_monster = 0,
	MonsterType_boss 	= 1,
	MonsterType_goblin 	= 2,
	MonsterType_elite   = 3, 		//精英怪
	MonsterType_lansquenet = 4,
};

//SoldierBtnScale
enum SoldierBtnScale 
{
// 	[1] = 0.8,
// 	[2] = 0.5,
// 	[3] = 0.3, 
};

//Goblin_Escape_Type
enum Goblin_Escape_Type 
{
	Goblin_Escape_Type_lose 	= 1,
	Goblin_Escape_Type_win 	= 2,
};

//infinite loop  ,cann't push cca.RepeatForver to sequence  in cocos2dx
#define REPEATFOREVER = 9999999999


//screen shake type

enum ScreenShakeType 
{
	ScreenShakeType_rhomb = 1, 			//菱形
	ScreenShakeType_top_bottom = 2, 	//上下

}; 


//bg monster acton id
enum ActionID 
{
	ActionID_show = 1,
	ActionID_idle = 2,
};

//刷怪类型

enum CraeteMonsterType
{
	CraeteMonsterType_createByAi 		= 1, 		//通过场景事件种到战斗层
	CraeteMonsterType_createByBgAi 	= 2, 		//通过场景事件种到背景层，然后跳到战斗层
	CraeteMonsterType_plantInMap 		= 3, 		//通过编辑器种到地图
};

//种在编辑器中的monster是否激活
enum MonsterActive
{
	MonsterActive_Active = 1,
	MonsterActive_UnActive = 0,
};

//monster 朝向
enum MonsterFace
{
	MonsterFace_faceLeft = 0,
	MonsterFace_faceRight = 1,
};
//背包中tableView类型
enum BagTblType 
{
	BagTblType_Fashion = 1,
	BagTblType_Bag = 2,
};

//锻造系统tableView类型
enum ForgeTblType 
{
	ForgeTblType_Enhance = 1,
	ForgeTblType_Polish = 2,
	ForgeTblType_Inlay	= 3,
};

//enhance_item_num
#define ENHANCE_ITEM_NUM = 4
#define POLISH_ITEM_NUM = 4

//强化系统的数据类型
enum Enhance_Data_Type
{
	Enhance_Data_Type_equip 			= 1, 		//装备
	Enhance_Data_Type_enhanceStone 	= 2, 		//强化石
	Enhance_Data_Type_gem 			= 3,		//宝石
};

//enhance_bottom_alert
enum Enhance_Bottom_Alert 
{
	Enhance_Bottom_Alert_Null 			= 1,
	Enhance_Bottom_Alert_RareQuality 	= 2,
	Enhance_Bottom_Alert_HightAbility 	= 3,
};


//tips btn index
enum TipsBntIndex 
{

};

//gemHeighstTypeId
enum gemHeighstTypeId 
{
// 	[1] = 20001515,
// 	[2] = 20002515,
// 	[3] = 20003515,
// 	[4] = 20004515,
// 	[5] = 20005515,
};

//box leftTop Icon type
enum leftTopIconType
{
	leftTopIconType_equiped 	= 1,
	leftTopIconType_maxLevel 	= 2,
	leftTopIconType_canPolish 	= 3,
	leftTopIconType_recommend	= 4, 	//推荐
	leftTopIconType_canCompose 	= 5,
	leftTopIconType_canEquip 	= 6,
};


// enum ReturnType //返回值
// {
// 	SUCCESS =0,				//成功
// 	NOT_ENOUGH_STRENGTH =1, //缺少体力
// 	NOT_ENOUGH_COIN =2,		//缺少金币
// 	NOT_ENOUGH_GOLD =3,		//缺少元宝
// 	NOT_ENOUGH_TIMES =4,	//次数不足	
// 	OVER_MAX_TIMES =5,		//超过上限次数
// 	NOT_RIGHT_TIME =6,		//不是开放时间
// 	FAIL_OTHER = 20,			//其他失败
// };
enum ReFlashType
{
	REFLASH_ROLE_PROPERTY	 =1,//人物属性
	REFLASH_BACKPACK		 =2,//背包
	REFLASH_EQUIP			 =3,//装备
	REFLASH_CHAPTER			 =4,//关卡
	REFLASH_AWARD			 =5,//结算界面
	REFLASH_BOX				 =6,//箱子
	REFLASH_GIFT			 =7,//礼包
	REFLASH_VIP				 =8,//VIP
	REFLASH_AWARD_CARD		 =9,//翻牌
	REFLASH_LOTTERY			 =10,//翻牌
	REFLASH_WELFALE_MARKET	 =11,//招福集市
	REFLASH_FOOD			 =23,//美食屋
};

enum CopyType
{

	STAGE_NORMAL	 = 1,	//普通
	STAGE_ELITE		 = 2,	//精英2
	STAGE_BONUS		 = 3,	//奖励3
	STAGE_MASTAR	 = 4,	//大师4
	STAGE_SPECIAL	 = 5,	//爬塔5
	STAGE_TRIAL		 = 6,	//多人副本
	STAGE_TWELVEPALACE    = 7,	//十二宗宫
	STAGE_COURAGE    = 10,	//勇气试炼
	STAGE_WELFARE    = 116,	//


};
enum
{
	STAGE_RECORD_TYPE			= 21, //多人关卡
	TWELVE_PALACE_RECORD_TYPE   = 22, //十二宗宫
	TWELVE_PALACE_MY_RECORD		= 23, //十二宗宫个人纪录
	TWELVE_PALACE_RESET_TIMES	= 24, //十二宗宫重置次数

	LOCAL_DATA_SELECT			= 25, //本地信息，入侵者勾选
	CHAPTER_DATA_LOCK			= 26, //章节解锁信息
	
	
};
enum AwardType
{
	AWARD_BASE = 1,//基础物品，铜钱，元宝，韵魂，体力，经验，声望
    AWARD_GRASS = 2,//奖励薄荷草
	AWARD_FOOD = 3,//美食类
	AWARD_WELFAREMARKET = 4,//招福集市
 
	AWARD_HOARSTONEPIECE = 5,//界石碎片
	AWARD_SERVANTMATERIAL = 6, //佣兵消耗道具，包括五种便当和原石
	AWARD_SERVANT = 7,//佣兵
	AWARD_SERVANTPIECE = 8, //佣兵碎片
	AWARD_SERVANTTREASURE = 9,//佣兵宝物
	AWARD_SERVANTSWITCH = 10, //佣兵的生命之光，用于佣兵兑换
    AWARD_ENHANCEMATERIAL = 11, //强化材料
	AWARD_GEM = 12,
	AWARD_JEWELRY = 13,
	AWARD_FASHION = 14,
	AWARD_HOARSTONEEQUIP = 16,//符文
	AWARD_HEROEQUIP = 17,//英雄装备，目前不可获取

	AWARD_CONSORTCONTRIBUTE = 32,





	//........................以下内容不是策划定义获取的物品，只是神仙方便通用添加的
	AWARD_TOWERBUFF = 18,//爬塔buff
	AWARD_TOWERATTR = 19,//爬塔属性
	AWARD_STAGEDATA = 20,//关卡信息
	AWARD_COUNTER = 21,//计数器
	AWARD_REWARDCOUNTER = 22,//奖励计算器
	AWARD_STRENGTHDATA = 24,//体力
	AWARD_TWELVEPALACE_STAGEDATA = 25,//十二宗宫关卡信息
	AWARD_SHOP = 26,//神秘商品
	AWARD_SERVANTSHOP = 27,//佣兵商品
	AWARD_GIFTSHOP = 28,//礼包商品
	
	AWARD_CATCOIN = 29,//勇气试炼硬币
	AWARD_CURRENCY = 30,//通宝
	AWARD_FREETICKET = 31,//抽奖券

	

	AWARD_RANDOM_SERVANTPIECE = 80, //随机佣兵碎片
	AWARD_JEWELRY_SELL = 113,//饰品卖
	AWARD_RANDOM_GEM = 120,//随机宝石

	


};

enum AwardBase
{
	AWARD_BASE_COIN = 1,	//铜钱
	AWARD_BASE_GOLD = 2,	//元宝
	AWARD_BASE_STRENGTH = 3,	//体力
	AWARD_BASE_EXP = 4,	//经验
	AWARD_BASE_COMPETITIVE_POINT = 5,	//竞技点（脸谱）
	AWARD_BASE_RHYME_SOUL = 6,	//韵魂	
	AWARD_BASE_SCORE = 7,	//爬塔积分

	AWARD_BASE_VIP_EXP = 9,	//VIP经验
	AWARD_BASE_VIP_LEVEL = 10,	//VIP等级

	AWARD_BASE_RANKEDSCORE = 20001,	//排位赛积分
	AWARD_BASE_LEVEL = 100	//等级


};
enum RhymeGrass
{
	SmallGrass = 1,
	MidGrass =2,
	LargeGrass = 3

};

enum RobotData
{
	RobotMinRoleID = 1000,
	RobotMaxRoleID = 10000,
	RankGameInitScore = 1000
};

enum AwardTowerAttr
{
	AWARD_TOWERATTR_AP = 1,//攻击力
	AWARD_TOWERATTR_HP =2,//生命值

};


enum ServantMaterial
{
	ServantFood1 = 1,
	ServantFood2 = 2,
	ServantFood3 = 3,
	ServantFood4 = 4,
	ServantFood5 = 5,
	ServantBattleStone = 6
};

//玩家主阵容开启信息
enum ServantMainBatBit
{
	SERVANT_MAIN_NONE =  0x00000000,
	SERVANT_MAIN1_OPEN = 0x00000001,
	SERVANT_MAIN2_OPEN = 0x00000002,
	SERVANT_MAIN3_OPEN = 0x00000004,
	SERVANT_MAIN4_OPEN = 0x00000008,
	SERVANT_MAIN5_OPEN = 0x00000010

};


//玩家助阵阵容开启信息
enum ServantAssistBatBit
{
	SERVANT_ASSIST_NONE =  0x00000000,
	SERVANT_ASSIST1_OPEN = 0x00000001,
	SERVANT_ASSIST2_OPEN = 0x00000002,
	SERVANT_ASSIST3_OPEN = 0x00000004,
	SERVANT_ASSIST4_OPEN = 0x00000008,
	SERVANT_ASSIST5_OPEN = 0x00000010,
	SERVANT_ASSIST6_OPEN = 0x00000020,
	SERVANT_ASSIST7_OPEN = 0x00000040
};


enum ChannelType
{
	WORLDCHAT = 1, //世界
	CONSORTIACHAT = 2,//公会聊天
	PRIVATECHAT = 3,//私聊
	SYSTEMMSG = 4,//系统信息
	GMMSG = 5,//GM消息
	CONSORTMASG = 10 //工会招募
};

enum GlobelVar
{
	MAXJEWELRYCOUNT = 96,
	OFFLINESAVETIME = 120,//离线保存时间120s
	RANKGAMECOPYID = 100000000,//排位赛关卡id

	GETAWARDSCOPYID = 99999999,//获取奖励物品用fireconfirm
	XINSHOUYINDAOSTAGE = 10000,//新手引导关卡服务器不用

};

enum Ticket
{	
	ONCEFREELOTTERYTICKET = 1,
	TENFREELOTTERYTICKET	 = 2,
};

enum Foods
{	
	// 		1	美食烧制次数
	// 			2	全鱼火锅
	// 			3	糖稀鱼丸
	// 			4	鱼尾烧麦
	
	FOODTYPE1 = 1,
	FOODTYPE2 = 2,
	FOODTYPE3 = 3,
	FOODTYPE4 = 4,
};

//成就，关联的事件id
 enum AssocialEvent
 {
	BARRIER = 2001,   //通关关卡
	ELITEBARRIER = 2002,   //通关精英关卡
	RHYMEENHANCELV = 2003,  //韵文强化到指定
	RHYMESKILLACTIVE = 2004,  //韵功激活
	HONORSTONEACTIVE = 2005, //界石激活
	GETSERVANT = 2006, //获得一定数量的佣兵
	PLAYERLV = 2007, //玩家等级达到某数量
	VIPLV = 2008, // vip达到某一级别
	POWER = 2009, //战力到达到数值
	OWN4STARSERVANT = 2010,  //拥有四星佣兵数量
	OWNPURPULESERVANT = 2011, //拥有紫色佣兵数量
	UNLOCKSERVANTASSIS = 2012, //解锁助阵位数量
	OWNGREENASSIST = 2013, //拥有绿色助阵位数量

	OWNBLUEASSIST = 2014,  //拥有蓝色助阵位数量
	OWNPURPLEASSIST = 2015,  //拥有紫色助阵位数量
	QUALIFYDANLV = 2016,  //排位赛段数
	QUALIFYSUCCESS = 2017, //排位赛胜利场次
	ARIEASUCCESS = 2018,  //竞技场胜利场次
	BUYCHARACTOR = 2019,  //购买角色个数
	COURAGESUCCESS = 2020,  //勇气试炼成功次数

	WXCHALLENGESUC = 2021,  //通关某一楼
	USERHYMESKILL = 2022,  //释放韵功次数


	USERHYMESPEED = 2023, //释放韵力激发次数
	CALLSERVANTCNT = 2024,  //战斗中召唤佣兵次数
	FRIENDCNT = 2025,  //好友个数
	OWNLV5GEMCNT = 2026,  //拥有五级宝石个数
	OWNLV10GEMCNT = 2027,  //拥有十级宝石个数
	OWNLV15GEMCNT = 2028,  //拥有十五级宝石个数
	LINGRENCALLSERVANT = 2029, //伶人召唤次数
	XIBANCALLSERVANT = 2030, //戏班子召唤次数
	MINGLINGCALLSERVANT = 2031, //名妓召唤次数
	MAXGEMLV = 2032 //宝石最大等级
 };

 //每日任务关联的事件id
 enum DailyEvent
 {
		DLYBARRIERCNT = 1001,//普通副本通关次数，包含扫荡
		DLYELITEBARRIRECNT    = 1002,//精英副本通关次数,包含扫荡
		DLYLINGRENCALLCNT   = 1003, //伶人招募次数
		DLYXIBANCALLCNT       = 1004, //戏班招募次数
		DLYMULTYSUCCESS      = 1005,  //成功挑战多人副本，包含扫荡
		DLYTELVESUCCESS		= 1006,  //成功挑战12宗宫，包含扫荡
		DLYDOMARKETCNT		= 1007, //进行招福集市挑战，无论胜负
		DLYDOCOURAGECNT	= 1008, //进行勇气试炼挑战，无论胜负
		DLYDOWXCHALLENGE  = 1009, //进行无限挑战次数，无论胜负
		DLYDOQUALIFYCNT		= 1010, //进行排位赛次数，无论胜负
		DLYDOARIEACNT			= 1011,//挑战竞技场中对手次数，无论胜负
		DLYRHYMEENHANCECNT		= 1012, //韵文强化次数
		DLYHORNORSTONELVUPCNT	= 1013, //界石升级次数
		DLYBAICAISHEN                    = 1014, //拜财神次数（免费和付费都算次数）
        DLYBUYENERGE						= 1015,//购买体力
		DLYFOODCOOK						= 1017,//美食屋烹饪
		DLYCONSORTSIGN					= 1016//戏班签到
		
 };

 //7日训
 enum SevenDayTranningEvent
 {
	 SDT01		= 3001,//成功通关任意关卡%d次
	 SDT02     = 3002,//强化任意装备%d次
	 SDT03     = 3003, //进行%d次玉器合成操作
	 SDT04      = 3004, //升级任意技能%d次
	 SDT05      = 3005,  //进行%d次体力购买
	 SDT06		= 3006,  //扫荡任意精英关卡%d次
	 SDT07		= 3007, //拜财神%d次
	 SDT08		= 3008, //进行%d次伶人招募
	 SDT09		 = 3009, //对任意京剧猫进行升级操作%d次
	 SDT10		= 3010, //进行%d次饰品洗练操作
	 SDT11		= 3011,//成功挑战多人副本%d次
	 SDT12		= 3012, //挑战招福集市%d次
	 SDT13		= 3013, //挑战任意十二宗宫关卡%d次
	 SDT14      = 3014, //刷新神秘商店%d次
	 SDT15		= 3015,//成功挑战任意大师关卡%d次
	 SDT16      = 3016,//成功进行%d次勇气试炼挑战
	 SDT17      = 3017,//进行%d排位赛挑战
// 	 SDT18      = 3018,//进行%d次同步PVP挑战  
	 SDT19      = 3019,//挑战%d次无限挑战玩法
	 SDT20      = 3020,//在京剧猫商店购买%d次商品
	 SDT21      = 3021//成功挑战%d次任意十二宗宫的宗主关

 };

 //公会日志关联id
enum ConsortLogEvent
 {
		CNLCREATE = 111, //创建公会
		CNLLVUP	= 112, //公会升级
		CNLSETUPVICELEADER = 113, //公会设置副会长
		CNLCHANGENAME = 114, //公会改名
		CNLMEMBERJOIN = 115, //公会成员加入
		CNLMEMBERLEAVE = 116, //成员离开
		CNLCHANGEDESCS = 117, //更改公会宣言
		CNLCHANGECHECK = 118, //更改公会招募方式
		CNLCHANGEPOWERLIMIT = 119,  //更改加入战力需求
		CNLKICKMEMBER = 120,//被踢出公会
		CNLREFUSEAPPLY	= 121, //拒绝申请
		CNLCLEARAPPLY = 122,
		CNLSETDOWNMEMBER = 123
};

//公会常量条件
enum ConsortCondition
{
	CREATENEEDVIPLV = 3, //公会创建需要vip等级
	CREATENEEDGOLD = 1000, //公会创建需要的元宝
    POWERLIMIT = 1000 //公会战力限制，至少1000才可加入
};

enum ConsortNotifyMsgType
{
	NOTIFYJOINCONSORT = 1,
	APPLYADD = 2,
	APPLYREFUSE = 3,
	APPLYACCEPT = 4,
	APPLYCLEAR = 5,
	CHANGECHECKNOTIFY = 6,
	CHANGEPOWERLIMIT = 7,
	CHANGEDESC = 8,
	KICKMEMBERNOTIFY = 9,
	SETVICELEADER = 10,
	LEAVECONSORT = 11

};
// 装备
// 行头
// 冒险
// 普通难度冒险
// 邮件
// 设置
// 公告
// 充值
// 体力购买
// 拜财神
// 福利
// 在线奖励
// 纳宗七日训
// 角色信息
// 招式
// 京剧猫
// 图鉴
// 赏罚令
// 玉器
// 任务
// 成就
// 每日任务
// 七天登陆奖励
// 美食屋
// 全民福利
// 珍宝阁
// 礼包商店
// 珍宝商店
// 月签到
// 好友
// 聊天
// 三庆便当
// 咚锵镇基金
// 竞技场
// 排位赛
// 斗技场
// 排行榜
// 饰品
// 界石
// 大师难度冒险
// 十二宗宫
// 韵纹
// 巴山试炼
// 时装
// 招福集市
// 通宝当铺
// 猛禽市场
// 玉石工坊
// 市集安保
// 京剧猫商店
// 勇气试炼
// 戏班
// 助阵
// 无限挑战


enum SYSTEM_ID
{
	SYSTEM_SHOP 			= 2001,
	SYSTEM_ZHAOFUJISHI 	= 1501,
	SYSTEM_SHANGFALING 	= 701,
	SYSTEM_RANK 			= 2201,
	SYSTEM_FOOD 			= 2101,
	SYSTEM_GUILD 			= 2301,//0
	SYSTEM_PVP			= 1702,//0
	SYSTEM_COURAGE 		= 1802,
	SYSTEM_PALACE 			= 1901,
	SYSTEM_CHALLENGE 		= 1801,
	SYSTEM_MULTI 			= 1601,
	SYSTEM_EQUIP 			= 104,//0
	SYSTEM_SERVANT 		= 501,//0
	SYSTEM_JEWELRY		= 201,//0
	SYSTEM_SKILL 			= 401,//0
	SYSTEM_HOARSTONE 		= 302,//0
	SYSTEM_RHYME 			= 301,//0
	SYSTEM_GROW 			= 300,//0
	SYSTEM_SETTING 		= 1101,//0
	SYSTEM_FRIEND 			= 601,//0
	SYSTEM_CHAT 			= 1201,//0
	SYSTEM_EMAIL 			= 1001,
	SYSTEM_MISSION 		= 901,//0
	SYSTEM_FULI 			= 1401,//0
	SYSTEM_PAY				= 1301,//0
	SYSTEM_ONLINEAWARD		= 1403,//0
	SYSTEM_STAGE			= 801,//0
	SYSTEM_NOTICE 			= 1102,//0
	SYSTEM_CONSORT			= 2301,
	SYSTEM_TASK			= 5000,
	SYSTEM_NEWPLAYERGUIDE			= 5001,
	SYSTEM_ELSEAWARD			= 5002,
};

enum systemidrefreshtime
{
	TIME_REFRESH_ID01 = 802,  //普通难度冒险（包含普通+精英关卡）可挑战次数每日重置时间
	TIME_REFRESH_ID02 = 803,  //大师难度冒险（不包含哥布林关卡）可挑战次数每日重置时间
	TIME_REFRESH_ID03 = 903,  //每日任务每日重置时间
	TIME_REFRESH_ID04 = 1302,  //体力购买每日重置次数时间
	TIME_REFRESH_ID05 = 1303,  //拜财神每日重置次数时间
	TIME_REFRESH_ID06 = 1404,  //月签到每日判断时间
	TIME_REFRESH_ID07 = 1408,  //三庆便当每日重置时间
	TIME_REFRESH_ID08 = 1501,  //招福集市每日可挑战次数重置时间
	TIME_REFRESH_ID09 = 1601,  //巴山试炼每日可挑战次数重置时间
	TIME_REFRESH_ID10 = 1702,  //排位赛每日可挑战次数重置时间
	TIME_REFRESH_ID11 = 1703,  //排位赛每日可额外购买次数重置时间
	TIME_REFRESH_ID12 = 1801,  //无限挑战每日可挑战次数重置时间
	TIME_REFRESH_ID13 = 1802,  //勇气试炼通宝商店每日重置时间
	TIME_REFRESH_ID14 = 1803,  //勇气试炼每日可额外购买次数重置时间
	TIME_REFRESH_ID15 = 1804,  //勇气试炼免费刷新挑战对象次数重置时间1
	TIME_REFRESH_ID16 = 1805,  //勇气试炼免费刷新挑战对象次数重置时间2
	TIME_REFRESH_ID17 = 1901,  //十二宗宫每日副本重置时间
	TIME_REFRESH_ID18 = 701,  //赏罚令免费最大次数重置时间
};

enum LOG_LEVEL
{
	LogInfo	= 1,
	LogWarn	=	2,
	LogError	= 3,
	LogFatal	= 4,

};

enum LOG_TYPE
{
	CoinChange = 25,
	GoldChange = 26,
	ServantChange = 27,
	ServantPieceChange = 28,
	JewelryChange = 29,

	LogType50 = 50, //	新增账号
	LogType51 = 51, //  新增角色
	LogType52 = 52, // 活跃用户
	LogType53 = 53, //	7天连续登陆+月连续
	LogType54 = 54, //   
	LogType55 = 55, //	
	LogType56 = 56, //	玩家登陆
	LogType57 = 57, //玩家离线

	LogType58 = 58, //玩家在线时长

	LogType59 = 59, //	在线人数
	LogType60 = 60, //最高在线人数
	LogType61 = 61, //每天登陆不同账号总数

	LogType62 = 62, //体力恢复
	LogType63= 63, //体力购买
	LogType64 = 64, //gm修改体力
	LogType65 = 65, //美食次数恢复
	LogType66 = 66, //美食总个数变化
	LogType67 = 67, //购买美食次数直接变成美食个数

	LogType68 = 68, //玉石
	LogType69 = 69, //经验
	LogType70 = 70, //韵魂
	LogType71 = 71, //排位赛积分
	LogType72 = 72, //竞技点（脸谱）
	LogType73 = 73, //薄荷草
	LogType74 = 74, //界石碎片
	LogType75 = 75, //强化材料
	LogType76 = 76, //符文
	LogType77 = 77, //普通关卡次数
	LogType78 = 78, //十二宗宫关卡次数
	LogType79 = 79, //佣兵消耗道具，包括五种便当和原石
	LogType80 = 80, //佣兵宝物
	LogType81 = 81, //佣兵的生命之光
	LogType82 = 82, //勇气试炼硬币
	LogType83 = 83, //赏罚令单抽券
	LogType84 = 84, //赏罚令十连抽券
	LogType85 = 85, //勇气试炼通币
	LogType86 = 86, //公会贡献
	LogType87 = 87, //时装更换
	LogType88 = 88, //角色切换
	LogType89 = 89, //饰品装备
	LogType90 = 90, //饰品卸载
	LogType91 = 91, //京剧猫上阵
	LogType92 = 92, //玉器镶嵌
	LogType93 = 93, //界石装备符文
	LogType94 = 94, //京剧猫助阵位上阵
	LogType95 = 95, //
	LogType96 = 96,//招式切换
	LogType97 = 97,//
	LogType98 = 98,//宝石卸载

	LogType99 = 99,//等级
	LogType100 = 100,//gm调等级
	LogType101 = 101,//VIP等级
	LogType102 = 102,//gm调VIP等级
	LogType103 = 103,//vip经验



	LogType200 = 200//


};

enum MINI_LOG
{
	MiniLog1=1,//装备强化
	MiniLog2=2,//装备升阶
	MiniLog3=3,//GM命令修改
	MiniLog4=4,//装备洗练
	MiniLog5=5,//角色重命名
	MiniLog6=6,//饰品出售
	MiniLog7=7,//饰品洗练
	MiniLog8=8,//多人副本双倍花费
	MiniLog9=9,//多人副本挑战
	MiniLog10=10,//时装购买
	MiniLog11=11,//	购买角色
	MiniLog12=12,//
	MiniLog13=13,//
	MiniLog14=14,//
	MiniLog15=15,//招式升级
	MiniLog16=16,//
	MiniLog17=17,//
	MiniLog18=18,//
	MiniLog19=19,//
	MiniLog20=20,//京剧猫升星
	MiniLog21=21,//京剧猫助阵位
	MiniLog22=22,//
	MiniLog23=23,//
	MiniLog24=24,//
	MiniLog25=25,//玉器解锁
	MiniLog26=26,//
	MiniLog27=27,//
	MiniLog28=28,//
	MiniLog29=29,//
	MiniLog30=30,//界石激活
	MiniLog31=31,//界石升级
	MiniLog32=32,//
	MiniLog33=33,//
	MiniLog34=34,//
	MiniLog35=35,//韵纹强化
	MiniLog36=36,//
	MiniLog37=37,//
	MiniLog38=38,//
	MiniLog39=39,//
	MiniLog40=40,//福利月签到领取
	MiniLog41=41,//基金
	MiniLog42=42,//全民福利
	MiniLog43=43,//三庆便当
	MiniLog44=44,//7天登录
	MiniLog45=45,//7天任务
	MiniLog46=46,//在线奖励
	MiniLog47=47,//成长基金
	MiniLog48=48,//
	MiniLog49=49,//
	MiniLog50=50,//任务每日任务
	MiniLog51=51,//任务成就
	MiniLog52=52,//奖励关卡挑战
	MiniLog53=53,//精英关卡挑战
	MiniLog54=54,//大师关卡挑战
	MiniLog55=55,//关卡宝箱
	MiniLog56=56,//普通关卡挑战
	MiniLog57=57,//普通关卡扫荡
	MiniLog58=58,//关卡购买体力
	MiniLog59=59,//关卡翻牌
	MiniLog60=60,//招福集市挑战
	MiniLog61=61,//招福集市扫荡
	MiniLog62=62,//奖励关卡扫荡
	MiniLog63=63,//精英关卡扫荡
	MiniLog64=64,//大师关卡扫荡
	MiniLog65=65,//十二宗宫购买号角
	MiniLog66=66,//十二宗宫购买挑战次数
	MiniLog67=67,//十二宗宫挑战
	MiniLog68=68,//十二宗宫扫荡
	MiniLog69=69,//
	MiniLog70=70,//美食屋购买次数
	MiniLog71=71,//
	MiniLog72=72,//
	MiniLog73=73,//
	MiniLog74=74,//
	MiniLog75=75,//戏班 创建
	MiniLog76=76,//戏班 签到
	MiniLog77=77,//戏班 箱子
	MiniLog78=78,//戏班 行侠仗义
	MiniLog79=79,//戏班 行商猫
	MiniLog80=80,//戏班 厨房特训
	MiniLog81=81,//戏班 眼力修炼
	MiniLog82=82,//戏班 兑换
	MiniLog83=83,//
	MiniLog84=84,//勇气试炼通宝商店刷新
	MiniLog85=85,//勇气试炼刷新
	MiniLog86=86,//勇气试炼购买次数
	MiniLog87=87,//勇气试炼挑战
	MiniLog88=88,//勇气试炼通宝商店购买
	MiniLog89=89,//硬币盒
	MiniLog90=90,//赏罚令令人免费招募
	MiniLog91=91,//赏罚令令人招募
	MiniLog92=92,//赏罚令戏班免费招募
	MiniLog93=93,//赏罚令戏班招募
	MiniLog94=94,//赏罚令名伶招募
	MiniLog95=95,//赏罚令铜鼓招募
	MiniLog96=96,//代金券十连抽
	MiniLog97=97,//代金券vip抽
	MiniLog98=98,//
	MiniLog99=99,//
	MiniLog100=100,//珍宝阁珍宝
	MiniLog101=101,//珍宝阁京剧猫
	MiniLog102=102,//珍宝阁珍宝刷新
	MiniLog103=103,//珍宝阁京剧猫刷新
	MiniLog104=104,//珍宝阁礼包
	MiniLog105=105,//无限挑战购买次数
	MiniLog106=106,//无限挑战挑战
	MiniLog107=107,//无限挑战扫荡
	MiniLog108=108,//无限挑战选择获得
	MiniLog109=109,//无限挑战复活
	MiniLog110=110,//排位赛购买次数
	MiniLog111=111,//排位赛脸谱商店购买
	MiniLog112=112,//排位赛段位奖励
	MiniLog113=113,//排位赛挑战
	MiniLog114=114,//排位赛脸谱商店刷新
	MiniLog115=115,//巴山试炼挑战
	MiniLog116=116,//巴山试炼翻倍
	MiniLog117=117,//
	MiniLog118=118,//
	MiniLog119=119,//
	MiniLog120=120,//邮件收邮件
	MiniLog121=121,//
	MiniLog122=122,//
	MiniLog123=123,//
	MiniLog124=124,//
	MiniLog125=125,//聊天发消息
	MiniLog126=126,//
	MiniLog127=127,//
	MiniLog128=128,//
	MiniLog129=129,//
	MiniLog130=130,//拜财神免费拜
	MiniLog131=131,//拜财神1次拜
	MiniLog132=132,//拜财神10次拜
	MiniLog133=133,//拜财神箱子
	MiniLog134=134,//
	MiniLog135=135,//VIP购买vip
	MiniLog136=136,//新手礼物
	MiniLog137=137,//客户端进度条奖励
	MiniLog138=138,//
	MiniLog139=139,//
	MiniLog140=140,//复活复活 
	MiniLog141=141,//
	MiniLog142=142,//
	MiniLog143=143,//
	MiniLog144=144,//首次登录赠送
	MiniLog145=145,//技能升级
	MiniLog146=146,//
	MiniLog147=147,//
	MiniLog148=148,//

};

#endif // __LYNX_COMMON_LIB_CONST_DEFINITION_DEFINITION_H__