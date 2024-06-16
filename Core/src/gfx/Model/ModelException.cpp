#include "ModelException.h"
#include <sstream>

namespace Hydro::gfx
{

	ModelException::ModelException( int line, const char* file, std::string note ) noexcept
		:
		HydroException( line, file ),
		note( std::move( note ) )
	{}

	const char* ModelException::what() const noexcept
	{
		std::ostringstream oss;
		oss << HydroException::what() << std::endl
			<< "[Note] " << GetNote();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* ModelException::GetType() const noexcept
	{
		return "Hydro Model Exception";
	}

	const std::string& ModelException::GetNote() const noexcept
	{
		return note;
	}

}