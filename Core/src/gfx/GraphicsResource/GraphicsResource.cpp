#include "GraphicsResource.h"
#include <stdexcept>

namespace Hydro::gfx
{

	ID3D11DeviceContext* GraphicsResource::GetContext( Graphics& gfx ) noexcept
	{
		return gfx.pContext.Get();
	}

	ID3D11Device* GraphicsResource::GetDevice( Graphics& gfx ) noexcept
	{
		return gfx.pDevice.Get();
	}

}