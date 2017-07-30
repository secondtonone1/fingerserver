#include "TicketFriendTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
TicketFriendTable::loadFromDbc(const String& fileName)
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
TicketFriendTable::reloadFromDbc(const String& fileName)
{
	TicketFriendTemplateMap tmpMap = mMap;
	TicketFriendLvMap tmpLvMap = mLvMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mLvMap = tmpLvMap;
		LOG_WARN("Failed to reload servantlvup table.");
		return false;
	}
	return true;
}

bool 
TicketFriendTable::saveToDbc(const String& filePath)
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

bool 
TicketFriendTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	mLvMap.clear();

	std::ifstream fileStream(filePath.c_str());
	if (fileStream.fail())
	{
		LOG_WARN("Failed to open %s file.", filePath.c_str());
		return false;
	}
	CsvReader csvReader(fileStream, ",");
	csvReader.initTitle();
	UInt32 totalRate = 0;
	while (csvReader.readLine())
	{
		TicketFriendTemplate ticketFriendTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", ticketFriendTemp.mId))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [id]");
			return false;
		}
		
		if (!csvReader.bind("lv", ticketFriendTemp.mLv))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [lv]");
			return false;
		}
		
		if (!csvReader.bind("rate", ticketFriendTemp.mRate))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [rate]");
			return false;
		}

		if (!csvReader.bind("award1", ticketFriendTemp.mAward1))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [award1]");
			return false;
		}

		ticketFriendTemp.mAwardList.insertTail(ticketFriendTemp.mAward1);

		if (!csvReader.bind("award2", ticketFriendTemp.mAward2))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [award2]");
			return false;
		}

		ticketFriendTemp.mAwardList.insertTail(ticketFriendTemp.mAward2);

		if (!csvReader.bind("award3", ticketFriendTemp.mAward3))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [award1]");
			return false;
		}

		ticketFriendTemp.mAwardList.insertTail(ticketFriendTemp.mAward3);

		if (!csvReader.bind("award4", ticketFriendTemp.mAward4))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [award4]");
			return false;
		}

		ticketFriendTemp.mAwardList.insertTail(ticketFriendTemp.mAward4);

		if (!csvReader.bind("award5", ticketFriendTemp.mAward5))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [award5]");
			return false;
		}
		
		ticketFriendTemp.mAwardList.insertTail(ticketFriendTemp.mAward5);

		if (!csvReader.bind("awardcount3", ticketFriendTemp.mPeapleCount3))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [awardcount3]");
			return false;
		}
		
		if (!csvReader.bind("awardcount2", ticketFriendTemp.mPeapleCount2))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [awardcount2]");
			return false;
		}

		if (!csvReader.bind("awardcount1", ticketFriendTemp.mPeapleCount1))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [awardcount1]");
			return false;
		}

		ticketFriendTemp.mPeapleCountList.insertTail(ticketFriendTemp.mPeapleCount3);
		ticketFriendTemp.mPeapleCountList.insertTail(ticketFriendTemp.mPeapleCount2);
		ticketFriendTemp.mPeapleCountList.insertTail(ticketFriendTemp.mPeapleCount1);

		
		if (!csvReader.bind("count1", ticketFriendTemp.mJoinCount1))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [count1]");
			return false;
		}

		if (!csvReader.bind("count2", ticketFriendTemp.mJoinCount2))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [count2]");
			return false;
		}

		if (!csvReader.bind("count3", ticketFriendTemp.mJoinCount3))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [count3]");
			return false;
		}


		if (!csvReader.bind("support4", ticketFriendTemp.mSupport4))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [support4]");
			return false;
		}

		if (!csvReader.bind("support3", ticketFriendTemp.mSupport3))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [support3]");
			return false;
		}
		
		if (!csvReader.bind("support2", ticketFriendTemp.mSupport2))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [support2]");
			return false;
		}

		if (!csvReader.bind("support1", ticketFriendTemp.mSupport1))
		{
			LOG_WARN("Failed to load ticketfriend.csv for [support1]");
			return false;
		}

		ticketFriendTemp.mSupportList.insertTail(ticketFriendTemp.mSupport1);	
		ticketFriendTemp.mSupportList.insertTail(ticketFriendTemp.mSupport2);
		ticketFriendTemp.mSupportList.insertTail(ticketFriendTemp.mSupport3);
		ticketFriendTemp.mSupportList.insertTail(ticketFriendTemp.mSupport4);

		mMap.insert(ticketFriendTemp.mId, ticketFriendTemp);

		TicketFriendLvMap::Iter* lvIter = mLvMap.find(ticketFriendTemp.mLv);
		if(lvIter)
		{
			totalRate += ticketFriendTemp.mRate;
			ticketFriendTemp.mTag = totalRate;
			lvIter->mValue.insertTail(ticketFriendTemp);
		}
		else
		{
			totalRate = 0;
			totalRate += ticketFriendTemp.mRate;

			ticketFriendTemp.mTag = totalRate;
			List<TicketFriendTemplate> ticketFriendList;
			ticketFriendList.insertTail(ticketFriendTemp);
			mLvMap.insert(ticketFriendTemp.mLv, ticketFriendList);
		}

	}
	fileStream.close();
	return true;
}

