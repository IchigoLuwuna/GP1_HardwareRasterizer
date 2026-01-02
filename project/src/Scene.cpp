#include "Scene.h"
#include "Mesh.h"
#include "Error.h"
#include "Utils.h"

namespace dae
{
void Scene::Update( Timer* pTimer )
{
	m_Camera.Update( pTimer );

	for ( auto& mesh : m_Meshes )
	{
		mesh.SetWorldViewProjection( m_Camera.GetViewMatrix() * m_Camera.GetProjectionMatrix() );
	}
}

void Scene::Draw( ID3D11DeviceContext* pDeviceContext )
{
	if ( m_Meshes.empty() )
	{
		throw error::scene::SceneIsEmpty();
	}

	for ( auto& mesh : m_Meshes )
	{
		mesh.Draw( pDeviceContext );
	}
}

void Scene::Initialize( ID3D11Device* pDevice, float aspectRatio )
{
}

void CameraAndTexturesScene::Initialize( ID3D11Device* pDevice, float aspectRatio )
{
	m_Camera = Camera{ { 0.f, 0.f, -10.f }, 45.f, aspectRatio };

	const std::vector<Vertex> vertices{
		{ { 3.f, 3.f, 2.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f } },
		{ { 3.f, -3.f, 2.f }, { 0.f, 0.f, 1.f }, { 1.f, 1.f } },
		{ { -3.f, -3.f, 2.f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f } },
		{ { -3.f, 3.f, 2.f }, { 1.f, 0.f, 1.f }, { 0.f, 0.f } },
	};
	const std::vector<uint32_t> indices{ 0, 1, 3, 2 };
	const D3D11_PRIMITIVE_TOPOLOGY topology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP };
	const std::wstring effectPath{ L"./resources/PosUV3D.fx" };
	const std::string texturePath{ "./resources/uv_grid_2.png" };

	m_Meshes.push_back( Mesh{
		pDevice,
		vertices,
		indices,
		topology,
		effectPath,
		texturePath,
	} );
}

void VehicleScene::Initialize( ID3D11Device* pDevice, float aspectRatio )
{
	m_Camera = Camera{ { 0.f, 0.f, -64.f }, 45.f, aspectRatio };

	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	Utils::ParseOBJ( "./resources/vehicle.obj", vertices, indices );
	const D3D11_PRIMITIVE_TOPOLOGY topology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
	const std::wstring effectPath{ L"./resources/PosUV3D.fx" };
	const std::string texturePath{ "./resources/vehicle_diffuse.png" };

	m_Meshes.push_back( Mesh{
		pDevice,
		vertices,
		indices,
		topology,
		effectPath,
		texturePath,
	} );
}
} // namespace dae
