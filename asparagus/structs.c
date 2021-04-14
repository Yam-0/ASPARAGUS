/* STRUCTS */
typedef struct
{
	int r;
	int b;
	int g;
	int a;
} ASP_Color;
typedef struct
{
	int x;
	int y;
} ASP_IVector2;
typedef struct
{
	int x;
	int y;
	int z;
} ASP_IVector3;
typedef struct
{
	float x;
	float y;
} ASP_FVector2;
typedef struct
{
	float x;
	float y;
	float z;
} ASP_FVector3;
typedef struct
{
	char name[50];
	int id;
	ASP_FVector3 position;
	ASP_FVector3 rotation;
	ASP_FVector3 scale;
	int type;
} ASP_Entity;

/* CONSTRUCTORS */
ASP_Color ASP_ColorC(int _r, int _g, int _b, int _a)
{
	ASP_Color color;
	color.r = _r;
	color.g = _g;
	color.b = _b;
	color.a = _a;
	return color;
}
ASP_IVector2 ASP_IVector2C(int _x, int _y)
{
	ASP_IVector2 vector;
	vector.x = _x;
	vector.y = _y;
	return vector;
}
ASP_IVector3 ASP_IVector3C(int _x, int _y, int _z)
{
	ASP_IVector3 vector;
	vector.x = _x;
	vector.y = _y;
	vector.z = _z;
	return vector;
}
ASP_FVector2 ASP_FVector2C(float _x, float _y)
{
	ASP_FVector2 vector;
	vector.x = _x;
	vector.y = _y;
	return vector;
}
ASP_FVector3 ASP_FVector3C(float _x, float _y, float _z)
{
	ASP_FVector3 vector;
	vector.x = _x;
	vector.y = _y;
	vector.z = _z;
	return vector;
}
ASP_Entity ASP_EntityC()
{
	ASP_Entity entity;
	return entity;
}