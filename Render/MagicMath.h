/*****************
*
*���ļ���������ά�ռ����ݽ��д���ĺ���
*Ŀǰ������
*
*ֱ�ߵļ��ú���
*��ά�ռ�ĵ��
*����ĳ˷�
*
*����ƽ�ƾ���
*
*������ת����
*
*�������ž���
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
//TODO


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

//���������ӿھ���
MATRIX4 GetWorldToViewMatrix4(CAMERA *);

//������ĵ������ռ�ת�����ӿڿռ�
void SingleObjectToViewTransform(OBJECT*, MATRIX4);

#endif