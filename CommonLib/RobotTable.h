#ifndef __LYNX_COMMON_LIB_COPY_ROBOT_TABLE_H__
#define __LYNX_COMMON_LIB_COPY_ROBOT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct RobotTableTemplate
	{		
		UInt32 roleID;
		String name;
		String guild;
		String title;
		UInt32 level;		
		UInt32 power;
		UInt32 vipLv;
		UInt32 modelID;
		String puonfashionID;
		String equipID1;
		String skill1;
		String skill2;
		String skill3;
		
		String rhymeskill1;
		String rhymeskill2;
		String rhymeskill3;
		String onstation1;
		String onstation2;
		String onstation3;
		String onstation4;
		String onstation5;
		LevelGrowTemplate levelGrow;
// 
// 		UInt32 MaxHP;
// 		UInt32 MaxMP;
// 		UInt32 MaxSP;
// 		UInt32 PA;
// 		UInt32 PF;
// 		UInt32 MA;
// 		UInt32 MF;
// 		UInt32 HPRecover;
// 		UInt32 MPRecover;
// 		UInt32 SPRecover;
// 		UInt32 AttSpeed;
// 		UInt32 CombinationAtt;
// 		UInt32 MoveSpeed;
// 		UInt32 JumpHeight;
// 		UInt32 FlyTimes;
// 		UInt32 SPDamage;
// 		UInt32 AttPush;
// 		UInt32 AttPushDef;
// 		UInt32 AttRaise;
// 		UInt32 AttRaiseDef;
// 		UInt32 Stiff;
// 		UInt32 StiffDef;
// 		UInt32 BaseCrit;
// 		UInt32 CritRate;
// 		UInt32 CritDef;
// 		UInt32 StunDef;
// 		UInt32 StunTimeDef;
// 		UInt32 SlowDef;
// 		UInt32 SlowTimeDef;
// 		UInt32 PoisonDef;
// 		UInt32 BloodDef;
// 		UInt32 KODef;
// 		UInt32 FloatVal;
// 		UInt32 FloatDef;
// 		UInt32 Fall;
// 		UInt32 FallDef;
// 	
		
		LYNX_S11N_22(RobotTableTemplate,UInt32, roleID,String, name,
			String, guild,
			String, title,
			UInt32, level,		
			UInt32, power,
			UInt32, vipLv,
			UInt32, modelID,
			String, puonfashionID,
			String, equipID1,
			String, skill1,
			String, skill2,
			String, skill3,
			String, rhymeskill1,
			String, rhymeskill2,
			String, rhymeskill3,
			String, onstation1,
			String, onstation2,
			String, onstation3,
			String, onstation4,
			String, onstation5,
			LevelGrowTemplate, levelGrow

// 			UInt32 MaxHP,
// 			UInt32 MaxMP,
// 			UInt32 MaxSP,
// 			UInt32 PA,
// 			UInt32 PF,
// 			UInt32 MA,
// 			UInt32 MF,
// 			UInt32 HPRecover,
// 			UInt32 MPRecover,
// 			UInt32 SPRecover,
// 			UInt32 AttSpeed,
// 			UInt32 CombinationAtt,
// 			UInt32 MoveSpeed,
// 			UInt32 JumpHeight,
// 			UInt32 FlyTimes,
// 			UInt32 SPDamage,
// 			UInt32 AttPush,
// 			UInt32 AttPushDef,
// 			UInt32 AttRaise,
// 			UInt32 AttRaiseDef,
// 			UInt32 Stiff,
// 			UInt32 StiffDef,
// 			UInt32 BaseCrit,
// 			UInt32 CritRate,
// 			UInt32 CritDef,
// 			UInt32 StunDef,
// 			UInt32 StunTimeDef,
// 			UInt32 SlowDef,
// 			UInt32 SlowTimeDef,
// 			UInt32 PoisonDef,
// 			UInt32 BloodDef,
// 			UInt32 KODef,
// 			UInt32 FloatVal,
// 			UInt32 FloatDef,
// 			UInt32 Fall,
// 			UInt32 FallDef
		);
	};

	typedef Map<UInt64, RobotTableTemplate> RobotTableMap;


	struct RobotTable
	{	
		RobotTableMap mMap;


		LYNX_S11N_1(RobotTable, RobotTableMap, mMap);

		RobotTableTemplate* get(UInt32 id)
		{
			RobotTableMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_COPY_ROBOT_TABLE_H__


