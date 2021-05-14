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
	ASP_FVector3 vertices[128];
	int vertexcount;
	int faces[128][3];
	int facecount;
	ASP_Color faceColor[128];
	int type;
} ASP_Entity;

/* CONSTRUCTOR */
ASP_Entity ASP_EntityC()
{
	ASP_Entity entity;
	return entity;
}

/* Standard Asparagus Sprite */
typedef struct
{
	int w;
	int h;
	ASP_Color *pixels;
} ASP_Sprite;

/* CONSTRUCTOR */
ASP_Sprite ASP_SpriteC(int w, int h, ASP_Color *pixels)
{
	ASP_Sprite sprite;
	sprite.w = w;
	sprite.h = h;
	sprite.pixels = pixels;
	return sprite;
}

/* UNIT VECTORS */
const ASP_IVector2 ASP_UNIT_i_I2 = {1, 0};
const ASP_IVector3 ASP_UNIT_i_I3 = {1, 0, 0};
const ASP_FVector2 ASP_UNIT_i_F2 = {1.0f, 0.0f};
const ASP_FVector3 ASP_UNIT_i_F3 = {1.0f, 0.0f, 0.0f};

const ASP_IVector2 ASP_UNIT_j_I2 = {0, 1};
const ASP_IVector3 ASP_UNIT_j_I3 = {0, 1, 0};
const ASP_FVector2 ASP_UNIT_j_F2 = {0.0f, 1.0f};
const ASP_FVector3 ASP_UNIT_j_F3 = {0.0f, 1.0f, 0.0f};

const ASP_IVector2 ASP_UNIT_k_I2 = {0, 0};
const ASP_IVector3 ASP_UNIT_k_I3 = {0, 0, 1};
const ASP_FVector2 ASP_UNIT_k_F2 = {0.0f, 0.0f};
const ASP_FVector3 ASP_UNIT_k_F3 = {0.0f, 0.0f, 1.0f};

/* Vector Arithmetic */
ASP_IVector2 ASP_V_Add_I2(ASP_IVector2 vector1, ASP_IVector2 vector2)
{
	return ASP_IVector2C(vector1.x + vector2.x, vector1.y + vector2.y);
}
ASP_IVector3 ASP_V_Add_I3(ASP_IVector3 vector1, ASP_IVector3 vector2)
{
	return ASP_IVector3C(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z);
}
ASP_FVector2 ASP_V_Add_F2(ASP_FVector2 vector1, ASP_FVector2 vector2)
{
	return ASP_FVector2C(vector1.x + vector2.x, vector1.y + vector2.y);
}
ASP_FVector3 ASP_V_Add_F3(ASP_FVector3 vector1, ASP_FVector3 vector2)
{
	return ASP_FVector3C(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z);
}

ASP_IVector2 ASP_V_Sub_I2(ASP_IVector2 vector1, ASP_IVector2 vector2)
{
	return ASP_IVector2C(vector1.x - vector2.x, vector1.y - vector2.y);
}
ASP_IVector3 ASP_V_Sub_I3(ASP_IVector3 vector1, ASP_IVector3 vector2)
{
	return ASP_IVector3C(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z);
}
ASP_FVector2 ASP_V_Sub_F2(ASP_FVector2 vector1, ASP_FVector2 vector2)
{
	return ASP_FVector2C(vector1.x - vector2.x, vector1.y - vector2.y);
}
ASP_FVector3 ASP_V_Sub_F3(ASP_FVector3 vector1, ASP_FVector3 vector2)
{
	return ASP_FVector3C(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z);
}

ASP_IVector2 ASP_V_Mult_I2(ASP_IVector2 vector, int constant)
{
	return ASP_IVector2C(vector.x * constant, vector.y * constant);
}
ASP_IVector3 ASP_V_Mult_I3(ASP_IVector3 vector, int constant)
{
	return ASP_IVector3C(vector.x * constant, vector.y * constant, vector.z * constant);
}
ASP_FVector2 ASP_V_Mult_F2(ASP_FVector2 vector, float constant)
{
	return ASP_FVector2C(vector.x * constant, vector.y * constant);
}
ASP_FVector3 ASP_V_Mult_F3(ASP_FVector3 vector, float constant)
{
	return ASP_FVector3C(vector.x * constant, vector.y * constant, vector.z * constant);
}

ASP_IVector2 ASP_V_Div_I2(ASP_IVector2 vector, int constant)
{
	return ASP_IVector2C(vector.x / constant, vector.y / constant);
}
ASP_IVector3 ASP_V_Div_I3(ASP_IVector3 vector, int constant)
{
	return ASP_IVector3C(vector.x / constant, vector.y / constant, vector.z / constant);
}
ASP_FVector2 ASP_V_Div_F2(ASP_FVector2 vector, float constant)
{
	return ASP_FVector2C(vector.x / constant, vector.y / constant);
}
ASP_FVector3 ASP_V_Div_F3(ASP_FVector3 vector, float constant)
{
	return ASP_FVector3C(vector.x / constant, vector.y / constant, vector.z / constant);
}