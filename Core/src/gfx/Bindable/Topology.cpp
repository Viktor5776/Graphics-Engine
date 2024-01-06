#include "Topology.h"

namespace Hydro::gfx
{

	Topology::Topology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY topology )
		:
		topology( topology )
	{}

	void Topology::Bind( Graphics& gfx ) noexcept
	{
		GetContext( gfx )->IASetPrimitiveTopology( topology );
	}

}
