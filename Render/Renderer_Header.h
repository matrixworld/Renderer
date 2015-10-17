#ifndef RENDERER_RENDERER_HEADER_H
#define RENDERER_RENDERER_HEADER_H

/********************************
������Ŀ�����壺
����ϵ����������ϵ
�����ζ�����Ʒ���˳ʱ�� Ϊ�������ӵ�
**********************************/
#define WINDOW_X (600)
#define WINDOW_Y (600)

#define RENDER_X (512)
#define RENDER_Y (512)

//���ڴ�СΪ600x600
//ֻ��ֹ���������ص�
//ƫ��һ��
#define OFFSET(x) ((x) + 20)
//�����ֵAbsolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//��άƽ���ڵĶ���ṹ��
typedef struct _FLOAT2D
{
	int x, y;
}FLOAT2D;

#endif