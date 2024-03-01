#pragma once
#include "Bindable.h"

namespace Hydro::gfx::Bind
{
	class Topology : public Bindable
	{
	public:
		Topology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY topology );
		void Bind( Graphics& gfx ) noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};
}