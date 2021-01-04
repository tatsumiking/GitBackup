#if !defined(DLGIMGDSP_H)
#define DLGIMGDSP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImgDsp.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogImageDisp �_�C�A���O
class CCrctWaku;

class CDialogImageDisp : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogImageDisp(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDialogImageDisp();
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogImageDisp)
	enum { IDD = IDD_IMAGEDISP };
	CStatic	m_stTop;
	CStatic	m_stImgTop;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogImageDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	TCHAR	m_szTitle[QRECMAX];
	TCHAR	m_szFont[RECMAX];
	COLORREF	m_clrSlct;

	DBL	m_dDispTime;
	DBL	m_dPrntTime;
	DBL	m_nPrntXSize;
	DBL	m_nPrntYSize;

	int	m_nNoWakuOut;
	DBL	m_dDispBlkXSize;
	DBL	m_dDispBlkYSize;
	DBL	m_dDispBlkXSpace;
	DBL	m_dDispBlkYSpace;

	DBL	m_dPrntBlkXStart;
	DBL	m_dPrntBlkYStart;
	DBL	m_dPrntBlkXSize;
	DBL	m_dPrntBlkYSize;
	DBL	m_dPrntBlkXSpace;
	DBL	m_dPrntBlkYSpace;

	int	m_nCrtTop;
	int	m_nDataMax;
	int	m_nOnePageCnt;
	int	m_nInSort;
	int	m_nExitCall;
// �C���v�������e�[�V����
public:
	void InitChangeElement();
	void SetChangeElement();
	void DrawBoxs(CDC *pcDC, CRect rect);
	void InitPrintEnv(CDC *pcDC);
	void PrintOutMain(CDC *pcDC);
	void InitDataMax();
	void LoadImageElement();
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogImageDisp)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBtnexit();
	afx_msg void OnBtnprepage();
	afx_msg void OnBtnlistdisp();
	afx_msg void OnBtnprintout();
	afx_msg void OnBtnnextpage();
	afx_msg void OnBtnjpgout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DLGIMGDSP_H)
