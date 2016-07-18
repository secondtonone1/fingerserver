#ifndef __LYNX_COMMON_LIB_ERROR_DEFINITION_DEFINITION_H__
#define __LYNX_COMMON_LIB_ERROR_DEFINITION_DEFINITION_H__

#include "TypeDefinition.h"

namespace Lynx
{
	namespace LynxErrno
	{
		// CommonError
		static const errorId None                          = 0;		//成功
		static const errorId UnknownLogicError             = 1001;	//
		static const errorId InvalidIdentify               = 1002;	//
		static const errorId InvalidOperation              = 1003;	//
		static const errorId InvalidType                   = 1004;	//
		static const errorId InvalidPermission             = 1005;	//
		static const errorId InvalidParameter              = 1006;	//
		static const errorId InvalidState                  = 1007;	//  
        static const errorId TemplateNotExist              = 1008;	//
     
        static const errorId ItemNotExist                  = 1011;	//
        static const errorId EquipNotExist                 = 1012;	//
        static const errorId RuneNotExist                  = 1013;	// 符文不存在
    
        static const errorId CoinNotEnough                 = 1016;	//铜钱不足
        static const errorId LevelExpNotEnough             = 1017;	//经验不足
        static const errorId StarExpNotEnough              = 1018;	//
		static const errorId StepExpNotEnough              = 1019;

        static const errorId LevelNotEnough                = 1020;	//等级不足
        static const errorId StarNotEnouth                 = 1021;	//星数不足
      
      
    
        static const errorId RmbNotEnough                  = 1029;	//元宝不足
        static const errorId BagIsFull                     = 1030;	//
        static const errorId ItemIsFull                    = 1031;	//
 
		static const errorId MaterialNotEnough             = 1036;	//

		static const errorId StrengthNotEnough             = 1037;	//缺少体力
		static const errorId TimesNotEnough				   = 1038;	//次数不足
		static const errorId OverMaxTimes			       = 1039;	//超过上限次数
		static const errorId TimeNotRight		           = 1040;	//不是开放时间 //完成时间不正确
 		static const errorId NotOpen					   = 1041;	//未开启
 		static const errorId NetWorkError				   = 1042;	//客户端传输数据错误，个数不对，大小不对

		static const errorId IsUnlocked					   = 1043;	//已经解锁
		static const errorId IsLocked					   = 1044;	//未解锁
		static const errorId bugleNotEnough				   = 1045;	//号角不足
		static const errorId scoreNotEnough				   = 1046;	//积分不足
		static const errorId IsDad						   = 1047;	//已死亡
		static const errorId IsBuying					   = 1048;	//正在购买

		static const errorId OnceTicketNotEnough			= 1049;	//一抽券不足
		static const errorId TenTicketNotEnough				= 1050;	//十抽券不足
		static const errorId BeingCooled				    = 1051;	//正在冷却

		

 		static const errorId VipLevelNotEnough             = 1081;	//vip等级不够
		static const errorId VipBuyTimesNotEnough		   = 1082;	//vip购买次数不足
		static const errorId TableElementNotExit           = 1100;  //表里没有这个数据
		static const errorId PlayerNotFound	   			   = 1101;  //没有找到玩家
		static const errorId ServantSwitchNotEnough	       = 1102;  //生命之光不足



		// LoginError
		static const errorId AccountNotExist               = 2001;	//
		static const errorId AccountHasExist               = 2002;	//
        static const errorId AccountAuthFailed             = 2003;	//
        static const errorId PlayerNotExist                = 2004;	//
        static const errorId LoadPlayerFailed              = 2005;	//
        static const errorId InitPlayerFailed              = 2006;	//
        static const errorId LoginTokenFailed              = 2007;	//
        static const errorId PlayerInfoNotExist            = 2008;	//
        static const errorId NameLengthError               = 2009;	//
        static const errorId ActiveNameSpace               = 2010;	//
        static const errorId PlayerNameForbidden           = 2011;	//

    
        // DataBase
        static const errorId SqlExecuteFail                = 5001;	//
        static const errorId CreatePlayerFailed            = 5002;	//
        static const errorId PlayerNameConflict            = 5003;	//

        // Email
        static const errorId InvalidEmailItem              = 6001;	//
        static const errorId EmailNotExist                 = 6002;	//

      
		//Gem
		static const errorId GemLevelLimit				   = 9005;  //
		static const errorId GemNotExist				   = 9006;  //
		static const errorId GemNotEnough				   = 9007;

		//Rhyme
		static const errorId RhymeLevelLimit               = 10001;	//
		//韵功已经激活
		static const errorId AcupointActived               = 10002;	//

		static const errorId RhymeStepLimit                = 10003;

		//技能
		static const errorId SkillLevelLimit			   = 11001;	//

		//洗练

		static const errorId MaterialRdEmpty               = 12001;	//

		//界石
		static const errorId HoarStoneActived              = 13001;	//
		static const errorId HoarStoneLvLimit              = 13002;	//
		static const errorId HoarStoneCondition            = 13003;	//

		//装备
		//装备等级达到最高级别
		static const errorId EquipLvLimit				   = 14001;

		static const errorId EquipNoneActive			   = 14002;

		static const errorId EquipGemSlotLock			   = 14003;

		static const errorId EquipGemHasExist			   = 14004;

		static const errorId EquipGemTypeError			   = 14005;

		static const errorId EquipGemUnlocked			   = 14006;

		static const errorId FashionHasExit							= 14007;

		static const errorId FashionNotExit							= 14008;


		//关卡

		static const errorId AttackPowerNotRight		   = 15001;//战力不足

		static const errorId NumberNotRight				   = 15002;//数值不正确

 		static const errorId SeatIsFull					   = 15003;//位置已满
				
		static const errorId BeDissolved				   = 15004;//解散
				
 		static const errorId NotFound					   = 15005;//没找到
		
		static const errorId TimeOut					   = 15010;//时间到
		
		static const errorId NotRelive					   = 15011;//没有复活

		static const errorId LeaveStage					   = 15012;//离开关卡

		static const errorId Found						   = 15013;//找到

		static const errorId Used						   = 15014;//已使用

		static const errorId ReliveTimesUsedUp			   = 15015;//复活次数已使用
// 	
// 				
// 		static const errorId AttackPowerNotRight				   = 15001;

		//饰品
		static const errorId JewelryLock                   = 16001;
		static const errorId JewelryEquiped                = 16002;
		static const errorId SlotEmpty                     = 16003;
		static const errorId SlotPosError                  = 16004;

		//佣兵
		static const errorId ServantPieceLess              = 17001;
		static const errorId ServantExist                  = 17002;
		static const errorId ServantLvLimit                = 17003;
		static const errorId ServantNotExit                = 17004;
		static const errorId ServantStarLimit              = 17005;
		static const errorId ServantFloorLimit             = 17006;
		static const errorId STreasureNotExit              = 17007;
		static const errorId STreasureNotEnough            = 17008;
		static const errorId ServantBattleOpen             = 17009;
		static const errorId ServantBattleLock             = 17010;
		static const errorId BattleQualityLvLimit          = 17011;

		//好友
		static const errorId OtherFriendFull							= 18001; //别人的好友满了
		static const errorId OtherBlackFull							= 18002; //别人的黑名单满了
		static const errorId SelfFriendFull							= 18003; //自己的好友满了
		static const errorId SelfBlackFull							= 18004; //自己的黑名单满了

		//
		static const errorId Lose							= 19001; //失败
		static const errorId Win							= 19002; //胜率
		static const errorId HadBuy							= 19003; //已经购买
		static const errorId RefreshTimesNotEnough			= 19004; //刷新次数不足
		static const errorId IsActivated					= 19010; //已经激活
		static const errorId CatCoinNotEnough				= 19011; //猫币不足
		static const errorId VipBuyCourageChallengeTimesNotEnough	  = 19012;	//vip购买勇气挑战次数不足
		static const errorId CoinGroupNotEnough				 = 19013;	//硬币组合解锁不足

		static const errorId HadGot							= 19014; //已经领取
		static const errorId HadSign						= 19015; //已经签到
		static const errorId SignCountNotEnough				= 19016; //签到个数不足
		static const errorId RechargeNumNotEnough			= 19017; //充值人数不足
		static const errorId TaskNotFinished				= 19018; //任务未完成

		



		//角色
		static const errorId CharactorExits         = 21000; //角色已经存在
		static const errorId CharactorNotExits   =  21001; //角色不存在
		static const errorId IsNotCurCharactor  =  21002; //角色不是当前装备角色 
	
		//成就和每日任务
		static const errorId EventNotFind       = 22000; //成就对应的事件没找到
		static const errorId AchIdNotMatch   = 22001; //成就id不匹配
		static const errorId AchIdNotExist      = 22002; //配置表里没有对应的id
		static const errorId ConditionNotReach = 22003; //条件未达成
		static const errorId DailyTaskIdNotMatch		= 22004;//日常任务id不匹配
		static const errorId DailyTaskHasExist          = 22005;//每日任务已经存在
		

		

	} // namespace LynxErrno
} //namespace Lynx

#endif // __LYNX_COMMON_LIB_ERROR_DEFINITION_DEFINITION_H__