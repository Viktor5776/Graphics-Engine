#pragma once
#include "ConstantBuffers.h"
#include "../Drawable/Drawable.h"
#include <DirectXMath.h>

namespace Hydro::gfx
{
	class Camera;

	namespace Bind
	{
		class SkyboxTransformCbuf : public Bindable
		{
		protected:
			struct Transforms
			{
				DirectX::XMMATRIX viewProj;
			};
		public:
			SkyboxTransformCbuf( Graphics& gfx, UINT slot = 0u );
			void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		protected:
			void UpdateBindImpl( Graphics& gfx, const Transforms& tf ) noexcept(!_DEBUG);
			Transforms GetTransforms( Graphics& gfx ) noexcept(!_DEBUG);
		private:
			std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		};
	}
}