#include "Effect.h"

Effect::Effect( ID3D11Device* pDevice, const std::string& assetFile )
{
}

Effect::~Effect()
{
	if ( m_pEffect )
	{
		m_pEffect->Release();
	}
}

ID3DX11Effect* Effect::LoadEffect( ID3D11Device* pDevice, const std::wstring& assetFile )
{
	HRESULT result{};
	ID3D10Blob* pErrorBlob{};
	ID3DX11Effect* pEffect{};

	DWORD shaderFlags{};
#if defined( DEBUG ) || defined( _DEBUG )
	shaderflags |= D3DCOMPILE_DEBUG;
	shaderflags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	result = D3DX11CompileEffectFromFile(
		assetFile.c_str(), nullptr, nullptr, shaderFlags, 0, pDevice, &pEffect, &pErrorBlob );

	return pEffect;
}
