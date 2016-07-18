#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"
#include "CopyFinishTime.h"

using namespace Lynx;


bool 
CopyFinishTable::loadFromDbc(const String& fileName)
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
CopyFinishTable::reloadFromDbc(const String& fileName)
{
	List<CopyFinishTemplate> tmpList = mList;
	mList.clear();
	if (!loadFromDbc(fileName))
	{
		mList = tmpList;
		LOG_WARN("Failed to reload Skill table.");
		return false;
	}
	return true;
}



bool 
CopyFinishTable::loadFromCsv(const String& filePath)
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
		
		CopyFinishTemplate copyfinishtemplate;

// 		if (!csvReader.bind("copyid", copyfinishtemplate.CopyID))
// 		{
// 			LOG_WARN("Failed to load copyfinishtime.csv for [copyid]");
// 			return false;
// 		}
// 		if (!csvReader.bind("num", copyfinishtemplate.Num))
// 		{
// 			LOG_WARN("Failed to load copyfinishtime.csv for [num]");
// 			return false;
// 		}
// 		if (!csvReader.bind("maxattack", copyfinishtemplate.MaxAttack))
// 		{
// 			LOG_WARN("Failed to load copyfinishtime.csv for [maxattack]");
// 			return false;
// 		}
// 		if (!csvReader.bind("finishtime", copyfinishtemplate.FinishTime))
// 		{
// 			LOG_WARN("Failed to load copyfinishtime.csv for [finishtime]");
// 			return false;
// 		}


		mList.insertTail(copyfinishtemplate);
	}
	fileStream.close();
	return true;
}

bool CopyFinishTable::saveToDbc(const String& filePath)
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
