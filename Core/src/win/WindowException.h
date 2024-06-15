#pragma once
#include "HydroWin.h"
#include "../misc/HydroException.h"

namespace Hydro::win
{
	class WindowException : public misc::HydroException
	{
	public:
		WindowException( int line, const char* file, HRESULT hr ) noexcept;
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

#define WIN_EXCEPT( hr) WindowException( __LINE__, __FILE__, hr );
}