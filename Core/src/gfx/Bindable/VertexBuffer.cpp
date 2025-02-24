#include "VertexBuffer.h"
#include "BindableCodex.h"

namespace Hydro::gfx::Bind
{

	VertexBuffer::VertexBuffer( Graphics& gfx, const DynamicVertexBuffer& vbuf )
		:
		VertexBuffer( gfx, "?", vbuf )
	{}


	VertexBuffer::VertexBuffer( Graphics& gfx, const std::string& tag, const DynamicVertexBuffer& vbuf )
		:
		stride( (UINT)vbuf.GetLayout().Size() ),
		tag( tag ),
		layout( vbuf.GetLayout() )
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT( vbuf.SizeBytes() );
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.GetData();
		GFX_THROW_FAILED( GetDevice( gfx )->CreateBuffer( &bd, &sd, &pVertexBuffer ) );
	}

	const VertexLayout& VertexBuffer::GetLayout() const noexcept
	{
		return layout;
	}

	void VertexBuffer::Bind( Graphics& gfx ) noexcept(!_DEBUG)
	{
		const UINT offset = 0u;
		GetContext( gfx )->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
	}
	std::shared_ptr<VertexBuffer> VertexBuffer::Resolve( Graphics& gfx, const std::string& tag,
		const DynamicVertexBuffer& vbuf )
	{
		assert( tag != "?" );
		return Codex::Resolve<VertexBuffer>( gfx, tag, vbuf );
	}
	std::string VertexBuffer::GenerateUID_( const std::string& tag )
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
	std::string VertexBuffer::GetUID() const noexcept
	{
		return GenerateUID( tag );
	}

}
