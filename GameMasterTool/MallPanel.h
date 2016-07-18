#ifndef __LYNX_GAME_MASTER_TOOL_MALL_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_MALL_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXlib.h"

namespace Lynx
{
    class MallPanel : public wxListCtrl
    {
        DECLARE_CLASS(MallPanel)
    public:
        MallPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxLC_REPORT | wxLC_SINGLE_SEL);
        virtual ~MallPanel();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        DECLARE_EVENT_TABLE()
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_MALL_PANEL_H__


