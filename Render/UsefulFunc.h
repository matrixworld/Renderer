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

void InitModel(MODEL *,int);
//Ϊģ����������
void InitModelWithCube22(MODEL *);

void DeleteModel(MODEL *);

//Ϊ��������ṹ��������
void InitObject(OBJECT *, float x, float y, float z, float i, float j, float k);

//Ϊ������ṹ����������
void InitCamera(CAMERA *c, float x, float y, float z, float pitch, float yaw, float roll, float nearz, float farz, int fovv, int fovh);

#endif