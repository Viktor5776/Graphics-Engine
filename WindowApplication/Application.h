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
	void ShowModelWindow();
private:
	Hydro::gfx::Camera cam;
	Hydro::utility::Timer timer;
	Hydro::gfx::PointLight light;
	float speed_factor = 1.0f;
	Hydro::gfx::Model nano{ window.Gfx(), "models/nanosuit.obj" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
