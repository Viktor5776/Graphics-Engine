 #include "SolidSphere.h"
#include "../Bindable/BindableCommon.h"
#include "Sphere.h"

namespace Hydro::gfx
{
	SolidSphere::SolidSphere( Graphics& gfx, float radius )
	{
		using namespace Bind;
		if( !IsStaticInitialized() )
		{
			struct Vertex
			{
				DirectX::XMFLOAT3 pos;
			};

			auto model = Sphere::Make<Vertex>();
			model.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );

			AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );	

			auto pvs = std::make_unique<VertexShader>( gfx, L"SolidVS.cso" );
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind( std::move( pvs ) );

			AddStaticBind( std::make_unique<PixelShader>( gfx, L"SolidPS.cso" ) );

			AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

			struct PSColorConstant
			{
				float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			} colorConst;
			AddStaticBind( std::make_unique<PixelConstantBuffer<PSColorConstant>>( gfx, colorConst ) );

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind( std::make_unique<InputLayout>( gfx, ied, pvsbc ) );

			AddStaticBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind( std::make_unique<TransformCbuf>( gfx, *this ) );
	}

	void SolidSphere::Update( float dt ) noexcept {}

	void SolidSphere::SetPos( DirectX::XMFLOAT3 pos ) noexcept
	{
		this->pos = pos;
	}

	DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
	}
}
