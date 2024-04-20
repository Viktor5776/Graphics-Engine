#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

namespace Hydro::gfx
{
	using namespace Bind;

	void Drawable::Draw( Graphics& gfx ) const noexcept(!_DEBUG)
	{
		for( auto& b : binds )
		{
			b->Bind( gfx );
		}
		gfx.DrawIndexed( pIndexBuffer->GetCount() );
	}

	void Drawable::AddBind( std::shared_ptr<Bindable> bind ) noexcept(!_DEBUG)
	{
		if( typeid(*bind) == typeid(IndexBuffer) )
		{
			assert( "Binding multiple index buffers not allowed" && pIndexBuffer == nullptr );
			pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
		}
		binds.push_back( std::move( bind ) );
	}
}