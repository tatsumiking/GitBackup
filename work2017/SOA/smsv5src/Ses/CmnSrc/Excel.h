// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class
#ifndef __EXCEL_H
#define __EXCEL_H

// [�N���X��] _Application
// [�e�N���X] COleDispatchDriver
// [�N���X�T�v] �G�N�Z���p�A�v���P�[�V���������N���X
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

// [�N���X��] Workbooks
// [�e�N���X] COleDispatchDriver
// [�N���X�T�v] �G�N�Z���p���[�N�u�b�N�R���N�V���������N���X
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

// [�N���X��] _Workbook
// [�e�N���X] COleDispatchDriver
// [�N���X�T�v] �G�N�Z���p���[�N�u�b�N�����N���X
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

// [�N���X��] Worksheets
// [�e�N���X] COleDispatchDriver
// [�N���X�T�v] �G�N�Z���p���[�N�V�[�g�R���N�V���������N���X
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

// [�N���X��] _Worksheet
// [�e�N���X] COleDispatchDriver
// [�N���X�T�v] �G�N�Z���p���[�N�V�[�g�����N���X
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

// [�N���X��] Range
// [�e�N���X] COleDispatchDriver
// [�N���X�T�v] �G�N�Z���p�Z���͈͏����N���X
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
