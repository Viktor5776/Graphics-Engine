#pragma once
#include "Bindable.h"

namespace Hydro::misc
{
	class Surface;
}

namespace Hydro::gfx::Bind
{
	class CubeTexture : public Bindable
	{
	public:
		CubeTexture( Graphics& gfx, const std::string& path, UINT slot = 0 );
		void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
	private:
		unsigned int slot;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}