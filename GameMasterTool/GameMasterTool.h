#ifndef __LYNX_GAME_MASTER_TOOL_H__
#define __LYNX_GAME_MASTER_TOOL_H__

#include "MainFrame.h"

#include "LogicSystem.h"
#include "RecordSystem.h"
#include "EventSystem.h"
#include "ConfigSystem.h"
#include "CommandSystem.h"

namespace Lynx
{
    class GameMasterTool : public wxApp
    {
    public:
        GameMasterTool();
        virtual ~GameMasterTool();

        bool OnInit();
        int OnExit();
        void OnIdle(wxIdleEvent& ev);
        void OnSize(wxSizeEvent& ev);
        void OnTimer(wxTimerEvent& ev);

        MainFrame* getMainFrame() { return mMainFrame; }

        DECLARE_EVENT_TABLE()
    
    private:
        MainFrame* mMainFrame;
        wxTimer mTimer;
        wxDateTime mStartTime;
    };

} // namespace Lynx

extern Lynx::GameMasterTool* gGmt;
#define MAIN_FRAME() (*gGmt->getMainFrame())

#endif // __LYNX_GAME_MASTER_TOOL_H__


