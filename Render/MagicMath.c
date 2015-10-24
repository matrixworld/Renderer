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

FLOAT3D CrossProduct(FLOAT3D a, FLOAT3D b)
{
	FLOAT3D output;
	output.x = a.y*b.z - a.z*b.y;
	output.y = a.z*b.x - a.x*b.z;
	output.z = a.x*b.y - a.y*b.x;
	return output;
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
	if (-0.001f <= degree && degree <= 0.001f)
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

void VectorTransform(FLOAT3D *src, MATRIX4 TRS)
{
	FLOAT3D newPOS;

	newPOS.x = src->x*TRS.var[0][0] + src->y*TRS.var[1][0] + src->z*TRS.var[2][0] + TRS.var[3][0];
	newPOS.y = src->x*TRS.var[0][1] + src->y*TRS.var[1][1] + src->z*TRS.var[2][1] + TRS.var[3][1];
	newPOS.z = src->x*TRS.var[0][2] + src->y*TRS.var[1][2] + src->z*TRS.var[2][2] + TRS.var[3][2];

	src->x = newPOS.x;
	src->y = newPOS.y;
	src->z = newPOS.z;
}

//计算逆矩阵
//人蠢就是这么艰难
MATRIX4 InvertMatrix4(MATRIX4 input)
{
	MATRIX4 output;
	//用于储存每次的余子式
	MATRIX3 tmp;

	Matrix4SetZero(&output);

	/*
	float firstPart ;
	//先算分母，再求倒
	firstPart =
		(input.var[0][0] * input.var[1][1] * input.var[2][2] * input.var[3][3]) +
		(input.var[0][1] * input.var[1][2] * input.var[2][3] * input.var[3][0]) +
		(input.var[0][2] * input.var[1][3] * input.var[2][0] * input.var[3][1]) +
		(input.var[0][3] * input.var[1][0] * input.var[2][1] * input.var[3][2]) -
		(input.var[0][3] * input.var[1][2] * input.var[2][1] * input.var[3][0]) -
		(input.var[0][2] * input.var[1][1] * input.var[2][0] * input.var[3][3]) -
		(input.var[0][1] * input.var[1][0] * input.var[2][3] * input.var[3][2]) -
		(input.var[0][0] * input.var[1][3] * input.var[2][2] * input.var[3][1]);

	//求倒
	firstPart = 1 / firstPart;
	*/

	//最里层的循环
	int x, y;
	//用于标识行列式的x y
	int x_tmp, y_tmp;

	for (int lop = 0; lop < 4; lop++)
	{
		for (int lop2 = 0; lop2 < 4; lop2++)
		{
			Matrix3SetZero(&tmp);
			x_tmp = 0;
			y_tmp = 0;
			//为output矩阵的元素求值
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

MATRIX4 GetWorldToViewMatrix4(CAMERA *camera)
{
	MATRIX4 WTV;
	WTV = InvertMatrix4(MatrixMul4(Rotation(camera->rotation), Transition(camera->POS)));

	MATRIX4 hMatrix4 = { 0.0f };
	Matrix4SetZero(&hMatrix4);

	camera->POS.x = 0.0f;
	camera->POS.y = 0.0f;
	camera->POS.z = 0.0f;

	camera->rotation.x = 0.0f;
	camera->rotation.y = 0.0f;
	camera->rotation.z = 0.0f;

	return WTV;
}

//将物体的矩阵和摄像机的逆矩阵乘起来
//再对物体进行变换
void SingleObjectToViewTransform(OBJECT* object, MATRIX4 WTV)
{
	MATRIX4 ObjectToViewMatrix4;
	Matrix4SetZero(&ObjectToViewMatrix4);

	//物体坐标 -> 世界坐标
	//先缩放和旋转
	//再平移
	ObjectToViewMatrix4 = RST(Scale(1.0f), Rotation(object->rotation), Transition(object->position));
	ObjectToViewMatrix4 = MatrixMul4(ObjectToViewMatrix4, WTV);

	for (int lop = 0; lop < object->model.vertexNum; lop++)
	{
		VectorTransform(&object->model.vertexList[lop], ObjectToViewMatrix4);
	}

	//模型的中心被改变
	object->position.x = 0.0f;
	object->position.y = 0.0f;
	object->position.z = 0.0f;

	object->rotation.x = 0.0f;
	object->rotation.y = 0.0f;
	object->rotation.z = 0.0f;
}

MATRIX4 GetViewToHomoMatrix4(CAMERA *camera)
{
	MATRIX4 hMatrix4;
	Matrix4SetZero(&hMatrix4);

	float l, r, t, b;

	r = camera->NearZ*tanf(camera->FOV *0.01745f / 2.0f);
	l = -r;
	t = r / camera->aspect;
	b = -t;

	hMatrix4.var[0][0] = (2 * camera->NearZ) / (r - l);
	hMatrix4.var[1][1] = (2 * camera->NearZ) / (t - b);
	//hMatrix4.var[2][0] = (r + l) / (r - l);
	//hMatrix4.var[2][1] = (t + b) / (t - b);
	hMatrix4.var[2][2] = (camera->FarZ + camera->NearZ) / (camera->FarZ - camera->NearZ);
	hMatrix4.var[2][3] = 1.0f;
	hMatrix4.var[3][2] = -(2.0f*camera->NearZ*camera->FarZ) / (camera->FarZ - camera->NearZ);

	return hMatrix4;
}

void SingleObectFromViewToHomoTransform(OBJECT *object, MATRIX4 VTH)
{
	float z;

	for (int lop = 0; lop < object->model.vertexNum; lop++)
	{
		z = object->model.vertexList[lop].z;

		VectorTransform(&object->model.vertexList[lop], VTH);

		object->model.vertexList[lop].x /= z;
		object->model.vertexList[lop].y /= z;
		object->model.vertexList[lop].z /= z;
	}
}

int TriangleBackCull(FLOAT3D p0, FLOAT3D p1, FLOAT3D p2)
{
	//p1->p0
	//p1->p2
	FLOAT3D view = { 0.0f,0.0f,1.0f };
	FLOAT3D a, b;
	a.x = p0.x - p1.x;
	a.y = p0.y - p1.y;
	a.z = p0.z - p1.z;

	b.x = p2.x - p1.x;
	b.y = p2.y - p1.y;
	b.z = p2.z - p1.z;

	if (DotProduct(view, CrossProduct(a, b)) <= 0)
	{
		return 1;
	}
	return 0;
}