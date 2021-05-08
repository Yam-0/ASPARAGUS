
/* KEYDOWN STATES */
int ASPK_Right, ASPK_Left, ASPK_Down, ASPK_Up;
int ASPK_W, ASPK_A, ASPK_S, ASPK_D;
int ASPK_Q, ASPK_E;
int ASPK_SHIFT, ASPK_CTRL, ASPK_SPACE, ASPK_ESC;
int ASPK_0, ASPK_1, ASPK_2, ASPK_3, ASPK_4, ASPK_5, ASPK_6, ASPK_7, ASPK_8, ASPK_9;

/* KEYPRESS STATES */
int ASPKP_Right, ASPKP_Left, ASPKP_Down, ASPKP_Up;
int ASPKP_W, ASPKP_A, ASPKP_S, ASPKP_D;
int ASPKP_Q, ASPKP_E;
int ASPKP_SHIFT, ASPKP_CTRL, ASPKP_SPACE, ASPKP_ESC;
int ASPKP_0, ASPKP_1, ASPKP_2, ASPKP_3, ASPKP_4, ASPKP_5, ASPKP_6, ASPKP_7, ASPKP_8, ASPKP_9;

int ASP_ResetKeyStates()
{
	ASPKP_Right, ASPKP_Left, ASPKP_Down, ASPKP_Up = 0;
	ASPKP_W, ASPKP_A, ASPKP_S, ASPKP_D = 0;
	ASPKP_Q, ASPKP_E = 0;
	ASPKP_SHIFT, ASPKP_CTRL, ASPKP_SPACE, ASPKP_ESC = 0;
	ASPKP_0, ASPKP_1, ASPKP_2, ASPKP_3, ASPKP_4, ASPKP_5, ASPKP_6, ASPKP_7, ASPKP_8, ASPKP_9 = 0;
}
int ASP_SetKeyState(int key, int value)
{
	if (value != 0 && value != 1)
	{
		return 1;
	}

	switch (key)
	{
	case 1073741903:
		ASPKP_Right = (ASPKP_Right == 0 && value == 1) ? 1 : 0;
		ASPK_Right = value;
		break;
	case 1073741904:
		ASPKP_Left = (ASPKP_Left == 0 && value == 1) ? 1 : 0;
		ASPK_Left = value;
		break;
	case 1073741905:
		ASPKP_Down = (ASPKP_Down == 0 && value == 1) ? 1 : 0;
		ASPK_Down = value;
		break;
	case 1073741906:
		ASPKP_Up = (ASPKP_Up == 0 && value == 1) ? 1 : 0;
		ASPK_Up = value;
		break;
	case 119:
		ASPKP_W = (ASPKP_W == 0 && value == 1) ? 1 : 0;
		ASPK_W = value;
		break;
	case 97:
		ASPKP_A = (ASPKP_A == 0 && value == 1) ? 1 : 0;
		ASPK_A = value;
		break;
	case 115:
		ASPKP_S = (ASPKP_S == 0 && value == 1) ? 1 : 0;
		ASPK_S = value;
		break;
	case 100:
		ASPKP_D = (ASPKP_D == 0 && value == 1) ? 1 : 0;
		ASPK_D = value;
		break;
	case 113:
		ASPKP_Q = (ASPKP_Q == 0 && value == 1) ? 1 : 0;
		ASPK_Q = value;
		break;
	case 101:
		ASPKP_E = (ASPKP_E == 0 && value == 1) ? 1 : 0;
		ASPK_E = value;
		break;
	case 1073742049:
		ASPKP_SHIFT = (ASPKP_SHIFT == 0 && value == 1) ? 1 : 0;
		ASPK_SHIFT = value;
		break;
	case 1073742048:
		ASPKP_CTRL = (ASPKP_CTRL == 0 && value == 1) ? 1 : 0;
		ASPK_CTRL = value;
		break;
	case 32:
		ASPKP_SPACE = (ASPKP_SPACE == 0 && value == 1) ? 1 : 0;
		ASPK_SPACE = value;
		break;
	case 27:
		ASPKP_ESC = (ASPKP_ESC == 0 && value == 1) ? 1 : 0;
		ASPK_ESC = value;
		break;
	case 48:
		ASPKP_0 = (ASPKP_0 == 0 && value == 1) ? 1 : 0;
		ASPK_0 = value;
		break;
	case 49:
		ASPKP_1 = (ASPKP_1 == 0 && value == 1) ? 1 : 0;
		ASPK_1 = value;
		break;
	case 50:
		ASPKP_2 = (ASPKP_2 == 0 && value == 1) ? 1 : 0;
		ASPK_2 = value;
		break;
	case 51:
		ASPKP_3 = (ASPKP_3 == 0 && value == 1) ? 1 : 0;
		ASPK_3 = value;
		break;
	case 52:
		ASPKP_4 = (ASPKP_4 == 0 && value == 1) ? 1 : 0;
		ASPK_4 = value;
		break;
	case 53:
		ASPKP_5 = (ASPKP_5 == 0 && value == 1) ? 1 : 0;
		ASPK_5 = value;
		break;
	case 54:
		ASPKP_6 = (ASPKP_6 == 0 && value == 1) ? 1 : 0;
		ASPK_6 = value;
		break;
	case 55:
		ASPKP_7 = (ASPKP_7 == 0 && value == 1) ? 1 : 0;
		ASPK_7 = value;
		break;
	case 56:
		ASPKP_8 = (ASPKP_8 == 0 && value == 1) ? 1 : 0;
		ASPK_8 = value;
		break;
	case 57:
		ASPKP_9 = (ASPKP_9 == 0 && value == 1) ? 1 : 0;
		ASPK_9 = value;
		break;

	default:
		break;
	}

	return 0;
}

int ASP_KeyDownEvent(int key)
{
}
int ASP_KeyUpEvent(int key)
{
}