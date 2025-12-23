#ifndef EFFECT_H
#define EFFECT_H

// This is an RAII wrapper around DirectX effects
// DirectX Headers
#include <string>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

class Effect
{
public:
	Effect( ID3D11Device* pDevice, const std::string& assetFile );
	~Effect();

	Effect( const Effect& ) = delete;
	Effect( Effect&& ) = default;
	Effect& operator=( const Effect& ) = delete;
	Effect& operator=( Effect&& ) = default;

private:
#ifdef WIN32
	ID3DX11Effect* m_pEffect{};
#else
	ID3D10Effect* m_pEffect{};
#endif

	ID3DX11Effect* LoadEffect( ID3D11Device* pDevice, const std::string& assetFile );
};

#endif
