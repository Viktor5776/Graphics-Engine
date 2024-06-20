#pragma once
#include "TransformCbuf.h"
#include "../DynamicConstant.h"

namespace Hydro::gfx::Bind
{
	class TransformCbufScaling : public TransformCbuf
	{
	public:
		TransformCbufScaling( Graphics& gfx, float scale );
		void Accept( TechniqueProbe& probe ) override;
		void Bind( Graphics& gfx ) noexcept(!_DEBUG) override;
		std::unique_ptr<CloningBindable> Clone() const noexcept override;
	private:
		static Dcb::RawLayout MakeLayout();
	private:
		Dcb::Buffer buf;
	};
}