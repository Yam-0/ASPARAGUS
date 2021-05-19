#ifndef ASP_VEC_H
#define ASP_VEC_H

/* Asparagus Integer Vector2 */
typedef struct
{
	int x;
	int y;
} ASP_IVector2;
/* CONSTRUCTOR */
ASP_IVector2 ASP_IVector2C(int x, int y)
{
	ASP_IVector2 vector;
	vector.x = x;
	vector.y = y;
	return vector;
}

/* Asparagus Integer Vector3 */
typedef struct
{
	int x;
	int y;
	int z;
} ASP_IVector3;
/* CONSTRUCTOR */
ASP_IVector3 ASP_IVector3C(int x, int y, int z)
{
	ASP_IVector3 vector;
	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

/* Asparagus Float Vector2 */
typedef struct
{
	float x;
	float y;
} ASP_FVector2;
/* CONSTRUCTOR */
ASP_FVector2 ASP_FVector2C(float x, float y)
{
	ASP_FVector2 vector;
	vector.x = x;
	vector.y = y;
	return vector;
}

/* Asparagus Float Vector3 */
typedef struct
{
	float x;
	float y;
	float z;
} ASP_FVector3;
/* CONSTRUCTOR */
ASP_FVector3 ASP_FVector3C(float x, float y, float z)
{
	ASP_FVector3 vector;
	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

#endif