#pragma once
#include "Base/FullscreenPass.h"
#include "../../Bindable/ConstantBuffersEx.h"

namespace Hydro::gfx
{
	class Graphics;
	namespace Bind
	{
		class PixelShader;
		class RenderTarget;
	}

	namespace Rgph
	{
		class VerticalBlurPass : public FullscreenPass
		{
		public:
			VerticalBlurPass( std::string name, Graphics& gfx );
			void Execute( Graphics& gfx ) const noexcept(!_DEBUG) override;
		private:
			std::shared_ptr<Bind::CachingPixelConstantBufferEx> direction;
		};
	}
}