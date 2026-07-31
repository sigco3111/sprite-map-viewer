#ifndef _DIRECTDRAW_H_
#define _DIRECTDRAW_H_

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include "SetDDraw.h"
#include "DIB.h"
#include "Map.h"
#include "Sprite.h"
#include "CObject.h"

#include "RESOURCE.h"
#include "commdlg.h"

#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code)) &0x8000 ? 1 : 0)		//KeyDown 이벤트
#define KeyUp(vk_code)   ((GetAsyncKeyState(vk_code)) &0x8000 ? 0 : 1)		//KeyUp이벤트

#endif _DIRECTDRAW_H_