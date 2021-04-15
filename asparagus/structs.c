/* Color : (R, G, B, A) 0-255 */
typedef struct
{
	int r;
	int g;
	int b;
	int a;
} ASP_Color;

/* CONSTRUCTOR */
ASP_Color ASP_ColorC(int r, int g, int b, int a)
{
	ASP_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}
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

/* Standard Asparagus Entity */
typedef struct
{
	char name[50];
	int id;
	ASP_FVector3 position;
	ASP_FVector3 rotation;
	ASP_FVector3 scale;
	int type;
} ASP_Entity;

/* CONSTRUCTOR */
ASP_Entity ASP_EntityC()
{
	ASP_Entity entity;
	return entity;
}