
#pragma once

#include	"excel.h"

// [クラス名] CExcelLib
// [親クラス] CObject
// [クラス概要] エクセル操作ライブラリクラス
class CExcelLib : public CObject
{
public:
	CExcelLib();
public:
	_Application	m_excel;	// エクセルアプリケーションクラス
	Workbooks	m_workbooks;	// エクセルワークブック集合体クラス
	_Workbook	m_workbook;	// エクセルワークブッククラス
	Worksheets	m_worksheets;	// エクセルシート集合体クラス
	_Worksheet	m_worksheet;	// エクセルシートクラス
	Range		m_range;	// エクセルセル領域クラス
	// 共通
	LPDISPATCH	m_pWorkbooks;	// エクセルワークブック集合体クラスデスパッチポインタ
	LPDISPATCH	m_pWorkbook;	// エクセルワークブッククラスデスパッチポインタ
	LPDISPATCH	m_pWorksheets;	// エクセルシート集合体クラスデスパッチポインタ
	LPDISPATCH	m_pWorksheet;	// エクセルシートクラスデスパッチポインタ
	LPDISPATCH	m_pRange;	// エクセルセル領域クラスデスパッチポインタ
	int		m_nExcelType;	// エクセルタイプデスパッチポインタ

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

