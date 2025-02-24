#pragma once
#include "Bindable.h"

namespace Hydro::gfx::Bind
{
	class Topology : public Bindable
	{
	public:
		Topology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY topology );
		void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		static std::shared_ptr<Topology> Resolve( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		static std::string GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type );
		std::string GetUID() const noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};
}