#include "CommandPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(CommandPanel, wxPanel)

BEGIN_EVENT_TABLE(CommandPanel, wxPanel)
EVT_TEXT_ENTER(myID_COMMAND_INPUT_TEXT_CTRL, onInputTextCtrlEntered)
EVT_BUTTON(myID_COMMAND_ENTER_BUTTON, onEnterButtonClicked)
END_EVENT_TABLE()

CommandPanel::CommandPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    mInputTextCtrl = NULL;
    mEnterButton = NULL;
    mHistoryCmdList.clear();
}

CommandPanel::~CommandPanel()
{
    release();
}

bool
CommandPanel::initial()
{
    mIndex = 0;

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(subSizer, wxSizerFlags(1).Expand());
    SetSizer(topSizer);

    mInputTextCtrl = new wxTextCtrl(this, myID_COMMAND_INPUT_TEXT_CTRL, wxT(""), wxDefaultPosition,  wxSize(0, 20), wxTE_PROCESS_ENTER);
    subSizer->Add(mInputTextCtrl, wxSizerFlags(1));

    mInputTextCtrl->Connect(wxEVT_KEY_UP, wxKeyEventHandler(CommandPanel::onKeyUpEvent), NULL, this);
    //mInputTextCtrl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CommandPanel::onKeyDownEvent), NULL, this);

    mEnterButton = new wxButton(this, myID_COMMAND_ENTER_BUTTON, wxT("发送"), wxDefaultPosition,  wxSize(100, 20));
    subSizer->Add(mEnterButton, wxSizerFlags(0));

    // 设置发送按钮为窗口默认Btn
    mEnterButton->SetDefault();

    SetSizerAndFit(topSizer);

    registerEvents();

    Disable();
    return true;
}

void
CommandPanel::release()
{
    mInputTextCtrl = NULL;
    mEnterButton = NULL;

    deregisterEvents();
}

void 
CommandPanel::onEnterButtonClicked(wxCommandEvent& ev)
{
    onSendGameCommand();
    ev.Skip();
}

void 
CommandPanel::onInputTextCtrlEntered(wxCommandEvent& ev)
{
    onSendGameCommand();
    ev.Skip();
}

void 
CommandPanel::onSendGameCommand()
{
    // 获取指令字符串
    std::string gmContent = mInputTextCtrl->GetValue().c_str();
    if (gmContent.empty())
    {
        return;
    }

    CommandSystem::getSingleton().execCommand(gmContent.c_str());

    if (mHistoryCmdList.size() >= 100)
    {
        Int32 i = 0;
        for (List<String>::Iter* iter = mHistoryCmdList.r_begin();
            iter != mHistoryCmdList.r_end(); )
        {
            if (i++ >= 50)
            {
                break;
            }
            iter = mHistoryCmdList.erase(iter);
        }
    }
    mHistoryCmdList.insertHead(gmContent.c_str());
    mIndex = 0;

    // 清空输入内容
    mInputTextCtrl->Clear();
}

void 
CommandPanel::registerEvents()
{
    REGISTER_EVENT(ServerSelectEvent, this, &CommandPanel::onServerSelectEvent);
    REGISTER_EVENT(LoginSuccessEvent, this, &CommandPanel::onLoginSuccessEvent);
    REGISTER_EVENT(LoginBrokenEvent, this, &CommandPanel::onLoginBrokenEvent);
}

void
CommandPanel::deregisterEvents()
{
    DEREGISTER_EVENT(ServerSelectEvent, this, &CommandPanel::onServerSelectEvent);
    DEREGISTER_EVENT(LoginSuccessEvent, this, &CommandPanel::onLoginSuccessEvent);
    DEREGISTER_EVENT(LoginBrokenEvent, this, &CommandPanel::onLoginBrokenEvent);
}

void 
CommandPanel::onServerSelectEvent(const ServerSelectEvent& ev)
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
CommandPanel::onLoginSuccessEvent(const LoginSuccessEvent& ev)
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
CommandPanel::onLoginBrokenEvent(const LoginBrokenEvent& ev)
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

void
CommandPanel::resetLayout()
{
    if (mInputTextCtrl)
    {
        mInputTextCtrl->Clear();
    }
}

void
CommandPanel::onKeyUpEvent(wxKeyEvent& ev)
{
    int keyCode = ev.GetKeyCode();

    if (keyCode == 315)
    {
        if (mIndex == 0)
        {
            std::string currentText = mInputTextCtrl->GetValue().c_str();
            if (!currentText.empty())
            {
                mHistoryCmdList.insertHead(currentText.c_str());
                mIndex++;
            }
        }
        mInputTextCtrl->Clear();
        String historyCmd = "";
        if (!mHistoryCmdList.empty())
        {
            List<String>::Iter* iter = mHistoryCmdList.getn(mIndex);
            if (iter)
            {
                historyCmd = iter->mValue;
                mIndex++;
            }
        }
        mInputTextCtrl->AppendText(historyCmd.c_str());
    }
    else if (keyCode == 317)
    {
        mInputTextCtrl->Clear();
        String historyCmd = "";
        if (!mHistoryCmdList.empty())
        {
            if (mIndex > 0)
            {
                List<String>::Iter* iter = mHistoryCmdList.getn(mIndex - 1);
                if (iter)
                {
                    historyCmd = iter->mValue;
                    mIndex--;
                }
            }
        }
        mInputTextCtrl->AppendText(historyCmd.c_str());
    }

    ev.Skip();
}

void 
CommandPanel::onKeyDownEvent(wxKeyEvent& ev)
{
    ev.Skip();
}
