#include "TransformCbufPixel.h"

//namespace Hydro::gfx::Bind
//{
//	
//	TransformCbufPixel::TransformCbufPixel( Graphics& gfx, const Drawable& parent, UINT slotV, UINT slotP )
//		:
//		TransformCbuf( gfx, parent, slotV )
//	{
//		if( !pPcbuf )
//		{
//			pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>( gfx, slotP );
//		}
//	}
//
//	void TransformCbufPixel::Bind( Graphics& gfx ) noexcept
//	{
//		const auto tf = GetTransforms( gfx );
//		TransformCbuf::UpdateBindImpl( gfx, tf );
//		UpdateBindImpl( gfx, tf );
//	}
//
//	void TransformCbufPixel::UpdateBindImpl( Graphics& gfx, const Transforms& tf ) noexcept
//	{
//		pPcbuf->Update( gfx, tf );
//		pPcbuf->Bind( gfx );
//	}
//
//	std::unique_ptr<PixelConstantBuffer<TransformCbuf::Transforms>> TransformCbufPixel::pPcbuf;
//
//}
