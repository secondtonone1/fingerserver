#ifndef __LYNX_GAME_MASTER_TOOL_FORBIDDEN_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_FORBIDDEN_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXlib.h"

namespace Lynx
{
    class ForbiddenPanel : public wxListCtrl
    {
        DECLARE_CLASS(ForbiddenPanel)
    public:
        ForbiddenPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxLC_REPORT | wxLC_SINGLE_SEL);
        virtual ~ForbiddenPanel();

        enum ColType
        {
            COL_TYPE_FORBIDDEN_TYPE = 0,
            COL_TYPE_FORBIDDEN_START_TIME, 
            COL_TYPE_FORBIDDEN_END_TIME,
            COL_TYPE_FORBIDDEN_ID,
            COL_TYPE_FORBIDDEN_NAME,
            COL_TYPE_FORBIDDEN_ACCOUNT_ID,
            COL_TYPE_FORBIDDEN_ACCOUNT_NAME,
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

    private:
        wxString getColTypeText(int colType);

    private:
        DECLARE_EVENT_TABLE()
        wxMenu* mOperateMenu;
        bool mSortFlag[COL_TYPE_MAX_COUNT];
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_FORBIDDEN_PANEL_H__


