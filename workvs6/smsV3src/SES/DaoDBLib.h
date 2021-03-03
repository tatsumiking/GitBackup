#if !defined(DAODBLIB_H)
#define DAODBLIB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaoDBLib.h : �w�b�_�[ �t�@�C��
//
#include	"ExcelLib.h"

/////////////////////////////////////////////////////////////////////////////
// CDaoDBLib

// [�N���X��] CDaoDBLib
// [�e�N���X] CObject
// [�N���X�T�v] XLS�ACSV�AMDB�����N�����N���X
class CDaoDBLib : public CObject
{
public:
	CDaoDBLib();
	DECLARE_DYNCREATE(CDaoDBLib);
	virtual ~CDaoDBLib();
// �A�g���r���[�g
public:
	int	m_nType;	// �����N�f�[�^�x�[�X�敪
	int	m_nDBOpen;	// �����N�I�[�v����ԃt���O
	CString	m_csFileName;	// �����N�t�@�C����
	CString	m_csTableName;	// �����N�e�[�u���A�V�[�g����

	CDaoRecordset* m_pcDaoRecordset;	// �f�[�^�x�[�X���R�[�h�Z�b�g
	CDaoDatabase* m_pcDaoDatabase;	// �f�[�^�x�[�X
	CDaoTableDef *m_pcTableDef;	// �f�[�^�x�[�X�e�[�u��
	CDaoQueryDef *m_pcQueryDef;	// �f�[�^�x�[�X�N�G��
	FILE	*m_fpCSV;	// CSV�t�@�C���|�C���^
	LPSTR	m_lpDBBuf;	// �ǂݍ��݃f�[�^�i�[�o�b�t�@
	LPSTR	m_lpCsvsp[DBFILDMAX];	// �ǂݍ��݃f�[�^�i�[�o�b�t�@
	int		m_nFildMax;	// �t�B�[���h��
	CExcelLib	m_cExcelLib;	// �G�N�Z���t�@�C���������C�u����
	int		m_nCrtLine;	// �ǂݍ��ݍs�ԍ�

// �I�y���[�V����
public:
	int CheckCsvFile(LPSTR lpFileName);
	CString ConversionVerToCStr(COleVariant var);
	void	Open();
	void	Close();
	int		InitGetFild();
	void	EndGetFild();
	int		InitGetRecord();
	void	EndGetRecord();
	int	GetFildName(int idx, LPSTR lpFildName);
	int	SetNextRecord();
	int	GetItemStr(int idx, LPSTR lpStr);
// �C���v�������e�[�V����
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	int IsKanji(UINT far hc);
	int DivCsvSetSp(LPSTR csvsp[], LPSTR csvbuf);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
#endif // !defined(DAODBLIB_H)