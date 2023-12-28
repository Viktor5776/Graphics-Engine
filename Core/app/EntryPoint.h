#pragma once
#include <format>
#include <locale>
#include <codecvt>
#include <string>
#include "App.h"

extern Hydro::app::App* Hydro::app::CreateApplication();

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
		auto app = Hydro::app::CreateApplication();
		return app->Go();
	}
	catch( const Hydro::win::WindowException& e )
	{
		MessageBox( nullptr, CharToWString( e.what() ).c_str(), CharToWString( e.GetType() ).c_str(), MB_OK | MB_ICONEXCLAMATION );
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