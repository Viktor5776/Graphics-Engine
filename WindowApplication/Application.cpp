#include "Application.h"
#include <Core/src/utility/Surface.h>
#include <Core/src/utility/GDIPlusManager.h>
#include <Core/src/gfx/Drawable/Box.h>
#include <Core/src/gfx/Drawable/Sheet.h>
#include <Core/src/gfx/Drawable/SkinnedBox.h>
#include <Core/third/ImGui/imgui.h>
#include <Core/third/ImGui/imgui_impl_dx11.h>
#include <Core/third/ImGui/imgui_impl_win32.h>


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
	for( auto i = 0; i < 30; i++ )
	{
		drawables.push_back( std::make_unique<gfx::Box>(
			window.Gfx(), rng, adist,
			ddist, odist, rdist
		) );
	}
	for( auto i = 0; i < 30; i++ )
	{
		drawables.push_back( std::make_unique<gfx::Sheet>( window.Gfx(), rng, adist, ddist, odist, rdist ) );
	}
	for( auto i = 0; i < 30; i++ )
	{
		drawables.push_back( std::make_unique<gfx::SkinnedBox>( window.Gfx(), rng, adist, ddist, odist, rdist ) );
	}

	window.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 80.0f ) );
	window.Gfx().SetCamera( cam.GetMatrix() );
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	float dt = timer.Mark() * speed_factor;
	window.Gfx().BeginFrame( 0.2f, 0.1f, 0.4f );

	for( auto& b : drawables )
	{
		b->Update( window.kbd.KeyIsPressed( VK_SPACE ) ? 0.0f : dt );
		b->Draw( window.Gfx() );
	}

	if( ImGui::Begin( "Simulation speed " ) )
	{
		ImGui::SliderFloat( "Speed", &speed_factor, 0.0f, 10.0f, "%.2f" );
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "Status: %s ( hold space to pause )", window.kbd.KeyIsPressed( VK_SPACE ) ?  "Paused" : "Running" );
	}
	ImGui::End();

	cam.SpawnControlWindow();

	window.Gfx().SetCamera( cam.GetMatrix() );
	
	window.Gfx().EndFrame();
}
