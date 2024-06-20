#pragma once

namespace Hydro::gfx
{
	class Graphics;

	namespace Bind
	{
		class BufferResource
		{
		public:
			virtual ~BufferResource() = default;
			virtual void BindAsBuffer( Graphics& ) noexcept(!_DEBUG) = 0;
			virtual void BindAsBuffer( Graphics&, BufferResource* ) noexcept(!_DEBUG) = 0;
			virtual void Clear( Graphics& ) noexcept(!_DEBUG) = 0;
		};
	}
}