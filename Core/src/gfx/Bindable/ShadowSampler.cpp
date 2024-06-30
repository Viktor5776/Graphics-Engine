#include "ShadowSampler.h"
#include "BindableCodex.h"


namespace Hydro::gfx::Bind
{

    ShadowSampler::ShadowSampler( Graphics& gfx )
	{
		HRESULT hr;

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

		GFX_THROW_FAILED( GetDevice( gfx )->CreateSamplerState( &samplerDesc, &pSampler ) );
	}

	void ShadowSampler::Bind( Graphics& gfx ) noexcept(!_DEBUG)
	{
		GetContext( gfx )->PSSetSamplers( 1, 1, pSampler.GetAddressOf() );
	}

}
