#include "DirectDraw.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SetDlgProc(HWND, UINT, WPARAM, LPARAM);
void DD_Rotat(int x, int y, int r);

HWND g_hWnd;
BOOL GAME = 1;

LPDIRECTDRAWSURFACE7	lpOff_sb;

int edit_x=0, edit_y=0;

RECT g_rectWindow;

void DDTextOut();
void DD_DibDraw();
void DDBit(int width, int height, CDIB *dib, LPDIRECTDRAWSURFACE7 lpOffscreen);
void DrawPlayer();
void DrawTester();

void OpenSprite(char* path);
void OpenMap(char* path);

BYTE g_TActionFlag;
BOOL g_Map_Attr_View;
WORD g_Sel_Action;
WORD g_Sel_FPS = 50;
HINSTANCE g_hInst;
/////////////////////////////////////////

/////////////////////////////////////////
SETDDRAW	*SetDDraw = new SETDDRAW;

CMAP		*g_Map = new CMAP;

CSprite		*g_player_Spr = new CSprite;
CObject		*g_player = new CObject;
/////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static char	szAppName[] = "HelloDX";
	HWND		hWnd;
	MSG			msg;
	WNDCLASSEX	wndclass;

	g_hInst = hInstance;
	
	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW |WS_EX_TOPMOST; // WS_EX_TOPMOST : 최상위 윈도우
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(0, IDC_ARROW);	
	wndclass.hbrBackground	=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName	= (LPCSTR)IDR_MENU_FILE;
	wndclass.lpszClassName	= szAppName;
	wndclass.hIconSm		= LoadIcon(0, IDI_APPLICATION);
	
	RegisterClassEx(&wndclass);
		
	hWnd = CreateWindow(szAppName, "HelloDX", WS_POPUP|WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX, 
		0, 0,ScreenWidth, ScreenHeight, 0, 0, hInstance, 0);

	RECT winrect;
	GetWindowRect(hWnd,&winrect);
	LONG wx = (GetSystemMetrics(SM_CXSCREEN) - (winrect.right - winrect.left)) >> 1;
	LONG wy = (GetSystemMetrics(SM_CYSCREEN) - (winrect.bottom - winrect.top)) >> 1;
	MoveWindow(hWnd,wx,wy,ScreenWidth, ScreenHeight,1);
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	//SetDDraw.InItDDraw(hWnd,ScreenWidth,ScreenHeight,16);
	SetDDraw->InItDDrawNormal(hWnd,ScreenWidth,ScreenHeight,16);
	////////////////////////////////////////////////////////////////////////////////// BMP LOAD

	
	//Dib_back->Load("forest.bmp");

	
	//////////////////////////////////////////	MapLoad;		
	g_Map->SetDrawBase(SetDDraw);
	g_Map->MapLoad("MapTest.Map");
	SetDDraw->DD_OffScreen(ScreenWidth, ScreenHeight, &lpOff_sb);	
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////	SpriteLoad;
	g_player_Spr->SetDrawBase(SetDDraw);	
	g_player_Spr->LoadSprite("TestSpr.spr");

	g_player->SetSpriteBase(g_player_Spr);
	g_player->m_X		= 200;
	g_player->m_Y		= 300;
	g_player->m_Move	= 8;
	g_player->m_jump	= 1000;
	g_player->m_gravity = 50;
		
	//////////////////////////////////////////	TestLoad;
	/*CDIB *Dib_Player = new CDIB;
	Dib_Player->Load("test.bmp");
	g_player.width = Dib_Player->GetWidth();
	g_player.height = Dib_Player->GetHeight();
	SetDDraw->DD_OffScreen(Dib_Player->GetWidth(), Dib_Player->GetHeight(), &g_player.lpOff_Player, Dib_Player, 1, Convert565(231,0,255) );
	//SetDDraw.DD_OffScreen(Dib_Player->GetWidth(), Dib_Player->GetHeight(), &g_player.lpOff_Player, Dib_Player, 1, Convert565(0,0,0) );
	delete Dib_Player;
	*/	
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);	
	
	while(GAME)
	{	
		if(PeekMessage(&msg, NULL,0, 0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DD_DibDraw();
			if(GetAsyncKeyState(VK_ESCAPE)) GAME=0;
		}
	}
	
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	int wmId, wmEvent;

	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH]="";
	char szFileTitle[MAX_PATH];

	switch(iMsg)
	{
	case WM_MOUSEMOVE:		
		break;	
	case WM_RBUTTONDOWN:
		g_TActionFlag=2;
		break;
	case WM_LBUTTONDOWN:
		g_TActionFlag=3;
		break;
	case WM_RBUTTONUP:
		g_TActionFlag=0;
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		///////////////////////////////////////////// File
		case IDM_OPEN_MAP:
			{
				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hwnd;
				OFN.lpstrFilter = "MAP(*.map)\0*.map\0";
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = MAX_PATH;
				OFN.lpstrTitle = "MAP 파일 선택";
				OFN.lpstrFileTitle = szFileTitle;
				OFN.nMaxFileTitle = MAX_PATH;
				OFN.lpstrDefExt = "map";
				if(GetOpenFileName(&OFN) != 0)
				{
					OpenMap(lpstrFile);					
				}
			}
			break;
		case IDM_OPEN_SPRITE:
			{
				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hwnd;
				OFN.lpstrFilter = "SPRITE(*.spr)\0*.spr\0";
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = MAX_PATH;
				OFN.lpstrTitle = "SPRITE 파일 선택";
				OFN.lpstrFileTitle = szFileTitle;
				OFN.nMaxFileTitle = MAX_PATH;
				OFN.lpstrDefExt = "spr";
				if(GetOpenFileName(&OFN) != 0)
				{
					OpenSprite(lpstrFile);					
				}
			}
			break;
		case IDM_EXIT:
			GAME=0;
			PostQuitMessage(0);
			break;
		///////////////////////////////////////////// Sprite View
		case IDM_NULL_VIEW:
			{
				g_player->m_RectView = 0;
			}
			break;
		case IDM_MAP_CHK_RECT:
			{
				g_player->m_RectView = 1;
			}
			break;
		case IDM_DEFAULT_VIEW:
			{
				g_player->m_RectView = 2;
			}
			break;
		case IDM_DEFENSE_VIEW:
			{
				g_player->m_RectView = 3;
			}
			break;
		case IDM_ATTACK_VIEW:
			{
				g_player->m_RectView = 4;
			}
			break;
		case IDM_ETC_VIEW:
			{
				g_player->m_RectView = 5;
			}
			break;
		///////////////////////////////////////////// Map View
		case IDM_ATTRIBUTE_VIEW:
			{
				if(g_Map_Attr_View==0)
					g_Map_Attr_View=1;
				else
					g_Map_Attr_View=0;
			}
			break;
		case IDM_SET:
			{
				if(DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1),hwnd, SetDlgProc)==IDOK)
				{
				}
			}
			break;
		}
		break;
	case WM_MOVE:
		GetClientRect(hwnd, &g_rectWindow);
		ClientToScreen(hwnd, (LPPOINT)&g_rectWindow);
		ClientToScreen(hwnd, (LPPOINT)&g_rectWindow+1);
		break;	
	case WM_DESTROY:
		GAME=0;
		PostQuitMessage(0);
		return 0;			
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void DD_DibDraw()
{
	
	SetDDraw->DD_Restore();

	//SetDDraw.GetBackSurface()->Blt(0,lpOff_back,0,DDBLTFAST_WAIT,0);

	/////////////////////////////////////////////////////// 바탕이 될 검은색 스크린 출력	
	if(GetAsyncKeyState(VK_NUMPAD4))	g_Map->m_EditX+=20;	
	if(GetAsyncKeyState(VK_NUMPAD8))	g_Map->m_EditY+=20;
	if(GetAsyncKeyState(VK_NUMPAD6))	g_Map->m_EditX-=20;
	if(GetAsyncKeyState(VK_NUMPAD2))	g_Map->m_EditY-=20;
	
	if(SetDDraw->GetNormal())
		SetDDraw->GetBackOffScreenSurface()->Blt(0, lpOff_sb, 0, DDBLTFAST_WAIT, 0);
	else
		SetDDraw->GetBackSurface()->Blt(0, lpOff_sb, 0, DDBLTFAST_WAIT, 0);
	/////////////////////////////////////////////////////////////////////////////////////
	g_Map->DrawMap();
	
	if(g_Map_Attr_View)
	{
		char temp[10]={0,};
		char temp2[10]={0,};
		int tempint=0;
		for(int i=0;i<g_Map->m_Map.MapSizeX;i++)
		{
			for(int j=0;j<g_Map->m_Map.MapSizeY;j++)
			{			
				//wsprintf(temp,"%d",tempint);
				//SetDDraw->DDTextOut(j*g_Map->m_Map.TileSizeX+1, i* g_Map->m_Map.TileSizeY+1,temp);
				//tempint++;
				wsprintf(temp2,"%d", g_Map->m_Map.Attr[j][i]);
				SetDDraw->DDTextOut(i*g_Map->m_Map.TileSizeY+15+g_Map->m_EditX, 
					j* g_Map->m_Map.TileSizeY+10+g_Map->m_EditY,temp2);
			}
		}
	}

	DrawPlayer();
	
	if(SetDDraw->GetNormal())
		SetDDraw->GetPrimarySurface()->Blt(&g_rectWindow, SetDDraw->GetBackOffScreenSurface(), NULL, DDBLT_WAIT, NULL);
	else
		SetDDraw->GetPrimarySurface()->Flip(0,DDFLIP_WAIT);
}

void DrawPlayer()
{
	if ( GetAsyncKeyState(VK_LEFT) )
	{		
		g_Map->Mapchk(g_player,	LEFT, 1);
	}
	if ( GetAsyncKeyState(VK_UP) )
	{
		g_Map->Mapchk(g_player, TOP, 1);
	}
	if ( GetAsyncKeyState(VK_RIGHT) )
	{
		g_Map->Mapchk(g_player, RIGHT, 1);
	}	
	if ( GetAsyncKeyState(VK_DOWN) )
	{
		g_Map->Mapchk(g_player, BOTTOM, 1);
	}

	g_player->DrawSprite(g_player->m_X, g_player->m_Y, g_Sel_Action, 0, 1, g_Sel_FPS);	
}


void OpenSprite(char* path)
{
	delete g_player_Spr;
	delete g_player;
	g_player_Spr = new CSprite;
	g_player = new CObject;

	g_player_Spr->SetDrawBase(SetDDraw);
	g_player_Spr->LoadSprite(path);

	g_player->SetSpriteBase(g_player_Spr);
	g_player->m_X		= 100;
	g_player->m_Y		= 100;
	g_player->m_Move	= 8;
	g_player->m_jump	= 1000;
	g_player->m_gravity = 50;	
}

void OpenMap(char* path)
{
	delete g_Map;
	g_Map = new CMAP;
	g_Map->SetDrawBase(SetDDraw);
	g_Map->MapLoad(path);
}

BOOL CALLBACK SetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	WORD temp;
	switch(iMessage)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_EDIT_ACTION, g_Sel_Action, 0);
		SetDlgItemInt(hDlg, IDC_EDIT_FPS, g_Sel_FPS, 0);
		return 1;
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			temp = GetDlgItemInt(hDlg, IDC_EDIT_ACTION,0,0);
			if(temp < g_player->m_Sprite->m_MaxAction)
			{
				g_Sel_Action = temp;
				g_player->m_FrameCount = 0;
			}
			else
			{
				MessageBox(hDlg, "값이 초과했습니다.", "경고", MB_OK);
			}
			g_Sel_FPS = GetDlgItemInt(hDlg, IDC_EDIT_FPS,0,0);
			EndDialog(hDlg, IDOK);
			return 1;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return 1;
		}
		break;
	}
	return 0;
}