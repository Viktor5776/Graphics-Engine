#include <Core/win/Window.h>
#include <format>

#include <locale>
#include <codecvt>
#include <string>

int StringToWString( std::wstring& ws, const std::string& s )
{
	std::wstring wsTmp( s.begin(), s.end() );

	ws = wsTmp;

	return 0;
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) 
{
	Hydro::win::Window window;

	std::optional<int> quiting = {};
	while( !quiting )
	{
		quiting = window.ProcessMessages();

		if( window.kbd.KeyIsPressed( 'A' ) )
		{
			std::wstring title;
			StringToWString( title, std::format( "x: {} y: {}", window.mouse.GetPosX(), window.mouse.GetPosY() ) );
			

			window.SetTitle( title );
		}
		//Do some cool game stuff here
	}

	return *quiting;
}