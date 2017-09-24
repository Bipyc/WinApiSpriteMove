#pragma once
#include "stdafx.h"

#define DIRUP 0
#define DIRDOWN 1
#define DIRLEFT 2
#define DIRRIGHT 3

typedef struct
{
	HDC spriteDC;
	HBITMAP spriteBitmap;
	HBITMAP spriteOldBitmap;
} SpriteAssocHandles;

typedef struct
{
	int x;
	int y;
} VectorCoords;

typedef struct
{
	SpriteAssocHandles handles;
	int x;
	int y;
	int currSpeed;
	VectorCoords vectorCoords;
	int currAcceleration;
	int width;
	int height;
	int maxSpeedX;
	int maxSpeedY;
	BOOL isMovedKey;
	BOOL isCatchedCursor;
	int direction;
	int relativeCenterOffsetX;
	int relativeCenterOffsetY;
	float breakingIntensity;
} SpriteInfo;

