#include "Job.h"
#include "Step.h"
#include "../Drawable/Drawable.h"

namespace Hydro::gfx
{

	Job::Job( const Step* pStep, const Drawable* pDrawable )
		:
		pDrawable{ pDrawable },
		pStep{ pStep }
	{}

	void Job::Execute( Graphics& gfx ) const noexcept(!_DEBUG)
	{
		pDrawable->Bind( gfx );
		pStep->Bind( gfx );
		gfx.DrawIndexed( pDrawable->GetIndexCount() );
	}

}