#include "Sampler.h"
#include "../GraphicsException.h"
#include "BindableCodex.h"

namespace Hydro::gfx::Bind
{
	Sampler::Sampler( Graphics& gfx )
	{
		HRESULT hr;

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		GFX_THROW_FAILED( GetDevice( gfx )->CreateSamplerState( &samplerDesc, &pSampler ) );
	}

	void Sampler::Bind( Graphics& gfx ) noexcept
	{
		GetContext( gfx )->PSSetSamplers( 0u, 1u, pSampler.GetAddressOf() );
	}

	std::shared_ptr<Sampler> Sampler::Resolve( Graphics& gfx )
	{
		return Codex::Resolve<Sampler>( gfx );
	}

	std::string Sampler::GenerateUID()
	{
		return typeid(Sampler).name();
	}

	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID();
	}
}