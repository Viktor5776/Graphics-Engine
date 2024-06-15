#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"
#include "../Bindable/BindableCommon.h"
#include "../Bindable/BindableCodex.h"

namespace Hydro::gfx
{
	using namespace Bind;

	void Drawable::Submit( FrameCommander& frame ) const noexcept
	{
		for( const auto& tech : techniques )
		{
			tech.Submit( frame, *this );
		}
	}

	void Drawable::AddTechnique( Technique tech_in ) noexcept
	{
		tech_in.InitializeParentReferences( *this );
		techniques.push_back( std::move( tech_in ) );
	}

	void Drawable::Bind( Graphics& gfx ) const noexcept
	{
		pTopology->Bind( gfx );
		pIndices->Bind( gfx );
		pVertices->Bind( gfx );
	}

	UINT Drawable::GetIndexCount() const noexcept(!_DEBUG)
	{
		return pIndices->GetCount();
	}

	Drawable::~Drawable()
	{}
}