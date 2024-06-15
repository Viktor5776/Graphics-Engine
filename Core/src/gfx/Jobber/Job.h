#pragma once

namespace Hydro::gfx
{

	class Job
	{
	public:
		Job( const class Step* pStep, const class Drawable* pDrawable );
		void Execute( class Graphics& gfx ) const noexcept(!_DEBUG);
	private:
		const class Drawable* pDrawable;
		const class Step* pStep;
	};

}