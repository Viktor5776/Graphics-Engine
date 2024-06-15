#pragma once
#include "../win/Window.h"
#include "../misc/Timer.h"
#include "../misc/ImGuiManager.h"

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
		misc::ImGuiManager imgui;
		win::Window window;
	};

	App* CreateApplication();
}