 #include "SolidSphere.h"
#include "../Bindable/BindableCommon.h"
#include "Sphere.h"
#include "../Vertex.h"
#include "../Bindable/Stencil.h"
#include <string>
#include "../Jobber/Channels.h"

namespace Hydro::gfx
{
	SolidSphere::SolidSphere( Graphics& gfx, float radius )
	{
		using namespace Bind;
			
		auto model = Sphere::Make();
		model.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );
		const auto geometryTag = "$sphere." + std::to_string( radius );
		pVertices = VertexBuffer::Resolve( gfx, geometryTag, model.vertices );
		pIndices = IndexBuffer::Resolve( gfx, geometryTag, model.indices );
		pTopology = Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		{
			Technique solid{ Channels::main };
			Step only( "lambertian" );

			auto pvs = VertexShader::Resolve( gfx, "Solid_VS.cso" );
			only.AddBindable( InputLayout::Resolve( gfx, model.vertices.GetLayout(), *pvs ) );
			only.AddBindable( std::move( pvs ) );

			only.AddBindable( PixelShader::Resolve( gfx, "Solid_PS.cso" ) );

			struct PSColorConstant
			{
				DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
				float padding = 0.0f;
			} colorConst;
			only.AddBindable( PixelConstantBuffer<PSColorConstant>::Resolve( gfx, colorConst, 1u ) );

			only.AddBindable( std::make_shared<TransformCbuf>( gfx ) );

			only.AddBindable( Rasterizer::Resolve( gfx, false ) );

			solid.AddStep( std::move( only ) );
			AddTechnique( std::move( solid ) );
		}
	}

	void SolidSphere::SetPos( DirectX::XMFLOAT3 pos ) noexcept
	{
		this->pos = pos;
	}

	DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
	}
}
