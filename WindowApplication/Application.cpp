#include "Application.h"
#include <Core/src/utility/Surface.h>
#include <Core/src/utility/GDIPlusManager.h>
#include <Core/src/gfx/Drawable/Box.h>
#include <Core/src/gfx/Drawable/Sheet.h>

using namespace Hydro;
utility::GDIPlusManager gdipm;

WindowApplication::WindowApplication()
	:
	App( 1280, 720 )
{
	std::mt19937 rng( std::random_device{}() );
	std::uniform_real_distribution<float> adist( 0.0f, 3.1415f * 2.0f );
	std::uniform_real_distribution<float> ddist( 0.0f, 3.1415f * 2.0f );
	std::uniform_real_distribution<float> odist( 0.0f, 3.1415f * 0.3f );
	std::uniform_real_distribution<float> rdist( 6.0f, 20.0f );
	for( auto i = 0; i < 80; i++ )
	{
		boxes.push_back( std::make_unique<gfx::Box>(
			window.Gfx(), rng, adist,
			ddist, odist, rdist
		) );
	}

	boxes.push_back( std::make_unique<gfx::Sheet>( window.Gfx(), rng, adist, ddist, odist, rdist ) );

	window.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 80.0f ) );
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	window.Gfx().ClearBuffer( 0.2f, 0.1f, 0.4f );
	float dt = timer.Mark();

	for( auto& b : boxes )
	{
		b->Update( window.kbd.KeyIsPressed( VK_SPACE ) ? 0.0f : dt );
		b->Draw( window.Gfx() );
	}


	window.Gfx().EndFrame();
}
