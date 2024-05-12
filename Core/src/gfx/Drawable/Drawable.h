#pragma once
#include "../Graphics.h"
#include <DirectXMath.h>
#include <memory>

namespace Hydro::gfx
{

	namespace Bind
	{
		class Bindable;
		class IndexBuffer;
	}

	class Drawable
	{
	public:
		Drawable() = default;
		Drawable( const Drawable& ) = delete;
		virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
		void Draw( Graphics& gfx ) const noexcept(!_DEBUG);
		virtual ~Drawable() = default;
		template<class T>
		T* QueryBindable() noexcept
		{
			for( const auto& pb : binds )
			{
				if( const T* pt = dynamic_cast<T*>(pb.get()) )
				{
					return const_cast<T*>( pt );
				}
			}
			return nullptr;
		}
	protected:
		void AddBind( std::shared_ptr<Bind::Bindable> bind ) noexcept(!_DEBUG);
	private:
		const class Bind::IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::shared_ptr<Bind::Bindable>> binds;
	};

}