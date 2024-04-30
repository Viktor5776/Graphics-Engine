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

			DynamicVertexBuffer vb{ std::move( layout ) };

			{
				const float side_x = width / 2.0f;
				const float side_y = height / 2.0f;
				const float divisionSize_x = width / float( divisions_x );
				const float divisionSize_y = height / float( divisions_y );
				const float divisionSize_x_tc = 1.0f / float( divisions_x );
				const float divisionSize_y_tc = 1.0f / float( divisions_y );

				for( int y = 0, i = 0; y < nVertices_y; y++ )
				{
					const float y_pos = float( y ) * divisionSize_y - 1.0f;
					const float y_pos_tc = 1.0f - float( y ) * divisionSize_y_tc;
					for( int x = 0; x < nVertices_x; x++, i++ )
					{
						const float x_pos = float( x ) * divisionSize_x - 1.0f;
						const float x_pos_tc = float( x ) * divisionSize_x_tc;
						vb.EmplaceBack(
							DirectX::XMFLOAT3{ x_pos,y_pos,0.0f },
							DirectX::XMFLOAT3{ 0.0f,0.0f,-1.0f },
							DirectX::XMFLOAT2{ x_pos_tc,y_pos_tc }
						);
					}
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