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
	Mesh( ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices );
	Mesh( const Mesh& ) = delete;
	Mesh( Mesh&& rhs );

	Mesh& operator=( const Mesh& ) = delete;
	Mesh& operator=( Mesh&& rhs );

	~Mesh();

	// Methods
	void Draw( ID3D11DeviceContext* pDeviceContext, Effect* effect ) const;

	// Getters
	ID3D11Buffer* GetVertexBufferPtr() const;
	ID3D11Buffer* GetIndexBufferPtr() const;
	uint32_t GetVertexCount() const;
	uint32_t GetIndexCount() const;

private:
	// SOFTWARE RESOURCES
	uint32_t m_VertexCount{};
	uint32_t m_IndexCount{};

	// HARDWARE RESOURCES: OWNING
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	//

	// HARDWARE RESOURCES: NON-OWNING
	ID3D11Device* m_pDevice{};
	//
};
}; // namespace dae
#endif
