#pragma once
#include "TechniqueProbe.h"

namespace Hydro::gfx
{
	namespace Rgph
	{
		class RenderGraph;
	}

	class StepLinkingProbe : public TechniqueProbe
	{
	protected:
		void OnSetStep() override;
	private:
		Rgph::RenderGraph& rg;
	};
}