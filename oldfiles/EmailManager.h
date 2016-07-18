#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EMAIL_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EMAIL_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class EmailManager : public MainThread
    {
    public:
        EmailManager();
        virtual ~EmailManager();

        bool initial(Player* player);
        void release();

        const EmailData* createEmail(UInt8 source, const String& theme, 
            const String& content, const List<EmailItem> itemList);
        ErrorId destroyEmail(const Guid& emailId);
        const EmailData* getEmail(const Guid& emailId);
        
        ErrorId takeEmailItem(const Guid& emailId);
        void openEmail(const Guid& emailId);

    private:
        bool checkTemplateInvalid(TemplateId templateId, UInt8 itemType);

    private:
        Player* mPlayer;
        PlayerEmailData* mPlayerEmailData;
        Map<Guid, EmailData*> mEmailDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_EMAIL_MAMAGER_H__
