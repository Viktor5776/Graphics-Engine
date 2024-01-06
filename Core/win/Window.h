#pragma once
#include <string>
#include <optional>
#include <memory>
#include "WindowException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "../gfx/Graphics.h"

namespace Hydro::win
{
	class Window
	{
	public:
		Window( int width = 1280, int height = 720 );
		Window( const Window& ) = delete;
		Window& operator=( const Window& ) = delete;
		~Window();
		void SetTitle( const std::wstring& title );
		std::optional<int> ProcessMessages();
		gfx::Graphics& Gfx();
	private:
		static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	public:
		Keyboard kbd;
		Mouse mouse;
		std::unique_ptr<gfx::Graphics> pGfx;
	private:
		HINSTANCE hInstance;
		HWND hWnd;
		int width = 0;
		int height = 0;
		static constexpr const wchar_t* className = L"Hydro Window";
	};

}