static void clearScreen(uint color)
{
	uint* pixel = (uint*)renderState.memory;
	for (int i = 0; i < renderState.height; i++)
	{
		for (int j = 0; j < renderState.width; j++)
		{
			*pixel++ = color;
		}
	}
}

static void drawRectInPix(int x0, int y0, int x1, int y1, unsigned int color)
{
	uint* pixel;

	x0 = clamp(0, renderState.width, x0);
	x1 = clamp(0, renderState.width, x1);
	y0 = clamp(0, renderState.height, y0);
	y1 = clamp(0, renderState.height, y1);

	for (int i = y0; i < y1; i++)
	{
		pixel = (uint*)renderState.memory + x0 + i*renderState.width;
		for (int j = x0; j < x1; j++)
		{
			*pixel++ = color;
		}
	}
}

static float renderScale = 0.01;

static void drawRect(float x, float y, float halfSize_x, float halfSize_y, uint color)
{
	x *= renderState.height * renderScale;
	y *= renderState.height * renderScale;
	halfSize_x *= renderState.height * renderScale;
	halfSize_y *= renderState.height * renderScale;

	x += renderState.width / 2;
	y += renderState.height / 2;

	//change to pixels
	int x0 = x - halfSize_x; //draw half before the center (x)
	int x1 = x + halfSize_x; //other half after the center (x)
	int y0 = y - halfSize_y;
	int y1 = y + halfSize_y;

	drawRectInPix(x0, y0, x1, y1, color);
}