#include "Map.h"

CMAP::CMAP()
{
	m_EditX=0; 
	m_EditY=0;
	ZeroMemory(&m_Map,sizeof(MapH));
	ZeroMemory(&m_Tile,sizeof(TileH)*MaxArr);
}
CMAP::~CMAP()
{
	delete []lpOff_map;
}

void CMAP::MapLoad(char* path)
{
	FILE *fMap;
	int filelen=0;

	if((fMap=fopen(path,"r+b"))!=NULL)
	{
		fread(&m_Map.MaxTile,sizeof(int),1,fMap);
		fread(&m_Map.TileSizeXCount,sizeof(int),1,fMap);
		fread(&m_Map.TileSizeX,sizeof(int),1,fMap);
		fread(&m_Map.TileSizeY,sizeof(int),1,fMap);
		fread(&m_Map.MapSizeX,sizeof(int),1,fMap);
		fread(&m_Map.MapSizeY,sizeof(int),1,fMap);		
		
		m_Dib_Map = new CDIB[m_Map.MaxTile];
		lpOff_map = new LPDIRECTDRAWSURFACE7[m_Map.MaxTile];

		for(int i=0; i<m_Map.MaxTile; i++)
		{
			//BITMAPFILEHEADER	bmfh;
			//fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fMap);

			WORD bfSize, bfOffBits;
			fread(&bfSize, sizeof(WORD), 1, fMap);
			fread(&bfOffBits, sizeof(WORD), 1, fMap);

			m_Dib_Map[i].m_pBmi = (BITMAPINFO*)new BYTE[bfOffBits-sizeof(BITMAPFILEHEADER)];
			fread(m_Dib_Map[i].m_pBmi, sizeof(BITMAPINFO), 1, fMap);
			m_Dib_Map[i].m_pBmih = (BITMAPINFOHEADER*)m_Dib_Map[i].m_pBmi;
			m_Dib_Map[i].m_ImgSize = m_Dib_Map[i].GetBytePerLine() * abs(m_Dib_Map[i].m_pBmih->biHeight);
			m_Dib_Map[i].m_pImage = new BYTE[m_Dib_Map[i].m_ImgSize];

			fread(m_Dib_Map[i].m_pImage, sizeof(BYTE), m_Dib_Map[i].m_ImgSize,fMap);
			if(m_Dib_Map[i].m_pBmih->biHeight < 0)			//데카르트좌표를 사용하지 않은 BMP를 데카르트좌료로 바꿔준다.
			{
				LONG	i,XLen = m_Dib_Map[i].GetBytePerLine();
				BYTE	*pImg = new BYTE[m_Dib_Map[i].m_ImgSize];
				BYTE	*pDest = pImg;
				BYTE	*pSrc = m_Dib_Map[i].m_pImage + m_Dib_Map[i].m_ImgSize - XLen;
				
				m_Dib_Map[i].m_pBmih->biHeight = abs(m_Dib_Map[i].m_pBmih->biHeight);
				for(i=0;m_Dib_Map[i].m_pBmih->biHeight;i++)
				{
					memcpy(pDest,pSrc,XLen);
					pDest += XLen;
					pSrc -= XLen;
				}
				memcpy(m_Dib_Map[i].m_pImage,pImg,m_Dib_Map[i].m_ImgSize);
				delete []pImg;
			}
		}
		for(int z=0; z<MaxArr; z++)
		{
			for(int i=0;i<m_Map.MapSizeY;i++)
			{	
				for(int j=0;j<m_Map.MapSizeX;j++)
				{			
					fread(&m_Tile[z].MapArr[i][j],sizeof(BYTE),1,fMap);					
					fread(&m_Tile[z].Tile[i][j],sizeof(BYTE),1,fMap);
				}
			}
		}
		for( i=0;i<m_Map.MapSizeY;i++)
		{	
			for(int j=0;j<m_Map.MapSizeX;j++)
			{			
				fread(&m_Map.Attr[i][j],sizeof(BYTE),1,fMap);
			}
		}
		fclose(fMap);
		
		for( i=0; i< m_Map.MaxTile; i++)
		{
			m_SetDDraw->DD_OffScreen(m_Map.TileSizeX*m_Map.MapSizeX, m_Map.TileSizeY*m_Map.MapSizeX, &lpOff_map[i], &m_Dib_Map[i], 1, Convert565(255,0,255));
			//m_SetDDraw->DD_OffScreen(Map.TileSizeX*Map.MapSizeX, Map.TileSizeY*Map.MapSizeX, &lpOff_back[i], &g_Dib_Map[i], 1, Convert565(255,255,255));
		}
		delete [] m_Dib_Map;
	}	
}

void CMAP::DrawMap()
{
	char temp[10]={0,};
	char temp2[10]={0,};
	int tempint=0;
	
	for(int z=0; z<MaxArr;z++)
	{
		for(int i=0;i<m_Map.MapSizeX;i++)
		{
			for(int j=0;j<m_Map.MapSizeY;j++)
			{
				//if((z!=0) && (m_Tile[z].Tile[j][i]!=0)) //2번 레이어 부터는 0번 타일 출력하지 않음(if문 삭제시 컬러키만이 트랜스페어런스의 조건이 된다.)
				{
					RECT dest={i*m_Map.TileSizeX+m_EditX, j*m_Map.TileSizeY+m_EditY,
						(i*m_Map.TileSizeX)+m_Map.TileSizeX+m_EditX, (j*m_Map.TileSizeY)+m_Map.TileSizeY+m_EditY};
					RECT sorc={
						(m_Tile[z].MapArr[j][i] % m_Map.TileSizeXCount)*m_Map.TileSizeX,
							(m_Tile[z].MapArr[j][i] / m_Map.TileSizeXCount)*m_Map.TileSizeY,
							(m_Tile[z].MapArr[j][i] % m_Map.TileSizeXCount)*m_Map.TileSizeX+m_Map.TileSizeX,
							(m_Tile[z].MapArr[j][i] / m_Map.TileSizeXCount)*m_Map.TileSizeY+m_Map.TileSizeY
					};
					
					if(m_SetDDraw->GetNormal())
						m_SetDDraw->GetBackOffScreenSurface()->Blt(&dest, lpOff_map[m_Tile[z].Tile[j][i]], &sorc, DDBLTFAST_WAIT|DDBLT_KEYSRC, 0);
					else
						m_SetDDraw->GetBackSurface()->Blt(&dest, lpOff_map[m_Tile[z].Tile[j][i]], &sorc, DDBLTFAST_WAIT|DDBLT_KEYSRC, 0);
				}
			}
		}
	}
	/*
	for(int i=0;i<Map.MapSizeX;i++)
	{
		for(int j=0;j<Map.MapSizeY;j++)
		{			
			wsprintf(temp,"%d",tempint);
			m_SetDDraw->DDTextOut(j*m_Tile.TileSizeX+1+tempX,i*m_Tile.TileSizeY+1+tempY,temp);
			tempint++;
			wsprintf(temp2,"%d", m_Tile.Attr[j][i]);
			m_SetDDraw->DDTextOut(i*Map.TileSizeY+15+tempX,j*m_Tile.TileSizeX+10+tempY,temp2);
		}
	}*/
}

BYTE CMAP::Mapchk(CObject* Obj, WORD val, WORD Action, WORD Frame)
				  //RECT sorc, WORD val, int* cx, int* cy, int move)
{	
	RECT sorc;	
	sorc.left	= Obj->m_Sprite->m_Ation[Action].MapChk_Rt.left
		- Obj->m_Sprite->m_Ation[Action].pFrame[Frame].Center.x;

	sorc.top	= Obj->m_Sprite->m_Ation[Action].MapChk_Rt.top
		- Obj->m_Sprite->m_Ation[Action].pFrame[Frame].Center.y;

	sorc.right	= Obj->m_Sprite->m_Ation[Action].MapChk_Rt.right
		- Obj->m_Sprite->m_Ation[Action].pFrame[Frame].Center.x;

	sorc.bottom	= Obj->m_Sprite->m_Ation[Action].MapChk_Rt.bottom
		- Obj->m_Sprite->m_Ation[Action].pFrame[Frame].Center.y;

	RECT rt;	
	rt.left		= Obj->Get_CX(Action) + Obj->m_Sprite->m_Ation[Action].MapChk_Rt.left - m_EditX;
	rt.top		= Obj->Get_CY(Action) + Obj->m_Sprite->m_Ation[Action].MapChk_Rt.top - m_EditY;
	rt.right	= Obj->Get_CX(Action) + Obj->m_Sprite->m_Ation[Action].MapChk_Rt.right - m_EditX;
	rt.bottom	= Obj->Get_CY(Action) + Obj->m_Sprite->m_Ation[Action].MapChk_Rt.bottom - m_EditY;

	WORD move = Obj->m_Move;
	
	//m_SetDDraw->DDRectOut(rt.left+m_EditX, rt.top+m_EditY, rt.right+m_EditX, rt.bottom+m_EditY);
	switch(val)
	{
	case LEFT:
		{	
			if((m_Map.Attr[rt.top / m_Map.TileSizeY][rt.left / m_Map.TileSizeX] == 0) 
				&& (m_Map.Attr[rt.bottom / m_Map.TileSizeY][rt.left / m_Map.TileSizeX] == 0))
			{
				if((m_Map.Attr[rt.top / m_Map.TileSizeY][((Obj->m_X+sorc.left - m_EditX) - move) / m_Map.TileSizeX] != 0)
					|| (m_Map.Attr[rt.bottom / m_Map.TileSizeY][((Obj->m_X+sorc.left - m_EditX) - move) / m_Map.TileSizeX] != 0))
				{
					//player->Px = (rt.right * Map.TileSizeX);
					return 2;
				}
				else
				{
					Obj->m_X -= move;
				}
			}				
			return 1;
		}
		break;
	case TOP:
		{			
			if((m_Map.Attr[rt.top / m_Map.TileSizeY][rt.left / m_Map.TileSizeX] == 0) 
				&& (m_Map.Attr[rt.top / m_Map.TileSizeY][rt.right / m_Map.TileSizeX] == 0))
			{
				if((m_Map.Attr[((Obj->m_Y+sorc.top - m_EditY)-move) / m_Map.TileSizeY][rt.left / m_Map.TileSizeX] != 0) 
					|| (m_Map.Attr[((Obj->m_Y+sorc.top - m_EditY) - move) / m_Map.TileSizeY][rt.right / m_Map.TileSizeX] != 0))
				{
					//player->Py = (rt.top * Map.TileSizeY)+Map.TileSizeY;
					return 2;
				}
				else
				{				
					Obj->m_Y -= move;
				}
			}
			return 1;			
		}
		break;
	case RIGHT:
		{	
			if((m_Map.Attr[rt.top / m_Map.TileSizeY][rt.right / m_Map.TileSizeX] == 0) 
				&& (m_Map.Attr[rt.bottom / m_Map.TileSizeY][rt.right / m_Map.TileSizeX] == 0))
			{
				if((m_Map.Attr[rt.top / m_Map.TileSizeY][((Obj->m_X+sorc.right - m_EditX)+move) / m_Map.TileSizeX] != 0)
					|| (m_Map.Attr[rt.bottom / m_Map.TileSizeY][((Obj->m_X+sorc.right - m_EditX)+move) / m_Map.TileSizeX] != 0))					
				{
					//player->Px = (rt.right * Map.TileSizeX) - player->width-1;
					return 2;
				}
				else
				{
					Obj->m_X += move;
				}
			}			
			return 1;
		}
		break;
	case BOTTOM:
		{
			if((m_Map.Attr[rt.bottom / m_Map.TileSizeY][rt.left / m_Map.TileSizeX] == 0) 
				&& (m_Map.Attr[rt.bottom / m_Map.TileSizeY][rt.right / m_Map.TileSizeX] == 0))
			{
				if((m_Map.Attr[((Obj->m_Y+sorc.bottom - m_EditY)+move) / m_Map.TileSizeY][rt.left / m_Map.TileSizeX] != 0) 
					|| (m_Map.Attr[((Obj->m_Y+sorc.bottom - m_EditY)+move) / m_Map.TileSizeY][rt.right / m_Map.TileSizeX] != 0))
				{
					//player->Py = (rt.bottom * Map.TileSizeY) - player->height-1;
					return 2;
				}
				else
				{
					Obj->m_Y += move;
				}
			}
			return 1;
		}
		break;
	}
	return 0;
}