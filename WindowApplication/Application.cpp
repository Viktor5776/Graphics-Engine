#include "Application.h"
#include <algorithm>
#include <Core/src/misc/HydroMath.h>
#include <Core/third/ImGui/imgui.h>
#include <Core/src/misc/HydroUtility.h>
#include <Core/src/gfx/TestModelProbe.h>
#include <Core/src/gfx/Camera.h>
#include <Core/src/gfx/Jobber/Channels.h>

using namespace Hydro;
using namespace Hydro::gfx;

WindowApplication::WindowApplication()
	:
	App( 1280, 720 ),
	scene( window.Gfx(), reinterpret_cast<Rgph::RenderGraph&>(rg), "Scenes\\scene.json" )
{
	//nano.SetRootTransform(
	//	dx::XMMatrixRotationY( PI / 2.f ) *
	//	dx::XMMatrixTranslation( 27.f, -0.56f, 1.7f )
	//);
	
	rg.BindShadowCamera( *scene.GetLight().ShareCamera());
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	float dt = timer.Mark() * speed_factor;
	window.Gfx().BeginFrame( 0.07f, 0.0f, 0.12f );

	scene.RenderSceneWindow( window.Gfx(), reinterpret_cast<Rgph::RenderGraph&>(rg) );
	rg.BindMainCamera( scene.GetActiveCamera() );
	scene.Submit( window.Gfx() );
	rg.Execute( window.Gfx() );
	
	HandleInput(dt);

	//Imgui windows
	//static MP sponzeProbe{ sponza.GetName() };
	//static MP gobberProbe{ gobber.GetName() };
	//static MP nanoProbe{ nano.GetName() };
	//sponzeProbe.SpawnWindow( sponza );
	//gobberProbe.SpawnWindow( gobber );
	//nanoProbe.SpawnWindow( nano );
	//cameras.SpawnWindow( window.Gfx() );
	//light.SpawnControlWindow();
	//cube.SpawnControlWindow( window.Gfx(), "Cube 1" );
	//cube2.SpawnControlWindow( window.Gfx(), "Cube 2" );
	//rg.RenderWindows( window.Gfx() );

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
			scene.GetActiveCamera().Translate({0.0f,0.0f,dt});
		}
		if( window.kbd.KeyIsPressed( 'A' ) )
		{
			scene.GetActiveCamera().Translate( { -dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'S' ) )
		{
			scene.GetActiveCamera().Translate( { 0.0f,0.0f,-dt } );
		}
		if( window.kbd.KeyIsPressed( 'D' ) )
		{
			scene.GetActiveCamera().Translate( { dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'R' ) )
		{
			scene.GetActiveCamera().Translate( { 0.0f,dt,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'F' ) )
		{
			scene.GetActiveCamera().Translate( { 0.0f,-dt,0.0f } );
		}
	}

	while( const auto delta = window.mouse.ReadRawDelta() )
	{
		if( !window.CursorEnabled() )
		{
			scene.GetActiveCamera().Rotate( (float)delta->x, (float)delta->y );
		}
	}
}



