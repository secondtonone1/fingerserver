#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__

#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "Character.h"


namespace Lynx
{
	class Player;

	struct VertigoVal
	{
		UInt64 time;
		float rate;
		float val ;

	};

	class BattleCalc : public Singleton<BattleCalc>
	{
	public:
		BattleCalc();			
		virtual ~BattleCalc();
		bool checkCanAttackByCampID(Character &attacker,Character &defencer);

		void cleanTargets();
		bool checkInTargets(Guid uid );

		void addToTargets(UInt64 playerID);

		Damages calcDamage(Character &attacker,Character &defencer,AttParams attParams);

		float calcAttackPush(Character &attacker,Character &defencer,Map<UInt32, AttrSkillAdd*> attrSkillAdd);

		float calcAttackRaise(Character &attacker,Character &defencer,float notstand,Map<UInt32, AttrSkillAdd*> attrSkillAdd);

		float calcAttackFall(Character &attacker,Character &defencer,float notstand);

		float calcAttackFloat(Character &attacker,Character &defencer,float notstand);

		float calcAttackStiff(Character &attacker,Character &defencer,Map<UInt32, AttrSkillAdd*> attrSkillAdd);

		List<UInt32> calcBuffState(Character &attacker,Character &defencer,List<UInt32> buffArray);

		VertigoVal getEffectValOfVertigo( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx );

		float getEffectValOfPoison( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx );

		float getEffectValOfBlood( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx );

		float getEffectValOfKO( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx );

		Damage getDamage(Character &attacker,Character defencer,Damage damageContent);

		Damage getCritValue( Character &attacker,Damage damage );

		float getCritRate( Character &attacker,Character &defencer );

		VertigoVal getEffectValOfSlow( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx );


	private:

	};


}
#endif // __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__
