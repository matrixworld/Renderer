#ifndef RENDERER_RENDERER_HEADER_H
#define RENDERER_RENDERER_HEADER_H

/********************************
整个项目规则定义：
坐标系：左手坐标系
三角形顶点缠绕方向：顺时针 为正面向视点
**********************************/
#define WINDOW_X (600)
#define WINDOW_Y (600)

#define RENDER_X (512)
#define RENDER_Y (512)

//窗口大小为600x600
//只防止被标题栏重叠
//偏移一段
#define OFFSET(x) ((x) + 20)
//求绝对值Absolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//二维平面内的顶点结构体
typedef struct _FLOAT2D
{
	int x, y;
}FLOAT2D;

#endif