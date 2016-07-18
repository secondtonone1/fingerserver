#ifndef __LYNX_EXCEL_TO_DBC_TOOL_MAIN_FRAME_H__
#define __LYNX_EXCEL_TO_DBC_TOOL_MAIN_FRAME_H__

#include "WXLib.h"
#include "PlatformLib.h"

namespace Lynx
{
    class MainFrame : public wxFrame
    {
        DECLARE_CLASS(MainFrame)
    public:
        MainFrame(wxFrame* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);
        virtual ~MainFrame();

        bool Initial();
        void Release();

        void OnExportSelected(wxCommandEvent& ev);
        void OnSelectAll(wxCommandEvent& ev);
        void OnUnSelectAll(wxCommandEvent& ev);

    private:
        DECLARE_EVENT_TABLE()
        wxAuiManager m_AuiManager;
        wxPanel* m_CommandPanel;
        wxButton* m_ExportSelected;
        wxButton* m_SelectAll;
        wxButton* m_UnSelectAll;
    };
} // namespace Lynx

#endif // __LYNX_EXCEL_TO_DBC_TOOL_MAIN_FRAME_H__

