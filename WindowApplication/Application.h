#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/utility/Timer.h>
#include <Core/src/gfx/Drawable/Drawable.h>

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
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
