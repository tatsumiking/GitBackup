#if !defined(AFX_DSETMACKEY_H)
#define AFX_DSETMACKEY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSetMacKey.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetMacKey �_�C�A���O

class CDialogSetMacKey : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogSetMacKey(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogSetMacKey)
	enum { IDD = IDD_SETMACKEY };
	CString	m_csChkKey;
	CString	m_csSubKey;
	CString	m_csMsg;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSetMacKey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSetMacKey)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DSETMACKEY_H)
