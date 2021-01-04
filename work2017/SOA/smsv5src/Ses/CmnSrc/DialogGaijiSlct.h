
#pragma once

// DGjSlct.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSlct �_�C�A���O

class CDialogGaijiSlct : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogGaijiSlct(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nCrtTop;
	int	m_nLstMax;
	int	m_nPageMax;
	int	m_nFirstCode;
	int	m_nLastCode;
	TCHAR	m_szFolderName[RECMAX];
	TCHAR	m_szGaijiText[QRECMAX];
	HANDLE		m_hDib;   // �摜�\�p�o�b�t�@�n���h��
	ULONG		m_nDibSize;   // �摜�\�p�o�b�t�@�T�C�Y
	//{{AFX_DATA(CDialogGaijiSlct)
	enum { IDD = IDD_GAIJI_SLCT };
	CListBox	m_lstCode;
	CListBox	m_lstImg;
	CStatic	m_stTop;
	CStatic	m_stImg;
	CString	m_csCode;
	int		m_nSortKind;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogGaijiSlct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey);
	void InitListImageCode();
	void InitListKakusuSort();
	void InitListBusyuSort();
	void InitListCodeSort();
	void DrawPartImg(CDC *pcDC, CRect rect);
	void ImageDraw(CDC *pcDC, int sx, int sy, LPTSTR filename);
	void LegalRect(CRect *rect);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogGaijiSlct)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeLstimg();
	afx_msg void OnBtnload();
	afx_msg void OnRdocodesort();
	afx_msg void OnRdobusyusort();
	afx_msg void OnRdokakususort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
