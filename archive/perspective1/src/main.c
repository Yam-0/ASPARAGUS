#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

ASP_Entity player;

int main(int argc, char *argv[])
{
	ASP_init(&update, &start);
	getchar();
	return 0;
}

int start()
{
	printf("Starting update loop!\n");
	player = ASP_EntityC();
	player.position.x = 0;
	player.position.y = 0;
	player.rotation.z = PI;
	return 0;
}

int update(float deltatime)
{
	if (deltatime <= 0.001f)
	{
		deltatime = 0.001f;
	}

	int fps = 1.0f / deltatime;
	//printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, fps);

	float p_mspeed = 75;
	float p_rspeed = 2;
	int iHorizontal = 0;
	int iVertical = 0;
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

	if (ASPK_Right == 1)
	{
		player.rotation.z -= p_rspeed * deltatime;
	}
	if (ASPK_Left == 1)
	{
		player.rotation.z += p_rspeed * deltatime;
	}

	int boxSize = (SCREEN_WIDTH - 60) / 3;
	int xOffset = (SCREEN_WIDTH - boxSize * 3) / 2;
	ASP_IVector2 boxScale = ASP_IVector2C(boxSize, boxSize);
	int box1posx = (boxSize * 0) + xOffset;
	int box2posx = (boxSize * 1) + xOffset;
	int box3posx = (boxSize * 2) + xOffset;

	/* TEMPORARY COLISION */
	if (player.position.x > boxSize / 2 - 5)
	{
		player.position.x = boxSize / 2 - 5;
	}
	if (player.position.x < -boxSize / 2 + 5)
	{
		player.position.x = -boxSize / 2 + 5;
	}
	if (player.position.y > boxSize / 2 - 5)
	{
		player.position.y = boxSize / 2 - 5;
	}
	if (player.position.y < -boxSize / 2 + 5)
	{
		player.position.y = -boxSize / 2 + 5;
	}

	/* SCENE SQUARES */
	ASP_Color RED = ASP_ColorC(255, 0, 0, 255);
	ASP_Color BLACK = ASP_ColorC(0, 0, 0, 255);
	ASP_Color GRAY = ASP_ColorC(10, 10, 10, 255);
	ASP_DrawRect(renderer, RED, ASP_IVector2C(box1posx, SCREEN_HEIGHT / 2 - boxSize / 2), boxScale);
	ASP_DrawRect(renderer, RED, ASP_IVector2C(box2posx, SCREEN_HEIGHT / 2 - boxSize / 2), boxScale);
	ASP_DrawRect(renderer, RED, ASP_IVector2C(box3posx, SCREEN_HEIGHT / 2 - boxSize / 2), boxScale);

	/* WALL POINTS*/
	ASP_IVector2 w_p1 = ASP_IVector2C(-50, -40);
	ASP_IVector2 w_p2 = ASP_IVector2C(50, -30);

	/* FIRST SQUARE*/
	ASP_IVector2 box1center = ASP_IVector2C(box1posx + boxSize / 2, SCREEN_HEIGHT / 2);
	ASP_IVector2 box1playerpos = ASP_IVector2C(box1center.x + player.position.x - 5, box1center.y + player.position.y - 5);
	ASP_DrawRect(renderer, RED, box1playerpos, ASP_IVector2C(10, 10));
	ASP_IVector2 box1wp1pos = ASP_IVector2C(box1center.x + w_p1.x, box1center.y + w_p1.y);
	ASP_IVector2 box1wp2pos = ASP_IVector2C(box1center.x + w_p2.x, box1center.y + w_p2.y);
	ASP_DrawLine(renderer, RED, box1wp1pos, box1wp2pos);
	int box1arrowx = sinf(player.rotation.z) * 15;
	int box1arrowy = cosf(player.rotation.z) * 15;
	ASP_IVector2 box1arrowpos = ASP_IVector2C(box1arrowx, box1arrowy);
	ASP_DrawLine(renderer, RED, ASP_IVector2C(box1playerpos.x + 5, box1playerpos.y + 5), ASP_IVector2C(box1playerpos.x + box1arrowpos.x + 5, box1playerpos.y + box1arrowpos.y + 5));

	/* SECOND SQUARE*/
	ASP_IVector2 box2center = ASP_IVector2C(box2posx + boxSize / 2, SCREEN_HEIGHT / 2);
	ASP_IVector2 box2playerpos = ASP_IVector2C(box2center.x - 5, box2center.y - 5);
	ASP_DrawRect(renderer, RED, box2playerpos, ASP_IVector2C(10, 10));

	ASP_IVector2 box2wp1pos = ASP_IVector2C(w_p1.x - player.position.x, w_p1.y - player.position.y);
	ASP_IVector2 box2wp2pos = ASP_IVector2C(w_p2.x - player.position.x, w_p2.y - player.position.y);

	ASP_IVector2 box2wp1postmp;
	ASP_IVector2 box2wp2postmp;

	box2wp1postmp.x = cosf(PI + player.rotation.z) * box2wp1pos.x - sinf(PI + player.rotation.z) * box2wp1pos.y;
	box2wp1postmp.y = sinf(PI + player.rotation.z) * box2wp1pos.x + cosf(PI + player.rotation.z) * box2wp1pos.y;
	box2wp2postmp.x = cosf(PI + player.rotation.z) * box2wp2pos.x - sinf(PI + player.rotation.z) * box2wp2pos.y;
	box2wp2postmp.y = sinf(PI + player.rotation.z) * box2wp2pos.x + cosf(PI + player.rotation.z) * box2wp2pos.y;

	int l_dl = 0;
	float l_length = sqrtf(box2wp1postmp.x * box2wp1postmp.x + box2wp1postmp.y * box2wp1postmp.y);
	float l_tr = (float)(box2wp1postmp.y - box2wp2postmp.y) / (float)(box2wp1postmp.x - box2wp2postmp.x);
	float l_angle = atanf(l_tr);

	box2wp1pos.x = box2wp1postmp.x + box2center.x;
	box2wp1pos.y = box2wp1postmp.y + box2center.y;
	box2wp2pos.x = box2wp2postmp.x + box2center.x;
	box2wp2pos.y = box2wp2postmp.y + box2center.y;

	ASP_DrawLine(renderer, RED, box2wp1pos, box2wp2pos);
	ASP_DrawLine(renderer, RED, ASP_IVector2C(box2center.x, box2center.y), ASP_IVector2C(box2center.x, box2center.y - 15));
	//ASP_DrawLine(renderer, GRAY, ASP_IVector2C(box2center.x, box2center.y), ASP_IVector2C(box2center.x - 65, box2center.y - 65));
	//ASP_DrawLine(renderer, GRAY, ASP_IVector2C(box2center.x, box2center.y), ASP_IVector2C(box2center.x + 65, box2center.y - 65));

	/* THIRD SQUARE */
	ASP_IVector2 box3center = ASP_IVector2C(box3posx + boxSize / 2, SCREEN_HEIGHT / 2);
	ASP_DrawPixel(renderer, RED, box3center);

	ASP_IVector2 box3wp1pos = ASP_IVector2C(w_p1.x - player.position.x, w_p1.y - player.position.y);
	ASP_IVector2 box3wp2pos = ASP_IVector2C(w_p2.x - player.position.x, w_p2.y - player.position.y);

	float p_fov = PI / 2;

	ASP_IVector2 box3wp1postmp;
	ASP_IVector2 box3wp2postmp;

	box3wp1postmp.x = cosf(PI + player.rotation.z) * box3wp1pos.x - sinf(PI + player.rotation.z) * box3wp1pos.y;
	box3wp1postmp.y = sinf(PI + player.rotation.z) * box3wp1pos.x + cosf(PI + player.rotation.z) * box3wp1pos.y;
	box3wp2postmp.x = cosf(PI + player.rotation.z) * box3wp2pos.x - sinf(PI + player.rotation.z) * box3wp2pos.y;
	box3wp2postmp.y = sinf(PI + player.rotation.z) * box3wp2pos.x + cosf(PI + player.rotation.z) * box3wp2pos.y;

	box3wp1pos = box3wp1postmp;
	box3wp2pos = box3wp2postmp;

	if (box3wp1pos.y >= 0)
	{
		box3wp1pos.y = -1;
	}
	if (box3wp2pos.y >= 0)
	{
		box3wp2pos.y = -1;
	}

	ASP_IVector2 w_p1_t;
	ASP_IVector2 w_p1_b;
	ASP_IVector2 w_p2_t;
	ASP_IVector2 w_p2_b;

	float l_w_p1 = sqrtf(box3wp1pos.x * box3wp1pos.x + box3wp1pos.y * box3wp1pos.y) + 1;
	float l_w_p2 = sqrtf(box3wp2pos.x * box3wp2pos.x + box3wp2pos.y * box3wp2pos.y) + 1;

	float a_w_p1 = atanf((float)(box3wp1pos.x) / (float)(box3wp1pos.y));
	float a_w_p2 = atanf((float)(box3wp2pos.x) / (float)(box3wp2pos.y));

	int inview = 0;
	if (box3wp1postmp.y <= 0 || box3wp2postmp.y <= 0)
	{
		inview = 1;
	}

	int w_p1_h = (400) / sqrtf(l_w_p2);
	int w_p2_h = (400) / sqrtf(l_w_p1);

	w_p1_t.y = box3center.y - (w_p1_h / 2);
	w_p1_b.y = box3center.y + (w_p1_h / 2);
	w_p2_t.y = box3center.y - (w_p2_h / 2);
	w_p2_b.y = box3center.y + (w_p2_h / 2);

	w_p1_t.x = box3center.x + mapf(a_w_p1, -p_fov, p_fov, -boxScale.x / 2, boxScale.x / 2);
	w_p2_t.x = box3center.x + mapf(a_w_p2, -p_fov, p_fov, -boxScale.x / 2, boxScale.x / 2);

	//printf("Angles: (w_p1: %f, w_p2: %f) \n", a_w_p1 * (180 / PI), a_w_p2 * (180 / PI));
	//w_p1_t.x = box3center.x + -50;
	//w_p2_t.x = box3center.x + 50;

	w_p1_b.x = w_p1_t.x;
	w_p2_b.x = w_p2_t.x;

	if (inview == 1)
	{
		ASP_DrawLine(renderer, RED, w_p1_t, w_p2_t);
		ASP_DrawLine(renderer, RED, w_p2_t, w_p2_b);
		ASP_DrawLine(renderer, RED, w_p2_b, w_p1_b);
		ASP_DrawLine(renderer, RED, w_p1_b, w_p1_t);
	}

	return 0;
}
