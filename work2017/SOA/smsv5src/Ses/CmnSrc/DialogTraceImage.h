
#pragma once

// DlgTrImg.h : �w�b�_�[ �t�@�C��
//
#include	"mydialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogTraceImage �_�C�A���O

class CDialogTraceImage : public CMyDialog
{
// �R���X�g���N�V����
public:
	CDialogTraceImage(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDialogTraceImage();

	CTonView*	m_pcView;
	COLORREF	m_clrPicup;
	TCHAR		m_szBmpFname[FNAMEMAX];
	HANDLE		m_hDib;

public:
	void ChengRGBQUAD();
	void SetImage(LPTSTR lpBmpFname);
	void GetImageAreaSize(LPDBL xsize, LPDBL ysize);
	void PicupClrDraw();
	virtual void KeyIn(UINT nTCHAR, UINT nID);
	void SetItemElement();
	void InitItemElement();
	void SetClr(COLORREF picupClr);

// �_�C�A���O �f�[�^
	int		m_nFureLimit;
	//{{AFX_DATA(CDialogTraceImage)
	enum { IDD = IDD_TRACE_IMAGE };
	CSliderCtrl	m_slidAreaLimit;
	int		m_nAreaLimit;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogTraceImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogTraceImage)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnChangeEdit1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLineTrace();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
