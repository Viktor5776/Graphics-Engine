#include "Mesh.h"
#include "../../../third/ImGui/imgui.h"
#include "../../misc/Surface.h"
#include <unordered_map>
#include <sstream>
#include <filesystem>
#include "../../misc/HydroXM.h"
#include "../DynamicConstant.h"
#include "../Bindable/ConstantBuffersEx.h"
#include "../LayoutCodex.h"
#include "../Bindable/Stencil.h"

namespace Hydro::gfx
{

	Mesh::Mesh( Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale ) noexcept(!_DEBUG)
		:
		Drawable( gfx, mat, mesh, scale )
	{}

	void Mesh::Submit( FrameCommander& frame, DirectX::FXMMATRIX accumulatedTranform ) const noexcept(!_DEBUG)
	{
		DirectX::XMStoreFloat4x4( &transform, accumulatedTranform );
		Drawable::Submit( frame );
	}

	DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
	{
		return DirectX::XMLoadFloat4x4( &transform );
	}

}