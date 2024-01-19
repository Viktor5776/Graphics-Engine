#define FULL_WINTARD
#include "Surface.h"
#include <algorithm>
namespace Gdiplus
{
		using std::min;
		using std::max;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment( lib,"Gdiplus.lib" )


Hydro::utility::Surface::Surface( unsigned int width, unsigned int height, unsigned int pitch ) noexcept
	:
	pBuffer( std::make_unique<Color[]>( ((size_t)pitch) * height ) ),
	width( width ),
	height( height )
{}

Hydro::utility::Surface& Hydro::utility::Surface::operator=( Surface&& donor ) noexcept
{
	width = donor.width;
	height = donor.height;
	pBuffer = std::move( donor.pBuffer );
	donor.pBuffer = nullptr;
	return *this;
}

Hydro::utility::Surface::Surface( unsigned int width, unsigned int height ) noexcept
	:
	Surface( width, height, width )
{}

Hydro::utility::Surface::Surface( Surface&& source ) noexcept
	:
	pBuffer( std::move( source.pBuffer ) ),
	width( source.width ),
	height( source.height )
{}

Hydro::utility::Surface::~Surface()
{}

void Hydro::utility::Surface::Clear( Color fillValue ) noexcept
{
	memset( pBuffer.get(), fillValue.dword, ((size_t)width) * height * sizeof( Color ) );
}

void Hydro::utility::Surface::PutPixel( unsigned int x, unsigned int y, Color c ) noexcept(!_DEBUG)
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < width );
	assert( y < height );
	pBuffer[((size_t)y) * width + x] = c;
}

Hydro::utility::Surface::Color Hydro::utility::Surface::GetPixel( unsigned int x, unsigned int y ) const noexcept(!_DEBUG)
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < width );
	assert( y < height );
	return pBuffer[((size_t)y) * width + x];
}

unsigned int Hydro::utility::Surface::GetWidth() const noexcept
{
	return width;
}

unsigned int Hydro::utility::Surface::GetHeight() const noexcept
{
	return height;
}

Hydro::utility::Surface::Color* Hydro::utility::Surface::GetBufferPtr() noexcept
{
	return pBuffer.get();
}

const Hydro::utility::Surface::Color* Hydro::utility::Surface::GetBufferPtr() const noexcept
{
	return pBuffer.get();
}

const Hydro::utility::Surface::Color* Hydro::utility::Surface::GetBufferPtrConst() const noexcept
{
	return pBuffer.get();
}

Hydro::utility::Surface Hydro::utility::Surface::FromFile( const std::string& name )
{
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int pitch = 0;
	std::unique_ptr<Color[]> pBuffer = nullptr;

	{
		// convert filenam to wide string (for Gdiplus)
		wchar_t wideName[512];
		mbstowcs_s( nullptr, wideName, name.c_str(), _TRUNCATE );
		
		Gdiplus::Bitmap bitmap( wideName );
		if( bitmap.GetLastStatus() != Gdiplus::Status::Ok )
		{
			std::stringstream ss;
			ss << "Loading image [" << name << "]: failed to load.";
			throw Exception( __LINE__, __FILE__, ss.str() );
		}

		height = bitmap.GetHeight();
		width = bitmap.GetWidth();
		pitch = width;
		pBuffer = std::make_unique<Color[]>( ((size_t)width) * height );

		for( unsigned int y = 0; y < height; y++ )
		{
			for( unsigned int x = 0; x < width; x++ )
			{
				Gdiplus::Color c;
				bitmap.GetPixel( x, y, &c );
				pBuffer[((size_t)y) * pitch + x] = c.GetValue();
			}
		}
	}

	return Surface( width, height, std::move( pBuffer ) );
}

void Hydro::utility::Surface::Save( const std::string& filename ) const
{
	auto GetEncoderClsid = [&filename]( const WCHAR* format, CLSID* pClsid ) -> void
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize( &num, &size );
		if( size == 0 )
		{
			std::stringstream ss;
			ss << "Saving surface to [" << filename << "]: failed to get encoder; size == 0.";
			throw Exception( __LINE__, __FILE__, ss.str() );
		}

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc( size ));
		if( pImageCodecInfo == nullptr )
		{
			std::stringstream ss;
			ss << "Saving surface to [" << filename << "]: failed to get encoder; failed to allocate memory.";
			throw Exception( __LINE__, __FILE__, ss.str() );
		}

		GetImageEncoders( num, size, pImageCodecInfo );

#pragma warning( push )
#pragma warning( disable : 6385 )

		for( UINT j = 0; j < num; ++j )
		{
			if( wcscmp( pImageCodecInfo[j].MimeType, format ) == 0 )
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free( pImageCodecInfo );
				return;
			}
		}
#pragma warning( pop )

		free( pImageCodecInfo );
		std::stringstream ss;
		ss << "Saving surface to [" << filename <<
			"]: failed to get encoder; failed to find matching encoder.";
		throw Exception( __LINE__, __FILE__, ss.str() );
	};

	CLSID bmpID;
	GetEncoderClsid( L"image/bmp", &bmpID );


	// convert filenam to wide string (for Gdiplus)
	wchar_t wideName[512];
	mbstowcs_s( nullptr, wideName, filename.c_str(), _TRUNCATE );

	Gdiplus::Bitmap bitmap( width, height, width * sizeof( Color ), PixelFormat32bppARGB, (BYTE*)pBuffer.get() );
	if( bitmap.Save( wideName, &bmpID, nullptr ) != Gdiplus::Status::Ok )
	{
		std::stringstream ss;
		ss << "Saving surface to [" << filename << "]: failed to save.";
		throw Exception( __LINE__, __FILE__, ss.str() );
	}
}

void Hydro::utility::Surface::Copy( const Surface& src ) noexcept(!_DEBUG)
{
	assert( width == src.width );
	assert( height == src.height );
	memcpy( pBuffer.get(), src.pBuffer.get(), ((size_t)width) * height * sizeof( Color ) );
}

Hydro::utility::Surface::Surface( unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam ) noexcept
	:
	width( width ),
	height( height ),
	pBuffer( std::move( pBufferParam ) )
{}


// surface exception stuff
Hydro::utility::Surface::Exception::Exception( int line, const char* file, std::string note ) noexcept
	:
	HydroException( line, file ),
	note( std::move( note ) )
{}

const char* Hydro::utility::Surface::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << HydroException::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Hydro::utility::Surface::Exception::GetType() const noexcept
{
	return "Hydro Graphics Exception";
}

const std::string& Hydro::utility::Surface::Exception::GetNote() const noexcept
{
	return note;
}



