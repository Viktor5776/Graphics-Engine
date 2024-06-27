#pragma once
#include "../Graphics.h"
#include "../Drawable/Drawable.h"

struct aiMesh;

namespace Hydro::gfx
{
	class Material;

	class Mesh : public Drawable
	{
	public:
		Mesh( Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale = 1.0f ) noexcept(!_DEBUG);
		DirectX::XMMATRIX GetTransformXM() const noexcept override;
		void Submit( size_t channels, DirectX::FXMMATRIX accumulatedTranform ) const noexcept(!_DEBUG);
	private:
		mutable DirectX::XMFLOAT4X4 transform;
	};
}