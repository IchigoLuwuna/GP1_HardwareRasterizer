#include "Mesh.h"
#include "Error.h"

namespace dae
{
Mesh::Mesh( ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices )
	: m_pDevice{ pDevice }
{
	if ( vertices.size() == 0 )
	{
		throw error::mesh::BufferIsEmpty();
	}

	if ( indices.size() == 0 )
	{
		throw error::mesh::BufferIsEmpty();
	}

	// Create Vertex Buffer
	m_VertexCount = vertices.size();
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof( Vertex ) * m_VertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof( Vertex );

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices.data();

	HRESULT result{ pDevice->CreateBuffer( &bufferDesc, &initData, &m_pVertexBuffer ) };
	if ( FAILED( result ) )
	{
		throw static_cast<int>( result );
	}
	//

	// Create Index Buffer
	m_IndexCount = indices.size();
	bufferDesc.ByteWidth = sizeof( uint32_t ) * m_IndexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof( uint32_t );

	initData.pSysMem = indices.data();

	result = pDevice->CreateBuffer( &bufferDesc, &initData, &m_pIndexBuffer );
	if ( FAILED( result ) )
	{
		throw static_cast<int>( result );
	}
	//
}

Mesh::~Mesh()
{
	if ( m_pVertexBuffer )
	{
		m_pVertexBuffer->Release();
	}
}
} // namespace dae
