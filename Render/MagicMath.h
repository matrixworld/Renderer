/*****************
*
*该文件包含对数据进行处理的函数
*目前包含：
*
*直线的剪裁函数
*
********************/


#ifndef RENDERER_MATHLINE_H_
#define RENDERER_MATHLINE_H_

#include "Definitions_Header.h"

//该函数将直线进行剪裁
//返回值为0不绘画
//返回值为1就画线
int LiangBarskyLineClipping(FLOAT2D *, FLOAT2D *);

#endif