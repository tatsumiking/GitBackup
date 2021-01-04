#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGjSave.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSave �_�C�A���O

class CDialogGaijiSave : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogGaijiSave(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nCrtTop;
	int	m_nPageMax;
	int	m_nFirstCode;
	int	m_nLastCode;
	TCHAR	m_szFolderName[RECMAX];
	HANDLE		m_hDib;   // �摜�\�p�o�b�t�@�n���h��
	ULONG		m_nDibSize;   // �摜�\�p�o�b�t�@�T�C�Y
	//{{AFX_DATA(CDialogGaijiSave)
	enum { IDD = IDD_GAIJI_SAVE };
	CComboBox	m_cmbBusyu;
	CComboBox	m_cmbKakusu;
	CComboBox	m_cmbFont;
	CListBox	m_lstImg;
	CStatic	m_stTop;
	CStatic	m_stImg;
	CString	m_csCode;
	CString	m_csBusyu;
	CString	m_csFont;
	CString	m_csKakusu;
	Bitmap *m_pcGPBitmap;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogGaijiSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitCmbKakusu();
	void InitCmbBusyu();
	void InitCmbFont();
	void BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey);
	void InitListImage();
	void DrawPartImg(CDC *pcDC, CRect rect);
	void ImageDraw(CDC *pcDC, int sx, int sy, LPTSTR filename, ULONG nSize);
	void LegalRect(CRect *rect);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogGaijiSave)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeLstimg();
	afx_msg void OnSelchangeCmbfont();
	afx_msg void OnBtnsave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lstCode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
