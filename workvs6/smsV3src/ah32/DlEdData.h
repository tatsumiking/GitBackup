#if !defined(AFX_DLEDDATA_H__42E0CB21_95F9_11D3_9DFE_00A0B0086478__INCLUDED_)
#define AFX_DLEDDATA_H__42E0CB21_95F9_11D3_9DFE_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlEdData.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEditData �_�C�A���O

class CDialogEditData : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogEditData(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
public:

// �_�C�A���O �f�[�^
public:
	CAh32Dlg	*m_pcAh32Dlg;
	HGLOBAL m_hEditBuf;
	LPSTR	m_lpEditBuf;
	//{{AFX_DATA(CDialogEditData)
	enum { IDD = IDD_EDIT_DATA };
	CEdit	m_edData;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogEditData)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	void SetStrgTextToFile();
	LPSTR CrDelFgets(char * buf, int max, FILE * fp);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogEditData)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLEDDATA_H__42E0CB21_95F9_11D3_9DFE_00A0B0086478__INCLUDED_)
