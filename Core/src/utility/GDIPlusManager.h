#pragma once
#include "../win/HydroWin.h"

namespace Hydro::utility
{
	class GDIPlusManager
	{
	public:
		GDIPlusManager();
		~GDIPlusManager();
	private:
		static ULONG_PTR token;
		static int refCount;
	};
}