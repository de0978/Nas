
// ToolDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "MyImage.h"
#include "ROI.h"

// CToolDlg ��ȭ ����
class CToolDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CToolDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ��Ʈ ����
	CFont m_font;
	
	// ROI ���콺 �̺�Ʈ ó�� �Լ�
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// �������� �ε� 
	void LoadFile(MyImage& image);


	afx_msg void OnBnClickedButtonLoad0();
	afx_msg void OnBnClickedButtonLoad1();
	afx_msg void OnBnClickedButtonLoad2();
	afx_msg void OnBnClickedButtonLoad3();
private:
	// �̹��� ��ü
	MyImage m_image0, m_image1, m_image2, m_image3;
	// ROI ��ü
	ROI roi0, roi1, roi2, roi3;
	CRect rect;

	
public:

	//���� ����ȭ
	void Binarization(MyImage& image);
	afx_msg void OnBnClickedButtonBin0();
	afx_msg void OnBnClickedButtonRoi0();

};
