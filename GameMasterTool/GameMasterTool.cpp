#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_APP(GameMasterTool)

BEGIN_EVENT_TABLE(GameMasterTool, wxApp)
EVT_IDLE(GameMasterTool::OnIdle)
EVT_TIMER(myID_TIMER, GameMasterTool::OnTimer)
END_EVENT_TABLE()

Lynx::GameMasterTool* gGmt = NULL;

GameMasterTool::GameMasterTool() : mMainFrame(NULL), mTimer(this, myID_TIMER)
{

}

GameMasterTool::~GameMasterTool()
{

}

bool 
GameMasterTool::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    wxInitAllImageHandlers();

    gGmt = this;

#ifdef _DEBUG
    LOG_INITIAL(Logger::OUTPUT_VSOUTPUT, true, false);
    LOG_SET_OUTPUT_FILE(0);
#else
    LOG_INITIAL(Logger::OUTPUT_FILE, true, false);
    LOG_SET_OUTPUT_FILE("GameMasterTool");
#endif

    if (!ConfigSystem::getSingleton().initial())
    {
        LOG_WARN("Failed to initial config system.");
        return false;
    }

    String assetsFilePath = ConfigSystem::getSingleton().getAssetsFilePath();
    if (!ResourceGroupManager::getSingleton().initial(assetsFilePath, true))
    {
        LOG_WARN("Failed to initial resource group.");
        return false;
    }

    if (!TableManager::getSingleton().load())
    {
        LOG_WARN("Failed to initial table manager.");
        return false;
    }

    if (!EventSystem::getSingleton().initial())
    {
        LOG_WARN("Failed to initial event system.");
        return false;
    }

    if (!LogicSystem::getSingleton().initial())
    {
        LOG_WARN("Failed to initial logic system.");
        return false;
    }

    if (!RecordSystem::getSingleton().initial())
    {
        LOG_WARN("Failed to initial record system.");
        return false;
    }

    if (!CommandSystem::getSingleton().initial())
    {
        LOG_WARN("Failed to initial command system.");
        return false;
    }

    mMainFrame = new MainFrame(NULL, myID_MAIN_FRAME, GetAppName(),
        wxPoint(0, 0), wxSize(1280, 800));
    assert(mMainFrame);

    SetTopWindow(mMainFrame);

    if (!mMainFrame->initial())
    {
        LOG_WARN("Failed to initial main frame.");
        return false;
    }

    mTimer.Start(30);

    return true;
}

int 
GameMasterTool::OnExit()
{
    EventSystem::getSingleton().release();
    LogicSystem::getSingleton().release();
    RecordSystem::getSingleton().release();
    CommandSystem::getSingleton().release();

    return 0;
}

void 
GameMasterTool::OnIdle(wxIdleEvent& ev)
{
    wxApp::OnIdle(ev);
    if (mMainFrame)
    {
        mMainFrame->Update();
    }
}

void 
GameMasterTool::OnSize(wxSizeEvent& ev)
{

}

void 
GameMasterTool::OnTimer(wxTimerEvent& ev)
{
    EventSystem::getSingleton().update();
    LogicSystem::getSingleton().update();
}

