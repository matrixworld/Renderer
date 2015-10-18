#ifndef RENDERER_DEFINITIONS_HEADER_H
#define RENDERER_DEFINITIONS_HEADER_H

/********************************
������Ŀ�����壺
����ϵ����������ϵ
x�᣺����
y�᣺����
z�᣺����
�����ζ�����Ʒ���˳ʱ�� Ϊ�������ӵ�
**********************************/

///////////////////////
//��Ļ��С����Ⱦ�ռ��С//
//////////////////////

#define WINDOW_X (600)
#define WINDOW_Y (600)

#define RENDER_X (512)
#define RENDER_Y (512)

/////////
//�궨��//
/////////

#define OFFSET(x) ((x) + 20)
//�����ֵAbsolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//////////////
//�������ݶ���//
//////////////

//������ɫ ��ɫ
#define BGCOLOR (RGB(255,255,255))
//��ɫ
#define BLACKCOLOR (RGB(0,0,0))

//////////////
//��ѧ�����//
//////////////

//��άƽ���ڵĵ�
typedef struct _FLOAT2D
{
	int x, y;
}FLOAT2D;

#endif