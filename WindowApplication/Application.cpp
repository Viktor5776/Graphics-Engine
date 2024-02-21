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
	App( 1280, 720 ),
	light( window.Gfx(), 0.5f )
{
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 0.0f,3.1415 * 2.0f };
	std::uniform_real_distribution<float> ddist{ 0.0f,3.1415 * 0.5f };
	std::uniform_real_distribution<float> odist{ 0.0f,3.1415 * 0.08f };
	std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
	std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
	std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
	
	for( auto i = 0; i < 160; i++ )
	{
		drawables.push_back( std::make_unique<gfx::Box>(
			window.Gfx(), rng, adist,
			ddist, odist, rdist, bdist, cdist
		) );
	}

	window.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 80.0f ) );
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	float dt = timer.Mark() * speed_factor;
	window.Gfx().BeginFrame( 0.07f, 0.0f, 0.12f );
	window.Gfx().SetCamera( cam.GetMatrix() );
	light.Bind( window.Gfx(), cam.GetMatrix() );

	for( auto& b : drawables )
	{
		b->Update( window.kbd.KeyIsPressed( VK_SPACE ) ? 0.0f : dt );
		b->Draw( window.Gfx() );
	}
	light.Draw( window.Gfx() );

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
	
	window.Gfx().EndFrame();
}
