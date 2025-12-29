#include "Effect.h"
#include <iostream>
#include <sstream>
#include "Error.h"

Effect::Effect( ID3D11Device* pDevice, const std::wstring& assetFile )
{
	m_pEffect = LoadEffect( pDevice, assetFile );

	if ( !m_pEffect )
	{
		throw error::effect::CreateFail();
	}

	if ( !m_pEffect->IsValid() )
	{
		throw error::effect::InvalidEffect();
	}

	m_pTechnique = m_pEffect->GetTechniqueByName( "DefaultTechnique" );

	if ( !m_pTechnique->IsValid() )
	{
		throw error::effect::InvalidTechnique();
	}

	// Create Vertex Layout
	constexpr int elementCount{ 2 };
	D3D11_INPUT_ELEMENT_DESC vertexDescription[elementCount]{};

	vertexDescription[0].SemanticName = "POSITION";
	vertexDescription[0].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDescription[0].AlignedByteOffset = 0;
	vertexDescription[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDescription[1].SemanticName = "COLOR";
	vertexDescription[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDescription[1].AlignedByteOffset = 12;
	vertexDescription[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//

	// Create Input Layout
	D3DX11_PASS_DESC passDesc{};
	m_pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );

	HRESULT result{ pDevice->CreateInputLayout(
		vertexDescription, elementCount, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout ) };
	if ( FAILED( result ) )
	{
		throw error::effect::LayoutCreateFail();
	}
	//
}

Effect::Effect( Effect&& rhs )
{
	m_pEffect = rhs.m_pEffect;
	rhs.m_pEffect = nullptr;

	m_pTechnique = rhs.m_pTechnique;
	rhs.m_pTechnique = nullptr;
}

Effect& Effect::operator=( Effect&& rhs )
{
	m_pEffect = rhs.m_pEffect;
	rhs.m_pEffect = nullptr;

	m_pTechnique = rhs.m_pTechnique;
	rhs.m_pTechnique = nullptr;

	return *this;
}

Effect::~Effect()
{
	if ( m_pEffect )
	{
		m_pEffect->Release();
	}

	if ( m_pTechnique )
	{
		m_pTechnique->Release();
	}
}

ID3DX11Effect* Effect::operator->()
{
	return m_pEffect;
}

ID3DX11EffectTechnique* Effect::GetTechniquePtr() const
{
	return m_pTechnique;
}

ID3D11InputLayout* Effect::GetInputLayoutPtr() const
{
	return m_pInputLayout;
}

ID3DX11Effect* Effect::LoadEffect( ID3D11Device* pDevice, const std::wstring& assetFile )
{
	HRESULT result{};
	ID3D10Blob* pErrorBlob{};
	ID3DX11Effect* pEffect{};

	DWORD shaderFlags{};
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	result = D3DX11CompileEffectFromFile(
		assetFile.c_str(), nullptr, nullptr, shaderFlags, 0, pDevice, &pEffect, &pErrorBlob );

	if ( FAILED( result ) )
	{
		if ( pErrorBlob )
		{
			const char* pErrors{ reinterpret_cast<char*>( pErrorBlob->GetBufferPointer() ) };

			std::wstringstream stringStream{};
			for ( size_t index{}; index < pErrorBlob->GetBufferSize(); ++index )
			{
				stringStream << pErrors[index];
			}

			OutputDebugStringW( stringStream.str().c_str() );
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << stringStream.str() << "\n";

			return nullptr;
		}
		else
		{
			std::wcout << L"EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile << "\n";
			return nullptr;
		}
	}

	return pEffect;
}
