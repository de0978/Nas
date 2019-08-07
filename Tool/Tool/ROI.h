#pragma once

class MyImage;

class ROI
{
private:
	CPoint m_pos, m_dPos; // ROI ���� ó�� ��ǥ, ������ ��ǥ
	BOOL m_centerFlag;
	BOOL m_roiFlag;
	
public:
	ROI();
	~ROI();

	// ROI ��ǥ ����
	void SetROI(CPoint pos, CPoint dPos);
	void SetPos(CPoint m_pos);
	void SetdPos(CPoint m_dPos);
	CPoint GetPos();
	CPoint GetdPos();

	// ROI ���� ����ȭ
	void ROIDibBinarization(MyImage& image, int th);

	// ������ ROI ���������� Hole�� �߽��� ��ǥ�� ��ȯ
	CPoint FindHole(MyImage& image, CPoint x1, CPoint x2);
	// ROI ���� ���� Ȯ��
	BOOL IsValid();
	// ��ǥ ������ ���� �Լ�
	CPoint CoordinateContingency(CPoint pos);

	void SetCenter(BOOL b);
	BOOL IsCenter();
	void SetROI(BOOL b);

	BOOL m_drawROI;
};

