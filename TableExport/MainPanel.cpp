#include "MainPanel.h"
#include "TableExport.h"

using namespace Lynx;

IMPLEMENT_CLASS(MainPanel, wxAuiNotebook)

BEGIN_EVENT_TABLE(MainPanel, wxAuiNotebook)
EVT_SIZE(MainPanel::OnSize)
EVT_PG_CHANGED(myID_MAIN_PANEL_PROPERTY_BAR, MainPanel::OnPropertyGridChange)
END_EVENT_TABLE()

MainPanel::MainPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxAuiNotebook(parent, id, pos, size, style)
{
    m_PropertyGrid = NULL;
}

MainPanel::~MainPanel()
{
    Release();
}

bool
MainPanel::Initial()
{
    m_PropertyGrid = new wxPropertyGrid(this, myID_MAIN_PANEL_PROPERTY_BAR, wxDefaultPosition, wxDefaultSize,
        wxPG_SPLITTER_AUTO_CENTER |
        wxPG_BOLD_MODIFIED );
    RegisterMsgHandler();

    BuildPropertyGrid();
    return true;
}

void
MainPanel::Release()
{
    if (m_PropertyGrid)
    {
        delete m_PropertyGrid;
        m_PropertyGrid = NULL;
    }   
}

void 
MainPanel::RegisterMsgHandler()
{
    REGISTER_MSG_HANDLER(ReBuildMainPanelNotify, this, &MainPanel::OnReBuildMainPanelNotify);
}

void 
MainPanel::OnSize(wxSizeEvent& ev)
{
    if (!m_PropertyGrid)
    {
        return;
    }
    m_PropertyGrid->SetSize(ev.GetSize());
}

void 
MainPanel::BuildPropertyGrid()
{
    if (!m_PropertyGrid)
    {
        return;
    }
    // Çå¿ÕÃæ°å
    m_PropertyGrid->Clear();

    wxPGProperty* prop = NULL;
    for (FileSelectedMap::Iter* iter = DATA().m_ExcelSelectedMap.begin(); iter != DATA().m_ExcelSelectedMap.end();
        iter = DATA().m_ExcelSelectedMap.next(iter))
    {
        if (!iter)
        {
            continue;
        }

        prop = m_PropertyGrid->Append(new wxBoolProperty(iter->mKey.c_str(), wxPG_LABEL, iter->mValue));
        m_PropertyGrid->SetPropertyAttribute(prop, wxPG_BOOL_USE_CHECKBOX, true);
    }
}

void 
MainPanel::OnPropertyGridChange(wxPropertyGridEvent& ev)
{
    if (!m_PropertyGrid)
    {
        return;
    }

    wxPGProperty* pgProperty = ev.GetProperty();
    if (pgProperty == NULL)
    {
        return;
    }

    String key = pgProperty->GetName().c_str();
    FileSelectedMap::Iter* iter = DATA().m_ExcelSelectedMap.find(key);
    if (iter == DATA().m_ExcelSelectedMap.end())
    {
        return;
    }

    iter->mValue = m_PropertyGrid->GetPropertyValueAsBool(key.c_str());
}

void 
MainPanel::OnReBuildMainPanelNotify(const ReBuildMainPanelNotify& notify)
{
    BuildPropertyGrid();
}


