#ifndef __LYNX_GAME_MASTER_TOOL_PLAYER_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_PLAYER_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXlib.h"

namespace Lynx
{
    class PlayerPanel : public wxPanel
    {
        DECLARE_CLASS(PlayerPanel)
    public:
        PlayerPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~PlayerPanel();

        enum ColType
        {
            COL_TYPE_PLAYER_ID = 0,
            COL_TYPE_PLAYER_NAME,
            COL_TYPE_PLAYER_LEVEL,
            COL_TYPE_PLAYER_ACCOUNT_ID,
            COL_TYPE_PLAYER_ACCOUNT_NAME,
            COL_TYPE_PLAYER_ONLINE,
            COL_TYPE_MAX_COUNT,
        };

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        void onGetPlayerByOnlineEvent(const GetPlayerByOnlineEvent& ev);
        void onGetPlayerByPlayerGuidEvent(const GetPlayerByPlayerGuidEvent& ev);
        void onGetPlayerByPlayerNameEvent(const GetPlayerByPlayerNameEvent& ev);
        void onGetPlayerByAccountGuidEvent(const GetPlayerByAccountGuidEvent& ev);
        void onGetPlayerByAccountNameEvent(const GetPlayerByAccountNameEvent& ev);
        void onGetPlayerDataEvent(const GetPlayerDataEvent& ev);

    private:
        void onSelected(wxListEvent& ev);
        void onActived(wxListEvent& ev);
        void onRightClicked(wxListEvent& ev);
        void onColClicked(wxListEvent& ev);
        void onShowPlayerInfoDialog(wxCommandEvent& ev);
        void onShowOnlinePlayerList(wxCommandEvent& ev);
        void onClearResult(wxCommandEvent& ev);
        void onResetLayout(wxCommandEvent& ev);
        void onCopyPlayerId(wxCommandEvent& ev);
        void onCopyPlayerName(wxCommandEvent& ev);
        void onCopyAccountId(wxCommandEvent& ev);
        void onCopyAccountName(wxCommandEvent& ev);
        void onFindByPlayerId(wxCommandEvent& ev);
        void onClearByPlayerId(wxCommandEvent& ev);
        void onFindByPlayerName(wxCommandEvent& ev);
        void onClearByPlayerName(wxCommandEvent& ev);
        void onFindByAccountId(wxCommandEvent& ev);
        void onClearByAccountId(wxCommandEvent& ev);
        void onFindByAccountName(wxCommandEvent& ev);
        void onClearByAccountName(wxCommandEvent& ev);

    private:
        wxListCtrl* createSearchResultListCtrl();
        wxString getColTypeText(int colType);
        
        void clearPlayerGuid();
        void clearPlayerName();
        void clearAccountGuid();
        void clearAccountName();
        void clearResultSet(bool isClearTips = true);
        bool getResultSetTextContents(long index, ColType colType, std::string& res);

    private:
        DECLARE_EVENT_TABLE()
        wxButton* mBtnResetLayout;
        wxButton* mBtnShowOnline;
        wxButton* mBtnClearResult;
        wxTextCtrl* mTextCtrlPlayerGuid;
        wxButton* mBtnSearchPlayerGuid;
        wxButton* mBtnClearPlayerGuid;
        wxTextCtrl* mTextCtrlPlayerName;
        wxButton* mBtnSearchPlayerName;
        wxButton* mBtnClearPlayerName;
        wxTextCtrl* mTextCtrlAccountGuid;
        wxButton* mBtnSearchAccountGuid;
        wxButton* mBtnClearAccountGuid;
        wxTextCtrl* mTextCtrlAccountName;
        wxButton* mBtnSearchAccountName;
        wxButton* mBtnClearAccountName;
        wxStaticText* mTextResultTips;
        wxListCtrl* mListResultSet;
        wxStaticBox* mResultBox;
        wxMenu* mOperateMenu;
        bool mSortFlag[COL_TYPE_MAX_COUNT];
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_PLAYER_PANEL_H__


