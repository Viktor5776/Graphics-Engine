#pragma once
#include "Graphics.h"
#include "Drawable\SolidSphere.h"
#include "Bindable\ConstantBuffers.h"

namespace Hydro::gfx
{
	class PointLight
	{
	public:
		PointLight( Graphics& gfx, float radius = 0.5f );
		void SpawnControlWindow() noexcept;
		void Reset() noexcept;
		void Submit( class FrameCommander& frame) const noexcept(!_DEBUG);
		void Bind( Graphics& gfx, DirectX::FXMMATRIX view ) const noexcept;
	private:
		struct PointLightCBuf
		{
			alignas(16) DirectX::XMFLOAT3 pos;
			alignas(16) DirectX::XMFLOAT3 ambient;
			alignas(16) DirectX::XMFLOAT3 diffuseColor;
			float diffuseIntensity;
			float attConst;
			float attLin;
			float attQuad;
		};
	private:
		PointLightCBuf pcb;
		mutable SolidSphere mesh;
		mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
	};
}