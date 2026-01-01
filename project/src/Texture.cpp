#include "Texture.h"
#include <SDL_image.h>
#include <SDL_surface.h>
#include "Error.h"

namespace dae
{
Texture::Texture( ID3D11Device* pDevice, const std::string& texturePath )
{
	HRESULT result{};

	SDL_Surface* pSurface{ IMG_Load( texturePath.c_str() ) };

	DXGI_FORMAT format{ DXGI_FORMAT_R8G8B8A8_UNORM };
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = pSurface->w;
	desc.Height = pSurface->h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA texData{};
	texData.pSysMem = pSurface->pixels;
	texData.SysMemPitch = static_cast<UINT>( pSurface->pitch );
	texData.SysMemSlicePitch = static_cast<UINT>( pSurface->h * pSurface->pitch );

	result = pDevice->CreateTexture2D( &desc, &texData, &m_pResource );
	if ( FAILED( result ) )
	{
		throw error::texture::ResourceCreateFail();
	}

	SDL_FreeSurface( pSurface );

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
	resourceViewDesc.Format = format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = 1;

	result = pDevice->CreateShaderResourceView( m_pResource, &resourceViewDesc, &m_pResourceView );
	if ( FAILED( result ) )
	{
		throw error::texture::ResourceViewCreateFail();
	}
}

Texture::Texture( Texture&& rhs )
{
	if ( this == &rhs )
	{
		return;
	}

	m_pResource = rhs.m_pResource;
	rhs.m_pResource = nullptr;

	m_pResourceView = rhs.m_pResourceView;
	rhs.m_pResourceView = nullptr;
}

Texture& Texture::operator=( Texture&& rhs )
{
	if ( this == &rhs )
	{
		return *this;
	}

	m_pResource = rhs.m_pResource;
	rhs.m_pResource = nullptr;

	m_pResourceView = rhs.m_pResourceView;
	rhs.m_pResourceView = nullptr;

	return *this;
}

Texture::~Texture() noexcept
{
	if ( m_pResourceView )
	{
		m_pResourceView->Release();
	}

	if ( m_pResource )
	{
		m_pResource->Release();
	}
}

ID3D11ShaderResourceView* Texture::GetSRV() const
{
	return m_pResourceView;
}
} // namespace dae
