#include "App.h"



namespace Hydro::app
{
	App::App()
		:
		window( 1280, 720 )
	{
	}

	int App::Go()
	{
		std::optional<int> quiting = {};
		while( !quiting )
		{
			quiting = window.ProcessMessages();
			DoFrame();
		}

		return *quiting;
	}

	//Application loop
	void App::DoFrame()
	{
		const float c = sin( timer.Peek() ) / 2.0f + 0.5f;
		window.Gfx().ClearBuffer( c, 1 - c, 1.0f );
		window.Gfx().EndFrame();
	}
}
