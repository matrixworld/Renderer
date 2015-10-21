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

//FLOAT3D CrossProduct()

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

MATRIX4 Transition(FLOAT3D transition)
{
	MATRIX4 matrix_T;

	Matrix4SetZero(&matrix_T);

	matrix_T.var[0][0] = 1.0f;
	matrix_T.var[1][1] = 1.0f;
	matrix_T.var[2][2] = 1.0f;
	matrix_T.var[3][3] = 1.0f;

	matrix_T.var[3][0] = transition.x;
	matrix_T.var[3][1] = transition.y;
	matrix_T.var[3][2] = transition.z;

	return matrix_T;
}

MATRIX4 Rotation(FLOAT3D rotation)
{
	return MatrixMul4(MatrixMul4(Rotation_SingleAxis('x', rotation.x), Rotation_SingleAxis('y', rotation.y)),
		Rotation_SingleAxis('z', rotation.z));
}

MATRIX4 Rotation_SingleAxis(char axis, float degree)
{
	MATRIX4 matrix_R = { 0.0f };

	Matrix4SetZero(&matrix_R);

	matrix_R.var[0][0] = 1.0f;
	matrix_R.var[1][1] = 1.0f;
	matrix_R.var[2][2] = 1.0f;
	matrix_R.var[3][3] = 1.0f;

	//如果没进行旋转
	if (degree <= 0.001)
	{
		return matrix_R;
	}

	degree *= 0.01745f;
	float c = cosf(degree);
	float s = sinf(degree);

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
	MATRIX4 matrix_S;
	Matrix4SetZero(&matrix_S);

	matrix_S.var[0][0] = multi;
	matrix_S.var[1][1] = multi;
	matrix_S.var[2][2] = multi;
	matrix_S.var[3][3] = 1.0f;

	return matrix_S;
}

MATRIX4 RST(MATRIX4 S, MATRIX4 R,MATRIX4 T)
{
	return MatrixMul4(MatrixMul4(S, R), T);
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
	MATRIX4 FinalTransformMatrix44;
	Matrix4SetZero(&FinalTransformMatrix44);

	FinalTransformMatrix44 = RST(Scale(1.0f), Rotation(object->rotation), Transition(object->position));

	for (int lop = 0; lop < 3; lop++)
	{
		object->model.selfVertex[lop] = VectorTransform(object->model.selfVertex[lop], FinalTransformMatrix44);
	}
	//模型的中心被改变
	object->position.x = 0.0f;
	object->position.y = 0.0f;
	object->position.z = 0.0f;

	object->rotation.x = 0.0f;
	object->rotation.y = 0.0f;
	object->rotation.z = 0.0f;
}

void WorldToViewTransform(CAMERA *camera,OBJECT *object,MATRIX4 RST)
{
	//TODO

}

//计算逆矩阵
//人蠢就是这么艰难
MATRIX4 InvertMatrix4(MATRIX4 input)
{
	MATRIX4 output;
	//用于储存每次的余子式
	MATRIX3 tmp;
	float firstPart = 0.0f;

	Matrix4SetZero(&output);

	//先算分母，再求倒
	firstPart = (input.var[0][0] * input.var[1][1] * input.var[2][2] * input.var[3][3]);
	firstPart += (input.var[0][1] * input.var[1][2] * input.var[2][3] * input.var[3][0]);
	firstPart += (input.var[0][2] * input.var[1][3] * input.var[2][0] * input.var[3][1]);
	firstPart += (input.var[0][3] * input.var[1][0] * input.var[2][1] * input.var[3][2]);
	firstPart -= (input.var[0][3] * input.var[1][2] * input.var[2][1] * input.var[3][0]);
	firstPart -= (input.var[0][2] * input.var[1][1] * input.var[2][0] * input.var[3][3]);
	firstPart -= (input.var[0][1] * input.var[1][0] * input.var[2][3] * input.var[3][2]);
	firstPart -= (input.var[0][0] * input.var[1][3] * input.var[2][2] * input.var[3][1]);

	//求倒
	firstPart = 1 / firstPart;

	//最里层的循环...
	int x, y;
	//用于标识行列式的x y...
	int x_tmp, y_tmp;

	for (int lop = 0; lop < 4; lop++)
	{
		for (int lop2 = 0; lop2 < 4; lop2++)
		{

			//每次开始为输出矩阵的元素求值
			Matrix3SetZero(&tmp);
			x_tmp = 0;
			y_tmp = 0;

			for (x = 0; x < 4; x++)
			{
				y_tmp = 0;
				if (x == lop)
				{
					continue;
				}
				for (y = 0; y < 4; y++)
				{
					if (y == lop2)
					{
						continue;
					}
					tmp.var[x_tmp][y_tmp++] = input.var[x][y];
				}
				x_tmp++;
			}

			output.var[lop2][lop] = Determinant(tmp);

			//带-1
			if ((lop + 1 + lop2 + 1) % 2)
			{
				output.var[lop2][lop] = (-1.0f)*output.var[lop2][lop];
			}
			//output.var[lop2][lop] *= firstPart;
		}
	}

	return output;
}