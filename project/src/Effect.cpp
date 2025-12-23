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
