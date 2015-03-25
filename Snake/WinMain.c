#include <windows.h>
#include <time.h>
#include "resource.h"
#include "Snake.h"

HINSTANCE hInstance;
TCHAR szAppName[] = TEXT("贪吃蛇");



BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmd, int nShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wcls;

	wcls.cbSize = sizeof(wcls);
	wcls.style = CS_HREDRAW | CS_VREDRAW;
	wcls.lpfnWndProc = WndProc;
	wcls.hInstance = hInst;
	wcls.cbWndExtra = 0;
	wcls.cbClsExtra = 0;
	wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//(COLOR_BTNFACE + 1);
	wcls.hIcon = LoadIcon(hInst, szAppName);
	wcls.hIconSm = LoadIcon(hInst, szAppName);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.lpszClassName = szAppName;
	wcls.lpszMenuName = szAppName;

	if (!RegisterClassEx(&wcls)){
		MessageBox(NULL, TEXT("注册类出错！"), TEXT("错误"), MB_ICONERROR);
		return 0;
	}

	hInstance = hInst;

	hwnd = CreateWindow(szAppName,
		szAppName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		400,
		NULL, NULL,
		hInst, NULL);
	ShowWindow(hwnd, nShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
//窗口字体初始化
HFONT FontInitialize(HWND hwnd)
{
	static LOGFONT logfont;

	wcscpy_s(logfont.lfFaceName, sizeof(logfont.lfFaceName), TEXT("宋体"));
	logfont.lfHeight = 14;

	return CreateFontIndirect(&logfont);
}
//显示相关位图
void ShowBitmap(HWND hwnd, RECT rect, TCHAR *szBitmapName)
{
	HDC hdc, hdcMem;
	BITMAP bitmap;
	HBITMAP hBitmap;

	hdc = GetDC(hwnd);

	hBitmap = LoadBitmap(hInstance, szBitmapName);
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);

	TransparentBlt(hdc,
		rect.left + (rect.right - rect.left - bitmap.bmWidth) / 2,
		rect.top + (rect.bottom - rect.top - bitmap.bmHeight) / 2,
		bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth,
		bitmap.bmHeight, RGB(255, 255, 255));

	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
	ReleaseDC(hwnd, hdc);
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static HFONT hfont;
	static BOOL bGameBegin, bGamePause, bGameOver;
	static int cyChar, iBlockSize, iDirection, iNowDir, iTime;
	HDC hdc;
	HMENU hmenu;
	PAINTSTRUCT ps;
	TEXTMETRIC txm;

	switch (message)
	{
	case WM_CREATE:
		srand((UINT)time(NULL));
		hfont = FontInitialize(hwnd);

		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &txm);
		cyChar = txm.tmHeight + txm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);

		bGameBegin = bGameOver = bGamePause = FALSE;
		return 0;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		rect.left = cyChar;
		rect.top = cyChar;
		iBlockSize = (rect.bottom - cyChar - 1) / VERT_BLK_NUMS;
		rect.right = iBlockSize * HORZ_BLK_NUMS + rect.left;
		rect.bottom = iBlockSize * VERT_BLK_NUMS + rect.top;
		return 0;
	case WM_INITMENUPOPUP:
		hmenu = GetMenu(hwnd);
		switch (LOWORD(lParam))
		{
		case 0:
			EnableMenuItem(hmenu, IDM_PAUSE, bGameBegin && !bGamePause ? MF_ENABLED : MF_DISABLED);
			EnableMenuItem(hmenu, IDM_RESUME, bGameBegin && bGamePause ? MF_ENABLED : MF_DISABLED);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_START://新游戏
			SnakeInit();//初始化蛇
			FoodInit();//初始化食物
			iDirection = VK_LEFT;//初始移动方向
			bGameBegin = TRUE;
			bGameOver = bGamePause = FALSE;
			SetTimer(hwnd, ID_TIMER, 1, NULL);//移动速度
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case IDM_PAUSE://暂停游戏
			bGamePause = TRUE;
			ShowBitmap(hwnd, rect, TEXT("暂停"));
			KillTimer(hwnd, ID_TIMER);
			break;
		case IDM_RESUME://开始游戏
			bGamePause = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
			SetTimer(hwnd, ID_TIMER, iTime, NULL);
			break;
		case IDM_ABOUT:
			DialogBox(hInstance, szAppName, hwnd, DlgProc);
			break;
		case IDM_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		return 0;
	case WM_KEYDOWN:
		if (!bGameBegin || bGamePause)
			break;
		switch (wParam)
		{
		case VK_LEFT:
			if (iNowDir != VK_RIGHT)//限制转向
				iDirection = wParam;
			break;
		case VK_RIGHT:
			if (iNowDir != VK_LEFT)//限制转向
				iDirection = wParam;
			break;
		case VK_DOWN:
			if (iNowDir != VK_UP)//限制转向
				iDirection = wParam;
			break;
		case VK_UP:
			if (iNowDir != VK_DOWN)//限制转向
				iDirection = wParam;
			break;
		}
		return 0;
	case WM_TIMER:
		iTime = 1000 - 10 * GetScore();//加快移动速度
		SetTimer(hwnd, ID_TIMER, iTime, NULL);
		if (!(iNowDir = SnakeWalk(hwnd, rect, iBlockSize, hfont, iDirection)))//判断游戏是否结束
		{
			bGameBegin = FALSE;
			bGameOver = TRUE;
			KillTimer(hwnd, ID_TIMER);
			MessageBeep(MB_ICONWARNING);
			ShowBitmap(hwnd, rect, TEXT("游戏结束"));
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		DrawFoodAndScore(hdc, rect, iBlockSize, hfont);
		DrawSnake(hdc, rect, iBlockSize);
		if (bGamePause)
			ShowBitmap(hwnd, rect, TEXT("暂停"));
		if (bGameOver)
			ShowBitmap(hwnd, rect, TEXT("游戏结束"));

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		DeleteObject(hfont);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}