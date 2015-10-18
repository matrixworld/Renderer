#ifndef RENDERER_DEFINITIONS_HEADER_H
#define RENDERER_DEFINITIONS_HEADER_H

/********************************
整个项目规则定义：
坐标系：左手坐标系
x轴：向右
y轴：向上
z轴：向里
三角形顶点缠绕方向：顺时针 为正面向视点
**********************************/

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
	int x, y;
}FLOAT2D;

#endif