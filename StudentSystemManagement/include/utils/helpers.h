#ifndef HELPERS_H
#define HELPERS_H

#include <gdiplus.h>
using namespace Gdiplus;

namespace Helpers {
    // 1. Hàm vẽ VIỀN bo góc (Dùng cho shadow/khung)
    inline void DrawRoundRect(Graphics& g, Pen* p, float x, float y, float w, float h, float r) {
        GraphicsPath path;
        path.AddArc(x, y, r, r, 180, 90);
        path.AddArc(x + w - r, y, r, r, 270, 90);
        path.AddArc(x + w - r, y + h - r, r, r, 0, 90);
        path.AddArc(x, y + h - r, r, r, 90, 90);
        path.CloseFigure();
        g.DrawPath(p, &path);
    }

    // 2. Hàm TÔ MÀU đặc bo góc (Dùng cho Card và Nút bấm)
    inline void FillRoundRect(Graphics& g, Brush* b, float x, float y, float w, float h, float r) {
        GraphicsPath path;
        path.AddArc(x, y, r, r, 180, 90);
        path.AddArc(x + w - r, y, r, r, 270, 90);
        path.AddArc(x + w - r, y + h - r, r, r, 0, 90);
        path.AddArc(x, y + h - r, r, r, 90, 90);
        path.CloseFigure();
        g.FillPath(b, &path);
    }
}
#endif