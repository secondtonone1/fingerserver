#ifndef __LYNX_GAME_MASTER_TOOL_NOTICE_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_NOTICE_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

#define NOTICE_CONTENT_SIZE_LIMIT 20

namespace Lynx
{
    class NoticePanel : public wxPanel
    {
        DECLARE_CLASS(EmailPanel)
    public:
        NoticePanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~NoticePanel();

        enum ResultColType
        {
            RESULT_COL_TYPE_ID    = 0,
            RESULT_COL_TYPE_START_TIME,
            RESULT_COL_TYPE_CYCLE_COUNT,
            RESULT_COL_TYPE_CONTENT,
            RESULT_COL_TYPE_MAX_COUNT,
        };

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

        void onPublishClicked(wxCommandEvent& ev);

    private:
        wxListCtrl* createSearchResultListCtrl();
        wxString getResultColTypeText(int resultColType);
        


    private:
        DECLARE_EVENT_TABLE()
        wxTextCtrl* mTextCtrlContent;
        wxTextCtrl* mTextCtrlStartTime;
        wxTextCtrl* mTextCtrlCycleCount;
        wxButton* mBtnPublish;
        wxButton* mBtnSearch;
        wxListCtrl* mListSearchResult;
        wxStaticText* mTextResultTips;
        wxMenu* mResultOperateMenu;
        bool mSortFlag[RESULT_COL_TYPE_MAX_COUNT];
    };
} // namespace KGame

#endif // __LYNX_GAME_MASTER_TOOL_NOTICE_PANEL_H__