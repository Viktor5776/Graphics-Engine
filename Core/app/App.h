#pragma once
#include "../win/Window.h"
#include "../utility/Timer.h"

namespace Hydro::app
{
	class App
	{
	public:
		App( int width = 1280, int height = 720 );
		virtual ~App() = default;
		int Go();
	private:
		virtual void DoFrame() = 0;
	protected:
		win::Window window;
	};

	App* CreateApplication();
}