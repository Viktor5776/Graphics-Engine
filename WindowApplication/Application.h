#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/misc/Timer.h>
#include <Core/src/gfx/CameraContainer.h>
#include <Core/src/gfx/PointLight.h>
#include <Core/src/gfx/Drawable/TestCube.h>
#include <Core/src/gfx/Model/Model.h>
#include <Core/src/gfx/Jobber/Graphlib/BlurOutlineRenderGraph.h>
#include <Core/src/misc/HydroMath.h>
#include <Core/src/gfx/Scene.h>

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
	Hydro::gfx::Scene scene;
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
