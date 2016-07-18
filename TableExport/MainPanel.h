#ifndef __LYNX_EXCEL_TO_DBC_TOOL_PANEL_H__
#define __LYNX_EXCEL_TO_DBC_TOOL_PANEL_H__

#include "MsgDefine.h"
#include "WXLib.h"

namespace Lynx
{
    class MainPanel : public wxAuiNotebook
    {
        DECLARE_CLASS(MainPanel)
    public:
        MainPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~MainPanel();

        bool Initial();
        void Release();
        void RegisterMsgHandler();

        void OnSize(wxSizeEvent& ev);

        void BuildPropertyGrid();
        void OnPropertyGridChange(wxPropertyGridEvent& ev);
        void OnReBuildMainPanelNotify(const ReBuildMainPanelNotify& notify);

    private:
        wxPropertyGrid* m_PropertyGrid;
        DECLARE_EVENT_TABLE()
    };
} // namespace Lynx

#endif // __LYNX_EXCEL_TO_DBC_TOOL_PANEL_H__