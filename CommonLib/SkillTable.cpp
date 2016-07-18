#include "SkillTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
SkillTable::loadFromDbc(const String& fileName)
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
SkillTable::reloadFromDbc(const String& fileName)
{
    SkillTemplateMap tmpMap = mMap;
    clear();
    if (!loadFromDbc(fileName))
    {
        mMap = tmpMap;
        LOG_WARN("Failed to reload Skill table.");
        return false;
    }
    return true;
}

bool 
SkillTable::saveToDbc(const String& filePath)
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
SkillTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
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
		SkillTemplate skillTemplate;

		if (!csvReader.bind("SkillId", skillTemplate.mTemplateId))
		{
			LOG_WARN("Failed to load SkillTable.csv for [SkillId]");
			return false;
		}
        if (!csvReader.bind("Name", skillTemplate.mSkillName))
        {
            LOG_WARN("Failed to load SkillTable.csv for [Name]");
            return false;
        }
        if (!csvReader.bind("DescripeId", skillTemplate.mDescripeId))
        {
            LOG_WARN("Failed to load SkillTable.csv for [DescripeId]");
            return false;
        }
        if (!csvReader.bind("Description", skillTemplate.mDescription))
        {
            LOG_WARN("Failed to load SkillTable.csv for [Description]");
            return false;
        }
        if (!csvReader.bind("Level", skillTemplate.mLevel))
        {
            LOG_WARN("Failed to load SkillTable.csv for [Level]");
            return false;
        }
        //if (!csvReader.bind("CombatType", skillTemplate.mCombatType))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [CombatType]");
        //    return false;
        //}
        //if (!csvReader.bind("AttackType", skillTemplate.mAttackType))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [AttackType]");
        //    return false;
        //}
        //if (!csvReader.bind("Relationship", skillTemplate.mRelationship))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [Relationship]");
        //    return false;
        //}
        if (!csvReader.bind("TriggerAnger", skillTemplate.mTriggerAnger))
        {
            LOG_WARN("Failed to load SkillTable.csv for [TriggerAnger]");
            return false;
        }
        if (!csvReader.bind("IncAnger", skillTemplate.mIncAnger))
        {
            LOG_WARN("Failed to load SkillTable.csv for [IncAnger]");
            return false;
        }
        if (!csvReader.bind("CoolDownRound", skillTemplate.mCoolDownRound))
        {
            LOG_WARN("Failed to load SkillTable.csv for [CoolDownRound]");
            return false;
        }
        if (!csvReader.bind("PhysicsAttack", skillTemplate.mPhysicsAttack))
        {
            LOG_WARN("Failed to load SkillTable.csv for [PhysicsAttack]");
            return false;
        }
        if (!csvReader.bind("MagicAttack", skillTemplate.mMagicAttack))
        {
            LOG_WARN("Failed to load SkillTable.csv for [MagicAttack]");
            return false;
        }
        if (!csvReader.bindFloat("CurePercent", skillTemplate.mCurePercent))
        {
            LOG_WARN("Failed to load SkillTable.csv for [CurePercent]");
            return false;
        }
        if (skillTemplate.mCurePercent < 0.0f)
        {
            skillTemplate.mCurePercent = 0.0f;
        }
        if (!csvReader.bindFloat("SuckPercent", skillTemplate.mSuckPercent))
        {
            LOG_WARN("Failed to load SkillTable.csv for [SuckPercent]");
            return false;
        }
        if (skillTemplate.mSuckPercent < 0.0f)
        {
            skillTemplate.mSuckPercent = 0.0f;
        }
        //if (!csvReader.bind("MultiAttack", skillTemplate.mMultiAttack))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [MultiAttack]");
        //    return false;
        //}
        //if (!csvReader.bind("AttackEffectType", skillTemplate.mAttackEffectType))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [AttackEffectType]");
        //    return false;
        //}
        //if (!csvReader.bind("AttackPoint", skillTemplate.mAttackPoint))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [AttackPoint]");
        //    return false;
        //}
        //if (!csvReader.bind("BindPoint", skillTemplate.mBindPoint))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [BindPoint]");
        //    return false;
        //}
        if (!csvReader.bind("CameraPos", skillTemplate.mCameraPos))
        {
            LOG_WARN("Failed to load SkillTable.csv for [CameraPos]");
            return false;
        }
        //if (!csvReader.bind("MultiNum", skillTemplate.mMultiNum))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [MultiNum]");
        //    return false;
        //}
        //if (!csvReader.bind("FlyerEffect", skillTemplate.mFlyerEffect))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [FlyerEffect]");
        //    return false;
        //}
        //if (!csvReader.bind("BeHitEffect", skillTemplate.mBeHitEffect))
        //{
        //    LOG_WARN("Failed to load SkillTable.csv for [BeHitEffect]");
        //    return false;
        //}
        if (!csvReader.bind("DeadEffect", skillTemplate.mDeadEffect))
        {
            LOG_WARN("Failed to load SkillTable.csv for [DeadEffect]");
            return false;
        }
        if (!csvReader.bind("Icon", skillTemplate.mIcon))
        {
            LOG_WARN("Failed to load SkillTable.csv for [Icon]");
            return false;
        }
        if (!csvReader.bind("BufferId1", skillTemplate.mSkillBuffer1))
        {
            LOG_WARN("Failed to load SkillTable.csv for [BufferId1]");
            return false;
        }
        if (!csvReader.bind("BufferId2", skillTemplate.mSkillBuffer2))
        {
            LOG_WARN("Failed to load SkillTable.csv for [BufferId2]");
            return false;
        }
        if (!csvReader.bind("BufferId3", skillTemplate.mSkillBuffer3))
        {
            LOG_WARN("Failed to load SkillTable.csv for [BufferId3]");
            return false;
        }
	
		mMap.insert(skillTemplate.mTemplateId, skillTemplate);
	}
	fileStream.close();
	return true;
}

