#pragma once
#include "Bindable.h"
#include "../../utility/Surface.h"

namespace Hydro::gfx::Bind
{
	class Texture : public Bindable
	{
	public:
		Texture( Graphics& gfx, const Hydro::utility::Surface& s, UINT slot = 0 );
		void Bind( Graphics& gfx ) noexcept override;
	private:
		UINT slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}