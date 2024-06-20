#include "BufferClearPass.h"
#include "../../Bindable/RenderTarget.h"
#include "../../Bindable/DepthStencil.h"
#include "Base/Sink.h"
#include "Base/Source.h"


namespace Hydro::gfx::Rgph
{
	BufferClearPass::BufferClearPass( std::string name )
		:
		Pass( std::move( name ) )
	{
		RegisterSink( DirectBufferSink<Bind::BufferResource>::Make( "buffer", buffer ) );
		RegisterSource( DirectBufferSource<Bind::BufferResource>::Make( "buffer", buffer ) );
	}

	void BufferClearPass::Execute( Graphics& gfx ) const noexcept(!_DEBUG)
	{
		buffer->Clear( gfx );
	}
}