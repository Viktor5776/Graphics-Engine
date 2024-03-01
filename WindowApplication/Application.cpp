#include "Application.h"
#include <Core/src/utility/GDIPlusManager.h>
#include <Core/third/ImGui/imgui.h>
#include <Core/third/ImGui/imgui_impl_dx11.h>
#include <Core/third/ImGui/imgui_impl_win32.h>
#include <Core/src/gfx/Drawable/AssTest.h>

using namespace Hydro;
utility::GDIPlusManager gdipm;

WindowApplication::WindowApplication()
	:
	App( 1280, 720 ),
	light( window.Gfx(), 0.5f )
{
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

	const auto transform = DirectX::XMMatrixRotationRollPitchYaw( pos.roll, pos.pitch, pos.yaw ) *
		DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
	nano.Draw( window.Gfx(), transform  );
	
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
	ShowModelWindow();
	
	window.Gfx().EndFrame();

}

void WindowApplication::ShowModelWindow()
{
	if( ImGui::Begin( "Model" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &pos.x, -20.0f, 20.0f, "%.1f" );
		ImGui::SliderFloat( "Y", &pos.y, -20.0f, 20.0f, "%.1f" );
		ImGui::SliderFloat( "Z", &pos.z, -20.0f, 20.0f, "%.1f" );
		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll", &pos.roll, -180.0f, 180.0f );
		ImGui::SliderAngle( "Pitch", &pos.pitch, -180.0f, 180.0f );
		ImGui::SliderAngle( "Yaw", &pos.yaw, -180.0f, 180.0f );
	}
	ImGui::End();
}
