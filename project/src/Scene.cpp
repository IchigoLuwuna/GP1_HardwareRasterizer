#include "Scene.h"
#include "Mesh.h"
#include "Error.h"

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

void BasicTriangleScene::Initialize( ID3D11Device* pDevice, float aspectRatio )
{
	m_Camera = Camera{ { 0.f, 0.f, -1.f }, 45.f, aspectRatio };

	std::vector<Vertex> vertices{
		{ { 0.0f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f } },
		{ { 0.5f, -0.5f, 0.5f }, { 0.f, 0.f, 1.f } },
		{ { -0.5f, -0.5f, 0.5f }, { 0.f, 1.f, 0.f } },
	};
	std::vector<uint32_t> indices{ 0, 1, 2 };

	m_Meshes.push_back( Mesh( pDevice, vertices, indices ) );
}

void CameraAndTexturesScene::Initialize( ID3D11Device* pDevice, float aspectRatio )
{
	m_Camera = Camera{ { 0.f, 0.f, -10.f }, 45.f, aspectRatio };

	std::vector<Vertex> vertices{
		{
			{
				0.f,
				3.f,
				2.f,
			},
			{ 1.f, 0.f, 0.f },
		},
		{
			{
				3.f,
				-3.f,
				2.f,
			},
			{ 0.f, 0.f, 1.f },
		},
		{
			{
				-3.f,
				-3.f,
				2.f,
			},
			{ 0.f, 1.f, 0.f },
		},
	};
	std::vector<uint32_t> indices{ 0, 1, 2 };

	m_Meshes.push_back( Mesh( pDevice, vertices, indices ) );
}
} // namespace dae
