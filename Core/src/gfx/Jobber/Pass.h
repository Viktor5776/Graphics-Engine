#pragma once
#include "../Graphics.h"
#include "Job.h"
#include <vector>

namespace Hydro::gfx
{

	class Pass
	{
	public:
		void Accept( Job job ) noexcept
		{
			jobs.push_back( job );
		}
		void Execute( Graphics& gfx ) const noexcept(!_DEBUG)
		{
			for( const auto& j : jobs )
			{
				j.Execute( gfx );
			}
		}
		void Reset() noexcept
		{
			jobs.clear();
		}
	private:
		std::vector<Job> jobs;
	};

}