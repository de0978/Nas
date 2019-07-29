#pragma once

class MyImage;

class ROI
{
private:
	CPoint m_pos, m_dPos; // ROI ���� ó�� ��ǥ, ������ ��ǥ

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
	void ROIDibBinarization(MyImage& dib, int th);

	// ������ ROI ���������� Hole�� �߽��� ��ǥ�� ��ȯ
	CPoint FindHole(MyImage& dib);
	// ROI ���� ���� Ȯ��
	BOOL IsValid();
};

