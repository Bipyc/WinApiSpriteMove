#pragma once
#include "CommonTypes.h"

#define ACSELERATIONINTENSITY 0.5f

#define NOREBOUND -1
#define UPREBOUND 0
#define RIGTHREBOUND 1
#define DOWNREBOUND 2
#define LEFTREBOUND 3

#define MOVEVIAMOUSE 0
#define MOVEVIAOTHER 1

typedef int DIRECTION;

void CalculateNewState(SpriteInfo *spriteInfo, int newCursorX, int newCursorY, float time, int kindOfDevice);
VectorCoords SumVectors(VectorCoords vector1, VectorCoords vector2, int maxSpeedX, int maxSpeedY);
int CalculateVectorLength(VectorCoords vector);
void CalculateRebound(VectorCoords *vector, DIRECTION direction, int maxSpeedX, int maxSpeedY);
int CheckForRebound(SpriteInfo *spriteInfo, int width, int height);
void CalcMove(SpriteInfo *spriteInfo, int width, int height);
void CalcBreaking(SpriteInfo *spriteInfo);



