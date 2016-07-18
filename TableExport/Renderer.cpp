#include "Renderer.h"
#include "TableExport.h"

using namespace Lynx;

Renderer::Renderer() : m_RenderWindowHandle(NULL)
{

}

Renderer::~Renderer()
{

}

bool 
Renderer::Initial(void* windowHandle)
{
    return true;
}

void 
Renderer::Release()
{

}

bool 
Renderer::Update(float accTime)
{   
    return true;
}

void
Renderer::OnSizeChanged()
{

}

void 
Renderer::RegisterMsgHandler()
{

}