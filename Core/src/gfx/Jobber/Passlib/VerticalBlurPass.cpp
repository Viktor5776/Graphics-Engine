#include "VerticalBlurPass.h"
#include "Base/Sink.h"
#include "Base/Source.h"
#include "../../Bindable/PixelShader.h"
#include "../../Bindable/Blender.h"
#include "../../Bindable/Stencil.h"
#include "../../Bindable/Sampler.h"

namespace Hydro::gfx
{
	using namespace Bind;

	namespace Rgph
	{
		VerticalBlurPass::VerticalBlurPass( std::string name, Graphics& gfx )
			:
			FullscreenPass( std::move( name ), gfx )
		{
			AddBind( PixelShader::Resolve( gfx, "BlurOutline_PS.cso" ) );
			AddBind( Blender::Resolve( gfx, true ) );
			AddBind( Stencil::Resolve( gfx, Stencil::Mode::Mask ) );
			AddBind( Sampler::Resolve( gfx, Sampler::Type::Bilinear, true ) );

			AddBindSink<RenderTarget>( "scratchIn" );
			AddBindSink<CachingPixelConstantBufferEx>( "kernel" );
			RegisterSink( DirectBindableSink<CachingPixelConstantBufferEx>::Make( "direction", direction ) );
			RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget", renderTarget ) );
			RegisterSink( DirectBufferSink<DepthStencil>::Make( "depthStencil", depthStencil ) );

			RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget", renderTarget ) );
			RegisterSource( DirectBufferSource<DepthStencil>::Make( "depthStencil", depthStencil ) );
		}

		// see the note on HorizontalBlurPass::Execute
		void VerticalBlurPass::Execute( Graphics& gfx ) const noexcept(!_DEBUG)
		{
			auto buf = direction->GetBuffer();
			buf["isHorizontal"] = false;
			direction->SetBuffer( buf );

			direction->Bind( gfx );
			FullscreenPass::Execute( gfx );
		}
	}
}
