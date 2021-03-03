#if !defined(DCLRBAR_H)
#define DCLRBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlClrBar.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogClrBar �_�C�A���O

#define TOPSX 4
#define TOPSY 4
#define XSPACE 22
#define XSIZE 18
#define YSIZE 18
#define PLTMAX 50
#define MOVEPLT	10
#define XSIZESLCT	18
#define YSIZESLCT	18

class CTonView;

class CDialogClrBar : public CDialogBar
{
// �R���X�g���N�V����
public:
	void SetOutLineClr();
	void SetFillClr();
	CDialogClrBar(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CBitmapButton	cPreBtn;
	CBitmapButton	cNextBtn;
	CBitmapButton	cNonClrBtn;
	CRect	m_rectPre;
	CRect	m_rectNext;
	CRect	m_rectNonClr;
	int m_nTopSx, m_nTopSy;
	int m_nFirstFlag;
	int	m_nCrtPltTopNo;
	int	m_nCrtFillPlt;
	int	m_nCrtLinePlt;
	DBL	m_dCrtThick;

	void PltListInit();
	void DrawSelectPaletBoxs(CPaintDC *dc);
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogClrBar)
	enum { IDD = IDD_CLRBAR };
	CString	m_csPltFilename;
	double	m_dLineThick;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogClrBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogClrBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNextbtn();
	afx_msg void OnPrebtn();
	afx_msg void OnSelchangePltcombo();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeThick();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DCLRBAR_H)
