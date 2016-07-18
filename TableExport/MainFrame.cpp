#include "MainFrame.h"
#include "TableExport.h"
#include "MainPanel.h"

using namespace Lynx;

IMPLEMENT_CLASS(MainFrame, wxFrame)
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(myID_MAIN_FRAME_EDIT_EXPORT_SELECTED_BUTTON, MainFrame::OnExportSelected)
EVT_BUTTON(myID_MAIN_FRAME_EDIT_SELECT_ALL_BUTTON, MainFrame::OnSelectAll)
EVT_BUTTON(myID_MAIN_FRAME_EDIT_UNSELECT_ALL_BUTTON, MainFrame::OnUnSelectAll)
END_EVENT_TABLE()

MainFrame::MainFrame(wxFrame* parent, wxWindowID id, const wxString& title,
                     const wxPoint& pos, const wxSize& size, long style, const wxString& name)
                     : wxFrame(parent, id, title, pos, size, style | wxTAB_TRAVERSAL, name)
{

}

MainFrame::~MainFrame()
{
    Release();
}

bool 
MainFrame::Initial()
{
    Centre();
    Show(true);
    Update();

    m_AuiManager.SetManagedWindow(this);

    // 创建主面板
    MainPanel* mainPanel = new MainPanel(this, myID_MAIN_PANEL,
        wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mainPanel);
    if (!mainPanel->Initial())
    {
        LOG_WARN("Failed to initial main panel.");
        delete mainPanel;
        return false;
    }

    m_AuiManager.AddPane(mainPanel, wxAuiPaneInfo().Center().CloseButton(false).
        Floatable(false).MaximizeButton(false).CaptionVisible(false));

    // 创建命令面板
    m_CommandPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 45));
    assert(m_CommandPanel);

    m_AuiManager.AddPane(m_CommandPanel, wxAuiPaneInfo().Name(wxT("命令面板")).
        Bottom().Layer(1).Position(1).CloseButton(false).
        MaximizeButton(false).Floatable(false).CaptionVisible(false));

    // 初始化按钮
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(buttonSizer, 0, wxALIGN_CENTER);

    m_ExportSelected = new wxButton(m_CommandPanel, myID_MAIN_FRAME_EDIT_EXPORT_SELECTED_BUTTON, wxT("导出选中"), wxDefaultPosition, wxDefaultSize);
    m_SelectAll = new wxButton(m_CommandPanel, myID_MAIN_FRAME_EDIT_SELECT_ALL_BUTTON, wxT("全选"), wxDefaultPosition, wxDefaultSize);
    m_UnSelectAll = new wxButton(m_CommandPanel, myID_MAIN_FRAME_EDIT_UNSELECT_ALL_BUTTON, wxT("取消全选"), wxDefaultPosition, wxDefaultSize);


    buttonSizer->Add(m_ExportSelected, 0, wxALL, 10);
    buttonSizer->Add(m_SelectAll, 0, wxALL, 10);
    buttonSizer->Add(m_UnSelectAll, 0, wxALL, 10);

    m_CommandPanel->SetSizer(topSizer);
    topSizer->Fit(m_CommandPanel);
    topSizer->SetSizeHints(m_CommandPanel);

    m_AuiManager.Update();
    return true;
}

void
MainFrame::Release()
{
    m_AuiManager.UnInit();
}

void
MainFrame::OnExportSelected(wxCommandEvent& ev)
{
    DATA().ExportAllSelected();
}

void
MainFrame::OnSelectAll(wxCommandEvent& ev)
{
    DATA().SelectAllExcel();
}

void 
MainFrame::OnUnSelectAll(wxCommandEvent& ev)
{
    DATA().SelectAllExcel(false);
}