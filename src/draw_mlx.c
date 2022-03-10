#include "cluster.h"
#include <math.h>


static uint32_t ColorFromRGB(unsigned char r, unsigned char g, unsigned char b)
{
	return (r << 24) | (g << 16) | (b << 8) | 0xFF;
}

static float fract(float n)
{
	return n - floorf(n);
}

static float hexagon(float x, float y) {
    x /= sqrtf(3);
	y /= 1.5f;

    y -= 0.5f;
    x -= fract(floorf(y) * 0.5f);

    x = fabsf(fract(x) - 0.5f);
	y = fabsf(fract(y) - 0.5f);
    return fabsf(1 - fmaxf(x + y * 1.5f, x * 2)) * sqrtf(3) * 0.5f;
}

static uint32_t HexDistanceToColor(float d, t_axial hex)
{
	if (d < 0)	// Background
		return ColorFromRGB(0, 0, 0);
	else if (d < 0.05f)	// Edge
		return ColorFromRGB(255, 255, 255);

	t_hex piece = gameData.gameGrid[hex.q][hex.r];

	unsigned char v = (unsigned char)(d * 255);

	// TODO: Use something better
	switch (piece)
	{
		case 1:	// BLUE
			return ColorFromRGB(v, v, 255);
		case 2:	// CYAN
			return ColorFromRGB(v, 255, 255);
		case 3:	// RED
			return ColorFromRGB(255, v, v);
		case 4:	// ORANGE
			return ColorFromRGB(255, 255, v);
		default:
			return ColorFromRGB(v, v, v);
	}
}

static t_axial axial_round(float fq, float fr)
{
	float fs = -fq-fr;

    float q = roundf(fq);
    float r = roundf(fr);
    float s = roundf(fs);

    float q_diff = fabsf(q - fq);
    float r_diff = fabsf(r - fr);
    float s_diff = fabsf(s - fs);

    if (q_diff > r_diff && q_diff > s_diff)
        q = -r-s;
    else if (r_diff > s_diff)
        r = -q-s;

    return (t_axial) { (int)q, (int)r };
}

static void pointy_hex_to_pixel(t_axial hex, float* x, float *y)
{
    *x = sqrtf(3) * hex.q  +  sqrtf(3)/2 * hex.r;
    *y =                            3./2 * hex.r;
}

static t_axial pixel_to_pointy_hex(float x, float y)
{
    float q = (sqrt(3)/3 * x  -  1./3 * y);
    float r = (                  2./3 * y);
    return axial_round(q, r);
}

void DrawHexagons(mlx_image_t* image, float angle)
{
	float ox, oy;
	pointy_hex_to_pixel((t_axial){gameData.maxLine / 2, gameData.maxLine / 2}, &ox, &oy);

	// We need to display gameData.maxLine tiles at most, for a with of image->width, thus the scale should be...
	float scale = gameData.maxLine / (float)image->width * 2; // mult by 2 for good measure (it was too big!)

	// construct a 2x2 matrix
	float xx = cosf(angle);
	float xy = sinf(angle);
	float yx = -sinf(angle);
	float yy = cosf(angle);

	for (int x = 0; x < image->width; x++)
		for (int y = 0; y < image->height; y++)
		{
			// Center coords
			float px = x-image->width/2.0f;
			float py = y-image->height/2.0f;

			// Rotate coords
			float rx = px * xx + py * yx;
			float ry = px * xy + py * yy;

			// Scale & offset coords
			float upx = rx * scale + ox;
			float upy = ry * scale + oy;

			// calculate hex  info
			float d = hexagon(upx, upy);
			t_axial hex = pixel_to_pointy_hex(upx, upy);

			// Should we draw this hex?
			bool BadHex = hex.q < 0 || hex.q >= gameData.maxLine
				|| hex.r < 0 || hex.r >= gameData.maxLine
				|| !areCoordinatesValid(hex.q, hex.r);
			
			if (BadHex)
				mlx_put_pixel(image, x, y, ColorFromRGB(0, 0, 0));
			else
				mlx_put_pixel(image, x, y, HexDistanceToColor(d - 0.05f, hex));	// - 0.05f to add a black inner part
		}
}

float GetBaseAngle()
{
	const float base = -M_PI / 2;
	const float rotate = M_PI / 3;

	return base + gameData.gravity * rotate;
}