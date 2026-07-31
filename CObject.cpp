#include "CObject.h"

CObject::CObject()
{
	m_X				= 0;
	m_Y				= 0;
	m_FrameCount	= 0;

	m_Move			= 0;
	m_jump			= 0;
	m_jumpPower		= 0;
	m_gravity		= 0;

	m_RectView		= 0;

	m_Timer = new Timer;
}

CObject::~CObject()
{
	delete m_Timer;
}

BOOL CObject::DrawSprite(int x, int y, WORD Action, WORD Frame, BOOL AutoFrame, WORD SetFps, DDBLTFX *ddbfx, int flag)
{
	m_Timer->SetFPS(SetFps);
	m_Timer->Update();	
	
	if(AutoFrame == 1)
	{			
		m_Sprite->m_SetDDraw->DDBit(
			x - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.x,
			y - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.y,
			&m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Img_Rt,			
			m_Sprite->m_lpOff_sprite[m_Sprite->m_Ation[Action].pFrame[m_FrameCount].FileID],
			flag, ddbfx);

		if(m_RectView==1) MapChkView(Action);
		if(m_RectView==2) DefaultRtView(Action, m_FrameCount);
		if(m_RectView==3) DefenseRtView(Action, m_FrameCount);
		if(m_RectView==4) AttackRtView(Action, m_FrameCount);
		if(m_RectView==5) EtcRtView(Action, m_FrameCount);
	}
	else
	{
		m_Sprite->m_SetDDraw->DDBit(
			x - m_Sprite->m_Ation[Action].pFrame[Frame].Center.x,
			y - m_Sprite->m_Ation[Action].pFrame[Frame].Center.y,
			&m_Sprite->m_Ation[Action].pFrame[Frame].Img_Rt, 
			m_Sprite->m_lpOff_sprite[m_Sprite->m_Ation[Action].pFrame[Frame].FileID], 
			flag, ddbfx);

		if(m_RectView==1) MapChkView(Action);
		if(m_RectView==2) DefaultRtView(Action, m_FrameCount);
		if(m_RectView==3) DefenseRtView(Action, m_FrameCount);
		if(m_RectView==4) AttackRtView(Action, m_FrameCount);
		if(m_RectView==5) EtcRtView(Action, m_FrameCount);
	}

	if(m_FrameCount >= m_Sprite->m_Ation[Action].MaxFrame-1) 
	{
		m_FrameCount=0;
		return 1;		//프레임을 다 읽으면 0을 반환
	}
	
	if( (m_Timer->GetDrawChance() == 1) && (AutoFrame == 1) ) m_FrameCount++;	

	return 0;
}

RECT CObject::GetDefaultRect(WORD Action, WORD Frame)
{
	RECT rt;
	rt = m_Sprite->m_Ation[Action].pFrame[Frame].pDefault_Rt[Frame];
	rt.left		= rt.left	+ m_X;
	rt.top		= rt.top	+ m_Y;
	rt.right	= rt.right	+ m_X;
	rt.bottom	= rt.bottom	+ m_Y;
	return rt;
}
RECT CObject::GetDefenseRect(WORD Action, WORD Frame)
{
	RECT rt;
	rt = m_Sprite->m_Ation[Action].pFrame[Frame].pDefense_Rt[Frame];
	rt.left		= rt.left	+ m_X;
	rt.top		= rt.top	+ m_Y;
	rt.right	= rt.right	+ m_X;
	rt.bottom	= rt.bottom	+ m_Y;
	return rt;
}
RECT CObject::GetAttackRect(WORD Action, WORD Frame)
{
	RECT rt;
	rt = m_Sprite->m_Ation[Action].pFrame[Frame].pAttack_Rt[Frame];
	rt.left		= rt.left	+ m_X;
	rt.top		= rt.top	+ m_Y;
	rt.right	= rt.right	+ m_X;
	rt.bottom	= rt.bottom	+ m_Y;
	return rt;
}
RECT CObject::GetEtcRect(WORD Action, WORD Frame)
{
	RECT rt;
	rt = m_Sprite->m_Ation[Action].pFrame[Frame].pEtc_Rt[Frame];
	rt.left		= rt.left	+ m_X;
	rt.top		= rt.top	+ m_Y;
	rt.right	= rt.right	+ m_X;
	rt.bottom	= rt.bottom	+ m_Y;
	return rt;
}

BOOL CObject::Collision_Chk_Rect(CObject* pObject1, WORD Action1, WORD Frame1, 
								 CObject* pObject2, WORD Action2, WORD Frame2, BYTE flag)
{
	switch(flag)
	{
	case 1:
		{
			if( pObject1->GetDefaultRect(Action1, Frame1).right  < pObject2->GetDefaultRect(Action2, Frame2).left   ) 
				return FALSE;
			if( pObject1->GetDefaultRect(Action1, Frame1).left   > pObject2->GetDefaultRect(Action2, Frame2).right  ) 
				return FALSE;
			if( pObject1->GetDefaultRect(Action1, Frame1).bottom < pObject2->GetDefaultRect(Action2, Frame2).top    ) 
				return FALSE;
			if( pObject1->GetDefaultRect(Action1, Frame1).top    > pObject2->GetDefaultRect(Action2, Frame2).bottom ) 
				return FALSE;
		}
		break;
	case 2:
		{
			if( pObject1->GetDefenseRect(Action1, Frame1).right  < pObject2->GetDefenseRect(Action2, Frame2).left   ) 
				return FALSE;
			if( pObject1->GetDefenseRect(Action1, Frame1).left   > pObject2->GetDefenseRect(Action2, Frame2).right  ) 
				return FALSE;
			if( pObject1->GetDefenseRect(Action1, Frame1).bottom < pObject2->GetDefenseRect(Action2, Frame2).top    ) 
				return FALSE;
			if( pObject1->GetDefenseRect(Action1, Frame1).top    > pObject2->GetDefenseRect(Action2, Frame2).bottom ) 
				return FALSE;
		}
		break;
	case 3:
		{
			if( pObject1->GetAttackRect(Action1, Frame1).right  < pObject2->GetAttackRect(Action2, Frame2).left   ) 
				return FALSE;
			if( pObject1->GetAttackRect(Action1, Frame1).left   > pObject2->GetAttackRect(Action2, Frame2).right  ) 
				return FALSE;
			if( pObject1->GetAttackRect(Action1, Frame1).bottom < pObject2->GetAttackRect(Action2, Frame2).top    ) 
				return FALSE;
			if( pObject1->GetAttackRect(Action1, Frame1).top    > pObject2->GetAttackRect(Action2, Frame2).bottom ) 
				return FALSE;
		}
		break;
	case 4:
		{
			if( pObject1->GetEtcRect(Action1, Frame1).right  < pObject2->GetEtcRect(Action2, Frame2).left   ) 
				return FALSE;
			if( pObject1->GetEtcRect(Action1, Frame1).left   > pObject2->GetEtcRect(Action2, Frame2).right  ) 
				return FALSE;
			if( pObject1->GetEtcRect(Action1, Frame1).bottom < pObject2->GetEtcRect(Action2, Frame2).top    ) 
				return FALSE;
			if( pObject1->GetEtcRect(Action1, Frame1).top    > pObject2->GetEtcRect(Action2, Frame2).bottom ) 
				return FALSE;
		}
		break;		
	}

	//////// TRUE == 충돌
	return TRUE;
}

//////////////////////////////////////////////////////// 테스트 코드
void CObject::MapChkView(WORD Action)
{
	m_Sprite->m_SetDDraw->DDRectOut(
		Get_CX(Action) + m_Sprite->m_Ation[Action].MapChk_Rt.left,
		Get_CY(Action) + m_Sprite->m_Ation[Action].MapChk_Rt.top,
		Get_CX(Action) + m_Sprite->m_Ation[Action].MapChk_Rt.right,
		Get_CY(Action) + m_Sprite->m_Ation[Action].MapChk_Rt.bottom );
}

void CObject::DefaultRtView(WORD Action, WORD Frame)
{
	for(int i=0; i<m_Sprite->m_Ation[Action].pFrame[Frame].MaxDefault; i++)
	{
		m_Sprite->m_SetDDraw->DDRectOut(
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefault_Rt[i].left,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefault_Rt[i].top,
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefault_Rt[i].right,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefault_Rt[i].bottom,
			RGB(0, 255, 0));
	}
}

void CObject::DefenseRtView(WORD Action, WORD Frame)
{
	for(int i=0; i<m_Sprite->m_Ation[Action].pFrame[Frame].MaxDefense; i++)
	{
		m_Sprite->m_SetDDraw->DDRectOut(
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefense_Rt[i].left,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefense_Rt[i].top,
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefense_Rt[i].right,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pDefense_Rt[i].bottom,
			RGB(0, 255, 255));
	}
}

void CObject::AttackRtView(WORD Action, WORD Frame)
{
	for(int i=0; i<m_Sprite->m_Ation[Action].pFrame[Frame].MaxAttack; i++)
	{
		m_Sprite->m_SetDDraw->DDRectOut(
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pAttack_Rt[i].left,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pAttack_Rt[i].top,
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pAttack_Rt[i].right,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pAttack_Rt[i].bottom,
			RGB(255, 0, 0));
	}
}

void CObject::EtcRtView(WORD Action, WORD Frame)
{
	for(int i=0; i<m_Sprite->m_Ation[Action].pFrame[Frame].MaxEtc; i++)
	{
		m_Sprite->m_SetDDraw->DDRectOut(
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pEtc_Rt[i].left,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pEtc_Rt[i].top,
			Get_CX(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pEtc_Rt[i].right,
			Get_CY(Action) + m_Sprite->m_Ation[Action].pFrame[Frame].pEtc_Rt[i].bottom,
			RGB(255, 255, 255));
	}
}

/*BOOL CObject::Collision_chk_Circle(CObject* pObject1, WORD Action, WORD Frame, CObject* pObject2, BYTE flag)
{
	int TarX = pObject1->GetX();
	int TarY = pObject1->GetY();
	int TarR = pObject1->GetCrashRT()->right >> 1;
	int ObjX = pObject2->GetX();
	int ObjY = pObject2->GetY();
	int ObjR = pObject2->GetCrashRT()->right >> 1;
	
	int dx, dy, r, rr, dd;
	
	dx= (TarX+TarR)-(ObjX+ObjR); // x좌표 사이의 거리
	dy= (TarY+TarR)-(ObjY+ObjR); // y좌표 사이의 거리
	r= TarR+ObjR;
	
	rr =  r*r;// 반지름 합의 제곱
	dd = (dx*dx)+(dy*dy); // 중심 사이의 거리의 제곱
	
	if( rr  > dd ) // 충돌
		return TRUE;
	else
		return FALSE;
}*/