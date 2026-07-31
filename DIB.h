#ifndef _CDIB_H_
#define _CDIB_H_

#include <windows.h>
#include <stdio.h>

#define Convert555(R, G, B) ((R >> 3) << 10 | (G >> 3) << 5 | (B >> 3))
#define Convert565(R, G, B) ((R >> 3) << 11 | (G >> 2) << 5 | (B >> 3))

#define offmax 100

class CDIB  
{
public:
	void Convert32To16(BOOL flag);
	void Convert24To16(BOOL flag);
	void Convert8To16(BOOL flag);
	void Convert16(BOOL flag);
	DWORD GetSizeImage();
	void Convert32To16(char *fileName, BOOL flag);
	void Convert24To16(char *fileName, BOOL flag);
	void Convert8To16(char *fileName, BOOL flag);
	void Convert16(char *fileName, BOOL flag);
	BYTE * GetImage();
	BOOL IsSuccessed();
	LONG GetHeight();
	LONG GetWidth();
	void Draw(HDC hdc,POINT point);
	void ConvertDDB(HDC hdc);
	void RemoveAll();
	CDIB(CDIB &r);
	CDIB(char *filename);
	BYTE GetByteCount();
	LONG GetBytePerLine();
	BOOL Load(char *filename);
	CDIB& operator=(CDIB &r);
	CDIB();
	virtual ~CDIB();

	void DrawDib(HDC hDC, int x, int y, int w, int h);	

protected:
	HBITMAP m_hOldBitmap;
	HDC m_MemDC;	
	RGBQUAD* m_pRgb;	
	BOOL m_bSuccess;

public:
	BITMAPINFOHEADER* m_pBmih;
	DWORD m_InfoSize;
	BITMAPINFO* m_pBmi;
	DWORD m_ImgSize;
	BYTE* m_pImage;
};

#endif // !defined(AFX_DIB_H__7B52C8B8_A0C3_42BF_A93E_E63CF2796EBF__INCLUDED_)

/*
CDIB dib("1.bmp")
g_lpBack->GetDC(&hdc);
StretchDIBits(..........)
*/