#ifndef __LYNX_EXCEL_TO_DBC_TOOL_H__
#define __LYNX_EXCEL_TO_DBC_TOOL_H__

#include "MainFrame.h"
#include "MsgQueue.h"
#include "Renderer.h"
#include "Data.h"
#include "ConfigManager.h"

namespace Lynx
{
    class TableExport : public wxApp
    {
    public:
        TableExport();
        virtual ~TableExport();

        bool OnInit();
        int  OnExit();
        void OnIdle(wxIdleEvent& ev);
        void OnSize(wxSizeEvent& ev);
        void OnTimer(wxTimerEvent& ev);
        DECLARE_EVENT_TABLE()

    public:
        MainFrame* m_MainFrame;
        MsgQueue* m_MsgQueue;
        Renderer* m_Renderer;
        Data* m_Data;
        ConfigManager* m_ConfigManager;
        wxTimer m_Timer;
    };
} // namespace Lynx

extern Lynx::TableExport* g_Editor;
#define MAIN_FRAME() (*g_Editor->m_MainFrame)
#define MSG_QUEUE() (*g_Editor->m_MsgQueue)
#define RENDERER() (*g_Editor->m_Renderer)
#define DATA() (*g_Editor->m_Data)
#define CONFIG_MANAGER() (*g_Editor->m_ConfigManager)

#endif // __LYNX_EXCEL_TO_DBC_TOOL_H__