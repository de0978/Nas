// BinarizationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "BinarizationDlg.h"
#include "afxdialogex.h"
#include "ImageEdit.h";
#include "ROI.h"


// CBinarizationDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBinarizationDlg, CDialog)

CBinarizationDlg::CBinarizationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BINARIZATION, pParent)
	, m_nThreshold(128)
{

}

CBinarizationDlg::~CBinarizationDlg()
{
}

void CBinarizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_sliderThreshold);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_nThreshold);
}


BEGIN_MESSAGE_MAP(CBinarizationDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_THRESHOLD_EDIT, &CBinarizationDlg::OnEnChangeThresholdEdit)
END_MESSAGE_MAP()


// CBinarizationDlg �޽��� ó�����Դϴ�.


// �̹������� Set
void CBinarizationDlg::SetImage(MyImage& image)
{
	m_ImgSrc = image;
}


BOOL CBinarizationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//  �����̴� ��Ʈ���� �ʱ��� �Ѵ�.
	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetTicFreq(32);
	m_sliderThreshold.SetPageSize(32);
	m_sliderThreshold.SetPos(m_nThreshold);
	
	// ���� �ڽ��� ���� ������ ���̸� ���Ѵ�.
	CWnd* pImageWnd = GetDlgItem(IDC_IMAGE_WND);
	pImageWnd->GetClientRect(rect);

	// �̹����� ���纻�� �����.
	m_ImgRes = m_ImgSrc;

	// ���纻�� ����ȭ �Ѵ�.
	DibBinarization(m_ImgRes, m_nThreshold);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CBinarizationDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.


   // ���纻�� ����Ѵ�.
	CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND));
	m_ImgRes.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
}


void CBinarizationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// �����̵�ٿ��� �߻��� WM_HSCROLL �޽����� ��� ó�� �Ѵ�.
	if (m_sliderThreshold.GetSafeHwnd() == pScrollBar->GetSafeHwnd()) {
		int nPos = m_sliderThreshold.GetPos();
		m_nThreshold = nPos;
		UpdateData(FALSE);
	}

	// ���� ������ �Ӱ谪�� �̿��Ͽ� ���纻 ������ ����ȭ�� �����Ѵ�.
	m_ImgRes = m_ImgSrc;
	DibBinarization(m_ImgRes, m_nThreshold);
	Invalidate(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBinarizationDlg::OnEnChangeThresholdEdit()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ����Ʈ ��Ʈ�ѿ��� ���ڰ� �ٲ� ���, �����̴� ��Ʈ���� ��ġ�� �����Ѵ�.
	UpdateData(TRUE);
	m_sliderThreshold.SetPos(m_nThreshold);

	// ���� ������ �Ӱ谪�� �̿��Ͽ� ���纻 ������ ����ȭ�� �����Ѵ�.
	m_ImgRes = m_ImgSrc;
	DibBinarization(m_ImgRes, m_nThreshold);
	Invalidate(FALSE);
}
