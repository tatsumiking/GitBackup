// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class
#ifndef __EXCEL_H
#define __EXCEL_H

// [クラス名] _Application
// [親クラス] COleDispatchDriver
// [クラス概要] エクセル用アプリケーション処理クラス
class _Application : public COleDispatchDriver
{
public:
	_Application() {}		// Calls COleDispatchDriver default constructor
	_Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Application(const _Application & dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
// Operations
public:
	void FindFile();
	BOOL FindFileV9();
	LPDISPATCH GetWorksheets();
	LPDISPATCH GetWorkbooks();
	LPDISPATCH GetActiveWorkbook();
	void Quit();
};

/////////////////////////////////////////////////////////////////////////////
// Workbooks wrapper class

// [クラス名] Workbooks
// [親クラス] COleDispatchDriver
// [クラス概要] エクセル用ワークブックコレクション処理クラス
class Workbooks : public COleDispatchDriver
{
public:
	Workbooks() {}		// Calls COleDispatchDriver default constructor
	Workbooks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Workbooks(const Workbooks & dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH OpenBook(LPCTSTR Filename, const VARIANT& UpdateLinks);
	void Close();
	long GetCount();
};

/////////////////////////////////////////////////////////////////////////////
// _Workbook wrapper class

// [クラス名] _Workbook
// [親クラス] COleDispatchDriver
// [クラス概要] エクセル用ワークブック処理クラス
class _Workbook : public COleDispatchDriver
{
public:
	_Workbook() {}		// Calls COleDispatchDriver default constructor
	_Workbook(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Workbook(const _Workbook & dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetWorksheets();
	CString GetPath();
	CString GetFullName();
	CString GetName();
};

/////////////////////////////////////////////////////////////////////////////
// Worksheets wrapper class

// [クラス名] Worksheets
// [親クラス] COleDispatchDriver
// [クラス概要] エクセル用ワークシートコレクション処理クラス
class Worksheets : public COleDispatchDriver
{
public:
	Worksheets() {}		// Calls COleDispatchDriver default constructor
	Worksheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Worksheets(const Worksheets & dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(const VARIANT& Index);
};

/////////////////////////////////////////////////////////////////////////////
// _Worksheet wrapper class

// [クラス名] _Worksheet
// [親クラス] COleDispatchDriver
// [クラス概要] エクセル用ワークシート処理クラス
class _Worksheet : public COleDispatchDriver
{
public:
	_Worksheet() {}		// Calls COleDispatchDriver default constructor
	_Worksheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Worksheet(const _Worksheet & dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString	GetName();

// Operations
public:
	void Select(); // added
	LPDISPATCH GetRange(const VARIANT& Cell1); // added
};
/////////////////////////////////////////////////////////////////////////////
// Range wrapper class

// [クラス名] Range
// [親クラス] COleDispatchDriver
// [クラス概要] エクセル用セル範囲処理クラス
class Range : public COleDispatchDriver
{
public:
	Range() {}		// Calls COleDispatchDriver default constructor
	Range(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Range(const Range & dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
};

#endif
