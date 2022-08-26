#include "WindowsMessageProcessing.h"
#include "Component/Input/Input.h"
#include <windowsx.h>


LRESULT CALLBACK EngineWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_RBUTTONDOWN:
		MouseDownDelegate.Broadcast(EMouseState::RButton,GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONUP:
		MouseUpDelegate.Broadcast(EMouseState::RButton, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDOWN:
		MouseDownDelegate.Broadcast(EMouseState::LButton, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
		MouseUpDelegate.Broadcast(EMouseState::LButton, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MBUTTONDOWN:
		MouseDownDelegate.Broadcast(EMouseState::MButton, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MBUTTONUP:
		MouseUpDelegate.Broadcast(EMouseState::MButton, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEMOVE:
		MouseMoveDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEWHEEL:
		MousesWheelsDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (short)HIWORD(wParam));
		break; 
	case WM_KEYDOWN:
		{
			FInputKey InputKey(wParam);
			InputKey.PressState=EPressState::Press;
			KeyDownDelegate.Broadcast(std::move(InputKey));
		}

		//VK_ESCAPE;
		break;
	case WM_KEYUP:
	{
		FInputKey InputKey(wParam);
		InputKey.PressState = EPressState::Release;
		KeyUpDelegate.Broadcast(std::move(InputKey));
	}
		break;
	}
	//交给windows自己处理完成
	return DefWindowProc(hwnd, msg, wParam, lParam);
}