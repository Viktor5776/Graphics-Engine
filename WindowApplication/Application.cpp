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
	light( window.Gfx(), { 10.0f,5.0f,0.0f } )
{
	cameras.AddCamera( std::make_unique<Camera>( window.Gfx(), "A", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f ) );
	cameras.AddCamera( std::make_unique<Camera>( window.Gfx(), "B", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f ) );
	cameras.AddCamera( light.ShareCamera() );

	cube.SetPos( { 10.0f,5.0f,6.0f } );
	cube2.SetPos( { 10.0f,5.0f,14.0f } );

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
	cameras.LinkTechniques( rg );

	rg.BindShadowCamera( *light.ShareCamera() );
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	float dt = timer.Mark() * speed_factor;
	window.Gfx().BeginFrame( 0.07f, 0.0f, 0.12f );

	light.Bind( window.Gfx(), cameras->GetMatrix() );
	rg.BindMainCamera( cameras.GetActiveCamera() );

	light.Submit( Channels::main );
	cube.Submit( Channels::main );
	sponza.Submit( Channels::main );
	cube2.Submit( Channels::main );
	gobber.Submit( Channels::main );
	nano.Submit( Channels::main );
	cameras.Submit( Channels::main );

	sponza.Submit( Channels::shadow );
	cube.Submit( Channels::shadow );
	cube2.Submit( Channels::shadow );
	gobber.Submit( Channels::shadow );
	nano.Submit( Channels::shadow );

	rg.Execute( window.Gfx() );

	if( savingDepth )
	{
		rg.DumpShadowMap( window.Gfx(), "shadow.png" );
		savingDepth = false;
	}

	HandleInput(dt);

	//Imgui windows
	static MP sponzeProbe{ "Sponza" };
	static MP gobberProbe{ "Gobber" };
	static MP nanoProbe{ "Nano" };
	sponzeProbe.SpawnWindow( sponza );
	gobberProbe.SpawnWindow( gobber );
	nanoProbe.SpawnWindow( nano );
	cameras.SpawnWindow( window.Gfx() );
	light.SpawnControlWindow();
	cube.SpawnControlWindow( window.Gfx(), "Cube 1" );
	cube2.SpawnControlWindow( window.Gfx(), "Cube 2" );
	rg.RenderWindows( window.Gfx() );

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
		case VK_RETURN:
			savingDepth = true;
			break;
		}
	}

	//Camera Movement
	if( !window.CursorEnabled() )
	{
		if( window.kbd.KeyIsPressed( 'W' ) )
		{
			cameras->Translate( { 0.0f,0.0f,dt } );
		}
		if( window.kbd.KeyIsPressed( 'A' ) )
		{
			cameras->Translate( { -dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'S' ) )
		{
			cameras->Translate( { 0.0f,0.0f,-dt } );
		}
		if( window.kbd.KeyIsPressed( 'D' ) )
		{
			cameras->Translate( { dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'R' ) )
		{
			cameras->Translate( { 0.0f,dt,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'F' ) )
		{
			cameras->Translate( { 0.0f,-dt,0.0f } );
		}
	}

	while( const auto delta = window.mouse.ReadRawDelta() )
	{
		if( !window.CursorEnabled() )
		{
			cameras->Rotate( (float)delta->x, (float)delta->y );
		}
	}
}



