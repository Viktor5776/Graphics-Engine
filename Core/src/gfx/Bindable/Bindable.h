#pragma once
#include "../Graphics.h"
#include <string>

namespace Hydro::gfx
{
	class Drawable;
	class TechniqueProbe;
}

namespace Hydro::gfx::Bind
{
	class Bindable
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
	protected:
		static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept;
		static ID3D11Device* GetDevice( Graphics& gfx ) noexcept;
	};
}