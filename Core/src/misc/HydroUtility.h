#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>


inline std::vector<std::string> TokenizeQuoted( const std::string& input )
{
	std::istringstream stream;
	stream.str( input );
	std::vector<std::string> tokens;
	std::string token;

	while( stream >> std::quoted( token ) )
	{
		tokens.push_back( std::move( token ) );
	}
	return tokens;
}

inline std::wstring ToWide( const std::string& narrow )
{
	wchar_t wide[512];
	mbstowcs_s( nullptr, wide, narrow.c_str(), _TRUNCATE );
	return wide;
}

inline std::string ToNarrow( const std::wstring& wide )
{
	char narrow[512];
	wcstombs_s( nullptr, narrow, wide.c_str(), _TRUNCATE );
	return narrow;
}

template<class Iter>
inline void SplitStringIter( const std::string& s, const std::string& delim, Iter out )
{
	if( delim.empty() )
	{
		*out++ = s;
	}
	else
	{
		size_t a = 0, b = s.find( delim );
		for( ; b != std::string::npos;
			a = b + delim.length(), b = s.find( delim, a ) )
		{
			*out++ = std::move( s.substr( a, b - a ) );
		}
		*out++ = std::move( s.substr( a, s.length() - a ) );
	}
}

inline std::vector<std::string> SplitString( const std::string& s, const std::string& delim )
{
	std::vector<std::string> strings;
	SplitStringIter( s, delim, std::back_inserter( strings ) );
	return strings;
}
