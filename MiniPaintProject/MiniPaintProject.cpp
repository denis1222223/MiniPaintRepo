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

void ClearHDC(HWND hWnd) {
	RECT rect;
	HDC hdc = GetDC(hWnd);
	GetClientRect(hWnd, &rect);
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
}

void DrawAllFigures(HDC hdc, vector<Figure*> figures) {
	for each(Figure* figure in figures) {
		(*figure).draw(hdc);
	}
}

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

int OnMenuClick(HWND hWnd, WORD itemId) {
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
	/*case ID_EDIT_UNDO:
		MessageBox(NULL, _T("undo"), _T(""), NULL);
		break; */
	case ID_COLOR_PEN:
		OnChangePenColor(hWnd);
		break;
	case ID_COLOR_FILL:
		OnChangeFillColor(hWnd);
		break;
	//case ID_INSTRUMENT_LINE:
	//	return ID_INSTRUMENT_LINE;
	//case ID_INSTRUMENT_RECTANGLE:
	//	return ID_INSTRUMENT_RECTANGLE;
	//case ID_INSTRUMENT_PENCIL:
	//	return ID_INSTRUMENT_PENCIL;
	default:
		return itemId;
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
	static bool paintNow = false;
	static int currentInstrument;
	static vector<Figure*> figures;
	static POINT startPoint;
	static Figure* currentFigure;

	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_NORMAL);
		hdc = GetDC(hWnd);
		break;
	case WM_COMMAND:
		currentInstrument = OnMenuClick(hWnd, LOWORD(wParam));
		//undo is shit
		if (currentInstrument == ID_EDIT_UNDO) {
			figures.pop_back();
			ClearHDC(hWnd);
			DrawAllFigures(hdc, figures);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
//		vector<Figure*> figures;
//		POINT point1; POINT point2;
//
//		RectangleFigure rect1;
//		point1.x = 23; point1.y = 23;
//		point2.x = 234; point2.y = 234;
//		rect1.points.push_back(point1);
//		rect1.points.push_back(point2);
//		figures.push_back(&rect1);
//
//		LineFigure line1;
//		point1.x = 23; point1.y = 23;
//		point2.x = 234; point2.y = 234;
//		line1.points.push_back(point1);
//		line1.points.push_back(point2);
//		figures.push_back(&line1);
//
//		PencilFigure pencil1;
//		point1.x = 23; point1.y = 23;
//		point2.x = 122; point2.y = 122;
//		POINT point3; point3.x = 122; point3.y = 322;
//		POINT point4; point4.x = 345; point4.y = 122;	
//		pencil1.points.push_back(point1);
//		pencil1.points.push_back(point2);
//		pencil1.points.push_back(point3);
//		pencil1.points.push_back(point4);
//		figures.push_back(&pencil1);
//
//		/*for each(Figure* figure in figures) {
//			(*figure).draw(hdc);
//		}
//*/
//		DrawAllFigures(hdc, figures);
		//EndPaint(hWnd, &ps);
	//	DrawAllFigures(hdc, figures);
		break;
	}
	case WM_MOUSEMOVE:
		if (paintNow) {
			POINT point;
			point.x = (short)LOWORD(lParam);
			point.y = (short)HIWORD(lParam);
			ClearHDC(hWnd);
			switch (currentInstrument)
			{
			case ID_INSTRUMENT_LINE:
			case ID_INSTRUMENT_RECTANGLE:
			case ID_INSTRUMENT_ELLIPSE:
			case ID_INSTRUMENT_POLYGON:
				if (currentFigure->points.size() > 1)
					currentFigure->points.pop_back();			
				break;
			}
			currentFigure->points.push_back(point);
			if (currentInstrument != 0) {
				DrawAllFigures(hdc, figures);
				currentFigure->draw(hdc);
			}	
		}
		break;
	case WM_LBUTTONDOWN:
		paintNow = true;
		startPoint.x = (short)LOWORD(lParam);
		startPoint.y = (short)HIWORD(lParam);
		switch (currentInstrument)
		{
		case ID_INSTRUMENT_LINE:
			currentFigure = new LineFigure();
			break;
		case ID_INSTRUMENT_PENCIL:
			currentFigure = new PencilFigure();
			break;
		case ID_INSTRUMENT_RECTANGLE:
			currentFigure = new RectangleFigure();
			break;
		case ID_INSTRUMENT_ELLIPSE:
			currentFigure = new EllipseFigure();
			break;
		case ID_INSTRUMENT_POLYGON:
			if (currentFigure == NULL)
				currentFigure = new PolygonFigure();			
			break;
		}
		if (currentInstrument != 0) {
			currentFigure->points.push_back(startPoint);			
		}
		if (currentInstrument == ID_INSTRUMENT_POLYGON) {
			currentFigure->draw(hdc);
		}
		break;
	case WM_LBUTTONUP:
		paintNow = false;
		if (currentInstrument != ID_INSTRUMENT_POLYGON) {
			figures.push_back(currentFigure);
			currentFigure = NULL;
		}		
		break;
	case WM_LBUTTONDBLCLK:
		if (currentInstrument == ID_INSTRUMENT_POLYGON) {
			figures.push_back(currentFigure);
			currentFigure = NULL;
		}
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

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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