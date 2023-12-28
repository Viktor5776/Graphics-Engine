#include "WindowApplication.h"

WindowApplication::WindowApplication() {}

WindowApplication::~WindowApplication() {}

void WindowApplication::DoFrame()
{
	const float c = sin( timer.Peek() ) / 2.0f + 0.5f;
	window.Gfx().ClearBuffer( c, 1 - c, 1.0f );
	window.Gfx().EndFrame();
}