
/////////////////////////////////////////////////////////////////////////////
// CCmdDBEdt

#include "stdafx.h"
#include "math.h"

#include "..\cmnsrc\cmd_base.h"
#include "cmddbedt.h"
#include "DMDBSlct.h"
#include "DXLSSlct.h"

extern "C" {
	void senvgetdatpath(LPSTR path);
}

CCmdDBEdit::CCmdDBEdit(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdDBEdit::ExecDBLoad(UINT event, DBL dx, DBL dy)
{
	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	if(m_pcDoc->m_pcDialogDBEdit != NULL){
		m_pcDoc->DestroyDialogDBEdit();
	}
	SelectInitDB();
	if(m_pcDoc->m_nDBType == 0){
		return(ENDEVENT);
	}
	if(m_pcDoc->CreateDialogDBEdit(m_pcView) == FALSE){
		return(ENDEVENT);
	}
	m_pcDoc->ShowDialogDBEdit();
	InitToolBarElement();
	return(ENDEVENT);
}

UINT CCmdDBEdit::ExecDBDelete(UINT event, DBL dx, DBL dy)
{
	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	if(m_pcDoc->m_pcDialogDBEdit != NULL){
		m_pcDoc->DestroyDialogDBEdit();
		m_pcDoc->m_pcDialogDBEdit = NULL;
	}
	m_pcDoc->UpdateDBObject();
	InitToolBarElement();
	ScrnAllRedraw();
	return(ENDEVENT);
}

void CCmdDBEdit::SelectInitDB()
{
	char	szDataPath[FNAMEMAX];
	char	szExportFilter[RECMAX];
	CFileDialog	*pcFileDialog;
	CString		csStr;
	char	szFileName[FNAMEMAX];
	char	szExtension[QRECMAX];

	senvgetdatpath(szDataPath);
	strcpy(szExportFilter, "データーベースファイル(*.csv;*.xls;*.xlsx;*.mdb)|*.csv;*.xls;*xlsx;*.mdb||");
	pcFileDialog = new CFileDialog(TRUE, NULL, "*.csv;*.xls;*.xlsx;*.mdb", OFN_HIDEREADONLY, szExportFilter, m_pcView);
	pcFileDialog->m_ofn.lpstrInitialDir = _T(szDataPath);
	if(pcFileDialog->DoModal( ) == IDOK){
		m_pcDoc->m_nDBType = 0;
		csStr = pcFileDialog->GetPathName();
		strcpy(szFileName, csStr.GetBuffer(FNAMEMAX));
		strcpy(szExtension, strrchr(szFileName, '.') + 1);
		strupr(szExtension);

		HANDLE hFp = CreateFile(szFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFp == 0 || hFp == (HANDLE)INVALID_HANDLE_VALUE){
			::AfxMessageBox("ファイルが使用中のため接続できません", MB_OK);
			return;
		}
		CloseHandle(hFp);

		m_pcView->m_nCursorKind = CUSKIND_WAIT;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
		if(strcmp(szExtension, "CSV") == 0){
			FileCSVLoadMDBSet(szFileName);
		}else if(strcmp(szExtension, "MDB") == 0){
			FileMDBLoadMDBSet(szFileName);
		}else if(strcmp(szExtension, "XLS") == 0){
			FileXLSLoadMDBSet(szFileName);
		}else if(strcmp(szExtension, "XLSX") == 0){
			FileXLSLoadMDBSet(szFileName);
		}else{
			m_pcView->m_nCursorKind = CUSKIND_ARROW;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			return;
		}
		m_pcDoc->m_nDBType = m_nType;
		m_pcDoc->m_csDBFileName = m_csFileName;
		m_pcDoc->m_csDBTableName = m_csTableName;
		m_pcView->m_nCursorKind = CUSKIND_ARROW;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
	}
	delete(pcFileDialog);
}

void CCmdDBEdit::FileCSVLoadMDBSet(LPSTR lpFileName)
{
	m_nType = DBCSV;
	m_csFileName = _T(lpFileName);
	m_csTableName = _T("");
}

void CCmdDBEdit::FileXLSLoadMDBSet(LPSTR lpFileName)
{
	CDialogXLSSelect	*pcDXLSSlct;
	CString		csTable;

	m_nType = 0;
	pcDXLSSlct = new CDialogXLSSelect(m_pcView);
	pcDXLSSlct->m_csFileName = _T(lpFileName);
	int ret = pcDXLSSlct->DoModal();
	if(ret == IDOK){
		csTable = pcDXLSSlct->m_csSheetName;
	}
	delete(pcDXLSSlct);
	if(ret == IDCANCEL){
		return;
	}
	m_nType = DBXLS;
	m_csFileName = _T(lpFileName);
	m_csTableName = csTable;
}

void CCmdDBEdit::FileMDBLoadMDBSet(LPSTR lpFileName)
{
	CDialogMDBSelect	*pcDMDBSlct;
	CString		csTable;
	int			nTBLorQRY;

	nTBLorQRY = 0;
	m_nType = 0;
	pcDMDBSlct = new CDialogMDBSelect(m_pcView);
	pcDMDBSlct->m_csDBFname = _T(lpFileName);
	int ret = pcDMDBSlct->DoModal();
	if(ret == IDOK){
		nTBLorQRY = pcDMDBSlct->m_nTBLorQRY;
		csTable = pcDMDBSlct->m_csTableName;
	}
	delete(pcDMDBSlct);
	if(ret == IDCANCEL){
		return;
	}
	if(nTBLorQRY == 0){
		m_nType = DBMDBT;
	}else{
		m_nType = DBMDBQ;
	}
	m_csFileName = _T(lpFileName);
	m_csTableName = csTable;
}
