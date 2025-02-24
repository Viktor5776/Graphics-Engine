#include "HydroException.h"
#include <sstream>

namespace Hydro::misc
{

	HydroException::HydroException( int line, const char* file ) noexcept
		:
		line( line ),
		file( file )
	{}

	const char* HydroException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* HydroException::GetType() const noexcept
	{
		return "Hydro Exception";
	}

	int HydroException::GetLine() const noexcept
	{
		return line;
	}

	const std::string& HydroException::GetFile() const noexcept
	{
		return file;
	}

	std::string HydroException::GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << file << std::endl
			<< "[Line] " << line;
		return oss.str();
	}

}