#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/utility/Timer.h>
#include <Core/src/gfx/Drawable/Drawable.h>
#include <Core/src/gfx/Mesh.h>
#include <Core/src/gfx/Drawable/TestPlane.h>
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
	Hydro::gfx::Camera cam;
	Hydro::utility::Timer timer;
	Hydro::gfx::PointLight light;
	float speed_factor = 1.0f;
	//Models
	Hydro::gfx::Model sponza{ window.Gfx(), "models\\Sponza\\sponza.obj", 1.0f / 20.0f };
	//Hydro::gfx::Model gobber{ window.Gfx(), "models\\gobber\\GoblinX.obj", 6.0f };
	//Hydro::gfx::Model wall{ window.Gfx(), "models\\brick_wall\\brick_wall.obj", 6.0f };
	//Hydro::gfx::TestPlane tp{ window.Gfx(),6.0f };
	//Hydro::gfx::Model nano{ window.Gfx(), "models\\nano_textured\\nanosuit.obj", 2.0f };
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
