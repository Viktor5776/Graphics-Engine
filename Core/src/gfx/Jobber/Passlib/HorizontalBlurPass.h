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
		class HorizontalBlurPass : public FullscreenPass
		{
		public:
			HorizontalBlurPass( std::string name, Graphics& gfx, unsigned int fullWidth, unsigned int fullHeight );
			void Execute( Graphics& gfx ) const noexcept(!_DEBUG) override;
		private:
			std::shared_ptr<Bind::CachingPixelConstantBufferEx> direction;
		};
	}

}