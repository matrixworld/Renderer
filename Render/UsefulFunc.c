#include "UsefulFunc.h"

void swap(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

//清空矩阵
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

void InitModel(MODEL *input,int vertexNum)
{
	input->vertexList = (FLOAT3D*)malloc(vertexNum*sizeof(FLOAT3D));
	input->vertexNum = vertexNum;
}

void InitModelWithCube22(MODEL *input)
{
	InitModel(input, 8);

	input->vertexList[0].x = 100.0f;
	input->vertexList[0].y = 100.0f;
	input->vertexList[0].z = 100.0f;

	input->vertexList[1].x = -100.0f;
	input->vertexList[1].y = 100.0f;
	input->vertexList[1].z = 100.0f;

	input->vertexList[2].x = -100.0f;
	input->vertexList[2].y = 100.0f;
	input->vertexList[2].z = -100.0f;

	input->vertexList[3].x = 100.0f;
	input->vertexList[3].y = 100.0f;
	input->vertexList[3].z = -100.0f;

	input->vertexList[4].x = 100.0f;
	input->vertexList[4].y = -100.0f;
	input->vertexList[4].z = 100.0f;

	input->vertexList[5].x = -100.0f;
	input->vertexList[5].y = -100.0f;
	input->vertexList[5].z = 100.0f;

	input->vertexList[6].x = -100.0f;
	input->vertexList[6].y = -100.0f;
	input->vertexList[6].z = -100.0f;

	input->vertexList[7].x = 100.0f;
	input->vertexList[7].y = -100.0f;
	input->vertexList[7].z = -100.0f;

	//三角形索引表
	for (int lop = 0; lop < 40; lop++)
	{
		input->verterListIndex[lop] = -1;
	}
	input->verterListIndex[0] = 0;
	input->verterListIndex[1] = 2;
	input->verterListIndex[2] = 1;

	input->verterListIndex[3] = 0;
	input->verterListIndex[4] = 3;
	input->verterListIndex[5] = 2;

	input->verterListIndex[6] = 0;
	input->verterListIndex[7] = 4;
	input->verterListIndex[8] = 3;

	input->verterListIndex[9] = 3;
	input->verterListIndex[10] = 4;
	input->verterListIndex[11] = 7;

	input->verterListIndex[12] = 2;
	input->verterListIndex[13] = 3;
	input->verterListIndex[14] = 7;

	input->verterListIndex[15] = 2;
	input->verterListIndex[16] = 7;
	input->verterListIndex[17] = 6;

	input->verterListIndex[18] = 1;
	input->verterListIndex[19] = 2;
	input->verterListIndex[20] = 6;

	input->verterListIndex[21] = 1;
	input->verterListIndex[22] = 6;
	input->verterListIndex[23] = 5;

	input->verterListIndex[24] = 0;
	input->verterListIndex[25] = 1;
	input->verterListIndex[26] = 5;

	input->verterListIndex[27] = 0;
	input->verterListIndex[28] = 5;
	input->verterListIndex[29] = 4;

	input->verterListIndex[30] = 6;
	input->verterListIndex[31] = 7;
	input->verterListIndex[32] = 4;

	input->verterListIndex[33] = 6;
	input->verterListIndex[34] = 4;
	input->verterListIndex[35] = 5;

	input->verterListIndex[36] = -1;
}

void DeleteModel(MODEL *input)
{
	free(input->vertexList);
	input->vertexNum = 0;
}

void InitObject(OBJECT *object, float x, float y, float z, float i, float j, float k)
{
	InitModelWithCube22(&object->model);

	object->position.x = x;
	object->position.y = y;
	object->position.z = z;

	object->rotation.x = i;
	object->rotation.y = j;
	object->rotation.z = k;
}

void InitCamera(CAMERA *c, float x, float y, float z, float pitch, float yaw, float roll, float nearz, float farz, int fov,float aspect)
{
	c->POS.x = x;
	c->POS.y = y;
	c->POS.z = z;

	c->rotation.x = pitch;
	c->rotation.y = yaw;
	c->rotation.z = roll;

	c->NearZ = nearz;
	c->FarZ = farz;

	c->FOV = fov;
	c->screenAspect = aspect;
}
