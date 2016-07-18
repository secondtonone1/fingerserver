#include "MallPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(MallPanel, wxListCtrl)

BEGIN_EVENT_TABLE(MallPanel, wxListCtrl)
END_EVENT_TABLE()

MallPanel::MallPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxListCtrl(parent, id, pos, size, style)
{
    
}

MallPanel::~MallPanel()
{
    release();
}

bool
MallPanel::initial()
{
    registerEvents();
    return true;
}

void 
MallPanel::release()
{

    deregisterEvents();
}

void
MallPanel::registerEvents()
{

}

void
MallPanel::deregisterEvents()
{

}

void
MallPanel::resetLayout()
{

}



