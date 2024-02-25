#include "AssTest.h"
#include "../Bindable/BindableBase.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assert.h>
#include "../Vertex.h"

namespace Hydro::gfx
{

	AssTest::AssTest( Graphics& gfx, std::mt19937& rng, 
		std::uniform_real_distribution<float>& adist, 
		std::uniform_real_distribution<float>& ddist, 
		std::uniform_real_distribution<float>& odist, 
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& cdist,
		float scale )
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
			DynamicVertexBuffer vbuf( std::move(
				VertexLayout{}
				.Append( VertexLayout::Position3D )
				.Append( VertexLayout::Normal )
			) );

			Assimp::Importer imp;
			const auto pModel = imp.ReadFile( "models\\suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices );
			const auto pMesh = pModel->mMeshes[0];

			for( unsigned int i = 0; i < pMesh->mNumVertices; i++ )
			{
				vbuf.EmplaceBack( 
					DirectX::XMFLOAT3{ pMesh->mVertices[i].x * scale, pMesh->mVertices[i].y * scale, pMesh->mVertices[i].z * scale },
					*reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])
				 );
			} 
			std::vector<unsigned short> indices;
			indices.reserve( static_cast<size_t>(pMesh->mNumFaces) * 3 );
			for( unsigned int i = 0; i < pMesh->mNumFaces; i++ )
			{
				const auto& face = pMesh->mFaces[i];
				assert( face.mNumIndices == 3 );
				indices.push_back( face.mIndices[0] );
				indices.push_back( face.mIndices[1] );
				indices.push_back( face.mIndices[2] );
			}
				
			AddStaticBind( std::make_unique<VertexBuffer>( gfx, vbuf ) );

			auto pvs = std::make_unique<VertexShader>( gfx, L"PhongVS.cso" );
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind( std::move( pvs ) );

			AddStaticBind( std::make_unique<PixelShader>( gfx, L"PhongPS.cso" ) );


			AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, indices ) );

			AddStaticBind( std::make_unique<InputLayout>( gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));

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

		mat.color = { cdist( rng ), cdist( rng ), cdist( rng ) };

		AddBind( std::make_unique<PixelConstantBuffer<MaterialConsts>>( gfx, mat, 1 ) );
	}

	void AssTest::Update( float dt ) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX AssTest::GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
			DirectX::XMMatrixTranslation( r, 0.0f, 0.0f ) *
			DirectX::XMMatrixRotationRollPitchYaw( theta, phi, chi );
	}

}