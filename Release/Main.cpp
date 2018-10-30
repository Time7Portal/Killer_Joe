#include <Windows.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
unsigned int GetListBoxCount(HWND listWin);
void ProcesserKiller(wchar_t *processName);

WNDCLASS wc; // 전역 윈도우 클래스 정의

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Killer-Joe";

	wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx
	(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Killer-Joe",    // Window text
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND editWin;
	static HWND listWin;
	static int selectedListNum = 0;
	static wchar_t listBuf[2048] = L"";

	switch (uMsg)
	{
	case WM_CREATE:
		editWin = CreateWindowExW(NULL, L"edit", L"", WS_CHILD | WS_VISIBLE, 0, 3, 284, 24, hwnd, (HMENU)30, wc.hInstance, NULL);
		CreateWindowExW(NULL, L"button", L"ADD LIST", WS_CHILD | WS_VISIBLE, 0, 30, 200, 50, hwnd, (HMENU)10, wc.hInstance, NULL);
		CreateWindowExW(NULL, L"button", L"DELETE", WS_CHILD | WS_VISIBLE, 200, 30, 84, 50, hwnd, (HMENU)15, wc.hInstance, NULL);
		listWin = CreateWindowExW(NULL, L"listbox", L"", WS_CHILD | WS_VISIBLE, 0, 80, 284, 142, hwnd, (HMENU)40, wc.hInstance, NULL);
		CreateWindowExW(NULL, L"button", L"!! LET'S SLAY !!", WS_CHILD | WS_VISIBLE, 0, 211, 284, 50, hwnd, (HMENU)20, wc.hInstance, NULL);
		return 0;
		
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = 300;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = 300;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 300;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 300;
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 10: // 10 번 버튼
			GetWindowTextW(editWin, listBuf, 2048);
			SendMessageW(listWin, LB_ADDSTRING, NULL, (LPARAM)listBuf);
			break;

		case 15: // 15 번 버튼
			selectedListNum = (int)SendMessageW(listWin, LB_GETCURSEL, NULL, NULL);
			SendMessageW(listWin, LB_DELETESTRING, selectedListNum, NULL);
			break;

		case 20: // 20 번 버튼
			for (unsigned int i = 0; i < GetListBoxCount(listWin); i++)
			{
				SendMessageW(listWin, LB_GETTEXT, i, (WPARAM)listBuf);
				ProcesserKiller(listBuf);
			}
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

unsigned int GetListBoxCount(HWND listWin)
{
	unsigned int listCount = 0;
	listCount = SendMessageW(listWin, LB_GETCOUNT, NULL, NULL);
	return listCount;
}

void ProcesserKiller(wchar_t *processName)
{
	HANDLE hGetProcess = NULL;
	DWORD dGetProgramID = NULL;
	HWND hGetWnd = NULL;

	hGetWnd = FindWindowW(NULL, processName);
	GetWindowThreadProcessId(hGetWnd, &dGetProgramID);
	hGetProcess = OpenProcess(PROCESS_TERMINATE, false, dGetProgramID);
	TerminateProcess(hGetProcess, 0);
}