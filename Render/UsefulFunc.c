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

void InitModel(MODEL *input,int vertexNum,int indexNum)
{
	input->vertexList = (FLOAT3D*)malloc(vertexNum*sizeof(FLOAT3D));
	input->vertexNum = vertexNum;

	input->vertexListIndex = (int*)malloc(indexNum*sizeof(int));
	input->indexNumber = indexNum;
}

void InitModelWithCube22(MODEL *input)
{
	InitModel(input, 8, 37);

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
	for (int lop = 0; lop < input->indexNumber; lop++)
	{
		input->vertexListIndex[lop] = -1;
	}
	input->vertexListIndex[0] = 0;
	input->vertexListIndex[1] = 2;
	input->vertexListIndex[2] = 1;

	input->vertexListIndex[3] = 0;
	input->vertexListIndex[4] = 3;
	input->vertexListIndex[5] = 2;

	input->vertexListIndex[6] = 0;
	input->vertexListIndex[7] = 4;
	input->vertexListIndex[8] = 3;

	input->vertexListIndex[9] = 3;
	input->vertexListIndex[10] = 4;
	input->vertexListIndex[11] = 7;

	input->vertexListIndex[12] = 2;
	input->vertexListIndex[13] = 3;
	input->vertexListIndex[14] = 7;

	input->vertexListIndex[15] = 2;
	input->vertexListIndex[16] = 7;
	input->vertexListIndex[17] = 6;

	input->vertexListIndex[18] = 1;
	input->vertexListIndex[19] = 2;
	input->vertexListIndex[20] = 6;

	input->vertexListIndex[21] = 1;
	input->vertexListIndex[22] = 6;
	input->vertexListIndex[23] = 5;

	input->vertexListIndex[24] = 0;
	input->vertexListIndex[25] = 1;
	input->vertexListIndex[26] = 5;

	input->vertexListIndex[27] = 0;
	input->vertexListIndex[28] = 5;
	input->vertexListIndex[29] = 4;

	input->vertexListIndex[30] = 6;
	input->vertexListIndex[31] = 7;
	input->vertexListIndex[32] = 4;

	input->vertexListIndex[33] = 6;
	input->vertexListIndex[34] = 4;
	input->vertexListIndex[35] = 5;

	input->vertexListIndex[36] = -1;
}
void InitModelWithSingleTriangle(MODEL *input)
{
	InitModel(input, 3, 4);
	//上点
	input->vertexList[0].x = 0.0f;
	input->vertexList[0].y = 100.0f;
	input->vertexList[0].z = 0.0f;
	//右点
	input->vertexList[1].x = 100.0f;
	input->vertexList[1].y = 0.0f;
	input->vertexList[1].z = 0.0f;
	//左点
	input->vertexList[2].x = -100.0f;
	input->vertexList[2].y = 0.0f;
	input->vertexList[2].z = 0.0f;

	input->vertexListIndex[0] = 0;
	input->vertexListIndex[1] = 1;
	input->vertexListIndex[2] = 2;
	input->vertexListIndex[3] = -1;
}

void DeleteModel(MODEL *input)
{
	free(input->vertexList);
	input->vertexList = NULL;
	input->vertexNum = 0;

	free(input->vertexListIndex);
	input->vertexListIndex = NULL;
	input->indexNumber = 0;
}

void InitObject(OBJECT *object, float x, float y, float z, float i, float j, float k)
{
	InitModelWithCube22(&object->model);
	//InitModelWithSingleTriangle(&object->model);

	object->position.x = x;
	object->position.y = y;
	object->position.z = z;

	object->rotation.x = i;
	object->rotation.y = j;
	object->rotation.z = k;
}

void InitCamera(CAMERA *c, float x, float y, float z, float pitch, float yaw, float roll, float nearz, float farz, int fov,float aspect,float spd)
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
	c->speed = spd;
}

//TODO
//BUG
int CVV(FLOAT3D a, FLOAT3D b, FLOAT3D c)
{
	if ((a.x<-1.0f || a.x > 1.0f) &&
		(a.y<-1.0f || a.y > 1.0f) &&
		(a.z<-1.0f || a.z > 1.0f) &&

		(b.x<-1.0f || b.x > 1.0f) &&
		(b.y<-1.0f || b.y > 1.0f) &&
		(b.z<-1.0f || b.z > 1.0f) &&

		(c.x<-1.0f || c.x > 1.0f) &&
		(c.y<-1.0f || c.y > 1.0f) &&
		(c.z<-1.0f || c.z > 1.0f))
	{
		return 1;
	}
	//剪裁掉该三角形
	return 0;
}

float Min3(float a, float b, float c)
{
	return min(min(a, b), c);
}
float Max3(float a, float b, float c)
{
	return max(max(a, b), c);
}