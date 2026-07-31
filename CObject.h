#ifndef _COBJECT_H_
#define _COBJECT_H_

#include "Sprite.h"

class CObject
{
protected:
	void MapChkView(WORD Action);
	void DefaultRtView(WORD Action, WORD Frame);
	void DefenseRtView(WORD Action, WORD Frame);
	void AttackRtView(WORD Action, WORD Frame);
	void EtcRtView(WORD Action, WORD Frame);
	Timer*		m_Timer;	
	
public:
	CObject();
	~CObject();
	
	void SetSpriteBase(CSprite* SpriteBase){m_Sprite = SpriteBase;}
	
	BOOL DrawSprite(int x, int y, WORD Action, WORD Frame=0, BOOL AutoFrame=0, WORD SetFps=50, DDBLTFX *ddbfx = 0, int flag = DDBLT_WAIT | DDBLT_KEYSRC);
	
	RECT GetDefaultRect(WORD Action, WORD Frame);
	RECT GetDefenseRect(WORD Action, WORD Frame);
	RECT GetAttackRect(WORD Action, WORD Frame);
	RECT GetEtcRect(WORD Action, WORD Frame);
	
	inline int Get_CX(WORD Action, WORD Frame=0)
	{return m_X - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.x;}	//중심점이 적용된 X좌표	
	inline int Get_CY(WORD Action, WORD Frame=0)
	{return m_Y - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.y;}	//중심점이 적용된 Y좌표

	//flag =  1. 일반영역, 2. 방어영역, 3. 공격영역, 4.기타
	BOOL  Collision_Chk_Rect(CObject* pObject1, WORD Action1, WORD Frame1, 
								 CObject* pObject2, WORD Action2, WORD Frame2, BYTE flag);	// 사각 영역 충돌체크
	//BOOL  Collision_Chk_Circle(CObject* pObject1, WORD Action1, WORD Frame1, 
	//							 CObject* pObject2, WORD Action2, WORD Frame2, BYTE flag);	// 원형 충돌체크
	
	int		m_X;			// X좌표
	int		m_Y;			// Y좌표	

	int     m_FrameCount;

	int		m_Move;			//이동거리
	BOOL	m_jump;			//점프 상태
	int		m_jumpPower;	//점프력	
	int		m_gravity;		//중력값

	CSprite*	m_Sprite;	//CSprite 클레스에서 빌려서 사용하는 것이니 여기서 절대 delete하지 말것.
	BYTE		m_RectView;
};

#endif _COBJECT_H_