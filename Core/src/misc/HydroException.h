#pragma once
#include <exception>
#include <string>

namespace Hydro::misc
{
	class HydroException : public std::exception
	{
	public:
		HydroException( int line, const char* file ) noexcept;
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;
	private:
		int line;
		std::string file;
	protected:
		mutable std::string whatBuffer;
	};
}