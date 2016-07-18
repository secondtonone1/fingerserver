#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__



namespace Lynx
{
	class Player;
	
// 
// 	struct attParams		
// 	{
// 		UInt32 damage;
// 		UInt32 *attackArea ;
// 		UInt32 *attrSkillAdd ;
// 		UInt32 *addBuff;
// 		UInt32 notstand;
// 		UInt32 isKeep;
// 		bool isAttacksBegin;
// 
// 
// 		// 	local attParams = {}
// 		// 	attParams.damage = params[1]	-- {dType=0, dValue=100, hurtType=1}    dType: 0:法术伤害, 1:物理伤害
// 		// 		attParams.attackArea = params[2] 	-- {x=0, y=0, w=0, h=0}
// 		// 	attParams.attrSkillAdd = params[3] 	-- {["GameDefine.CHAR_ATTR.AttPush"] = {affectType=1, affectValue=100}, ["GameDefine.CHAR_ATTR.AttRaise"] = {affectType=1, affectValue=100}, ...}
// 		// 	attParams.addBuff = params[4] 		-- {1, 2}
// 		// 	attParams.notstand = params[5] 		-- 0:无效果, 1:浮空, 2:击倒, 3:击飞 
// 		// 		if isKeep == true then
// 		// 			if (params[6] == nil) or (params[6] == 0) then 	-- 0:不是攻击区间的开始帧, 1:是攻击区间的开始帧
// 		// 				attParams.isAttacksBegin = false 	
// 		// 				elseif params[6] == 1 then
// 		// 				attParams.isAttacksBegin = true
// 		// 				end
// 		// 				end
// 
// 	};
// 	struct finalDamages
// 	{
// 		UInt32 spcost;
// 		UInt32 push;
// 		UInt32 raise;
// 		UInt32 fall;
// 		UInt32 fly;
// 		UInt32 stiff;
// 		UInt32 buff;
// 		UInt32 critRate;
// 		UInt32 damage;
// 		UInt32 isCrit;
// 		UInt32 hurtType;
// 
// 		// 
// 		// 		local finalDamages = {}
// 		// 
// 		// 		local skillIdRate = attParams.damage.dValue
// 		// 			finalDamages.spcost = attacker:getCharacter():getAttr(GameDefine.CHAR_ATTR_KEY.SPDamage) * (-1) * (skillIdRate)
// 		// 
// 		// 			finalDamages.push = BattleCalc.calcAttackPush(attacker, defencer, attParams.attrSkillAdd)
// 		// 			finalDamages.raise = BattleCalc.calcAttackRaise(attacker, defencer, attParams.notstand, attParams.attrSkillAdd)
// 		// 			finalDamages.fall = BattleCalc.calcAttackFall(attacker, defencer, attParams.notstand)
// 		// 			finalDamages.fly = BattleCalc.calcAttackFloat(attacker, defencer, attParams.notstand)
// 		// 			finalDamages.stiff = BattleCalc.calcAttackStiff(attacker, defencer, attParams.attrSkillAdd)
// 		// 			finalDamages.buff = BattleCalc.calcBuffState(attacker, defencer, attParams.addBuff)
// 		// 
// 		// 			local critRate = BattleCalc.getCritRate( attacker, defencer )
// 		// 			local damage = BattleCalc.getDamage( attacker, defencer, attParams.damage)
// 		// 			if (math.random(0,100)/100) <= critRate then
// 		// 				finalDamages.isCrit= true
// 		// 				finalDamages.damage = BattleCalc.getCritValue(attacker, damage)
// 		// 			else
// 		// 			finalDamages.isCrit = false
// 		// 			finalDamages.damage =  damage
// 		// 			end
// 		// 
// 		// 			--hurt type
// 		// 			finalDamages.hurtType = attParams.damage.hurtType or GameDefine.HURT_TYPE.Default
// 	};


	class BattleCalc
	{
	public:
		BattleCalc();			
		virtual ~BattleCalc();

// 			bool checkCanAttackByCampID(Player* plaattacker, Player*defencer);
// 			finalDamages calcDamage(Player *attacker, Player *defencer, attParams attparams);

	private:

	};

	





}
#endif // __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__
