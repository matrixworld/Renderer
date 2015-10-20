#ifndef RENDERER_USEFULFUNC_H_
#define RENDERER_USEFULFUNC_H_

#include "Definitions_Header.h"

//交换数据
void swap(float *, float *);

//为世界物体结构填入数据
void InitObject(OBJECT *, MODEL, float x, float y, float z, float i, float j, float k);

//为摄像机结构体填入数据
void InitCamera(CAMERA *c, float x, float y, float z, float i, float j, float k, float near, float far, int fovv, int fovh);

#endif