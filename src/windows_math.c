#include "windows_math.h"

void drawTriangle(HDC hdc, triangle toDraw)
{
    MoveToEx(hdc, toDraw.p[0].x, toDraw.p[0].y,NULL);
    LineTo(hdc, toDraw.p[1].x, toDraw.p[1].y);
    LineTo(hdc, toDraw.p[2].x, toDraw.p[2].y);
    LineTo(hdc, toDraw.p[0].x, toDraw.p[0].y);
}

void drawFilledTriangle(HDC hdc, triangle toDraw, COLORREF color)
{
    HPEN hPen = CreatePen(PS_NULL, 1, RGB(0,0,0));
    HPEN oldPen = (HPEN) SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, hBrush);

    POINT pTriangle[3];
    pTriangle[0].x = toDraw.p[0].x;
    pTriangle[0].y = toDraw.p[0].y;

    pTriangle[1].x = toDraw.p[1].x;
    pTriangle[1].y = toDraw.p[1].y;

    pTriangle[2].x = toDraw.p[2].x;
    pTriangle[2].y = toDraw.p[2].y;

    Polygon(hdc, pTriangle, 3);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}