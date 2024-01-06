#pragma once
#include <Core/src/app/App.h>
#include <Core/src/app/EntryPoint.h>
#include <Core/src/utility/Timer.h>


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
