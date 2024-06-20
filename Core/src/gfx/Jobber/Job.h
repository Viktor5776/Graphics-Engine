#pragma once

namespace Hydro::gfx
{
	class Drawable;
	class Graphics;
	class Step;

	namespace Rgph
	{
		class Job
		{
		public:
			Job( const Step* pStep, const Drawable* pDrawable );
			void Execute( Graphics& gfx ) const noexcept(!_DEBUG);
		private:
			const Drawable* pDrawable;
			const Step* pStep;
		};
	}
}