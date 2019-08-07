#pragma once
#include "afxcmn.h"
#include "MyImage.h"


// CBinarizationDlg ��ȭ �����Դϴ�.

class CBinarizationDlg : public CDialog
{
	DECLARE_DYNAMIC(CBinarizationDlg)

public:
	CBinarizationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBinarizationDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// �����̴� ���� ����
	CSliderCtrl m_sliderThreshold;
	// �ؽ�Ʈ ���� ����
	int m_nThreshold;
private:
	MyImage m_ImgSrc; // �Է� ������ ��� ���纻
	MyImage m_ImgRes; // ���� �Ӱ谪�� �̿��Ͽ� m_DibSrc�� ����ȭ�� ����
	CRect rect; // �̹��� ������ ũ�� ����


public:
	// �̹������� ����
	void SetImage(MyImage& image);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeThresholdEdit();
};
