#include "DrawingUtils.h"

// Нормализует прямоугольник: приводит координаты к виду left <= right, top <= bottom.
RECT NormalizeRect(const RECT& r) {
    RECT out = r;
    if (out.left > out.right) std::swap(out.left, out.right);
    if (out.top > out.bottom) std::swap(out.top, out.bottom);
    return out;
}

// Рисует одну фигуру с использованием текущего пера и кисти.
void DrawShapeGDI(HDC hdc, const Shape& s) {
    HPEN hPen = CreatePen(PS_SOLID, s.penWidth, s.color);
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    if (s.type == ShapeType::Rectangle) {
        Rectangle(hdc, s.rc.left, s.rc.top, s.rc.right, s.rc.bottom);
    }
    else if (s.type == ShapeType::Ellipse) {
        Ellipse(hdc, s.rc.left, s.rc.top, s.rc.right, s.rc.bottom);
    }
    else if (s.type == ShapeType::Triangle) {
        POINT pts[3];
        pts[0].x = (s.rc.left + s.rc.right) / 2; pts[0].y = s.rc.top;
        pts[1].x = s.rc.left; pts[1].y = s.rc.bottom;
        pts[2].x = s.rc.right; pts[2].y = s.rc.bottom;
        Polygon(hdc, pts, 3);
    }
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}
    
// Рисует все фигуры
void DrawAll(HDC hdc, const AppState& state) {
    for (const auto& s : state.shapes) DrawShapeGDI(hdc, s);
}