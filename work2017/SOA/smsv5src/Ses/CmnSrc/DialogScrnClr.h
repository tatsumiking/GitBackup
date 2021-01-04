
#pragma once

// DialogScrnClr.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogScrnClr �_�C�A���O

class CDialogScrnClr : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogScrnClr(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogScrnClr)
	enum { IDD = IDD_SCRNCLRSET };
	CStatic	m_stBezeClr;
	CStatic	m_stBkClr;
	CStatic	m_stFrClr;
	CStatic	m_stLineClr;
	CStatic	m_stCurvClr;
	CStatic	m_stAtnClr;
	CStatic	m_stSlctClr;
	CStatic	m_stNoslctClr;
	CStatic	m_stImageClr;
	CStatic	m_stLslctClr;
	CStatic	m_stSublClr;
	CStatic	m_stAreaClr;
	CStatic	m_stGridClr;
	CStatic	m_stScalClr;
	CStatic	m_stCntrClr;
	CStatic	m_stAsstClr;
	CStatic	m_stTopRect;
	int		m_nDotSize;
	//}}AFX_DATA

	long	m_rgbBkClr;
	long	m_rgbFrClr;
	long	m_rgbLineClr;
	long	m_rgbCurvClr;
	long	m_rgbBezeClr;
	long	m_rgbAtnClr;
	long	m_rgbSlctClr;
	long	m_rgbNoslctClr;
	long	m_rgbImageClr;
	long	m_rgbLslctClr;
	long	m_rgbSublClr;
	long	m_rgbAreaClr;
	long	m_rgbGridClr;
	long	m_rgbScalClr;
	long	m_rgbCntrClr;
	long	m_rgbAsstClr;
	long	m_rgbTopRect;

	CRect	m_rectBkClr;
	CRect	m_rectFrClr;
	CRect	m_rectLineClr;
	CRect	m_rectCurvClr;
	CRect	m_rectBezeClr;
	CRect	m_rectAtnClr;
	CRect	m_rectSlctClr;
	CRect	m_rectNoslctClr;
	CRect	m_rectImageClr;
	CRect	m_rectLslctClr;
	CRect	m_rectSublClr;
	CRect	m_rectAreaClr;
	CRect	m_rectGridClr;
	CRect	m_rectScalClr;
	CRect	m_rectCntrClr;
	CRect	m_rectAsstClr;
	CRect	m_rectTopRect;
	// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogScrnClr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SetItemElement();
	void GetItemElement();
	void ClrBarItemLocate();
protected:
	void ClrSelect(long *lRGB);
	void GetRGBValue(long lRGBClr, int *rval, int *gval, int *bval);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogScrnClr)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnChangeEddotsize();
	afx_msg void OnBtndefset();
	afx_msg void OnBtnClrchange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

