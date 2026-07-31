
#include <ddraw.h>
#include "dib.h"

#ifndef _SETDDRAW_H_
#define _SETDDRAW_H_

#define ScreenWidth 800
#define ScreenHeight 600

class SETDDRAW
{

 protected:
	 static int m_nInit;

      HWND						m_hwnd;
	  HRESULT					m_hResult;
	  DDBLTFX					m_ddbf;
      LPDIRECTDRAW7				m_lpdd7;
      LPDIRECTDRAWSURFACE7		m_lpPrimary; 
      LPDIRECTDRAWSURFACE7		m_lpBack;
	  LPDIRECTDRAWSURFACE7		m_lpBackOffScreen;
	  LPDIRECTDRAWCLIPPER		m_lpClipper;
	  LPDIRECTDRAWCLIPPER		m_lpClipper2;
	  
      BOOL			m_Normal;  
      int			m_Screen_Width;
      int			m_Screen_Heigh;
      int			m_BitColor;         	// 사용할 비트 수 ex) 8, 16, 24,  32 

	  int			m_Display_Width;
	  int			m_Display_Heigh;
      int			m_Display_BitColor;
   
 public:

	 inline SETDDRAW():m_hwnd(0),m_lpdd7(0),m_lpPrimary(0),m_lpBack(0),m_lpClipper(0),m_lpClipper2(0),m_lpBackOffScreen(0),
		        m_Screen_Width(0),m_Screen_Heigh(0),m_BitColor(0)
	 {
		 Set_Display_Width(GetSystemMetrics(SM_CXSCREEN));
		 Set_Display_Heigh(GetSystemMetrics(SM_CYSCREEN));
	 };	         	         
                   
	 inline ~SETDDRAW(){
		 if(m_lpdd7)
		 {
			 if(m_lpClipper)
				 m_lpClipper->Release();
			 if(m_lpClipper2)
				 m_lpClipper2->Release();
			 if(m_lpBackOffScreen)
				 m_lpBackOffScreen->Release();
			 if(m_lpPrimary)
				 m_lpPrimary->Release();
		 }
		 m_lpdd7->Release();
	 }
     
      // Direct Draw 를 초기화                                
      BOOL InItDDraw(HWND hWnd, int Width, int Heigh, int BitColor);
	  BOOL InItDDrawNormal(HWND hWnd, int Width, int Heigh, int BitColor);
	  void ConvertWin(HWND hWnd, int Width, int Heigh, int BitColor);

      // 백서피스를 잃어버렸을 경우 재생시킨다....ㅡㅡ;
      void DD_Restore();
      
      // 지정된 color 으로  백서피스를 칠한다  
      void DD_BackColor(int color);
	  
	  //오프스크린 설정
	  //void DD_OffScreen(DWORD dwWidth, DWORD dwHeight, CDIB *Dib);
	  void DD_OffScreen(DWORD dwWidth, DWORD dwHeight, LPDIRECTDRAWSURFACE7	*lpOffScreen,  CDIB *Dib = NULL, BOOL flag=0, WORD colorKey=0); //flag : 1 DDCOLORKEY 사용
      
	  //Display return
	  void Set_Display_Width(int x){m_Display_Width = x;}
	  void Set_Display_Heigh(int x){m_Display_Heigh = x;}
	  void Set_Display_BitColor(int x){m_Display_BitColor = x;}

      // 각 각의 서피스들을 return 한다
	  LPDIRECTDRAW7        Getlpdd7(){return m_lpdd7;}
      LPDIRECTDRAWSURFACE7 GetPrimarySurface() {return  m_lpPrimary;}
      LPDIRECTDRAWSURFACE7 GetBackSurface()    {return  m_lpBack;}  
	  LPDIRECTDRAWSURFACE7 GetBackOffScreenSurface()    {return  m_lpBackOffScreen;}
	  //LPDIRECTDRAWSURFACE7 GetOffScreen()    {return  m_lpOffScreen;}
	  LPDIRECTDRAWCLIPPER	GetClipper()    {return  m_lpClipper;}
	  BOOL					GetNormal()		{return m_Normal;}
	  void					SetNormal(BOOL x)		{m_Normal = x;}

	  // 이미지 출력
	  void DDBit(int width, int height, RECT *sorRect, LPDIRECTDRAWSURFACE7 lpOffscreen, int flag, DDBLTFX *ddbfx);
	  void DDBitFast(int x, int y, RECT *sorRect, LPDIRECTDRAWSURFACE7 lpOffscreen,int flag);
	  void DDBitClip(int width, int height, RECT *sorRect, LPDIRECTDRAWSURFACE7 lpOffscreen, int flag, DDBLTFX *ddbfx);
	  LPDIRECTDRAWSURFACE7 AlphaB(int x, int y, int x1, int y1, LPDIRECTDRAWSURFACE7 lp,LPDIRECTDRAWSURFACE7 lp1,LPDIRECTDRAWSURFACE7 lp2, WORD colorkey);
	  LPDIRECTDRAWSURFACE7 AlphaB4(int x, int y, int x1, int y1, LPDIRECTDRAWSURFACE7 lp1,LPDIRECTDRAWSURFACE7 lp2,LPDIRECTDRAWSURFACE7 lp, WORD colorkey);
	  LPDIRECTDRAWSURFACE7 AlphaB25(int x, int y, int x1, int y1, LPDIRECTDRAWSURFACE7 lp1,LPDIRECTDRAWSURFACE7 lp2,LPDIRECTDRAWSURFACE7 lp, WORD colorkey, BYTE alpha);

      // 메세지 출력
	  void DDTextOut(int x, int y, char *str);
	  void DDRectOut(int left,int top, int right, int bottom, COLORREF penc = RGB(255,255,255));
      int ErrorMessage(char *str);
};

#endif _SETDDRAW_H_

/*
Normal Mode
1. 해상도 변경 불가.
2. Flip 불가
3. BackSurface 생성 불가
4. Complex 불가
5. Clipping 붙이기
6. BltFast 사용불가
7. 자원 독점 불가
*/