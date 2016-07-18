#include "TableExport.h"
#include "ResourceGroupManager.h"

using namespace Lynx;

IMPLEMENT_APP(TableExport)

BEGIN_EVENT_TABLE(TableExport, wxApp)
EVT_IDLE(TableExport::OnIdle)
//EVT_SIZE(TableExport::OnSize)
EVT_TIMER(myID_TIMER, TableExport::OnTimer)
END_EVENT_TABLE()

Lynx::TableExport* g_Editor = NULL;

TableExport::TableExport() : m_MainFrame(NULL), m_MsgQueue(NULL)
, m_Data(NULL), m_Renderer(NULL), m_Timer(this, myID_TIMER)
{

}

TableExport::~TableExport()
{

}

bool
TableExport::OnInit()
{
    g_Editor = this;

#ifdef _DEBUG
    LOG_INITIAL(Logger::OUTPUT_VSOUTPUT, true, false);
    LOG_SET_OUTPUT_FILE(0);
#else
    LOG_INITIAL(Logger::OUTPUT_FILE, true, false);
    LOG_SET_OUTPUT_FILE("TableExport");
#endif

    m_ConfigManager = new ConfigManager();
    assert(m_ConfigManager);

    if (!m_ConfigManager->initial("TableExport.conf"))
    {
        LOG_WARN("Failed to initial config file");
        return false;
    }

    String resourceConfigPath = m_ConfigManager->getAsString("Assets");
    
     //加载资源组管理
    if (!ResourceGroupManager::getSingleton().initial(resourceConfigPath, true))
    {
        LOG_ERROR("Failed to load resource config file.");
        return false;
    }

    m_MainFrame = new MainFrame(NULL, myID_MAIN_FRAME, GetAppName(),
        wxPoint(0, 0), wxSize(400, 600));
    assert(m_MainFrame);

    SetTopWindow(m_MainFrame);

    m_MsgQueue = new MsgQueue();
    assert(m_MsgQueue);

    m_Data = new Data();
    assert(m_Data);

    try
    {
        if (!m_Data->Initial())
        {
            LOG_WARN("Failed to initial data.");
            return false;
        }

        if (!m_MainFrame->Initial())
        {
            LOG_WARN("Faield to initial main frame.");
            return false;
        }

        if (!m_MsgQueue->Initial())
        {
            LOG_WARN("Failed to initial msg queue.");
            return false;
        }
    }
    catch (std::exception& e)
    {
        LOG_WARN("Failed to initial server debug tool for %s", e.what());
        return false;
    }

    m_Timer.Start(10);

    return true;
}

int
TableExport::OnExit()
{
    if (m_Timer.IsRunning())
    {
        m_Timer.Stop();
    }

    if (m_Data)
    {
        m_Data->Release();
        delete m_Data;
        m_Data = NULL;
    }

    if (m_MsgQueue)
    {
        m_MsgQueue->Release();
        delete m_MsgQueue;
        m_MsgQueue = NULL;
    }

    if (m_ConfigManager)
    {
        m_ConfigManager->release();
        delete m_ConfigManager;
        m_ConfigManager = NULL;
    }
    ResourceGroupManager::getSingleton().release();
    LOG_RELEASE();
    return 0;
}

void
TableExport::OnIdle(wxIdleEvent& ev)
{
    wxApp::OnIdle(ev);
    if (m_MainFrame)
    {
        m_MainFrame->Update();
    }

    m_MsgQueue->Update();
}

void 
TableExport::OnTimer(wxTimerEvent& ev)
{
    m_Data->Update();
}

