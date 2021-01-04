// ExcelLib.cpp : インプリメンテーション ファイル
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
// [名前] CExcelLib
// [概要] コンストラクタ
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// CEditEx メッセージ ハンドラ

// --------------------------------------------------------------------------------
// [名前] SlctInit
// [概要] 選択の初期化
// [引数] なし
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] SlctEnd
// [概要] 選択終了
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
// --------------------------------------------------------------------------------
void CExcelLib::SlctEnd()
{
	EndWorkSheet();
	CloseFile();
}

// --------------------------------------------------------------------------------
// [名前] Open
// [概要] ファイルのオープン処理
// [引数] CString *pcsFileName	// ファイル名
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
		//ファイル指定でEXCELを使用するとき
		//表示状態にしないとEXCELが残ってしまう
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
// [名前] Close
// [概要] 終了処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] OpenSheet
// [概要] シートオープン処理
// [引数] CString *pcsSheetName	// シート名
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
// --------------------------------------------------------------------------------
BOOL CExcelLib::OpenSheet(CString *pcsSheetName)
{
	InitWorkSheet(pcsSheetName);
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [名前] CloseSheet
// [概要] シート終了処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
// --------------------------------------------------------------------------------
void CExcelLib::CloseSheet()
{
	EndWorkSheet();
}

// --------------------------------------------------------------------------------
// [名前] GetFileName
// [概要] ファイル名の取得
// [引数] LPTSTR fname	// ファイル名
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] GetCell
// [概要] セルの文字列取得
// [引数] int col		// カラム番号
// [引数] int line		// 行番号
// [引数] LPTSTR str	// 文字列
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
	// #N/A に対する処理
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
// [名前] SetCell
// [概要] セルの文字列設定
// [引数] int col		// カラム番号
// [引数] int line		// 行番号
// [引数] LPTSTR str	// 文字列
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] CheckType
// [概要] Microsoft Excelのバージョン確認
// [引数] なし
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
// --------------------------------------------------------------------------------
BOOL CExcelLib::CheckType()
{
	CLSID clsid;

	// 2002.06.20 立身 サーバー使用中が出るため修正
	// 原因 CreateDispatchの引数は厳密でないとだめ？
	// Excel.ApplicationではだめExcel.Application.8に
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
		AfxMessageBox(_T("Microsoft Excelが必要です"));
		return(FALSE);
	}
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [名前] CreateApp
// [概要] 初期化
// [引数] なし
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
// --------------------------------------------------------------------------------
BOOL CExcelLib::CreateApp()
{
	if(m_nExcelType == 8){
		if(!m_excel.CreateDispatch(_T("Excel.Application.8"))) {
			AfxMessageBox(_T("Excel97をインストールしなおす必要があります"));
			return(FALSE);
		}
	}else if(m_nExcelType == 9){
		if(!m_excel.CreateDispatch(_T("Excel.Application.9"))) {
			AfxMessageBox(_T("Excel2000をインストールしなおす必要があります"));
			return(FALSE);
		}
	}else if(m_nExcelType == 10){
		if(!m_excel.CreateDispatch(_T("Excel.Application.10"))) {
			AfxMessageBox(_T("Excel2002をインストールしなおす必要があります"));
			return(FALSE);
		}
	}else if(m_nExcelType == 11){
		if(!m_excel.CreateDispatch(_T("Excel.Application.11"))) {
			AfxMessageBox(_T("Excel2003をインストールしなおす必要があります"));
			return(FALSE);
		}
	}else if(m_nExcelType == 12){
		if(!m_excel.CreateDispatch(_T("Excel.Application.12"))) {
			AfxMessageBox(_T("Excel2007をインストールしなおす必要があります"));
			return(FALSE);
		}
	}else{
		return(FALSE);
	}
	return(TRUE);
}

// --------------------------------------------------------------------------------
// [名前] SlctFile
// [概要] ファイル選択処理
// [引数] なし
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] OpenFile
// [概要] ファイルのオープン処理
// [引数] LPTSTR lpFileName	// ファイル名
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] EndWorkBook
// [概要] 終了処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] GetSheetName
// [概要] シート名の取得
// [引数] int idx	// インデックス
// [引数] CString *pcsSheetName	// シート名
// [戻値] BOOL
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] InitWorkSheet
// [概要] シートの初期化
// [引数] CString *pcsSheetName	// シート名
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
// [名前] EndWorkSheet
// [概要] シートの終了処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
// --------------------------------------------------------------------------------
void CExcelLib::EndWorkSheet()
{
	m_worksheet.DetachDispatch();
	m_worksheet.ReleaseDispatch();
	m_pWorksheet->Release();
}

// --------------------------------------------------------------------------------
// [名前] CloseFile
// [概要] ファイルの終了処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
// --------------------------------------------------------------------------------
void CExcelLib::CloseFile()
{
	m_excel.Quit();
	m_excel.ReleaseDispatch();
	m_excel.DetachDispatch();
}

// --------------------------------------------------------------------------------
// [名前] CreateCelName
// [概要] セルの位置から文字列の作成
// [引数] int col		// カラム番号
// [引数] int line		// 行番号
// [引数] LPTSTR cellname	// セル名称
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/01/10 TAKE コメント作成
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
