#include "stdafx.h"
#include <windows.h>
#include <CommDlg.h>
#include <stdio.h>
#include <string>
#include "resource.h"
#include "Figure.h"
#include <list>
#include <vector>
using namespace std;

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

//// menu / color dialog
//static HPEN hPen;
//static HBRUSH hBrush;
//static COLORREF  crCustColor[16];
//static HDC hCompatibleDC = 0;
//static HDC hBitmapDC = 0;
//static HDC hdc1;
//static int width = 0;
//
//// line
//HDC static hdc;
//static int xBegin = 0, yBegin = 0;
//static double scale;
//static bool b, bText, flag, bPoly, bPolyline;
//static int id;
//RECT rect;
//static HBITMAP hCompatibleBitmap, hBitmap;
//static int f = 0;
//TEXTMETRIC tm;
//static string text;
//static int cxChar, cyChar;
//static int xT, yT;
/////////////////////////

CHOOSECOLOR GetColorDialog(HWND hWnd) {
	CHOOSECOLOR chooseColor = { 0 };
	//chooseColor.lStructSize = sizeof(CHOOSECOLOR);
	//chooseColor.hInstance = NULL;
	//chooseColor.hwndOwner = hWnd;
	//chooseColor.lpCustColors = crCustColor;
	//chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
	//chooseColor.lCustData = 0L;
	//chooseColor.lpfnHook = NULL;
	//chooseColor.rgbResult = RGB(0x80, 0x80, 0x80);
	//chooseColor.lpTemplateName = NULL;
	return chooseColor;
}

void OnChangePenColor(HWND hWnd) {
	CHOOSECOLOR chooseColor = GetColorDialog(hWnd);
	if (ChooseColor(&chooseColor))
	{
		//DeleteObject(hPen);
		//hPen = CreatePen(PS_SOLID, width, chooseColor.rgbResult);
		//DeleteObject(SelectObject(hCompatibleDC, hPen));
		//DeleteObject(SelectObject(hBitmapDC, hPen));
		//DeleteObject(SelectObject(hdc1, hPen));
	}
}

void OnChangeFillColor(HWND hWnd) {
	CHOOSECOLOR chooseColor = GetColorDialog(hWnd);
	if (ChooseColor(&chooseColor))
	{
	/*	DeleteObject(hBrush);
		hBrush = CreateSolidBrush(chooseColor.rgbResult);
		DeleteObject(SelectObject(hCompatibleDC, hBrush));
		DeleteObject(SelectObject(hBitmapDC, hBrush));
		DeleteObject(SelectObject(hdc1, hBrush));*/
	}
}

//void Line(HWND hWnd) {
//	id = ID_INSTRUMENT_LINE;
//	scale = 1;
//	xBegin = 0;
//	yBegin = 0;
//	bPoly = false;
//	bPolyline = false;
//	InvalidateRect(hWnd, NULL, FALSE);
//}

// Aboutbox click handler.
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

void OnMenuClick(HWND hWnd, WORD itemId) {
	switch (itemId) {
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case ID_FILE_OPEN:
		MessageBox(NULL, _T("open"), _T(""), NULL);
		break;
	case ID_FILE_SAVE:
		MessageBox(NULL, _T("save"), _T(""), NULL);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case ID_FILE_PRINT:
		MessageBox(NULL, _T("print"), _T(""), NULL);
		break;
	case ID_EDIT_UNDO:
		MessageBox(NULL, _T("undo"), _T(""), NULL);
		break; 
	case ID_COLOR_PEN:
		OnChangePenColor(hWnd);
		break;
	case ID_COLOR_FILL:
		OnChangeFillColor(hWnd);
		break;
	case ID_INSTRUMENT_LINE:
		break;
	}
}

//   saving instance processing and creating main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // saving instance discriptor to the global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//  process messages in main window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;

	/*static int x0, y0, x1, y1, x2, y2, oldMixMode;*/
	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_NORMAL);
		hdc = GetDC(hWnd);
		/*scale = 1;
		GetClientRect(hWnd, &rect);
		hdc1 = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
		flag = false;

		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(hdc, hBrush);
		hPen = (HPEN)GetStockObject(BLACK_PEN);
		SelectObject(hdc1, hBrush);
		SelectObject(hdc1, hPen);

		width = 0;

		hCompatibleDC = CreateCompatibleDC(hdc);
		hCompatibleBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		hBitmapDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		DeleteObject(SelectObject(hCompatibleDC, hCompatibleBitmap));
		DeleteObject(SelectObject(hCompatibleDC, (HBRUSH)WHITE_BRUSH));
		PatBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, PATCOPY);
		DeleteObject(SelectObject(hBitmapDC, hBitmap));
		DeleteObject(SelectObject(hBitmapDC, (HBRUSH)WHITE_BRUSH));
		PatBlt(hBitmapDC, 0, 0, rect.right, rect.bottom, PATCOPY);
		DeleteObject(SelectObject(hCompatibleDC, hPen));
		DeleteObject(SelectObject(hCompatibleDC, hBrush));
		DeleteObject(SelectObject(hBitmapDC, hPen));
		DeleteObject(SelectObject(hBitmapDC, hBrush));

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;
*/
		break;
	case WM_COMMAND:
	 	OnMenuClick(hWnd, LOWORD(wParam));
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...

		vector<Figure*> figures;
		LineFigure line1;
		line1.instrument = ID_INSTRUMENT_LINE;
		POINT point1; point1.x = 23; point1.y = 23;
		POINT point2; point2.x = 234; point2.y = 234;
		line1.points.push_back(point1);
		line1.points.push_back(point2);
		figures.push_back(&line1);

		for each(Figure* figure in figures) {
			(*figure).draw(hdc);
		}

		EndPaint(hWnd, &ps);

		break;
	}
	case WM_MOUSEMOVE:
	/*	GetClientRect(hWnd, &rect);
		hCompatibleBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	    DeleteObject(SelectObject(hCompatibleDC, hCompatibleBitmap));
		BitBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, hBitmapDC, 0, 0, SRCCOPY);
		hCompatibleDC = CreateCompatibleDC(hdc);
		if (b && id == ID_INSTRUMENT_LINE) {
			x2 = (short)LOWORD(lParam);
			y2 = (short)HIWORD(lParam);
			MoveToEx(hCompatibleDC, x1, y1, NULL);
			LineTo(hCompatibleDC, x2, y2);
		}

		hCompatibleBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		DeleteObject(SelectObject(hCompatibleDC, hCompatibleBitmap));
		BitBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, hBitmapDC, 0, 0, SRCCOPY);
		x2 = (short)LOWORD(lParam);
		y2 = (short)HIWORD(lParam);
		MoveToEx(hCompatibleDC, x1, y1, NULL);
		LineTo(hCompatibleDC, x2, y2);

		GetClientRect(hWnd, &rect);
		if (b && (bPoly == false) && bText == false)
		{
			hCompatibleBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			DeleteObject(SelectObject(hCompatibleDC, hCompatibleBitmap));
			BitBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, hBitmapDC, 0, 0, SRCCOPY);
			switch (id)
			{
			case ID_INSTRUMENT_LINE:
				x2 = (short)LOWORD(lParam);
				y2 = (short)HIWORD(lParam);
				MoveToEx(hCompatibleDC, x1, y1, NULL);
				LineTo(hCompatibleDC, x2, y2);
				break;
			}
			f = 2;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
		}
		if (b&&bPoly&&bText == false)
		{
			hCompatibleBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			DeleteObject(SelectObject(hCompatibleDC, hCompatibleBitmap));
			BitBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, hBitmapDC, 0, 0, SRCCOPY);
			x2 = (short)LOWORD(lParam);
			y2 = (short)HIWORD(lParam);
			MoveToEx(hCompatibleDC, x1, y1, NULL);
			LineTo(hCompatibleDC, x2, y2);
			f = 2;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
		}
		f=2;
			  InvalidateRect(hWnd,NULL,FALSE);
			  UpdateWindow(hWnd);*/
		break;
	case WM_LBUTTONDOWN:
		//if (id == ID_BUTTONZOOM)
		//	id = ID_BUTTONPAN;
		//switch (id)
		//{		
		//case ID_INSTRUMENT_LINE:
		//	x1 = x2 = (short)LOWORD(lParam);
		//	y1 = y2 = (short)HIWORD(lParam);
		//	MoveToEx(hdc, x1, y1, NULL);
		//	if((id==ID_BUTTONLINE))
		//		MoveToEx(hBitmapDC,x1,y1,NULL);
		//	break;		
		//}
		//SetCapture(hWnd);
		//b = true;
		break;
	case WM_LBUTTONUP:
		/*ReleaseCapture();
		if (bText)
		{
			xT = (short)LOWORD(lParam);
			yT = (short)HIWORD(lParam);
			text.clear();
			b = false;
			break;
		}
		GetClientRect(hWnd, &rect);
		hCompatibleBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		DeleteObject(SelectObject(hCompatibleDC, hCompatibleBitmap));
		BitBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, hBitmapDC, 0, 0, SRCCOPY);
		if (b && (bPoly == false))
		{
			x2 = (short)LOWORD(lParam);
			y2 = (short)HIWORD(lParam);
			switch (id)
			{
			case ID_INSTRUMENT_LINE:
				MoveToEx(hdc1, x1, y1, NULL);
				LineTo(hdc1, x2, y2);
				MoveToEx(hBitmapDC, x1, y1, NULL);
				LineTo(hBitmapDC, x2, y2);
				break;
			}
			f = 1;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			b = false;
		}
		if (b&&bPoly)
		{
			MoveToEx(hdc1, x1, y1, NULL);
			MoveToEx(hBitmapDC, x1, y1, NULL);
			x2 = (short)LOWORD(lParam);
			y2 = (short)HIWORD(lParam);
			LineTo(hdc1, x2, y2);
			LineTo(hBitmapDC, x2, y2);
			x1 = x2;
			y1 = y2;
			b = false;
			f = 1;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
		}*/
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//  Registration window's class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINIPAINTPROJECT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_MINIPAINTPROJECT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MINIPAINTPROJECT));

	return RegisterClassExW(&wcex);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MINIPAINTPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIPAINTPROJECT));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}