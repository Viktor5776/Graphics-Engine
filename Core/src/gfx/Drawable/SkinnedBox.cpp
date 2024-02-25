#include "SkinnedBox.h"
#include "../Bindable/BindableBase.h"
#include "../GraphicsException.h"
#include "Cube.h"
#include "../../utility/Surface.h"

namespace Hydro::gfx
{
	SkinnedBox::SkinnedBox( Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist )
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
		phi( adist( rng ) )
	{

		if( !IsStaticInitialized() )
		{
			struct Vertex
			{
				DirectX::XMFLOAT3 pos;
				DirectX::XMFLOAT3 n;
				struct
				{
					float u;
					float v;
				} tc;
			};
			auto model = Cube::MakeIndependentSkinned<Vertex>();
			model.SetNormalsIndependentFlat();

			AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

			AddStaticBind( std::make_unique<Texture>( gfx, Hydro::utility::Surface::FromFile( "Images\\cube.png" ) ) );

			AddStaticBind( std::make_unique<Sampler>( gfx ) );

			auto pvs = std::make_unique<VertexShader>( gfx, L"TexturedPhongVS.cso" );
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind( std::move( pvs ) );

			AddStaticBind( std::make_unique<PixelShader>( gfx, L"TexturedPhongPS.cso" ) );

			AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
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
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} mat;

		AddBind( std::make_unique<PixelConstantBuffer<MaterialConsts>>( gfx, mat, 1 ) );
	}

	void SkinnedBox::Update( float dt ) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX SkinnedBox::GetTransformXM() const noexcept
	{
		namespace dx = DirectX;
		return dx::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
			dx::XMMatrixTranslation( r, 0.0f, 0.0f ) *
			dx::XMMatrixRotationRollPitchYaw( theta, phi, chi );
	}
}