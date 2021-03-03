// wr32Dlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_WR32DLG_H__5CDF5607_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
#define AFX_WR32DLG_H__5CDF5607_823E_11D3_BF1B_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWr32Dlg �_�C�A���O

class CWr32Dlg : public CDialog
{
// �\�z
public:
	CWr32Dlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

public:
	void InitExePath(LPSTR lpExePath);
	void LoadSeting();
	void CreateHead(LPSTR wakunum, LPSTR head);
	void ProgramExec();
	void GetProgranName(int no, char *lpprog);
	BOOL ConvertWroutRec();
	void SrchWakuName(LPSTR wakunum, LPSTR waku);
	int DivCsvData(LPSTR csvsp[], LPSTR csvbuf);
	int IsKanji(int hc);

public:
	int		m_nFirstFlag;
	char	m_szExePath[FNAMEMAX];
	char	m_szGrup[QRECMAX];
	char	m_szPaper[QRECMAX];
	char	m_szWakuPath[FNAMEMAX];

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CWr32Dlg)
	enum { IDD = IDD_WR32_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CWr32Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CWr32Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_WR32DLG_H__5CDF5607_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
