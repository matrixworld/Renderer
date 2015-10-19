#ifndef RENDERER_DEFINITIONS_HEADER_H
#define RENDERER_DEFINITIONS_HEADER_H

/********************************
整个项目规则定义：
坐标系：左手坐标系

世界坐标定义：
x轴：向右
y轴：向上
z轴：向里

物体：
三角形顶点缠绕方向：顺时针 为正面向视点
**********************************

**********************************
本文件包含
点，矩阵的定义
常用宏定义

世界坐标

物体坐标

摄像机

**********************/

///////////////////////
//屏幕大小及渲染空间大小//
//////////////////////

#define WINDOW_X (600)
#define WINDOW_Y (600)

#define RENDER_X (512)
#define RENDER_Y (512)

/////////
//宏定义//
/////////

#define OFFSET(x) ((x) + 20)
//求绝对值Absolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//////////////
//常用数据定义//
//////////////

//背景颜色 白色
#define BGCOLOR (RGB(255,255,255))
//黑色
#define BLACKCOLOR (RGB(0,0,0))

//////////////
//数学概念定义//
//////////////

//二维平面内的点
typedef struct _FLOAT2D
{
	float x, y;
}FLOAT2D;
//三维空间内的点
typedef struct _FLOAT3D
{
	float x, y, z;
}FLOAT3D;
//3x3矩阵
typedef struct _MATRIX3
{
	float var[3][3];
}MATRIX3;
//4x4矩阵
typedef struct _MATRIX4
{
	float var[4][4];
}MATRIX4;

///////////
//物体定义//
//////////

//摄像机
typedef struct _CAMERA
{
	//摄像机在世界空间的坐标
	FLOAT3D POS;
	//摄像机在世界空间的朝向
	//z轴为摄像机指向的轴
	FLOAT3D HEADING;

	//最近的距离，最远显示的距离
	float NearZ, FarZ;

	//Field of View
	//v -> vertical h -> horizontal
	int FOVV, FOVH;
}CAMERA;

//Recommended:
//POS:0,0,-256
//HEADING:0,0,1
//NearZ:256
//FarZ:1000
//FOVV:90 (degree)
//FOVH:90 (degree)

#endif