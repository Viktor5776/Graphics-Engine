#pragma once
#include "Bindable.h"
#include "BufferResource.h"
#include "../../misc/Surface.h"

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
			enum class Usage
			{
				DepthStencil,
				ShadowDepth,
			};
		public:
			void BindAsBuffer( Graphics& gfx ) noexcept(!_DEBUG) override;
			void BindAsBuffer( Graphics& gfx, BufferResource* renderTarget ) noexcept(!_DEBUG) override;
			void BindAsBuffer( Graphics& gfx, RenderTarget* rt ) noexcept(!_DEBUG);
			void Clear( Graphics& gfx ) noexcept(!_DEBUG) override;
			misc::Surface ToSurface( Graphics& gfx, bool linearlize = true ) const;
			unsigned int GetWidth() const;
			unsigned int GetHeight() const;
		protected:
			DepthStencil( Graphics& gfx, UINT width, UINT height, bool canBindShaderInput, Usage usage );
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
			unsigned int width;
			unsigned int height;
		};

		class ShaderInputDepthStencil : public DepthStencil
		{
		public:
			ShaderInputDepthStencil( Graphics& gfx, UINT slot, Usage usage = Usage::DepthStencil );
			ShaderInputDepthStencil( Graphics& gfx, UINT width, UINT height, UINT slot, Usage usage = Usage::DepthStencil );
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