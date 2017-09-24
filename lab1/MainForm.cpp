#include "stdafx.h"
#include "MainForm.h"
#include "Physics.h"

#define CLASS_NAME "MyWindowsApp"
#define FORM_NAME "Lab1"
#define STARTWINDOWWIDTH 500
#define STARTWINDOWHEIGHT 750
#define IDT_TIMERTIMERREPAINTING 1
#define IDT_TIMERCALCMOVEMENT 2
#define DELAYREPAINTING 34
#define DELAYMOVEMENT 34
#define STRENGHTGRAVITY 0.000000000001f
#define MAXSPEEDX 50
#define MAXSPEEDY 50
#define BREAKINGINTENSITY 1.5f
#define MOVESPEEDVIAKEY 10
#define MOVESPEEDVIAWHEEL 10

#define ISUP TRUE
#define ISDOWN FALSE

SpriteInfo sprite;
HWND hMainWnd;
int delay;
RECT windowAreaSize;


void CalcKeyMove(SpriteInfo *info, int direction)
{
	switch (direction)
	{
	case DIRUP:
	{
		CalculateNewState(&sprite, sprite.x, sprite.y - MOVESPEEDVIAKEY, STRENGHTGRAVITY, MOVEVIAOTHER);
	}
	break;
	case DIRDOWN:
	{
		CalculateNewState(&sprite, sprite.x, sprite.y + MOVESPEEDVIAKEY, STRENGHTGRAVITY, MOVEVIAOTHER);
	}
	break;
	case DIRLEFT:
	{
		CalculateNewState(&sprite, sprite.x - MOVESPEEDVIAKEY, sprite.y, STRENGHTGRAVITY, MOVEVIAOTHER);
	}
	break;
	case DIRRIGHT:
	{
		CalculateNewState(&sprite, sprite.x + MOVESPEEDVIAKEY, sprite.y, STRENGHTGRAVITY, MOVEVIAOTHER);
	}
	break;
	}
}

void CheckKeyCode(WPARAM key, SpriteInfo *info, BOOL isDown)
{
	switch (key)
	{
	case VK_UP:
	{
		if (isDown == ISDOWN)
		{
			info->isMovedKey = TRUE;
			info->direction = DIRUP;
		}
		else
		{
			info->isMovedKey = FALSE;
		}
	}
	break;
	case VK_DOWN:
	{
		if (isDown == ISDOWN)
		{
			info->isMovedKey = TRUE;
			info->direction = DIRDOWN;
		}
		else
		{
			info->isMovedKey = FALSE;
		}
	}
	break;
	case VK_LEFT:
	{
		if (isDown == ISDOWN)
		{
			info->isMovedKey = TRUE;
			info->direction = DIRLEFT;
		}
		else
		{
			info->isMovedKey = FALSE;
		}
	}
	break;
	case VK_RIGHT:
	{
		if (isDown == ISDOWN)
		{
			info->isMovedKey = TRUE;
			info->direction = DIRRIGHT;
		}
		else
		{
			info->isMovedKey = FALSE;
		}
	}
	break;
	}
}

SpriteAssocHandles MyLoadBitmap(const char *path)
{
	SpriteAssocHandles result;
	result.spriteBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	result.spriteDC = CreateCompatibleDC(NULL);
	result.spriteOldBitmap = (HBITMAP)SelectObject(result.spriteDC, result.spriteBitmap);

	return result;
}

void GetBitmapInfo(SpriteInfo *info)
{
	BITMAP bitmap;
	GetObject(info->handles.spriteBitmap, sizeof(BITMAP), &bitmap);

	info->width = bitmap.bmWidth;
	info->height = bitmap.bmHeight;
	info->maxSpeedX = MAXSPEEDX;
	info->maxSpeedY = MAXSPEEDY;
	info->isMovedKey = FALSE;
	info->isCatchedCursor = FALSE;

	info->x = 150;
	info->y = 150;
	info->breakingIntensity = BREAKINGINTENSITY;
}

void DrawImage(HDC screen, SpriteInfo sprite)
{
	BitBlt(
		screen,         
		sprite.x, sprite.y,            
		sprite.width,   
		sprite.height,   
		sprite.handles.spriteDC,        
		0, 0,            
		SRCCOPY         
	);
}

void CleanUpImage(SpriteInfo sprite)
{
	SelectObject(sprite.handles.spriteDC, sprite.handles.spriteOldBitmap);
	DeleteObject(sprite.handles.spriteBitmap);
	DeleteDC(sprite.handles.spriteDC);
}

void DisplayError(DWORD dw, const char* szFuncName)
{
	LPSTR buff;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		dw,
		LANG_NEUTRAL,
		(LPSTR)&buff,
		0,
		NULL);
	MessageBox(NULL, buff, szFuncName, MB_OK);
}

BOOL CheckIsCursorOnSprite(POINT cursor, SpriteInfo info)
{
	return ((cursor.x >= info.x) && (cursor.x <= info.x + info.width) && (cursor.y >= info.y) && (cursor.y <= info.y + info.height));
}

void TrackMouseMovement(POINT coords, SpriteInfo *info)
{
	if ( (coords.x > windowAreaSize.right) || (coords.x < windowAreaSize.left) )
	{
		info->isCatchedCursor = FALSE;
	}
	if ( (coords.y > windowAreaSize.bottom) || (coords.y < windowAreaSize.top) )
	{
		info->isCatchedCursor = FALSE;
	}
}

LRESULT CALLBACK WndProc(HWND hInst, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_LBUTTONDOWN:
			{
				POINT cursorCoords;
				GetCursorPos(&cursorCoords);
				ScreenToClient(hMainWnd, &cursorCoords);
				if ( CheckIsCursorOnSprite(cursorCoords, sprite) )
				{
					sprite.isCatchedCursor = TRUE;
					sprite.relativeCenterOffsetX = cursorCoords.x - sprite.x;
					sprite.relativeCenterOffsetY = cursorCoords.y - sprite.y;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				sprite.isCatchedCursor = FALSE;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				int wheelData = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
				if (wheelData > 0)
				{
					if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
					{
						CalcKeyMove(&sprite, DIRLEFT);
					}
					else
					{
						CalcKeyMove(&sprite, DIRUP);
					}
				}
				else
				{
					if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
					{
						CalcKeyMove(&sprite, DIRRIGHT);
					}
					else
					{
						CalcKeyMove(&sprite, DIRDOWN);
					}
				}
				CalcMove(&sprite, windowAreaSize.right, windowAreaSize.bottom);
			}
			break;
		case WM_KEYDOWN:
			{
				CheckKeyCode(wParam, &sprite, ISDOWN);
			}
			break;
		case WM_KEYUP:
			{
				CheckKeyCode(wParam, &sprite, ISUP);
			}
			break;
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;
		case WM_PAINT:
			{
				InvalidateRect(hMainWnd, NULL, TRUE);
				PAINTSTRUCT paintStruct;
				HDC screen = BeginPaint(hInst, &paintStruct);
				DrawImage(screen, sprite);
				EndPaint(hInst, &paintStruct);
			}
			break;
		case WM_SIZE:
			{
				GetClientRect(hMainWnd, &windowAreaSize);
				if ((LONG)sprite.x > windowAreaSize.right)
				{
					sprite.x = 0;
				}
				if ((LONG)sprite.y > windowAreaSize.bottom)
				{
					sprite.y = 0;
				}
		}
			break;
		case WM_TIMER:
			{
				switch (wParam)
				{
					case IDT_TIMERTIMERREPAINTING:
					{
						RedrawWindow(hMainWnd, NULL, NULL, RDW_INVALIDATE);
					}
					break;
					case IDT_TIMERCALCMOVEMENT:
					{
						if (sprite.isCatchedCursor)
						{
							POINT cursorCoords;
							GetCursorPos(&cursorCoords);
							ScreenToClient(hMainWnd, &cursorCoords);
							CalculateNewState(&sprite, cursorCoords.x, cursorCoords.y, STRENGHTGRAVITY, MOVEVIAMOUSE);
							TrackMouseMovement(cursorCoords, &sprite);
						}
						if (sprite.isMovedKey)
						{
							CalcKeyMove(&sprite, sprite.direction);
						}
						CalcBreaking(&sprite);

						CalcMove(&sprite, windowAreaSize.right, windowAreaSize.bottom);
					}
					break;
				}

			}
			break;
		default:
			return DefWindowProc(hInst, msg, wParam, lParam);
			break;
	}
	return 0;
}

WNDCLASSEX InitFormClass(HINSTANCE hInst)
{
	WNDCLASSEX wcFormClass;
	wcFormClass.hInstance = hInst;
	wcFormClass.cbSize = sizeof(WNDCLASSEX);
	wcFormClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcFormClass.lpfnWndProc = WndProc;
	wcFormClass.cbClsExtra = 0;
	wcFormClass.cbWndExtra = 0;
	wcFormClass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wcFormClass.hIconSm = NULL;
	wcFormClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcFormClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcFormClass.lpszClassName = CLASS_NAME;
	wcFormClass.lpszMenuName = NULL;

	return wcFormClass;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCommandLine, int nCommandShow)
{
	WNDCLASSEX wcFormClass;
	
	wcFormClass = InitFormClass(hInst);
	if (!RegisterClassEx(&wcFormClass))
	{
		MessageBox(NULL, "Error. Can't register window class", "Error", MB_OK);
		return 1;
	}
	hMainWnd = CreateWindow(wcFormClass.lpszClassName, FORM_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, STARTWINDOWWIDTH, STARTWINDOWHEIGHT, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInst, (LPVOID)NULL);
	if (!hMainWnd)
	{
		MessageBox(NULL, "Error. Can't create window", "Error", MB_OK);
		return 1;
	}

	sprite.handles = MyLoadBitmap("bomb.bmp");
	if (sprite.handles.spriteBitmap == NULL)
	{
		MessageBox(NULL, "Error. Can't load bitmap.", "Error", MB_OK);
		return 1;
	}
	GetBitmapInfo(&sprite);
	SetTimer(hMainWnd, IDT_TIMERTIMERREPAINTING, DELAYREPAINTING, (TIMERPROC)NULL);
	SetTimer(hMainWnd, IDT_TIMERCALCMOVEMENT, DELAYMOVEMENT, (TIMERPROC)NULL);
	GetClientRect(hMainWnd, &windowAreaSize);
	
	ShowWindow(hMainWnd, nCommandShow);
	UpdateWindow(hMainWnd);
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	CleanUpImage(sprite);
	KillTimer(hMainWnd, IDT_TIMERTIMERREPAINTING);
	KillTimer(hMainWnd, IDT_TIMERCALCMOVEMENT);

	return message.wParam;
}

