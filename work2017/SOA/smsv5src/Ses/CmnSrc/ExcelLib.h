
#pragma once

#include	"excel.h"

// [�N���X��] CExcelLib
// [�e�N���X] CObject
// [�N���X�T�v] �G�N�Z�����색�C�u�����N���X
class CExcelLib : public CObject
{
public:
	CExcelLib();
public:
	_Application	m_excel;	// �G�N�Z���A�v���P�[�V�����N���X
	Workbooks	m_workbooks;	// �G�N�Z�����[�N�u�b�N�W���̃N���X
	_Workbook	m_workbook;	// �G�N�Z�����[�N�u�b�N�N���X
	Worksheets	m_worksheets;	// �G�N�Z���V�[�g�W���̃N���X
	_Worksheet	m_worksheet;	// �G�N�Z���V�[�g�N���X
	Range		m_range;	// �G�N�Z���Z���̈�N���X
	// ����
	LPDISPATCH	m_pWorkbooks;	// �G�N�Z�����[�N�u�b�N�W���̃N���X�f�X�p�b�`�|�C���^
	LPDISPATCH	m_pWorkbook;	// �G�N�Z�����[�N�u�b�N�N���X�f�X�p�b�`�|�C���^
	LPDISPATCH	m_pWorksheets;	// �G�N�Z���V�[�g�W���̃N���X�f�X�p�b�`�|�C���^
	LPDISPATCH	m_pWorksheet;	// �G�N�Z���V�[�g�N���X�f�X�p�b�`�|�C���^
	LPDISPATCH	m_pRange;	// �G�N�Z���Z���̈�N���X�f�X�p�b�`�|�C���^
	int		m_nExcelType;	// �G�N�Z���^�C�v�f�X�p�b�`�|�C���^

public:
	void GetFileName(LPTSTR fname);
	BOOL SlctInit();
	void SlctEnd();
	BOOL Open(CString *pcsFileName);
	void Close();
	BOOL GetSheetName(int idx, CString *pcsSheetName);
	BOOL OpenSheet(CString *pcsSheetName);
	void CloseSheet();
	void SetCell(int col, int line, LPTSTR str);
	void GetCell(int col, int line, LPTSTR str);
private:
	BOOL CheckType();
	BOOL CreateApp();
	BOOL OpenFile(LPTSTR lpFileName);
	BOOL SlctFile();
	void CloseFile();
	void EndWorkBook();
	void InitWorkSheet(CString *pcsSheetName);
	void EndWorkSheet();
	void CreateCelName(int col, int line, LPTSTR cellname);
private:


};

