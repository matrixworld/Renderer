/*****************
*
*该文件包含对三维空间数据进行处理的函数
*
*存在四种空间/坐标系
*1)物体的空间/坐标系
*2)世界的空间/坐标系
*3)摄像机的空间/坐标系 简称视图/视口 空间/坐标系
*4)齐次剪裁空间
*主要用于进行透视投影
*
*齐次剪裁空间的定义：不按照 DirectX 标准：
*
*z 从 -1 到 1
*
*位于(0,0,0)


变换顺序：
世界到视口
物体到世界

合并以上两个，对点进行变换
*
********************/


#ifndef RENDERER_MATHLINE_H_
#define RENDERER_MATHLINE_H_

#include <math.h>

#include "UsefulFunc.h"
#include "Declarations_Header.h"

//该函数将直线进行剪裁
//返回值为0不绘画
//返回值为1就画线
int LiangBarskyLineClipping(FLOAT2D *, FLOAT2D *);

//三维空间的点积
//将两个三维矢量算成浮点数并返回
float DotProduct(FLOAT3D, FLOAT3D);

//叉积
FLOAT3D CrossProduct(FLOAT3D, FLOAT3D);

//矩阵相乘
//3x3阶矩阵相乘
MATRIX3 MatrixMul3(MATRIX3, MATRIX3);
//4x4阶矩阵相乘
MATRIX4 MatrixMul4(MATRIX4, MATRIX4);

//生成平移矩阵
MATRIX4 Transition(FLOAT3D);

//生成最终的旋转矩阵
MATRIX4 Rotation(FLOAT3D);

//生成只绕一个轴的旋转矩阵
MATRIX4 Rotation_SingleAxis(char axis, float degree);

//生辰缩放矩阵
MATRIX4 Scale(float multi);

//整合RST操作
MATRIX4 RST(MATRIX4 S, MATRIX4 R,MATRIX4 T);

//最后将变换作用于点或向量
void VectorTransform(FLOAT3D *src, MATRIX4 Transform);

//求逆矩阵
MATRIX4 InvertMatrix4(MATRIX4);

//求世界至视口
MATRIX4 GetWorldToViewMatrix4(CAMERA *);

//将物体的点从物体空间转换到齐次剪裁空间空间
void SingleObjectToViewTransform(OBJECT*, MATRIX4);

MATRIX4 GetViewToHomoMatrix4(CAMERA *);

void SingleObectFromViewToHomoTransform(OBJECT*, MATRIX4);

int TriangleBackCull(FLOAT3D p0, FLOAT3D p1, FLOAT3D p2);

#endif