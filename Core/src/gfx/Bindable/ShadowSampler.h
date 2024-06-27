#pragma once
#include "Bindable.h"

namespace Hydro::gfx::Bind
{
	class ShadowSampler : public Bindable
	{
	public:
		ShadowSampler( Graphics& gfx );
		void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}