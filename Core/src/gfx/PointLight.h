#pragma once
#include "Graphics.h"
#include "Drawable\SolidSphere.h"
#include "Bindable\ConstantBuffers.h"

namespace Hydro::gfx
{
	namespace Rgph
	{
		class RenderGraph;
	}

	class Camera;

	class PointLight
	{
	public:
		PointLight( Graphics& gfx, DirectX::XMFLOAT3 pos = { 10.0f,9.0f,2.5f }, float radius = 0.5f, const size_t channels = 0 );
		void SpawnControlWindow() noexcept;
		void Reset() noexcept;
		void Reset( DirectX::XMFLOAT3 pos, size_t channels ) noexcept;
		void Submit( size_t channels ) const noexcept(!_DEBUG);
		void Bind( Graphics& gfx, DirectX::FXMMATRIX view ) const noexcept;
		void LinkTechniques( Rgph::RenderGraph& );
		size_t GetChannels() const noexcept;
		std::shared_ptr<Camera> ShareCamera() const noexcept;
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
		size_t channels;
		PointLightCBuf home;
		PointLightCBuf cbData;
		mutable SolidSphere mesh;
		mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
		std::shared_ptr<Camera> pCamera;
	};
}