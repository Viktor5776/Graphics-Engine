#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/misc/Timer.h>
#include <Core/src/gfx/Drawable/Drawable.h>
#include <Core/src/gfx/Model/Model.h>
#include <Core/src/gfx/Drawable/TestPlane.h>
#include <Core/src/gfx/PointLight.h>
#include <Core/src/gfx/Camera.h>
#include <Core/src/gfx/Drawable/TestCube.h>
#include <Core/src/gfx/Jobber/FrameCommander.h>
#include <Core/src/gfx/Model/Material.h>

class WindowApplication : public Hydro::app::App
{
public:
	WindowApplication();
	~WindowApplication();
private:
	void DoFrame() override;
private:
	float speed_factor = 1.0f;
	Hydro::misc::Timer timer;
	Hydro::gfx::FrameCommander fc;
	Hydro::gfx::Camera cam;
	Hydro::gfx::PointLight light;
	//Models
	Hydro::gfx::Model sponza{ window.Gfx(), "models\\Sponza\\sponza.obj", 1.0f / 20.0f };
	Hydro::gfx::TestCube cube{ window.Gfx(),4.0f };
	Hydro::gfx::TestCube cube2{ window.Gfx(),4.0f };

	//Hydro::gfx::Model gobber{ window.Gfx(), "models\\gobber\\GoblinX.obj", 4.0f };
	//Hydro::gfx::TestPlane bluePlane{ window.Gfx(),6.0f,{ 0.3f,0.3f,1.0f,0.0f } };
	//Hydro::gfx::TestPlane redPlane{ window.Gfx(),6.0f,{ 1.0f,0.3f,0.3f,0.0f } };
	//Hydro::gfx::Model wall{ window.Gfx(), "models\\brick_wall\\brick_wall.obj", 6.0f };
	//Hydro::gfx::TestPlane tp{ window.Gfx(),6.0f };
	//Hydro::gfx::Model nano{ window.Gfx(), "models\\nano_textured\\nanosuit.obj", 2.0f };
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
