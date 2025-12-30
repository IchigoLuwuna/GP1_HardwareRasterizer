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
	Effect() = default;
	Effect( ID3D11Device* pDevice, const std::wstring& assetFile );
	Effect( const Effect& ) = delete;
	Effect( Effect&& rhs );
	Effect& operator=( const Effect& ) = delete;
	Effect& operator=( Effect&& rhs );

	~Effect();

	ID3DX11Effect* operator->(); // Access effect

	// Getters
	ID3DX11EffectTechnique* GetTechniquePtr() const;
	ID3D11InputLayout* GetInputLayoutPtr() const;

private:
	// HARDWARE RESOURCES: OWNING
	ID3DX11Effect* m_pEffect{};
	ID3D11InputLayout* m_pInputLayout{};
	//

	// HARDWARE RESOURCES: NON-OWNING
	ID3DX11EffectTechnique* m_pTechnique{};
	//

	ID3DX11Effect* LoadEffect( ID3D11Device* pDevice, const std::wstring& assetFile );
};

#endif
