#pragma once
#include "Base/Pass.h"

namespace Hydro::gfx
{

	namespace Bind
	{
		class BufferResource;
	}

	namespace Rgph
	{
		class BufferClearPass : public Pass
		{
		public:
			BufferClearPass( std::string name );
			void Execute( Graphics& gfx ) const noexcept(!_DEBUG) override;
		private:
			std::shared_ptr<Bind::BufferResource> buffer;
		};
	}

}