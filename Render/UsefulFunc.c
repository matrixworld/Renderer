#include "UsefulFunc.h"

void swap(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

void InitObject(OBJECT *object, MODEL model, float x, float y, float z, float i, float j, float k)
{
	object->model = model;
	object->position.x = x;
	object->position.y = y;
	object->position.z = z;

	object->rotation.x = i;
	object->rotation.y = j;
	object->rotation.z = k;
}

void InitCamera(CAMERA *c, float x, float y, float z, float i, float j, float k, float near, float far, int fovv, int fovh)
{
	c->POS.x = x;
	c->POS.y = y;
	c->POS.z = z;

	c->HEADING.x = i;
	c->HEADING.y = j;
	c->HEADING.z = k;

	c->NearZ = near;
	c->FarZ = far;

	c->FOVV = fovv;
	c->FOVH = fovh;
}
