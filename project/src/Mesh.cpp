#include "Mesh.h"
#include "Effect.h"
#include "Error.h"

namespace dae
{
Mesh::Mesh( ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices )
	: m_Effect( pDevice, L"./resources/PosCol3D.fx" )
{
	if ( vertices.size() == 0 )
	{
		throw error::mesh::BufferIsEmpty();
	}

	if ( indices.size() == 0 )
	{
		throw error::mesh::BufferIsEmpty();
	}
	m_VertexCount = vertices.size();
	m_IndexCount = indices.size();

	// Create Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData{};
	vertexData.pSysMem = vertices.data();

	HRESULT result{ pDevice->CreateBuffer( &vertexBufferDesc, &vertexData, &m_pVertexBuffer ) };
	if ( FAILED( result ) )
	{
		throw static_cast<int>( result );
	}
	//

	// Create Index Buffer
	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof( UINT ) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = indices.data();

	result = pDevice->CreateBuffer( &indexBufferDesc, &indexData, &m_pIndexBuffer );
	if ( FAILED( result ) )
	{
		throw static_cast<int>( result );
	}
	//
}

Mesh::Mesh( Mesh&& rhs )
{
	m_VertexCount = rhs.m_VertexCount;
	m_IndexCount = rhs.m_IndexCount;

	m_pVertexBuffer = rhs.m_pVertexBuffer;
	rhs.m_pVertexBuffer = nullptr;

	m_pIndexBuffer = rhs.m_pIndexBuffer;
	rhs.m_pIndexBuffer = nullptr;

	m_Effect = std::move( rhs.m_Effect );
}

Mesh& Mesh::operator=( Mesh&& rhs )
{
	m_VertexCount = rhs.m_VertexCount;
	m_IndexCount = rhs.m_IndexCount;

	m_pVertexBuffer = rhs.m_pVertexBuffer;
	rhs.m_pVertexBuffer = nullptr;

	m_pIndexBuffer = rhs.m_pIndexBuffer;
	rhs.m_pIndexBuffer = nullptr;

	m_Effect = std::move( rhs.m_Effect );

	return *this;
}

Mesh::~Mesh() noexcept
{
	if ( m_pVertexBuffer )
	{
		m_pVertexBuffer->Release();
	}

	if ( m_pIndexBuffer )
	{
		m_pIndexBuffer->Release();
	}
}

void Mesh::SetWorldViewProjection( const Matrix& wvp )
{
	m_Effect.SetWorldViewProjection( m_WorldMatrix * wvp );
}

void Mesh::Draw( ID3D11DeviceContext* pDeviceContext ) const
{
	// 1. Set primitive topology
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// 2. Set input layout
	pDeviceContext->IASetInputLayout( m_Effect.GetInputLayoutPtr() );

	// 3. Set vertex buffer
	constexpr UINT stride{ sizeof( Vertex ) };
	constexpr UINT offset{};
	pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	// 4. Set index buffer
	pDeviceContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// 5. Draw
	D3DX11_TECHNIQUE_DESC techDesc;
	m_Effect.GetTechniquePtr()->GetDesc( &techDesc );
	for ( UINT passIdx{}; passIdx < techDesc.Passes; ++passIdx )
	{
		m_Effect.GetTechniquePtr()->GetPassByIndex( passIdx )->Apply( 0, pDeviceContext );
		pDeviceContext->DrawIndexed( m_IndexCount, 0, 0 );
	}
}

ID3D11Buffer* Mesh::GetVertexBufferPtr() const
{
	return m_pVertexBuffer;
}

ID3D11Buffer* Mesh::GetIndexBufferPtr() const
{
	return m_pIndexBuffer;
}

Effect* Mesh::GetEffectPtr()
{
	return &m_Effect;
}

uint32_t Mesh::GetVertexCount() const
{
	return m_VertexCount;
}

uint32_t Mesh::GetIndexCount() const
{
	return m_IndexCount;
}
} // namespace dae
