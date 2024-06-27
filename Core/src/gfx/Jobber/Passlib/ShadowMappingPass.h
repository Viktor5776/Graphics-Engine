#pragma once
#include "Base/RenderQueuePass.h"
#include "../Job.h"
#include <vector>
#include "../../Bindable/PixelShader.h"
#include "../../Bindable/VertexShader.h"
#include "../../Bindable/Stencil.h"
#include "../../Bindable/Rasterizer.h"
#include "Base/Source.h"
#include "../../Bindable/RenderTarget.h"
#include "../../Bindable/Blender.h"
#include "../../Bindable/NullPixelShader.h"
#include "../../Camera.h"

namespace Hydro::gfx
{

	class Graphics;

	namespace Rgph
	{
		class ShadowMappingPass : public RenderQueuePass
		{
		public:
			void BindShadowCamera( const Camera& cam ) noexcept
			{
				pShadowCamera = &cam;
			}
			ShadowMappingPass( Graphics& gfx, std::string name )
				:
				RenderQueuePass( std::move( name ) )
			{
				using namespace Bind;
				depthStencil = std::make_unique<ShaderInputDepthStencil>( gfx, 3, DepthStencil::Usage::ShadowDepth );
				AddBind( VertexShader::Resolve( gfx, "Solid_VS.cso" ) );
				AddBind( NullPixelShader::Resolve( gfx ) );
				AddBind( Stencil::Resolve( gfx, Stencil::Mode::Off ) );
				AddBind( Blender::Resolve( gfx, false ) );
				RegisterSource( DirectBindableSource<Bind::DepthStencil>::Make( "map", depthStencil ) );
			}
			void Execute( Graphics& gfx ) const noexcept(!_DEBUG) override
			{
				depthStencil->Clear( gfx );
				pShadowCamera->BindToGraphics( gfx );
				RenderQueuePass::Execute( gfx );
			}
			void DumpShadowMap( Graphics& gfx, const std::string& path ) const
			{
				depthStencil->ToSurface( gfx ).Save( path );
			}
		private:
			const Camera* pShadowCamera = nullptr;
		};
	}
}