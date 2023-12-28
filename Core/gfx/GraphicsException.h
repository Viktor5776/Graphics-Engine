#pragma once
#include "../utility/HydroException.h"
#include "../win/HydroWin.h"

namespace Hydro::gfx
{
	class Exception : public utility::HydroException
	{
		using HydroException::HydroException;
	};
	class HrException : public utility::HydroException
	{
	public:
		HrException( int line, const char* file, HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw HrException( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException( __LINE__,__FILE__,(hr) )
}