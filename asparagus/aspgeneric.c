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