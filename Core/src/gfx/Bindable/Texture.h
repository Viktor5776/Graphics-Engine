#pragma once
#include "Bindable.h"
#include "../../utility/Surface.h"

namespace Hydro::gfx
{
	class Texture : public Bindable
	{
	public:
		Texture( Graphics& gfx, const Hydro::utility::Surface& s );
		void Bind( Graphics& gfx ) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}