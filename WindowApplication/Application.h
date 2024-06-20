#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/misc/Timer.h>
#include <Core/src/gfx/Camera.h>
#include <Core/src/gfx/PointLight.h>
#include <Core/src/gfx/Drawable/TestCube.h>
#include <Core/src/gfx/Model/Model.h>
#include <Core/src/gfx/Jobber/Graphlib/BlurOutlineRenderGraph.h>

class WindowApplication : public Hydro::app::App
{
public:
	WindowApplication();
	~WindowApplication();
private:
	void DoFrame() override;
	void HandleInput( float dt );
private:
	float speed_factor = 1.0f;
	Hydro::misc::Timer timer;
	Hydro::gfx::Rgph::BlurOutlineRenderGraph rg{ window.Gfx() };
	Hydro::gfx::Camera cam;
	Hydro::gfx::PointLight light;
	//Models
	Hydro::gfx::Model sponza{ window.Gfx(), "models\\Sponza\\sponza.obj", 1.0f / 20.0f };
	Hydro::gfx::TestCube cube{ window.Gfx(),4.0f };
	Hydro::gfx::TestCube cube2{ window.Gfx(),4.0f };
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
