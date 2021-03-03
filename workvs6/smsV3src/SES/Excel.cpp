// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "excel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// [名前] FindFile
// [概要] ファイル選択処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
void _Application::FindFile()
{
	InvokeHelper(0x42c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [名前] FindFileV9
// [概要] ファイル選択処理
// [引数] なし
// [戻値] BOOL	// 処理の可否
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
BOOL _Application::FindFileV9()
{
	BOOL result;
	InvokeHelper(0x6eb, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetWorkbooks
// [概要] ブックコレクションの取得
// [引数] なし
// [戻値] LPDISPATCH	// オブジェクトアドレス
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
LPDISPATCH _Application::GetWorkbooks()
{
	LPDISPATCH result;
	InvokeHelper(0x23c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetWorksheets
// [概要] シートコレクションの取得
// [引数] なし
// [戻値] LPDISPATCH	// オブジェクトアドレス
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
LPDISPATCH _Application::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x1ee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetActiveWorkbook
// [概要] アプリケーションオブジェクトを取得
// [引数] なし
// [戻値] LPDISPATCH	// オブジェクトアドレス
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
LPDISPATCH _Application::GetActiveWorkbook()
{
	LPDISPATCH result;
	InvokeHelper(0x134, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] Quit
// [概要] アプリケーション終了処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
void _Application::Quit()
{
	InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [名前] OpenBook
// [概要] ブックコレクションクオープン処理
// [引数] LPCTSTR Filename	// オープン対象ファイル名
// [引数] const VARIANT& UpdateLinks	// オープン状態
// [戻値] LPDISPATCH	// オブジェクトアドレス
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
LPDISPATCH Workbooks::OpenBook(LPCTSTR Filename, const VARIANT& UpdateLinks)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x2aa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Filename, &UpdateLinks);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] Close
// [概要] ブックコレクションクローズ処理
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
void Workbooks::Close()
{
	InvokeHelper(0x115, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [名前] GetCount
// [概要] ブック個数の取得
// [引数] なし
// [戻値] long	// ブック個数
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
long Workbooks::GetCount()
{
	long result;
	InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetWorksheets
// [概要] シートコレクションの取得
// [引数] なし
// [戻値] LPDISPATCH	// オブジェクトアドレス
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
LPDISPATCH _Workbook::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x1ee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetPath
// [概要] ブックパス名の取得
// [引数] なし
// [戻値] CString	// ブックパス名
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
CString _Workbook::GetPath()
{
	CString result;
	InvokeHelper(0x123, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetFullName
// [概要] フルブック名の取得
// [引数] なし
// [戻値] CString	// フルブック名
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
CString _Workbook::GetFullName()
{
	CString result;
	InvokeHelper(0x121, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetName
// [概要] ブック名の取得
// [引数] なし
// [戻値] CString	// ブック名
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
CString _Workbook::GetName()
{
	CString result;
	InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetItem
// [概要] シートコレクションから対応するシートを取得する
// [引数] const VARIANT& Index	// シート番号
// [戻値] LPDISPATCH	// オブジェクトアドレス
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
LPDISPATCH Worksheets::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] Select
// [概要] シートを選択状態にする
// [引数] なし
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
void _Worksheet::Select() // added
{
	InvokeHelper(0xeb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [名前] GetName
// [概要] シート名の取得
// [引数] なし
// [戻値] CString	// シート名
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
CString _Worksheet::GetName()
{
	CString result;
	InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetRange
// [概要] 所得する領域
// [引数] const VARIANT& Cell1	// 取得するセル文字列
// [戻値] LPDISPATCH	// オブジェクトアドレス
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
LPDISPATCH _Worksheet::GetRange(const VARIANT& Cell1) // added
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, 
		parms,&Cell1);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] GetValue
// [概要] 領域に値を取得する
// [引数] なし
// [戻値] VARIANT	// 取得した値
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
VARIANT Range::GetValue()
{
	VARIANT result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [名前] SetValue
// [概要] 領域に値をセットする
// [引数] const VARIANT& newValue	// セットする値
// [戻値] なし
// [更新日 / 更新者 / 更新内容]
// 2007/11/01 STAN コメント作成
// --------------------------------------------------------------------------------
void Range::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

