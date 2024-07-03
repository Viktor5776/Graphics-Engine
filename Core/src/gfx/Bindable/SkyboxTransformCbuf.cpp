#include "SkyboxTransformCbuf.h"

namespace Hydro::gfx::Bind
{
	SkyboxTransformCbuf::SkyboxTransformCbuf( Graphics& gfx, UINT slot )
		:
		pVcbuf{ std::make_unique<VertexConstantBuffer<Transforms>>( gfx,slot ) }
	{}

	void SkyboxTransformCbuf::Bind( Graphics& gfx ) noexcept(!_DEBUG)
	{
		UpdateBindImpl( gfx, GetTransforms( gfx ) );
	}

	void SkyboxTransformCbuf::UpdateBindImpl( Graphics& gfx, const Transforms& tf ) noexcept(!_DEBUG)
	{
		pVcbuf->Update( gfx, tf );
		pVcbuf->Bind( gfx );
	}

	SkyboxTransformCbuf::Transforms SkyboxTransformCbuf::GetTransforms( Graphics& gfx ) noexcept(!_DEBUG)
	{
		return {
			DirectX::XMMatrixTranspose( gfx.GetCamera() * gfx.GetProjection() )
		};
	}
}