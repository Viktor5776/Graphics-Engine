#pragma once
#include "../win/Window.h"
#include "../utility/HydroTimer.h"

namespace Hydro::app
{
	class App
	{
	public:
		App();
		int Go();
	private:
		void DoFrame();
	private:
		win::Window window;
		utility::Timer timer;
	};
}