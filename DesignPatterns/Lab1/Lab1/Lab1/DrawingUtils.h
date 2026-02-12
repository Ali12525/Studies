#pragma once
#include <windows.h>
#include "AppState.h"

// Нормализует прямоугольник: приводит координаты к виду left <= right, top <= bottom.
RECT NormalizeRect(const RECT& r);

// Рисует одну фигуру с помощью GDI.
void DrawShapeGDI(HDC hdc, const Shape& s);
void DrawAll(HDC hdc, const AppState& state);