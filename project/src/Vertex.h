#include "ColorRGB.h"
#include "Vector3.h"
#include "Vector4.h"

namespace hardware
{
struct VertexIn
{
	dae::Vector3 position{};
	dae::ColorRGB color{};
};

struct VertexOut
{
	dae::Vector4 position{};
	dae::ColorRGB color{};
};
} // namespace hardware
