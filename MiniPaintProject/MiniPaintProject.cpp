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

HINSTANCE hInst;                          // current instance     
WCHAR szTitle[MAX_LOADSTRING];            // The title bar text      
WCHAR szWindowClass[MAX_LOADSTRING];      // the main window class name     
           
CHOOSECOLOR GetColorDialog(HWND hWnd) {
	CHOOSECOLOR chooseColor = { 0 };
	COLORREF  crCustColor[16];
	chooseColor.lStructSize = sizeof(CHOOSECOLOR);
	chooseColor.hInstance = NULL;
	chooseColor.hwndOwner = hWnd;
	chooseColor.lpCustColors = crCustColor;
	chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
	chooseColor.lCustData = 0L;
	chooseColor.lpfnHook = NULL;
	chooseColor.rgbResult = RGB(0x80, 0x80, 0x80);
	chooseColor.lpTemplateName = NULL;
	return chooseColor;
}

void OnChangePenColor(HWND hWnd, HPEN &hPen, HDC hdc, int &width, COLORREF &penColor) {
	CHOOSECOLOR chooseColor = GetColorDialog(hWnd);
	if (ChooseColor(&chooseColor)) 
	{
		penColor = chooseColor.rgbResult;
	}
	hPen = CreatePen(PS_SOLID, width, penColor);
	SelectObject(hdc, hPen);
}

void OnChangeFillColor(HWND hWnd, HBRUSH &hBrush, HDC hdc) {
	CHOOSECOLOR chooseColor = GetColorDialog(hWnd);
	if (ChooseColor(&chooseColor))
	{
		hBrush = CreateSolidBrush(chooseColor.rgbResult);
		SelectObject(hdc, hBrush);
	}
}

void OnChangePenWidth(HPEN &hPen, int itemId, HDC hdc, int &width, COLORREF &penColor) {
	switch (itemId)
	{
	case ID_WIDTH_1: width = 1; break;
	case ID_WIDTH_2: width = 2; break;
	case ID_WIDTH_3: width = 3; break;
	case ID_WIDTH_4: width = 4; break;
	case ID_WIDTH_5: width = 5; break;
	}
	hPen = CreatePen(PS_SOLID, width, penColor);
	SelectObject(hdc, hPen);
}

void ClearHDC(HWND hWnd, HDC hdc) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
}

void DrawAllFigures(HDC hdc, vector<Figure*> figures) {
	for each(Figure* figure in figures) {
		(*figure).draw(hdc);
	}
}

void Undo(HWND hWnd, vector<Figure*> &figures, HDC hdc) {
	if (figures.size() > 0) {
		figures.pop_back();
		ClearHDC(hWnd, hdc);
		DrawAllFigures(hdc, figures);
	}
}

void OnFiguresMoved(vector<Figure*> &figures, POINT startPoint, POINT point) {
	int valueX = point.x - startPoint.x;
	int valueY = point.y - startPoint.y;
	for each(Figure* figure in figures) {
		for each(POINT p in figure->points) {
			p.x += valueX;
			p.y += valueY;
		}
	}
}

string GetFilepathDialog(HWND hWnd, bool save) {
	static OPENFILENAME ofn;
	static char fullpath[255], filename[256], dir[256];
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = hInst; // дескриптор копии приложения
	ofn.lpstrFilter = "Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;
	if (save) {
		ofn.lpstrTitle = "Save picture as...";
		GetSaveFileName(&ofn);
	}
	else {
		ofn.lpstrTitle = "Open picture";
		GetOpenFileName(&ofn);
	}
	return fullpath;
}

void OpenPicture(HWND hWnd, HDC hdc) {
	static string fullpath;
	fullpath = GetFilepathDialog(hWnd, 0);
	if (!fullpath.empty())
	{
		RECT rect;
		HENHMETAFILE hEnhMtf;
		GetClientRect(hWnd, &rect);
		hEnhMtf = GetEnhMetaFile(fullpath.c_str());
		PlayEnhMetaFile(hdc, hEnhMtf, &rect);
	}
}

void SavePicture(HWND hWnd, HDC hdc, vector<Figure*> &figures) {
	static string fullpath;
	fullpath = GetFilepathDialog(hWnd, 1);
	if (!fullpath.empty())
	{
		RECT rect;
		HENHMETAFILE hEnhMtf;
		HDC mtfHdc;
		GetClientRect(hWnd, &rect);
		fullpath += ".emf";
		mtfHdc = CreateEnhMetaFile(hdc, fullpath.c_str(), NULL, NULL);
		DrawAllFigures(mtfHdc, figures);
		hEnhMtf = CloseEnhMetaFile(mtfHdc);
		DeleteEnhMetaFile(hEnhMtf);
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

void OnMenuClick(HWND hWnd, WORD itemId, vector<Figure*> &figures, HDC hdc, 
	int &currentInstrument, HPEN &hPen, HBRUSH &hBrush, int &width, COLORREF &penColor) {
	switch (itemId) {
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case ID_FILE_OPEN:
		OpenPicture(hWnd, hdc);
		break;
	case ID_FILE_SAVE:
		SavePicture(hWnd, hdc, figures);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case ID_FILE_PRINT:
		MessageBox(NULL, _T("print"), _T(""), NULL);
		break;
	case ID_EDIT_UNDO:
		Undo(hWnd, figures, hdc);
		break; 
	case ID_COLOR_PEN:
		OnChangePenColor(hWnd, hPen, hdc, width, penColor);
		break;
	case ID_COLOR_FILL:
		OnChangeFillColor(hWnd, hBrush, hdc);
		break;
	case ID_INSTRUMENT_LINE:
	case ID_INSTRUMENT_RECTANGLE:
	case ID_INSTRUMENT_PENCIL:
	case ID_INSTRUMENT_ELLIPSE:
	case ID_INSTRUMENT_POLYGON:
	case ID_INSTRUMENT_TEXT:
	case ID_EDIT_MOVE:
	case ID_EDIT_ZOOM:
		currentInstrument = itemId;
		break;
	case ID_WIDTH_1:
	case ID_WIDTH_2:
	case ID_WIDTH_3:
	case ID_WIDTH_4:
	case ID_WIDTH_5:
		OnChangePenWidth(hPen, itemId, hdc, width, penColor);
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
	static vector<Figure*> figures;
	static HDC hdc;
	static bool paintNow = false;
	static int currentInstrument = ID_INSTRUMENT_PENCIL;	
	static POINT startPoint;
	static Figure* currentFigure;
	static int width = 1;
	static COLORREF penColor = RGB(0, 0, 0);
	static HPEN hPen = CreatePen(PS_SOLID, width, penColor);
	static HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	static double scale = 1;

	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_NORMAL);
		hdc = GetDC(hWnd);
		SelectObject(hdc, hPen);
		break;
	case WM_COMMAND:
		OnMenuClick(hWnd, LOWORD(wParam), figures, hdc, currentInstrument, hPen, hBrush, width, penColor);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc2 = BeginPaint(hWnd, &ps);
		HDC          hdcMem;
		HBITMAP      hbmMem;
		HANDLE       hOld;
		static HDC hBitmapDC = 0;
		RECT rect;
		if (currentInstrument == ID_EDIT_ZOOM) {
			GetClientRect(hWnd, &rect);
			hdcMem = CreateCompatibleDC(hdc);
			hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			hOld = SelectObject(hdcMem, hbmMem);
			FillRect(hdcMem, &rect, WHITE_BRUSH);
			StretchBlt(hdcMem, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale),
				hdc, 0, 0, rect.right, rect.bottom, SRCCOPY);
			SelectObject(hdcMem, (HBRUSH)GetStockObject(NULL_BRUSH));
			SelectObject(hdcMem, (HPEN)GetStockObject(BLACK_PEN));
			Rectangle(hdcMem, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale));
			BitBlt(hdc2, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC(hdcMem);
			
		}

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_MOUSEMOVE:
		if (paintNow) {
			POINT point;
			point.x = (short)LOWORD(lParam);
			point.y = (short)HIWORD(lParam);
			ClearHDC(hWnd, hdc);
			switch (currentInstrument)
			{
			case ID_INSTRUMENT_LINE:
			case ID_INSTRUMENT_RECTANGLE:
			case ID_INSTRUMENT_ELLIPSE:
			case ID_INSTRUMENT_POLYGON:
				if (currentFigure->points.size() > 1)
					currentFigure->points.pop_back();
				break;
			/*case ID_EDIT_MOVE:
				OnFiguresMoved(figures, startPoint, point);
				DrawAllFigures(hdc, figures);
				return 0;*/
			}
			currentFigure->points.push_back(point);		
			DrawAllFigures(hdc, figures);
			SelectObject(hdc, hPen);
			SelectObject(hdc, hBrush);
			currentFigure->draw(hdc);
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
		case ID_INSTRUMENT_TEXT:	
			paintNow = false;
			currentFigure = new TextFigure();
			break;
		}
		//if (currentInstrument != ID_EDIT_MOVE)
			currentFigure->points.push_back(startPoint);	
		if (currentInstrument == ID_INSTRUMENT_POLYGON) {
			currentFigure->draw(hdc);
		}
		break;
	case WM_LBUTTONUP:
		paintNow = false;
		if ((currentInstrument != ID_INSTRUMENT_POLYGON) && (currentInstrument != ID_INSTRUMENT_TEXT)) {
			currentFigure->hPen = hPen;
			currentFigure->hBrush = hBrush;
			figures.push_back(currentFigure);
			currentFigure = NULL;
		}		
		break;
	case WM_LBUTTONDBLCLK:
		if (currentInstrument == ID_INSTRUMENT_POLYGON) {
			currentFigure->hPen = hPen;
			figures.push_back(currentFigure);
			currentFigure = NULL;
		}
		break;
	case WM_MOUSEWHEEL:
		
		if (currentInstrument == ID_EDIT_ZOOM) {
			HDC          hdcMem;
			HBITMAP      hbmMem;
			HANDLE       hOld;
			static HDC hBitmapDC = 0;
			RECT rect;
			if ((GET_WHEEL_DELTA_WPARAM(wParam) > 0) && (scale < 2)) {
				scale += 0.3;

			/*	RECT rect;
				GetClientRect(hWnd, &rect);
				SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
				Rectangle(hdc, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale));
				SetStretchBltMode(hdc, BLACKONWHITE);
				StretchBlt(hdc, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale),
					hdc, 0, 0, rect.right, rect.bottom, SRCCOPY);*/

				//GetClientRect(hWnd, &rect);
				//hdcMem = CreateCompatibleDC(hdc);
				//hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
				//hOld = SelectObject(hdcMem, hbmMem);
				//FillRect(hdcMem, &rect, WHITE_BRUSH);
				//StretchBlt(hdcMem, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale),
				//	hBitmapDC, 0, 0, rect.right, rect.bottom, SRCCOPY);
				//SelectObject(hdcMem, (HBRUSH)GetStockObject(NULL_BRUSH));
				//SelectObject(hdcMem, (HPEN)GetStockObject(BLACK_PEN));
				////Rectangle(hdcMem, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale));
				//BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
				//SelectObject(hdcMem, hOld);
				//DeleteObject(hbmMem);
				//DeleteDC(hdcMem);
			}
				
			if ((GET_WHEEL_DELTA_WPARAM(wParam) < 0) && (scale > 0.3)) {
				scale -= 0.3;

			/*	RECT rect;
				GetClientRect(hWnd, &rect);
				SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
				Rectangle(hdc, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale));
				SetStretchBltMode(hdc, BLACKONWHITE);
				StretchBlt(hdc, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale),
					hdc, 0, 0, rect.right, rect.bottom, SRCCOPY);*/

				/*GetClientRect(hWnd, &rect);
				hdcMem = CreateCompatibleDC(hdc);
				hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
				hOld = SelectObject(hdcMem, hbmMem);
				FillRect(hdcMem, &rect, WHITE_BRUSH);
				StretchBlt(hdcMem, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale),
					hBitmapDC, 0, 0, rect.right, rect.bottom, SRCCOPY);
				SelectObject(hdcMem, (HBRUSH)GetStockObject(NULL_BRUSH));
				SelectObject(hdcMem, (HPEN)GetStockObject(BLACK_PEN));
				Rectangle(hdcMem, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale));
				BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
				SelectObject(hdcMem, hOld);
				DeleteObject(hbmMem);
				DeleteDC(hdcMem);*/
			}
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
		}
		break;
	case  WM_CHAR:
		
		if (currentInstrument == ID_EDIT_MOVE) {
			for each(Figure* figure in figures) {
				for each(POINT p in figure->points) {
					p.x += 5;
					p.y += 5;
				}
			}
			ClearHDC(hWnd, hdc);
			DrawAllFigures(hdc, figures);
		}
		if ((currentInstrument == ID_INSTRUMENT_TEXT) && (!currentFigure->points.empty()))
		{		
			char c = (char)wParam;
			if (c == VK_RETURN) {
				figures.push_back(currentFigure);
				break;
			}			
			((TextFigure*)currentFigure)->text += c;
			currentFigure->draw(hdc);
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

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MINIPAINTPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Execute application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIPAINTPROJECT));

    MSG msg;

    // main message cycle:
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