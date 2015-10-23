#ifndef RENDERER_USEFULFUNC_H_
#define RENDERER_USEFULFUNC_H_

#include "Declarations_Header.h"
#include <stdlib.h>

//交换数据
void swap(float *, float *);

//清空矩阵
void Matrix3SetZero(MATRIX3 *);
void Matrix4SetZero(MATRIX4 *);

//行列式求值
float Determinant(MATRIX3);

void InitModel(MODEL *,int);
//为模型填入数据
void InitModelWithCube22(MODEL *);

void DeleteModel(MODEL *);

//为世界物体结构填入数据
void InitObject(OBJECT *, float x, float y, float z, float i, float j, float k);

//为摄像机结构体填入数据
void InitCamera(CAMERA *c, float x, float y, float z, float pitch, float yaw, float roll, float nearz, float farz, int fovv, int fovh);

#endif