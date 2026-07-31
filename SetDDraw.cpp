
#include "SetDDraw.h"

int SETDDRAW::m_nInit = 0;
BOOL SETDDRAW::InItDDraw(HWND hWnd, int Width, int Heigh, int BitColor)
{	
	m_Screen_Width	= Width;
	m_Screen_Heigh	= Heigh;
	m_BitColor		= BitColor;
	
	if(m_nInit == 0)
	{
		m_hResult = DirectDrawCreateEx(0,(LPVOID*)&m_lpdd7,IID_IDirectDraw7,0);	
		if(m_hResult != DD_OK)
		{
			ErrorMessage("DirectDraw 생성 실패");		
			return 0;
		}

		m_nInit ++;
	}

	////////////////////////////////////////////////////// 협력수준 설정
	m_hResult = m_lpdd7->SetCooperativeLevel(hWnd, DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN); //CTRL+ALT+DEL사용가능|독점모드|풀스크린
	if(m_hResult != DD_OK) 
	{
		ErrorMessage("협력수준 설정 실패");		
		return 0;
	}

	m_hResult = m_lpdd7->SetDisplayMode(Width,Heigh,BitColor,0,0);//주파율과 플래그는 사용하지 않는다.
	if(m_hResult != DD_OK) 
	{
		ErrorMessage("해상도 설정 실패");		
		return 0;
	}
	//////////////////////////////////////////////////////
	
	DDSURFACEDESC2	ddsd2;
	ZeroMemory(&ddsd2, sizeof(ddsd2));				//DDSURFACEDESC2 초기화
	ddsd2.dwSize = sizeof(ddsd2);					//Size는 항상 알려줘야 한다.
	ddsd2.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;	//ddsd2.ddsCaps.dwCaps 수정 가능하게 해주는 플래그			
	ddsd2.dwBackBufferCount = 1;					//백버퍼 1개 생성
	ddsd2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_COMPLEX|DDSCAPS_FLIP;	//복합표면을 만든다.
	m_hResult = m_lpdd7->CreateSurface(&ddsd2, &m_lpPrimary, 0);	
		
	if(m_hResult != DD_OK)
	{
		ErrorMessage("PRIMARYSURFACE 설정 실패");
		return 0;
	}

	DDSCAPS2 ddscaps2;							//DDSCAPS2 생성
	ZeroMemory(&ddscaps2, sizeof(ddscaps2));	//DDSCAPS2 초기화
	ddscaps2.dwCaps = DDSCAPS_BACKBUFFER;		//백버퍼의 설정을 가능하게 하는 플래그

	m_hResult = m_lpPrimary->GetAttachedSurface(&ddscaps2, &m_lpBack);	//PRIMARYSURFACE와 합체 ^^;
	if(m_hResult != DD_OK)
	{
		ErrorMessage("PRIMARYSURFACE와 합체 실패 ^^;");
		return 0;
	}
	/////////////////////////////////////////////클리퍼 생성
	m_lpdd7->CreateClipper(0,&m_lpClipper,0);	//Clipper 객체 생성 : CreateClipper

	RGNDATA *pRgn;
	DWORD size = sizeof(RGNDATAHEADER)+sizeof(RECT);
	pRgn=(RGNDATA*) new BYTE[size];	
	ZeroMemory(pRgn, size);		//LPRGNDATA 초기화
	pRgn->rdh.dwSize = sizeof(RGNDATAHEADER);
	pRgn->rdh.iType = RDH_RECTANGLES;
	pRgn->rdh.nCount = 1;			//클리퍼 갯수
	pRgn->rdh.nRgnSize = sizeof(RECT); //ex) 클리핑할 RECT 영역이 3개면 sizeof(RECT)*3
	RECT rt={0,0,m_Screen_Width,m_Screen_Heigh};
	pRgn->rdh.rcBound = rt;
	
	CopyMemory(pRgn->Buffer, &rt, sizeof(RECT));
		
	m_lpClipper->SetClipList(pRgn,0);	//Clipping 시킬 영역 지정 : SetClipList
	m_lpBack->SetClipper(m_lpClipper);	//Surface에  Clipper 객체 결합 : SetClipper
	delete[](BYTE*)pRgn;
	
    /////////////////////////////////////////////
	m_Normal=0;
	return 1;
}

BOOL SETDDRAW::InItDDrawNormal(HWND hWnd, int Width, int Heigh, int BitColor)
{
	m_Screen_Width	= Width;
	m_Screen_Heigh	= Heigh;
	m_BitColor		= BitColor;
	
	if(m_nInit == 0)
	{
		m_hResult = DirectDrawCreateEx(0,(LPVOID*)&m_lpdd7,IID_IDirectDraw7,0);	
		if(m_hResult != DD_OK)
		{
			ErrorMessage("DirectDraw 생성 실패");		
			return 0;
		}
		m_nInit ++;
	}

	////////////////////////////////////////////////////// 협력수준 설정
	m_hResult = m_lpdd7->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
	if(m_hResult != DD_OK) 
	{
		ErrorMessage("협력수준 설정 실패");		
		return 0;
	}
	
	RECT rect, rcWork, rectWindow;
	SetRect(&rect, 0, 0, Width, Heigh);
	
	AdjustWindowRectEx(&rect, GetWindowLong(hWnd, GWL_STYLE),
		//GetMenu(hWnd) != NULL, GetWindowLong(hWnd, GWL_EXSTYLE));
		NULL, GetWindowLong(hWnd, GWL_EXSTYLE));
	
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
	GetWindowRect(hWnd, &rectWindow);
	
	if(rectWindow.left < rcWork.left )
		rectWindow.left = rcWork.left;
	
	if(rectWindow.top < rcWork.top )
		rectWindow.top = rcWork.top;
	
	SetWindowPos(hWnd, NULL, 
		rectWindow.left, rectWindow.top, 
		rect.right - rect.left, rect.bottom - rect.top, 
		SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);

	//////////////////////////////////////////////////////	프라이머리 서피스
	DDSURFACEDESC2	ddsd2;
	ZeroMemory(&ddsd2, sizeof(ddsd2));				//DDSURFACEDESC2 초기화
	ddsd2.dwSize = sizeof(ddsd2);					//Size는 항상 알려줘야 한다.
	ddsd2.dwFlags = DDSD_CAPS;
	ddsd2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;	
	m_hResult = m_lpdd7->CreateSurface(&ddsd2, &m_lpPrimary, 0);			
	if(m_hResult != DD_OK)
	{
		ErrorMessage("PRIMARYSURFACE 설정 실패 (Normal)");
		return 0;
	}

	////////////////////////////////////////////////////// 백서피스 대용 오프스크린
	ddsd2.dwFlags = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
	ddsd2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	ddsd2.dwWidth = Width;
	ddsd2.dwHeight = Heigh;

	m_hResult = m_lpdd7->CreateSurface(&ddsd2, &m_lpBackOffScreen, 0);
	if(m_hResult != DD_OK)
	{
		ErrorMessage("BackOffScreen 설정 실패");
		return 0;
	}

	/////////////////////////////////////////////클리퍼 생성
	m_lpdd7->CreateClipper(0,&m_lpClipper,0);	//Clipper 객체 생성 : CreateClipper

	RGNDATA *pRgn;
	DWORD size = sizeof(RGNDATAHEADER)+sizeof(RECT);
	pRgn=(RGNDATA*) new BYTE[size];	
	ZeroMemory(pRgn, size);		//LPRGNDATA 초기화
	pRgn->rdh.dwSize = sizeof(RGNDATAHEADER);
	pRgn->rdh.iType = RDH_RECTANGLES;
	pRgn->rdh.nCount = 1;			//클리퍼 갯수
	pRgn->rdh.nRgnSize = sizeof(RECT); //ex) 클리핑할 RECT 영역이 3개면 sizeof(RECT)*3
	RECT rt={0,0,m_Screen_Width,m_Screen_Heigh};
	pRgn->rdh.rcBound = rt;
	
	CopyMemory(pRgn->Buffer, &rt, sizeof(RECT));
		
	m_lpClipper->SetClipList(pRgn,0);	//Clipping 시킬 영역 지정 : SetClipList
	m_lpBackOffScreen->SetClipper(m_lpClipper);	//Surface에  Clipper 객체 결합 : SetClipper
	delete[](BYTE*)pRgn;
	
    /////////////////////////////////////////////

	m_lpdd7->CreateClipper(0, &m_lpClipper2, NULL);
	m_lpClipper2->SetHWnd(0,hWnd);
	m_lpPrimary->SetClipper(m_lpClipper2);	

	m_Normal=1;
	return 1;
}

void SETDDRAW::DD_OffScreen(DWORD dwWidth, DWORD dwHeight, LPDIRECTDRAWSURFACE7	*lpOffScreen, CDIB *Dib, BOOL flag, WORD colorKey)
{
	DDSURFACEDESC2	ddsd2;	
	//////////////////////////////////////////////////////// OffScreen 설정
	ZeroMemory(&ddsd2, sizeof(ddsd2));				
	ddsd2.dwSize = sizeof(ddsd2);

	if(flag)	
		ddsd2.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_CKSRCBLT;	//컬러키 설정
	else		
		ddsd2.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;

	ddsd2.dwWidth = dwWidth;
	ddsd2.dwHeight = dwHeight;
	ddsd2.ddckCKSrcBlt.dwColorSpaceHighValue = 0;
	ddsd2.ddckCKSrcBlt.dwColorSpaceLowValue = 0;
	ddsd2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;	//OffScreen을 VIDEOMEMORY로 사용하면
																		//비용도 비싸고, Restore가 않된다.
	m_hResult = m_lpdd7->CreateSurface(&ddsd2, lpOffScreen, 0);
	
	//DWORD colorKey = 0;
	if(Dib)
	{
		//////////////////////////////////////////////////////// Bit Color 검색
		ZeroMemory(&ddsd2,sizeof(ddsd2));
		ddsd2.dwSize = sizeof(ddsd2);
		(*lpOffScreen)->GetSurfaceDesc(&ddsd2);
		
		if(ddsd2.ddpfPixelFormat.dwRGBBitCount==16)
		{
			if(ddsd2.ddpfPixelFormat.dwRBitMask==0xf800)
			{	
				if(Dib->GetByteCount()!=2)
					Dib->Convert16(TRUE);	//565로 Convert
			}
			else
			{
				if(Dib->GetByteCount()!=2)
					Dib->Convert16(0);	//555로 Convert			
			}
		}	
		////////////////////////////////////////////////////////	
		
		//////////////////////////////////////////////////////// Lock
		DWORD srclen = Dib->GetBytePerLine();	//반복문 안에 에로우 연산자 사용은 가능한 자제할것.
		LONG srcheight = Dib->GetHeight();

		ZeroMemory(&ddsd2, sizeof(ddsd2));				
		ddsd2.dwSize = sizeof(ddsd2);

		(*lpOffScreen)->Lock(NULL,&ddsd2,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL);
		BYTE* pDest = (BYTE*)ddsd2.lpSurface;
		BYTE* pSrc = Dib->GetImage()+Dib->GetSizeImage()-srclen;
		LONG i;
		for(i=0;i<srcheight;i++)
		{
			CopyMemory(pDest,pSrc,srclen);
			pDest += ddsd2.lPitch;
			pSrc -= srclen;
		}
		(*lpOffScreen)->Unlock(NULL);

		WORD *pColor = (WORD *)Dib->GetImage();
		//colorKey = *pColor;
	}
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////// 컬러키 생성
	if(flag)
	{	
		DDCOLORKEY		ddck;
		ZeroMemory(&ddck, sizeof(ddck));
		ddck.dwColorSpaceHighValue = colorKey;
		ddck.dwColorSpaceLowValue = colorKey;
		
		(*lpOffScreen)->SetColorKey(DDCKEY_SRCBLT,&ddck);
	}
}

void SETDDRAW::DD_BackColor(int color)
{	
	ZeroMemory(&m_ddbf, sizeof(m_ddbf));
	m_ddbf.dwSize=sizeof(m_ddbf);				
	m_ddbf.dwFillColor=color;		
	m_lpBack->Blt(0,0,0,DDBLT_COLORFILL|DDBLT_WAIT,&m_ddbf);
}

void SETDDRAW::DD_Restore()
{
	m_hResult = m_lpPrimary->IsLost();	//백서피스를 잃어버렸을 경우를 체크 (Ex : Alt+Tab등을 눌렀을때)
	if(m_hResult == DDERR_SURFACELOST)
	{
		m_lpPrimary->Restore();
	}
}

void SETDDRAW::DDBit(int width, int height, RECT *sorRect, LPDIRECTDRAWSURFACE7 lpOffscreen, int flag, DDBLTFX *ddbfx)
{
	RECT s2_rt={sorRect->left ,sorRect->top ,sorRect->right , sorRect->bottom};
	RECT d2_rt={width,height,((sorRect->right - sorRect->left )+width), 
		((sorRect->bottom - sorRect->top)+height)};
	
	if(m_Normal==1)
		m_lpBackOffScreen->Blt(&d2_rt, lpOffscreen, &s2_rt, flag, ddbfx);
	else
		m_lpBack->Blt(&d2_rt, lpOffscreen, &s2_rt, flag, ddbfx);
}

void SETDDRAW::DDBitClip(int width, int height, RECT *sorRect, LPDIRECTDRAWSURFACE7 lpOffscreen, int flag, DDBLTFX *ddbfx)
{
	RECT s2_rt={sorRect->left ,sorRect->top ,sorRect->right , sorRect->bottom};
	RECT d2_rt={width,height,((sorRect->right - sorRect->left )+width), 
		((sorRect->bottom - sorRect->top)+height)};
	
	if ( width < 0 )
    {
        s2_rt.left = abs(width);
		d2_rt.left = 0;		
        width = 0;
    }
    if ( height < 0 )
    {
        s2_rt.top  = abs(height);
		d2_rt.top  = 0;
        height = 0;
    }
	
	if ( width+(s2_rt.right - s2_rt.left) >= m_Screen_Width )
	{
		s2_rt.right  -= (width+(s2_rt.right - s2_rt.left) - m_Screen_Width);
		d2_rt.right = m_Screen_Width;
	}
	
	if ( height + (s2_rt.bottom - s2_rt.top) >= m_Screen_Heigh )
	{
		s2_rt.bottom -= ((height+ (s2_rt.bottom - s2_rt.top)) - m_Screen_Heigh);
		d2_rt.bottom = m_Screen_Heigh;
	}
	if(m_Normal==1)
		m_lpBackOffScreen->Blt(&d2_rt,lpOffscreen,&s2_rt,flag, ddbfx);
	else
		m_lpBack->Blt(&d2_rt,lpOffscreen,&s2_rt,flag, ddbfx);
	
/*
	if(flag==0)
	{
		m_lpBack->Blt(&d2_rt,lpOffscreen,&s2_rt,DDBLTFAST_WAIT,0);
	}
	else
	{		
		m_lpBack->Blt(&d2_rt,lpOffscreen,&s2_rt,DDBLTFAST_WAIT | DDBLT_KEYSRC |DDBLT_KEYSRC,0);
	}
*/
}

void SETDDRAW :: DDBitFast(int x, int y, RECT *sorRect, LPDIRECTDRAWSURFACE7 lpOffscreen,int flag)
{
	RECT rect = {sorRect->left, sorRect->top, sorRect->right, sorRect->bottom};
		
	if ( x < 0 )
    {
        rect.left = abs(x);
        x = 0;
    }
    if ( y < 0 )
    {
        rect.top  = abs(y);
        y = 0;
    }	
    if ( x+(rect.right - rect.left) >= m_Screen_Width )
	{
		rect.right  -= (x+(rect.right - rect.left) - m_Screen_Width);
	}
    if ( y + (rect.bottom - rect.top) >= m_Screen_Heigh )
	{
		rect.bottom -= ((y+ (rect.bottom - rect.top)) - m_Screen_Heigh);
	}

	m_lpBack->BltFast( x, y, lpOffscreen, &rect, flag);	
	/*
	if(flag==1)
		m_lpBack->BltFast( x, y, lpOffscreen, &rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );	
	else
		m_lpBack->BltFast( x, y, lpOffscreen, &rect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY  );
	*/
}

BOOL SETDDRAW::ErrorMessage(char *str)
{
	MessageBox(m_hwnd,str,"연산실패",MB_OK);
	DestroyWindow(m_hwnd);
	return 0;
}

LPDIRECTDRAWSURFACE7 SETDDRAW::AlphaB(int x, int y, int x1, int y1, LPDIRECTDRAWSURFACE7 lp1,LPDIRECTDRAWSURFACE7 lp2,LPDIRECTDRAWSURFACE7 lp, WORD colorkey)
{
	DDSURFACEDESC2 ddsd;
			
	WORD *buffer,*buffer1,*buffer2;
	WORD color1, color2, halfcolor;	
	DWORD lPitch,lPitch1,lPitch2;
	
	RECT rect={x,y,x1+x,y1+y};	

	RECT backRt;
	RECT sprRt=rect;

	backRt.left = max(0, rect.left);
	backRt.top = max(0, rect.top);
	backRt.right = min(m_Screen_Width, rect.right);
	backRt.bottom = min(m_Screen_Heigh, rect.bottom);

	if(rect.left >= 0)
		sprRt.left = 0;
	else
		sprRt.left = -rect.left;

	if(rect.top >= 0)
		sprRt.top = 0;
	else
		sprRt.top = -rect.top;

	if(rect.right <= m_Screen_Width)
		sprRt.right = x1;
	else
		sprRt.right = x1 - (rect.right - m_Screen_Width);

	if(rect.bottom <= m_Screen_Heigh)
		sprRt.bottom = y1;
	else
		sprRt.bottom = y1 -(rect.bottom - m_Screen_Heigh);
	
	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp1->Lock(&sprRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
	lPitch1 = ddsd.lPitch>>1; 
	buffer1 = (WORD *)ddsd.lpSurface;
	lp1->Unlock(&sprRt);

	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp2->Lock(&backRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	lPitch2 = ddsd.lPitch>>1; 
	buffer2 = (WORD *)ddsd.lpSurface;
	lp2->Unlock(&backRt);

	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp->Lock(&sprRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	lPitch = ddsd.lPitch>>1; 
	buffer = (WORD *)ddsd.lpSurface;			
	lp->Unlock(&sprRt);
	
	for(int i=0; i<sprRt.bottom - sprRt.top; i++)	
	{		
		for(int j=0; j<sprRt.right - sprRt.left; j++)		
		{
			
			color1 = buffer1[j+i*lPitch1];
			color2 = buffer2[j+i*lPitch2];			

			if(color1==colorkey)
			{				
				halfcolor=color2;
				//continue;
			}
			else
			{
				halfcolor = ((color1&0xf7de)+(color2&0xf7de))>>1;
			}
			buffer[j+i*lPitch] = halfcolor;
		}
	}
	
	return lp;
}

LPDIRECTDRAWSURFACE7 SETDDRAW::AlphaB4(int x, int y, int x1, int y1, LPDIRECTDRAWSURFACE7 lp1,LPDIRECTDRAWSURFACE7 lp2,LPDIRECTDRAWSURFACE7 lp, WORD colorkey)
{
	DDSURFACEDESC2 ddsd;
			
	DWORD *buffer,*buffer1,*buffer2;
	DWORD color1, color2, halfcolor;	
	DWORD lPitch,lPitch1,lPitch2;
	
	RECT rect={x,y,x1+x,y1+y};	

	RECT backRt;
	RECT sprRt=rect;

	backRt.left = max(0, rect.left);
	backRt.top = max(0, rect.top);
	backRt.right = min(m_Screen_Width, rect.right);
	backRt.bottom = min(m_Screen_Heigh, rect.bottom);

	if(rect.left >= 0)
		sprRt.left = 0;
	else
		sprRt.left = -rect.left;

	if(rect.top >= 0)
		sprRt.top = 0;
	else
		sprRt.top = -rect.top;

	if(rect.right <= m_Screen_Width)
		sprRt.right = x1;
	else
		sprRt.right = x1 - (rect.right - m_Screen_Width);

	if(rect.bottom <= m_Screen_Heigh)
		sprRt.bottom = y1;
	else
		sprRt.bottom = y1 -(rect.bottom - m_Screen_Heigh);
	
	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp1->Lock(&sprRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
	lPitch1 = ddsd.lPitch>>2; 
	buffer1 = (DWORD *)ddsd.lpSurface;
	lp1->Unlock(&sprRt);

	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp2->Lock(&backRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	lPitch2 = ddsd.lPitch>>2; 
	buffer2 = (DWORD *)ddsd.lpSurface;
	lp2->Unlock(&backRt);

	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp->Lock(&sprRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	lPitch = ddsd.lPitch>>2; 
	buffer = (DWORD *)ddsd.lpSurface;			
	lp->Unlock(&sprRt);
	
	
	DWORD ck2=0, ck3=0;
	//ck2= (ck2|colorkey);
	//ck2= ((ck2<<16)|colorkey);
	ck2 = (colorkey<<16)|colorkey;

	int xLen = sprRt.right - sprRt.left;
	int yLen = sprRt.bottom - sprRt.top;

	for(int i=0; i<(yLen); i++)	
	{		
		for(int j=0; j<((xLen/2)*2); j++)		
		{			
			color1 = buffer1[j+i*lPitch1];
			color2 = buffer2[j+i*lPitch2];			
						
			if(color1==ck2)			
			{			
				halfcolor=color1;				
				//continue;
			}
			else if((color1&0xffff0000) == (ck2&0xffff0000))
			{				
				halfcolor = ((color1 & 0x0000f7de) >> 1 ) + ((color2 & 0x0000f7de) >> 1);
				halfcolor = (ck2 & 0xffff0000) | halfcolor;
			}			
			else if((color1&0x0000ffff) == colorkey)
			{				
				halfcolor = ((color1 & 0xf7de0000) >> 1 ) + ((color2 & 0xf7de0000) >> 1);
				halfcolor = (ck2 & 0x0000ffff) | halfcolor;
			}
			else
			{
				halfcolor = (((color1&0xf7def7de)>>1)+((color2&0xf7def7de)>>1));
			}
			buffer[j+i*lPitch] = halfcolor;
		}
	}
	if(xLen % 2)
	{
		for(int i=0; i<(yLen); i++)	
		{
			color1 = buffer1[x1+i*lPitch1];
			color2 = buffer2[x1+i*lPitch2];			
			
			if(color1==ck2)			
			{			
				halfcolor=color1;				
			}
			else if((color1&0xffff0000) == (ck2&0xffff0000))
			{				
				halfcolor = ((color1 & 0x0000f7de) >> 1 ) + ((color2 & 0x0000f7de) >> 1);
				halfcolor = (ck2 & 0xffff0000) | halfcolor;
			}			
			else if((color1&0x0000ffff) == colorkey)
			{
				halfcolor = ((color1 & 0xf7de0000) >> 1 ) + ((color2 & 0xf7de0000) >> 1);
				halfcolor = (ck2 & 0x0000ffff) | halfcolor;
			}
			else
			{
				halfcolor = (((color1&0xf7def7de)>>1)+((color2&0xf7def7de)>>1));
			}
			buffer[x1+i*lPitch] = halfcolor;
		}
	}
	
	return lp;
}

LPDIRECTDRAWSURFACE7 SETDDRAW::AlphaB25(int x, int y, int x1, int y1, LPDIRECTDRAWSURFACE7 lp1,LPDIRECTDRAWSURFACE7 lp2,LPDIRECTDRAWSURFACE7 lp, WORD colorkey, BYTE alpha)
{
	DDSURFACEDESC2 ddsd;
			
	DWORD *buffer,*buffer1,*buffer2;
	DWORD color1, color2, halfcolor;	
	DWORD lPitch,lPitch1,lPitch2;
	
	RECT rect={x,y,x1+x,y1+y};	

	RECT backRt;
	RECT sprRt=rect;

	backRt.left = max(0, rect.left);
	backRt.top = max(0, rect.top);
	backRt.right = min(m_Screen_Width, rect.right);
	backRt.bottom = min(m_Screen_Heigh, rect.bottom);

	if(rect.left >= 0)
		sprRt.left = 0;
	else
		sprRt.left = -rect.left;

	if(rect.top >= 0)
		sprRt.top = 0;
	else
		sprRt.top = -rect.top;

	if(rect.right <= m_Screen_Width)
		sprRt.right = x1;
	else
		sprRt.right = x1 - (rect.right - m_Screen_Width);

	if(rect.bottom <= m_Screen_Heigh)
		sprRt.bottom = y1;
	else
		sprRt.bottom = y1 -(rect.bottom - m_Screen_Heigh);
	
	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp1->Lock(&sprRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
	lPitch1 = ddsd.lPitch>>2; 
	buffer1 = (DWORD *)ddsd.lpSurface;
	lp1->Unlock(&sprRt);

	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp2->Lock(&backRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	lPitch2 = ddsd.lPitch>>2; 
	buffer2 = (DWORD *)ddsd.lpSurface;
	lp2->Unlock(&backRt);

	memset(&ddsd,0,sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);
	lp->Lock(&sprRt,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	lPitch = ddsd.lPitch>>2; 
	buffer = (DWORD *)ddsd.lpSurface;			
	lp->Unlock(&sprRt);
	
	
	DWORD ck2=0;
	ck2 = (colorkey<<16)|colorkey;

	int xLen = sprRt.right - sprRt.left;
	int yLen = sprRt.bottom - sprRt.top;

	int per1, per2;
	
	if(alpha<1)alpha=1;
	if(alpha>31)alpha=31;
	per1 = (32-alpha);
	per2 = alpha;	

	for(int i=0; i<(yLen); i++)	
	{		
		for(int j=0; j<(xLen>>1); j++)		
		{			
			color1 = buffer1[j+i*lPitch1];
			color2 = buffer2[j+i*lPitch2];			
						
			if(color1==ck2)			
			{			
				halfcolor=color1;				
				//continue;
			}
			else if((color1&0xffff0000) == (ck2&0xffff0000))
			{				
				color1 = ((( (color1 & 0xf81f07e0) >>5 ) * per1) & 0xf81f07e0) | ((((color1 & 0x7e0f81f) * per1 ) >>5) &0x7e0f81f);
				color2 = ((( (color2 & 0xf81f07e0) >>5 ) * per2) & 0xf81f07e0) | ((((color2 & 0x7e0f81f) * per2 ) >>5) &0x7e0f81f);								
				halfcolor = color1 + color2;
				halfcolor = (ck2 & 0xffff0000) | (halfcolor & 0x0000ffff);			
			}			
			else if((color1&0x0000ffff) == colorkey)
			{
				color1 = ((( (color1 & 0xf81f0000) >>5 ) * per1) & 0xf81f0000) | ((((color1 & 0x07e00000) * per1 ) >>5) &0x07e00000);
				color2 = ((( (color2 & 0xf81f0000) >>5 ) * per2) & 0xf81f0000) | ((((color2 & 0x07e00000) * per2 ) >>5) &0x07e00000);
				halfcolor = color1 + color2;
				halfcolor = (ck2 & 0x0000ffff) | halfcolor;				
			}
			else
			{
				color1 = ((( (color1 & 0xf81f07e0) >>5 ) * per1) & 0xf81f07e0) | ((((color1 & 0x7e0f81f) * per1 ) >>5) &0x7e0f81f);
				color2 = ((( (color2 & 0xf81f07e0) >>5 ) * per2) & 0xf81f07e0) | ((((color2 & 0x7e0f81f) * per2 ) >>5) &0x7e0f81f);
								
				halfcolor = color1 + color2;
			}
			buffer[j+i*lPitch] = halfcolor;
		}
	}
	if(xLen % 2)
	{
		for(int i=0; i<(yLen); i++)	
		{		
			color1 = buffer1[x1+i*lPitch1];
			color2 = buffer2[x1+i*lPitch2];			
						
			if(color1==ck2)			
			{			
				halfcolor=color1;				
				//continue;
			}
			else if((color1&0xffff0000) == (ck2&0xffff0000))
			{				
				color1 = ((( (color1 & 0xf81f07e0) >>5 ) * per1) & 0xf81f07e0) | ((((color1 & 0x7e0f81f) * per1 ) >>5) &0x7e0f81f);
				color2 = ((( (color2 & 0xf81f07e0) >>5 ) * per2) & 0xf81f07e0) | ((((color2 & 0x7e0f81f) * per2 ) >>5) &0x7e0f81f);								
				halfcolor = color1 + color2;
				halfcolor = (ck2 & 0xffff0000) | (halfcolor & 0x0000ffff);			
			}			
			else if((color1&0x0000ffff) == colorkey)
			{
				color1 = ((( (color1 & 0xf81f0000) >>5 ) * per1) & 0xf81f0000) | ((((color1 & 0x07e00000) * per1 ) >>5) &0x07e00000);
				color2 = ((( (color2 & 0xf81f0000) >>5 ) * per2) & 0xf81f0000) | ((((color2 & 0x07e00000) * per2 ) >>5) &0x07e00000);
				halfcolor = color1 + color2;
				halfcolor = (ck2 & 0x0000ffff) | halfcolor;				
			}
			else
			{
				color1 = ((( (color1 & 0xf81f07e0) >>5 ) * per1) & 0xf81f07e0) | ((((color1 & 0x7e0f81f) * per1 ) >>5) &0x7e0f81f);
				color2 = ((( (color2 & 0xf81f07e0) >>5 ) * per2) & 0xf81f07e0) | ((((color2 & 0x7e0f81f) * per2 ) >>5) &0x7e0f81f);
								
				halfcolor = color1 + color2;
			}
			buffer[x1+i*lPitch] = halfcolor;				
		}
	}	
	return lp;
}

void SETDDRAW::ConvertWin(HWND hWnd, int Width, int Heigh, int BitColor)
{
	if(m_Normal)
	{
		if(m_lpClipper)
			m_lpClipper->Release();
    	if(m_lpClipper2)
			m_lpClipper2->Release();
		if(m_lpBackOffScreen)
			m_lpBackOffScreen->Release();
		if(m_lpPrimary)
			m_lpPrimary->Release();
	
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		InItDDraw(hWnd,m_Screen_Width,m_Screen_Heigh,16);
	}
	else
	{
		if(m_lpClipper)
			m_lpClipper->Release();
		if(m_lpBackOffScreen)
			m_lpBackOffScreen->Release();
		if(m_lpPrimary)
			m_lpPrimary->Release();

		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX);		
		InItDDrawNormal(hWnd,m_Screen_Width,m_Screen_Heigh,16);
		
		LONG wx = (m_Display_Width - m_Screen_Width) >> 1;
		LONG wy = (m_Display_Heigh - m_Screen_Heigh) >> 1;
		MoveWindow(hWnd,wx,wy,m_Screen_Width, m_Screen_Heigh,1);		
	}
}

void SETDDRAW::DDTextOut(int x, int y, char *str)
{
	HDC hdc;
	DD_Restore();
	if(m_Normal==1)
	{
		m_lpBackOffScreen->GetDC(&hdc);
		TextOut(hdc,x,y,str,strlen(str));
		m_lpBackOffScreen->ReleaseDC(hdc);
	}
	else
	{
		m_lpBack->GetDC(&hdc);		
		TextOut(hdc,x,y,str,strlen(str));
		m_lpBack->ReleaseDC(hdc);
	}	
}

void SETDDRAW::DDRectOut(int left,int top, int right, int bottom, COLORREF color)
{
	HDC hdc;
	HPEN pen, oldpen;
	HBRUSH brush, oldbrush;
	
	DD_Restore();
	if(m_Normal==1)
	{
		m_lpBackOffScreen->GetDC(&hdc);
		
		pen = CreatePen(PS_SOLID,1,color);
		oldpen = (HPEN)SelectObject(hdc, pen);
		brush = CreateSolidBrush(color);
		oldbrush = (HBRUSH)SelectObject(hdc, brush);
		
		Rectangle(hdc, left, top, right, bottom);	
		SelectObject(hdc, oldpen);
		SelectObject(hdc, oldbrush);

		m_lpBackOffScreen->ReleaseDC(hdc);
	}
	else
	{
		m_lpBack->GetDC(&hdc);	
		
		pen = CreatePen(PS_SOLID,1,color);
		oldpen = (HPEN)SelectObject(hdc, pen);
		brush = CreateSolidBrush(color);
		oldbrush = (HBRUSH)SelectObject(hdc, brush);
		
		Rectangle(hdc, left, top, right, bottom);		
		SelectObject(hdc, oldpen);
		SelectObject(hdc, oldbrush);
		
		m_lpBack->ReleaseDC(hdc);
	}
}