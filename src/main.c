#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

ASP_Entity player;
float p_mspeed = 5;
float p_rspeed = 1.0f;
int iHorizontal = 0;
int iVertical = 0;

DrawEntity(ASP_Entity entity);
int ProjectAndDrawLine(ASP_FVector3 v, ASP_FVector3 w, ASP_Entity frustum);

int main(int argc, char *argv[])
{
	ASP_init(&update, &start);
	getchar();
	return 0;
}

int start()
{
	player = ASP_EntityC();
	player.position.x = 0;
	player.position.y = 0;
	player.rotation.z = PI;
	return 0;
}

int update(float deltatime)
{
	//printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, ASP_FPS);

	iHorizontal = 0;
	iVertical = 0;

	if (ASPK_D == 1)
	{
		iHorizontal += 1;
	}
	if (ASPK_A == 1)
	{
		iHorizontal -= 1;
	}
	if (ASPK_W == 1)
	{
		iVertical += 1;
	}
	if (ASPK_S == 1)
	{
		iVertical -= 1;
	}

	float p_x1 = sinf(player.rotation.z) * p_mspeed;
	float p_y1 = cosf(player.rotation.z) * p_mspeed;

	float p_x2 = sinf(player.rotation.z + (PI / 2)) * -p_mspeed;
	float p_y2 = cosf(player.rotation.z + (PI / 2)) * -p_mspeed;

	float p_vx = (p_x1 * iVertical) + (p_x2 * iHorizontal);
	float p_vy = (p_y1 * iVertical) + (p_y2 * iHorizontal);

	player.position.x += (p_vx * deltatime);
	player.position.y += (p_vy * deltatime);

	float ra = (float)(p_rspeed * deltatime);

	if (ASPK_Right == 1)
	{
		player.rotation.z -= ra;
	}
	if (ASPK_Left == 1)
	{
		player.rotation.z += ra;
	}

	DrawEntity(ASP_EntityC());

	/* MINIMAP */
	ASP_Color color = ASP_ColorC(255, 0, 0, 255);
	ASP_DrawRect(renderer, color, ASP_IVector2C(10, 10), ASP_IVector2C(100, 100));
	ASP_IVector2 ssp_bc = ASP_IVector2C(60, 60);
	ASP_DrawPixel(renderer, color, ssp_bc);
	ASP_IVector2 ssp_pp = ASP_IVector2C(ssp_bc.x + player.position.x / 10, ssp_bc.y + player.position.y / 10);
	ASP_DrawRect(renderer, color, ASP_IVector2C(ssp_pp.x - 5, ssp_pp.y - 5), ASP_IVector2C(10, 10));
	int ssp_ax = sinf(player.rotation.z) * 15;
	int ssp_ay = cosf(player.rotation.z) * 15;
	ASP_IVector2 ssp_ap = ASP_IVector2C(ssp_ax, ssp_ay);
	ASP_DrawLine(renderer, color, ASP_IVector2C(ssp_pp.x, ssp_pp.y), ASP_IVector2C(ssp_pp.x + ssp_ap.x, ssp_pp.y + ssp_ap.y));

	return 0;
}

int DrawEntity(ASP_Entity entity)
{
	ASP_FVector3 vertices[8] = {
		ASP_FVector3C(0.5f, 0.5f, -0.5f),
		ASP_FVector3C(-0.5f, 0.5f, -0.5f),
		ASP_FVector3C(-0.5f, -0.5f, -0.5f),
		ASP_FVector3C(0.5f, -0.5f, -0.5f),
		ASP_FVector3C(0.5f, 0.5f, 0.5f),
		ASP_FVector3C(-0.5f, 0.5f, 0.5f),
		ASP_FVector3C(-0.5f, -0.5f, 0.5f),
		ASP_FVector3C(0.5f, -0.5f, 0.5f)};

	int faces[12][3] =
		{0, 1, 3,
		 1, 2, 3,
		 0, 1, 4,
		 1, 4, 5,
		 1, 2, 5,
		 2, 6, 5,
		 3, 2, 6,
		 3, 7, 6,
		 0, 3, 7,
		 0, 4, 7,
		 4, 5, 6,
		 4, 6, 7};

	for (int i = 0; i < 12; i++)
	{
		int face[3] = {
			faces[i][0],
			faces[i][1],
			faces[i][2]};

		ProjectAndDrawLine(vertices[face[0]], vertices[face[1]], player);
		ProjectAndDrawLine(vertices[face[1]], vertices[face[2]], player);
		ProjectAndDrawLine(vertices[face[2]], vertices[face[0]], player);
	}

	return 0;
}

int ProjectAndDrawLine(ASP_FVector3 v, ASP_FVector3 w, ASP_Entity entity)
{
	ASP_IVector2 dcenter = ASP_IVector2C(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	ASP_Color color = ASP_ColorC(255, 0, 0, 255);
	float p_fov = PI / 2;
	float depth = PI;

	ASP_FVector3 vp = ASP_FVector3C(v.x - player.position.x, v.y - player.position.y, v.z - player.position.z);
	ASP_FVector3 wp = ASP_FVector3C(w.x - player.position.x, w.y - player.position.y, w.z - player.position.z);
	ASP_FVector3 tempvp = vp;
	ASP_FVector3 tempwp = wp;
	tempvp.x = cosf(player.rotation.z) * vp.x - sinf(player.rotation.z) * vp.y;
	tempvp.y = sinf(player.rotation.z) * vp.x + cosf(player.rotation.z) * vp.y;
	tempwp.x = cosf(player.rotation.z) * wp.x - sinf(player.rotation.z) * wp.y;
	tempwp.y = sinf(player.rotation.z) * wp.x + cosf(player.rotation.z) * wp.y;
	vp = tempvp;
	wp = tempwp;

	float vdist = sqrtf(vp.y * vp.y) + 0.1f;
	float wdist = sqrtf(wp.y * wp.y) + 0.1f;
	float vdot = DotProduct(ASP_UNIT_j_F3, v);
	float wdot = DotProduct(ASP_UNIT_j_F3, w);
	vp.x *= (depth / vdist);
	wp.x *= (depth / wdist);
	//vp.y *= (depth / vdist);
	//wp.y *= (depth / wdist);
	vp.z *= (depth / vdist);
	wp.z *= (depth / wdist);

	float vaz = atanf((float)(vp.x) / (float)(vp.y));
	float waz = atanf((float)(wp.x) / (float)(wp.y));
	float vay = atanf((float)(vp.z) / (float)(vdist));
	float way = atanf((float)(wp.z) / (float)(wdist));
	int vssx = mapf(vaz, p_fov / 2, -p_fov / 2, 0, SCREEN_WIDTH);
	int wssx = mapf(waz, p_fov / 2, -p_fov / 2, 0, SCREEN_WIDTH);
	int vssy = mapf(vay, p_fov / 2, -p_fov / 2, 0, SCREEN_HEIGHT);
	int wssy = mapf(way, p_fov / 2, -p_fov / 2, 0, SCREEN_HEIGHT);
	vssx = (vssx < 0) ? 0 : vssx;
	wssx = (wssx < 0) ? 0 : wssx;
	vssy = (vssy < 0) ? 0 : vssy;
	wssy = (wssy < 0) ? 0 : wssy;

	printf("vp.z: %f | vdist: %f\n", vp.z, vdist);

	ASP_DrawLine(renderer, color, ASP_IVector2C(vssx, vssy), ASP_IVector2C(wssx, wssy));
	ASP_DrawPixel(renderer, color, dcenter);

	return 0;
}