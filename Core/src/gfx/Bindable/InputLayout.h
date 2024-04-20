#pragma once
#include "Bindable.h"
#include "../Vertex.h"

namespace Hydro::gfx::Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout( Graphics& gfx, 
			VertexLayout layout,
			ID3DBlob* pVertexShaderBytecode );
		void Bind( Graphics& gfx ) noexcept override;
		static std::shared_ptr<InputLayout> Resolve( Graphics& gfx, const VertexLayout& layout, ID3DBlob* pVertexShaderBytecode );
		static std::string GenerateUID( const VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr );
		std::string GetUID() const noexcept override;
	protected:
		VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}