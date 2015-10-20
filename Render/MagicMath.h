/*****************
*
*该文件包含对三维空间数据进行处理的函数
*目前包含：
*
*直线的剪裁函数
*三维空间的点积
*矩阵的乘法
*
*生成平移矩阵
*
*生成旋转矩阵
*
*生成缩放矩阵
*
********************/


#ifndef RENDERER_MATHLINE_H_
#define RENDERER_MATHLINE_H_

#include <math.h>

#include "Definitions_Header.h"

//该函数将直线进行剪裁
//返回值为0不绘画
//返回值为1就画线
int LiangBarskyLineClipping(FLOAT2D *, FLOAT2D *);

//三维空间的点积
//将两个三维矢量算成浮点数并返回
float DotProduct(FLOAT3D, FLOAT3D);

//矩阵相乘
//3x3阶矩阵相乘
MATRIX3 MatrixMul3(MATRIX3, MATRIX3);
//4x4阶矩阵相乘
MATRIX4 MatrixMul4(MATRIX4, MATRIX4);

//生成平移矩阵
MATRIX4 Translation(float i, float j, float k);

//生成旋转矩阵
MATRIX4 Rotation(char axis, float degree);

//生辰缩放矩阵
MATRIX4 Scale(float multi);

//整合RS操作
MATRIX4 RS(MATRIX4 S, MATRIX4 R);

//最后将变换作用于点或向量
FLOAT3D VectorTransform(FLOAT3D src, MATRIX4 Transform);

void ObjectToWorldTransform(OBJECT *);

#endif