#include "globalvar.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
GlobalVarTable::loadFromDbc(const String& fileName)
{
	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
	Int32 length = RESOURCE_GROUP_MANAGER().loadFileToStream(fileName, 
		streamBuffer, "Data");
	if (length <= 0)
	{
		LOG_WARN("Failed to load file %s", fileName.c_str());
		return false;
	}
	if (!unserialize(*this, streamBuffer, 0))
	{
		LOG_WARN("Failed to unserialize file %s", fileName.c_str());
		return false;
	}

	return true;
}


bool 
GlobalVarTable::reloadFromDbc(const String& fileName)
{
	mGlobalList tmpList = mList;
	mList.clear();
	if (!loadFromDbc(fileName))
	{
		mList = tmpList;
		LOG_WARN("Failed to reload gloabalvar table.");
		return false;
	}
	return true;
}



bool 
GlobalVarTable::loadFromCsv(const String& filePath)
{
	mList.clear();
	std::ifstream fileStream(filePath.c_str());
	if (fileStream.fail())
	{
		LOG_WARN("Failed to open %s file.", filePath.c_str());
		return false;
	}
	CsvReader csvReader(fileStream, ",");
	csvReader.initTitle();
	while (csvReader.readLine())
	{
		GlobalTemplate globaltemplate;

		if (!csvReader.bind("skey", globaltemplate.sKey))
		{
			LOG_WARN("Failed to load gloabalvar.csv for [skey]");
			return false;
		}
		if (!csvReader.bind("svalue", globaltemplate.sValue))
		{
			LOG_WARN("Failed to load gloabalvar.csv for [svalue]");
			return false;
		}
		
		mList.insertTail(globaltemplate);
	}
	fileStream.close();
	return true;
}

bool GlobalVarTable::saveToDbc(const String& filePath)
{
	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
	serialize(*this, streamBuffer, 0);
	if (!FileUtil::saveFile(filePath, streamBuffer))
	{
		LOG_WARN("Failed to save file %s.", filePath.c_str());
		return false;
	}
	return true;
}
UInt32 GlobalVarManager::initial()
{
	if (initMeiShiWu() != 1)
	{
		return 2;
	}
	if (initStrength() != 1)
	{
		return 2;
	}
	if (initSaoDang() != 1)
	{
		return 2;
	}
	if (initeliterelive() != 1)
	{
		return 2;
	}
	if (initnormalrelive() != 1)
	{
		return 2;
	}
	if (initspecialrelive() != 1)
	{
		return 2;
	}
	if (initnormalbuff() != 1)
	{
		return 2;
	}
	if (initelitebuff() != 1)
	{
		return 2;
	}
	if (inittrial() != 1)
	{
		return 2;
	}

	if (inittwelvepalace() != 1)
	{
		return 2;
	}

	if (initclimbtower() != 1)
	{
		return 2;
	}

	if (initmysticalshopcost() != 1)
	{
		return 2;
	}
	if (initservantshopcost() != 1)
	{
		return 2;
	}
	if (initgainwealth() != 1)
	{
		return 2;
	}

	if (initterritorylucky() != 1)
	{
		return 2;
	}

	if (initGrowthFound() != 1)
	{
		return 2;
	}
	
	return 1;
	
}

UInt32 GlobalVarManager::initMeiShiWu()
{
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("meishiwu");
	if (skeySvalueTemplate == NULL )
	{
		return 2;
	}	

	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		if (it->mValue.sKey == "weight1")
		{			
			
			meiShiWuTemplate.weight1 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "weight2")
		{
			meiShiWuTemplate.weight2 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "weight3")
		{
			meiShiWuTemplate.weight3 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	
		else if (it->mValue.sKey == "max")
		{
			meiShiWuTemplate.max = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "recover")
		{
			meiShiWuTemplate.recover = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "eachBuy")
		{
			meiShiWuTemplate.eachBuy = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price")
		{
			meiShiWuTemplate.price = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "time")
		{
			meiShiWuTemplate.time = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "rate1")
		{
			meiShiWuTemplate.rate1 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "rate2")
		{
			meiShiWuTemplate.rate2 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "rate3")
		{
			meiShiWuTemplate.rate3 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
	}
	return 1;

}


UInt32 GlobalVarManager::initStrength()
{
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("strength");
	if (skeySvalueTemplate == NULL )
	{
		return 2;
	}	

	strengthTemplate.recover = 0;

	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		if (it->mValue.sKey == "max")
		{
			strengthTemplate.max = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}		
		else if (it->mValue.sKey == "eachBuy")
		{
			strengthTemplate.eachBuy = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}		
		else if (it->mValue.sKey == "time")
		{
			strengthTemplate.time = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price1")
		{
			strengthTemplate.price1 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price2")
		{
			strengthTemplate.price2 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price3")
		{
			strengthTemplate.price3 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price4")
		{
			strengthTemplate.price4 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price5")
		{
			strengthTemplate.price5 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price6")
		{
			strengthTemplate.price6 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price7")
		{
			strengthTemplate.price7 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price8")
		{
			strengthTemplate.price8 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price9")
		{
			strengthTemplate.price9 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "price10")
		{
			strengthTemplate.price10 = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}

	}
	return 1;

}


UInt32 GlobalVarManager::initSaoDang()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("saodang");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"price%d",i);
		strData = (String)dest;
		if (it->mValue.sKey == "rate")
		{			
			saoDangTemplate.rate = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "sweeptimes")
		{
			saoDangTemplate.sweeptimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "sweepfreetimes")
		{
			saoDangTemplate.sweepfreetimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "amount")
		{
			saoDangTemplate.amount = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData)
		{
			i++;
			saoDangTemplate.prices.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}

	
	return 1;

}


UInt32 GlobalVarManager::initnormalrelive()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("normalrelive");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"cost%d",i);
		strData = (String)dest;
		if (it->mValue.sKey == "dayfreetimes")
		{			
			normalReliveTemplate.dayfreetimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "maxtimes")
		{
			normalReliveTemplate.maxtimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData)
		{
			i++;
			normalReliveTemplate.costs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}

	return 1;
}

UInt32 GlobalVarManager::initeliterelive()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("eliterelive");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"cost%d",i);
		strData = (String)dest;
		if (it->mValue.sKey == "dayfreetimes")
		{			
			eliteReliveTemplate.dayfreetimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "maxtimes")
		{
			eliteReliveTemplate.maxtimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData)
		{
			i++;
			eliteReliveTemplate.costs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}

	return 1;
}

UInt32 GlobalVarManager::initspecialrelive()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("specialrelive");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"cost%d",i);
		strData = (String)dest;
		if (it->mValue.sKey == "dayfreetimes")
		{			
			specialReliveTemplate.dayfreetimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "maxtimes")
		{
			specialReliveTemplate.maxtimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData)
		{
			i++;
			specialReliveTemplate.costs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}
	return 1;
}

UInt32 GlobalVarManager::initbonusrelive()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("bonusrelive");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"cost%d",i);
		strData = (String)dest;
		if (it->mValue.sKey == "dayfreetimes")
		{			
			bonusReliveTemplate.dayfreetimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "maxtimes")
		{
			bonusReliveTemplate.maxtimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData)
		{
			i++;
			bonusReliveTemplate.costs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}
	return 1;
}

UInt32 GlobalVarManager::initmasterrelive()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("masterrelive");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"cost%d",i);
		strData = (String)dest;
		if (it->mValue.sKey == "dayfreetimes")
		{			
			masterReliveTemplate.dayfreetimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == "maxtimes")
		{
			masterReliveTemplate.maxtimes = (lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData)
		{
			i++;
			masterReliveTemplate.costs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}
	return 1;
}


UInt32 GlobalVarManager::initnormalbuff()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("normalbuff");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"buffid%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			normalBuffTemplate.buffIDs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}
	return 1;
}

UInt32 GlobalVarManager::initelitebuff()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("elitebuff");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"buffid%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			eliteBuffTemplate.buffIDs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}


	return 1;

}

UInt32 GlobalVarManager::initspecialbuff()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("specialbuff");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"buffid%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			specialBuffTemplate.buffIDs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	
	}
	return 1;
}

UInt32 GlobalVarManager::inittwelvepalace()
{

	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("twelvepalace");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}

	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		if (it->mValue.sKey == "dailyreset")
		{
			twelvePalaceTemplate.dailyreset = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "needitemid")
		{
			twelvePalaceTemplate.needitemid = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "canresetitem")
		{
			twelvePalaceTemplate.canresetitem = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "resetlowerlimit")
		{
			twelvePalaceTemplate.resetlowerlimit = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "buycost")
		{
			twelvePalaceTemplate.buycost = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "buybosscost")
		{
			twelvePalaceTemplate.buybosscost = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
	}
	
	return 1;
}


UInt32 GlobalVarManager::initclimbtower()
{

	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("climbtower");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}

	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		if (it->mValue.sKey == "dailyreset")
		{
			climbTowerTemplate.dailyreset = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "needitemid")
		{
			climbTowerTemplate.needitemid = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "canresetitem")
		{
			climbTowerTemplate.canresetitem = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "resetlowerlimit")
		{
			climbTowerTemplate.resetlowerlimit = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "buycost")
		{
			climbTowerTemplate.buycost = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
	}

	return 1;
}

UInt32 GlobalVarManager::inittrial()
{
	UInt32 i = 1;
	UInt32 ii = 1;
	UInt32 iii = 1;
	char dest[64] = {};
	char dest2[64] = {};
	char dest3[64] = {};
	String strData1;
	String strData2;
	String strData3;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("trial");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"cost%d",i);
		strData1 = (String)dest;
		sprintf(dest2,"player2cost%d",ii);
		strData2 = (String)dest2;
		sprintf(dest3,"ratecost%d",iii);
		strData3 = (String)dest3;

		if (it->mValue.sKey == "dayfreetimes")
		{
			trialTemplate.dayfreetimes = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "relivemaxtimes")
		{
			trialTemplate.relivemaxtimes = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "player2dayfreetimes")
		{
			trialTemplate.player2dayfreetimes = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		else if (it->mValue.sKey == "reliveplayer2maxtimes")
		{
			trialTemplate.reliveplayer2maxtimes = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}

		else if (it->mValue.sKey == strData1)
		{
			i++;
			trialTemplate.costs.insert(i, lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData2)
		{
			ii++;
			trialTemplate.player2costs.insert(ii, lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		else if (it->mValue.sKey == strData3)
		{
			iii++;
			trialTemplate.ratecost.insert(iii, lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	
		
	}
	return 1;
}

UInt32 GlobalVarManager::inittrailbuff()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("trialbuff");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"buffid%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			trailBuffTemplate.buffIDs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}
	return 1;
}

UInt32 GlobalVarManager::initmasterbuff()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("masterrelivebuff");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"buffid%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			masterBuffTemplate.buffIDs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}
	return 1;
}

UInt32 GlobalVarManager::initbonusbuff()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("bonusrelivebuff");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"buffid%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			bonusBuffTemplate.buffIDs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}	

	}
	return 1;
}

UInt32 GlobalVarManager::initmysticalshopcost()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("mysticalshopcost");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"refreshtime%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			mysticalShopCostTemplate.refreshTimeList.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
	}
	return 1;
}


UInt32 GlobalVarManager::initservantshopcost()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("servantshopcost");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		sprintf(dest,"refreshtime%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			servantShopCostTemplate.refreshTimeList.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
	}
	return 1;
}

UInt32 GlobalVarManager::initgainwealth()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("gainwealth");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		if (it->mValue.sKey == "freetimes")
		{
			gainWealth.freeTimes = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
	}
	return 1;
}

UInt32 GlobalVarManager::initGrowthFound()
{
	UInt32 i = 1;
	char dest[64] = {};
	String strData;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("growthfound");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		if (it->mValue.sKey == "viplvneed")
		{
			growthFoundTemplate.viplvNeed = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		if (it->mValue.sKey == "cost")
		{
			growthFoundTemplate.cost = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
	}
	return 1;
}

UInt32 GlobalVarManager::initterritorylucky()
{
	UInt32 i = 1;
	UInt32 ii = 1;
	UInt32 iii = 1;
	char dest[64] = {};
	String strData;
	char dest1[64] = {};
	String strData1;
	char dest2[64] = {};
	String strData2;
	SkeySvalueTemplate *skeySvalueTemplate;
	skeySvalueTemplate = gSkeySvalueTable->get("territorylucky");

	if (skeySvalueTemplate == NULL)
	{
		return 2;
	}
	for (List<SkeySvalue>::Iter*it = skeySvalueTemplate->SkeySvalueList.begin();it != NULL;it = skeySvalueTemplate->SkeySvalueList.next(it))
	{
		

		if (it->mValue.sKey == "maxtimes")
		{
			territoryLuckyTemplate.maxtimes = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		if (it->mValue.sKey == "recovertime")
		{
			territoryLuckyTemplate.recovertime = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		if (it->mValue.sKey == "challage")
		{
			territoryLuckyTemplate.challage = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		if (it->mValue.sKey == "freerefresh")
		{
			territoryLuckyTemplate.freerefresh = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}
		if (it->mValue.sKey == "freerefreshtimes")
		{
			territoryLuckyTemplate.freerefreshtimes = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}


		sprintf(dest,"refresh%d",i);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			i++;
			territoryLuckyTemplate.refreshs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}

		sprintf(dest,"cost%d",ii);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			ii++;
			territoryLuckyTemplate.costs.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		sprintf(dest,"buytimes%d",iii);
		strData = (String)dest;

		if (it->mValue.sKey == strData)
		{
			iii++;
			territoryLuckyTemplate.buytimess.insertTail(lynxAtoi<UInt32>(it->mValue.sValue.c_str()));
		}
		if (it->mValue.sKey == "rmbrefresh")
		{
			territoryLuckyTemplate.rmbrefresh = lynxAtoi<UInt32>(it->mValue.sValue.c_str());
		}


	}
	return 1;
}
