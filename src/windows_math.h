#ifndef TRIDIMENSIONAL_WINDOWS_MATH_H
#define TRIDIMENSIONAL_WINDOWS_MATH_H

#include <windows.h>
#include "types.h"

void drawTriangle(HDC hdc, triangle toDraw);
void drawFilledTriangle(HDC hdc, triangle toDraw, COLORREF color);

#endif //TRIDIMENSIONAL_WINDOWS_MATH_H
