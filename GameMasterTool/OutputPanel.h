#ifndef __LYNX_GAME_MASTER_OUTPUT_PANEL_H__
#define __LYNX_GAME_MASTER_OUTPUT_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class OutputPanel : public wxTextCtrl
    {
        DECLARE_CLASS(OutputPanel)
    public:
        OutputPanel(wxWindow* parent);
        virtual ~OutputPanel();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

        void onWindowLogPrintEvent(const WindowLogPrintEvent& ev);
        void onWindowLogClearEvent(const WindowLogClearEvent& ev);
        void onServerSelectEvent(const ServerSelectEvent& ev);
        void onLoginSuccessEvent(const LoginSuccessEvent& ev);
        void onLoginBrokenEvent(const LoginBrokenEvent& ev);

    private:
        DECLARE_EVENT_TABLE()
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_OUTPUT_PANEL_H__


