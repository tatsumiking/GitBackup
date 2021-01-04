
/////////////////////////////////////////////////////////////////////////////
// CCmdDBEdt

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdDBEdit.h"

#include "DialogXLSSelect.h"
#include "DialogMDBSelect.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
//}

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
	TCHAR	szDataPath[FNAMEMAX];
	TCHAR	szExportFilter[RECMAX];
	CFileDialog	*pcFileDialog;
	CString		csStr;
	TCHAR	szFileName[FNAMEMAX];
	TCHAR	szExtension[QRECMAX];

	senvgetdatpath(szDataPath);
	_tcscpy_s(szExportFilter, _T("データーベースファイル(*.csv;*.xls;*.xlsx;*.mdb)|*.csv;*.xls;*xlsx;*.mdb||"));
	pcFileDialog = new CFileDialog(TRUE, NULL, _T("*.csv;*.xls;*.xlsx;*.mdb"), OFN_HIDEREADONLY, szExportFilter, m_pcView);
	pcFileDialog->m_ofn.lpstrInitialDir = szDataPath;
	if(pcFileDialog->DoModal( ) == IDOK){
		m_pcDoc->m_nDBType = 0;
		csStr = pcFileDialog->GetPathName();
		_tcscpy_s(szFileName, csStr.GetBuffer(FNAMEMAX));
		_tcscpy_s(szExtension, _tcschr(szFileName, _T('.')) + 1);
		_tcsupr_s(szExtension, QRECMAX);

		HANDLE hFp = CreateFile(szFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFp == 0 || hFp == (HANDLE)INVALID_HANDLE_VALUE){
			::AfxMessageBox(_T("ファイルが使用中のため接続できません"), MB_OK);
			return;
		}
		CloseHandle(hFp);

		m_pcView->m_nCursorKind = CUSKIND_WAIT;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
		if(_tcscmp(szExtension, _T("CSV")) == 0){
			FileCSVLoadMDBSet(szFileName);
		}else if(_tcscmp(szExtension, _T("MDB")) == 0){
			FileMDBLoadMDBSet(szFileName);
		}else if(_tcscmp(szExtension, _T("XLS")) == 0){
			FileXLSLoadMDBSet(szFileName);
		}else if(_tcscmp(szExtension, _T("XLSX")) == 0){
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

void CCmdDBEdit::FileCSVLoadMDBSet(LPTSTR lpFileName)
{
	m_nType = DBCSV;
	m_csFileName = lpFileName;
	m_csTableName = _T("");
}

void CCmdDBEdit::FileXLSLoadMDBSet(LPTSTR lpFileName)
{
	CDialogXLSSelect	*pcDXLSSlct;
	CString		csTable;

	m_nType = 0;
	pcDXLSSlct = new CDialogXLSSelect(m_pcView);
	pcDXLSSlct->m_csFileName = lpFileName;
	int ret = pcDXLSSlct->DoModal();
	if(ret == IDOK){
		csTable = pcDXLSSlct->m_csSheetName;
	}
	delete(pcDXLSSlct);
	if(ret == IDCANCEL){
		return;
	}
	m_nType = DBXLS;
	m_csFileName = lpFileName;
	m_csTableName = csTable;
}

void CCmdDBEdit::FileMDBLoadMDBSet(LPTSTR lpFileName)
{
	CDialogMDBSelect	*pcDMDBSlct;
	CString		csTable;
	int			nTBLorQRY;

	nTBLorQRY = 0;
	m_nType = 0;
	pcDMDBSlct = new CDialogMDBSelect(m_pcView);
	pcDMDBSlct->m_csDBFname = lpFileName;
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
	m_csFileName = lpFileName;
	m_csTableName = csTable;
}
