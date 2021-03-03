#if !defined(AFX_DLGLSTDSP_H__FDE97CA2_D7F3_4E11_ADC1_3CED96040A50__INCLUDED_)
#define AFX_DLGLSTDSP_H__FDE97CA2_D7F3_4E11_ADC1_3CED96040A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLstDsp.h : �w�b�_�[ �t�@�C��
//

#include	"rdsoutcrct.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogListDisp �_�C�A���O
#define	CNTTBLMAX	50
#define CNTMINI		6
#define NOCLMSIZE	3

class CDialogListDisp : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogListDisp(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogListDisp)
	enum { IDD = IDD_LISTDISP };
	CListBox	m_lstRecText;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogListDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	char	m_szTitle[QRECMAX];
	int		m_nInSort;
	int		m_nExitCall;
	CDaoDatabase	m_dbOutcrct;
	CRdsOutCrct*	m_pcRdsOutCrct;
	DBL	m_dPrntTime;
	DBL	m_nPrntXSize;
	DBL	m_nPrntYSize;
	int	m_nCntTbl[CNTTBLMAX];
// �C���v�������e�[�V����
public:
	void InitSetDB();
	void InitPrintEnv(CDC *pcDC);
	void PrintOutMain(CDC *pcDC);
	void EndCrctDB();
	void InitCrctDB();
	void InitRecTextList();
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogListDisp)
	afx_msg void OnBtnimagedisp();
	afx_msg void OnBtnprintout();
	afx_msg void OnBtnexit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGLSTDSP_H__FDE97CA2_D7F3_4E11_ADC1_3CED96040A50__INCLUDED_)
