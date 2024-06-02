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
	//wall.SetRootTransform( DirectX::XMMatrixTranslation( -12.0f, 0.0f, 0.0f ) );
	//tp.SetPos( { 12.0f,0.0f,0.0f } );
	//gobber.SetRootTransform( DirectX::XMMatrixTranslation( 0.0f, 0.0f, -4.0f ) );
	//nano.SetRootTransform( DirectX::XMMatrixTranslation( 0.0f, -7.0f, 6.0f ) );
	bluePlane.SetPos( cam.GetPos() );
	redPlane.SetPos( cam.GetPos() );

	window.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 400.0f ) );
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	float dt = timer.Mark() * speed_factor;
	window.Gfx().BeginFrame( 0.07f, 0.0f, 0.12f );
	window.Gfx().SetCamera( cam.GetMatrix() );
	light.Bind( window.Gfx(), cam.GetMatrix() );
	
	//wall.Draw( window.Gfx() );
	//tp.Draw( window.Gfx() );
	//nano.Draw( window.Gfx() );
	//gobber.Draw( window.Gfx() );
	sponza.Draw( window.Gfx() );
	light.Draw( window.Gfx() );
	bluePlane.Draw( window.Gfx() );
	redPlane.Draw( window.Gfx() );

	while( const auto e = window.kbd.ReadKey() )
	{
		if( !e->IsPress() )
		{
			continue;
		}

		switch( e->GetCode() )
		{
		case VK_ESCAPE:
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
			break;
		}
	}

	//Camera Movement
	if( !window.CursorEnabled() )
	{
		if( window.kbd.KeyIsPressed( 'W' ) )
		{
			cam.Translate( { 0.0f,0.0f,dt } );
		}
		if( window.kbd.KeyIsPressed( 'A' ) )
		{
			cam.Translate( { -dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'S' ) )
		{
			cam.Translate( { 0.0f,0.0f,-dt } );
		}
		if( window.kbd.KeyIsPressed( 'D' ) )
		{
			cam.Translate( { dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'R' ) )
		{
			cam.Translate( { 0.0f,dt,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'F' ) )
		{
			cam.Translate( { 0.0f,-dt,0.0f } );
		}
	}

	while( const auto delta = window.mouse.ReadRawDelta() )
	{
		if( !window.CursorEnabled() )
		{
			cam.Rotate( (float)delta->x, (float)delta->y );
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
	//wall.ShowWindow( window.Gfx(), "Wall" );
	//gobber.ShowWindow( window.Gfx(), "gobber" ); 
	//tp.SpawnControlWindow( window.Gfx() );
	//nano.ShowWindow( window.Gfx(), "Nano" );
	sponza.ShowWindow( window.Gfx(), "Sponza" );
	bluePlane.SpawnControlWindow( window.Gfx(), "Blue Plane" );
	redPlane.SpawnControlWindow( window.Gfx(), "Red Plane" );

	window.Gfx().EndFrame();

}



