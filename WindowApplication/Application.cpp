#include "Application.h"
#include <algorithm>
#include <Core/src/misc/HydroMath.h>
#include <Core/third/ImGui/imgui.h>
#include <Core/src/misc/HydroUtility.h>
#include <Core/src/gfx/TestModelProbe.h>

using namespace Hydro;
using namespace Hydro::gfx;

WindowApplication::WindowApplication()
	:
	App( 1280, 720 ),
	light( window.Gfx(), 0.5f )
{
	cube.SetPos( { 4.0f,0.0f,0.0f } );
	cube2.SetPos( { 0.0f,4.0f,0.0f } );

	nano.SetRootTransform(
		dx::XMMatrixRotationY( PI / 2.f ) *
		dx::XMMatrixTranslation( 27.f, -0.56f, 1.7f )
	);
	gobber.SetRootTransform(
		dx::XMMatrixRotationY( -PI / 2.f ) *
		dx::XMMatrixTranslation( -8.f, 10.f, 0.f )
	);

	cube.LinkTechniques( rg );
	cube2.LinkTechniques( rg );
	light.LinkTechniques( rg );
	sponza.LinkTechniques( rg );
	gobber.LinkTechniques( rg );
	nano.LinkTechniques( rg );


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
	
	light.Submit();
	cube.Submit();
	sponza.Submit();
	cube2.Submit();
	gobber.Submit();
	nano.Submit();

	rg.Execute( window.Gfx() );

	HandleInput(dt);

	//Imgui windows
	static MP sponzeProbe{ "Sponza" };
	static MP gobberProbe{ "Gobber" };
	static MP nanoProbe{ "Nano" };
	sponzeProbe.SpawnWindow( sponza );
	gobberProbe.SpawnWindow( gobber );
	nanoProbe.SpawnWindow( nano );
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	cube.SpawnControlWindow( window.Gfx(), "Cube 1" );
	cube2.SpawnControlWindow( window.Gfx(), "Cube 2" );
	rg.RenderWidgets( window.Gfx() );

	window.Gfx().EndFrame();
	rg.Reset();
}

void WindowApplication::HandleInput( float dt )
{
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
}



