#ifndef RENDERER_USEFULFUNC_H_
#define RENDERER_USEFULFUNC_H_

#include "Definitions_Header.h"

//��������
void swap(float *, float *);

//Ϊ������ṹ����������
void InitCamera(CAMERA *c, float x, float y, float z, float i, float j, float k, float near, float far, int fovv, int fovh);

#endif