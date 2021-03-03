// prtctDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_PRTCTDLG_H__71320367_D9FE_11D3_9F64_0000E835797C__INCLUDED_)
#define AFX_PRTCTDLG_H__71320367_D9FE_11D3_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPrtctDlg �_�C�A���O

class CPrtctDlg : public CDialog
{
// �\�z
public:
	void ObjectCopy();
	char m_szDriveName[QRECMAX];
	int m_nDriveNo;
	char m_szSetLabel[RECMAX];	
	char m_szDelLabel[RECMAX];	
	CPrtctDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	HANDLE	m_hDev;
	char	m_szExePath[FNAMEMAX];
	//{{AFX_DATA(CPrtctDlg)
	enum { IDD = IDD_PRTCT_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPrtctDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

// �C���v�������e�[�V����
protected:
	void InitExePath(LPSTR lpExePath);
	void ReadLabel(LPSTR buf);
	void WriteLabel(LPSTR buf);
	void SetDriveNo();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPrtctDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnprtctset();
	afx_msg void OnBtnprtctreset();
	afx_msg void OnBtnset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PRTCTDLG_H__71320367_D9FE_11D3_9F64_0000E835797C__INCLUDED_)
