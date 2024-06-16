#define DVTX_SOURCE_FILE
#include "Vertex.h"

namespace Hydro::gfx
{
	// VertexLayout
	const VertexLayout::Element& VertexLayout::ResolveByIndex( size_t i ) const noexcept(!_DEBUG)
	{
		return elements[i];
	}
	VertexLayout& VertexLayout::Append( ElementType type ) noexcept(!_DEBUG)
	{
		if( !Has( type ) )
		{
			elements.emplace_back( type, Size() );
		}
		return *this;
	}
	bool VertexLayout::Has( ElementType type ) const noexcept
	{
		for( auto& e : elements )
		{
			if( e.GetType() == type )
			{
				return true;
			}
		}
		return false;
	}
	size_t VertexLayout::Size() const noexcept(!_DEBUG)
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}
	size_t VertexLayout::GetElementCount() const noexcept
	{
		return elements.size();
	}
	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const noexcept(!_DEBUG)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve( GetElementCount() );
		for( const auto& e : elements )
		{
			desc.push_back( e.GetDesc() );
		}
		return desc;
	}
	std::string VertexLayout::GetCode() const noexcept(!_DEBUG)
	{
		std::string code;
		for( const auto& e : elements )
		{
			code += e.GetCode();
		}
		return code;
	}


	// VertexLayout::Element
	VertexLayout::Element::Element( ElementType type, size_t offset )
		:
		type( type ),
		offset( offset )
	{}
	size_t VertexLayout::Element::GetOffsetAfter() const noexcept(!_DEBUG)
	{
		return offset + Size();
	}
	size_t VertexLayout::Element::GetOffset() const
	{
		return offset;
	}
	size_t VertexLayout::Element::Size() const noexcept(!_DEBUG)
	{
		return SizeOf( type );
	}
	VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
	{
		return type;
	}

	template<VertexLayout::ElementType type>
	struct SysSizeLookup
	{
		static constexpr auto Exec() noexcept
		{
			return sizeof( VertexLayout::Map<type>::SysType );
		}
	};
	constexpr size_t VertexLayout::Element::SizeOf( ElementType type ) noexcept(!_DEBUG)
	{
		return Bridge<SysSizeLookup>( type );
	}

	template<VertexLayout::ElementType type>
	struct CodeLookup
	{
		static constexpr auto Exec() noexcept
		{
			return VertexLayout::Map<type>::code;
		}
	};
	const char* VertexLayout::Element::GetCode() const noexcept
	{
		return Bridge<CodeLookup>( type );
	}

	template<VertexLayout::ElementType type> struct DescGenerate {
		static constexpr D3D11_INPUT_ELEMENT_DESC Exec( size_t offset ) noexcept {
			return {
				VertexLayout::Map<type>::semantic,0,
				VertexLayout::Map<type>::dxgiFormat,
				0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0
			};
		}
	};
	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const noexcept(!_DEBUG)
	{
		return Bridge<DescGenerate>( type, GetOffset() );
	}


	// Vertex
	Vertex::Vertex( char* pData, const VertexLayout& layout ) noexcept(!_DEBUG)
		:
	pData( pData ),
		layout( layout )
	{
		assert( pData != nullptr );
	}
	ConstVertex::ConstVertex( const Vertex& v ) noexcept(!_DEBUG)
		:
	vertex( v )
	{}


	// VertexBuffer
	DynamicVertexBuffer::DynamicVertexBuffer( VertexLayout layout, size_t size ) noexcept(!_DEBUG)
		:
	layout( std::move( layout ) )
	{
		Resize( size );
	}

	void DynamicVertexBuffer::Resize( size_t newSize ) noexcept(!_DEBUG)
	{
		const auto size = Size();
		if( size < newSize )
		{
			buffer.resize( buffer.size() + layout.Size() * (newSize - size) );
		}
	}
	const char* DynamicVertexBuffer::GetData() const noexcept(!_DEBUG)
	{
		return buffer.data();
	}

	template<VertexLayout::ElementType type>
	struct AttributeAiMeshFill
	{
		static constexpr void Exec( DynamicVertexBuffer* pBuf, const aiMesh& mesh ) noexcept(!_DEBUG)
		{
			for( auto end = mesh.mNumVertices, i = 0u; i < end; i++ )
			{
				(*pBuf)[i].Attr<type>() = VertexLayout::Map<type>::Extract( mesh, i );
			}
		}
	};
	DynamicVertexBuffer::DynamicVertexBuffer( VertexLayout layout_in, const aiMesh& mesh )
		:
		layout( std::move( layout_in ) )
	{
		Resize( mesh.mNumVertices );
		Resize( mesh.mNumVertices );
		for( size_t i = 0, end = layout.GetElementCount(); i < end; i++ )
		{
			VertexLayout::Bridge<AttributeAiMeshFill>( layout.ResolveByIndex( i ).GetType(), this, mesh );
		}
	}

	const VertexLayout& DynamicVertexBuffer::GetLayout() const noexcept
	{
		return layout;
	}
	size_t DynamicVertexBuffer::Size() const noexcept(!_DEBUG)
	{
		return buffer.size() / layout.Size();
	}
	size_t DynamicVertexBuffer::SizeBytes() const noexcept(!_DEBUG)
	{
		return buffer.size();
	}
	Vertex DynamicVertexBuffer::Back() noexcept(!_DEBUG)
	{
		assert( buffer.size() != 0u );
		return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
	}
	Vertex DynamicVertexBuffer::Front() noexcept(!_DEBUG)
	{
		assert( buffer.size() != 0u );
		return Vertex{ buffer.data(),layout };
	}
	Vertex DynamicVertexBuffer::operator[]( size_t i ) noexcept(!_DEBUG)
	{
		assert( i < Size() );
		return Vertex{ buffer.data() + layout.Size() * i,layout };
	}
	ConstVertex DynamicVertexBuffer::Back() const noexcept(!_DEBUG)
	{
		return const_cast<DynamicVertexBuffer*>(this)->Back();
	}
	ConstVertex DynamicVertexBuffer::Front() const noexcept(!_DEBUG)
	{
		return const_cast<DynamicVertexBuffer*>(this)->Front();
	}
	ConstVertex DynamicVertexBuffer::operator[]( size_t i ) const noexcept(!_DEBUG)
	{
		return const_cast<DynamicVertexBuffer&>(*this)[i];
	}
}