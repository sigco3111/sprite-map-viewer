#ifndef _MAP_H_
#define _MAP_H_

#include <windows.h>
#include <stdio.h>
#include <ddraw.h>
#include "DIB.h"
#include "SetDDraw.h"
#include "CObject.h"

#define MapArrMax_X 100
#define MapArrMax_Y 100
#define MaxArr		3

#define LEFT		1
#define TOP			2
#define RIGHT		3
#define BOTTOM		4

struct MapH
{
	int		MaxTile;
	//char	*BMPFilePath;
	int		TileSizeXCount;
	int		TileSizeX;
	int		TileSizeY;	
	int		MapSizeX;
	int		MapSizeY;	
	BYTE	Attr[MapArrMax_X][MapArrMax_Y];	
};

struct TileH
{
	BYTE	MapArr[MapArrMax_X][MapArrMax_Y];
	BYTE	Tile[MapArrMax_X][MapArrMax_Y];
};

class CMAP  :CDIB
{
public:
	CMAP();
	~CMAP();
	BYTE	Mapchk(CObject* Obj, WORD val,  WORD Action, WORD Frame=0);		//0: 이동, 1: 이동 못함, 2 ~ 255: 기타

	void	MapLoad(char* path);
	void	DrawMap();
	void	SetDrawBase(SETDDRAW *DDrawBase){m_SetDDraw = DDrawBase;}

protected:	
	CDIB		*m_Dib_Map;
	SETDDRAW	*m_SetDDraw;

public:
	MapH		m_Map;
	TileH		m_Tile[MaxArr];
	LPDIRECTDRAWSURFACE7	*lpOff_map;

	int			m_EditX;
	int			m_EditY;
};
#endif

//BYTE	Mapchk(RECT sorc, WORD val, int* cx, int* cy, int move);
//BYTE	Mapchk(체크할 사각영역, 
//             LEFT or TOP or RIGHT or BOTTOM 중 선택
//             캐릭터의 x좌표(중심점을 적용시킨 좌표)
//             캐릭터의 y좌표(중심점을 적용시킨 좌표)
//             캐릭터가 움직일 거리)							0: 이동, 1: 이동 못함, 2 ~ 255: 기타
//ex) g_Map->Mapchk(g_player->m_Sprite->m_Ation[1].MapChk_Rt, 
//	       LEFT, g_player->Get_CX(1), g_player->Get_CY(1), g_player->m_Move);