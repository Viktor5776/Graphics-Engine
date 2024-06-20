#pragma once
#include "Bindable.h"
#include "BufferResource.h"

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
			RenderTarget( Graphics& gfx, ID3D11Texture2D* pTexture );
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
		private:
			UINT slot;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
		};

		// RT for Graphics to create RenderTarget for the back buffer
		class OutputOnlyRenderTarget : public RenderTarget
		{
			friend Graphics;
		public:
			void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		private:
			OutputOnlyRenderTarget( Graphics& gfx, ID3D11Texture2D* pTexture );
		};
	}
}