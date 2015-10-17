#include "MathLine.h"

int LiangBarskyLineClipping(FLOAT2D *p0, FLOAT2D *p1)
{
	float t0 = 0.0, t1 = 1.0;
	//规定剪裁的边界
	float edgeLeft = 0.0, edgeRight = (float)RENDER_X, edgeTop = (float)RENDER_Y, edgeBottom = 0.0;
	float x0 = (float)p0->x, y0 = (float)p0->y, x1 = (float)p1->x, y1 = (float)p1->y;
	float dx = x1 - x0;
	float dy = y1 - y0;
	float p, q, r;

	for (int edge = 0; edge < 4; edge++)
	{
		if (edge == 0) { p = -dx; q = -(edgeLeft - x0); }
		if (edge == 1) { p = dx; q = edgeRight - x0; }
		if (edge == 2) { p = -dy; q = -(edgeBottom - y0); }
		if (edge == 3) { p = dy; q = edgeTop - y0; }
		r = q / p;

		//平行于边界且在边界外
		if (p == 0 && q < 0) {
			return 0;
		}

		if (p < 0) {
			if (r > t1) {
				return 0;
			}
			else if (r > t0) {
				t0 = r;
			}
		}
		else if (p > 0) {
			if (r < t0) {
				return 0;
			}
			else if (r < t1) {
				t1 = r;
			}
		}
	}

	p0->x = (int)(x0 + t0*dx);
	p0->y = (int)(y0 + t0*dy);
	p1->x = (int)(x0 + t1*dx);
	p1->y = (int)(y0 + t1*dy);
	return 1;
}