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
/////////
//宏定义//
/////////

#define OFFSET(x) ((x) + 20)
//求绝对值Absolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//////////////
//常用数据定义//
//////////////
//仅影响窗口刚创建时的位置及大小
#define RENDER_X (800)
#define RENDER_Y (600)

//背景颜色 白色
#define BGCOLOR (RGB(255,255,255))
//黑色
#define BLACKCOLOR (RGB(0,0,0))
#define DEGREE(x) (0.01745f*(x))
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
	//用索引表来记录三角形
	FLOAT3D *vertexList;
	//记录点的个数
	int vertexNum;

	//索引表以-1结束
	int *vertexListIndex;
	//记录索引表的长度
	int indexNumber;
	//n为三角形的数量
	//3*n + 1
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
	//x -> pitch
	//y -> yaw
	//z -> roll

	//最近的距离，最远显示的距离
	float NearZ, FarZ;
	//Field of View
	int FOV;
	//屏幕高宽比
	float screenAspect;

	//摄像机移动速度
	float speed;
}CAMERA;

#endif