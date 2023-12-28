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
}
