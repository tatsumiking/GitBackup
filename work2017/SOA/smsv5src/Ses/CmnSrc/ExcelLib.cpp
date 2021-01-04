// ExcelLib.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include <stdio.h>
#include <io.h>

#include "stdafx.h"
#pragma optimize( "", off )

#include "ExcelLib.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
//}

/////////////////////////////////////////////////////////////////////////////
// CExcelLib

// --------------------------------------------------------------------------------
// [���O] CExcelLib
// [�T�v] �R���X�g���N�^
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
CExcelLib::CExcelLib()
:CObject()
{
	m_pWorkbooks = NULL;
	m_pWorkbook = NULL;
	m_pWorksheets = NULL;
	m_pWorksheet = NULL;
	m_pRange = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CEditEx ���b�Z�[�W �n���h��

// --------------------------------------------------------------------------------
// [���O] SlctInit
// [�T�v] �I���̏�����
// [����] �Ȃ�
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::SlctInit()
{
	m_pWorkbooks = NULL;
	m_pWorkbook = NULL;
	m_pWorksheets = NULL;
	m_pWorksheet = NULL;
	m_pRange = NULL;
	if(CheckType() == FALSE){
		return(FALSE);
	}
	if(CreateApp() == FALSE){
		return(FALSE);
	}
	if(SlctFile() == FALSE){
		return(FALSE);
	}
	CWnd* pWnd = CWnd::FindWindow(_T("XLMAIN"), NULL);
	if (pWnd != NULL) {
		pWnd->ShowWindow(SW_HIDE);
		pWnd->UpdateWindow();
	}else{
		return(FALSE);
	}
	//InitWorkSheet();
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] SlctEnd
// [�T�v] �I���I��
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::SlctEnd()
{
	EndWorkSheet();
	CloseFile();
}

// --------------------------------------------------------------------------------
// [���O] Open
// [�T�v] �t�@�C���̃I�[�v������
// [����] CString *pcsFileName	// �t�@�C����
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::Open(CString *pcsFileName)
{
	TCHAR	szFileName[FNAMEMAX];

	_tcscpy_s(szFileName, pcsFileName->GetBuffer(FNAMEMAX));
	m_pWorkbooks = NULL;
	m_pWorkbook = NULL;
	m_pWorksheets = NULL;
	m_pWorksheet = NULL;
	m_pRange = NULL;
	if(_taccess_s(szFileName, 0) != 0){
		return(FALSE);
	}
	if(CheckType() == FALSE){
		return(FALSE);
	}
	if(CreateApp() == FALSE){
		return(FALSE);
	}
	if(OpenFile(szFileName) == FALSE){
		return(FALSE);
	}
	CWnd* pWnd = CWnd::FindWindow(_T("XLMAIN"), NULL);
	if (pWnd != NULL) {
		pWnd->ShowWindow(SW_SHOWNORMAL);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
		//�t�@�C���w���EXCEL���g�p����Ƃ�
		//�\����Ԃɂ��Ȃ���EXCEL���c���Ă��܂�
		//pWnd->ShowWindow(SW_HIDE);
		//pWnd->UpdateWindow();
 	}else{
		return(FALSE);
	}
	m_pWorksheets = m_workbook.GetWorksheets();
	m_worksheets.AttachDispatch(m_pWorksheets);
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] Close
// [�T�v] �I������
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::Close()
{
	m_worksheets.DetachDispatch();
	m_worksheets.ReleaseDispatch();
	m_pWorksheets->Release();
	EndWorkBook();
	CloseFile();
}

// --------------------------------------------------------------------------------
// [���O] OpenSheet
// [�T�v] �V�[�g�I�[�v������
// [����] CString *pcsSheetName	// �V�[�g��
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::OpenSheet(CString *pcsSheetName)
{
	InitWorkSheet(pcsSheetName);
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] CloseSheet
// [�T�v] �V�[�g�I������
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::CloseSheet()
{
	EndWorkSheet();
}

// --------------------------------------------------------------------------------
// [���O] GetFileName
// [�T�v] �t�@�C�����̎擾
// [����] LPTSTR fname	// �t�@�C����
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::GetFileName(LPTSTR fname)
{
	CString csPath;
	CString csFile;
	TCHAR	szPath[FNAMEMAX];
	TCHAR	szFile[FNAMEMAX];

	if(m_pWorkbook == NULL){
		m_pWorkbook = m_excel.GetActiveWorkbook();
		m_workbook.AttachDispatch(m_pWorkbook);
		csPath = m_workbook.GetPath();
		csFile = m_workbook.GetName();
		m_workbook.DetachDispatch();
		m_workbook.ReleaseDispatch();
		m_pWorkbook->Release();
		m_pWorkbook = NULL;
	}else{
		csPath = m_workbook.GetPath();
		csFile = m_workbook.GetName();
	}
	_tcscpy_s(szPath, csPath.GetBuffer(RECMAX));
	_tcscpy_s(szFile, csFile.GetBuffer(RECMAX));
	_stprintf_s(fname, FNAMEMAX, _T("%s\\%s"), szPath, szFile);
}

// --------------------------------------------------------------------------------
// [���O] GetCell
// [�T�v] �Z���̕�����擾
// [����] int col		// �J�����ԍ�
// [����] int line		// �s�ԍ�
// [����] LPTSTR str	// ������
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::GetCell(int col, int line, LPTSTR str)
{
	COleVariant		vaResult;
	TCHAR	cellname[QRECMAX];
	CString	csStr;
	int		len;

	CreateCelName(col, line, cellname);
	m_pRange = m_worksheet.GetRange(COleVariant(cellname));
	m_range.AttachDispatch(m_pRange);
	vaResult = m_range.GetValue();
	// #N/A �ɑ΂��鏈��
	if(vaResult.vt == VT_ERROR){
		csStr = _T("");
	}else{
		vaResult.ChangeType(VT_BSTR, &vaResult);
		csStr = vaResult.bstrVal;
	}
	len = csStr.GetLength()+1;
	if(RECMAX8 < len){
		len = RECMAX8;
	}
	_tcscpy_s(str, len, csStr.GetBuffer(RECMAX8));
	str[RECMAX8-2] = _T('\0');
	str[RECMAX8-1] = _T('\0');
	m_range.DetachDispatch();
	m_range.ReleaseDispatch();
	m_pRange->Release();
}

// --------------------------------------------------------------------------------
// [���O] SetCell
// [�T�v] �Z���̕�����ݒ�
// [����] int col		// �J�����ԍ�
// [����] int line		// �s�ԍ�
// [����] LPTSTR str	// ������
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::SetCell(int col, int line, LPTSTR str)
{
	COleVariant		vaResult;
	TCHAR	cellname[QRECMAX];
	CString	csStr;

	CreateCelName(col, line, cellname);
	m_pRange = m_worksheet.GetRange(COleVariant(cellname));
	m_range.AttachDispatch(m_pRange);
	m_range.SetValue(COleVariant(str));
	m_range.DetachDispatch();
	m_range.ReleaseDispatch();
	m_pRange->Release();
}

// --------------------------------------------------------------------------------
// [���O] CheckType
// [�T�v] Microsoft Excel�̃o�[�W�����m�F
// [����] �Ȃ�
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::CheckType()
{
	CLSID clsid;

	// 2002.06.20 ���g �T�[�o�[�g�p�����o�邽�ߏC��
	// ���� CreateDispatch�̈����͌����łȂ��Ƃ��߁H
	// Excel.Application�ł͂���Excel.Application.8��
	if(::CLSIDFromProgID(L"Excel.Application.8", &clsid) == S_OK){
		m_nExcelType = 8;
	}else if(::CLSIDFromProgID(L"Excel.Application.9", &clsid) == S_OK){
		m_nExcelType = 9;
	}else if(::CLSIDFromProgID(L"Excel.Application.10", &clsid) == S_OK){
		m_nExcelType = 10;
	}else if(::CLSIDFromProgID(L"Excel.Application.11", &clsid) == S_OK){
		m_nExcelType = 11;
	}else if(::CLSIDFromProgID(L"Excel.Application.12", &clsid) == S_OK){
		m_nExcelType = 12;
	}else{
		AfxMessageBox(_T("Microsoft Excel���K�v�ł�"));
		return(FALSE);
	}
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] CreateApp
// [�T�v] ������
// [����] �Ȃ�
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::CreateApp()
{
	if(m_nExcelType == 8){
		if(!m_excel.CreateDispatch(_T("Excel.Application.8"))) {
			AfxMessageBox(_T("Excel97���C���X�g�[�����Ȃ����K�v������܂�"));
			return(FALSE);
		}
	}else if(m_nExcelType == 9){
		if(!m_excel.CreateDispatch(_T("Excel.Application.9"))) {
			AfxMessageBox(_T("Excel2000���C���X�g�[�����Ȃ����K�v������܂�"));
			return(FALSE);
		}
	}else if(m_nExcelType == 10){
		if(!m_excel.CreateDispatch(_T("Excel.Application.10"))) {
			AfxMessageBox(_T("Excel2002���C���X�g�[�����Ȃ����K�v������܂�"));
			return(FALSE);
		}
	}else if(m_nExcelType == 11){
		if(!m_excel.CreateDispatch(_T("Excel.Application.11"))) {
			AfxMessageBox(_T("Excel2003���C���X�g�[�����Ȃ����K�v������܂�"));
			return(FALSE);
		}
	}else if(m_nExcelType == 12){
		if(!m_excel.CreateDispatch(_T("Excel.Application.12"))) {
			AfxMessageBox(_T("Excel2007���C���X�g�[�����Ȃ����K�v������܂�"));
			return(FALSE);
		}
	}else{
		return(FALSE);
	}
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] SlctFile
// [�T�v] �t�@�C���I������
// [����] �Ȃ�
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::SlctFile()
{
	if(m_nExcelType == 8 
	|| m_nExcelType == 10
	|| m_nExcelType == 11
	|| m_nExcelType == 12){
		m_excel.FindFile();
	}else if(m_nExcelType == 9){
		if(m_excel.FindFileV9() == FALSE){
			return(FALSE);
		}
	}
	m_pWorksheets = m_excel.GetWorksheets();
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] OpenFile
// [�T�v] �t�@�C���̃I�[�v������
// [����] LPTSTR lpFileName	// �t�@�C����
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::OpenFile(LPTSTR lpFileName)
{
	short	num;

	num = 0;
	m_pWorkbooks = m_excel.GetWorkbooks();
	m_workbooks.AttachDispatch(m_pWorkbooks);
	m_pWorkbook = m_workbooks.OpenBook(lpFileName, COleVariant(num)); 
	m_workbook.AttachDispatch(m_pWorkbook);
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] EndWorkBook
// [�T�v] �I������
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::EndWorkBook()
{
	m_workbook.DetachDispatch();
	m_workbook.ReleaseDispatch();
	m_pWorkbook->Release();
	m_workbooks.DetachDispatch();
	m_workbooks.ReleaseDispatch();
	m_pWorkbooks->Release();
}

// --------------------------------------------------------------------------------
// [���O] GetSheetName
// [�T�v] �V�[�g���̎擾
// [����] int idx	// �C���f�b�N�X
// [����] CString *pcsSheetName	// �V�[�g��
// [�ߒl] BOOL
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL CExcelLib::GetSheetName(int idx, CString *pcsSheetName)
{
	try{
		m_pWorksheet = m_worksheets.GetItem(COleVariant((short) idx));
		m_worksheet.AttachDispatch(m_pWorksheet);
		*pcsSheetName = m_worksheet.GetName();
		m_worksheet.DetachDispatch();
		m_worksheet.ReleaseDispatch();
		m_pWorksheet->Release();
		m_pWorksheet = NULL;
	}
	catch (CException* e1){
		e1->Delete();
		return(FALSE);
	}
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [���O] InitWorkSheet
// [�T�v] �V�[�g�̏�����
// [����] CString *pcsSheetName	// �V�[�g��
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::InitWorkSheet(CString *pcsSheetName)
{
	int		idx;
	CString	csChkName;
	
	idx = 1;
	try{
		while(1){
			m_pWorksheet = m_worksheets.GetItem(COleVariant((short) idx));
			m_worksheet.AttachDispatch(m_pWorksheet);
			csChkName = m_worksheet.GetName();
			if(*pcsSheetName == csChkName){
				break;
			}
			m_worksheet.DetachDispatch();
			m_worksheet.ReleaseDispatch();
			m_pWorksheet->Release();
			idx++;
		}
	}
	catch (CException* e1){
		e1->Delete();
		if(idx != 1){
			m_pWorksheet = m_worksheets.GetItem(COleVariant((short) idx));
			m_worksheet.AttachDispatch(m_pWorksheet);
		}else{
			return;
		}
	}
	m_worksheet.Select();
}

// --------------------------------------------------------------------------------
// [���O] EndWorkSheet
// [�T�v] �V�[�g�̏I������
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::EndWorkSheet()
{
	m_worksheet.DetachDispatch();
	m_worksheet.ReleaseDispatch();
	m_pWorksheet->Release();
}

// --------------------------------------------------------------------------------
// [���O] CloseFile
// [�T�v] �t�@�C���̏I������
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::CloseFile()
{
	m_excel.Quit();
	m_excel.ReleaseDispatch();
	m_excel.DetachDispatch();
}

// --------------------------------------------------------------------------------
// [���O] CreateCelName
// [�T�v] �Z���̈ʒu���當����̍쐬
// [����] int col		// �J�����ԍ�
// [����] int line		// �s�ԍ�
// [����] LPTSTR cellname	// �Z������
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/01/10 TAKE �R�����g�쐬
// --------------------------------------------------------------------------------
void CExcelLib::CreateCelName(int col, int line, LPTSTR cellname)
{
	TCHAR	colname[4];
	int		div, rest;

	col -= 1;
	if(col < 26){
		colname[0] = _T('A')+col;
		colname[1] = _T('\0');
	}else{
		div = (col / 26 - 1);
		rest = col % 26;
		colname[0] = _T('A')+div;
		colname[1] = _T('A')+rest;
		colname[2] = _T('\0');
	}
	_stprintf_s(cellname, QRECMAX, _T("%s%d"), colname, line);
}
