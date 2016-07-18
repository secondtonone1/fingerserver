#ifndef __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_NOTEBOOK_H__
#define __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_NOTEBOOK_H__

#include "PlayerInfoBagPanel.h"
#include "PlayerInfoBasePanel.h"
#include "PlayerInfoHeroPanel.h"
#include "PlayerInfoPetPanel.h"

namespace Lynx
{
    class PlayerInfoNotebook : public wxAuiNotebook
    {
        DECLARE_CLASS(PlayerInfoNotebook);
    public:
        PlayerInfoNotebook(wxWindow *parent = NULL, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~PlayerInfoNotebook();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();
        int getPageIndex(wxWindow* win);

    private:
        DECLARE_EVENT_TABLE()
        PlayerInfoBagPanel* mPlayerInfoBagPanel;
        PlayerInfoBasePanel* mPlayerInfoBasePanel;
        PlayerInfoHeroPanel* mPlayerInfoHeroPanel;
        PlayerInfoPetPanel* mPlayerInfoPetPanel;
    };
} // namespace KGame

#endif  // __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_NOTEBOOK_H__