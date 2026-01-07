#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "Mesh.h"

namespace dae
{
class Scene
{
public:
	Scene() = default;

	virtual void Update( Timer* pTimer );
	virtual void Draw( ID3D11DeviceContext* pDeviceContext );

	virtual void Initialize( ID3D11Device* pDevice, float aspectRatio ) = 0;

protected:
	Camera m_Camera{};
	std::vector<Mesh> m_Meshes{};
	Vector3 m_LightDir{};

	// TODO:Make this a bitmask
	bool m_F2Held{};
};

class CameraAndTexturesScene : public Scene
{
public:
	virtual void Initialize( ID3D11Device* pDevice, float aspectRatio ) override;
};

class VehicleScene : public Scene
{
public:
	virtual void Update( Timer* pTimer ) override;

	virtual void Initialize( ID3D11Device* pDevice, float aspectRatio ) override;
};
} // namespace dae

#endif
