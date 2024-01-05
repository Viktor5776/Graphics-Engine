#pragma once
#include <Core/app/App.h>
#include <Core/app/EntryPoint.h>
#include <Core/utility/Timer.h>


class WindowApplication : public Hydro::app::App
{
public:
	WindowApplication()
		:
		App( 1280, 720 )
	{}
private:
	void DoFrame() override;
private:
	Hydro::utility::Timer timer;
};

Hydro::app::App* Hydro::app::CreateApplication()
{
	return new WindowApplication();
}
