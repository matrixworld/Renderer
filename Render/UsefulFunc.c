#include "UsefulFunc.h"

void swap(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

//Çå¿Õ¾ØÕó
void Matrix3SetZero(MATRIX3 *input)
{
	for (int lop = 0; lop < 3; lop++)
	{
		for (int lop2 = 0; lop2 < 3; lop2++)
		{
			input->var[lop][lop2] = 0.0f;
		}
	}
}
void Matrix4SetZero(MATRIX4 *input)
{
	for (int lop = 0; lop < 4; lop++)
	{
		for (int lop2 = 0; lop2 < 4; lop2++)
		{
			input->var[lop][lop2] = 0.0f;
		}
	}
}

float Determinant(MATRIX3 input)
{
	float output = 0.1f;
	output =
		(input.var[0][0] * input.var[1][1] * input.var[2][2]) +
		(input.var[0][1] * input.var[1][2] * input.var[2][0]) +
		(input.var[0][2] * input.var[1][0] * input.var[2][1]) -
		(input.var[0][2] * input.var[1][1] * input.var[2][0]) -
		(input.var[0][1] * input.var[1][0] * input.var[2][2]) -
		(input.var[0][0] * input.var[1][2] * input.var[2][1]);

	return output;
}

void IniteModelToCube22(MODEL *input)
{
	input->selfVertex[0].x = 100.0f;
	input->selfVertex[0].y = 100.0f;
	input->selfVertex[0].z = 100.0f;

	input->selfVertex[1].x = -100.0f;
	input->selfVertex[1].y = 100.0f;
	input->selfVertex[1].z = 100.0f;

	input->selfVertex[2].x = -100.0f;
	input->selfVertex[2].y = 100.0f;
	input->selfVertex[2].z = -100.0f;

	input->selfVertex[3].x = 100.0f;
	input->selfVertex[3].y = 100.0f;
	input->selfVertex[3].z = -100.0f;

	input->selfVertex[4].x = 100.0f;
	input->selfVertex[4].y = -100.0f;
	input->selfVertex[4].z = 100.0f;

	input->selfVertex[5].x = -100.0f;
	input->selfVertex[5].y = -100.0f;
	input->selfVertex[5].z = 100.0f;

	input->selfVertex[6].x = -100.0f;
	input->selfVertex[6].y = -100.0f;
	input->selfVertex[6].z = -100.0f;

	input->selfVertex[7].x = 100.0f;
	input->selfVertex[7].y = -100.0f;
	input->selfVertex[7].z = -100.0f;
}

void InitObject(OBJECT *object, MODEL model, float x, float y, float z, float i, float j, float k)
{
	object->model = model;
	object->position.x = x;
	object->position.y = y;
	object->position.z = z;

	object->rotation.x = i;
	object->rotation.y = j;
	object->rotation.z = k;
}

void InitCamera(CAMERA *c, float x, float y, float z, float pitch, float yaw, float roll, float nearz, float farz, int fovv, int fovh)
{
	c->POS.x = x;
	c->POS.y = y;
	c->POS.z = z;

	c->rotation.x = pitch;
	c->rotation.y = yaw;
	c->rotation.z = roll;

	c->NearZ = nearz;
	c->FarZ = farz;

	c->FOVV = fovv;
	c->FOVH = fovh;
}
