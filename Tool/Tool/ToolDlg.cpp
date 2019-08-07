
// ToolDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolDlg.h"
#include "afxdialogex.h"
#include "BinarizationDlg.h"
#include "ImageEdit.h"
#include <math.h>
#include <fstream>
#include <iostream>

using namespace std;


#define Image0 9506150
#define Image1 9506151
#define Image2 9506152
#define Image3 9506153
#define PI 3.14159

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CToolDlg ��ȭ ����



CToolDlg::CToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOOL_DIALOG, pParent), m_ROIFlags(-1), m_ROIDrag(FALSE), m_dragFlag(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD0, &CToolDlg::OnBnClickedButtonLoad0)
	ON_BN_CLICKED(IDC_BUTTON_LOAD1, &CToolDlg::OnBnClickedButtonLoad1)
	ON_BN_CLICKED(IDC_BUTTON_LOAD2, &CToolDlg::OnBnClickedButtonLoad2)
	ON_BN_CLICKED(IDC_BUTTON_LOAD3, &CToolDlg::OnBnClickedButtonLoad3)
	ON_BN_CLICKED(IDC_BUTTON_BIN0, &CToolDlg::OnBnClickedButtonBin0)
	ON_BN_CLICKED(IDC_BUTTON_ROI0, &CToolDlg::OnBnClickedButtonRoi0)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_HOLE0, &CToolDlg::OnBnClickedButtonHole0)
	ON_BN_CLICKED(IDC_BUTTON_ROI1, &CToolDlg::OnBnClickedButtonRoi1)
	ON_BN_CLICKED(IDC_BUTTON_ROI2, &CToolDlg::OnBnClickedButtonRoi2)
	ON_BN_CLICKED(IDC_BUTTON_ROI3, &CToolDlg::OnBnClickedButtonRoi3)
	ON_BN_CLICKED(IDC_BUTTON_BIN1, &CToolDlg::OnBnClickedButtonBin1)
	ON_BN_CLICKED(IDC_BUTTON_BIN2, &CToolDlg::OnBnClickedButtonBin2)
	ON_BN_CLICKED(IDC_BUTTON_BIN3, &CToolDlg::OnBnClickedButtonBin3)
	ON_BN_CLICKED(IDC_BUTTON_HOLE1, &CToolDlg::OnBnClickedButtonHole1)
	ON_BN_CLICKED(IDC_BUTTON_HOLE2, &CToolDlg::OnBnClickedButtonHole2)
	ON_BN_CLICKED(IDC_BUTTON_HOLE3, &CToolDlg::OnBnClickedButtonHole3)
	ON_BN_CLICKED(IDC_BUTTON3, &CToolDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CToolDlg �޽��� ó����

BOOL CToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_nTh0 = m_nTh1 = m_nTh2 = m_nTh3 = -1;

	SettingLoad();



	m_font.CreatePointFont(180, (LPCTSTR)"����");

	GetDlgItem(IDC_STATIC)->SetFont(&m_font, TRUE);
	GetDlgItem(IDC_STATIC_1)->SetFont(&m_font, TRUE);
	GetDlgItem(IDC_STATIC_2)->SetFont(&m_font, TRUE);
	GetDlgItem(IDC_STATIC_3)->SetFont(&m_font, TRUE);

	// ���� �ڽ��� ���� ������ ���̸� ����
	CWnd* pImageWnd = GetDlgItem(IDC_IMAGE_WND0);
	pImageWnd->GetClientRect(rect);


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE)
	{
		//�����ư ���� ��
		SettingSave();
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CToolDlg::OnPaint()
{


	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialogEx::OnPaint();

		if (m_image0.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND0));
			m_image0.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}
		if (m_image1.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND1));
			m_image1.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}
		if (m_image2.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND2));
			m_image2.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}
		if (m_image3.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND3));
			m_image3.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}

		if (m_ROIDrag == TRUE) {
			if (m_ROIFlags == Image0) {
				CClientDC dc(GetDlgItem(IDC_IMAGE_WND0));
				SetROIPos(IDC_IMAGE_WND0, m_sPos);
				CPen pen0;
				pen0.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ������ �� ����
				CPen* oldPen = dc.SelectObject(&pen0);
				dc.MoveTo(m_sPos);
				dc.LineTo(m_pos.x, m_sPos.y);
				dc.LineTo(m_pos);
				dc.LineTo(m_sPos.x, m_pos.y);
				dc.LineTo(m_sPos);
				dc.SelectObject(oldPen);
			}
			else if (m_ROIFlags == Image1) {
				CClientDC dc(GetDlgItem(IDC_IMAGE_WND1));
				SetROIPos(IDC_IMAGE_WND1, m_sPos);
				CPen pen;
				pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));   // ������ �� ����
				CPen* oldPen = dc.SelectObject(&pen);
				dc.MoveTo(m_sPos);
				dc.LineTo(m_pos.x, m_sPos.y);
				dc.LineTo(m_pos);
				dc.LineTo(m_sPos.x, m_pos.y);
				dc.LineTo(m_sPos);
				dc.SelectObject(oldPen);
			}
			else if (m_ROIFlags == Image2) {
				CClientDC dc(GetDlgItem(IDC_IMAGE_WND2));
				SetROIPos(IDC_IMAGE_WND2, m_sPos);
				CPen pen;
				pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));   // ������ �� ����
				CPen* oldPen = dc.SelectObject(&pen);
				dc.MoveTo(m_sPos);
				dc.LineTo(m_pos.x, m_sPos.y);
				dc.LineTo(m_pos);
				dc.LineTo(m_sPos.x, m_pos.y);
				dc.LineTo(m_sPos);
				dc.SelectObject(oldPen);
			}
			else if (m_ROIFlags == Image3) {
				CClientDC dc(GetDlgItem(IDC_IMAGE_WND3));
				SetROIPos(IDC_IMAGE_WND3, m_sPos);
				CPen pen;
				pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ������ �� ����
				CPen* oldPen = dc.SelectObject(&pen);
				dc.MoveTo(m_sPos);
				dc.LineTo(m_pos.x, m_sPos.y);
				dc.LineTo(m_pos);
				dc.LineTo(m_sPos.x, m_pos.y);
				dc.LineTo(m_sPos);
				dc.SelectObject(oldPen);
			}
		}


		if (m_roi0.m_drawROI == TRUE) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND0));
			//SetROIPos(IDC_IMAGE_WND0, m_sPos);
			CPen pen4;
			pen4.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen4);
			dc.MoveTo(m_roi0.GetPos());
			dc.LineTo(m_roi0.GetdPos().x, m_roi0.GetPos().y);
			dc.LineTo(m_roi0.GetdPos());
			dc.LineTo(m_roi0.GetPos().x, m_roi0.GetdPos().y);
			dc.LineTo(m_roi0.GetPos());
			dc.SelectObject(oldPen);
		}
		if (m_roi1.m_drawROI == TRUE) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND1));

			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(m_roi1.GetPos());
			dc.LineTo(m_roi1.GetdPos().x, m_roi1.GetPos().y);
			dc.LineTo(m_roi1.GetdPos());
			dc.LineTo(m_roi1.GetPos().x, m_roi1.GetdPos().y);
			dc.LineTo(m_roi1.GetPos());
			dc.SelectObject(oldPen);
		}
		if (m_roi2.m_drawROI == TRUE) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND2));
			//SetROIPos(IDC_IMAGE_WND2, m_sPos);
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(m_roi2.GetPos());
			dc.LineTo(m_roi2.GetdPos().x, m_roi2.GetPos().y);
			dc.LineTo(m_roi2.GetdPos());
			dc.LineTo(m_roi2.GetPos().x, m_roi2.GetdPos().y);
			dc.LineTo(m_roi2.GetPos());
			dc.SelectObject(oldPen);
		}
		if (m_roi3.m_drawROI == TRUE) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND3));
			//SetROIPos(IDC_IMAGE_WND3, m_sPos);
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(m_roi3.GetPos());
			dc.LineTo(m_roi3.GetdPos().x, m_roi3.GetPos().y);
			dc.LineTo(m_roi3.GetdPos());
			dc.LineTo(m_roi3.GetPos().x, m_roi3.GetdPos().y);
			dc.LineTo(m_roi3.GetPos());
			dc.SelectObject(oldPen);
		}

		// Ȧ�߽� 
		if (hole0.x != 0 && hole0.y != 0) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND0));
			//SetROIPos(IDC_IMAGE_WND3, m_sPos);
			CPen pen;
			pen.CreatePen(1, 2, RGB(0, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			int x, y;
			x = hole0.x / 4;
			y = hole0.y / 4;
			dc.MoveTo(x, y);
			dc.LineTo(x, y + 10);
			dc.MoveTo(x, y);
			dc.LineTo(x, y - 10);
			dc.MoveTo(x, y);
			dc.LineTo(x - 10, y);
			dc.MoveTo(x, y);
			dc.LineTo(x + 10, y);


			dc.SelectObject(oldPen);
		}if (hole1.x != 0 && hole1.y != 0) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND1));
			//SetROIPos(IDC_IMAGE_WND3, m_sPos);
			CPen pen;
			pen.CreatePen(1, 2, RGB(0, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			int x, y;
			x = hole1.x / 4;
			y = hole1.y / 4;
			dc.MoveTo(x, y);
			dc.LineTo(x, y + 10);
			dc.MoveTo(x, y);
			dc.LineTo(x, y - 10);
			dc.MoveTo(x, y);
			dc.LineTo(x - 10, y);
			dc.MoveTo(x, y);
			dc.LineTo(x + 10, y);


			dc.SelectObject(oldPen);
		}if (hole2.x != 0 && hole2.y != 0) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND2));
			//SetROIPos(IDC_IMAGE_WND3, m_sPos);
			CPen pen;
			pen.CreatePen(1, 2, RGB(0, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			int x, y;
			x = hole2.x / 4;
			y = hole2.y / 4;
			dc.MoveTo(x, y);
			dc.LineTo(x, y + 10);
			dc.MoveTo(x, y);
			dc.LineTo(x, y - 10);
			dc.MoveTo(x, y);
			dc.LineTo(x - 10, y);
			dc.MoveTo(x, y);
			dc.LineTo(x + 10, y);


			dc.SelectObject(oldPen);
		}




		if (hole3.x != 0 && hole3.y != 0) {
			CClientDC dc(GetDlgItem(IDC_IMAGE_WND3));
			//SetROIPos(IDC_IMAGE_WND3, m_sPos);
			CPen pen;
			pen.CreatePen(1, 2, RGB(0, 0, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			int x, y;
			x = hole3.x / 4;
			y = hole3.y / 4;
			dc.MoveTo(x, y);
			dc.LineTo(x, y + 10);
			dc.MoveTo(x, y);
			dc.LineTo(x, y - 10);
			dc.MoveTo(x, y);
			dc.LineTo(x - 10, y);
			dc.MoveTo(x, y);
			dc.LineTo(x + 10, y);


			dc.SelectObject(oldPen);
		}



		// �̹��� ���� �׸���
		if (m_roi0.IsCenter()) {
			int x, y;
			CRect rt;
			// ���� ��Ʈ���� �簢�� ��ǥ�� ����
			// GetClientRect �� ũ�⸦ ���ϴ� GetWindowRect �̿�...

			((CStatic*)GetDlgItem(IDC_IMAGE_WND0))->GetWindowRect(&rt);

			// GetWindowRect �� ���� ��ǥ�� ��ũ�� ��ǥ(������) �̴� �̸�

			// ȭ�� ��ǥ(����, Ŭ���̾�Ʈ ��ǥ)�� ��ȯ
			ScreenToClient(&rt);

			x = (rt.right - rt.left) / 2;
			y = (rt.bottom - rt.top) / 2;

			CClientDC dc(GetDlgItem(IDC_IMAGE_WND0));
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(0, 255, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(x, 0);
			dc.LineTo(x, 2 * y);
			dc.MoveTo(0, y);
			dc.LineTo(2 * x, y);
			dc.SelectObject(oldPen);
		}

		if (m_roi1.IsCenter()) {
			int x, y;
			CRect rt;
			// ���� ��Ʈ���� �簢�� ��ǥ�� ����
			// GetClientRect �� ũ�⸦ ���ϴ� GetWindowRect �̿�...

			((CStatic*)GetDlgItem(IDC_IMAGE_WND1))->GetWindowRect(&rt);

			// GetWindowRect �� ���� ��ǥ�� ��ũ�� ��ǥ(������) �̴� �̸�

			// ȭ�� ��ǥ(����, Ŭ���̾�Ʈ ��ǥ)�� ��ȯ
			ScreenToClient(&rt);

			x = (rt.right - rt.left) / 2;
			y = (rt.bottom - rt.top) / 2;

			CClientDC dc(GetDlgItem(IDC_IMAGE_WND1));
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(0, 255, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(x, 0);
			dc.LineTo(x, 2 * y);
			dc.MoveTo(0, y);
			dc.LineTo(2 * x, y);
			dc.SelectObject(oldPen);
		}
		if (m_roi2.IsCenter()) {
			int x, y;
			CRect rt;
			// ���� ��Ʈ���� �簢�� ��ǥ�� ����
			// GetClientRect �� ũ�⸦ ���ϴ� GetWindowRect �̿�...

			((CStatic*)GetDlgItem(IDC_IMAGE_WND2))->GetWindowRect(&rt);

			// GetWindowRect �� ���� ��ǥ�� ��ũ�� ��ǥ(������) �̴� �̸�

			// ȭ�� ��ǥ(����, Ŭ���̾�Ʈ ��ǥ)�� ��ȯ
			ScreenToClient(&rt);

			x = (rt.right - rt.left) / 2;
			y = (rt.bottom - rt.top) / 2;

			CClientDC dc(GetDlgItem(IDC_IMAGE_WND2));
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(0, 255, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(x, 0);
			dc.LineTo(x, 2 * y);
			dc.MoveTo(0, y);
			dc.LineTo(2 * x, y);
			dc.SelectObject(oldPen);
		}
		if (m_roi3.IsCenter()) {
			int x, y;
			CRect rt;
			// ���� ��Ʈ���� �簢�� ��ǥ�� ����
			// GetClientRect �� ũ�⸦ ���ϴ� GetWindowRect �̿�...

			((CStatic*)GetDlgItem(IDC_IMAGE_WND3))->GetWindowRect(&rt);

			// GetWindowRect �� ���� ��ǥ�� ��ũ�� ��ǥ(������) �̴� �̸�

			// ȭ�� ��ǥ(����, Ŭ���̾�Ʈ ��ǥ)�� ��ȯ
			ScreenToClient(&rt);

			x = (rt.right - rt.left) / 2;
			y = (rt.bottom - rt.top) / 2;

			CClientDC dc(GetDlgItem(IDC_IMAGE_WND3));
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(0, 255, 0));    // ������ �� ����
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(x, 0);
			dc.LineTo(x, 2 * y);
			dc.MoveTo(0, y);
			dc.LineTo(2 * x, y);
			dc.SelectObject(oldPen);
		}

	}



}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ���Ϸε� �����丵
void CToolDlg::LoadFile(MyImage & image)
{
	static TCHAR BASED_CODE szFilter[] = _T("BMP ����(*.BMP) ||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		image.Load(dlg.GetPathName());
		Invalidate(FALSE);

	}

}

void CToolDlg::OnBnClickedButtonLoad0()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadFile(m_image0);
	m_oImage0 = m_image0;
	if (m_nTh0 >= 0 && m_nTh0 <= 256) {
		m_roi0.ROIDibBinarization(m_image0, m_nTh0);
	}
}


void CToolDlg::OnBnClickedButtonLoad1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadFile(m_image1);
	m_oImage1 = m_image1;
	if (m_nTh1 >= 0 && m_nTh1 <= 256) {
		m_roi1.ROIDibBinarization(m_image1, m_nTh1);
	}
}


void CToolDlg::OnBnClickedButtonLoad2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadFile(m_image2);
	m_oImage2 = m_image2;
	if (m_nTh2 >= 0 && m_nTh2 <= 256) {
		m_roi2.ROIDibBinarization(m_image2, m_nTh2);
	}
}


void CToolDlg::OnBnClickedButtonLoad3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadFile(m_image3);
	m_oImage3 = m_image3;
	if (m_nTh3 >= 0 && m_nTh3 <= 256) {
		m_roi3.ROIDibBinarization(m_image3, m_nTh3);
	}
}

void CToolDlg::Binarization(MyImage & image, int i)
{
	// ������ �ε� �� ���¶�� �����ϰ� �ƴ϶�� �޽����� ����.
	if (image.IsValid() == TRUE) {
		CBinarizationDlg dlg;
		switch (i)
		{
		case 0:
			image = m_oImage0;
			break;
		case 1:
			image = m_oImage1;
			break;
		case 2:
			image = m_oImage2;
			break;
		case 3:
			image = m_oImage3;
			break;
		default:
			
			break;
		}
		dlg.SetImage(image);

		int n;
		if (dlg.DoModal() == IDOK) {

			// image0 ���� ������ ����ȭ�� �����Ѵ�. 
			DibBinarization(image, dlg.m_nThreshold);
			// ����ȭ�� ���� �Ӱ谪 int���� CString ������ ����ȯ �� ����Ѵ�.
			switch (i)
			{
			case 0:
				m_nTh0 = dlg.m_nThreshold;
				n = IDC_STATIC0;
				break;
			case 1:
				m_nTh1 = dlg.m_nThreshold;
				n = IDC_STATIC1;
				break;
			case 2:
				m_nTh2 = dlg.m_nThreshold;
				n = IDC_STATIC1;
				break;
			case 3:
				m_nTh3 = dlg.m_nThreshold;
				n = IDC_STATIC3;
				break;
			default:
				n = IDC_STATIC0;
				break;
			}

			CStatic * pStaticTest = (CStatic *)GetDlgItem(n);
			CString str;
			str.Format(_T("%d"), dlg.m_nThreshold);
		
				
			pStaticTest->SetWindowText(str);


			Invalidate(FALSE);
		}
	}
	else {
		MessageBox(_T("�̹����� �ε� ���ּ���."));
	}
}

void CToolDlg::ROIBinarization(MyImage & image, ROI& roi, int i)
{
	// ������ �ε� �� ���¶�� �����ϰ� �ƴ϶�� �޽����� ����.
	if (image.IsValid() == TRUE) {
		CBinarizationDlg dlg;
		switch (i)
		{
		case 0:
			image = m_oImage0;
			break;
		case 1:
			image = m_oImage1;
			break;
		case 2:
			image = m_oImage2;
			break;
		case 3:
			image = m_oImage3;
			break;
		default:

			break;
		}

		dlg.SetImage(image);
		int n;
		if (dlg.DoModal() == IDOK) {

			// image0 ���� ������ ����ȭ�� �����Ѵ�. 
			roi.ROIDibBinarization(image, dlg.m_nThreshold);
			switch (i)
			{
			case 0:
				m_nTh0 = dlg.m_nThreshold;
				n = IDC_STATIC0;
				break;
			case 1:
				m_nTh1 = dlg.m_nThreshold;
				n = IDC_STATIC1;
				break;
			case 2:
				m_nTh2 = dlg.m_nThreshold;
				n = IDC_STATIC1;
				break;
			case 3:
				m_nTh3 = dlg.m_nThreshold;
				n = IDC_STATIC3;
				break;
				n = IDC_STATIC0;
			default:

				break;
			}
			// ����ȭ�� ���� �Ӱ谪 int���� CString ������ ����ȯ �� ����Ѵ�.
			CStatic * pStaticTest = (CStatic *)GetDlgItem(n);
			CString str;
			str.Format(_T("%d"), dlg.m_nThreshold);
			
			

			pStaticTest->SetWindowText(str);
			Invalidate(FALSE);

			
		}
	}
	else {
		MessageBox(_T("�̹����� �ε� ���ּ���."));
	}
}

// ����ȭ ��ư ����
void CToolDlg::OnBnClickedButtonBin0()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_roi0.m_drawROI == FALSE) {
		Binarization(m_image0, 0);
	}
	else if (m_roi0.m_drawROI == TRUE) {
		ROIBinarization(m_image0, m_roi0, 0);
	}

}


void CToolDlg::OnBnClickedButtonRoi0()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_ROIFlags = Image0;
	m_ROIDrag = TRUE;
	m_roi0.SetROI(TRUE);
	m_roi0.m_drawROI = FALSE;
}


void CToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rt;
	// ���� ��Ʈ���� �簢�� ��ǥ�� ����
	// GetClientRect �� ũ�⸦ ���ϴ� GetWindowRect �̿�...
	switch (m_ROIFlags)
	{
	case Image0:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND0))->GetWindowRect(&rt);
		break;
	case Image1:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND1))->GetWindowRect(&rt);
		break;
	case Image2:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND2))->GetWindowRect(&rt);
		break;
	case Image3:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND3))->GetWindowRect(&rt);
		break;
	default:
		break;
	}

	// GetWindowRect �� ���� ��ǥ�� ��ũ�� ��ǥ(������) �̴� �̸�

	// ȭ�� ��ǥ(����, Ŭ���̾�Ʈ ��ǥ)�� ��ȯ
	ScreenToClient(&rt);

	if (m_ROIDrag == TRUE) {


		m_sPos.x = point.x - rt.left;
		m_sPos.y = point.y - rt.top;
		m_dragFlag = TRUE;
		switch (m_ROIFlags)
		{
		case Image0:
			SetROIPos(IDC_IMAGE_WND0, m_sPos);
			break;
		case Image1:
			SetROIPos(IDC_IMAGE_WND1, m_sPos);
			break;
		case Image2:
			SetROIPos(IDC_IMAGE_WND2, m_sPos);
			break;
		case Image3:
			SetROIPos(IDC_IMAGE_WND3, m_sPos);
			break;
		default:
			break;
		}
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}


void CToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_ROIDrag == TRUE) {
		CRect rt;
		// ���� ��Ʈ���� �簢�� ��ǥ�� ����
		// GetClientRect �� ũ�⸦ ���ϴ� GetWindowRect �̿�...
		switch (m_ROIFlags)
		{
		case Image0:
			((CStatic*)GetDlgItem(IDC_IMAGE_WND0))->GetWindowRect(&rt);
			break;
		case Image1:
			((CStatic*)GetDlgItem(IDC_IMAGE_WND1))->GetWindowRect(&rt);
			break;
		case Image2:
			((CStatic*)GetDlgItem(IDC_IMAGE_WND2))->GetWindowRect(&rt);
			break;
		case Image3:
			((CStatic*)GetDlgItem(IDC_IMAGE_WND3))->GetWindowRect(&rt);
			break;
		default:
			break;
		}

		// GetWindowRect �� ���� ��ǥ�� ��ũ�� ��ǥ(������) �̴� �̸�

		// ȭ�� ��ǥ(����, Ŭ���̾�Ʈ ��ǥ)�� ��ȯ
		ScreenToClient(&rt);

		if (rt.PtInRect(point)) // ���� ��Ʈ���� �簢�� ������ ���콺 Ŭ�� ��ǥ(point) �� ������...TRUE
		{
			m_ePos = point;




			m_ePos.x = m_ePos.x - rt.left;
			m_ePos.y = m_ePos.y - rt.top;



			// ****************************************************
			switch (m_ROIFlags)
			{
			case Image0:
				SetROIdPos(IDC_IMAGE_WND0, m_ePos);
				m_roi0.m_drawROI = TRUE;
				break;
			case Image1:
				SetROIdPos(IDC_IMAGE_WND1, m_ePos);
				m_roi1.m_drawROI = TRUE;
				break;
			case Image2:
				SetROIdPos(IDC_IMAGE_WND2, m_ePos);
				m_roi2.m_drawROI = TRUE;
				break;
			case Image3:
				SetROIdPos(IDC_IMAGE_WND3, m_ePos);
				m_roi3.m_drawROI = TRUE;
				break;
			default:
				break;
			}

			//Invalidate(FALSE);
			m_ROIDrag = FALSE;
			m_ROIFlags = FALSE;
		}
		else {
			MessageBox(_T("ROI���� ����, �ٽ� ���� ���ּ���. "));
		}
		m_dragFlag = FALSE;
		m_ROIFlags = FALSE;

	}
}


void CToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CClientDC dc(this);

	CRect rt;
	// ���� ��Ʈ���� �簢�� ��ǥ�� ����
	// GetClientRect �� ũ�⸦ ���ϴ� GetWindowRect �̿�...
	switch (m_ROIFlags)
	{
	case Image0:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND0))->GetWindowRect(&rt);
		break;
	case Image1:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND1))->GetWindowRect(&rt);
		break;
	case Image2:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND2))->GetWindowRect(&rt);
		break;
	case Image3:
		((CStatic*)GetDlgItem(IDC_IMAGE_WND3))->GetWindowRect(&rt);
		break;
	default:
		break;
	}


	// GetWindowRect �� ���� ��ǥ�� ��ũ�� ��ǥ(������) �̴� �̸�

	// ȭ�� ��ǥ(����, Ŭ���̾�Ʈ ��ǥ)�� ��ȯ
	ScreenToClient(&rt);

	if (rt.PtInRect(point)) // ���� ��Ʈ���� �簢�� ������ ���콺 Ŭ�� ��ǥ(point) �� ������...TRUE
	{
		CString strPoint;
		strPoint.Format(L"%04d, %04d", point.x - rt.left, point.y - rt.top);

		dc.TextOut(0, 0, strPoint);

		if (m_dragFlag == TRUE) {
			m_pos.x = point.x - rt.left;
			m_pos.y = point.y - rt.top;
			Invalidate(FALSE);
		}

	}



	CDialogEx::OnMouseMove(nFlags, point);
}


// ROI ���� �׸���
void CToolDlg::SetROIPos(int nID, CPoint point)
{
	switch (nID)
	{
	case IDC_IMAGE_WND0:
		m_roi0.SetPos(point);
		break;
	case IDC_IMAGE_WND1:
		m_roi1.SetPos(point);
		break;
	case IDC_IMAGE_WND2:
		m_roi2.SetPos(point);
		break;
	case IDC_IMAGE_WND3:
		m_roi3.SetPos(point);
		break;
	default:
		break;
	}
}

void CToolDlg::SetROIdPos(int nID, CPoint point)
{
	switch (nID)
	{
	case IDC_IMAGE_WND0:
		m_roi0.SetdPos(point);
		break;
	case IDC_IMAGE_WND1:
		m_roi1.SetdPos(point);
		break;
	case IDC_IMAGE_WND2:
		m_roi2.SetdPos(point);
		break;
	case IDC_IMAGE_WND3:
		m_roi3.SetdPos(point);
		break;
	default:
		break;
	}
}


void CToolDlg::OnBnClickedButtonHole0()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


}


void CToolDlg::OnBnClickedButtonRoi1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_ROIFlags = Image1;
	m_ROIDrag = TRUE;
	m_roi1.SetROI(TRUE);
	m_roi1.m_drawROI = FALSE;
}


void CToolDlg::OnBnClickedButtonRoi2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_ROIFlags = Image2;
	m_ROIDrag = TRUE;
	m_roi2.SetROI(TRUE);
	m_roi2.m_drawROI = FALSE;
}


void CToolDlg::OnBnClickedButtonRoi3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_ROIFlags = Image3;
	m_ROIDrag = TRUE;
	m_roi3.SetROI(TRUE);
	m_roi3.m_drawROI = FALSE;
}


void CToolDlg::OnBnClickedButtonBin1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_roi1.m_drawROI == FALSE) {
		Binarization(m_image1, 1);
	}
	else if (m_roi1.m_drawROI == TRUE) {
		ROIBinarization(m_image1, m_roi1, 1);
	}
}


void CToolDlg::OnBnClickedButtonBin2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_roi2.m_drawROI == FALSE) {
		Binarization(m_image2, 2);
	}
	else if (m_roi2.m_drawROI == TRUE) {
		ROIBinarization(m_image2, m_roi2, 2);
	}
}


void CToolDlg::OnBnClickedButtonBin3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_roi3.m_drawROI == FALSE) {
		Binarization(m_image3, 3);
	}
	else if (m_roi3.m_drawROI == TRUE) {
		ROIBinarization(m_image3, m_roi3, 3);
	}
}

void CToolDlg::OnBnClickedButtonHole1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}

void CToolDlg::OnBnClickedButtonHole2()
{

}


void CToolDlg::OnBnClickedButtonHole3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}

FLOAT CToolDlg::Eval(CPoint pos1, CPoint pos2)
{

	float rad;
	float deg;

	int x = (pos2.x * 10) - (pos1.x * 10);
	int y = (pos2.y * 10) - (pos1.y * 10);

	rad = atan2(140000 + y, x);
	deg = (rad * 180 / PI);
	return deg;
}


void CToolDlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int x0, y0, x1, y1, x2, y2, x3, y3;
	if (m_image0.IsValid()) {
		m_roi0.SetCenter(TRUE);
		//
		DibMorphologyErosion(m_image0);
		DibMorphologyDilation(m_image0);
		hole0 = m_roi0.FindHole(m_image0, m_roi0.CoordinateContingency(m_roi0.GetPos()), m_roi0.CoordinateContingency(m_roi0.GetdPos()));

		if (hole0.x != 0 && hole0.y != 0) {
			// Ȧã�� �׽�Ʈ
			//BYTE** ptr = m_image0.GetPtr();
			//ptr[hole.y][hole.x] = 0;
			//int n = 20;
			//for (int i = hole.y - n; i < hole.y + n; i++) {
			//	ptr[i - 1][hole.x] = 0;
			//	ptr[i][hole.x] = 0;
			//	ptr[i + 1][hole.x] = 0;
			//}

			//for (int i = hole.x - n; i < hole.x + n; i++) {
			//	ptr[hole.y][i - 1] = 0;
			//	ptr[hole.y][i] = 0;
			//	ptr[hole.y][i + 1] = 0;
			//}

			x0 = hole0.x - (m_image0.GetWidth() / 2);

			CStatic * pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC0_X);
			CString str;
			str.Format(_T("%d"), x0);
			pStaticTest->SetWindowText(str);


			y0 = hole0.y - (m_image0.GetHeight() / 2);
			pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC0_Y);
			str;
			str.Format(_T("%d"), y0);
			pStaticTest->SetWindowText(str);

			Invalidate();
			m_val0.x = x0;
			m_val0.y = y0;
			//m_roi0.ROIDibBinarization(m_image0, 125);



			//dc.MoveTo(0, y);
			//dc.LineTo(2*x, y);

		}
	}if (m_image1.IsValid()) {
		m_roi1.SetCenter(TRUE);
		//
		DibMorphologyErosion(m_image1);
		DibMorphologyDilation(m_image1);
		hole1 = m_roi1.FindHole(m_image1, m_roi1.CoordinateContingency(m_roi1.GetPos()), m_roi1.CoordinateContingency(m_roi1.GetdPos()));

		if (hole1.x != 0 && hole1.y != 0) {
			// Ȧã�� �׽�Ʈ
			//BYTE** ptr = m_image1.GetPtr();
			//ptr[hole.y][hole.x] = 0;
			//int n = 20;
			//for (int i = hole.y - n; i < hole.y + n; i++) {
			//	ptr[i - 1][hole.x] = 0;
			//	ptr[i][hole.x] = 0;
			//	ptr[i + 1][hole.x] = 0;
			//}

			//for (int i = hole.x - n; i < hole.x + n; i++) {
			//	ptr[hole.y][i - 1] = 0;
			//	ptr[hole.y][i] = 0;
			//	ptr[hole.y][i + 1] = 0;
			//}

			x1 = hole1.x - (m_image1.GetWidth() / 2);

			CStatic * pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC1_X);
			CString str;
			str.Format(_T("%d"), x1);
			pStaticTest->SetWindowText(str);


			y1 = hole1.y - (m_image1.GetHeight() / 2);
			pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC1_Y);
			str;
			str.Format(_T("%d"), y1);
			pStaticTest->SetWindowText(str);

			Invalidate();
			m_val1.x = x1;
			m_val1.y = y1;
		}
	}if (m_image2.IsValid()) {
		m_roi2.SetCenter(TRUE);
		//
		DibMorphologyErosion(m_image2);
		DibMorphologyDilation(m_image2);
		hole2 = m_roi2.FindHole(m_image2, m_roi2.CoordinateContingency(m_roi2.GetPos()), m_roi2.CoordinateContingency(m_roi2.GetdPos()));

		if (hole2.x != 0 && hole2.y != 0) {
			// Ȧã�� �׽�Ʈ
			/*	BYTE** ptr = m_image2.GetPtr();
			ptr[hole.y][hole.x] = 0;
			int n = 20;
			for (int i = hole.y - n; i < hole.y + n; i++) {
			ptr[i - 1][hole.x] = 0;
			ptr[i][hole.x] = 0;
			ptr[i + 1][hole.x] = 0;
			}

			for (int i = hole.x - n; i < hole.x + n; i++) {
			ptr[hole.y][i - 1] = 0;
			ptr[hole.y][i] = 0;
			ptr[hole.y][i + 1] = 0;
			}*/

			x2 = hole2.x - (m_image2.GetWidth() / 2);

			CStatic * pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC2_X);
			CString str;
			str.Format(_T("%d"), x2);
			pStaticTest->SetWindowText(str);


			y2 = hole2.y - (m_image2.GetHeight() / 2);
			pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC2_Y);
			str;
			str.Format(_T("%d"), y2);
			pStaticTest->SetWindowText(str);

			Invalidate();
			m_val2.x = x2;
			m_val2.y = y2;
		}
	}if (m_image3.IsValid()) {
		m_roi3.SetCenter(TRUE);
		//
		DibMorphologyErosion(m_image3);
		DibMorphologyDilation(m_image3);
		hole3 = m_roi3.FindHole(m_image3, m_roi3.CoordinateContingency(m_roi3.GetPos()), m_roi3.CoordinateContingency(m_roi3.GetdPos()));

		if (hole3.x != 0 && hole3.y != 0) {
			// Ȧã�� �׽�Ʈ
			/*BYTE** ptr = m_image3.GetPtr();
			ptr[hole.y][hole.x] = 0;
			int n = 20;
			for (int i = hole.y - n; i < hole.y + n; i++) {
			ptr[i - 1][hole.x] = 0;
			ptr[i][hole.x] = 0;
			ptr[i + 1][hole.x] = 0;
			}

			for (int i = hole.x - n; i < hole.x + n; i++) {
			ptr[hole.y][i - 1] = 0;
			ptr[hole.y][i] = 0;
			ptr[hole.y][i + 1] = 0;
			}*/

			x3 = hole3.x - (m_image3.GetWidth() / 2);

			CStatic * pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC3_X);
			CString str;
			str.Format(_T("%d"), x3);
			pStaticTest->SetWindowText(str);


			y3 = hole3.y - (m_image3.GetHeight() / 2);
			pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC3_Y);
			str;
			str.Format(_T("%d"), y3);
			pStaticTest->SetWindowText(str);


			Invalidate();

			m_val3.x = x3;
			m_val3.y = y3;
		}
	}
	if (m_image0.IsValid() && m_image1.IsValid() && m_image2.IsValid() && m_image3.IsValid()) {
		CPoint h0, h1, h2, h3;
		h0.x = x0;
		h0.y = y0;
		h1.x = x1;
		h1.y = y1;
		h2.x = x2;
		h2.y = y2;
		h3.x = x3;
		h3.y = y3;


		float dgt = 90 - Eval(h0, h1);
		float dgx = EvalX(h0, h1);
		float dgy = EvalY(h0, h1);

		float dft = 90 - Eval(h2, h3);
		float dfx = EvalX(h2, h3);
		float dfy = EvalY(h2, h3);

		float dt = dft - dgt;
		float dx = dfx - dgx;
		float dy = dgy - dfy;

		CStatic * pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_TX);
		CString str;
		str.Format(_T("%d"), (int)dx);
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_TY);
		str;
		str.Format(_T("%d"), (int)dy);
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_TD);
		str;
		str.Format(_T("%.3f"), dt);
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_DGT);
		str;
		str.Format(_T("%.3f"), dgt);
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_DGX);
		str;
		str.Format(_T("%d"), (int)(dgx+0.5));
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_DGY);
		str;
		str.Format(_T("%d"), (int)(dgy+0.5));
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_DFT);
		str;
		str.Format(_T("%.3f"), dft);
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_DFX);
		str;
		str.Format(_T("%d"), (int)(dfx+0.5));
		pStaticTest->SetWindowText(str);

		pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC_DFY);
		str;
		str.Format(_T("%d"), (int)(dfy+0.5));
		pStaticTest->SetWindowText(str);


	}
}

FLOAT CToolDlg::EvalX(CPoint pos1, CPoint pos2)
{
	float rad;
	float deg;

	int x = (pos2.x * 10) - (pos1.x * 10);
	int y = (pos2.y * 10) - (pos1.y * 10);

	rad = atan2(140000 + y, x);
	float dx = cos(rad)*sqrt(pow(x, 2) + pow(y + 140000, 2)) / 2;
	float dgx = 0 - ((pos1.x * 10) + dx);
	return dgx;
}

FLOAT CToolDlg::EvalY(CPoint pos1, CPoint pos2)
{
	float rad;
	float deg;

	int x = (pos2.x * 10) - (pos1.x * 10);
	int y = (pos2.y * 10) - (pos1.y * 10);

	rad = atan2(140000 + y, x);
	float dy = sin(rad)*sqrt(pow(x, 2) + pow(y + 140000, 2)) / 2;
	float dgy = dy - 70000 - (pos2.y * 10);
	return dgy;
}

void CToolDlg::SettingLoad()
{
	char line[100];
	ifstream in("C:\\Users\\NOVA\\Desktop\\test.txt");
	int x, y, n;
	string str;
	if (in.is_open()) {

		in.getline(line,sizeof(line));
		x = atoi(line);

		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi0.SetPos(CPoint(x, y));
	
		in.getline(line, sizeof(line));
		x = atoi(line);
		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi0.SetdPos(CPoint(x, y));

		in.getline(line, sizeof(line));
		m_nTh0 = atoi(line);

		in.getline(line, sizeof(line));
		n = atoi(line);
		m_roi0.m_drawROI = n;

	//
		in.getline(line, sizeof(line));
		x = atoi(line);

		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi1.SetPos(CPoint(x, y));

		in.getline(line, sizeof(line));
		x = atoi(line);
		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi1.SetdPos(CPoint(x, y));

		in.getline(line, sizeof(line));
		m_nTh1 = atoi(line);

		in.getline(line, sizeof(line));
		n = atoi(line);
		m_roi1.m_drawROI = n;

		//
		in.getline(line, sizeof(line));
		x = atoi(line);

		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi2.SetPos(CPoint(x, y));

		in.getline(line, sizeof(line));
		x = atoi(line);
		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi2.SetdPos(CPoint(x, y));

		in.getline(line, sizeof(line));
		m_nTh2 = atoi(line);


		in.getline(line, sizeof(line));
		n = atoi(line);
		m_roi2.m_drawROI = n;
		

		//

		in.getline(line, sizeof(line));
		x = atoi(line);

		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi3.SetPos(CPoint(x, y));

		in.getline(line, sizeof(line));
		x = atoi(line);
		in.getline(line, sizeof(line));
		y = atoi(line);

		m_roi3.SetdPos(CPoint(x, y));

		in.getline(line, sizeof(line));
		m_nTh3 = atoi(line);


		in.getline(line, sizeof(line));
		n = atoi(line);
		m_roi3.m_drawROI = n;

		in.close();
	}
}

void CToolDlg::SettingSave()
{
	CString filePath(_T("C:\\Users\\NOVA\\Desktop\\test.txt"));
	ofstream writeFile(filePath);
	if (writeFile.is_open()) {
		writeFile << m_roi0.GetPos().x << endl;
		writeFile << m_roi0.GetPos().y << endl;
		writeFile << m_roi0.GetdPos().x << endl;
		writeFile << m_roi0.GetdPos().y << endl;
		writeFile << m_nTh0 << endl;
		writeFile << "1" << endl;
		writeFile << m_roi1.GetPos().x << endl;
		writeFile << m_roi1.GetPos().y << endl;
		writeFile << m_roi1.GetdPos().x << endl;
		writeFile << m_roi1.GetdPos().y << endl;
		writeFile << m_nTh1 << endl;
		writeFile << "1" << endl;
		writeFile << m_roi2.GetPos().x << endl;
		writeFile << m_roi2.GetPos().y << endl;
		writeFile << m_roi2.GetdPos().x << endl;
		writeFile << m_roi2.GetdPos().y << endl;
		writeFile << m_nTh2 << endl;
		writeFile << "1" << endl;
		writeFile << m_roi3.GetPos().x << endl;
		writeFile << m_roi3.GetPos().y << endl;
		writeFile << m_roi3.GetdPos().x << endl;
		writeFile << m_roi3.GetdPos().y << endl;
		writeFile << m_nTh3 << endl;
		writeFile << "1" << endl;
	}

}
