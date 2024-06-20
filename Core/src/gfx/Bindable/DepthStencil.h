#pragma once
#include "Bindable.h"
#include "BufferResource.h"

namespace Hydro::gfx
{

	class Graphics;

	namespace Bind
	{
		class RenderTarget;

		class DepthStencil : public Bindable, public BufferResource
		{
			friend RenderTarget;
		public:
			void BindAsBuffer( Graphics& gfx ) noexcept(!_DEBUG) override;
			void BindAsBuffer( Graphics& gfx, BufferResource* renderTarget ) noexcept(!_DEBUG) override;
			void BindAsBuffer( Graphics& gfx, RenderTarget* rt ) noexcept(!_DEBUG);
			void Clear( Graphics& gfx ) noexcept(!_DEBUG) override;
		protected:
			DepthStencil( Graphics& gfx, UINT width, UINT height, bool canBindShaderInput );
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
		};

		class ShaderInputDepthStencil : public DepthStencil
		{
		public:
			ShaderInputDepthStencil( Graphics& gfx, UINT slot );
			ShaderInputDepthStencil( Graphics& gfx, UINT width, UINT height, UINT slot );
			void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		private:
			UINT slot;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
		};

		class OutputOnlyDepthStencil : public DepthStencil
		{
		public:
			OutputOnlyDepthStencil( Graphics& gfx );
			OutputOnlyDepthStencil( Graphics& gfx, UINT width, UINT height );
			void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		};
	}

}