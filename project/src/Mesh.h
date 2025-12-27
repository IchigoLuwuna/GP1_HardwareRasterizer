#ifndef MESH_H
#define MESH_H
#include <cstdint>
#include <vector>
#include <d3d11.h>
#include "Structs.h"

namespace dae
{
class Mesh
{
public:
	Mesh( ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices );
	~Mesh();

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
