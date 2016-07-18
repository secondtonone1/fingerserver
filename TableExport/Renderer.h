#ifndef __LYNX_EXCEL_TO_DBC_TOOL_RENDER_H__
#define __LYNX_EXCEL_TO_DBC_TOOL_RENDER_H__

#include "PlatformLib.h"
#include "NetworkLib.h"
#include "MsgDefine.h"

namespace Lynx
{
    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();

        bool Initial(void* windowHandle);
        void Release();
        bool Update(float accTime);

        void OnSizeChanged();
		void RegisterMsgHandler();

    public:
        void* m_RenderWindowHandle;
    };
} // namespace Lynx

#endif // __LYNX_EXCEL_TO_DBC_TOOL_RENDER_H__