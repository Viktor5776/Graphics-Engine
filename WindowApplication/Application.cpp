#include "Application.h"
#include <Core/src/utility/GDIPlusManager.h>
#include <Core/third/ImGui/imgui.h>
#include <Core/third/ImGui/imgui_impl_dx11.h>
#include <Core/third/ImGui/imgui_impl_win32.h>

using namespace Hydro;
utility::GDIPlusManager gdipm;

WindowApplication::WindowApplication()
	:
	App( 1280, 720 ),
	light( window.Gfx(), 0.5f )
{
	window.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 80.0f ) );
	window.DisableCursor();
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	float dt = timer.Mark() * speed_factor;
	window.Gfx().BeginFrame( 0.07f, 0.0f, 0.12f );
	window.Gfx().SetCamera( cam.GetMatrix() );
	light.Bind( window.Gfx(), cam.GetMatrix() );

	nano.Draw( window.Gfx() );
	light.Draw( window.Gfx() );
	
	while( const auto e = window.kbd.ReadKey() )
	{
		if( e->IsPress() && e->GetCode() == VK_INSERT )
		{
			if( window.CursorEnabled() )
			{
				window.DisableCursor();
				window.mouse.EnableRaw();
			}
			else
			{
				window.EnableCursor();
				window.mouse.DisableRaw();
			}
		}
	}

	//Imgui window to control simulation speed
	if( ImGui::Begin( "Simulation speed " ) )
	{
		ImGui::SliderFloat( "Speed", &speed_factor, 0.0f, 6.0f, "%.4f" );
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "Status: %s ( hold space to pause )", window.kbd.KeyIsPressed( VK_SPACE ) ?  "Paused" : "Running" );
	}
	ImGui::End();
	
	//Imgui window to control camera and light
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	nano.ShowWindow( "nano" );
	ShowRawInputWindow();

	window.Gfx().EndFrame();

}

void WindowApplication::ShowRawInputWindow()
{
	while( const auto d = window.mouse.ReadRawDelta() )
	{
		x += d->x;
		y += d->y;
	}
	if( ImGui::Begin( "Raw Input" ) )
	{
		ImGui::Text( "Tally: (%d,%d)", x, y );
		ImGui::Text( "Cursor: %s", window.CursorEnabled() ? "enabled" : "disabled" );
	}
	ImGui::End();
}


