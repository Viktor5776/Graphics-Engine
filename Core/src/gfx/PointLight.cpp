#include "PointLight.h"
#include "../../third/ImGui/imgui.h"
#include "Camera.h"
#include "../misc/HydroMath.h"

namespace Hydro::gfx
{

	PointLight::PointLight( Graphics& gfx, DirectX::XMFLOAT3 pos, float radius, const size_t channels )
		:
		mesh( gfx, radius ),
		cbuf( gfx ),
		channels( channels )
	{
		home = {
			pos,
			{ 0.05f,0.05f,0.05f },
			{ 1.0f,1.0f,1.0f },
			1.0f,
			1.0f,
			0.025f,
			0.0030f,
		};

		Reset();
		pCamera = std::make_shared<Camera>( gfx, "Light", cbData.pos, 0.0f, PI / 2.0f, true );
	}

	void Hydro::gfx::PointLight::SpawnControlWindow() noexcept
	{
		if( ImGui::Begin( "Light" ) )
		{
			bool dirtyPos = false;
			const auto d = [&dirtyPos]( bool dirty ) { dirtyPos = dirtyPos || dirty; };

			ImGui::Text( "Position" );
			d( ImGui::SliderFloat( "X", &cbData.pos.x, -60.0f, 60.0f, "%.1f" ) );
			d( ImGui::SliderFloat( "Y", &cbData.pos.y, -60.0f, 60.0f, "%.1f" ) );
			d( ImGui::SliderFloat( "Z", &cbData.pos.z, -60.0f, 60.0f, "%.1f" ) );

			if( dirtyPos )
			{
				pCamera->SetPos( cbData.pos );
			}

			ImGui::Text( "Intensity/Color" );
			ImGui::SliderFloat( "Intensity", &cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", ImGuiSliderFlags_Logarithmic );
			ImGui::ColorEdit3( "Ambient", &cbData.ambient.x );
			ImGui::ColorEdit3( "Diffuse Color", &cbData.diffuseColor.x );
			ImGui::Text( "Falloff" );
			ImGui::SliderFloat( "Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic );
			ImGui::SliderFloat( "Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f", ImGuiSliderFlags_Logarithmic );
			ImGui::SliderFloat( "Quadratic", &cbData.attQuad, 0.0000001f, 1.0f, "%.7f", ImGuiSliderFlags_Logarithmic );
			if( ImGui::Button( "Reset" ) )
			{
				Reset();
			}
		}
		ImGui::End();
	}

	void PointLight::Reset() noexcept
	{
		cbData = home;
	}

	void PointLight::Reset( DirectX::XMFLOAT3 pos, size_t channels ) noexcept
	{
		home = {
			pos,
			{ 0.05f,0.05f,0.05f },
			{ 1.0f,1.0f,1.0f },
			1.0f,
			1.0f,
			0.025f,
			0.0030f,
		};

		this->channels = channels;

		cbData = home;
	}

	void PointLight::Submit( size_t channels ) const noexcept(!_DEBUG)
	{
		mesh.SetPos( cbData.pos );
		mesh.Submit( channels );
	}

	void PointLight::Bind( Graphics& gfx, DirectX::FXMMATRIX view ) const noexcept
	{
		auto dataCopy = cbData;
		const auto pos = DirectX::XMLoadFloat3( &cbData.pos );
		DirectX::XMStoreFloat3( &dataCopy.pos, DirectX::XMVector3Transform( pos, view ) );
		cbuf.Update( gfx, dataCopy );
		cbuf.Bind( gfx );
	}

	void PointLight::LinkTechniques( Rgph::RenderGraph& rg )
	{
		mesh.LinkTechniques( rg );
	}

	DirectX::XMFLOAT3 PointLight::GetPos() const noexcept
	{
		return cbData.pos;
	}

	size_t PointLight::GetChannels() const noexcept
	{
		return channels;
	}

	std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
	{
		return pCamera;
	}

}
