#include "PointLight.h"
#include "../../third/ImGui/imgui.h"
#include "Jobber\FrameCommander.h"

namespace Hydro::gfx
{

	PointLight::PointLight( Graphics& gfx, float radius )
		:
		mesh( gfx, radius ),
		cbuf( gfx )
	{
		Reset();
	}

	void Hydro::gfx::PointLight::SpawnControlWindow() noexcept
	{
		if( ImGui::Begin( "Light" ) )
		{
			ImGui::Text( "Position" );
			ImGui::SliderFloat( "X", &pcb.pos.x, -60.0f, 60.0f, "%.1f" );
			ImGui::SliderFloat( "Y", &pcb.pos.y, -60.0f, 60.0f, "%.1f" );
			ImGui::SliderFloat( "Z", &pcb.pos.z, -60.0f, 60.0f, "%.1f" );
			ImGui::Text( "Intensity/Color" );
			ImGui::SliderFloat( "Intensity", &pcb.diffuseIntensity, 0.01f, 2.0f, "%.2f" );
			ImGui::ColorEdit3( "Ambient", &pcb.ambient.x );
			ImGui::ColorEdit3( "Diffuse Color", &pcb.diffuseColor.x );
			ImGui::Text( "Falloff" );
			ImGui::SliderFloat( "Constant", &pcb.attConst, 0.05f, 10.0f, "%.2f" );
			ImGui::SliderFloat( "Linear", &pcb.attLin, 0.0001f, 4.0f, "%.4f" );
			ImGui::SliderFloat( "Quadratic", &pcb.attQuad, 0.0000001f, 1.0f, "%.7f" );
			if( ImGui::Button( "Reset" ) )
			{
				Reset();
			}
		}
		ImGui::End();
	}

	void PointLight::Reset() noexcept
	{
		pcb.pos = { 2.0f,9.0f,-17.0f };
		pcb.ambient = { 0.05f, 0.05f, 0.05f };
		pcb.diffuseColor = { 1.0f, 1.0f, 1.0f };
		pcb.diffuseIntensity = 1.0f;
		pcb.attConst = 1.0f;
		pcb.attLin = 0.045f;
		pcb.attQuad = 0.0075f;
	}

	void PointLight::Submit( FrameCommander& frame ) const noexcept(!_DEBUG)
	{
		mesh.SetPos( pcb.pos );
		mesh.Submit( frame );
	}

	void PointLight::Bind( Graphics& gfx, DirectX::FXMMATRIX view ) const noexcept
	{
		auto dataCopy = pcb;
		const auto pos = DirectX::XMLoadFloat3( &pcb.pos );
		DirectX::XMStoreFloat3( &dataCopy.pos, DirectX::XMVector3Transform( pos, view ) );
		cbuf.Update( gfx, dataCopy );
		cbuf.Bind( gfx );
	}

}
