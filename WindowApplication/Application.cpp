#include "Application.h"

WindowApplication::WindowApplication() {}

WindowApplication::~WindowApplication() {}

void WindowApplication::DoFrame()
{
	const float c = sin( timer.Peek() ) / 2.0f + 0.5f;
	window.Gfx().ClearBuffer( 1-c, 1.0f, c );
	window.Gfx().EndFrame();
}
