#include "Physics.h"
#include <math.h>

#define SIGNUM(Value) ((Value) < 0 ? (-1) : 1)
#define MOVEVIAMOUSE 0
#define MOVEVIAOTHER 1

void CalculateNewState(SpriteInfo *spriteInfo, int newCursorX, int newCursorY, float time, int kindOfDevice)
{
	VectorCoords newVector;
	int newCoordX, newCoordY;
	switch (kindOfDevice)
	{
		case MOVEVIAMOUSE:
		{
			newCoordX = spriteInfo->x + spriteInfo->relativeCenterOffsetX;
			newCoordY = spriteInfo->y + spriteInfo->relativeCenterOffsetY;
		}
		break;
		case MOVEVIAOTHER:
		{
			newCoordX = spriteInfo->x;
			newCoordY = spriteInfo->y;
		}
		break;

	}
	newVector = { (int)( (newCursorX - newCoordX) * ACSELERATIONINTENSITY), (int)((newCursorY - newCoordY) * ACSELERATIONINTENSITY) };

	spriteInfo->vectorCoords = SumVectors(spriteInfo->vectorCoords, newVector, spriteInfo->maxSpeedX, spriteInfo->maxSpeedY);
}

void CalcMove(SpriteInfo *spriteInfo, int width, int height)
{
	spriteInfo->x = spriteInfo->vectorCoords.x + spriteInfo->x;
	spriteInfo->y = spriteInfo->vectorCoords.y + spriteInfo->y;

	int reboundDirection = 0;
	while (reboundDirection >= 0)
	{
		reboundDirection = CheckForRebound(spriteInfo, width, height);
		if (reboundDirection >= 0)
		{
			CalculateRebound(&(spriteInfo->vectorCoords), reboundDirection, spriteInfo->maxSpeedX, spriteInfo->maxSpeedY);
		}
	}
}

VectorCoords SumVectors(VectorCoords vector1, VectorCoords vector2, int maxSpeedX, int maxSpeedY)
{
	VectorCoords result;

	result.x = vector2.x + vector1.x;
	if (abs(result.x) > maxSpeedX)
	{
		result.x = SIGNUM(result.x) * maxSpeedX;
	}
	result.y = vector2.y + vector1.y;
	if (abs(result.y) > maxSpeedY)
	{
		int sign = 
		result.y = SIGNUM(result.y) * maxSpeedY;
	}

	return result;
}

int CheckForRebound(SpriteInfo *spriteInfo, int width, int height)
{
	if ((spriteInfo->y) < 0)
	{
		spriteInfo->y = 0;
		return UPREBOUND;
	}
	if ((spriteInfo->y + spriteInfo->height) >= height)
	{
		spriteInfo->y = height - spriteInfo->height - 1;
		return DOWNREBOUND;
	}
	if ((spriteInfo->x) < 0)
	{
		spriteInfo->x = 0;
		return LEFTREBOUND;
	}
	if ((spriteInfo->x + spriteInfo->width) >= width)
	{
		spriteInfo->x = width - spriteInfo->width - 1;
		return RIGTHREBOUND;
	}
	return NOREBOUND;
}

void CalculateRebound(VectorCoords *vector, DIRECTION direction, int maxSpeedX, int maxSpeedY)
{
	switch (direction)
	{
		case UPREBOUND:
		{
			vector->y = -vector->y;
		}
		break;
		case DOWNREBOUND:
		{
			vector->y = -vector->y;
		}
		break;
		case LEFTREBOUND:
		{
			vector->x = -vector->x;
		}
		break;
		case RIGTHREBOUND:
		{
			vector->x = -vector->x;
		}
		break;
	}
}

int CalculateVectorLength(VectorCoords vector)
{
	return (int)(sqrt(vector.x * vector.x + vector.y * vector.y));
}

void CalcBreaking(SpriteInfo *spriteInfo)
{
	int vectorLength = CalculateVectorLength(spriteInfo->vectorCoords);
	if (spriteInfo->vectorCoords.x != 0)
	{
		int absNewX = abs(spriteInfo->vectorCoords.x) - (int)(spriteInfo->breakingIntensity * abs(spriteInfo->vectorCoords.x) / vectorLength);
		if (absNewX < 0)
		{
			spriteInfo->vectorCoords.x = 0;
		}
		else
		{
			spriteInfo->vectorCoords.x = SIGNUM(spriteInfo->vectorCoords.x) * absNewX;
		}
	}
	if (spriteInfo->vectorCoords.y != 0)
	{
		int absNewY = abs(spriteInfo->vectorCoords.y) - (int)(spriteInfo->breakingIntensity * abs(spriteInfo->vectorCoords.y) / vectorLength);
		if (absNewY < 0)
		{
			spriteInfo->vectorCoords.y = 0;
		}
		else
		{
			spriteInfo->vectorCoords.y = SIGNUM(spriteInfo->vectorCoords.y) * absNewY;
		}
	}
}

