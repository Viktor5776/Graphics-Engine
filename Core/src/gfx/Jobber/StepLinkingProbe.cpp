#include "StepLinkingProbe.h"
#include "RenderGraph.h"
#include "Step.h"

namespace Hydro::gfx
{
	void StepLinkingProbe::OnSetStep()
	{
		pStep->Link( rg );
	}
}