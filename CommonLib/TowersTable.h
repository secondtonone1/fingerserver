#ifndef __LYNX_COMMON_LIB_TOWER_TABLE_H__
#define __LYNX_COMMON_LIB_TOWER_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct TowerTypeTemplate 
	{

		UInt32 id;

		String name;

		String desc;

		UInt32 towerheight;

		UInt32 attackpower;

		UInt32 hp;

		UInt32 strength;

		UInt32 startscore;

		UInt32 mopupcost; 

		UInt32 operatetlimittime;

		UInt32 strengthreduce;

		UInt32 hprecover;

		UInt32 roulettemaxtimes;

		UInt32 rouletteid1;

		UInt32 rouletteid2;

		UInt32 rouletteid3;

		UInt32 rouletteid4;

		UInt32 rouletteid5;

		UInt32 roulette;

		UInt32 scorerate;

		String awardshow;

		UInt32 mopupaward1;

		UInt32 mopupawardrate1;

		UInt32 mopupaward2;

		UInt32 mopupawardrate2;

		UInt32 mopupaward3;

		UInt32 mopupawardrate3;

		UInt32 mopupaward4;

		UInt32 mopupawardrate4;

		LYNX_S11N_29(TowerTypeTemplate,UInt32, id,String, name,String, desc,UInt32, towerheight,UInt32, attackpower,
			UInt32, hp,UInt32, strength,UInt32, startscore,UInt32, mopupcost,UInt32, operatetlimittime,UInt32, strengthreduce,
			UInt32, hprecover,UInt32, roulettemaxtimes,UInt32, rouletteid1,UInt32, rouletteid2,UInt32, rouletteid3,
			UInt32, rouletteid4,UInt32, rouletteid5,UInt32, roulette,UInt32, scorerate,String, awardshow,	
			UInt32, mopupaward1,UInt32, mopupawardrate1,UInt32, mopupaward2,UInt32,mopupawardrate2,
			UInt32, mopupaward3,UInt32, mopupawardrate3,UInt32 ,mopupaward4,UInt32, mopupawardrate4
			);

	};

	typedef Map<UInt32, TowerTypeTemplate> TowerTypeTemplateMap;


	struct TowerTypeTable
	{
		TowerTypeTemplateMap mMap;


		LYNX_S11N_1(TowerTypeTable, TowerTypeTemplateMap, mMap);

		TowerTypeTemplate* get(UInt32 id)
		{
			TowerTypeTemplateMap::Iter* iter = mMap.find(id);

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


	struct TowerMonsterTemplate 
	{

		UInt32 id;

		String name;

		String desc;

		UInt32 res;

		UInt32 attackpower;

		UInt32 HP;

		UInt32 briberycost;

		UInt32 score;

		LYNX_S11N_8(TowerMonsterTemplate,UInt32, id,String, name,String, desc,UInt32, res,UInt32, attackpower,
			UInt32, HP,UInt32, briberycost,UInt32, score
			);
	};

	typedef Map<UInt32, TowerMonsterTemplate> TowerMonsterTemplateMap;


	struct TowerMonsterTable
	{
		TowerMonsterTemplateMap mMap;


		LYNX_S11N_1(TowerMonsterTable, TowerMonsterTemplateMap, mMap);

		TowerMonsterTemplate* get(UInt32 id)
		{
			TowerMonsterTemplateMap::Iter* iter = mMap.find(id);

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

	struct TowerEventTemplate 
	{

		UInt32 id;			
		
		UInt32 type;

		String param;


		LYNX_S11N_3(TowerEventTemplate,UInt32, id,UInt32, type,String, param);
	};

	typedef Map<UInt32, TowerEventTemplate> TowerEventTemplateMap;


	struct TowerEventTable
	{
		TowerEventTemplateMap mMap;


		LYNX_S11N_1(TowerEventTable, TowerEventTemplateMap, mMap);

		TowerEventTemplate* get(UInt32 id)
		{
			TowerEventTemplateMap::Iter* iter = mMap.find(id);

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

	struct TowerBuffTemplate 
	{

		UInt32 id;

		UInt32 attr;

		UInt32 num;		

		UInt32 keeptime;

		UInt32 keepfloor;

		LYNX_S11N_5(TowerBuffTemplate,UInt32, id,UInt32, attr,UInt32, num,UInt32, keeptime,UInt32, keepfloor);
	};

	typedef Map<UInt32, TowerBuffTemplate> TowerBuffTemplateMap;


	struct TowerBuffTable
	{
		TowerBuffTemplateMap mMap;


		LYNX_S11N_1(TowerBuffTable, TowerBuffTemplateMap, mMap);

		TowerBuffTemplate* get(UInt32 id)
		{
			TowerBuffTemplateMap::Iter* iter = mMap.find(id);

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

	struct TowerAttrTemplate 
	{

		UInt32 id;

		UInt32 attr;

		UInt32 num;		

		LYNX_S11N_3(TowerAttrTemplate,UInt32, id,UInt32, attr,UInt32, num);
	};

	typedef Map<UInt32, TowerAttrTemplate> TowerAttrTemplateMap;


	struct TowerAttrTable
	{
		TowerAttrTemplateMap mMap;


		LYNX_S11N_1(TowerAttrTable, TowerAttrTemplateMap, mMap);

		TowerAttrTemplate* get(UInt32 id)
		{
			TowerAttrTemplateMap::Iter* iter = mMap.find(id);

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


	struct TowerTemplate 
	{
		UInt32 stageId;

		UInt32 chapterId;

		String name;

		String desc;

		String finShowList;

		String point;

		UInt32 stageStepCount;

		String icon;

		UInt32 awardGold;

		UInt32 awardExp; 

		UInt32 awardOther;//这里限定为积分，其他地方是可变的

		UInt32 awardID;

		UInt32 firstAwardID;

		UInt32 mopuptype;

		UInt32 event1;

		UInt32 eventrate1;

		UInt32 event2;

		UInt32 eventrate2;

		UInt32 event3;

		UInt32 eventrate3;

		UInt32 event4;

		UInt32 eventrate4;

		UInt32 event5;

		UInt32 eventrate5;

		LYNX_S11N_24(TowerTemplate, UInt32, stageId,UInt32, chapterId,String, name,String, desc,
			String, finShowList,String, point,UInt32, stageStepCount,String, icon,UInt32, awardGold,UInt32, awardExp,
			UInt32, awardOther,UInt32, awardID,UInt32, firstAwardID,UInt32, mopuptype,
			UInt32, event1,UInt32, eventrate1,UInt32, event2,UInt32, eventrate2,UInt32, event3,UInt32, eventrate3,
			UInt32 ,event4,UInt32, eventrate4,UInt32, event5,UInt32, eventrate5
			);

	};

	typedef Map<UInt32, TowerTemplate> TowerTemplateMap;


	struct TowersTable
	{
		TowerTemplateMap mMap;


		LYNX_S11N_1(TowersTable, TowerTemplateMap, mMap);

		TowerTemplate* get(UInt32 id)
		{
			TowerTemplateMap::Iter* iter = mMap.find(id);

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


	struct TowerBoxTemplate 
	{

		UInt32 id;

		UInt32 awardcontent;

		UInt32 attackpower;

		UInt32 HP;

		UInt32 scorecost;


		LYNX_S11N_5(TowerBoxTemplate,UInt32, id,UInt32 ,awardcontent,UInt32, attackpower,UInt32 ,HP,UInt32 ,scorecost);
	};

	typedef Map<UInt32, TowerBoxTemplate> TowerBoxTemplateMap;


	struct TowerBoxTable
	{
		TowerBoxTemplateMap mMap;


		LYNX_S11N_1(TowerBoxTable, TowerBoxTemplateMap, mMap);

		TowerBoxTemplate* get(UInt32 id)
		{
			TowerBoxTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_TOWER_TABLE_H__