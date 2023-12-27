#include <Core/win/Window.h>
#include <format>

#include <locale>
#include <codecvt>
#include <string>

std::wstring CharToWString( const char* str )
{
	std::wstring wstr;
	while( *str != '\0' )
		wstr.push_back( *str++ );
	return wstr.c_str();
}



int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) 
{
	try
	{

		Hydro::win::Window window;

		std::optional<int> quiting = {};
		while( !quiting )
		{
			quiting = window.ProcessMessages();

			if( window.kbd.KeyIsPressed( 'A' ) )
			{
				std::wstring title;


				window.SetTitle( title );
			}
			//Do some cool game stuff here
		}

		return *quiting;
	}
	catch( const Hydro::win::WindowException& e )
	{
		MessageBox( nullptr, CharToWString( e.what() ).c_str(), CharToWString(e.GetType()).c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch( const std::exception& e )
	{
		MessageBox( nullptr, CharToWString( e.what() ).c_str(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION );
	}
	catch( ... )
	{
		MessageBox( nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION );
	}

	return -1;
}