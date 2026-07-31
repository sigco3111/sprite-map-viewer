#include "Sprite.h"

CSprite::CSprite()
{
}

CSprite::~CSprite()
{
	delete [] m_Ation->pFrame->pDefault_Rt;
	delete [] m_Ation->pFrame->pDefense_Rt;
	delete [] m_Ation->pFrame->pAttack_Rt;
	delete [] m_Ation->pFrame->pEtc_Rt;
	delete [] m_Ation->pFrame;
	delete [] m_Ation;
	delete [] m_lpOff_sprite;
}

void CSprite::LoadSprite(char* path)
{	
	FILE *fSpr;
	int i=0, j=0, k=0;
	
	if((fSpr=fopen(path,"r+b"))!=NULL)
	{
		fread(&m_MaxImage, sizeof(WORD), 1, fSpr);				//------------------	 1-1. 로드할 이미지 갯수
		fread(&m_MaxAction, sizeof(WORD), 1, fSpr);				//------------------	 1-2. 액션의 총 갯수
		
		m_Ation = new ActionH[m_MaxAction];
		
		for( i=0; i<m_MaxAction; i++)
		{
			fread(&m_Ation[i].MaxFrame, sizeof(WORD), 1, fSpr);	//------------------	 2-1. 프레임의 총 갯수
			fread(&m_Ation[i].R, sizeof(BYTE), 1, fSpr);		//------------------	 2-2. 컬러키 R
			fread(&m_Ation[i].G, sizeof(BYTE), 1, fSpr);		//------------------	 2-3. 컬러키 G
			fread(&m_Ation[i].B, sizeof(BYTE), 1, fSpr);		//------------------	 2-4. 컬러키 B
			fread(&m_Ation[i].MapChk_Rt, sizeof(RECT), 1, fSpr);//------------------	 2-5. 타일맵 체크 사각영역
			
			m_Ation[i].pFrame = new FrameH[m_Ation[i].MaxFrame];
			for( j=0; j<m_Ation[i].MaxFrame; j++)
			{
				fread(&m_Ation[i].pFrame[j].FileID, sizeof(WORD), 1, fSpr);		//------------------	3-1. 파일의 ID
				fread(&m_Ation[i].pFrame[j].ActionID, sizeof(WORD), 1, fSpr);	//------------------	3-2. 액션의 ID
				fread(&m_Ation[i].pFrame[j].Center, sizeof(POINT), 1, fSpr);	//------------------	3-3. 중심점
				fread(&m_Ation[i].pFrame[j].Delay, sizeof(WORD), 1, fSpr);		//------------------	3-4. 프레임 지연 시간
				
				fread(&m_Ation[i].pFrame[j].Img_Rt, sizeof(RECT), 1, fSpr);		//------------------	3-5. 이미지의 출력될 위치
				
				fread(&m_Ation[i].pFrame[j].MaxDefault, sizeof(WORD), 1, fSpr);	//------------------	3-6. 통상 영역 설정값의 갯수
				fread(&m_Ation[i].pFrame[j].MaxDefense, sizeof(WORD), 1, fSpr);	//------------------	3-7. 방어 영역 설정값의 갯수
				fread(&m_Ation[i].pFrame[j].MaxAttack, sizeof(WORD), 1, fSpr);	//------------------	3-8. 공격 영역 설정값의 갯수
				fread(&m_Ation[i].pFrame[j].MaxEtc, sizeof(WORD), 1, fSpr);		//------------------	3-9. 기타 영역 설정값의 갯수
				
				m_Ation[i].pFrame[j].pDefault_Rt	= new RECT[m_Ation[i].pFrame[j].MaxDefault];
				m_Ation[i].pFrame[j].pDefense_Rt	= new RECT[m_Ation[i].pFrame[j].MaxDefense];
				m_Ation[i].pFrame[j].pAttack_Rt		= new RECT[m_Ation[i].pFrame[j].MaxAttack];
				m_Ation[i].pFrame[j].pEtc_Rt		= new RECT[m_Ation[i].pFrame[j].MaxEtc];
				
				for(k=0; k<m_Ation[i].pFrame[j].MaxDefault; k++)
				{
					fread(&m_Ation[i].pFrame[j].pDefault_Rt[k], sizeof(RECT), 1, fSpr);	//------------------	3-10. 통상 영역 설정값
				}
				for(k=0; k<m_Ation[i].pFrame[j].MaxDefense; k++)
				{
					fread(&m_Ation[i].pFrame[j].pDefense_Rt[k], sizeof(RECT), 1, fSpr);	//------------------	3-11. 방어 영역 설정값
				}
				for(k=0; k<m_Ation[i].pFrame[j].MaxAttack; k++)
				{
					fread(&m_Ation[i].pFrame[j].pAttack_Rt[k], sizeof(RECT), 1, fSpr);	//------------------	3-12. 공격 영역 설정값
				}
				for(k=0; k<m_Ation[i].pFrame[j].MaxEtc; k++)
				{
					fread(&m_Ation[i].pFrame[j].pEtc_Rt[k], sizeof(RECT), 1, fSpr);		//------------------	3-13. 기타 영역 설정값
				}				
			}			
		}
		
		i=0; j=0; k=0;
		m_lpOff_sprite	= new LPDIRECTDRAWSURFACE7[m_MaxImage];
		for(i=0; i<m_MaxImage; i++)
		{
			////////////////////////////////////////////////////////////////////////// 비트맵 로드			
			m_Dib_Spr			= new CDIB;

			WORD bfSize, bfOffBits;
			fread(&bfSize, sizeof(WORD), 1, fSpr);							//------------------	4-1. 비트맵 파일 사이즈
			fread(&bfOffBits, sizeof(WORD), 1, fSpr);						//------------------	4-2. OffBits
			
			m_Dib_Spr->m_pBmi = (BITMAPINFO*)new BYTE[bfOffBits-sizeof(BITMAPFILEHEADER)];			
			fread(m_Dib_Spr->m_pBmi, sizeof(BITMAPINFO), 1, fSpr);			//------------------	4-3. 비트맵 인포 헤더
			
			m_Dib_Spr->m_pBmih = (BITMAPINFOHEADER*)m_Dib_Spr->m_pBmi;			
			m_Dib_Spr->m_ImgSize = m_Dib_Spr->GetBytePerLine() * abs(m_Dib_Spr->m_pBmih->biHeight);
			
			m_Dib_Spr->m_pImage = new BYTE[m_Dib_Spr->m_ImgSize];			
			fread(m_Dib_Spr->m_pImage, sizeof(BYTE), m_Dib_Spr->m_ImgSize, fSpr);		//------------------	4-4. 비트 정보
			
			if(m_Dib_Spr->m_pBmih->biHeight < 0)			//데카르트좌표를 사용하지 않은 BMP를 데카르트좌료로 바꿔준다.
			{
				LONG	l,XLen	= m_Dib_Spr->GetBytePerLine();
				BYTE	*pImg	= new BYTE[m_Dib_Spr->m_ImgSize];
				BYTE	*pDest	= pImg;
				BYTE	*pSrc	= m_Dib_Spr->m_pImage + m_Dib_Spr->m_ImgSize - XLen;
				
				m_Dib_Spr->m_pBmih->biHeight = abs(m_Dib_Spr->m_pBmih->biHeight);
				for(l=0; m_Dib_Spr->m_pBmih->biHeight; l++)
				{
					memcpy(pDest,pSrc,XLen);
					pDest += XLen;
					pSrc -= XLen;
				}
				memcpy(m_Dib_Spr->m_pImage, pImg, m_Dib_Spr->m_ImgSize);
				delete []pImg;
			}
			
			////////////////////////////////////////////////////////////////////////// 오프스크린 생성			
		
			m_SetDDraw->DD_OffScreen(
				m_Dib_Spr->GetWidth(), m_Dib_Spr->GetHeight(), 
				&m_lpOff_sprite[i],
				m_Dib_Spr, 1,
				Convert565( 
				m_Ation[0].R, 
				m_Ation[0].G, 
				m_Ation[0].B)
				);
			delete m_Dib_Spr;
			
			/*int a=0;
			if(j==0)
			{
				a=a+m_Ation[k].MaxFrame;
			}
			for(k=0; k<j; k++)
			{
				a=a+m_Ation[k].MaxFrame;
			}
			
			j = i/(a+1);

			m_SetDDraw->DD_OffScreen(
				m_Dib_Spr->GetWidth(), m_Dib_Spr->GetHeight(), 
				&m_lpOff_sprite[i],
				m_Dib_Spr, 1,
				Convert565( 
				m_Ation[j].R,
				m_Ation[j].G, 
				m_Ation[j].B)
				);
			delete m_Dib_Spr;*/
		}		
	}
}