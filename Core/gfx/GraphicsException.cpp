#include "GraphicsException.h"
#include <sstream>
#include <algorithm>

namespace Hydro::gfx
{
	

	HrException::HrException( int line, const char* file, HRESULT hr ) noexcept
		:
		HydroException( line, file ),
		hr( hr )
	{}

	const char* HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* HrException::GetType() const noexcept
	{
		return "Graphics Exception";
	}

	const char* DeviceRemovedException::GetType() const noexcept
	{
		return "Graphics Device Removed Exception";
	}

	std::string HrException::TranslateErrorCode( HRESULT hr ) noexcept
	{
		wchar_t* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
			reinterpret_cast<LPWSTR>(&pMsgBuf),
			0,
			nullptr
		);

		if( nMsgLen == 0 )
		{
			return "Unidentified error code";
		}

		std::wstring errorStringWide = pMsgBuf;
		LocalFree( pMsgBuf );
		std::string errorString;
		std::transform( errorStringWide.begin(), errorStringWide.end(), std::back_inserter( errorString ), []( wchar_t c ) {
			return (char)c;
		} );

		return errorString;
	}

	HRESULT HrException::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string HrException::GetErrorString() const noexcept
	{
		return TranslateErrorCode( hr );
	}
}
