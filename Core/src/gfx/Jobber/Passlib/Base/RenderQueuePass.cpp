#include "RenderQueuePass.h"

namespace Hydro::gfx::Rgph
{
	void RenderQueuePass::Accept( Job job ) noexcept
	{
		jobs.push_back( job );
	}

	void RenderQueuePass::Execute( Graphics& gfx ) const noexcept(!_DEBUG)
	{
		BindAll( gfx );

		for( const auto& j : jobs )
		{
			j.Execute( gfx );
		}
	}

	void RenderQueuePass::Reset() noexcept(!_DEBUG)
	{
		jobs.clear();
	}
}
