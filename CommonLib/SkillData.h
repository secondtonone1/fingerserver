#ifndef __LYNX_COMMON_LIB_SKILL_DATA_H__
#define __LYNX_COMMON_LIB_SKILL_DATA_H__

#include "Definition.h"
// #include "../FireConfirm/Character.h"

namespace Lynx
{

	struct SkillData
	{
		SkillData():m_nID(0),m_nCD(0),m_nLevel(0),m_nEquipPos(0){}

		//技能的ID；
		Guid   m_nID;
		//技能的CD
		UInt32 m_nCD;
		//技能的等级
		UInt32 m_nLevel;
		//技能装备的槽位 0 表示没装备，1 表示装备在槽1,2表示装备在槽2 ... 
		UInt32 m_nEquipPos;

		LYNX_S11N_4(SkillData, Guid, m_nID, UInt32, m_nCD, UInt32, m_nLevel,UInt32,m_nEquipPos);
	};
	
	struct SkillEquipData
	{
		SkillEquipData():m_nID(0),m_nEquipPos(0){}
		//技能的id
		UInt64 m_nID;
		//技能的位置
		UInt32 m_nEquipPos;

		LYNX_S11N_2(SkillEquipData, UInt64, m_nID, UInt32, m_nEquipPos);
	};

	struct SkillEquip
	{
		UInt32 normal_attack ;
		UInt32 jump_attack;
		UInt32 up ;
		UInt32 down ;
// 		Pos current;
// 		Pos back;	
	};
	struct SkillEquipList
	{
// 		List<SkillEquip> m_SkillEquipList;
	};



} // namespace Lynx

#endif // __LYNX_COMMON_LIB_SKILL_DATA_H__