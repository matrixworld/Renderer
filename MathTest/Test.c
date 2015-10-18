#include <stdio.h>
#include "C:\Users\jeff\Documents\Visual Studio 2013\Projects\Renderer\Render\MagicMath.h"


int main()
{
	MATRIX3 a = { 1, 0, 0,
				  0, 1, 0,
				  0, 0, 1 };
	MATRIX3 b = { 1, 1, 0,
		0, 1, 0,
		0, 1, 1 };
	MATRIX3 ans;
	MatrixMul3(&ans, a, b);
	for (int lop = 0; lop < 3; lop++)
	{
		for (int lop2 = 0; lop2 < 3; lop2++)
		{
			printf("%0.f ", ans.var[lop][lop2]);
		}
		printf("\n");
	}
	return 0;
}