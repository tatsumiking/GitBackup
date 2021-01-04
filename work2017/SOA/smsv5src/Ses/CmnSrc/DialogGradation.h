
#pragma once

// DlgGrdtn.h : �w�b�_�[ �t�@�C��
//
#include	"mydialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogGradation �_�C�A���O

#define	MARKMAX	32
#define PTNMAX	10

//C �O���f�[�V�����ݒ�_�C�A���O�N���X
class CDialogGradation : public CMyDialog
{
// �R���X�g���N�V����
public:
	void PltCopy(LPRGBQUAD lpclrgraptn);
	void GraPtnLoad(int ptn);
	void ClrTableReset();
	void SelectMarkDraw(CDC *pcdc, int mark);
	void Create256Dib();
	void SetGraBarPtn();

	
	CTonView*	m_pcView;
	HANDLE	m_hDibGraPtn[PTNMAX];
	int		m_nPtnSx[PTNMAX];
	int		m_nPtnSy[PTNMAX];
	int		m_nBitCount[PTNMAX];
	int		m_nPtnMax;

	COLORREF	m_clrSet[MARKMAX];
	int			m_nSetx[MARKMAX];

	int		m_nCrtSelect;
	int		m_nSelectMark;
	int		m_nTopx, m_nTopy;
	int		m_nXSpace, m_nYSpace;
	int		m_nBarSx, m_nBarEx;
	int		m_nBarSy;

	long	m_sizeDib;
	HANDLE	m_hDib;

	CDialogGradation(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDialogGradation();
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogGradation)
	enum { IDD = IDD_GRADATION };
	CStatic	m_stTop;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogGradation)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogGradation)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

