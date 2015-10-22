#ifndef RENDERER_USEFULFUNC_H_
#define RENDERER_USEFULFUNC_H_

#include "Declarations_Header.h"

//��������
void swap(float *, float *);

//��վ���
void Matrix3SetZero(MATRIX3 *);
void Matrix4SetZero(MATRIX4 *);

//����ʽ��ֵ
float Determinant(MATRIX3);

//Ϊģ����������
void IniteModelToCube22(MODEL *);

//Ϊ��������ṹ��������
void InitObject(OBJECT *, MODEL, float x, float y, float z, float i, float j, float k);

//Ϊ������ṹ����������
void InitCamera(CAMERA *c, float x, float y, float z, float pitch, float yaw, float roll, float nearz, float farz, int fovv, int fovh);

#endif