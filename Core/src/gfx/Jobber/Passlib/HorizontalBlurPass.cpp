#include "HorizontalBlurPass.h"
#include "../../Bindable/PixelShader.h"
#include "../../Bindable/RenderTarget.h"
#include "Base/Sink.h"
#include "Base/Source.h"
#include "../../Bindable/Blender.h"
#include "../../Bindable/Sampler.h"
#include "../../Bindable/ConstantBuffersEx.h"




namespace Hydro::gfx::Rgph
{
	using namespace Bind;

	HorizontalBlurPass::HorizontalBlurPass( std::string name, Graphics& gfx, unsigned int fullWidth, unsigned int fullHeight )
		:
		FullscreenPass( std::move( name ), gfx )
	{
		AddBind( PixelShader::Resolve( gfx, "BlurOutline_PS.cso" ) );
		AddBind( Blender::Resolve( gfx, false ) );
		AddBind( Sampler::Resolve( gfx, Sampler::Type::Point, true ) );

		AddBindSink<Bind::RenderTarget>( "scratchIn" );
		AddBindSink<Bind::CachingPixelConstantBufferEx>( "kernel" );
		RegisterSink( DirectBindableSink<CachingPixelConstantBufferEx>::Make( "direction", direction ) );

		// the renderTarget is internally sourced and then exported as a Bindable
		renderTarget = std::make_shared<Bind::ShaderInputRenderTarget>( gfx, fullWidth / 2, fullHeight / 2, 0u );
		RegisterSource( DirectBindableSource<RenderTarget>::Make( "scratchOut", renderTarget ) );
	}

	// this override is necessary because we cannot (yet) link input bindables directly into
	// the container of bindables (mainly because vector growth buggers references)
	void HorizontalBlurPass::Execute( Graphics& gfx ) const noexcept(!_DEBUG)
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = true;
		direction->SetBuffer( buf );

		direction->Bind( gfx );
		FullscreenPass::Execute( gfx );
	}
}
