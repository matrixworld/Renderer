/*****************
*
*���ļ����������ݽ��д���ĺ���
*Ŀǰ������
*
*ֱ�ߵļ��ú���
*��ά�ռ�ĵ��
*����ĳ˷�
*
********************/


#ifndef RENDERER_MATHLINE_H_
#define RENDERER_MATHLINE_H_

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
void MatrixMul3(MATRIX3 *, MATRIX3, MATRIX3);
//4x4�׾������
void MatrixMul4(MATRIX4 *, MATRIX4, MATRIX4);

#endif