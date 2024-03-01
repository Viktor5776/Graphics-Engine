#pragma once
#include "Bindable.h"

namespace Hydro::gfx::Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader( Graphics& gfx, const std::wstring& path );
		void Bind( Graphics& gfx ) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}