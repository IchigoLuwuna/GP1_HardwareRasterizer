#ifndef MESH_H
#define MESH_H
#include <cstdint>
#include <vector>
#include <d3d11.h>
#include "Effect.h"
#include "Structs.h"

namespace dae
{
class Mesh
{
public:
	Mesh() = default;
	Mesh( ID3D11Device* pDevice,
		  const std::vector<Vertex>& vertices,
		  const std::vector<UINT>& indices,
		  D3D11_PRIMITIVE_TOPOLOGY topology,
		  const std::wstring& effectPath );
	Mesh( const Mesh& ) = delete;
	Mesh( Mesh&& rhs );

	Mesh& operator=( const Mesh& ) = delete;
	Mesh& operator=( Mesh&& rhs );

	~Mesh() noexcept;

	// Methods
	void Draw( ID3D11DeviceContext* pDeviceContext ) const;

	// Setters
	void SetWorldViewProjection( const Matrix& wvp );

	// Getters
	ID3D11Buffer* GetVertexBufferPtr() const;
	ID3D11Buffer* GetIndexBufferPtr() const;
	Effect* GetEffectPtr();
	uint32_t GetVertexCount() const;
	uint32_t GetIndexCount() const;

private:
	// SOFTWARE RESOURCES
	uint32_t m_VertexCount{};
	uint32_t m_IndexCount{};
	D3D11_PRIMITIVE_TOPOLOGY m_Topology{};
	Matrix m_WorldMatrix{ Matrix::CreateIdentity() };

	// HARDWARE RESOURCES: OWNING
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	Effect m_Effect{};
	//
};
}; // namespace dae
#endif
