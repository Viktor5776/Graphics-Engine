#include "InputLayout.h"
#include "BindableCodex.h"

namespace Hydro::gfx::Bind
{
	InputLayout::InputLayout( Graphics& gfx, VertexLayout layout_in, ID3DBlob* pVertexShaderBytecode )
		:
		layout( std::move( layout_in ) )
	{
		HRESULT hr;

		const auto d3dLayout = layout.GetD3DLayout();

		GFX_THROW_FAILED( GetDevice( gfx )->CreateInputLayout(
			d3dLayout.data(),
			(UINT)d3dLayout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&pInputLayout
		) );
	}

	void InputLayout::Bind( Graphics& gfx ) noexcept
	{
		GetContext( gfx )->IASetInputLayout( pInputLayout.Get() );
	}

	std::shared_ptr<InputLayout> InputLayout::Resolve( Graphics& gfx,
		const VertexLayout& layout, ID3DBlob* pVertexShaderBytecode )
	{
		return Codex::Resolve<InputLayout>( gfx, layout, pVertexShaderBytecode );
	}

	std::string InputLayout::GenerateUID( const VertexLayout& layout, ID3DBlob* pVertexShaderBytecode )
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode();
	}

	std::string InputLayout::GetUID() const noexcept
	{
		return GenerateUID( layout );
	}
}
