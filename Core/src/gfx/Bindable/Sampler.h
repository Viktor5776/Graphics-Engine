#pragma once
#include "Bindable.h"

namespace Hydro::gfx::Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler( Graphics& gfx );
		void Bind( Graphics& gfx ) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}