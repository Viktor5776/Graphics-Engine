#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/utility/Timer.h>
#include <Core/src/gfx/Drawable/Drawable.h>
#include <Core/src/gfx/Mesh.h>
#include <Core/src/gfx/PointLight.h>
#include <Core/src/gfx/Camera.h>

class WindowApplication : public Hydro::app::App
{
public:
	WindowApplication();
	~WindowApplication();
private:
	void DoFrame() override;
	void ShowRawInputWindow();
private:
	int x = 0, y = 0;
	Hydro::gfx::Camera cam;
	Hydro::utility::Timer timer;
	Hydro::gfx::PointLight light;
	float speed_factor = 1.0f;
	Hydro::gfx::Model nano{ window.Gfx(), "models/nano.gltf" };
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
