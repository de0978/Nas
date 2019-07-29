
#include "StdAfx.h"
#include "MyImage.h"
#include "RGBBYTE.h"
#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

// �⺻ ������. ��� ���� �ʱ�ȭ.
MyImage::MyImage()
{
	// ��� ���� �ʱ�ȭ
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;

	m_pDib = NULL;

	m_pPtr = NULL;
	m_pRgbPtr = NULL;
}

// ���� ������. ���ڷ� �Ѿ�� dib ��ü�� �����Ѵ�.
MyImage::MyImage(const MyImage& dib)
{
	// ��� ���� �ʱ�ȭ
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;

	m_nDibSize = 0;

	m_pDib = NULL;

	m_pPtr = NULL;
	m_pRgbPtr = NULL;

	if (dib.m_pDib != NULL)
	{
		// ���ڷκ��� DIB ������ �����Ѵ�.
		m_pDib = new BYTE[dib.m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);

		// ��� ���� �� ����
		InitDib();
	}
}

// �Ҹ���. DIB ��ü�� �����Ѵ�.
MyImage::~MyImage()
{
	Destroy();
}

// ������ ũ���� �׷��̽����� ������ �����Ѵ�.  ����, ���� DIB ��ü�� ������ �ִٸ� ���� �� �����Ѵ�.
BOOL MyImage::CreateGrayImage(int nWidth, int nHeight, BYTE value)
{
	// ���� ������ MyImage ��ü�� �ִٸ� �����Ѵ�.
	if (m_pDib) Destroy();

	int nBitCount = 8;       // �׷��̽����� �̹����� ��Ʈ �� �ȼ� ��

							 // ���� ���� �������� ũ��(����Ʈ ����)
	DWORD dwSizeImage = nHeight * (DWORD)((nWidth*nBitCount / 8 + 3)&~3);

	// (BITMAPINFOHEADER + ���� ���̺� + �ȼ� ������)�� ���� �޸� ���� �Ҵ�
	m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*(1 << nBitCount) + dwSizeImage];
	if (m_pDib == NULL) return FALSE;

	// BITMAPINFOHEADER ����ü �ʵ� ���� �����Ѵ�.
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = nWidth;
	lpbmi->biHeight = nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// ���� ���̺� ���� �׷��̽����� ���·� �����Ѵ�.
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)
	{
		pPal->rgbBlue = (BYTE)i;
		pPal->rgbGreen = (BYTE)i;
		pPal->rgbRed = (BYTE)i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	// ��� ���� �� ����
	InitDib();

	// �ȼ� ������ �� �ʱ�ȭ
	BYTE* pData = (BYTE*)GetDibBitsAddr();
	memset(pData, value, dwSizeImage);

	return TRUE;
}

// ������ ũ���� Ʈ�� �÷� ������ �����Ѵ�. ����, ���� DIB ��ü�� ������ �ִٸ� ���� �� �����Ѵ�.
BOOL MyImage::CreateRGBImage(int nWidth, int nHeight, COLORREF value)
{
	// ���� ������ MyImage ��ü�� �ִٸ� �����Ѵ�.
	if (m_pDib) Destroy();

	int nBitCount = 24;       // Ʈ��Į�� �̹���

							  // ���� ���� �������� ũ��(����Ʈ ����)
	DWORD dwSizeImage = nHeight * (DWORD)((nWidth*nBitCount / 8 + 3)&~3);

	// (BITMAPINFOHEADER + �ȼ� ������)�� ���� �޸� ���� �Ҵ�
	m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + dwSizeImage];
	if (m_pDib == NULL) return FALSE;

	// BITMAPINFOHEADER ����ü �ʵ� ���� �����Ѵ�.
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = nWidth;
	lpbmi->biHeight = nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// ��� ���� �� ����
	InitDib();

	// �ȼ� �� �ʱ�ȭ
	register int i, j;
	for (j = 0; j < m_nHeight; j++)
		for (i = 0; i < m_nWidth; i++)
		{
			// RGBBYTE Ŭ�������� = �� operator overloading �Ǿ��ִ�.
			m_pRgbPtr[j][i] = value;
		}

	return TRUE;
}

// m_nWidth, m_nHeight, m_nBitCount���� ��� ������ ���� �����Ѵ�. m_pDib�� BITMAPINFOHEADER�� ���۹����� ����Ű�� �ִ� ��Ȳ���� ȣ��Ǿ�� �Ѵ�.
// m_pPtr, m_pRgbPtr �������̽� ���� �����Ѵ�.
BOOL MyImage::InitDib()
{
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

	// �̹����� ���� ũ��, ���� ũ��, �ȼ��� ��Ʈ��, DIB ũ�⿡ ����
	// ��� ������ �����Ѵ�.
	m_nWidth = lpbmi->biWidth;
	m_nHeight = lpbmi->biHeight;
	m_nBitCount = lpbmi->biBitCount;

	DWORD dwSizeImage = m_nHeight * (DWORD)((m_nWidth*m_nBitCount / 8 + 3)&~3);

	m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*GetPaletteNums()
		+ dwSizeImage;

	// m_pPtr �Ǵ� m_pRgbPtr ��� ������ �����Ѵ�.
	if (!AllocPtr())
		return FALSE;

	return TRUE;
}

// �������� �Ҵ�� ��� �����͸� �����ϰ�, NULL�� �����Ѵ�. ��Ÿ ��� ������ �ʱ�ȭ�Ѵ�.
void MyImage::Destroy()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}

	FreePtr();  // m_pPtr �� m_pRgbPtr ��� ������ �Ҹ��Ų��.

				// ��Ÿ ��� ���� �ʱ�ȭ
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;
}

// ���ڷ� �Ѿ�� dib�� ������ MyImage ��ü�� �����Ѵ�.
MyImage& MyImage::operator=(const MyImage& dib)
{
	// ��� �˻�
	if (this == &dib)
		return *this;

	// dib�� ����ִ� MyImage ��ü��� �ڽŵ� ����.
	if (dib.m_pDib == NULL)
	{
		Destroy();
		return *this;
	}

	// ���� ������ MyImage ��ü�� �ִٸ� �����Ѵ�.
	if (m_pDib) Destroy();

	// (BITMAPINFOHEADER + �ȼ� ������)�� ���� �޸� ���� �Ҵ�
	m_pDib = new BYTE[dib.m_nDibSize];

	// DIB ���� ����
	memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);

	// ��� ���� �� ����
	InitDib();

	return *this;
}

// ���ڷ� �Ѿ�� pDib�� ����Ű�� MyImage ��ü�� �����Ѵ�.
BOOL MyImage::Copy(MyImage* pDib)
{
	// pDib�� ����ִ� MyImage ��ü�� ����Ű�� �ִٸ� �ڽŵ� ����.
	if (!pDib->IsValid())
	{
		Destroy();
		return TRUE;
	}

	// ���� ������ MyImage ��ü�� �ִٸ� �����Ѵ�.
	if (m_pDib) Destroy();

	// DIB�� ���� �޸� ���� �Ҵ�
	DWORD dwDibSize = pDib->GetDibSize();
	m_pDib = new BYTE[dwDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// DIB ���� ����
	BYTE* pbi = (BYTE*)pDib->GetBitmapInfoAddr();
	memcpy(m_pDib, pbi, dwDibSize);

	// ��� ���� �� ����
	InitDib();

	return TRUE;
}

// BMP ������ MyImage ��ü�� �ҷ��´�. Ȯ���ڸ� �˻��Ͽ� BMP �����̸� LoadBMP �Լ��� �ٽ� ȣ���Ѵ�.
BOOL MyImage::Load(LPCTSTR lpszFileName)
{
	CString filetype;
	filetype = lpszFileName;
	filetype.MakeUpper();

	if (filetype.Right(4) == ".BMP")
		return LoadBMP(lpszFileName);
	else
		return FALSE;
}

// MyImage ��ü�� BMP ���Ϸ� �����Ѵ�. Ȯ���ڸ� �˻��Ͽ� BMP �����̸� SaveBMP �Լ��� �ٽ� ȣ���Ѵ�.
BOOL MyImage::Save(LPCTSTR lpszFileName)
{
	CString filetype;
	filetype = lpszFileName;
	filetype.MakeUpper();

	if (filetype.Right(4) == ".BMP")
		return SaveBMP(lpszFileName);
	else
		return FALSE;
}

//  ��� ��ġ ��ǥ (0,0) ��ġ�� DIB�� ����Ѵ�.
void MyImage::Draw(HDC hDC)
{
	Draw(hDC, 0, 0);
}


/*
���� 
hDC - ��� ��ġ�� DC �ڵ�.
dx  - ��� ��ġ ��ġ�� x ��ǥ.
dy  - ��� ��ġ ��ġ�� y ��ǥ.
*/

// ��� ��ġ ��ǥ (dx,dy) ��ġ�� DIB�� ����Ѵ�.
void MyImage::Draw(HDC hDC, int dx, int dy)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	void* lpBits = (void*)GetDibBitsAddr();

	::SetDIBitsToDevice(hDC,	// hDC
		dx,					// DestX
		dy,					// DestY
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		0,					// SrcX
		0,					// SrcY
		0,					// nStartScan
		m_nHeight,			// nNumScans
		lpBits,				// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS);	// wUsage
}

/*
����
hDC - ��� ��ġ�� DC �ڵ�.
dx  - ��� ��ġ ��ġ�� x ��ǥ.
dy  - ��� ��ġ ��ġ�� y ��ǥ.
dw  - ����� ������ ���� ũ��
dh  - ����� ������ ���� ũ��
dwRop - ������ ���۷��̼� ����.
*/

// ��� ��ġ ��ǥ (dx,dy) ��ġ�� (dw, dh) ũ��� DIB�� ����Ѵ�.
void MyImage::Draw(HDC hDC, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

	::StretchDIBits(hDC,	// hDC
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		0,					// XSrc
		0,					// YSrc
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwROP
}


/*
����
hDC - ��� ��ġ�� DC �ڵ�.
dx  - ��� ��ġ ��ġ�� x ��ǥ.
dy  - ��� ��ġ ��ġ�� y ��ǥ.
dw  - ����� ������ ���� ũ��
dh  - ����� ������ ���� ũ��
dwRop - ������ ���۷��̼� ����.
sx  - ���� ������ �»�� x ��ǥ.
sy  - ���� ������ �»�� y ��ǥ.
sw  - ���� ���󿡼� ����� ������ ���� ũ��
sh  - ���� ���󿡼� ����� ������ ���� ũ��
*/

void MyImage::Draw(HDC hDC, int dx, int dy, int dw, int dh,
	int sx, int sy, int sw, int sh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

	::StretchDIBits(hDC,	// hDC
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		sx,					// XSrc
		sy,					// YSrc
		sw,					// nSrcWidth
		sh,					// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwROP
}

// m_pDib�� �����Ͽ�, �ȼ� �������� ���� �ּҸ� ��ȯ�Ѵ�.
LPVOID MyImage::GetDibBitsAddr()
{
	LPBITMAPINFOHEADER lpbmi;
	LPVOID lpDibBits;

	lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpDibBits = (LPVOID)((BYTE*)m_pDib + lpbmi->biSize
		+ sizeof(RGBQUAD)*GetPaletteNums());

	return lpDibBits;
}

// �ȷ�Ʈ���� �ʿ��� RGBQUAD ����ü �迭�� ������ ��ȯ�Ѵ�. Ʈ���÷��̸� 0�� ��ȯ�Ѵ�.
int MyImage::GetPaletteNums()
{
	switch (m_nBitCount)
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		return 0;
	}
}

// m_pPtr, m_pRgbPtr �������̽��� �����Ѵ�. �ȼ� �����Ͱ� bottom-up ���·� ����Ǿ��ִٰ� �����Ѵ�.
BOOL MyImage::AllocPtr()
{
	if (m_nBitCount == 8) // �׷��̽����� �̹���
	{
		if (m_pPtr) FreePtr();
		m_pPtr = new BYTE*[m_nHeight];
		if (!m_pPtr) return FALSE;

		int nRWidth = ((m_nWidth*m_nBitCount / 8 + 3)&~3);
		BYTE* pData = (BYTE*)GetDibBitsAddr();
		for (int i = 0; i < m_nHeight; i++)
			m_pPtr[i] = (BYTE*)(pData + (m_nHeight - i - 1)*nRWidth);
	}
	else if (m_nBitCount == 24)  // Ʈ��Į�� �̹���
	{
		if (m_pRgbPtr) FreePtr();
		m_pRgbPtr = new RGBBYTE*[m_nHeight];
		if (!m_pRgbPtr) return FALSE;

		int nRWidth = ((m_nWidth*m_nBitCount / 8 + 3)&~3);
		BYTE* pData = (BYTE*)GetDibBitsAddr();
		for (int i = 0; i < m_nHeight; i++)
			m_pRgbPtr[i] = (RGBBYTE*)(pData + (m_nHeight - i - 1)*nRWidth);
	}
	else
		return FALSE;

	return TRUE;
}

// m_pPtr, m_pRgbPtr �������̽��� �����Ѵ�.
void MyImage::FreePtr()
{
	if (m_pPtr != NULL)
	{
		delete[] m_pPtr;
		m_pPtr = NULL;
	}

	if (m_pRgbPtr != NULL)
	{
		delete[] m_pRgbPtr;
		m_pRgbPtr = NULL;
	}
}



/*
typedef struct tagBITMAPFILEHEADER { // bmfh
WORD    bfType;			// ��Ʈ���� ǥ�� (�׻� "BM")
DWORD   bfSize;			// ��ü ���� ũ��
WORD    bfReserved1;	// ����� ��. �׻� 0
WORD    bfReserved2;	// ����� ��. �׻� 0
DWORD   bfOffBits;		// BITMAPFILEHEADER���� �ȼ� �����ͱ����� �ɼ�
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{ // bmih
DWORD  biSize;			// �� ����ü�� ũ��
LONG   biWidth;			// �̹����� �� (�ȼ� ����)
LONG   biHeight;		// �̹����� ���� (�ȼ� ����)
WORD   biPlanes;		// ��Ʈ�÷��� �� (�׻� 1)
WORD   biBitCount		// �ȼ� �� ��Ʈ ��
DWORD  biCompression;	// ���� ����
DWORD  biSizeImage;		// �ȼ� �������� ũ��(����Ʈ ��)
LONG   biXPelsPerMeter;	// ���� �ػ�
LONG   biYPelsPerMeter; // ���� �ػ�
DWORD  biClrUsed;		// ���� ���Ǵ� ���� ��
DWORD  biClrImportant;	// �߿��� ���� �ε��� (0�̸� ��ü)
} BITMAPINFOHEADER;
*/

// BMP ������ �о MyImage ��ü�� �����Ѵ�.
BOOL MyImage::LoadBMP(LPCTSTR lpszFileName)
{
	CFile   file;
	UINT    nFileSize, nDibSize;

	BITMAPFILEHEADER    bmfh;
	LPBITMAPINFOHEADER  lpbmi;

	// �б� ���� ���� ����
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	// BMP ������ ũ�� (����Ʈ ����)
	nFileSize = (UINT)file.GetLength();

	// BITMAPFILEHEADER �б�
	if (file.Read((LPSTR)&bmfh, sizeof(BITMAPFILEHEADER))
		!= sizeof(BITMAPFILEHEADER))
	{
		file.Close();
		return FALSE;
	}

	// BMP �������� ��Ÿ���� "BM" ��Ŀ�� �ִ��� Ȯ��
	if (bmfh.bfType != DIB_HEADER_MARKER)
	{
		file.Close();
		return FALSE;
	}

	// Packed-DIB �޸� ���� ũ�� ���
	nDibSize = nFileSize - sizeof(bmfh);

	// ���� ������ MyImage ��ü�� �ִٸ� �����Ѵ�.
	if (m_pDib) Destroy();

	// ������ ����� ��ŭ�� �޸� �Ҵ�
	m_pDib = new BYTE[nDibSize];
	if (m_pDib == NULL)
	{
		file.Close();
		return FALSE;
	}

	// ���Ϸκ��� Packed-DIB ũ�⸸ŭ�� �б�
	if (file.Read(m_pDib, nDibSize) != nDibSize)
	{
		delete[] m_pDib;
		m_pDib = NULL;
		file.Close();
		return FALSE;
	}

	// BITMAPINFOHEADER ������ Ȯ���Ѵ�. 
	lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	if (lpbmi->biSize != sizeof(BITMAPINFOHEADER))
	{
		delete[] m_pDib;
		m_pDib = NULL;
		file.Close();
		return FALSE;
	}

	// ���� m_pDib�� DIB�� ����ִ� ������ ���� �ּҸ� ����Ű�� �ִ�. 
	// MyImage Ŭ������ ��� ������ ���� �����Ѵ�.
	if (!InitDib())
	{
		delete[] m_pDib;
		m_pDib = NULL;
		file.Close();
		return FALSE;
	}

	// ���� �ݱ�
	file.Close();

	return TRUE;
}

// MyImage ��ü�� BMP ���� �������� �����Ѵ�.
BOOL MyImage::SaveBMP(LPCTSTR lpszFileName)
{
	CFile	file;
	BITMAPFILEHEADER    bmfh;

	// DIB ��ü�� �����ϰ� ���� ������ FALSE�� ��ȯ.
	if (m_pDib == NULL)
		return FALSE;

	// ���� ���� ���� ����
	if (!file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite |
		CFile::shareDenyWrite))
		return FALSE;

	// BITMAPFILEHEADER ��� ���� ����
	bmfh.bfType = DIB_HEADER_MARKER;	// �׻� "BM"
	bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
	bmfh.bfReserved1 = 0;		// �׻� 0
	bmfh.bfReserved2 = 0;		// �׻� 0
	bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*GetPaletteNums());

	// BITMAPFILEHEADER ���� 
	file.Write((LPSTR)&bmfh, sizeof(bmfh));

	// DIB ����(BITMAPINFOHEADER + ���� ���̺� + �ȼ� ������) ����
	file.Write(m_pDib, m_nDibSize);

	// ���� �ݱ�
	file.Close();

	return TRUE;
}

