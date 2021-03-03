// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "..\mncmn\ltdlgctl.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "..\mncmn\font.h"

/////////////////////////////////////////////////////////////////////////////
// CLTDlgOCXCtl

IMPLEMENT_DYNCREATE(CLTDlgOCXCtl, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CLTDlgOCXCtl プロパティ

/////////////////////////////////////////////////////////////////////////////
// CLTDlgOCXCtl オペレーション

unsigned long CLTDlgOCXCtl::AddRef()
{
	unsigned long result;
	InvokeHelper(0x60000001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

unsigned long CLTDlgOCXCtl::Release()
{
	unsigned long result;
	InvokeHelper(0x60000002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetFont(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

COleFont CLTDlgOCXCtl::GetFont()
{
	LPDISPATCH pDispatch;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return COleFont(pDispatch);
}

long CLTDlgOCXCtl::GetAngle()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetAngle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetAngle(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

BOOL CLTDlgOCXCtl::GetAngleFlag()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetAngleFlag(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CLTDlgOCXCtl::GetUIFlags()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetUIFlags(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetShowHelpButton()
{
	BOOL result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShowHelpButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CLTDlgOCXCtl::GetNewWidth()
{
	short result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetNewWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetNewHeight()
{
	short result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetNewHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetSize(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

long CLTDlgOCXCtl::GetBitmap()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetBitmap(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetBitsPerPixel()
{
	short result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetBitsPerPixel(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetPaletteFlags()
{
	short result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetPaletteFlags(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetDitherFlags()
{
	short result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetDitherFlags(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetColorRes(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

long CLTDlgOCXCtl::GetFileDlgFlags()
{
	long result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetFileDlgFlags(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CLTDlgOCXCtl::GetDialogTitle()
{
	CString result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetDialogTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CLTDlgOCXCtl::GetFileName()
{
	CString result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetFileName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CLTDlgOCXCtl::GetFileTitle()
{
	CString result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetFileTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CLTDlgOCXCtl::GetFilter()
{
	CString result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long CLTDlgOCXCtl::GetFilterIndex()
{
	long result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetFilterIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CLTDlgOCXCtl::GetInitialDir()
{
	CString result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetInitialDir(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long CLTDlgOCXCtl::GetMaxFileSize()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetMaxFileSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CLTDlgOCXCtl::GetPageNumber()
{
	long result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetPageNumber(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CLTDlgOCXCtl::GetLoadPasses()
{
	long result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetLoadPasses(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetLoadCompressed()
{
	BOOL result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetLoadCompressed(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CLTDlgOCXCtl::GetLoadRotated()
{
	BOOL result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetLoadRotated(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CLTDlgOCXCtl::FileOpen(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

long CLTDlgOCXCtl::GetMaxFileTitleSize()
{
	long result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetMaxFileTitleSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CLTDlgOCXCtl::GetThumbnail()
{
	long result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetThumbnail(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetSaveBitsPerPixel()
{
	short result;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveBitsPerPixel(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetSaveFormat()
{
	short result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveFormat(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetSaveQFactor()
{
	short result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveQFactor(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetSaveMulti()
{
	short result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveMulti(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetSaveProgressive()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveProgressive(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CLTDlgOCXCtl::GetSavePasses()
{
	short result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSavePasses(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetSaveWithStamp()
{
	BOOL result;
	InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveWithStamp(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CLTDlgOCXCtl::GetSaveStampBits()
{
	short result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveStampBits(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetSaveStampWidth()
{
	short result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveStampWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetSaveStampHeight()
{
	short result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveStampHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetSaveInterlaced()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveInterlaced(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CLTDlgOCXCtl::FileSave(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

long CLTDlgOCXCtl::GetSaveFormatFlags()
{
	long result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSaveFormatFlags(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetDirection()
{
	short result;
	InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetDirection(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetDepth()
{
	short result;
	InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetDepth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetEmboss(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetRange()
{
	short result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetRange(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetChannel()
{
	short result;
	InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetChannel(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetNoise(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x31, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetGamma()
{
	short result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetGamma(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetGamma(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x33, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetLow()
{
	short result;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetLow(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x34, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetHigh()
{
	short result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetHigh(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x35, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetRange(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetChange()
{
	short result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetChange(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CLTDlgOCXCtl::GetProcess()
{
	long result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetProcess(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x38, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetChange(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetSpatialFilterType()
{
	short result;
	InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSpatialFilterType(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetBinaryFilterType()
{
	short result;
	InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetBinaryFilterType(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetFilter(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetFilterType()
{
	short result;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetFilterType(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetPreviewEnabled()
{
	BOOL result;
	InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetPreviewEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CLTDlgOCXCtl::GetEnableMethodErrors()
{
	BOOL result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetEnableMethodErrors(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CLTDlgOCXCtl::GetLowBit()
{
	short result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetLowBit(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetHighBit()
{
	short result;
	InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetHighBit(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CLTDlgOCXCtl::GetLowLevel()
{
	long result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetLowLevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CLTDlgOCXCtl::GetHighLevel()
{
	long result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetHighLevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CLTDlgOCXCtl::GetStartColor()
{
	unsigned long result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetStartColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x44, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long CLTDlgOCXCtl::GetEndColor()
{
	unsigned long result;
	InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetEndColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

short CLTDlgOCXCtl::GetWindowLevel(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x46, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetEffect()
{
	short result;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetEffect(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetGrain()
{
	short result;
	InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetGrain(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetDelay()
{
	short result;
	InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetDelay(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetMaxPass()
{
	short result;
	InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetMaxPass(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetTransparent()
{
	BOOL result;
	InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTransparent(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long CLTDlgOCXCtl::GetTransparentColor()
{
	unsigned long result;
	InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTransparentColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

short CLTDlgOCXCtl::GetWandWidth()
{
	short result;
	InvokeHelper(0x4d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetWandWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x4d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CLTDlgOCXCtl::GetWandColor()
{
	unsigned long result;
	InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetWandColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

short CLTDlgOCXCtl::GetEffect(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4f, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetGradientStyle()
{
	short result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetGradientStyle(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x50, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetGradientSteps()
{
	short result;
	InvokeHelper(0x51, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetGradientSteps(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x51, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetGradient(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetTransition()
{
	short result;
	InvokeHelper(0x53, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTransition(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x53, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetTransition(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

short CLTDlgOCXCtl::GetShape()
{
	short result;
	InvokeHelper(0x55, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShape(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x55, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetShapeBackStyle()
{
	short result;
	InvokeHelper(0x56, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeBackStyle(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x56, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetShapeFillStyle()
{
	short result;
	InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeFillStyle(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x57, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CLTDlgOCXCtl::GetShapeBorderColor()
{
	unsigned long result;
	InvokeHelper(0x58, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeBorderColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x58, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

short CLTDlgOCXCtl::GetShapeBorderStyle()
{
	short result;
	InvokeHelper(0x59, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeBorderStyle(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x59, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetShapeBorderWidth()
{
	short result;
	InvokeHelper(0x5a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeBorderWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetShapeInnerStyle()
{
	short result;
	InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeInnerStyle(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetShapeInnerWidth()
{
	short result;
	InvokeHelper(0x5c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeInnerWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CLTDlgOCXCtl::GetShapeInnerHiliteColor()
{
	unsigned long result;
	InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeInnerHiliteColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long CLTDlgOCXCtl::GetShapeInnerShadowColor()
{
	unsigned long result;
	InvokeHelper(0x5e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeInnerShadowColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

short CLTDlgOCXCtl::GetShapeOuterStyle()
{
	short result;
	InvokeHelper(0x5f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeOuterStyle(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetShapeOuterWidth()
{
	short result;
	InvokeHelper(0x60, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeOuterWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CLTDlgOCXCtl::GetShapeOuterHiliteColor()
{
	unsigned long result;
	InvokeHelper(0x61, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeOuterHiliteColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x61, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long CLTDlgOCXCtl::GetShapeOuterShadowColor()
{
	unsigned long result;
	InvokeHelper(0x62, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeOuterShadowColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x62, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

short CLTDlgOCXCtl::GetShadowXDepth()
{
	short result;
	InvokeHelper(0x63, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShadowXDepth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetShadowYDepth()
{
	short result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShadowYDepth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CLTDlgOCXCtl::GetShadowColor()
{
	unsigned long result;
	InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShadowColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

short CLTDlgOCXCtl::Get3dShape(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

long CLTDlgOCXCtl::GetShapeBackgroundBitmap()
{
	long result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetShapeBackgroundBitmap(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CLTDlgOCXCtl::GetSampleText()
{
	CString result;
	InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetSampleText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

short CLTDlgOCXCtl::GetTextStyle()
{
	short result;
	InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTextStyle(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CLTDlgOCXCtl::GetTextAlign()
{
	short result;
	InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTextAlign(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CLTDlgOCXCtl::GetTextWordWrap()
{
	BOOL result;
	InvokeHelper(0x6b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTextWordWrap(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long CLTDlgOCXCtl::GetTextColor()
{
	unsigned long result;
	InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTextColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long CLTDlgOCXCtl::GetTextHiliteColor()
{
	unsigned long result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTextHiliteColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CLTDlgOCXCtl::GetTextUseForegroundImage()
{
	BOOL result;
	InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTextUseForegroundImage(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CLTDlgOCXCtl::Get3dText(long hWnd)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		hWnd);
	return result;
}

long CLTDlgOCXCtl::GetTextForegroundBitmap()
{
	long result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetTextForegroundBitmap(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x70, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CLTDlgOCXCtl::GetForeColor()
{
	unsigned long result;
	InvokeHelper(0x71, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x71, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long CLTDlgOCXCtl::GetBackColor()
{
	unsigned long result;
	InvokeHelper(0x72, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CLTDlgOCXCtl::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x72, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void CLTDlgOCXCtl::UnlockSupport(short iType, LPCTSTR pszKey)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 iType, pszKey);
}

long CLTDlgOCXCtl::IsSupportLocked(short iType)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x74, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iType);
	return result;
}
