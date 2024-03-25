#include "Texture.h"
#include "../GraphicsException.h"

namespace Hydro::gfx::Bind
{
	Texture::Texture( Graphics& gfx, const Hydro::utility::Surface& s, UINT slot )
		:
		slot( slot )
	{
		HRESULT hr;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s.GetWidth();
		textureDesc.Height = s.GetHeight();
		textureDesc.MipLevels = 1u;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0u;
		textureDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = s.GetBufferPtr();
		sd.SysMemPitch = s.GetWidth() * sizeof( Hydro::utility::Surface::Color );
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_FAILED( GetDevice( gfx )->CreateTexture2D( &textureDesc, &sd, &pTexture ) );
		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0u;
		GFX_THROW_FAILED( GetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &srvDesc, &pTextureView ) );
	}

	void Texture::Bind( Graphics& gfx ) noexcept
	{
		GetContext( gfx )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() );
	}
}