#pragma once
#include "Base/RenderQueuePass.h"
#include "../Job.h"
#include <vector>
#include "../../Bindable/NullPixelShader.h"
#include "../../Bindable/VertexShader.h"
#include "../../Bindable/Stencil.h"
#include "../../Bindable/Rasterizer.h"

namespace Hydro::gfx
{
	class Graphics;

	namespace Rgph
	{
		class OutlineMaskGenerationPass : public RenderQueuePass
		{
		public:
			OutlineMaskGenerationPass( Graphics& gfx, std::string name )
				:
				RenderQueuePass( std::move( name ) )
			{
				using namespace Bind;
				RegisterSink( DirectBufferSink<Bind::DepthStencil>::Make( "depthStencil", depthStencil ) );
				RegisterSource( DirectBufferSource<Bind::DepthStencil>::Make( "depthStencil", depthStencil ) );
				AddBind( VertexShader::Resolve( gfx, "Solid_VS.cso" ) );
				AddBind( NullPixelShader::Resolve( gfx ) );
				AddBind( Stencil::Resolve( gfx, Stencil::Mode::Write ) );
				AddBind( Rasterizer::Resolve( gfx, false ) );
			}
		};
	}
}