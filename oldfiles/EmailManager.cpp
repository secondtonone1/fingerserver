#include "EmailManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

EmailManager::EmailManager() : mPlayer(NULL), mPlayerEmailData(NULL)
{

}


EmailManager::~EmailManager()
{

}

bool 
EmailManager::initial(Player* player)
{
   //邮件信息保存在playerdata里，目前被我注释了sec
	/* if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
    mPlayerEmailData = &mPlayer->mPlayerData.mEmailData;

    for (List<EmailData>::Iter* iter = mPlayerEmailData->mEmailList.begin();
        iter != NULL; iter = mPlayerEmailData->mEmailList.next(iter))
    {
        mEmailDataMap.insert(iter->mValue.mEmailId, &iter->mValue);
    }*/

    return true;
}

void 
EmailManager::release()
{
    mEmailDataMap.clear();
}

const EmailData* 
EmailManager::createEmail(UInt8 source, const String& theme, 
                          const String& content, const List<EmailItem> itemList)
{
    EmailData emailData;
    emailData.mEmailId = LogicSystem::getSingleton().generateEmailGuid();
    emailData.mTheme = theme;
    emailData.mContent = content;
    emailData.mTime = TimeUtil::getTimeSec();
    emailData.mPlayerGuid = mPlayer->getPlayerData().mBaseData.m_nPlayerID;
    emailData.mItemList = itemList;
    emailData.mIsOpened = false;

    List<EmailData>::Iter* iter = mPlayerEmailData->mEmailList.insertTail(emailData);
    mEmailDataMap.insert(emailData.mEmailId, &iter->mValue);

    PersistInsertEmailNotify notify;
    notify.mEmailData = emailData;
    PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());

    AddEmailNotify addEmailNotify;
    addEmailNotify.mEmailData = emailData;
    mPlayer->sendMsgWithAck(addEmailNotify);
    
    return &iter->mValue;
}

ErrorId 
EmailManager::destroyEmail(const Guid& emailId)
{
    Map<Guid, EmailData*>::Iter* iter = mEmailDataMap.find(emailId);
    if (!iter)
    {
        LOG_WARN("Failed to get email data %llu", emailId);
        return LynxErrno::EmailNotExist;
    }

    mEmailDataMap.erase(iter);
    for (List<EmailData>::Iter* jter = mPlayerEmailData->mEmailList.begin();
        jter != NULL; jter = mPlayerEmailData->mEmailList.next(jter))
    {
        if (jter->mValue.mEmailId == emailId)
        {
            mPlayerEmailData->mEmailList.erase(jter);
            break;
        }
    }

    PersistDelEmailNotify notify;
    notify.mEmailId = emailId;
    PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());

    DelEmailNotify delEmailNotify;
    delEmailNotify.mEmailId = emailId;
    mPlayer->sendMsgWithAck(delEmailNotify);

    return LynxErrno::None;
}

const EmailData* 
EmailManager::getEmail(const Guid& emailId)
{
    Map<Guid, EmailData*>::Iter* iter = mEmailDataMap.find(emailId);
    if (!iter)
    {
        LOG_WARN("Failed to get email data %llu", emailId);
        return NULL;
    }
    return iter->mValue;
}

ErrorId 
EmailManager::takeEmailItem(const Guid& emailId)
{
    Map<Guid, EmailData*>::Iter* iter = mEmailDataMap.find(emailId);
    if (!iter)
    {
        LOG_WARN("Failed to get email data %llu", emailId);
        return LynxErrno::EmailNotExist;
    }

    EmailData* emailData = iter->mValue;

    if (!emailData)
    {
        LOG_WARN("EmailData pointer is null.");
        return LynxErrno::EmailNotExist;
    }

    for (List<EmailItem>::Iter* jter = emailData->mItemList.begin();
        jter != NULL; jter = emailData->mItemList.next(jter))
    {
        EmailItem& emailItem = jter->mValue;

        if (!checkTemplateInvalid(jter->mValue.mTemplateId, jter->mValue.mType))
        {
            LOG_WARN("Failed to check template invalid %u, %u", jter->mValue.mTemplateId, jter->mValue.mType);
            return LynxErrno::TableError;
        }
    }

    // 执行领取操作
    for (List<EmailItem>::Iter* jter = emailData->mItemList.begin();
        jter != NULL; jter = emailData->mItemList.next(jter))
    {
        EmailItem& emailItem = jter->mValue;

        switch (emailItem.mType)
        {
        case EQUIP_TYPE:
            mPlayer->getEquipManager().createEquip(jter->mValue.mTemplateId, 0);
            break;
        case ITEM_TYPE:
            mPlayer->getItemManager().createItem(jter->mValue.mTemplateId, jter->mValue.mCount, 0);
            break;
        case RUNE_TYPE:
            mPlayer->getRuneManager().createRune(jter->mValue.mTemplateId, 0);
            break;
        case HERO_TYPE:
            mPlayer->getHeroManager().createHero(jter->mValue.mTemplateId);
            break;
        case PET_TYPE:
//           mPlayer->getPetManager().createPet(jter->mValue.mTemplateId);
            break;
        case HERO_DEBRIS_TYPE:
            mPlayer->getHeroDebrisManager().createHeroDebris(jter->mValue.mTemplateId, jter->mValue.mCount, 0);
            break;
        case PET_DEBRIS_TYPE:
            mPlayer->getPetDebrisManager().createPetDebris(jter->mValue.mTemplateId, jter->mValue.mCount, 0);
            break;
        };
    }

    destroyEmail(emailId);
    return LynxErrno::None;
}


void 
EmailManager::openEmail(const Guid& emailId)
{
    Map<Guid, EmailData*>::Iter* iter = mEmailDataMap.find(emailId);
    if (!iter)
    {
        LOG_WARN("Failed to get email data %llu", emailId);
        return;
    }

    EmailData* emailData = iter->mValue;

    if (emailData)
    {
        emailData->mIsOpened = true;

        PersistUpdateEmailNotify notify;
        notify.mEmailId = emailData->mEmailId;
        notify.mIsOpened = true;

        PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());
    }
}

bool 
EmailManager::checkTemplateInvalid(TemplateId templateId, UInt8 itemType)
{
    switch (itemType)
    {
    case EQUIP_TYPE:
        {
            EquipTemplate* equipTemplate = EQUIP_TABLE().get(templateId);
            if (equipTemplate == NULL)
            {
                LOG_WARN("Get equip template failed templateId[%u]", templateId);
                return false;
            }
        }
    	break;
    case ITEM_TYPE:
        {
            ItemTemplate* itemTemplate = ITEM_TABLE().get(templateId);
            if (itemTemplate == NULL)
            {
                LOG_WARN("Get item template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case HERO_TYPE:
        {
            HeroTemplate* heroTemplate = HERO_TABLE().get(templateId);
            if (heroTemplate == NULL)
            {
                LOG_WARN("Get hero template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case HERO_DEBRIS_TYPE:
        {
            HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(templateId);
            if (heroDebrisTemplate == NULL)
            {
                LOG_WARN("Get hero debris template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case PET_TYPE:
        {
            PetTemplate* petTemplate = PET_TABLE().get(templateId);
            if (petTemplate == NULL)
            {
                LOG_WARN("Get pet template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case PET_DEBRIS_TYPE:
        {
            PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(templateId);
            if (petDebrisTemplate == NULL)
            {
                LOG_WARN("Get pet debris template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case RUNE_TYPE:
        {
            RuneTemplate* runeTemplate = RUNE_TABLE().get(templateId);
            if (runeTemplate == NULL)
            {
                LOG_WARN("Get rune template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    default:
        LOG_WARN("Unknown template type[%u] id[%u]", itemType, templateId);
        return false;
    }

    return true;
}