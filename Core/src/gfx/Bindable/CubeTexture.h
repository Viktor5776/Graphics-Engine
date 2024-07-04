#pragma once
#include "Bindable.h"

namespace Hydro::misc
{
	class Surface;
}

namespace Hydro::gfx::Bind
{
	class OutputOnlyDepthStencil;

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

	class DepthCubeTexture : public Bindable
	{
	public:
		DepthCubeTexture( Graphics& gfx, UINT size, UINT slot = 0 );
		void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		std::shared_ptr<OutputOnlyDepthStencil> GetDepthBuffer( size_t index ) const;
	private:
		unsigned int slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
		std::vector<std::shared_ptr<OutputOnlyDepthStencil>> depthBuffers;
	};
}