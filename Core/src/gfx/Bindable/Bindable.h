#pragma once
#include "../Graphics.h"
#include "../GraphicsResource/GraphicsResource.h"
#include <memory>
#include <string>

namespace Hydro::gfx
{
	class Drawable;
	class TechniqueProbe;
}

namespace Hydro::gfx::Bind
{
	class Bindable : public GraphicsResource
	{
	public:
		virtual void Bind( Graphics& gfx ) noexcept = 0;
		virtual void InitializeParentReference( const Drawable& ) noexcept
		{}
		virtual void Accept( TechniqueProbe& )
		{}
		virtual std::string GetUID() const noexcept
		{
			assert( false );
			return "";
		}
		virtual ~Bindable() = default;
	};

	class CloningBindable : public Bindable
	{
	public:
		virtual std::unique_ptr<CloningBindable> Clone() const noexcept = 0;
	};
}