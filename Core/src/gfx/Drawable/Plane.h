#pragma once
#include <optional>
#include "../Vertex.h"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <math.h>

namespace Hydro::gfx
{

	class Plane
	{
	public:
		static IndexedTriangleList MakeTesselatedTextured( DynamicVertexBuffer layout, int divisions_x, int divisions_y )
		{
			assert( divisions_x >= 1 );
			assert( divisions_y >= 1 );

			constexpr float width = 2.0f;
			constexpr float height = 2.0f;
			const int nVertices_x = divisions_x + 1;
			const int nVertices_y = divisions_y + 1;

			const float step_x = width / nVertices_x;
			const float step_y = height / nVertices_y;
			const float step_u = 1.0f / nVertices_x;
			const float step_v = 1.0f / nVertices_y;

			DynamicVertexBuffer vb{ std::move( layout ) };

			for( int y = 0; y < nVertices_y; y++ )
			{
				for( int x = 0; x < nVertices_x; x++ )
				{
					DirectX::XMFLOAT3 calculatedPos;
					auto p = DirectX::XMVectorSet( x * step_x, y * step_y, 0.0f, 0.0f );
					DirectX::XMStoreFloat3( &calculatedPos, p );

					DirectX::XMFLOAT3 calculatedNormal;
					auto n = DirectX::XMVectorSet( 0.0f, 0.0f, -1.0f, 0.0f );
					DirectX::XMStoreFloat3( &calculatedNormal, n );

					DirectX::XMFLOAT2 calculatedTex;
					auto uv = DirectX::XMVectorSet( x * step_u, y * step_v, 0.0f, 0.0f );
					DirectX::XMStoreFloat2( &calculatedTex, uv );

					vb.EmplaceBack( calculatedPos, calculatedNormal, calculatedTex );
				}
			}

			std::vector<unsigned short> indices;

			const auto CalcIndex = [nVertices_x]( int x, int y )
			{ return y * nVertices_x + x; };

			for( int y = 0; y < nVertices_y - 1; y++ )
			{
				for( int x = 0; x < nVertices_x; x++ )
				{
					if( x != nVertices_x-1 )
					{
						indices.push_back( CalcIndex( x, y ) );
						indices.push_back( CalcIndex( x, y + 1 ) );
						indices.push_back( CalcIndex( x + 1, y ) );
					}
					if( x != 0 )
					{
						indices.push_back( CalcIndex( x, y ) );
						indices.push_back( CalcIndex( x - 1, y + 1 ) );
						indices.push_back( CalcIndex( x, y + 1) );
					}
				}
			}

			return { std::move( vb ), std::move( indices ) };
		}

		static IndexedTriangleList Make()
		{
			VertexLayout v1;
			v1.Append( VertexLayout::Position3D );
			v1.Append( VertexLayout::Normal );
			v1.Append( VertexLayout::Texture2D );

			return MakeTesselatedTextured( std::move( v1 ), 1, 1 );
		}
	};

}