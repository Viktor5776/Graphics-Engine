#include "VertexBuffer.h"

namespace Hydro::gfx
{

	void VertexBuffer::Bind( Graphics& gfx ) noexcept
	{
		const UINT offset = 0u;
		GetContext( gfx )->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
	}

}
