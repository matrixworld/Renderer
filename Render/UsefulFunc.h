#ifndef RENDERER_USEFULFUNC_H_
#define RENDERER_USEFULFUNC_H_

#include "Declarations_Header.h"
#include <stdlib.h>

//��������
void swap(float *, float *);

//��վ���
void Matrix3SetZero(MATRIX3 *);
void Matrix4SetZero(MATRIX4 *);

//����ʽ��ֵ
float Determinant(MATRIX3);

//Ϊģ�����봢��ռ�
void InitModel(MODEL *, int, int);

//Ϊģ����������
//void InitModelWithCube22(MODEL *);
void InitModelWithSingleTriangle01(MODEL *);
void InitModelWithSingleTriangle02(MODEL *);

void DeleteModel(MODEL *);

//Ϊ��������ṹ��������
void InitObject(OBJECT *, float x, float y, float z, float i, float j, float k, int);

//Ϊ������ṹ����������
void InitCamera(CAMERA *c, float x, float y, float z, float pitch, float yaw, float roll, float nearz, float farz, int fov,float aspect,float speed);

int CVV(FLOAT3D, FLOAT3D, FLOAT3D);

float Min3(float a, float b, float c);
float Max3(float a, float b, float c);
#endif