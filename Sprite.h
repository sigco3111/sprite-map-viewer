#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <windows.h>
#include <stdio.h>
#include <ddraw.h>
#include "DIB.h"
#include "SetDDraw.h"
#include "Timer.h"

struct FrameH
{
	WORD	FileID;
	WORD	ActionID;
	POINT	Center;
	WORD	Delay;
	
	RECT	Img_Rt;	
	
	WORD	MaxDefault;
	WORD	MaxDefense;
	WORD	MaxAttack;
	WORD	MaxEtc;
	
	RECT*	pDefault_Rt;
	RECT*	pDefense_Rt;
	RECT*	pAttack_Rt;
	RECT*	pEtc_Rt;
};

struct ActionH
{	
	WORD		MaxFrame;
	BYTE		R;
	BYTE		G;
	BYTE		B;
	RECT		MapChk_Rt;
	FrameH*		pFrame;
};

class CSprite  :CDIB
{
public:
	CSprite();
	~CSprite();
	
	void	LoadSprite(char* path);	
	void	SetDrawBase(SETDDRAW *DDrawBase){m_SetDDraw = DDrawBase;}

protected:	
	CDIB*		m_Dib_Spr;

public:	
	SETDDRAW*	m_SetDDraw;
	ActionH*	m_Ation;
	WORD		m_MaxImage;
	WORD		m_MaxAction;
	
	LPDIRECTDRAWSURFACE7*	m_lpOff_sprite;
};

#endif