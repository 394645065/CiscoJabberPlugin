// hid_native_driver.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define WND_CLASS_NAME TEXT("JabberPluginMsgClass")
#define WND_WINDOW_NAME TEXT("JabberPluginMsgWindow")


HDEVNOTIFY hDeviceNotify;
HWND hJabberMsgWnd = NULL;
HWND hSndWnd = NULL;



void SendEventToIntegration(int event)
{
	COPYDATASTRUCT cds;

	if(hSndWnd != NULL)
	{
		cds.cbData = event;
		cds.lpData = NULL;

		SendMessage(hSndWnd, WM_COPYDATA, (WPARAM)hJabberMsgWnd, (LPARAM)&cds);
	}
}


INT_PTR WINAPI WinProcCallback(
                              HWND hWnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam
                              )
{
    LRESULT lRet = 1;
    static HWND hEditWnd;
	COPYDATASTRUCT* cds;
	COPYDATASTRUCT snd_cds;

    switch (message)
    {
		case WM_COPYDATA:

			if(hSndWnd == NULL)
			{
				hSndWnd = (HWND)wParam;
			}

			cds = (COPYDATASTRUCT*)lParam;

			switch (cds -> cbData)
			{
			case HAND_SHAKE:
				/* hand shake with Jabra integration*/
				hSndWnd = (HWND)wParam;
				snd_cds.cbData = HAND_SHAKE;
				snd_cds.lpData = NULL;
				SendMessage(hSndWnd, WM_COPYDATA, (WPARAM)hJabberMsgWnd, (LPARAM)&snd_cds);
				break;

			default:
				break;
			}


			break;

		case WM_SETFOCUS: 
			SetFocus(hEditWnd); 

			break;

		
		case WM_CLOSE:		

			if(hSndWnd != NULL)
			{
				snd_cds.cbData = BREAK_UP;
				snd_cds.lpData = NULL;
				SendMessage(hSndWnd, WM_COPYDATA, (WPARAM)hJabberMsgWnd, (LPARAM)&snd_cds);
				hSndWnd = NULL;
			}

			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			// Send all other messages on to the default windows handler.
			lRet = DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}

	return lRet;
}




void MessagePump(
    HWND hWnd
)
{
    MSG msg; 
    int retVal;

    // Get all messages for any window that belongs to this thread,
    // without any filtering. Potential optimization could be
    // obtained via use of filter values if desired.

    while( (retVal = GetMessage(&msg, NULL, 0, 0)) != 0 ) 
    { 
        if ( retVal == -1 )
        {
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } 
}


BOOL InitWindowClass()
{
    WNDCLASSEX wndClass;

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
    wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WinProcCallback);
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hIcon = LoadIcon(0,IDI_APPLICATION);
    wndClass.hbrBackground = CreateSolidBrush(RGB(192,192,192));
    wndClass.hCursor = LoadCursor(0, IDC_ARROW);
    wndClass.lpszClassName = WND_CLASS_NAME;
    wndClass.lpszMenuName = NULL;
    wndClass.hIconSm = wndClass.hIcon;


    if ( ! RegisterClassEx(&wndClass) )
    {
        return FALSE;
    }
    return TRUE;
}


DWORD WINAPI CommucationThreadProc(LPVOID lpParam)
{
    int nArgC = 0;

    if ( ! InitWindowClass() )
    {
        // InitWindowClass displays any errors
        return -1;
    }

    // Main app window

   hJabberMsgWnd = CreateWindowEx(
                    NULL,
                    WND_CLASS_NAME,
                    WND_WINDOW_NAME,
                    WS_OVERLAPPEDWINDOW, // style 
                    CW_USEDEFAULT, 0, 
                    0, 0,
                    NULL, 
					NULL, 
                    NULL, 
                    NULL);


	if ( hJabberMsgWnd == NULL )
    {
		ExitProcess(1);
    }


    MessagePump(hJabberMsgWnd);

	return true;
}



