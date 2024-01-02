#include "App.h"

namespace Hydro::app
{
	App::App( int width, int height)
		:
		window( width, height )
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
