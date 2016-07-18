#ifndef __LYNX_GAME_MASTER_TOOL_EMAIL_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_EMAIL_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class EmailPanel : public wxPanel
    {
        DECLARE_CLASS(EmailPanel)
    public:
        EmailPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~EmailPanel();

        enum ItemColType
        {
            ITEM_COL_TYPE_TEMPLATE_ID = 0,
            ITEM_COL_TYPE_ITEM_COUNT,
            ITEM_COL_TYPE_MAX_COUNT,
        };

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        void onRightClicked(wxListEvent& ev);

    private:
        wxListCtrl* createItemListCtrl();
        wxString getItemColTypeText(int itemColType);

    private:
        DECLARE_EVENT_TABLE()
        wxStaticBox* mStaticBoxRecvName;
        wxTextCtrl* mTextCtrlRecvName;
        wxStaticBox* mStaticBoxContents;
        wxTextCtrl* mTextCtrlContents;
        wxStaticBox* mStaticBoxItem;
        wxButton* mBtnAddItem;
        wxButton* mBtnDelAllItem;
        wxListCtrl* mListCtrlItem;
        wxTextCtrl* mTextCtrlItemTemplateId;
        wxTextCtrl* mTextCtrlItemCount;
        wxTextCtrl* mTextCtrlRecord;
        wxButton* mBtnSendEmail;
        wxButton* mBtnClearRecord;
        wxButton* mBtnReset;
        wxMenu* mOperateMenu;
    };
} // namespace KGame

#endif // __LYNX_GAME_MASTER_TOOL_EMAIL_PANEL_H__