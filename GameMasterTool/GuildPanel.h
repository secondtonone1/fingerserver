#ifndef __LYNX_GAME_MASTER_TOOL_GUILD_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_GUILD_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXlib.h"

namespace Lynx
{
    class GuildPanel : public wxPanel
    {
        DECLARE_CLASS(GuildPanel)
    public:
        GuildPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~GuildPanel();

        enum ColType
        {
            COL_TYPE_GUILD_ID = 0,
            COL_TYPE_GUILD_NAME,
            COL_TYPE_GUILD_LEVEL,
            COL_TYPE_GUILD_MEMBER_COUNT,
            COL_TYPE_GUILD_ONLINE_MEMBER_COUNT,
            COL_TYPE_MAX_COUNT,
        };

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        void onSelected(wxListEvent& ev);
        void onActived(wxListEvent& ev);
        void onRightClicked(wxListEvent& ev);
        void onColClicked(wxListEvent& ev);
        void onShowGuildInfoDialog(wxCommandEvent& ev);

    private:
        wxListCtrl* createSearchResultListCtrl();
        wxString getColTypeText(int colType);
        void clearGuildGuid();
        void clearGuildName();
        void clearResultSet(bool isClearTips = true);

    private:
        DECLARE_EVENT_TABLE()
        wxButton* mBtnResetLayout;
        wxButton* mBtnShowOnline;
        wxButton* mBtnClearResult;
        wxTextCtrl* mTextCtrlGuildGuid;
        wxButton* mBtnSearchGuildGuid;
        wxButton* mBtnClearGuildGuid;
        wxTextCtrl* mTextCtrlGuildName;
        wxButton* mBtnSearchGuildName;
        wxButton* mBtnClearGuildName;
        wxStaticText* mTextResultTips;
        wxListCtrl* mListResultSet;
        wxMenu* mOperateMenu;
        bool mSortFlag[COL_TYPE_MAX_COUNT];
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_GUILD_PANEL_H__


