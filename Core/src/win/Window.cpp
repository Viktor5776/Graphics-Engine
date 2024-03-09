#include "Window.h"
#include "../../third/ImGui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace Hydro::win
{

	Window::Window( int width, int height )
		:
		hInstance( GetModuleHandle( nullptr ) ),
		width( width ),
		height( height )
	{
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof( wndClass );
		wndClass.style = CS_OWNDC;
		wndClass.lpfnWndProc = HandleMsgSetup;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInstance;
		wndClass.hIcon = static_cast<HICON>(LoadImage( hInstance, MAKEINTRESOURCE( 101 ), IMAGE_ICON, 32, 32, 0 ));
		wndClass.hCursor = nullptr;
		wndClass.hbrBackground = nullptr;
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = className;
		wndClass.hIconSm = static_cast<HICON>(LoadImage( hInstance, MAKEINTRESOURCE( 101 ), IMAGE_ICON, 16, 16, 0 ));

		if( RegisterClassEx( &wndClass ) == 0 )
		{
			throw WIN_EXCEPT( GetLastError() );
		}

		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		RECT rect = { 0, 0, width, height };

		if( AdjustWindowRect( &rect, style, FALSE ) == 0 )
		{
			throw WIN_EXCEPT( GetLastError() );
		}

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

		if( hWnd == nullptr )
		{
			throw WIN_EXCEPT( GetLastError() );
		}

		ShowWindow( hWnd, SW_SHOW );

		pGfx = std::make_unique<gfx::Graphics>( hWnd, width, height );

		ImGui_ImplWin32_Init( hWnd );

		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = nullptr;
		if( RegisterRawInputDevices( &rid, 1, sizeof( rid ) ) == FALSE )
		{
			throw WIN_EXCEPT( GetLastError() );
		}
	}

	Window::~Window()
	{
		ImGui_ImplWin32_Shutdown();
		UnregisterClass( className, hInstance );
		DestroyWindow( hWnd );
	}

	void Window::SetTitle( const std::wstring& title )
	{
		SetWindowText( hWnd, title.c_str() );
	}

	void Window::EnableCursor() noexcept
	{
		cursorEnabled = true;
		ShowCursor();
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		FreeCursor();
	}

	void Window::DisableCursor() noexcept
	{
		cursorEnabled = false;
		HideCursor();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		ConfineCursor();
	}

	bool Window::CursorEnabled() const noexcept
	{
		return cursorEnabled;
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

	gfx::Graphics& Window::Gfx()
	{
		return *pGfx;
	}

	void Window::HideCursor() noexcept
	{
		while( ::ShowCursor( FALSE ) >= 0 );
	}

	void Window::ShowCursor() noexcept
	{
		while( ::ShowCursor( TRUE ) < 0 );
	}

	void Window::ConfineCursor() noexcept
	{
		RECT rect;
		GetClientRect( hWnd, &rect );
		MapWindowPoints( hWnd, nullptr, reinterpret_cast<POINT*>( &rect ), 2 );
		ClipCursor( &rect );
	}

	void Window::FreeCursor() noexcept
	{
		ClipCursor( nullptr );
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
		if( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
		{
			return true;
		}
		const auto& imio = ImGui::GetIO();


		switch( msg )
		{
		case WM_CLOSE:
			PostQuitMessage( 0 );
			return 0;
			//Clear Keystate When Window Loses Focus
		case WM_KILLFOCUS:
			kbd.ClearState();
			break;
		case WM_ACTIVATE:
			if( !cursorEnabled )
			{
				if( wParam & WA_ACTIVE )
				{
					ConfineCursor();
					HideCursor();
				}
				else
				{
					FreeCursor();
					ShowCursor();
				}
			}
			break;
			/********** KEYBOARD MESSAGES **********/
		case WM_KEYDOWN:
			//SYSKEY Commands Need to be Handeld For ALT ( VM_MENU ) and F10
		case WM_SYSKEYDOWN:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			//Filter Auto Repeat
			if( !(lParam & 0x40000000) || kbd.AutorepeatIsEnabled() )
			{
				kbd.OnKeyPressed( static_cast<unsigned char>(wParam) );
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			kbd.OnKeyReleased( static_cast<unsigned char>(wParam) );
			break;
		case WM_CHAR:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			kbd.OnChar( static_cast<unsigned char>(wParam) );
			break;
			/******** END KEYBOARD MESSAGES ********/

			/********** Mouse MESSAGES **********/
		case WM_MOUSEMOVE:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
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
			SetForegroundWindow( hWnd );
			if( !cursorEnabled )
			{
				ConfineCursor();
				HideCursor();
			}
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			mouse.OnLeftPressed();
			break;
		case WM_LBUTTONUP:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			mouse.OnLeftRelease();
			break;
		case WM_RBUTTONDOWN:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			mouse.OnRightPressed();
			break;
		case WM_RBUTTONUP:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			mouse.OnRightRelease();
			break;
		case WM_MOUSEWHEEL:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			mouse.OnWheelDelta( GET_WHEEL_DELTA_WPARAM( wParam ) );
			break;
		case WM_MBUTTONDOWN:
			//Remove Message if Imgui Wants to Capture
			if( imio.WantCaptureKeyboard )
			{
				break;
			}
			mouse.OnWheelPressed();
			break;
		/********** END Mouse MESSAGES **********/

		/********** RAW MOUSE MESSAGES **********/
		case WM_INPUT:
		{
			if( !mouse.RawEnabled() )
			{
				break;
			}
			UINT size = 0;
			
			if( GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				nullptr,
				&size,
				sizeof( RAWINPUTHEADER ) ) == -1 )
			{
				break;
			}
			rawBuffer.resize( size );
			if( GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				rawBuffer.data(),
				&size,
				sizeof( RAWINPUTHEADER ) ) != size )
			{
				break;
			}
			auto& ri = reinterpret_cast<const RAWINPUT&>( *rawBuffer.data() );
			if( ri.header.dwType == RIM_TYPEMOUSE &&
				(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0) )
			{
				mouse.OnRawDelta( ri.data.mouse.lLastX, ri.data.mouse.lLastY );
			}
			break;
		}
		}

		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	
}