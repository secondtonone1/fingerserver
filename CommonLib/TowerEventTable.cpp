#include "TypeConditionTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;
// 
// bool 
// TypeConditionTable::loadFromDbc(const String& fileName)
// {
// 	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
// 	Int32 length = RESOURCE_GROUP_MANAGER().loadFileToStream(fileName, 
// 		streamBuffer, "Data");
// 	if (length <= 0)
// 	{
// 		LOG_WARN("Failed to load file %s", fileName.c_str());
// 		return false;
// 	}
// 	if (!unserialize(*this, streamBuffer, 0))
// 	{
// 		LOG_WARN("Failed to unserialize file %s", fileName.c_str());
// 		return false;
// 	}
// 	return true;
// }
// 
// bool 
// TypeConditionTable::reloadFromDbc(const String& fileName)
// {
// 	TypeConditionTemplateMap tmpMap = mMap;
// 	clear();
// 	if (!loadFromDbc(fileName))
// 	{
// 		mMap = tmpMap;
// 		LOG_WARN("Failed to reload TypeConditionTable table.");
// 		return false;
// 	}
// 	return true;
// }
// 
// bool 
// TypeConditionTable::saveToDbc(const String& filePath)
// {
// 	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
// 	serialize(*this, streamBuffer, 0);
// 	if (!FileUtil::saveFile(filePath, streamBuffer))
// 	{
// 		LOG_WARN("Failed to save file %s.", filePath.c_str());
// 		return false;
// 	}
// 	return true;
// }
// 
// bool 
// TypeConditionTable::loadFromCsv(const String& filePath)
// {
// 	mMap.clear();
// 	std::ifstream fileStream(filePath.c_str());
// 	if (fileStream.fail())
// 	{
// 		LOG_WARN("Failed to open %s file.", filePath.c_str());
// 		return false;
// 	}
// 	CsvReader csvReader(fileStream, ",");
// 	csvReader.initTitle();
// 	while (csvReader.readLine())
// 	{
// 		TypeConditionTemplate TypeConditionTemplate;
// 		// ¹«ÓÃ
// 		if (!csvReader.bind("id", TypeConditionTemplate.id))
// 		{
// 			LOG_WARN("Failed to load TypeConditionTemplate.csv for [id]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bind("name", TypeConditionTemplate.name))
// 		{
// 			LOG_WARN("Failed to load TypeConditionTemplate.csv for [name]");
// 			return false;
// 		}
// 
// 		char str1[32]={};
// 		char str2[32]={};
// 		for (int i=1;i<50;i++)
// 		{
// 			TypeNum typeNum;
// 			sprintf(str1, "%s%d","type",i);
// 			sprintf(str2, "%s%d","num",i);
// 
// 			if (!csvReader.bind(str1, typeNum.type))
// 			{
// 				LOG_WARN("Failed to load Award.csv for [type]");
// 				break;
// 			}
// 			if (typeNum.type == 0)
// 			{
// 				break;
// 			}
// 
// 			if (!csvReader.bind(str2, typeNum.num))
// 			{
// 				LOG_WARN("Failed to load Award.csv for [num]");
// 				break;
// 			}
// 		
// 			TypeConditionTemplate.typeNums.insertTail(typeNum);
// 
// 		}	
// 
// 		
// 		mMap.insert(TypeConditionTemplate.id, TypeConditionTemplate);
// 	}
// 	fileStream.close();
// 	return true;
// }

