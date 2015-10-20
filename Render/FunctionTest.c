#include "MagicMath.h"
#include "UsefulFunc.h"

#include <stdio.h>

int main()
{
	OBJECT triangularPoints;
	MODEL point3;
	point3.selfVertex[0].x = 1.0f;
	point3.selfVertex[0].y = 0.0f;
	point3.selfVertex[0].z = 0.0f;

	point3.selfVertex[1].x = 0.0f;
	point3.selfVertex[1].y = 1.0f;
	point3.selfVertex[1].z = 0.0f;

	point3.selfVertex[2].x = 0.0f;
	point3.selfVertex[2].y = 0.0f;
	point3.selfVertex[2].z = 1.0f;

	InitObject(&triangularPoints, point3, 0, 0, 1, 1, 0, 0);



	return 0;
}