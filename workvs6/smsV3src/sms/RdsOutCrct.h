#if !defined(RDSOUTCRCT_H)
#define RDSOUTCRCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutCrctMDB.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CRdsOutCrct DAO ���R�[�h�Z�b�g

class CRdsOutCrct : public CDaoRecordset
{
public:
	CRdsOutCrct(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRdsOutCrct)

// �t�B�[���h/�p�����[�^ �f�[�^
	//{{AFX_FIELD(CRdsOutCrct, CDaoRecordset)
	long	m_column1;
	CString	m_column2;
	CString	m_column3;
	CString	m_column4;
	CString	m_column5;
	CString	m_column6;
	CString	m_column7;
	CString	m_column8;
	//}}AFX_FIELD

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CRdsOutCrct)
	public:
	virtual CString GetDefaultDBName();		// �f�t�H���g �f�[�^�x�[�X��
	virtual CString GetDefaultSQL();		// ���R�[�h�Z�b�g�̃f�t�H���g SQL
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(RDSOUTCRCT_H)
