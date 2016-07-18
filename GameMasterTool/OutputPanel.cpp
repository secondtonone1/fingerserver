#include "OutputPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(OutputPanel, wxTextCtrl)

BEGIN_EVENT_TABLE(OutputPanel, wxTextCtrl)

END_EVENT_TABLE()


OutputPanel::OutputPanel(wxWindow* parent)
: wxTextCtrl(parent, wxID_ANY, wxT(""), wxPoint(0, 0), wxSize(150, 80), wxNO_BORDER | wxTE_MULTILINE)
{

}

OutputPanel::~OutputPanel()
{
    release();
}

bool 
OutputPanel::initial()
{
    //SetBackgroundColour(*wxBLACK);
    //SetForegroundColour(*wxWHITE);

    registerEvents();

    Disable();
    return true;
}

void 
OutputPanel::release()
{
    deregisterEvents();
}

void 
OutputPanel::registerEvents()
{
    REGISTER_EVENT(WindowLogPrintEvent, this, &OutputPanel::onWindowLogPrintEvent);
    REGISTER_EVENT(WindowLogClearEvent, this, &OutputPanel::onWindowLogClearEvent);
    REGISTER_EVENT(ServerSelectEvent, this, &OutputPanel::onServerSelectEvent);
    REGISTER_EVENT(LoginSuccessEvent, this, &OutputPanel::onLoginSuccessEvent);
    REGISTER_EVENT(LoginBrokenEvent, this, &OutputPanel::onLoginBrokenEvent);
}

void
OutputPanel::deregisterEvents()
{
    DEREGISTER_EVENT(WindowLogPrintEvent, this, &OutputPanel::onWindowLogPrintEvent);
    DEREGISTER_EVENT(WindowLogClearEvent, this, &OutputPanel::onWindowLogClearEvent);
    DEREGISTER_EVENT(ServerSelectEvent, this, &OutputPanel::onServerSelectEvent);
    DEREGISTER_EVENT(LoginSuccessEvent, this, &OutputPanel::onLoginSuccessEvent);
    DEREGISTER_EVENT(LoginBrokenEvent, this, &OutputPanel::onLoginBrokenEvent);
}

void 
OutputPanel::resetLayout()
{
    Clear();
}

void 
OutputPanel::onWindowLogPrintEvent(const WindowLogPrintEvent& ev)
{
    Int32 lineNumber = GetNumberOfLines();
    if (lineNumber >= 100)
    {
        Remove(0, GetLineLength(0) + 1);
    }

    AppendText(wxString(ev.mLogContents.c_str()));
    AppendText(wxString("\r\n"));
}

void 
OutputPanel::onWindowLogClearEvent(const WindowLogClearEvent& ev)
{
    Clear();
}

void
OutputPanel::onServerSelectEvent(const ServerSelectEvent& ev)
{
    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    if (gmClient->getServerName() != ev.mServerName)
    {
        return;
    }

    if (ev.mSelected)
    {
        if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
        {
            Enable();
            resetLayout();
        }
        else
        {
            Disable();
        }
    }
    else
    {
        Disable();
    }
}

void 
OutputPanel::onLoginSuccessEvent(const LoginSuccessEvent& ev)
{
    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    if (gmClient->getServerName() != ev.mServerName)
    {
        return;
    }

    if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
    {
        Enable();
        resetLayout();
    }
    else
    {
        resetLayout();
        Disable();
    }
}

void 
OutputPanel::onLoginBrokenEvent(const LoginBrokenEvent& ev)
{
    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    if (gmClient->getServerName() != ev.mServerName)
    {
        return;
    }

    if (gmClient->getLoginState() != GMClient::LOGINED_STATE)
    {
        resetLayout();
        Disable();
    }
}
