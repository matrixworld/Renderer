/*****************
*
*该文件包含对数据进行处理的函数
*目前包含：
*
*直线的剪裁函数
*三维空间的点积
*矩阵的乘法
*
********************/


#ifndef RENDERER_MATHLINE_H_
#define RENDERER_MATHLINE_H_

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
void MatrixMul3(MATRIX3 *, MATRIX3, MATRIX3);
//4x4阶矩阵相乘
void MatrixMul4(MATRIX4 *, MATRIX4, MATRIX4);

#endif