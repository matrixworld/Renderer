#ifndef RENDERER_DEFINITIONS_HEADER_H
#define RENDERER_DEFINITIONS_HEADER_H

/********************************
������Ŀ�����壺
����ϵ����������ϵ

�������궨�壺
x�᣺����
y�᣺����
z�᣺����

���壺
�����ζ�����Ʒ���˳ʱ�� Ϊ�������ӵ�
**********************************

**********************************
���ļ�����
�㣬����Ķ���
���ú궨��

��������

��������

�����

**********************/

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
	float x, y;
}FLOAT2D;
//��ά�ռ��ڵĵ�
typedef struct _FLOAT3D
{
	float x, y, z;
}FLOAT3D;
//3x3����
typedef struct _MATRIX3
{
	float var[3][3];
}MATRIX3;
//4x4����
typedef struct _MATRIX4
{
	float var[4][4];
}MATRIX4;

///////////
//���嶨��//
//////////

//�����
typedef struct _CAMERA
{
	//�����������ռ������
	FLOAT3D POS;
	//�����������ռ�ĳ���
	//z��Ϊ�����ָ�����
	FLOAT3D HEADING;

	//����ľ��룬��Զ��ʾ�ľ���
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