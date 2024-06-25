#include "InputLayout.h"
#include "BindableCodex.h"
#include "VertexShader.h"

namespace Hydro::gfx::Bind
{
	InputLayout::InputLayout( Graphics& gfx, VertexLayout layout_in, const VertexShader& vs )
		:
		layout( std::move( layout_in ) ),
		vertexShaderUID( vs.GetUID() )
	{
		HRESULT hr;

		const auto d3dLayout = layout.GetD3DLayout();
		const auto pBytecode = vs.GetBytecode();

		GFX_THROW_FAILED( GetDevice( gfx )->CreateInputLayout(
			d3dLayout.data(),
			(UINT)d3dLayout.size(),
			pBytecode->GetBufferPointer(),
			pBytecode->GetBufferSize(),
			&pInputLayout
		) );
	}

	void InputLayout::Bind( Graphics& gfx ) noexcept(!_DEBUG)
	{
		GetContext( gfx )->IASetInputLayout( pInputLayout.Get() );
	}

	const VertexLayout InputLayout::GetLayout() const noexcept
	{
		return layout;
	}

	std::shared_ptr<InputLayout> InputLayout::Resolve( Graphics& gfx,
		const VertexLayout& layout, const VertexShader& vs )
	{
		return Codex::Resolve<InputLayout>( gfx, layout, vs );
	}

	std::string InputLayout::GenerateUID( const VertexLayout& layout, const VertexShader& vs )
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode() + "#"s + vs.GetUID();
	}

	std::string InputLayout::GetUID() const noexcept
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode() + "#"s + vertexShaderUID;
	}
}
