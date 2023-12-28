#pragma once
#include "../win/Window.h"
#include "../utility/Timer.h"

namespace Hydro::app
{
	class App
	{
	public:
		App();
		virtual ~App() = default;
		int Go();
	private:
		virtual void DoFrame() = 0;
	protected:
		win::Window window;
	};

	App* CreateApplication();
}