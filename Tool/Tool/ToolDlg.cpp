
// ToolDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolDlg.h"
#include "afxdialogex.h"
#include "BinarizationDlg.h"
#include "ImageEdit.h"

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
	: CDialogEx(IDD_TOOL_DIALOG, pParent)
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


		if (roi0.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND0));
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ROI�� ������ �뽬�� ��Ÿ�� 
			CPen* oldPen = dc2.SelectObject(&pen);
			dc2.MoveTo(roi0.GetPos());
			dc2.LineTo(roi0.GetdPos());
			dc2.SelectObject(oldPen);
		}
		//if (m_image0.IsValid()) {
		//	CDC* p = m_wnd0.GetWindowDC();
		//	p->MoveTo(100, 200);
		//	p->LineTo(200, 200);
		//	CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND0));
		//	//CStatic *staticSize = (CStatic *)GetDlgItem(IDC_IMAGE_WND0);
		//	CRect rect;
		//	CWnd* pImageWnd = GetDlgItem(IDC_IMAGE_WND0);

		//	
		//	pImageWnd->GetClientRect(rect);
		//	//staticSize->GetClientRect(rect);
		//	m_image0.Draw(p->m_hDC, 0, 0, rect.Width(), rect.Height());
		//	m_wnd0.ReleaseDC(p);
		//}
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
}


void CToolDlg::OnBnClickedButtonLoad1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadFile(m_image1);
}


void CToolDlg::OnBnClickedButtonLoad2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadFile(m_image2);
}


void CToolDlg::OnBnClickedButtonLoad3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	static TCHAR BASED_CODE szFilter[] = _T("BMP ����(*.BMP) ||");
	LoadFile(m_image3);
}

void CToolDlg::Binarization(MyImage & image)
{
	// ������ �ε� �� ���¶�� �����ϰ� �ƴ϶�� �޽����� ����.
	if (m_image0.IsValid() == TRUE) {
		CBinarizationDlg dlg;
		dlg.SetImage(m_image0);
		if (dlg.DoModal() == IDOK) {

			// image0 ���� ������ ����ȭ�� �����Ѵ�. 
			DibBinarization(image, dlg.m_nThreshold);
			// ����ȭ�� ���� �Ӱ谪 int���� CString ������ ����ȯ �� ����Ѵ�.
			CStatic * pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC0);
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
	Binarization(m_image0);
}


void CToolDlg::OnBnClickedButtonRoi0()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!roi0.IsValid()) {
		CPoint temp(1,1);
		roi0.SetdPos(temp);
	}

}


void CToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (!roi0.IsValid()) {
		roi0.SetPos(point);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (roi0.IsValid()) {
		roi0.SetdPos(point);
		Invalidate(FALSE);
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}
