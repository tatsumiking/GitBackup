
#pragma once

// dlepskld.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsKamonLoad �_�C�A���O

#define	LISTMAX	10

class CDialogEpsKamonLoad : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogEpsKamonLoad(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
public:
	void KamnSelectDisp(int idx);
	void NameListInit();
	void EpsDispLoad(CDC *pcCDC, LPTSTR epsname);
	void SkipEpsHead(FILE *fp);
	void DispEpsLoadLoop(FILE *fp, CDC *pcCDC);
	void BezeDraw(CDC *pcCDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void DispTimeTrns(LPDBL xx, LPDBL yy);
	int NumChk(TCHAR buf[], int len);
	BOOL ReadString(FILE *fp, TCHAR *buffer, int max);

public:
	TCHAR m_szFileName[FNAMEMAX];	
	DBL	m_dDispSx;
	DBL	m_dDispSy;
	DBL	m_dDispSize;
	DBL	m_dTime;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogEpsKamonLoad)
	enum { IDD = IDD_EPSKAMON_LOAD };
	CEdit	m_edKmnView;
	CListBox	m_lstName;
	double m_dKmnSize;
	CString	m_csSrchChar;
	CString	m_csSrchStrg;
	CString	m_csKmnView;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogEpsKamonLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogEpsKamonLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSrchChar();
	afx_msg void OnChangeSrchstrg();
	afx_msg void OnSelchangeNamelist();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B


