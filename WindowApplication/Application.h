#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/utility/Timer.h>
#include <Core/src/gfx/Drawable/Drawable.h>
#include <Core/src/gfx/PointLight.h>
#include <Core/src/gfx/Camera.h>

class WindowApplication : public Hydro::app::App
{
public:
	WindowApplication();
	~WindowApplication();
private:
	void DoFrame() override;
private:
	Hydro::utility::Timer timer;
	std::vector<std::unique_ptr<Hydro::gfx::Drawable>> drawables;
	Hydro::gfx::PointLight light;
	float speed_factor = 1.0f;
	float x = 0;
	Hydro::gfx::Camera cam;
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
