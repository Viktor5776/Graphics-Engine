#include "Texture.h"
#include "../GraphicsException.h"
#include "BindableCodex.h"
#include "../../misc/Surface.h"

namespace Hydro::gfx::Bind
{
	Texture::Texture( Graphics& gfx, const std::string& path, UINT slot )
		:
		path( path ),
		slot( slot )
	{
		HRESULT hr;

		//load surface
		const auto s = misc::Surface::FromFile( path );
		hasAlpha = s.AlphaLoaded();

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s.GetWidth();
		textureDesc.Height = s.GetHeight();
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0u;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = s.GetBufferPtr();
		sd.SysMemPitch = s.GetWidth() * sizeof( Hydro::misc::Surface::Color );
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_FAILED( GetDevice( gfx )->CreateTexture2D( &textureDesc, nullptr, &pTexture ) );

		// write image data into top mip level
		GetContext( gfx )->UpdateSubresource(
			pTexture.Get(), 0u, nullptr, s.GetBufferPtrConst(), s.GetWidth() * sizeof( Hydro::misc::Surface::Color ), 0u
		);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = -1;
		srvDesc.Texture2D.MostDetailedMip = 0u;
		GFX_THROW_FAILED( GetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &srvDesc, &pTextureView ) );

		// generate the mip chain using the gpu rendering pipeline
		GetContext( gfx )->GenerateMips( pTextureView.Get() );
	}

	void Texture::Bind( Graphics& gfx ) noexcept
	{
		GetContext( gfx )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() );
	}

	std::shared_ptr<Texture> Texture::Resolve( Graphics& gfx, const std::string& path, UINT slot )
	{
		return Codex::Resolve<Texture>( gfx, path, slot );
	}

	std::string Texture::GenerateUID( const std::string& path, UINT slot )
	{
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string( slot );
	}

	std::string Texture::GetUID() const noexcept
	{
		return GenerateUID( path, slot );
	}

	bool Texture::HasAlpha() const noexcept
	{
		return hasAlpha;
	}

	UINT Texture::CalculateNumberOfMipLevels( UINT width, UINT height ) noexcept
	{
		const float xSteps = std::ceil( log2( (float)width ) );
		const float ySteps = std::ceil( log2( (float)height ) );
		return (UINT)std::max( xSteps, ySteps );
	}
}