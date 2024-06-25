#pragma once
#include "Bindable.h"
#include "../Vertex.h"

namespace Hydro::gfx::Bind
{
	class VertexShader;

	class InputLayout : public Bindable
	{
	public:
		InputLayout( Graphics& gfx, 
			VertexLayout layout,
			const VertexShader& vs );
		void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		const VertexLayout GetLayout() const noexcept;
		static std::shared_ptr<InputLayout> Resolve( Graphics& gfx, const VertexLayout& layout, const VertexShader& vs );
		static std::string GenerateUID( const VertexLayout& layout, const VertexShader& vs );
		std::string GetUID() const noexcept override;
	protected:
		std::string vertexShaderUID;
		VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}