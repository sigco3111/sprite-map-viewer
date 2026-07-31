// DIB.cpp: implementation of the CDIB class.
//
//////////////////////////////////////////////////////////////////////

#include "DIB.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDIB::CDIB():m_pBmi(NULL),m_bSuccess(FALSE),m_InfoSize(0),m_pBmih(NULL),
			 m_pRgb(NULL),m_ImgSize(0),m_pImage(NULL),m_MemDC(NULL),
			 m_hOldBitmap(NULL)
{

}

CDIB::~CDIB()
{
	RemoveAll();
}

BOOL CDIB::Load(char *filename)
{
	FILE				*fp;
	BITMAPFILEHEADER	bmfh;

	fp = fopen(filename,"rb");
	if(fp == NULL)
	{
		MessageBox(NULL,"Load Error","CDIB::Load",MB_OK);
		return FALSE;
	}
	fread(&bmfh,sizeof(bmfh),1,fp);
	if(bmfh.bfType != *(WORD*)"BM")//if(bmfh.bfType != 0x4d42)
	{
		MessageBox(NULL,"IT's NOT BMP","CDIB::Load",MB_OK);
		return FALSE;
	}
	m_InfoSize = bmfh.bfOffBits - sizeof(bmfh);
	m_pBmi = (BITMAPINFO*)new BYTE[m_InfoSize];
	fread(m_pBmi,sizeof(BYTE),m_InfoSize,fp);

	m_pBmih = (BITMAPINFOHEADER*)m_pBmi;
	
	if(m_pBmih->biBitCount == 8)
	{
		m_pRgb = (RGBQUAD*)((BYTE *)m_pBmi + m_pBmih->biSize);
	}
	else if(m_pBmih->biBitCount < 8)
	{
		MessageBox(NULL,"Unsupported bmp","CDIB::Load",MB_OK);
		return FALSE;
	}
	m_ImgSize = GetBytePerLine() * abs(m_pBmih->biHeight);
	m_pImage = new BYTE[m_ImgSize];
	fread(m_pImage,sizeof(BYTE),m_ImgSize,fp);
	fclose(fp);

	if(m_pBmih->biHeight < 0)			//데카르트좌표를 사용하지 않은 BMP를 데카르트좌료로 바꿔준다.
	{
		LONG	i,XLen = GetBytePerLine();
		BYTE	*pImg = new BYTE[m_ImgSize];
		BYTE	*pDest = pImg;
		BYTE	*pSrc = m_pImage + m_ImgSize - XLen;
		
		m_pBmih->biHeight = abs(m_pBmih->biHeight);
		for(i=0;m_pBmih->biHeight;i++)
		{
			memcpy(pDest,pSrc,XLen);
			pDest += XLen;
			pSrc -= XLen;
		}
		memcpy(m_pImage,pImg,m_ImgSize);
		delete []pImg;
	}
	
	m_bSuccess = TRUE;
	return TRUE;
}


LONG CDIB::GetBytePerLine()
{
	if(m_pBmih == NULL)
	{
		MessageBox(NULL,"There's No BitmapinfoHeader","CDIB::GetBytePerLine()",MB_OK);
		return 0;
	}
	LONG XLen = m_pBmih->biWidth * GetByteCount();
	XLen = (XLen + 3) & ~3;
	return XLen;
}

BYTE CDIB::GetByteCount()
{
	if(m_pBmih == NULL)
	{
		MessageBox(NULL,"There's No BitmapinfoHeader","CDIB::GetByteCount()",MB_OK);
		return 0;
	}

	return m_pBmih->biBitCount/8;
}

CDIB::CDIB(char *filename):m_pBmi(NULL),m_bSuccess(FALSE),m_InfoSize(0),m_pBmih(NULL),
						   m_pRgb(NULL),m_ImgSize(0),m_pImage(NULL),m_MemDC(NULL),m_hOldBitmap(NULL)
{
	Load(filename);
}

CDIB::CDIB(CDIB &r):m_pBmi(NULL),m_pBmih(NULL),m_pImage(NULL),m_pRgb(NULL),m_MemDC(NULL),m_hOldBitmap(NULL)
{
	m_bSuccess = r.m_bSuccess;
	m_InfoSize = r.m_InfoSize;
	m_ImgSize = r.m_ImgSize;

	if(m_ImgSize)
	{
		m_pImage = new BYTE[m_ImgSize];
		memcpy(m_pImage,r.m_pImage,m_ImgSize);
	}

	if(m_InfoSize)
	{
		m_pBmi = (BITMAPINFO*) new BYTE[m_InfoSize];
		memcpy(m_pBmi,r.m_pBmi,m_InfoSize);
		m_pBmih = (BITMAPINFOHEADER*)m_pBmi;
		if(m_pBmih->biBitCount == 8)
		{
			m_pRgb = (RGBQUAD*)((BYTE*)m_pBmi + m_pBmih->biSize);
		}
	}

	if(r.m_MemDC)
	{
		HDC		hdc;
		hdc = GetDC(NULL);
		m_MemDC = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateDIBitmap(hdc,r.m_pBmih,CBM_INIT,
			r.m_pImage,r.m_pBmi,DIB_RGB_COLORS);
		m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC,hBitmap);
		ReleaseDC(NULL,hdc);
	}

}

CDIB& CDIB::operator=(CDIB &r)
{
	if(this == &r)
		return *this;

	RemoveAll();

	m_bSuccess = r.m_bSuccess;
	m_InfoSize = r.m_InfoSize;
	m_ImgSize = r.m_ImgSize;

	if(m_ImgSize)
	{
		m_pImage = new BYTE[m_ImgSize];
		memcpy(m_pImage,r.m_pImage,m_ImgSize);
	}

	if(m_InfoSize)
	{
		m_pBmi = (BITMAPINFO*) new BYTE[m_InfoSize];
		memcpy(m_pBmi,r.m_pBmi,m_InfoSize);
		m_pBmih = (BITMAPINFOHEADER*)m_pBmi;
		if(m_pBmih->biBitCount == 8)
		{
			m_pRgb = (RGBQUAD*)((BYTE*)m_pBmi + m_pBmih->biSize);
		}
	}

	if(r.m_MemDC)
	{
		HDC		hdc;
		hdc = GetDC(NULL);
		m_MemDC = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateDIBitmap(hdc,r.m_pBmih,CBM_INIT,
			r.m_pImage,r.m_pBmi,DIB_RGB_COLORS);
		m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC,hBitmap);
		ReleaseDC(NULL,hdc);
	}

	return *this;
	
}

void CDIB::RemoveAll()
{
	if(m_ImgSize)
	{
		delete []m_pImage;
		m_pImage = NULL;
	}
	if(m_pBmi)
	{
		delete [](BYTE*)m_pBmi;
		m_pBmi = NULL;
		m_pBmih = NULL;
		m_pRgb = NULL;
	}
	
	if(m_MemDC)
	{
		DeleteObject(SelectObject(m_MemDC,m_hOldBitmap));
		DeleteDC(m_MemDC);
	}
	m_MemDC = NULL;
	m_hOldBitmap = NULL;

	m_InfoSize = m_ImgSize = 0;
	m_bSuccess = FALSE;
}
		

void CDIB::ConvertDDB(HDC hdc)
{
	if(m_MemDC != NULL || m_bSuccess == FALSE)
	{
		MessageBox(NULL, "파일을 변환할 수 없습니다.", "ConvertDDB", MB_OK);
		return;
	}

	m_MemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateDIBitmap(hdc,m_pBmih,CBM_INIT,
		m_pImage,m_pBmi,DIB_RGB_COLORS);
	m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC,hBitmap);
}

void CDIB::Draw(HDC hdc, POINT point)
{
	if(!m_bSuccess)
		return;

	BitBlt(hdc,point.x,point.y,GetWidth(),GetHeight(),m_MemDC,0,0,SRCCOPY);
}

LONG CDIB::GetWidth()
{
	return m_pBmih->biWidth;
}

LONG CDIB::GetHeight()
{
	return m_pBmih->biHeight;
}

BOOL CDIB::IsSuccessed()
{
	return m_bSuccess;
}

BYTE * CDIB::GetImage()
{
	return m_pImage;
}

void CDIB::Convert16(char *fileName, BOOL flag)
{
	BYTE byteCount = GetByteCount();

	switch(byteCount)
	{
	case 1:						//256->16bit로 전환
		Convert8To16(fileName, flag);
		break;

	case 2:						//16bit끼리는 전환 할 필요가 없음.
		MessageBox(NULL, "본 파일은 16bit이므로 변환이 필요없습니다.",  "Convert16", MB_OK);
		break;

	case 3:						//24bit->16bit로 전환
		Convert24To16(fileName, flag);
		break;

	case  4:
		Convert32To16(fileName, flag);
		break;

	default:
		MessageBox(NULL, "본 파일은 변환할 수 없습니다..",  "Convert16", MB_OK);
		break;
	}
}

void CDIB::Convert8To16(char *fileName, BOOL flag)
{
	BITMAPFILEHEADER bmfh;
	
	ZeroMemory(&bmfh, sizeof(bmfh));
	bmfh.bfType = 0x4d42;
	bmfh.bfOffBits = sizeof(bmfh) + m_pBmih->biSize;

	BITMAPINFO* pBmi = (BITMAPINFO*)new BYTE[m_pBmih->biSize];
	CopyMemory(pBmi, m_pBmih, m_pBmih->biSize);
	pBmi->bmiHeader.biBitCount = 16;

	LONG xlen = pBmi->bmiHeader.biWidth * 2;
	xlen = (xlen + 3) & ~3;
	pBmi->bmiHeader.biSizeImage = xlen * GetHeight();

	WORD* pImage = new WORD[pBmi->bmiHeader.biSizeImage/2];
	BYTE* pSrc = m_pImage;
	WORD* pDest = pImage;
	BYTE index, R, G, B;
	LONG x, y;

	for(y = 0; y < GetHeight(); y++)
	{
		for(x = 0; x < GetWidth(); x++)
		{
			index = pSrc[x];
			R = m_pRgb[index].rgbRed;
			G = m_pRgb[index].rgbGreen;
			B = m_pRgb[index].rgbBlue;

			if(flag)
				pDest[x] = Convert565(R, G, B);
			else
				pDest[x] = Convert555(R, G, B);

		}
		pSrc += GetBytePerLine();				//pSrc = m_pImgae + m_BytePerLine;
		pDest += (xlen/2);
	}

	bmfh.bfSize = sizeof(bmfh) + m_pBmih->biSize + pBmi->bmiHeader.biSizeImage;

	FILE *fp = fopen(fileName, "wb");
	fwrite(&bmfh, sizeof(bmfh), 1, fp);
	fwrite(pBmi, m_pBmih->biSize, 1, fp);
	fwrite(pImage, pBmi->bmiHeader.biSizeImage, 1, fp);
	fclose(fp);

	delete [](BYTE*)pBmi;
	delete []pImage;						//delete [](BYTE*)pImage;
}

void CDIB::Convert24To16(char *fileName, BOOL flag)
{
	BITMAPFILEHEADER bmfh;
	
	// Save File Header Info
	ZeroMemory(&bmfh, sizeof(bmfh));
	bmfh.bfType = 0x4d42;
	bmfh.bfOffBits = sizeof(bmfh) + m_pBmih->biSize;

	// Save Info Header
	BITMAPINFO* pBmi = (BITMAPINFO*)new BYTE[m_pBmih->biSize];
	CopyMemory(pBmi, m_pBmih, m_pBmih->biSize);
	pBmi->bmiHeader.biBitCount = 16;

	// Save Image Size
	LONG xlen = pBmi->bmiHeader.biWidth * 2;
	xlen = (xlen + 3) & ~3;
	pBmi->bmiHeader.biSizeImage = xlen * GetHeight();

	// Save Image Data
	WORD* pImage = new WORD[pBmi->bmiHeader.biSizeImage / 2];
	BYTE* pSrc = m_pImage;
	WORD* pDest = pImage;
	BYTE R, G, B;
	LONG x, y;

	for(y = 0; y < GetHeight(); y++)
	{
		for(x = 0; x < GetWidth(); x++)
		{
			B = pSrc[x*3];
			G = pSrc[x*3+1];
			R = pSrc[x*3+2];

			if(flag)
				pDest[x] = Convert565(R, G, B);
			else
				pDest[x] = Convert555(R, G, B);
		}
		pSrc += GetBytePerLine();	
		pDest += (xlen/2);
	}

	// Save File Header Size
	bmfh.bfSize = sizeof(bmfh) + m_pBmih->biSize + pBmi->bmiHeader.biSizeImage;

	FILE *fp = fopen(fileName, "wb");
	fwrite(&bmfh, sizeof(bmfh), 1, fp);
	fwrite(pBmi, m_pBmih->biSize, 1, fp);
	fwrite(pImage, pBmi->bmiHeader.biSizeImage, 1, fp);
	fclose(fp);

	delete [](BYTE*)pBmi;
	delete []pImage;						//delete [](BYTE*)pImage;
}

void CDIB::Convert32To16(char *fileName, BOOL flag)
{
	BITMAPFILEHEADER bmfh;
	
	// Save File Header Info
	ZeroMemory(&bmfh, sizeof(bmfh));
	bmfh.bfType = 0x4d42;
	bmfh.bfOffBits = sizeof(bmfh) + m_pBmih->biSize;

	// Save Info Header
	BITMAPINFO* pBmi = (BITMAPINFO*)new BYTE[m_pBmih->biSize];
	CopyMemory(pBmi, m_pBmih, m_pBmih->biSize);
	pBmi->bmiHeader.biBitCount = 16;

	// Save Image Size
	LONG xlen = pBmi->bmiHeader.biWidth * 2;
	xlen = (xlen + 3) & ~3;
	pBmi->bmiHeader.biSizeImage = xlen * GetHeight();

	// Save Image Data
	WORD* pImage = new WORD[pBmi->bmiHeader.biSizeImage / 2];
	BYTE* pSrc = m_pImage;
	WORD* pDest = pImage;
	BYTE R, G, B;
	LONG x, y;

	for(y = 0; y < GetHeight(); y++)
	{
		for(x = 0; x < GetWidth(); x++)
		{
			B = pSrc[x*4];
			G = pSrc[x*4+1];
			R = pSrc[x*4+2];

			if(flag)
				pDest[x] = Convert565(R, G, B);
			else
				pDest[x] = Convert555(R, G, B);
		}
		pSrc += GetBytePerLine();							//pSrc = m_pImgae + m_XLen;
		pDest += (xlen/2);
	}
 
	// Save File Header Size
	bmfh.bfSize = sizeof(bmfh) + m_pBmih->biSize + pBmi->bmiHeader.biSizeImage;

	FILE *fp = fopen(fileName, "wb");
	fwrite(&bmfh, sizeof(bmfh), 1, fp);
	fwrite(pBmi, m_pBmih->biSize, 1, fp);
	fwrite(pImage, pBmi->bmiHeader.biSizeImage, 1, fp);
	fclose(fp);

	delete [](BYTE*)pBmi;
	delete []pImage;						//delete [](BYTE*)pImage;
}

DWORD CDIB::GetSizeImage()
{
	DWORD size;
	size = GetBytePerLine() * GetHeight();

	return size;
}

void CDIB::Convert16(BOOL flag)
{
	BYTE byteCount = GetByteCount();

	switch(byteCount)
	{
	case 1:						//256->16bit로 전환
		Convert8To16(flag);
		break;

	case 2:						//16bit끼리는 전환 할 필요가 없음.
		MessageBox(NULL, "본 파일은 16bit이므로 변환이 필요없습니다.",  "Convert16", MB_OK);
		break;

	case 3:						//24bit->16bit로 전환
		Convert24To16(flag);
		break;

	case  4:
		Convert32To16(flag);
		break;

	default:
		MessageBox(NULL, "본 파일은 변환할 수 없습니다..",  "Convert16", MB_OK);
		break;
	}
}

void CDIB::Convert8To16(BOOL flag)
{
	BITMAPINFO* pBmi = (BITMAPINFO*)new BYTE[m_pBmih->biSize];
	CopyMemory(pBmi, m_pBmih, m_pBmih->biSize);
	pBmi->bmiHeader.biBitCount = 16;

	LONG xlen = pBmi->bmiHeader.biWidth * 2;
	xlen = (xlen + 3) & ~3;
	pBmi->bmiHeader.biSizeImage = xlen * GetHeight();

	WORD* pImage = new WORD[pBmi->bmiHeader.biSizeImage/2];
	BYTE* pSrc = m_pImage;
	WORD* pDest = pImage;
	BYTE index, R, G, B;
	LONG x, y;

	for(y = 0; y < GetHeight(); y++)
	{
		for(x = 0; x < GetWidth(); x++)
		{
			index = pSrc[x];
			R = m_pRgb[index].rgbRed;
			G = m_pRgb[index].rgbGreen;
			B = m_pRgb[index].rgbBlue;

			if(flag)
				pDest[x] = Convert565(R, G, B);
			else
				pDest[x] = Convert555(R, G, B);

		}
		pSrc += GetBytePerLine();				//pSrc = m_pImgae + m_BytePerLine;
		pDest += (xlen/2);
	}

	delete [](BYTE *)m_pBmi;
	m_pBmi = (BITMAPINFO*)new BYTE[pBmi->bmiHeader.biSize];
	CopyMemory(m_pBmi, pBmi, pBmi->bmiHeader.biSize);
	m_pBmih = (BITMAPINFOHEADER *)m_pBmi;
	m_pRgb = NULL;
	
	m_InfoSize = m_pBmi->bmiHeader.biSize;
	m_ImgSize = m_pBmi->bmiHeader.biSizeImage;

	delete []m_pImage;
	m_pImage = new BYTE[m_ImgSize];

	CopyMemory(m_pImage, pImage, m_ImgSize);

	delete [](BYTE*)pBmi;
	delete []pImage;						//delete [](BYTE*)pImage;
}

void CDIB::Convert24To16(BOOL flag)
{
	BITMAPINFO* pBmi = (BITMAPINFO*)new BYTE[m_pBmih->biSize];
	CopyMemory(pBmi, m_pBmih, m_pBmih->biSize);
	pBmi->bmiHeader.biBitCount = 16;

	LONG xlen = pBmi->bmiHeader.biWidth * 2;
	xlen = (xlen + 3) & ~3;
	pBmi->bmiHeader.biSizeImage = xlen * GetHeight();

	WORD* pImage = new WORD[pBmi->bmiHeader.biSizeImage/2];
	BYTE* pSrc = m_pImage;
	WORD* pDest = pImage;
	BYTE R, G, B;
	LONG x, y;

	for(y = 0; y < GetHeight(); y++)
	{
		for(x = 0; x < GetWidth(); x++)
		{
			B = pSrc[x*3];
			G = pSrc[x*3+1];
			R = pSrc[x*3+2];

			if(flag)
				pDest[x] = Convert565(R, G, B);
			else
				pDest[x] = Convert555(R, G, B);
		}
		pSrc += GetBytePerLine();	
		pDest += (xlen/2);
	}

	delete [](BYTE *)m_pBmi;
	m_pBmi = (BITMAPINFO*)new BYTE[pBmi->bmiHeader.biSize];
	CopyMemory(m_pBmi, pBmi, pBmi->bmiHeader.biSize);
	m_pBmih = (BITMAPINFOHEADER *)m_pBmi;
	m_pRgb = NULL;
	
	m_InfoSize = m_pBmi->bmiHeader.biSize;
	m_ImgSize = m_pBmi->bmiHeader.biSizeImage;

	delete []m_pImage;
	m_pImage = new BYTE[m_ImgSize];

	CopyMemory(m_pImage, pImage, m_ImgSize);

	delete [](BYTE*)pBmi;
	delete []pImage;						//delete [](BYTE*)pImage;
}

void CDIB::Convert32To16(BOOL flag)
{
	BITMAPINFO* pBmi = (BITMAPINFO*)new BYTE[m_pBmih->biSize];
	CopyMemory(pBmi, m_pBmih, m_pBmih->biSize);
	pBmi->bmiHeader.biBitCount = 16;

	LONG xlen = pBmi->bmiHeader.biWidth * 2;
	xlen = (xlen + 3) & ~3;
	pBmi->bmiHeader.biSizeImage = xlen * GetHeight();

	WORD* pImage = new WORD[pBmi->bmiHeader.biSizeImage/2];
	BYTE* pSrc = m_pImage;
	WORD* pDest = pImage;
	BYTE R, G, B;
	LONG x, y;

	for(y = 0; y < GetHeight(); y++)
	{
		for(x = 0; x < GetWidth(); x++)
		{
			B = pSrc[x*4];
			G = pSrc[x*4+1];
			R = pSrc[x*4+2];

			if(flag)
				pDest[x] = Convert565(R, G, B);
			else
				pDest[x] = Convert555(R, G, B);
		}
		pSrc += GetBytePerLine();							//pSrc = m_pImgae + m_XLen;
		pDest += (xlen/2);
	}


	delete [](BYTE *)m_pBmi;
	m_pBmi = (BITMAPINFO*)new BYTE[pBmi->bmiHeader.biSize];
	CopyMemory(m_pBmi, pBmi, pBmi->bmiHeader.biSize);
	m_pBmih = (BITMAPINFOHEADER *)m_pBmi;
	m_pRgb = NULL;
	
	m_InfoSize = m_pBmi->bmiHeader.biSize;
	m_ImgSize = m_pBmi->bmiHeader.biSizeImage;

	delete []m_pImage;
	m_pImage = new BYTE[m_ImgSize];

	CopyMemory(m_pImage, pImage, m_ImgSize);

	delete [](BYTE*)pBmi;
	delete []pImage;						//delete [](BYTE*)pImage;
}

void CDIB::DrawDib(HDC hDC, int x, int y, int w, int h)
{
	if(w<1||h<1)
	{
		StretchDIBits(hDC, x, y, GetWidth(), GetHeight(), 0, 0, GetWidth(), GetHeight(), m_pImage, m_pBmi, DIB_RGB_COLORS, SRCCOPY);		
	}
	else
	StretchDIBits(hDC, x, y, w, h,0, 0, GetWidth(), GetHeight(), m_pImage, m_pBmi, DIB_RGB_COLORS, SRCCOPY);
}

