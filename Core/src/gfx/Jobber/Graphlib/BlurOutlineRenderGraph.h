#pragma once
#include "../RenderGraph.h"
#include <memory>
#include "../../Bindable/ConstantBuffersEx.h"

namespace Hydro::gfx
{

	class Graphics;
	class Camera;

	namespace Bind
	{
		class Bindable;
		class RenderTarget;
		class ShadowSampler;
		class ShadowRasterizer;
	}

	namespace Rgph
	{
		class BlurOutlineRenderGraph : public RenderGraph
		{
		public:
			BlurOutlineRenderGraph( Graphics& gfx );
			void RenderWindows( Graphics& gfx );
			void DumpShadowMap( Graphics& gfx, const std::string& path );
			void BindMainCamera( Camera& cam );
			void BindShadowCamera( Camera& cam );
			void StoreDepth( Graphics& gfx, const std::string& path );
		private:
			void RenderKernelWindow( Graphics& gfx );
			void RenderShadowWindow( Graphics& gfx );
			// private functions
			void SetKernelGauss( int radius, float sigma ) noexcept(!_DEBUG);
			void SetKernelBox( int radius ) noexcept(!_DEBUG);
			// private data
			enum class KernelType
			{
				Gauss,
				Box,
			} kernelType = KernelType::Gauss;
			static constexpr int maxRadius = 7;
			int radius = 4;
			float sigma = 2.0f;
			std::shared_ptr<Bind::CachingPixelConstantBufferEx> blurKernel;
			std::shared_ptr<Bind::CachingPixelConstantBufferEx> blurDirection;
		};
	}

}