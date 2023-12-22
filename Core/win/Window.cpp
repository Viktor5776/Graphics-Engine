#include "Window.h"

namespace Hydro::win
{
	Window::Window( int width, int height )
		:
		hInstance( GetModuleHandle( nullptr ) ),
		width( width ),
		height( height )
	{
		WNDCLASS wndClass = {};
		wndClass.lpszClassName = className;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
		wndClass.lpfnWndProc = HandleMsgSetup;

		RegisterClass( &wndClass );

		DWORD e = GetLastError();

		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		RECT rect = { 0, 0, width, height };
		AdjustWindowRect( &rect, style, FALSE );

		hWnd = CreateWindow( 
			className, 
			L"Hydro", 
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, 
			rect.bottom - rect.top,
			nullptr, 
			nullptr, 
			hInstance, 
			this
		);

		ShowWindow( hWnd, SW_SHOW );
	}

	Window::~Window()
	{
		UnregisterClass( className, hInstance );
		DestroyWindow( hWnd );
	}

	void Window::SetTitle( const std::wstring& title )
	{
		SetWindowText( hWnd, title.c_str() );
	}

	std::optional<int> Window::ProcessMessages()
	{
		MSG msg = {};

		while( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
			{
				return (int)msg.wParam;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		return {};
	}

	LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) 
	{
		//WM_NCCREATE msg Gets Called When We Use CreateWindow()
		if( msg == WM_NCCREATE )
		{
			//Get ptr To Window Class
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			//Set WinAPI to store ptr to Window Class
			SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd) );
			//Set Message Proc to Normal (non-setup) Handler
			SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk) );
			//Forward message to Window Class Handler
			return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
		}
		//Messages before WM_NCCREATE Get Forwarded to Default Handler
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) 
	{
		//Retrive ptr to Window Class
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr( hWnd, GWLP_USERDATA ));
		//Forward message to Window Class Handler
		return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
	}

	LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) 
	{
		switch( msg )
		{
		case WM_CLOSE:
			PostQuitMessage( 0 );
			return 0;
			//Clear Keystate When Window Loses Focus
		case WM_KILLFOCUS:
			kbd.ClearState();
			break;
			/********** KEYBOARD MESSAGES **********/
		case WM_KEYDOWN:
			//SYSKEY Commands Need to be Handeld For ALT ( VM_MENU ) and F10
		case WM_SYSKEYDOWN:
			//Filter Auto Repeat
			if( !(lParam & 0x40000000) || kbd.AutorepeatIsEnabled() )
			{
				kbd.OnKeyPressed( static_cast<unsigned char>(wParam) );
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			kbd.OnKeyReleased( static_cast<unsigned char>(wParam) );
			break;
		case WM_CHAR:
			kbd.OnChar( static_cast<unsigned char>(wParam) );
			break;
			/******** END KEYBOARD MESSAGES ********/

			/********** Mouse MESSAGES **********/
		case WM_MOUSEMOVE:
			if( GetFocus() == hWnd )
			{
				const POINTS pt = MAKEPOINTS( lParam );
				//In Window
				if( pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height )
				{
					mouse.OnMouseMove( pt.x, pt.y );
					if( !mouse.IsInWindow() )
					{
						SetCapture( hWnd );
						mouse.OnMouseEnter();
					}
				}
				//Not in Window ( Maintain Capture if Button is Down )
				else
				{
					if( wParam & (MK_LBUTTON | MK_RBUTTON) )
					{
						mouse.OnMouseMove( pt.x, pt.y );
					}
					//Button Up ( Release Capture )
					else
					{
						ReleaseCapture();
						mouse.OnMouseLeave();

					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			mouse.OnLeftPressed();
			break;
		case WM_LBUTTONUP:
			mouse.OnLeftRelease();
			break;
		case WM_RBUTTONDOWN:
			mouse.OnRightPressed();
			break;
		case WM_RBUTTONUP:
			mouse.OnRightRelease();
			break;
		case WM_MOUSEWHEEL:
			mouse.OnWheelDelta( GET_WHEEL_DELTA_WPARAM( wParam ) );
			break;
		case WM_MBUTTONDOWN:
			mouse.OnWheelPressed();
			break;
		}

		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	
}