#ifndef RENDERER_DECLARATIONS_HEADER_H
#define RENDERER_DECLARATIONS_HEADER_H

/********************************
整个项目规则定义：
坐标系：左手坐标系

世界坐标定义：
x轴：向右
y轴：向上
z轴：向里

旋转坐标定义：向主轴相反方向望去的顺时针方向为 正 
x : Pitch
y : Yaw
z : Roll

物体：
三角形顶点缠绕方向：顺时针 为正面向视点
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

//模型，仅记录模型上各个点对于模型空间的位矢
typedef struct _MODEL
{
	FLOAT3D selfVertex[3];
}MODEL;

//模型在世界空间的结构体
typedef struct _OBJECT
{
	MODEL model;
	//模型在世界空间的位矢
	FLOAT3D position;
	//模型本身旋转的度数
	FLOAT3D rotation;
}OBJECT;



//摄像机
typedef struct _CAMERA
{
	//摄像机在世界空间的坐标
	FLOAT3D POS;
	//摄像机在世界空间的转动角度
	FLOAT3D rotation;

	//最近的距离，最远显示的距离
	float NearZ, FarZ;

	//Field of View
	//v -> vertical h -> horizontal
	int FOVV, FOVH;
}CAMERA;

//Recommended:
//POS:0,0,-256

//rotation:
//pitch yaw roll

//NearZ:256
//FarZ:1000
//FOVV:90 (degree)
//FOVH:90 (degree)

#endif