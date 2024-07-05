#pragma once
#include "Bindable.h"
#include "BufferResource.h"
#include <optional>

namespace Hydro::misc
{
	class Surface;
}

namespace Hydro::gfx
{
	class Graphics;

	namespace Bind
	{
		class DepthStencil;

		class RenderTarget : public Bindable, public BufferResource
		{
		public:
			void BindAsBuffer( Graphics& gfx ) noexcept(!_DEBUG) override;
			void BindAsBuffer( Graphics& gfx, BufferResource* depthStencil ) noexcept(!_DEBUG) override;
			void BindAsBuffer( Graphics& gfx, DepthStencil* depthStencil ) noexcept(!_DEBUG);
			void Clear( Graphics& gfx ) noexcept(!_DEBUG) override;
			void Clear( Graphics& gfx, const std::array<float, 4>& color ) noexcept(!_DEBUG);
			UINT GetWidth() const noexcept;
			UINT GetHeight() const noexcept;
		private:
			void BindAsBuffer( Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView ) noexcept(!_DEBUG);
		protected:
			RenderTarget( Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<UINT> face );
			RenderTarget( Graphics& gfx, UINT width, UINT height );
			UINT width;
			UINT height;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
		};

		class ShaderInputRenderTarget : public RenderTarget
		{
		public:
			ShaderInputRenderTarget( Graphics& gfx, UINT width, UINT height, UINT slot );
			void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
			misc::Surface ToSurface( Graphics& gfx ) const;
		private:
			UINT slot;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
		};

		class OutputOnlyRenderTarget : public RenderTarget
		{
		public:
			void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
			OutputOnlyRenderTarget( Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<UINT> face = {} );
		};
	}
}