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

MATRIX3 MatrixMul3(MATRIX3 a, MATRIX3 b)
{
	int lop, lop2;
	MATRIX3 matrix = { 0.0f };
	for (lop = 0; lop < 3; lop++)
	{
		for (lop2 = 0; lop2 < 3; lop2++)
		{
			matrix.var[lop][lop2] =
				(a.var[lop][0] * b.var[0][lop2]) +
				(a.var[lop][1] * b.var[1][lop2]) +
				(a.var[lop][2] * b.var[2][lop2]);
		}
	}

	return matrix;
}

MATRIX4 MatrixMul4(MATRIX4 a, MATRIX4 b)
{
	int lop, lop2;
	MATRIX4 matrix = { 0.0f };
	for (lop = 0; lop < 4; lop++)
	{
		for (lop2 = 0; lop2 < 4; lop2++)
		{
			matrix.var[lop][lop2] =
				(a.var[lop][0] * b.var[0][lop2]) +
				(a.var[lop][1] * b.var[1][lop2]) +
				(a.var[lop][2] * b.var[2][lop2]) +
				(a.var[lop][3] * b.var[3][lop2]);
		}
	}

	return matrix;
}

MATRIX4 Translation(float i, float j, float k)
{
	MATRIX4 matrix_T = { 0.0f };

	matrix_T.var[0][0] = 1.0f;
	matrix_T.var[1][1] = 1.0f;
	matrix_T.var[2][2] = 1.0f;
	matrix_T.var[3][3] = 1.0f;

	matrix_T.var[3][0] = i;
	matrix_T.var[3][1] = j;
	matrix_T.var[3][2] = k;

	return matrix_T;
}

MATRIX4 Rotation(char axis, float degree)
{
	degree *= 0.01745;
	float c = cosf(degree);
	float s = sinf(degree);

	MATRIX4 matrix_R = { 0.0f };
	matrix_R.var[0][0] = 1.0f;
	matrix_R.var[1][1] = 1.0f;
	matrix_R.var[2][2] = 1.0f;
	matrix_R.var[3][3] = 1.0f;

	switch (axis)
	{
	case 'x':
	case 'X':
		matrix_R.var[1][1] = c;
		matrix_R.var[2][2] = c;
		matrix_R.var[1][2] = s;
		matrix_R.var[2][1] = -s;
		return matrix_R;
	case 'y':
	case 'Y':
		matrix_R.var[0][0] = c;
		matrix_R.var[2][2] = c;
		matrix_R.var[2][0] = s;
		matrix_R.var[0][2] = -s;
		return matrix_R;
	case 'z':
	case 'Z':
		matrix_R.var[0][0] = c;
		matrix_R.var[1][1] = c;
		matrix_R.var[0][1] = s;
		matrix_R.var[1][0] = -s;
		return matrix_R;
	}

	//Should never reach here;
	return matrix_R;
}

MATRIX4 Scale(float multi)
{
	MATRIX4 matrix_S = { 0.0f };
	matrix_S.var[0][0] = multi;
	matrix_S.var[1][1] = multi;
	matrix_S.var[2][2] = multi;
	matrix_S.var[3][3] = 1.0f;

	return matrix_S;
}

MATRIX4 RS(MATRIX4 S, MATRIX4 R)
{
	return MatrixMul4(S, R);
}

FLOAT3D VectorTransform(FLOAT3D src, MATRIX4 TRS)
{
	FLOAT3D des = { 0.0f };
	des.x = src.x*TRS.var[0][0] + src.y*TRS.var[1][0] + src.z*TRS.var[2][0] + TRS.var[3][0];
	des.y = src.x*TRS.var[0][1] + src.y*TRS.var[1][1] + src.z*TRS.var[2][1] + TRS.var[3][1];
	des.z = src.x*TRS.var[0][2] + src.y*TRS.var[1][2] + src.z*TRS.var[2][2] + TRS.var[3][2];

	return des;
}

void ObjectToWorldTransform(OBJECT *object)
{
	//TODO
}