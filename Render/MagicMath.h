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

#include "Definitions_Header.h"

//�ú�����ֱ�߽��м���
//����ֵΪ0���滭
//����ֵΪ1�ͻ���
int LiangBarskyLineClipping(FLOAT2D *, FLOAT2D *);

//��ά�ռ�ĵ��
//��������άʸ����ɸ�����������
float DotProduct(FLOAT3D, FLOAT3D);

//�������
//3x3�׾������
MATRIX3 MatrixMul3(MATRIX3, MATRIX3);
//4x4�׾������
MATRIX4 MatrixMul4(MATRIX4, MATRIX4);

//����ƽ�ƾ���
MATRIX4 Translation(float i, float j, float k);

//������ת����
MATRIX4 Rotation(char axis, float degree);

//�������ž���
MATRIX4 Scale(float multi);

//����RS����
MATRIX4 RS(MATRIX4 S, MATRIX4 R);

//��󽫱任�����ڵ������
FLOAT3D VectorTransform(FLOAT3D src, MATRIX4 Transform);

void ObjectToWorldTransform(OBJECT *);

#endif