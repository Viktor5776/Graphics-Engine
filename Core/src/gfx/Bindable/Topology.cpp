#include "Topology.h"
#include "BindableCodex.h"

namespace Hydro::gfx::Bind
{

	Topology::Topology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY topology )
		:
		topology( topology )
	{}

	void Topology::Bind( Graphics& gfx ) noexcept(!_DEBUG)
	{
		GetContext( gfx )->IASetPrimitiveTopology( topology );
	}

	std::shared_ptr<Topology> Topology::Resolve( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type )
	{
		return Codex::Resolve<Topology>( gfx, type );
	}

	std::string Topology::GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type )
	{
		using namespace std::string_literals;
		return typeid(Topology).name() + "#"s + std::to_string( type );
	}

	std::string Topology::GetUID() const noexcept
	{
		return GenerateUID( topology );
	}

}
