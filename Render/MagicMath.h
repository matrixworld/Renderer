/*****************
*
*���ļ���������ά�ռ����ݽ��д���ĺ���
*
*�������ֿռ�/����ϵ
*1)����Ŀռ�/����ϵ
*2)����Ŀռ�/����ϵ
*3)������Ŀռ�/����ϵ �����ͼ/�ӿ� �ռ�/����ϵ
*4)��μ��ÿռ�
*��Ҫ���ڽ���͸��ͶӰ
*
*��μ��ÿռ�Ķ��壺������ DirectX ��׼��
*
*z �� -1 �� 1
*
*λ��(0,0,0)


�任˳��
���絽�ӿ�
���嵽����

�ϲ������������Ե���б任
*
********************/


#ifndef RENDERER_MATHLINE_H_
#define RENDERER_MATHLINE_H_

#include <math.h>

#include "UsefulFunc.h"
#include "Declarations_Header.h"

//�ú�����ֱ�߽��м���
//����ֵΪ0���滭
//����ֵΪ1�ͻ���
int LiangBarskyLineClipping(FLOAT2D *, FLOAT2D *);

//��ά�ռ�ĵ��
//��������άʸ����ɸ�����������
float DotProduct(FLOAT3D, FLOAT3D);

//���
FLOAT3D CrossProduct(FLOAT3D, FLOAT3D);

//�������
//3x3�׾������
MATRIX3 MatrixMul3(MATRIX3, MATRIX3);
//4x4�׾������
MATRIX4 MatrixMul4(MATRIX4, MATRIX4);

//����ƽ�ƾ���
MATRIX4 Transition(FLOAT3D);

//�������յ���ת����
MATRIX4 Rotation(FLOAT3D);

//����ֻ��һ�������ת����
MATRIX4 Rotation_SingleAxis(char axis, float degree);

//�������ž���
MATRIX4 Scale(float multi);

//����RST����
MATRIX4 RST(MATRIX4 S, MATRIX4 R,MATRIX4 T);

//��󽫱任�����ڵ������
void VectorTransform(FLOAT3D *src, MATRIX4 Transform);

//�������
MATRIX4 InvertMatrix4(MATRIX4);

//���������ӿ�
MATRIX4 GetWorldToViewMatrix4(CAMERA *);

//������ĵ������ռ�ת������μ��ÿռ�ռ�
void SingleObjectToViewTransform(OBJECT*, MATRIX4);

MATRIX4 GetViewToHomoMatrix4(CAMERA *);

void SingleObectFromViewToHomoTransform(OBJECT*, MATRIX4);

int TriangleBackCull(FLOAT3D p0, FLOAT3D p1, FLOAT3D p2);

#endif