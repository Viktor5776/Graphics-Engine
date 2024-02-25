#include "Box.h"
#include "../Bindable/BindableBase.h"
#include "Cube.h"

namespace Hydro::gfx
{

	Box::Box( Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		std::uniform_real_distribution<float>& cdist )
		:
		r( rdist( rng ) ),
		droll( ddist( rng ) ),
		dpitch( ddist( rng ) ),
		dyaw( ddist( rng ) ),
		dphi( odist( rng ) ),
		dtheta( odist( rng ) ),
		dchi( odist( rng ) ),
		chi( adist( rng ) ),
		theta( adist( rng ) ),
		phi( adist( rng ) ),
		mt()
	{
		if( !IsStaticInitialized() )
		{
			struct Vertex
			{
				DirectX::XMFLOAT3 pos;
				DirectX::XMFLOAT3 n;
			};
			auto model = Cube::MakeIndependent<Vertex>();
			model.SetNormalsIndependentFlat();

			AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

			auto pvs = std::make_unique<VertexShader>( gfx, L"PhongVS.cso" );
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind( std::move( pvs ) );

			AddStaticBind( std::make_unique<PixelShader>( gfx, L"PhongPS.cso" ) );

			
			AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind( std::make_unique<InputLayout>( gfx, ied, pvsbc ) );

			AddStaticBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind( std::make_unique<TransformCbuf>( gfx, *this ) );

		struct MaterialConsts
		{
			alignas(16) DirectX::XMFLOAT3 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[1] = { 0 };
		} mat;

		mat.color = { cdist(rng), cdist(rng), cdist(rng) };

		AddBind( std::make_unique<PixelConstantBuffer<MaterialConsts>>( gfx, mat, 1 ) );

		DirectX::XMStoreFloat3x3(
			&mt,
			DirectX::XMMatrixScaling( 1.0f, 1.0f, bdist( rng ) )
		);
	}

	void Box::Update( float dt ) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX Box::GetTransformXM() const noexcept
	{
		return DirectX::XMLoadFloat3x3( &mt )* DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
			DirectX::XMMatrixTranslation( r, 0.0f, 0.0f ) *
			DirectX::XMMatrixRotationRollPitchYaw( theta, phi, chi );
	}

}