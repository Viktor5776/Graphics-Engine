#include "Application.h"

void WindowApplication::DoFrame()
{
	const float c = sin( timer.Peek() ) / 2.0f + 0.5f;
	window.Gfx().ClearBuffer( c,c,1.0f );
	window.Gfx().DrawTestTriangle( -timer.Peek(), 0.0f, 0.0f );
	window.Gfx().DrawTestTriangle( timer.Peek(), 2*(window.mouse.GetPosX() / 1280.0f) -1 , -2 * (window.mouse.GetPosY() / 720.0f)+1 );
	window.Gfx().EndFrame();
}
