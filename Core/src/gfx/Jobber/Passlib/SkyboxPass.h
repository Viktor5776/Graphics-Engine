#pragma once
#include "Base/RenderQueuePass.h"
#include "../Job.h"
#include <vector>
#include "Base/Sink.h"
#include "Base/Source.h"
#include "../../Bindable/Stencil.h"
#include "../../Camera.h"
#include "../../Bindable/Sampler.h"
#include "../../Bindable/Rasterizer.h"
#include "../../Bindable/DepthStencil.h"
#include "../../Drawable/Cube.h"
#include "../../Bindable/CubeTexture.h"
#include "../../Bindable/SkyboxTransformCbuf.h"
#include "../../Bindable/VertexBuffer.h"
#include "../../Bindable/IndexBuffer.h"
#include "../../Bindable/Topology.h"
#include "../../Bindable/InputLayout.h"
#include "../../Drawable/Sphere.h"

namespace Hydro::gfx
{
	class Graphics;

	namespace Rgph
	{
		class SkyboxPass : public BindingPass
		{
		public:
			SkyboxPass( Graphics& gfx, std::string name )
				:
				BindingPass( std::move( name ) )
			{
				using namespace Bind;
				RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget", renderTarget ) );
				RegisterSink( DirectBufferSink<DepthStencil>::Make( "depthStencil", depthStencil ) );
				AddBind( std::make_shared<CubeTexture>( gfx, "Images\\SpaceBox" ) );
				AddBind( Stencil::Resolve( gfx, Stencil::Mode::DepthFirst ) );
				AddBind( Sampler::Resolve( gfx, Sampler::Type::Bilinear ) );
				AddBind( Rasterizer::Resolve( gfx, true ) );
				AddBind( std::make_shared<SkyboxTransformCbuf>( gfx ) );
				AddBind( PixelShader::Resolve( gfx, "Skybox_PS.cso" ) );
				AddBind( Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
				{ // geometry
					auto pvs = Bind::VertexShader::Resolve( gfx, "Skybox_VS.cso" );
					{ // cube
						auto model = Cube::Make();
						const auto geometryTag = "$cube_map";
						pCubeVertices = VertexBuffer::Resolve( gfx, geometryTag, std::move( model.vertices ) );
						pCubeIndices = IndexBuffer::Resolve( gfx, geometryTag, std::move( model.indices ) );
						cubeCount = (UINT)model.indices.size();
						// layout is shared between cube and sphere; use cube data to generate
						AddBind( InputLayout::Resolve( gfx, model.vertices.GetLayout(), *pvs ) );
					}
					{ // sphere
						auto model = Sphere::Make();
						const auto geometryTag = "$sphere_map";
						pSphereVertices = VertexBuffer::Resolve( gfx, geometryTag, std::move( model.vertices ) );
						pSphereIndices = IndexBuffer::Resolve( gfx, geometryTag, std::move( model.indices ) );
						sphereCount = (UINT)model.indices.size();
					}
					AddBind( std::move( pvs ) );
				}
				RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget", renderTarget ) );
				RegisterSource( DirectBufferSource<DepthStencil>::Make( "depthStencil", depthStencil ) );
			}
			void BindMainCamera( const Camera& cam ) noexcept
			{
				pMainCamera = &cam;
			}
			void Execute( Graphics& gfx ) const noexcept(!_DEBUG) override
			{
				assert( pMainCamera );
				UINT indexCount;
				pMainCamera->BindToGraphics( gfx );
				if( useSphere )
				{
					pSphereVertices->Bind( gfx );
					pSphereIndices->Bind( gfx );
					indexCount = sphereCount;
				}
				else
				{
					pCubeVertices->Bind( gfx );
					pCubeIndices->Bind( gfx );
					indexCount = cubeCount;
				}
				BindAll( gfx );
				gfx.DrawIndexed( indexCount );
			}

			void RenderWindow()
			{
				if( ImGui::Begin( "Skybox" ) )
				{
					ImGui::Checkbox( "Use sphere", &useSphere );
				}
				ImGui::End();
			}
		private:
			bool useSphere = true;
			const Camera* pMainCamera = nullptr;
			std::shared_ptr<Bind::VertexBuffer> pCubeVertices;
			std::shared_ptr<Bind::IndexBuffer> pCubeIndices;
			UINT cubeCount;
			std::shared_ptr<Bind::VertexBuffer> pSphereVertices;
			std::shared_ptr<Bind::IndexBuffer> pSphereIndices;
			UINT sphereCount;
		};
	}
}