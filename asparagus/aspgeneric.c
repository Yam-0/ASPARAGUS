#define ASP_AXIS_X 0
#define ASP_AXIS_Y 1
#define ASP_AXIS_Z 2
#define ASP_TRUE 1
#define ASP_FALSE 0

float overflowf(float value, float bottom, float top)
{
	float number = value;

	while (number < bottom)
	{
		number += top;
	}
	while (number > top)
	{
		number -= top;
	}

	return number;
}

float mapf(float value,
		   float x1, float y1,
		   float x2, float y2)
{
	float number = x2 + (y2 - x2) * ((value - x1) / (y1 - x1));

	return number;
}

int index(int x, int y, int width)
{
	return (y * width + x);
}

int isEven(int value)
{
	int x = value;
	x = x % 2 == 0 ? x : x - 1;
	return x;
}

float clampf(float value, float min, float max)
{
	value = (value > max) ? max : value;
	value = (value < min) ? min : value;

	return value;
}

ASP_FVector3 ASP_RotateVector(float a, ASP_FVector3 vector, int axis)
{
	ASP_FVector3 tempvector = vector;

	switch (axis)
	{
	case 0: //x
		tempvector.x = vector.x;
		tempvector.z = cosf(a) * vector.z - sinf(a) * vector.y;
		tempvector.y = sinf(a) * vector.z + cosf(a) * vector.y;
		break;
	case 1: //y
		//tempvector.x = cosf(a) * vector.z - sinf(a) * vector.x;
		//tempvector.y = vector.y;
		//tempvector.z = sinf(a) * vector.z + cosf(a) * vector.x;
		break;
	case 2: //z
		tempvector.x = cosf(a) * vector.x - sinf(a) * vector.y;
		tempvector.y = sinf(a) * vector.x + cosf(a) * vector.y;
		tempvector.z = vector.z;
		break;

	default:
		break;
	}

	return tempvector;
}

ASP_FVector3 CrossProduct(ASP_FVector3 v, ASP_FVector3 w)
{
	ASP_FVector3 cp;

	cp.x = v.y * w.z - v.z * w.y;
	cp.y = v.x * w.z - v.x * w.z;
	cp.z = v.x * w.y - v.y * w.x;

	return cp;
}

float DotProduct(ASP_FVector3 v, ASP_FVector3 w)
{
	float dot;

	dot = v.x * w.x + v.y * w.y + v.z * w.z;

	return dot;
}

float *ASP_Mat4f_Add(float m1[16], float m2[16])
{
	float *mr = malloc(sizeof(float) * 16);
	for (int i = 0; i < 16; i++)
		mr[i] = m1[i] + m2[i];
	return mr;
}

float *ASP_Mat4f_Sub(float m1[16], float m2[16])
{
	float *mr = malloc(sizeof(float) * 16);
	for (int i = 0; i < 16; i++)
		mr[i] = m1[i] - m2[i];
	return mr;
}

float *ASP_Mat4f_GetRow(float m1[16], int row)
{
	float *mr = malloc(sizeof(float) * 4);
	for (int i = 0; i < 4; i++)
	{
		mr[i] = m1[(row - 1) * 4 + i];
	}
	return mr;
}

float *ASP_Mat4f_GetColumn(float m1[16], int column)
{
	float *mr = malloc(sizeof(float) * 4);
	for (int i = 0; i < 4; i++)
	{
		mr[i] = m1[i * 4 + (column - 1)];
	}
	return mr;
}

float *ASP_Mat4f_SetRow(float m1[16], int row, float m2[4])
{
	float *mr = malloc(sizeof(float) * 4);
	for (int i = 0; i < 4; i++)
	{
		mr[(row - 1) * 4 + i] = m2[i];
	}
	return mr;
}

float *ASP_Mat4f_SetColumn(float m1[16], int column, float m2[4])
{
	float *mr = malloc(sizeof(float) * 4);
	for (int i = 0; i < 4; i++)
	{
		mr[i * 4 + (column - 1)] = m2[i];
	}
	return mr;
}

float *ASP_Mat4f_Transpose(float m1[16])
{
	float *mr = malloc(sizeof(float) * 4);

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			mr[y * 4 + x] = m1[x * 4 + y];
		}
	}

	return mr;
}

float *ASP_Mat4f_Mult(float m1[16], float m2[16])
{
	float *mr = malloc(sizeof(float) * 16);
	float *mcolumns = malloc(sizeof(float) * 4);
	float *mrows = malloc(sizeof(float) * 4);

	for (int x = 0; x < 4; x++)
	{
		mcolumns = ASP_Mat4f_GetColumn(m2, x + 1);

		for (int y = 0; y < 4; y++)
		{
			mrows = ASP_Mat4f_GetRow(m1, y + 1);

			for (int i = 0; i < 4; i++)
			{
				mr[y * 4 + x] += mcolumns[i] * mrows[i];
			}

			free(mrows);
		}

		free(mcolumns);
	}

	return mr;
}

float *ASP_Mat4f_Mult_C(float m1[16], float c)
{
	float *mr = malloc(sizeof(float) * 16);
	for (int i = 0; i < 16; i++)
		mr[i] = m1[i] * c;
	return mr;
}

float *ASP_Mat4f_GetTransformMatrix(ASP_FVector3 m)
{
	float *sm = malloc(sizeof(float) * 16);

	for (int i = 0; i < 16; i++)
	{
		sm[i] = ASP_MAT4F_I[i];
	}

	sm[3] = m.x;
	sm[7] = m.y;
	sm[11] = m.z;

	return sm;
}

float *ASP_Mat4f_GetRotationMatrix(int axis, float a)
{
	float *rm = malloc(sizeof(float) * 16);

	for (int i = 0; i < 16; i++)
	{
		rm[i] = ASP_MAT4F_I[i];
	}

	switch (axis)
	{
	case 0: //x
		rm[5] = cosf(a);
		rm[6] = -sinf(a);
		rm[9] = sinf(a);
		rm[10] = cos(a);
		break;
	case 1: //y
		rm[0] = cosf(a);
		rm[2] = sinf(a);
		rm[8] = -sinf(a);
		rm[10] = cos(a);
		break;
	case 2: //z
		rm[0] = cosf(a);
		rm[1] = -sinf(a);
		rm[4] = sinf(a);
		rm[5] = cos(a);
		break;

	default:
		break;
	}

	return rm;
}

float *ASP_Mat4f_GetOrientationMatrix(ASP_Entity entity, int inverted)
{
	inverted = (inverted > 1 || inverted < 0) ? 0 : inverted;
	float *om = malloc(sizeof(float) * 16);
	for (int i = 0; i < 16; i++)
	{
		om[i] = ASP_MAT4F_I[i];
	}

	float *rmx = malloc(sizeof(float) * 16);
	float *rmy = malloc(sizeof(float) * 16);
	float *rmz = malloc(sizeof(float) * 16);

	rmx = ASP_Mat4f_GetRotationMatrix(ASP_AXIS_X, -entity.rotation.x * (2 * inverted - 1));
	rmy = ASP_Mat4f_GetRotationMatrix(ASP_AXIS_Y, -entity.rotation.y * (2 * inverted - 1));
	rmz = ASP_Mat4f_GetRotationMatrix(ASP_AXIS_Z, -entity.rotation.z * (2 * inverted - 1));
	om = ASP_Mat4f_Mult(rmx, rmy);
	om = ASP_Mat4f_Mult(om, rmz);
	free(rmx);
	free(rmy);
	free(rmz);

	return om;
}

float *ASP_Mat4f_GetScaleMatrix(ASP_FVector3 m)
{
	float *sm = malloc(sizeof(float) * 16);

	for (int i = 0; i < 16; i++)
	{
		sm[i] = ASP_MAT4F_I[i];
	}

	sm[0] = m.x;
	sm[5] = m.y;
	sm[10] = m.z;

	return sm;
}

float *ASP_Mat4f_Rotate(float m1[16], float a, int axis)
{
	float *rm = malloc(sizeof(float) * 16);
	float *m2 = malloc(sizeof(float) * 16);
	rm = ASP_Mat4f_GetRotationMatrix(axis, a);
	m2 = ASP_Mat4f_Mult(rm, m1);
	free(rm);

	return m2;
}

float *ASP_Mat4f_GetModelMatrix(ASP_Entity entity)
{
	float *mm = malloc(sizeof(float) * 16);
	float *om = malloc(sizeof(float) * 16);
	float *sm = malloc(sizeof(float) * 16);
	float *tm = malloc(sizeof(float) * 16);

	sm = ASP_Mat4f_GetScaleMatrix(entity.scale);
	om = ASP_Mat4f_GetOrientationMatrix(entity, ASP_FALSE);
	tm = ASP_Mat4f_GetTransformMatrix(entity.position);

	mm = ASP_Mat4f_Mult(sm, om);
	mm = ASP_Mat4f_Mult(mm, tm);

	free(om);
	free(sm);
	free(tm);

	return mm;
}

float *ASP_Mat4f_GetViewMatrix(ASP_Entity camera)
{
	float *vm = malloc(sizeof(float) * 16);
	float *om = malloc(sizeof(float) * 16);
	float *tm = malloc(sizeof(float) * 16);

	tm = ASP_Mat4f_GetTransformMatrix(ASP_FVector3C(camera.position.x, camera.position.y, camera.position.z));
	om = ASP_Mat4f_GetOrientationMatrix(camera, ASP_TRUE);

	vm = ASP_Mat4f_Mult(tm, om);

	free(tm);

	return vm;
}

float *ASP_Mat4f_GetProjectionMatrix(float fov)
{
	float *pm = malloc(sizeof(float) * 16);
	for (int i = 0; i < 16; i++)
	{
		pm[i] = ASP_MAT4F_I[i];
	}

	/*
	float nearCP = 0.2f;
	float farCP = 1.0f;

	float topCP = nearCP * tanf(fov / 2);
	float bottomCP = -topCP;
	float rightCP = topCP * (16 / 9);
	float leftCP = -rightCP;

	pm[0] = 2 / (rightCP - leftCP);
	pm[2] = (rightCP + leftCP) / (rightCP - leftCP);
	pm[5] = (2 * nearCP) / (topCP - bottomCP);
	pm[6] = (topCP + bottomCP) / (topCP - bottomCP);
	pm[10] = -((farCP + nearCP) / (farCP - nearCP));
	pm[10] = -((2 * farCP * nearCP) / (farCP - nearCP));
	pm[14] = -1;
	*/

	return pm;
}

int ASP_Mat4f_Print(float *m1)
{
	printf("Matrix:");
	for (int x = 0; x < 4; x++)
	{
		printf("\n");
		for (int y = 0; y < 4; y++)
		{
			printf("%f ", m1[index(y, x, 4)]);
		}
	}
	printf("\n");
}