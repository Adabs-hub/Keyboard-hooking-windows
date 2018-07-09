#include <Windows.h>

HINSTANCE hInst;
HWND list;
LRESULT CALLBACK LowLevelKeyboardProc1(int code, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
HHOOK hhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc1, hInst, 0);


LRESULT CALLBACK LowLevelKeyboardProc1(int code, WPARAM wParam, LPARAM lParam) {
	//MessageBox(NULL, L"START", L"KEY", MB_OK);
	if ((code == HC_ACTION) &&
		((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN))) // 키 입력이면
	{

		KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*)lParam);
		TCHAR lpszName[256];
		DWORD dwMsg = 1;
		dwMsg += hooked.scanCode << 16; // ASCII 코드 변환
		dwMsg += hooked.flags << 24;
		GetKeyNameText(dwMsg, (lpszName), 256);
		//wchar_t lpszName[10];
		//memset(lpszName, 0, 256);
		//wsprintf(lpszName, L"%d", hooked.scanCode);
		SendMessage(list, LB_ADDSTRING, 0, (LPARAM)lpszName);
		//MessageBox(NULL,lpszName,L"KEY",MB_OK);
	}

	return CallNextHookEx(hhook, code, wParam, lParam);
}


LPCWSTR lpszClass = TEXT("HookTest");
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{


	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	hInst = hInstance;



	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		0, 0, 200, 500,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_CREATE:

		list = CreateWindow(L"listbox", NULL, WS_VISIBLE | WS_VSCROLL | WS_CHILD | WS_BORDER | LBS_NOTIFY, 0, 0, 200, 400, hWnd, (HMENU)0, hInst, NULL);
		SendMessage(list, LB_ADDSTRING, 0, (LPARAM)L"Start");

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}

	return(DefWindowProc(hWnd, Message, wParam, lParam));
}
