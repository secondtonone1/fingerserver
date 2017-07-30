
#include "BaseChang.h"

using namespace Lynx;


void BaseChangManager::checkStringToValue(UInt32 &dest,Json::Value &src)
{
	UInt32 testSize = -1;
	char cMaxUinttoString[128];

	sprintf(cMaxUinttoString,"%u",testSize);
	std::string sMaxUinttoString = (string)cMaxUinttoString;
	std::string inString = src.asString();

	UInt32 inLength= strlen(inString.c_str());
	UInt32 uintMaxLength =  strlen(sMaxUinttoString.c_str());

	if (uintMaxLength < inLength ||(uintMaxLength == inLength&& sMaxUinttoString < inString))
	{		
		dest = 0;
	}
	else
	{
		dest =  lynxAtoi<UInt32>(inString.c_str());
	}

}
void BaseChangManager::checkStringToValue(UInt64 &dest,Json::Value &src)
{
	UInt64 testSize = -1;
	char cMaxUinttoString[128];

	sprintf(cMaxUinttoString,"%llu",testSize);
	std::string sMaxUinttoString = (string)cMaxUinttoString;
	std::string inString = src.asString();

	UInt32 inLength= strlen(inString.c_str());
	UInt32 uintMaxLength =  strlen(sMaxUinttoString.c_str());

	if (uintMaxLength < inLength ||(uintMaxLength == inLength&& sMaxUinttoString < inString))
	{		
		dest = 0;
	}
	else
	{
		dest =  lynxAtoi<UInt64>(inString.c_str());
	}

}


void BaseChangManager::cGChapterEndconvertJsonToData(CGChapterEnd &chapterEnd)
{
	Json::Reader reader;    
	Json::Value root;    
	if (reader.parse(chapterEnd.jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
	{

		checkStringToValue(chapterEnd.chapterID,root["chapterID"]);
		checkStringToValue(chapterEnd.result,root["result"]);
		checkStringToValue(chapterEnd.star,root["star"]);

		AwardMonsterDamage awardMonsterDamage;
		for(int i =0; i <root["awardMonsterList"].size();i++)
		{
			checkStringToValue(awardMonsterDamage.ID,root["awardMonsterList"][i]["ID"]);
			checkStringToValue(awardMonsterDamage.damageType,root["awardMonsterList"][i]["damageType"]);
			checkStringToValue(awardMonsterDamage.times,root["awardMonsterList"][i]["times"]);

			chapterEnd.awardMonsterList.insertTail(awardMonsterDamage);
		}

		for(int i =0; i <root["fireConfirmData"].size();i++)
		{
			FireConfirm fireConfirm;
			checkStringToValue(fireConfirm.index ,root["fireConfirmData"][i]["index"]);
			checkStringToValue(fireConfirm.count,root["fireConfirmData"][i]["count"]);
			checkStringToValue(fireConfirm.keyValue.key,root["fireConfirmData"][i]["key"]);
			checkStringToValue(fireConfirm.keyValue.value,root["fireConfirmData"][i]["value"]);

			for(int ii =0; ii <root["fireConfirmData"][i]["groupList"].size();ii++)
			{
				IndexList indexList;
				checkStringToValue(indexList.subIndex,root["fireConfirmData"][i]["groupList"][ii]["subIndex"]);

				for(int iii =0; iii <root["fireConfirmData"][i]["groupList"][ii]["valueList"].size();iii++)
				{	
					UInt64 tmp = 0;
					checkStringToValue(tmp,root["fireConfirmData"][i]["groupList"][ii]["valueList"][iii]);
					float ftmp = (float)tmp;
					indexList.valueList.insertTail(ftmp);
				}
				fireConfirm.groupList.insertTail(indexList);
			}
			chapterEnd.fireConfirmData.insertTail(fireConfirm);
		}
	}
}



void BaseChangManager::rankGameEndconvertJsonToData(RankGameEndReq &rankGameEndReq)
{
	Json::Reader reader;    
	Json::Value root;    
	if (reader.parse(rankGameEndReq.strReceive, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
	{
		checkStringToValue(rankGameEndReq.result,root["result"]);
	
		for(int i =0; i <root["fireConfirmData"].size();i++)
		{
			FireConfirm fireConfirm;
			checkStringToValue(fireConfirm.index ,root["fireConfirmData"][i]["index"]);
			checkStringToValue(fireConfirm.count,root["fireConfirmData"][i]["count"]);
			checkStringToValue(fireConfirm.keyValue.key,root["fireConfirmData"][i]["key"]);
			checkStringToValue(fireConfirm.keyValue.value,root["fireConfirmData"][i]["value"]);

			for(int ii =0; ii <root["fireConfirmData"][i]["groupList"].size();ii++)
			{
				IndexList indexList;
				checkStringToValue(indexList.subIndex,root["fireConfirmData"][i]["groupList"][ii]["subIndex"]);

				for(int iii =0; iii <root["fireConfirmData"][i]["groupList"][ii]["valueList"].size();iii++)
				{	
					UInt32 tmp = 0;
					checkStringToValue(tmp,root["fireConfirmData"][i]["groupList"][ii]["valueList"][iii]);
					indexList.valueList.insertTail(tmp);
				}
				fireConfirm.groupList.insertTail(indexList);
			}
			rankGameEndReq.fireConfirmData.insertTail(fireConfirm);
		}
	}
}
