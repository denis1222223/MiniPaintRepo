#pragma once
#include <windows.h>
#include <list>
#include <vector>
#include "Resource.h"

using namespace std; 

class Figure
{
public:
	int instrument;
	vector<POINT> points;
	void virtual draw (HDC hdc) = 0;
};

class LineFigure : public Figure {
public: 
	LineFigure() {
		instrument = ID_INSTRUMENT_LINE;
	};

	void Figure::draw(HDC hdc) 
	{
		MoveToEx(hdc, points[0].x, points[0].y, NULL);
		LineTo(hdc, points[1].x, points[1].y);
	}
};

class RectangleFigure : Figure {
public:
	RectangleFigure() {
		instrument = ID_INSTRUMENT_RECTANGLE;
	};

	void Figure::draw(HDC hdc)
	{
		Rectangle(hdc, points[0].x, points[0].y, points[1].x, points[1].y);
	}
};
