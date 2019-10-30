/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#include "wincubes_gui.h"
#include "wincubes_renderer.hpp"

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				DisableAndEnableMenuItems(HWND hWnd, UINT uintItemToDisable, UINT uintItemToEnable);
void				SetMotionDelay(HWND hWnd, WORD wNumMenuItem);

// Globals.
#define MAX_LOADSTRING 100
struct {
	WCHAR szTitle[MAX_LOADSTRING] { 0 };                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING] { 0 };            // the main window class name
	WCHAR szNotImplemented[MAX_LOADSTRING] { 0 };         // Literal string: Not implemented.
	std::unique_ptr<WinCubesRenderer> pRenderer;
} appGlobals;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, appGlobals.szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINCUBES, appGlobals.szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_IMPLEMENTED, appGlobals.szNotImplemented, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // Perform application initialization:
	HWND hWnd = CreateWindowW(appGlobals.szWindowClass, appGlobals.szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
		return 0;

	// Have renderer initialised before creating the window
	// to make it ready to execute the set_size member function
	// of the geometry object.
	appGlobals.pRenderer = std::make_unique<WinCubesRenderer>(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINCUBES));
    MSG msg;
	// Main message loop.
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
			appGlobals.pRenderer->background_job();
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINCUBES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
 	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINCUBES);
    wcex.lpszClassName  = appGlobals.szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void DisableAndEnableMenuItems(HWND hWnd, UINT uintItemToDisable, UINT uintItemToEnable)
{
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, uintItemToDisable, MF_GRAYED);
	EnableMenuItem(hMenu, uintItemToEnable, MF_ENABLED);
	DrawMenuBar(hWnd);
}

void SetMotionDelay(HWND hWnd, WORD wNumMenuItem)
{
	// Set the respective delay.
	DWORD dwDelays[] {0, 1, 5, 10, 15, 20, 30, 40, 50};
	appGlobals.pRenderer->setDelayMS(dwDelays[wNumMenuItem]);

	HMENU hMenu = GetMenu(hWnd);
	// Uncheck all the menu items.
	WORD wNumItems = sizeof(dwDelays) / sizeof(dwDelays[0]);
	for (WORD wInx = 0; wInx < wNumItems; wInx++)
		CheckMenuItem(hMenu, ID_MOTIONCONTROL_NODELAY + wInx, MF_BYCOMMAND | MF_UNCHECKED);
	// Check the activated menu item.
	CheckMenuItem(hMenu, ID_MOTIONCONTROL_NODELAY + wNumMenuItem, MF_BYCOMMAND | MF_CHECKED);
	DrawMenuBar(hWnd);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_SIZE:
		appGlobals.pRenderer->setSize(LOWORD(lParam), HIWORD(lParam));
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

			case ID_EDIT_BIGCUBE:
			case ID_EDIT_SMALLCUBE1:
			case ID_EDIT_SMALLCUBE2:
			case ID_EDIT_SMALLCUBE3:
			case ID_EDIT_SMALLCUBE4:
			case ID_EDIT_SMALLCUBE5:
			case ID_EDIT_SMALLCUBE6:
			case ID_EDIT_SMALLCUBE7:
			case ID_EDIT_SMALLCUBE8:
				MessageBox(nullptr, appGlobals.szNotImplemented, appGlobals.szTitle, MB_OK);
				break;

			case ID_MOTIONCONTROL_STOP:
				DisableAndEnableMenuItems(hWnd, ID_MOTIONCONTROL_STOP, ID_MOTIONCONTROL_CONTINUE);
				appGlobals.pRenderer->stop();
				break;
			case ID_MOTIONCONTROL_CONTINUE:
				DisableAndEnableMenuItems(hWnd, ID_MOTIONCONTROL_CONTINUE, ID_MOTIONCONTROL_STOP);
				appGlobals.pRenderer->cont();
				break;
			case ID_MOTIONCONTROL_RESTART:
				appGlobals.pRenderer->restart();
				break;

			case ID_MOTIONCONTROL_NODELAY:
			case ID_MOTIONCONTROL_1XDELAY:
			case ID_MOTIONCONTROL_5XDELAY:
			case ID_MOTIONCONTROL_10XDELAY:
			case ID_MOTIONCONTROL_15XDELAY:
			case ID_MOTIONCONTROL_20XDELAY:
			case ID_MOTIONCONTROL_30XDELAY:
			case ID_MOTIONCONTROL_40XDELAY:
			case ID_MOTIONCONTROL_50XDELAY:
				SetMotionDelay(hWnd, wmId - ID_MOTIONCONTROL_NODELAY);
				break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_MENUSELECT:
		{
		WORD wMenuItemId = LOWORD(wParam);
			// Check if a cube is being selected.
			if (ID_EDIT_BIGCUBE <= wMenuItemId && wMenuItemId <= ID_EDIT_SMALLCUBE8)
				appGlobals.pRenderer->highlight(wMenuItemId - ID_EDIT_BIGCUBE);
		}
		return 0;
	default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
