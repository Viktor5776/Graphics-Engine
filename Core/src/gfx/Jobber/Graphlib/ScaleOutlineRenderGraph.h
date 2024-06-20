#pragma once
#include "../RenderGraph.h"

namespace Hydro::gfx
{
	class Graphics;

	namespace Rgph
	{
		class ScaleOutlineRenderGraph : public RenderGraph
		{
		public:
			ScaleOutlineRenderGraph( Graphics& gfx );
		};
	}
}