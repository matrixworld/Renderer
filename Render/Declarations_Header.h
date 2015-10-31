#ifndef RENDERER_DECLARATIONS_HEADER_H
#define RENDERER_DECLARATIONS_HEADER_H

/********************************
������Ŀ�����壺
����ϵ����������ϵ

�������궨�壺
x�᣺����
y�᣺����
z�᣺����

��ת���궨�壺�������෴������ȥ��˳ʱ�뷽��Ϊ �� 
x : Pitch
y : Yaw
z : Roll

���壺
�����ζ�����Ʒ���˳ʱ�� Ϊ�������ӵ�
**********************/
/////////
//�궨��//
/////////

#define OFFSET(x) ((x) + 20)
//�����ֵAbsolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//////////////
//�������ݶ���//
//////////////
//��Ӱ�촰�ڸմ���ʱ��λ�ü���С
#define RENDER_X (800)
#define RENDER_Y (600)

//������ɫ ��ɫ
#define BGCOLOR (RGB(255,255,255))
//��ɫ
#define BLACKCOLOR (RGB(0,0,0))
#define DEGREE(x) (0.01745f*(x))
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

//ģ�ͣ�����¼ģ���ϸ��������ģ�Ϳռ��λʸ
typedef struct _MODEL
{
	//������������¼������
	FLOAT3D *vertexList;
	//��¼��ĸ���
	int vertexNum;

	//��������-1����
	int *vertexListIndex;
	//��¼������ĳ���
	int indexNumber;
	//nΪ�����ε�����
	//3*n + 1
}MODEL;

//ģ��������ռ�Ľṹ��
typedef struct _OBJECT
{
	MODEL model;
	//ģ��������ռ��λʸ
	FLOAT3D position;
	//ģ�ͱ�����ת�Ķ���
	FLOAT3D rotation;
}OBJECT;

//�����
typedef struct _CAMERA
{
	//�����������ռ������
	FLOAT3D POS;
	//�����������ռ��ת���Ƕ�
	FLOAT3D rotation;
	//x -> pitch
	//y -> yaw
	//z -> roll

	//����ľ��룬��Զ��ʾ�ľ���
	float NearZ, FarZ;
	//Field of View
	int FOV;
	//��Ļ�߿��
	float screenAspect;

	//������ƶ��ٶ�
	float speed;
}CAMERA;

#endif