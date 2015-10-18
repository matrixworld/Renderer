#include "MagicMath.h"

int LiangBarskyLineClipping(FLOAT2D *p0, FLOAT2D *p1)
{
	float t0 = 0.0f, t1 = 1.0f;
	//规定剪裁的边界
	float edgeLeft = 0.0f, edgeRight = (float)RENDER_X, edgeTop = (float)RENDER_Y, edgeBottom = 0.0f;
	float x0 = p0->x, y0 = p0->y, x1 = p1->x, y1 = p1->y;
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

	p0->x = x0 + t0*dx;
	p0->y = y0 + t0*dy;
	p1->x = x0 + t1*dx;
	p1->y = y0 + t1*dy;
	return 1;
}

float DotProduct(FLOAT3D a, FLOAT3D b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

void MatrixMul3(MATRIX3 *output, MATRIX3 a, MATRIX3 b)
{
	int lop, lop2;
	for (lop = 0; lop < 3; lop++)
	{
		for (lop2 = 0; lop2 < 3; lop2++)
		{
			output->var[lop][lop2] =
				(a.var[lop][0] * b.var[0][lop2]) +
				(a.var[lop][1] * b.var[1][lop2]) +
				(a.var[lop][2] * b.var[2][lop2]);
		}
	}
}

void MatrixMul4(MATRIX4 *output, MATRIX4 a, MATRIX4 b)
{
	int lop, lop2;
	for (lop = 0; lop < 4; lop++)
	{
		for (lop2 = 0; lop2 < 4; lop2++)
		{
			output->var[lop][lop2] =
				(a.var[lop][0] * b.var[0][lop2]) +
				(a.var[lop][1] * b.var[1][lop2]) +
				(a.var[lop][2] * b.var[2][lop2]) +
				(a.var[lop][3] * b.var[3][lop2]);
		}
	}
}