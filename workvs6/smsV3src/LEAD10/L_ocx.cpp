// CLead control implementation
// Note:    プリコンパイルヘッダで問題が生じた場合、このファイルのプリ
//          コンパイルヘッダをコメントアウトするか、プロジェクト全体で
//          無効となるように設定してください。
#define IS_MFC40    (_MSC_VER >= 1000)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>

#include <stdarg.h>
#include <afxpriv.h>
#include <initguid.h>
#define INITGUID
#include <olectlid.h>

#include "l_ocx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLead, CWnd)
IMPLEMENT_DYNAMIC(CLeadContainer, CCmdTarget)

const CLSID CDECL BASED_CODE CLead::guid = \
   { 0x00100000, 0xb1ba, 0x11ce, { 0xab, 0xc6, 0xf5, 0xb2, 0xe7, 0x9d, 0x9e, 0x3f } } ;


#if IS_MFC40
static const WCHAR BASED_CODE _szLicString[] =
    L"LEADTOOLS OCX Copyright (c) 1998 LEAD Technologies, Inc.";

static const FONTDESC NEAR _fdDefault =
   { sizeof(FONTDESC), L"MS Sans Serif", FONTSIZE(12), FW_NORMAL,
     DEFAULT_CHARSET, FALSE, FALSE, FALSE };

#else
/*
    This is for 16 or 32-bit code compiled with Visual C++ 1.x or 2.0
*/
static const TCHAR BASED_CODE _szLicString[] =
   _T("LEADTOOLS OCX Copyright (c) 1998 LEAD Technologies, Inc.");

static const FONTDESC NEAR _fdDefault =
   { sizeof(FONTDESC), _T("MS Sans Serif"), FONTSIZE(12), FW_NORMAL,
     DEFAULT_CHARSET, FALSE, FALSE, FALSE };
#endif


BEGIN_MESSAGE_MAP(CLead, CWnd)
   //{{AFX_MSG_MAP(CLead)
   ON_WM_PAINT()
   ON_WM_DESTROY()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// static routines and defines
static long LongMulDiv(long numer, long denom);
static int XformWidthInPixelsToHimetric(HDC hDC, int iWidthInPix);
static int XformWidthInHimetricToPixels(HDC hDC, int iWidthInHiMetric);
static int XformHeightInPixelsToHimetric(HDC hDC, int iHeightInPix);
static int XformHeightInHimetricToPixels(HDC hDC, int iHeightInHiMetric);
static void XformSizeInPixelsToHimetric(
   HDC hDC, LPSIZEL lpSizeInPix, LPSIZEL lpSizeInHiMetric);
static void XformSizeInHimetricToPixels(
   HDC hDC, LPSIZEL lpSizeInHiMetric, LPSIZEL lpSizeInPix);
static void XformRectInPixelsToHimetric(
   HDC hDC, LPRECT lprcPix, LPRECT lprcHiMetric);
static int SetDCToDrawInHimetricRect(HDC hDC, LPRECT lprcPix,
   LPRECT lprcHiMetric, LPRECT lprcWindowOld, LPRECT lprcViewportOld);
#define HIMETRIC_PER_INCH   2540
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)

#undef RELEASE
#define RELEASE(p)  if ((p) != NULL) { (p)->Release(); (p) = NULL; };

static HRESULT InterfaceConnect(LPUNKNOWN pObj, REFIID riid, LPUNKNOWN pIUnknownSink, LPDWORD pdwConn);
static HRESULT InterfaceDisconnect(LPUNKNOWN pObj, REFIID riid, LPDWORD pdwConn);
static HRESULT ObjectTypeInfo(LPUNKNOWN pObj, LPTYPEINFO *ppITypeInfo);
static HRESULT ObjectTypeInfoEvents(LPUNKNOWN pObj, LPTYPEINFO *ppITypeInfo);
static HRESULT ObjectEventsIID(LPUNKNOWN pObj, IID *piid);

CLead::CLead()
{

   m_pIObject = NULL;
   m_pIOleInPlaceObject = NULL;
   m_pIOleInPlaceActiveObject = NULL;
   m_pIPersistStreamInit = NULL;
   m_pIViewObject2 = NULL;
   m_pEventsRedirDispatch = NULL;
   m_pIDispatchControl = NULL;
   m_pIOleControl = NULL;
   m_pAmbientsDispatch = NULL;
   m_pLeadContainer = NULL;
   
   m_hWndParent = NULL;

   m_dwStatus = 0;   
   m_dwConnection = 0;
   m_dwEventConnProp = 0;
   m_scLast = S_OK;
   m_nState = emptyState;
   m_bInClose = FALSE;
   m_uLockInPlace = 0;
   m_cntrlInfo.cb = sizeof(CONTROLINFO);
   m_cntrlInfo.hAccel = NULL;
   m_bDisplayAsDefault = FALSE;
   m_bPendingDeactivate = FALSE;

   AfxOleLockApp();
}

CLead::~CLead()
{

   Release();

   ExternalDisconnect();
   AfxOleUnlockApp();
}

VARIANT CLead::ReadComment(LPCTSTR pszFile, short iPage, short iIndex)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_BSTR VTS_I2 VTS_I2;
   InvokeHelper(dispidReadComment, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      pszFile, iPage, iIndex);
   return result;
}

REFCLSID CLead::GetClassID() const
{
   return guid;
}

short CLead::GetLoadInfoBits()
{
   short result;
   GetProperty(dispidLoadInfoBits, VT_I2, (void*)&result);
   return result;
}

void CLead::SetLoadInfoBits(short propval)
{
   SetProperty(dispidLoadInfoBits, VT_I2, propval);
}

float CLead::GetLoadInfoWidth()
{
   float result;
   GetProperty(dispidLoadInfoWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetLoadInfoWidth(float propval)
{
   SetProperty(dispidLoadInfoWidth, VT_R4, propval);
}

float CLead::GetLoadInfoHeight()
{
   float result;
   GetProperty(dispidLoadInfoHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetLoadInfoHeight(float propval)
{
   SetProperty(dispidLoadInfoHeight, VT_R4, propval);
}

short CLead::GetLoadInfoFormat()
{
   short result;
   GetProperty(dispidLoadInfoFormat, VT_I2, (void*)&result);
   return result;
}

void CLead::SetLoadInfoFormat(short propval)
{
   SetProperty(dispidLoadInfoFormat, VT_I2, propval);
}

short CLead::GetLoadInfoXRes()
{
   short result;
   GetProperty(dispidLoadInfoXRes, VT_I2, (void*)&result);
   return result;
}

void CLead::SetLoadInfoXRes(short propval)
{
   SetProperty(dispidLoadInfoXRes, VT_I2, propval);
}

short CLead::GetLoadInfoYRes()
{
   short result;
   GetProperty(dispidLoadInfoYRes, VT_I2, (void*)&result);
   return result;
}

void CLead::SetLoadInfoYRes(short propval)
{
   SetProperty(dispidLoadInfoYRes, VT_I2, propval);
}

long CLead::GetLoadInfoFlags()
{
   long result;
   GetProperty(dispidLoadInfoFlags, VT_I4, (void*)&result);
   return result;
}

void CLead::SetLoadInfoFlags(long propval)
{
   SetProperty(dispidLoadInfoFlags, VT_I4, propval);
}

long CLead::GetLoadInfoOffset()
{
   long result;
   GetProperty(dispidLoadInfoOffset, VT_I4, (void*)&result);
   return result;
}

void CLead::SetLoadInfoOffset(long propval)
{
   SetProperty(dispidLoadInfoOffset, VT_I4, propval);
}

long CLead::GetReadyState()
{
   long result;
   GetProperty(dispidReadyState, VT_I4, (void*)&result);
   return result;
}   
void CLead::SetReadyState(long propval)
{
   SetProperty(dispidReadyState, VT_I4, propval);
}

short CLead::GetAppearance()
{
   short result;
   GetProperty(dispidAppearance, VT_I2, (void*)&result);
   return result;
}

void CLead::SetAppearance(short propval)
{
   SetProperty(dispidAppearance, VT_I2, propval);
}

COleFont CLead::GetFont()
{
   LPDISPATCH pDispatch;
   GetProperty(dispidFont, VT_DISPATCH, (void*)&pDispatch);
   return COleFont(pDispatch);
}

BOOL CLead::GetForePalette()
{
   BOOL result;
   GetProperty(dispidForePalette, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetForePalette(BOOL propval)
{
   SetProperty(dispidForePalette, VT_BOOL, propval);
}

OLE_HANDLE CLead::GetHWnd()
{
   OLE_HANDLE result;
   GetProperty(0xFFFFFDFD, VT_HANDLE, (void*)&result);
   return result;
}

void CLead::SetHWnd(OLE_HANDLE propval)
{
   SetProperty(0xFFFFFDFD, VT_HANDLE, propval);
}

OLE_COLOR CLead::GetBackColor()
{
   OLE_COLOR result;
   GetProperty(0xFFFFFE0B, VT_I4, (void*)&result);
   return result;
}

void CLead::SetBackColor(OLE_COLOR propval)
{
   SetProperty(0xFFFFFE0B, VT_I4, propval);
}

BOOL CLead::GetEnabled()
{
   BOOL result;
   GetProperty(0xFFFFFDFE, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetEnabled(BOOL propval)
{
   SetProperty(0xFFFFFDFE, VT_BOOL, propval);
}

short CLead::GetIsGrayscale()
{
   short result;
   GetProperty(dispidIsGrayscale, VT_I2, (void*)&result);
   return result;
}

void CLead::SetIsGrayscale(short propval)
{
   SetProperty(dispidIsGrayscale, VT_I2, propval);
}

short CLead::GetBorderStyle()
{
   short result;
   GetProperty(0xFFFFFE08, VT_I2, (void*)&result);
   return result;
}

void CLead::SetBorderStyle(short propval)
{
   SetProperty(0xFFFFFE08, VT_I2, propval);
}

OLE_HANDLE CLead::GetBitmap()
{
   OLE_HANDLE result;
   GetProperty(dispidBitmap, VT_HANDLE, (void*)&result);
   return result;
}

void CLead::SetBitmap(OLE_HANDLE propval)
{
   SetProperty(dispidBitmap, VT_HANDLE, propval);
}

BOOL CLead::GetAutoRepaint()
{
   BOOL result;
   GetProperty(dispidAutoRepaint, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAutoRepaint(BOOL propval)
{
   SetProperty(dispidAutoRepaint, VT_BOOL, propval);
}

short CLead::GetPaintEffect()
{
   short result;
   GetProperty(dispidPaintEffect, VT_I2, (void*)&result);
   return result;
}

void CLead::SetPaintEffect(short propval)
{
   SetProperty(dispidPaintEffect, VT_I2, propval);
}

BOOL CLead::GetBackErase()
{
   BOOL result;
   GetProperty(dispidBackErase, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetBackErase(BOOL propval)
{
   SetProperty(dispidBackErase, VT_BOOL, propval);
}

BOOL CLead::GetAutoScroll()
{
   BOOL result;
   GetProperty(dispidAutoScroll, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAutoScroll(BOOL propval)
{
   SetProperty(dispidAutoScroll, VT_BOOL, propval);
}

short CLead::GetPaintDither()
{
   short result;
   GetProperty(dispidPaintDither, VT_I2, (void*)&result);
   return result;
}

void CLead::SetPaintDither(short propval)
{
   SetProperty(dispidPaintDither, VT_I2, propval);
}

BOOL CLead::GetEnableProgressEvent()
{
   BOOL result;
   GetProperty(dispidEnableProgressEvent, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetEnableProgressEvent(BOOL propval)
{
   SetProperty(dispidEnableProgressEvent, VT_BOOL, propval);
}

short CLead::GetBitonalScaling()
{
   short result;
   GetProperty(dispidBitonalScaling, VT_I2, (void*)&result);
   return result;
}

void CLead::SetBitonalScaling(short propval)
{
   SetProperty(dispidBitonalScaling, VT_I2, propval);
}

BOOL CLead::GetPaintWhileLoad()
{
   BOOL result;
   GetProperty(dispidPaintWhileLoad, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetPaintWhileLoad(BOOL propval)
{
   SetProperty(dispidPaintWhileLoad, VT_BOOL, propval);
}

float CLead::GetBitmapWidth()
{
   float result;
   GetProperty(dispidBitmapWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetBitmapWidth(float propval)
{
   SetProperty(dispidBitmapWidth, VT_R4, propval);
}

float CLead::GetBitmapHeight()
{
   float result;
   GetProperty(dispidBitmapHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetBitmapHeight(float propval)
{
   SetProperty(dispidBitmapHeight, VT_R4, propval);
}

short CLead::GetBitmapBits()
{
   short result;
   GetProperty(dispidBitmapBits, VT_I2, (void*)&result);
   return result;
}

void CLead::SetBitmapBits(short propval)
{
   SetProperty(dispidBitmapBits, VT_I2, propval);
}

short CLead::GetVersionLevel()
{
   short result;
   GetProperty(dispidVersionLevel, VT_I2, (void*)&result);
   return result;
}

void CLead::SetVersionLevel(short propval)
{
   SetProperty(dispidVersionLevel, VT_I2, propval);
}

short CLead::GetVersionMajor()
{
   short result;
   GetProperty(dispidVersionMajor, VT_I2, (void*)&result);
   return result;
}

void CLead::SetVersionMajor(short propval)
{
   SetProperty(dispidVersionMajor, VT_I2, propval);
}

short CLead::GetVersionMinor()
{
   short result;
   GetProperty(dispidVersionMinor, VT_I2, (void*)&result);
   return result;
}

void CLead::SetVersionMinor(short propval)
{
   SetProperty(dispidVersionMinor, VT_I2, propval);
}

CString CLead::GetVersionProduct()
{
   CString result;
   GetProperty(dispidVersionProduct, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetVersionProduct(LPCTSTR propval)
{
   SetProperty(dispidVersionProduct, VT_BSTR, propval);
}

CString CLead::GetVersionDate()
{
   CString result;
   GetProperty(dispidVersionDate, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetVersionDate(LPCTSTR propval)
{
   SetProperty(dispidVersionDate, VT_BSTR, propval);
}

CString CLead::GetVersionTime()
{
   CString result;
   GetProperty(dispidVersionTime, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetVersionTime(LPCTSTR propval)
{
   SetProperty(dispidVersionTime, VT_BSTR, propval);
}

short CLead::GetPaintPalette()
{
   short result;
   GetProperty(dispidPaintPalette, VT_I2, (void*)&result);
   return result;
}

void CLead::SetPaintPalette(short propval)
{
   SetProperty(dispidPaintPalette, VT_I2, propval);
}

BOOL CLead::GetEnableMethodErrors()
{
   BOOL result;
   GetProperty(dispidEnableMethodErrors, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetEnableMethodErrors(BOOL propval)
{
   SetProperty(dispidEnableMethodErrors, VT_BOOL, propval);
}

float CLead::GetDstLeft()
{
   float result;
   GetProperty(dispidDstLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstLeft(float propval)
{
   SetProperty(dispidDstLeft, VT_R4, propval);
}

float CLead::GetDstTop()
{
   float result;
   GetProperty(dispidDstTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstTop(float propval)
{
   SetProperty(dispidDstTop, VT_R4, propval);
}

float CLead::GetDstWidth()
{
   float result;
   GetProperty(dispidDstWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstWidth(float propval)
{
   SetProperty(dispidDstWidth, VT_R4, propval);
}

float CLead::GetDstHeight()
{
   float result;
   GetProperty(dispidDstHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstHeight(float propval)
{
   SetProperty(dispidDstHeight, VT_R4, propval);
}

float CLead::GetDstClipLeft()
{
   float result;
   GetProperty(dispidDstClipLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstClipLeft(float propval)
{
   SetProperty(dispidDstClipLeft, VT_R4, propval);
}

float CLead::GetDstClipTop()
{
   float result;
   GetProperty(dispidDstClipTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstClipTop(float propval)
{
   SetProperty(dispidDstClipTop, VT_R4, propval);
}

float CLead::GetDstClipWidth()
{
   float result;
   GetProperty(dispidDstClipWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstClipWidth(float propval)
{
   SetProperty(dispidDstClipWidth, VT_R4, propval);
}

float CLead::GetDstClipHeight()
{
   float result;
   GetProperty(dispidDstClipHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetDstClipHeight(float propval)
{
   SetProperty(dispidDstClipHeight, VT_R4, propval);
}

float CLead::GetSrcLeft()
{
   float result;
   GetProperty(dispidSrcLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcLeft(float propval)
{
   SetProperty(dispidSrcLeft, VT_R4, propval);
}

float CLead::GetSrcTop()
{
   float result;
   GetProperty(dispidSrcTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcTop(float propval)
{
   SetProperty(dispidSrcTop, VT_R4, propval);
}

float CLead::GetSrcWidth()
{
   float result;
   GetProperty(dispidSrcWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcWidth(float propval)
{
   SetProperty(dispidSrcWidth, VT_R4, propval);
}

float CLead::GetSrcHeight()
{
   float result;
   GetProperty(dispidSrcHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcHeight(float propval)
{
   SetProperty(dispidSrcHeight, VT_R4, propval);
}

float CLead::GetSrcClipLeft()
{
   float result;
   GetProperty(dispidSrcClipLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcClipLeft(float propval)
{
   SetProperty(dispidSrcClipLeft, VT_R4, propval);
}

float CLead::GetSrcClipTop()
{
   float result;
   GetProperty(dispidSrcClipTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcClipTop(float propval)
{
   SetProperty(dispidSrcClipTop, VT_R4, propval);
}

float CLead::GetSrcClipWidth()
{
   float result;
   GetProperty(dispidSrcClipWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcClipWidth(float propval)
{
   SetProperty(dispidSrcClipWidth, VT_R4, propval);
}

float CLead::GetSrcClipHeight()
{
   float result;
   GetProperty(dispidSrcClipHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetSrcClipHeight(float propval)
{
   SetProperty(dispidSrcClipHeight, VT_R4, propval);
}

short CLead::GetBitmapXRes()
{
   short result;
   GetProperty(dispidBitmapXRes, VT_I2, (void*)&result);
   return result;
}

void CLead::SetBitmapXRes(short propval)
{
   SetProperty(dispidBitmapXRes, VT_I2, propval);
}

short CLead::GetBitmapYRes()
{
   short result;
   GetProperty(dispidBitmapYRes, VT_I2, (void*)&result);
   return result;
}

void CLead::SetBitmapYRes(short propval)
{
   SetProperty(dispidBitmapYRes, VT_I2, propval);
}

short CLead::GetInfoPage()
{
   short result;
   GetProperty(dispidInfoPage, VT_I2, (void*)&result);
   return result;
}

void CLead::SetInfoPage(short propval)
{
   SetProperty(dispidInfoPage, VT_I2, propval);
}

float CLead::GetInfoWidth()
{
   float result;
   GetProperty(dispidInfoWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetInfoWidth(float propval)
{
   SetProperty(dispidInfoWidth, VT_R4, propval);
}

float CLead::GetInfoHeight()
{
   float result;
   GetProperty(dispidInfoHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetInfoHeight(float propval)
{
   SetProperty(dispidInfoHeight, VT_R4, propval);
}

short CLead::GetInfoBits()
{
   short result;
   GetProperty(dispidInfoBits, VT_I2, (void*)&result);
   return result;
}

void CLead::SetInfoBits(short propval)
{
   SetProperty(dispidInfoBits, VT_I2, propval);
}

short CLead::GetInfoFormat()
{
   short result;
   GetProperty(dispidInfoFormat, VT_I2, (void*)&result);
   return result;
}

void CLead::SetInfoFormat(short propval)
{
   SetProperty(dispidInfoFormat, VT_I2, propval);
}

long CLead::GetInfoSizeDisk()
{
   long result;
   GetProperty(dispidInfoSizeDisk, VT_I4, (void*)&result);
   return result;
}

void CLead::SetInfoSizeDisk(long propval)
{
   SetProperty(dispidInfoSizeDisk, VT_I4, propval);
}

long CLead::GetPaintROP3()
{
   long result;
   GetProperty(dispidPaintROP3, VT_I4, (void*)&result);
   return result;
}

void CLead::SetPaintROP3(long propval)
{
   SetProperty(dispidPaintROP3, VT_I4, propval);
}

long CLead::GetInfoSizeMem()
{
   long result;
   GetProperty(dispidInfoSizeMem, VT_I4, (void*)&result);
   return result;
}

void CLead::SetInfoSizeMem(long propval)
{
   SetProperty(dispidInfoSizeMem, VT_I4, propval);
}

CString CLead::GetInfoCompress()
{
   CString result;
   GetProperty(dispidInfoCompress, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetInfoCompress(LPCTSTR propval)
{
   SetProperty(dispidInfoCompress, VT_BSTR, propval);
}

BOOL CLead::GetEnableTwainEvent()
{
   BOOL result;
   GetProperty(dispidEnableTwainEvent, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetEnableTwainEvent(BOOL propval)
{
   SetProperty(dispidEnableTwainEvent, VT_BOOL, propval);
}

BOOL CLead::GetRubberBandVisible()
{
   BOOL result;
   GetProperty(dispidRubberBandVisible, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetRubberBandVisible(BOOL propval)
{
   SetProperty(dispidRubberBandVisible, VT_BOOL, propval);
}

float CLead::GetRubberBandLeft()
{
   float result;
   GetProperty(dispidRubberBandLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRubberBandLeft(float propval)
{
   SetProperty(dispidRubberBandLeft, VT_R4, propval);
}

float CLead::GetRubberBandTop()
{
   float result;
   GetProperty(dispidRubberBandTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRubberBandTop(float propval)
{
   SetProperty(dispidRubberBandTop, VT_R4, propval);
}

float CLead::GetRubberBandWidth()
{
   float result;
   GetProperty(dispidRubberBandWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRubberBandWidth(float propval)
{
   SetProperty(dispidRubberBandWidth, VT_R4, propval);
}

float CLead::GetRubberBandHeight()
{
   float result;
   GetProperty(dispidRubberBandHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRubberBandHeight(float propval)
{
   SetProperty(dispidRubberBandHeight, VT_R4, propval);
}

short CLead::GetPCDRes()
{
   short result;
   GetProperty(dispidPCDRes, VT_I2, (void*)&result);
   return result;
}

void CLead::SetPCDRes(short propval)
{
   SetProperty(dispidPCDRes, VT_I2, propval);
}

short CLead::GetMousePointer()
{
   short result;
   GetProperty(dispidMousePointer, VT_I2, (void*)&result);
   return result;
}

void CLead::SetMousePointer(short propval)
{
   SetProperty(dispidMousePointer, VT_I2, propval);
}

LPUNKNOWN CLead::GetMouseIcon()
{
   LPUNKNOWN result;
   GetProperty(dispidMouseIcon, VT_UNKNOWN, (void*)&result);
   return result;
}

void CLead::SetMouseIcon(LPUNKNOWN propval)
{
   SetProperty(dispidMouseIcon, VT_UNKNOWN, propval);
}

short CLead::GetScaleMode()
{
   short result;
   GetProperty(dispidScaleMode, VT_I2, (void*)&result);
   return result;
}

void CLead::SetScaleMode(short propval)
{
   SetProperty(dispidScaleMode, VT_I2, propval);
}

BOOL CLead::GetAutoSetRects()
{
   BOOL result;
   GetProperty(dispidAutoSetRects, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAutoSetRects(BOOL propval)
{
   SetProperty(dispidAutoSetRects, VT_BOOL, propval);
}

float CLead::GetScaleLeft()
{
   float result;
   GetProperty(dispidScaleLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetScaleLeft(float propval)
{
   SetProperty(dispidScaleLeft, VT_R4, propval);
}

float CLead::GetScaleTop()
{
   float result;
   GetProperty(dispidScaleTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetScaleTop(float propval)
{
   SetProperty(dispidScaleTop, VT_R4, propval);
}

float CLead::GetScaleWidth()
{
   float result;
   GetProperty(dispidScaleWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetScaleWidth(float propval)
{
   SetProperty(dispidScaleWidth, VT_R4, propval);
}

float CLead::GetScaleHeight()
{
   float result;
   GetProperty(dispidScaleHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetScaleHeight(float propval)
{
   SetProperty(dispidScaleHeight, VT_R4, propval);
}

BOOL CLead::GetdbCanAppend()
{
   BOOL result;
   GetProperty(dispidDbCanAppend, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbCanAppend(BOOL propval)
{
   SetProperty(dispidDbCanAppend, VT_BOOL, propval);
}

BOOL CLead::GetdbCanRestart()
{
   BOOL result;
   GetProperty(dispidDbCanRestart, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbCanRestart(BOOL propval)
{
   SetProperty(dispidDbCanRestart, VT_BOOL, propval);
}

BOOL CLead::GetdbCanScroll()
{
   BOOL result;
   GetProperty(dispidDbCanScroll, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbCanScroll(BOOL propval)
{
   SetProperty(dispidDbCanScroll, VT_BOOL, propval);
}

BOOL CLead::GetdbCanUpdate()
{
   BOOL result;
   GetProperty(dispidDbCanUpdate, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbCanUpdate(BOOL propval)
{
   SetProperty(dispidDbCanUpdate, VT_BOOL, propval);
}

long CLead::GetdbRecordCount()
{
   long result;
   GetProperty(dispidDbRecordCount, VT_I4, (void*)&result);
   return result;
}

void CLead::SetdbRecordCount(long propval)
{
   SetProperty(dispidDbRecordCount, VT_I4, propval);
}

BOOL CLead::GetdbIsOpen()
{
   BOOL result;
   GetProperty(dispidDbIsOpen, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbIsOpen(BOOL propval)
{
   SetProperty(dispidDbIsOpen, VT_BOOL, propval);
}

BOOL CLead::GetdbIsBOF()
{
   BOOL result;
   GetProperty(dispidDbIsBOF, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbIsBOF(BOOL propval)
{
   SetProperty(dispidDbIsBOF, VT_BOOL, propval);
}

BOOL CLead::GetdbIsEOF()
{
   BOOL result;
   GetProperty(dispidDbIsEOF, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbIsEOF(BOOL propval)
{
   SetProperty(dispidDbIsEOF, VT_BOOL, propval);
}

BOOL CLead::GetdbIsDeleted()
{
   BOOL result;
   GetProperty(dispidDbIsDeleted, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetdbIsDeleted(BOOL propval)
{
   SetProperty(dispidDbIsDeleted, VT_BOOL, propval);
}

long CLead::GetdbCurrentRecord()
{
   long result;
   GetProperty(dispidDbCurrentRecord, VT_I4, (void*)&result);
   return result;
}

void CLead::SetdbCurrentRecord(long propval)
{
   SetProperty(dispidDbCurrentRecord, VT_I4, propval);
}

short CLead::GetdbEditMode()
{
   short result;
   GetProperty(dispidDbEditMode, VT_I2, (void*)&result);
   return result;
}

void CLead::SetdbEditMode(short propval)
{
   SetProperty(dispidDbEditMode, VT_I2, propval);
}

short CLead::GetdbLockingMode()
{
   short result;
   GetProperty(dispidDbLockingMode, VT_I2, (void*)&result);
   return result;
}

void CLead::SetdbLockingMode(short propval)
{
   SetProperty(dispidDbLockingMode, VT_I2, propval);
}

short CLead::GetdbLoadBits()
{
   short result;
   GetProperty(dispidDbLoadBits, VT_I2, (void*)&result);
   return result;
}

void CLead::SetdbLoadBits(short propval)
{
   SetProperty(dispidDbLoadBits, VT_I2, propval);
}

CString CLead::GetTwainAppAuthor()
{
   CString result;
   GetProperty(dispidTwainAppAuthor, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetTwainAppAuthor(LPCTSTR propval)
{
   SetProperty(dispidTwainAppAuthor, VT_BSTR, propval);
}

CString CLead::GetTwainAppFamily()
{
   CString result;
   GetProperty(dispidTwainAppFamily, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetTwainAppFamily(LPCTSTR propval)
{
   SetProperty(dispidTwainAppFamily, VT_BSTR, propval);
}

CString CLead::GetTwainAppName()
{
   CString result;
   GetProperty(dispidTwainAppName, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetTwainAppName(LPCTSTR propval)
{
   SetProperty(dispidTwainAppName, VT_BSTR, propval);
}

CString CLead::GetTwainSourceName()
{
   CString result;
   GetProperty(dispidTwainSourceName, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetTwainSourceName(LPCTSTR propval)
{
   SetProperty(dispidTwainSourceName, VT_BSTR, propval);
}

short CLead::GetTwainMaxPages()
{
   short result;
   GetProperty(dispidTwainMaxPages, VT_I2, (void*)&result);
   return result;
}

void CLead::SetTwainMaxPages(short propval)
{
   SetProperty(dispidTwainMaxPages, VT_I2, propval);
}

short CLead::GetTwainPixelType()
{
   short result;
   GetProperty(dispidTwainPixelType, VT_I2, (void*)&result);
   return result;
}

void CLead::SetTwainPixelType(short propval)
{
   SetProperty(dispidTwainPixelType, VT_I2, propval);
}

short CLead::GetTwainRes()
{
   short result;
   GetProperty(dispidTwainRes, VT_I2, (void*)&result);
   return result;
}

void CLead::SetTwainRes(short propval)
{
   SetProperty(dispidTwainRes, VT_I2, propval);
}

long CLead::GetTwainFrameLeft()
{
   long result;
   GetProperty(dispidTwainFrameLeft, VT_I4, (void*)&result);
   return result;
}

void CLead::SetTwainFrameLeft(long propval)
{
   SetProperty(dispidTwainFrameLeft, VT_I4, propval);
}

long CLead::GetTwainFrameTop()
{
   long result;
   GetProperty(dispidTwainFrameTop, VT_I4, (void*)&result);
   return result;
}

void CLead::SetTwainFrameTop(long propval)
{
   SetProperty(dispidTwainFrameTop, VT_I4, propval);
}

long CLead::GetTwainFrameWidth()
{
   long result;
   GetProperty(dispidTwainFrameWidth, VT_I4, (void*)&result);
   return result;
}

void CLead::SetTwainFrameWidth(long propval)
{
   SetProperty(dispidTwainFrameWidth, VT_I4, propval);
}

long CLead::GetTwainFrameHeight()
{
   long result;
   GetProperty(dispidTwainFrameHeight, VT_I4, (void*)&result);
   return result;
}

void CLead::SetTwainFrameHeight(long propval)
{
   SetProperty(dispidTwainFrameHeight, VT_I4, propval);
}

short CLead::GetTwainBits()
{
   short result;
   GetProperty(dispidTwainBits, VT_I2, (void*)&result);
   return result;
}

void CLead::SetTwainBits(short propval)
{
   SetProperty(dispidTwainBits, VT_I2, propval);
}

short CLead::GetTwainSourceCount()
{
   short result;
   GetProperty(dispidTwainSourceCount, VT_I2, (void*)&result);
   return result;
}

void CLead::SetTwainSourceCount(short propval)
{
   SetProperty(dispidTwainSourceCount, VT_I2, propval);
}

short CLead::GetTwainFlags()
{
   short result;
   GetProperty(dispidTwainFlags, VT_I2, (void*)&result);
   return result;
}

void CLead::SetTwainFlags(short propval)
{
   SetProperty(dispidTwainFlags, VT_I2, propval);
}

void CLead::SetHasRgn(BOOL propval)
{
   SetProperty(dispidHasRgn, VT_BOOL, propval);
}

BOOL CLead::GetHasRgn()
{
   BOOL result;
   GetProperty(dispidHasRgn, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetRgnLeft(float propval)
{
   SetProperty(dispidRgnLeft, VT_R4, propval);
}

float CLead::GetRgnLeft()
{
   float result;
   GetProperty(dispidRgnLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRgnTop(float propval)
{
   SetProperty(dispidRgnTop, VT_R4, propval);
}

float CLead::GetRgnTop()
{
   float result;
   GetProperty(dispidRgnTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRgnWidth(float propval)
{
   SetProperty(dispidRgnWidth, VT_R4, propval);
}

float CLead::GetRgnWidth()
{
   float result;
   GetProperty(dispidRgnWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRgnHeight(float propval)
{
   SetProperty(dispidRgnHeight, VT_R4, propval);
}

float CLead::GetRgnHeight()
{
   float result;
   GetProperty(dispidRgnHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetRgnMarkingMode(short propval)
{
   SetProperty(dispidRgnMarkingMode, VT_I2, propval);
}

short CLead::GetRgnMarkingMode()
{
   short result;
   GetProperty(dispidRgnMarkingMode, VT_I2, (void*)&result);
   return result;
}

void CLead::SetRgnFrameType(short propval)
{
   SetProperty(dispidRgnFrameType, VT_I2, propval);
}

short CLead::GetRgnFrameType()
{
   short result;
   GetProperty(dispidRgnFrameType, VT_I2, (void*)&result);
   return result;
}

void CLead::SetFloater(OLE_HANDLE propval)
{
   SetProperty(dispidFloater, VT_HANDLE, propval);
}

OLE_HANDLE CLead::GetFloater()
{
   OLE_HANDLE result;
   GetProperty(dispidFloater, VT_HANDLE, (void*)&result);
   return result;
}

void CLead::SetFloaterVisible(BOOL propval)
{
   SetProperty(dispidFloaterVisible, VT_BOOL, propval);
}

BOOL CLead::GetFloaterVisible()
{
   BOOL result;
   GetProperty(dispidFloaterVisible, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetFloaterDstLeft(float propval)
{
   SetProperty(dispidFloaterDstLeft, VT_R4, propval);
}

float CLead::GetFloaterDstLeft()
{
   float result;
   GetProperty(dispidFloaterDstLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterDstTop(float propval)
{
   SetProperty(dispidFloaterDstTop, VT_R4, propval);
}

float CLead::GetFloaterDstTop()
{
   float result;
   GetProperty(dispidFloaterDstTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterDstWidth(float propval)
{
   SetProperty(dispidFloaterDstWidth, VT_R4, propval);
}

float CLead::GetFloaterDstWidth()
{
   float result;
   GetProperty(dispidFloaterDstWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterDstHeight(float propval)
{
   SetProperty(dispidFloaterDstHeight, VT_R4, propval);
}

float CLead::GetFloaterDstHeight()
{
   float result;
   GetProperty(dispidFloaterDstHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterDstClipLeft(float propval)
{
   SetProperty(dispidFloaterDstClipLeft, VT_R4, propval);
}

float CLead::GetFloaterDstClipLeft()
{
   float result;
   GetProperty(dispidFloaterDstClipLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterDstClipTop(float propval)
{
   SetProperty(dispidFloaterDstClipTop, VT_R4, propval);
}

float CLead::GetFloaterDstClipTop()
{
   float result;
   GetProperty(dispidFloaterDstClipTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterDstClipWidth(float propval)
{
   SetProperty(dispidFloaterDstClipWidth, VT_R4, propval);
}

float CLead::GetFloaterDstClipWidth()
{
   float result;
   GetProperty(dispidFloaterDstClipWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterDstClipHeight(float propval)
{
   SetProperty(dispidFloaterDstClipHeight, VT_R4, propval);
}

float CLead::GetFloaterDstClipHeight()
{
   float result;
   GetProperty(dispidFloaterDstClipHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterWidth(float propval)
{
   SetProperty(dispidFloaterWidth, VT_R4, propval);
}

float CLead::GetFloaterWidth()
{
   float result;
   GetProperty(dispidFloaterWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetFloaterHeight(float propval)
{
   SetProperty(dispidFloaterHeight, VT_R4, propval);
}

float CLead::GetFloaterHeight()
{
   float result;
   GetProperty(dispidFloaterHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetPolygonSize(short propval)
{
   SetProperty(dispidPolygonSize, VT_I2, propval);
}

short CLead::GetPolygonSize()
{
   short result;
   GetProperty(dispidPolygonSize, VT_I2, (void*)&result);
   return result;
}

short CLead::GetAnnUserMode()
{
   short result;
   GetProperty(dispidAnnUserMode, VT_I2, (void*)&result);
   return result;
}

void CLead::SetAnnUserMode(short propVal)
{
   SetProperty(dispidAnnUserMode, VT_I2, propVal);
}

short CLead::GetAnnTool()
{
   short result;
   GetProperty(dispidAnnTool, VT_I2, (void*)&result);
   return result;
}

void CLead::SetAnnTool(short propVal)
{
   SetProperty(dispidAnnTool, VT_I2, propVal);
}

BOOL CLead::GetAnnPasteReady()
{
   BOOL result;
   GetProperty(dispidAnnPasteReady, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAnnPasteReady(BOOL propVal)
{
   SetProperty(dispidAnnPasteReady, VT_BOOL, propVal);
}

OLE_HANDLE CLead::GetAnnContainer()
{
   OLE_HANDLE result;
   GetProperty(dispidAnnContainer, VT_HANDLE, (void*)&result);
   return result;
}

void CLead::SetAnnContainer(OLE_HANDLE propVal)
{
   SetProperty(dispidAnnContainer, VT_HANDLE, propVal);
}

OLE_HANDLE CLead::GetAnnAutomation()
{
   OLE_HANDLE result;
   GetProperty(dispidAnnAutomation, VT_HANDLE, (void*)&result);
   return result;
}

short CLead::GetProgressivePasses()
{
   short result;
   GetProperty(dispidProgressivePasses, VT_I2, (void*)&result);
   return result;
}

void CLead::SetProgressivePasses(short propval)
{
   SetProperty(dispidProgressivePasses, VT_I2, propval);
}

short CLead::GetBitmapListIndex()
{
   short result;
   GetProperty(dispidBitmapListIndex, VT_I2, (void*)&result);
   return result;
}

void CLead::SetBitmapListIndex(short propval)
{
   SetProperty(dispidBitmapListIndex, VT_I2, propval);
}

short CLead::GetBitmapListCount()
{
   short result;
   GetProperty(dispidBitmapListCount, VT_I2, (void*)&result);
   return result;
}

BOOL CLead::GetAnimationEnable()
{
   BOOL result;
   GetProperty(dispidAnimationEnable, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAnimationEnable(BOOL propval)
{
   SetProperty(dispidAnimationEnable, VT_BOOL, propval);
}

float CLead::GetBitmapLeft()
{
   float result;
   GetProperty(dispidBitmapLeft, VT_R4, (void*)&result);
   return result;
}

void CLead::SetBitmapLeft(float propval)
{
   SetProperty(dispidBitmapLeft, VT_R4, propval);
}

float CLead::GetBitmapTop()
{
   float result;
   GetProperty(dispidBitmapTop, VT_R4, (void*)&result);
   return result;
}

void CLead::SetBitmapTop(float propval)
{
   SetProperty(dispidBitmapTop, VT_R4, propval);
}

long CLead::GetBitmapDelay()
{
   long result;
   GetProperty(dispidBitmapDelay, VT_I4, (void*)&result);
   return result;
}

void CLead::SetBitmapDelay(long propval)
{
   SetProperty(dispidBitmapDelay, VT_I4, propval);
}

float CLead::GetAnimationWidth()
{
   float result;
   GetProperty(dispidAnimationWidth, VT_R4, (void*)&result);
   return result;
}

void CLead::SetAnimationWidth(float propval)
{
   SetProperty(dispidAnimationWidth, VT_R4, propval);
}

float CLead::GetAnimationHeight()
{
   float result;
   GetProperty(dispidAnimationHeight, VT_R4, (void*)&result);
   return result;
}

void CLead::SetAnimationHeight(float propval)
{
   SetProperty(dispidAnimationHeight, VT_R4, propval);
}

OLE_COLOR CLead::GetAnimationBackColor()
{
   OLE_COLOR result;
   GetProperty(dispidAnimationBackColor, VT_I4, (void*)&result);
   return result;
}

void CLead::SetAnimationBackColor(OLE_COLOR propval)
{
   SetProperty(dispidAnimationBackColor, VT_I4, propval);
}

short CLead::GetAnimationBitsPerPixel()
{
   short result;
   GetProperty(dispidAnimationBitsPerPixel, VT_I2, (void*)&result);
   return result;
}

void CLead::SetAnimationBitsPerPixel(short propval)
{
   SetProperty(dispidAnimationBitsPerPixel, VT_I2, propval);
}

BOOL CLead::GetBitmapEnableTransparency()
{
   BOOL result;
   GetProperty(dispidBitmapEnableTransparency, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetBitmapEnableTransparency(BOOL propval)
{
   SetProperty(dispidBitmapEnableTransparency, VT_BOOL, propval);
}

OLE_COLOR CLead::GetBitmapTransparentColor()
{
   OLE_COLOR result;
   GetProperty(dispidBitmapTransparentColor, VT_I4, (void*)&result);
   return result;
}

void CLead::SetBitmapTransparentColor(OLE_COLOR propval)
{
   SetProperty(dispidBitmapTransparentColor, VT_I4, propval);
}

short CLead::GetBitmapDisposalMethod()
{
   short result;
   GetProperty(dispidBitmapDisposalMethod, VT_I2, (void*)&result);
   return result;
}

void CLead::SetBitmapDisposalMethod(short propval)
{
   SetProperty(dispidBitmapDisposalMethod, VT_I2, propval);
}

OLE_HANDLE CLead::GetBitmapList()
{
   OLE_HANDLE result;
   GetProperty(dispidBitmapList, VT_HANDLE, (void*)&result);
   return result;
}

void CLead::SetBitmapList(OLE_HANDLE propval)
{
   SetProperty(dispidBitmapList, VT_HANDLE, propval);
}

BOOL CLead::GetAnnAutoMenuEnable()
{
   BOOL result;
   GetProperty(dispidAnnAutoMenuEnable, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAnnAutoMenuEnable(BOOL propval)
{
   SetProperty(dispidAnnAutoMenuEnable, VT_BOOL, propval);
}

BOOL CLead::GetAutoAnimate()
{
   BOOL result;
   GetProperty(dispidAutoAnimate, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAutoAnimate(BOOL propval)
{
   SetProperty(dispidAutoAnimate, VT_BOOL, propval);
}

BOOL CLead::GetAnimationLoop()
{
   BOOL result;
   GetProperty(dispidAnimationLoop, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAnimationLoop(BOOL propval)
{
   SetProperty(dispidAnimationLoop, VT_BOOL, propval);
}

float CLead::GetInfoAnimationWidth()
{
   float result;
   GetProperty(dispidInfoAnimationWidth, VT_R4, (void*)&result);
   return result;
}

float CLead::GetInfoAnimationHeight()
{
   float result;
   GetProperty(dispidInfoAnimationHeight, VT_R4, (void*)&result);
   return result;
}

BOOL CLead::GetInfoAnimation()
{
   BOOL result;
   GetProperty(dispidInfoAnimation, VT_BOOL, (void*)&result);
   return result;
}

CString CLead::GetBitmapDataPath()
{
   CString result;
   GetProperty(dispidBitmapDataPath, VT_BSTR, (void*)&result);
   return result;
}

void CLead::SetBitmapDataPath(LPCTSTR propval)
{
   SetProperty(dispidBitmapDataPath, VT_BSTR, propval);
}

short CLead::GetPaintSizeMode()
{
   short result;
   GetProperty(dispidPaintSizeMode, VT_I2, (void*)&result);
   return result;
}

void CLead::SetPaintSizeMode(short propval)
{
   SetProperty(dispidPaintSizeMode, VT_I2, propval);
}

float CLead::GetPaintZoomFactor()
{
   float result;
   GetProperty(dispidPaintZoomFactor, VT_R4, (void*)&result);
   return result;
}

void CLead::SetPaintZoomFactor(float propval)
{
   SetProperty(dispidPaintZoomFactor, VT_R4, propval);
}

short CLead::GetAutoAnimationLoop()
{
   short result;
   GetProperty(dispidAutoAnimationLoop, VT_I2, (void*)&result);
   return result;
}

void CLead::SetAutoAnimationLoop(short propval)
{
   SetProperty(dispidAutoAnimationLoop, VT_I2, propval);
}

BOOL CLead::GetAutoSize()
{
   BOOL result;
   GetProperty(dispidAutoSize, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAutoSize(BOOL propval)
{
   SetProperty(dispidAutoSize, VT_BOOL, propval);
}

BOOL CLead::GetAnnAutoDrawEnable()
{
   BOOL result;
   GetProperty(dispidAnnAutoDrawEnable, VT_BOOL, (void*)&result);
   return result;
}

void CLead::SetAnnAutoDrawEnable(BOOL propval)
{
   SetProperty(dispidAnnAutoDrawEnable, VT_BOOL, propval);
}

short CLead::GetDimensionCount()
{
   short result;
   GetProperty(dispidDimensionCount, VT_I2, (void*)&result);
   return result;
}

BOOL CLead::GetOLEDropAllowed()
{
   BOOL result;
   GetProperty(dispidOLEDropAllowed, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetOLEDropAllowed(BOOL propval)
{
   SetProperty(dispidOLEDropAllowed, VT_BOOL, propval);
}

BOOL CLead::GetAutoRubberBand()
{
   BOOL result;
   GetProperty(dispidAutoRubberBand, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetAutoRubberBand(BOOL propval)
{
   SetProperty(dispidAutoRubberBand, VT_BOOL, propval);
}

BOOL CLead::GetSaveInterlaced()
{
   BOOL result;
   GetProperty(dispidSaveInterlaced, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetSaveInterlaced(BOOL propval)
{
   SetProperty(dispidSaveInterlaced, VT_BOOL, propval);
}

BOOL CLead::GetPaintSizeUseDPI()
{
   BOOL result;
   GetProperty(dispidPaintSizeUseDPI, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetPaintSizeUseDPI(BOOL propval)
{
   SetProperty(dispidPaintSizeUseDPI, VT_BOOL, propval);
}

CString CLead::GetAnnDataPath()
{
   CString result;
   GetProperty(dispidAnnDataPath, VT_BSTR, (void *)&result);
   return result;
}

void CLead::SetAnnDataPath(LPCTSTR propval)
{
   SetProperty(dispidAnnDataPath, VT_BSTR, propval);
}

BOOL CLead::GetDrawPersistence()
{
   BOOL result;
   GetProperty(dispidDrawPersistence, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetDrawPersistence(BOOL propval)
{
   SetProperty(dispidDrawPersistence, VT_BOOL, propval);
}

short CLead::GetDrawMode()
{
   short result;
   GetProperty(dispidDrawMode, VT_I2, (void *)&result);
   return result;
}

void CLead::SetDrawMode(short propval)
{
   SetProperty(dispidDrawMode, VT_I2, propval);
}

OLE_COLOR CLead::GetDrawPenColor()
{
   OLE_COLOR result;
   GetProperty(dispidDrawPenColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetDrawPenColor(OLE_COLOR propval)
{
   SetProperty(dispidDrawPenColor, VT_I4, propval);
}

short CLead::GetDrawPenWidth()
{
   short result;
   GetProperty(dispidDrawPenWidth, VT_I2, (void *)&result);
   return result;
}

void CLead::SetDrawPenWidth(short propval)
{
   SetProperty(dispidDrawPenWidth, VT_I2, propval);
}

short CLead::GetDrawPenStyle()
{
   short result;
   GetProperty(dispidDrawPenStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetDrawPenStyle(short propval)
{
   SetProperty(dispidDrawPenStyle, VT_I2, propval);
}

OLE_COLOR CLead::GetDrawFillColor()
{
   OLE_COLOR result;
   GetProperty(dispidDrawFillColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetDrawFillColor(OLE_COLOR propval)
{
   SetProperty(dispidDrawFillColor, VT_I4, propval);
}

short CLead::GetDrawFillStyle()
{
   short result;
   GetProperty(dispidDrawFillStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetDrawFillStyle(short propval)
{
   SetProperty(dispidDrawFillStyle, VT_I2, propval);
}

OLE_COLOR CLead::GetDrawFontColor()
{
   OLE_COLOR result;
   GetProperty(dispidDrawFontColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetDrawFontColor(OLE_COLOR propval)
{
   SetProperty(dispidDrawFontColor, VT_I4, propval);
}

short CLead::GetPaintContrast()
{
   short result;
   GetProperty(dispidPaintContrast, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPaintContrast(short propval)
{
   SetProperty(dispidPaintContrast, VT_I2, propval);
}

short CLead::GetPaintGamma()
{
   short result;
   GetProperty(dispidPaintGamma, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPaintGamma(short propval)
{
   SetProperty(dispidPaintGamma, VT_I2, propval);
}

short CLead::GetPaintIntensity()
{
   short result;
   GetProperty(dispidPaintIntensity, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPaintIntensity(short propval)
{
   SetProperty(dispidPaintIntensity, VT_I2, propval);
}

BOOL CLead::GetLoadUseViewPerspective()
{
   BOOL result;
   GetProperty(dispidLoadUseViewPerspective, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetLoadUseViewPerspective(BOOL propval)
{
   SetProperty(dispidLoadUseViewPerspective, VT_BOOL, propval);
}

BOOL CLead::GetAnnAutoRotate()
{
   BOOL result;
   GetProperty(dispidAnnAutoRotate, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetAnnAutoRotate(BOOL propval)
{
   SetProperty(dispidAnnAutoRotate, VT_BOOL, propval);
}

short CLead::GetTransitionStyle()
{
   short result;
   GetProperty(dispidTransitionStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetTransitionStyle(short propval)
{
   SetProperty(dispidTransitionStyle, VT_I2, propval);
}

short CLead::GetTransitionEffect()
{
   short result;
   GetProperty(dispidTransitionEffect, VT_I2, (void *)&result);
   return result;
}

void CLead::SetTransitionEffect(short propval)
{
   SetProperty(dispidTransitionEffect, VT_I2, propval);
}

short CLead::GetGradientStyle()
{
   short result;
   GetProperty(dispidGradientStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetGradientStyle(short propval)
{
   SetProperty(dispidGradientStyle, VT_I2, propval);
}

OLE_COLOR CLead::GetGradientStartColor()
{
   OLE_COLOR result;
   GetProperty(dispidGradientStartColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetGradientStartColor(OLE_COLOR propval)
{
   SetProperty(dispidGradientStartColor, VT_I4, propval);
}

OLE_COLOR CLead::GetGradientEndColor()
{
   OLE_COLOR result;
   GetProperty(dispidGradientEndColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetGradientEndColor(OLE_COLOR propval)
{
   SetProperty(dispidGradientEndColor, VT_I4, propval);
}

short CLead::GetGradientSteps()
{
   short result;
   GetProperty(dispidGradientSteps, VT_I2, (void *)&result);
   return result;
}

void CLead::SetGradientSteps(short propval)
{
   SetProperty(dispidGradientSteps, VT_I2, propval);
}

short CLead::GetPatternStyle()
{
   short result;
   GetProperty(dispidPatternStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPatternStyle(short propval)
{
   SetProperty(dispidPatternStyle, VT_I2, propval);
}

OLE_COLOR CLead::GetPatternForeColor()
{
   OLE_COLOR result;
   GetProperty(dispidPatternForeColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetPatternForeColor(OLE_COLOR propval)
{
   SetProperty(dispidPatternForeColor, VT_I4, propval);
}

OLE_COLOR CLead::GetPatternBackColor()
{
   OLE_COLOR result;
   GetProperty(dispidPatternBackColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetPatternBackColor(OLE_COLOR propval)
{
   SetProperty(dispidPatternBackColor, VT_I4, propval);
}

short CLead::GetPaintPass()
{
   short result;
   GetProperty(dispidPaintPass, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPaintPass(short propval)
{
   SetProperty(dispidPaintPass, VT_I2, propval);
}

short CLead::GetPaintMaxPasses()
{
   short result;
   GetProperty(dispidPaintMaxPasses, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPaintMaxPasses(short propval)
{
   SetProperty(dispidPaintMaxPasses, VT_I2, propval);
}

short CLead::GetEffectGrain()
{
   short result;
   GetProperty(dispidEffectGrain, VT_I2, (void *)&result);
   return result;
}

void CLead::SetEffectGrain(short propval)
{
   SetProperty(dispidEffectGrain, VT_I2, propval);
}

short CLead::GetEffectDelay()
{
   short result;
   GetProperty(dispidEffectDelay, VT_I2, (void *)&result);
   return result;
}

void CLead::SetEffectDelay(short propval)
{
   SetProperty(dispidEffectDelay, VT_I2, propval);
}

short CLead::GetWandSize()
{
   short result;
   GetProperty(dispidWandSize, VT_I2, (void *)&result);
   return result;
}

void CLead::SetWandSize(short propval)
{
   SetProperty(dispidWandSize, VT_I2, propval);
}

OLE_COLOR CLead::GetWandColor()
{
   OLE_COLOR result;
   GetProperty(dispidWandColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetWandColor(OLE_COLOR propval)
{
   SetProperty(dispidWandColor, VT_I4, propval);
}

BOOL CLead::GetUseTransparentColor()
{
   BOOL result;
   GetProperty(dispidUseTransparentColor, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetUseTransparentColor(BOOL propval)
{
   SetProperty(dispidUseTransparentColor, VT_BOOL, propval);
}

OLE_COLOR CLead::GetTransparentColor()
{
   OLE_COLOR result;
   GetProperty(dispidTransparentColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetTransparentColor(OLE_COLOR propval)
{
   SetProperty(dispidTransparentColor, VT_I4, propval);
}

BOOL CLead::GetPaintRgnOnly()
{
   BOOL result;
   GetProperty(dispidPaintRgnOnly, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetPaintRgnOnly(BOOL propval)
{
   SetProperty(dispidPaintRgnOnly, VT_BOOL, propval);
}

float CLead::GetShapeTop()
{
   float result;
   GetProperty(dispidShapeTop, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShapeTop(float propval)
{
   SetProperty(dispidShapeTop, VT_R4, propval);
}

float CLead::GetShapeLeft()
{
   float result;
   GetProperty(dispidShapeLeft, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShapeLeft(float propval)
{
   SetProperty(dispidShapeLeft, VT_R4, propval);
}

float CLead::GetShapeWidth()
{
   float result;
   GetProperty(dispidShapeWidth, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShapeWidth(float propval)
{
   SetProperty(dispidShapeWidth, VT_R4, propval);
}

float CLead::GetShapeHeight()
{
   float result;
   GetProperty(dispidShapeHeight, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShapeHeight(float propval)
{
   SetProperty(dispidShapeHeight, VT_R4, propval);
}

float CLead::GetShapeBorderThickness()
{
   float result;
   GetProperty(dispidShapeBorderThickness, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShapeBorderThickness(float propval)
{
   SetProperty(dispidShapeBorderThickness, VT_R4, propval);
}

OLE_COLOR CLead::GetShapeBorderColor()
{
   OLE_COLOR result;
   GetProperty(dispidShapeBorderColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetShapeBorderColor(OLE_COLOR propval)
{
   SetProperty(dispidShapeBorderColor, VT_I4, propval);
}

short CLead::GetShapeInnerBandStyle()
{
   short result;
   GetProperty(dispidShapeInnerBandStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetShapeInnerBandStyle(short propval)
{
   SetProperty(dispidShapeInnerBandStyle, VT_I2, propval);
}

float CLead::GetShapeInnerBandThickness()
{
   float result;
   GetProperty(dispidShapeInnerBandThickness, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShapeInnerBandThickness(float propval)
{
   SetProperty(dispidShapeInnerBandThickness, VT_R4, propval);
}

OLE_COLOR CLead::GetShapeInnerBandHiliteColor()
{
   OLE_COLOR result;
   GetProperty(dispidShapeInnerBandHiliteColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetShapeInnerBandHiliteColor(OLE_COLOR propval)
{
   SetProperty(dispidShapeInnerBandHiliteColor, VT_I4, propval);
}

OLE_COLOR CLead::GetShapeInnerBandShadowColor()
{
   OLE_COLOR result;
   GetProperty(dispidShapeInnerBandShadowColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetShapeInnerBandShadowColor(OLE_COLOR propval)
{
   SetProperty(dispidShapeInnerBandShadowColor, VT_I4, propval);
}

short CLead::GetShapeBackgroundStyle()
{
   short result;
   GetProperty(dispidShapeBackgroundStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetShapeBackgroundStyle(short propval)
{
   SetProperty(dispidShapeBackgroundStyle, VT_I2, propval);
}

float CLead::GetBackgroundImageTop()
{
   float result;
   GetProperty(dispidBackgroundImageTop, VT_R4, (void *)&result);
   return result;
}

void CLead::SetBackgroundImageTop(float propval)
{
   SetProperty(dispidBackgroundImageTop, VT_R4, propval);
}

float CLead::GetBackgroundImageLeft()
{
   float result;
   GetProperty(dispidBackgroundImageLeft, VT_R4, (void *)&result);
   return result;
}

void CLead::SetBackgroundImageLeft(float propval)
{
   SetProperty(dispidBackgroundImageLeft, VT_R4, propval);
}

float CLead::GetBackgroundImageWidth()
{
   float result;
   GetProperty(dispidBackgroundImageWidth, VT_R4, (void *)&result);
   return result;
}

void CLead::SetBackgroundImageWidth(float propval)
{
   SetProperty(dispidBackgroundImageWidth, VT_R4, propval);
}

float CLead::GetBackgroundImageHeight()
{
   float result;
   GetProperty(dispidBackgroundImageHeight, VT_R4, (void *)&result);
   return result;
}

void CLead::SetBackgroundImageHeight(float propval)
{
   SetProperty(dispidBackgroundImageHeight, VT_R4, propval);
}

float CLead::GetShadowXDepth()
{
   float result;
   GetProperty(dispidShadowXDepth, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShadowXDepth(float propval)
{
   SetProperty(dispidShadowXDepth, VT_R4, propval);
}

float CLead::GetShadowYDepth()
{
   float result;
   GetProperty(dispidShadowYDepth, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShadowYDepth(float propval)
{
   SetProperty(dispidShadowYDepth, VT_R4, propval);
}

OLE_COLOR CLead::GetShadowColor()
{
   OLE_COLOR result;
   GetProperty(dispidShadowColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetShadowColor(OLE_COLOR propval)
{
   SetProperty(dispidShadowColor, VT_I4, propval);
}

OLE_COLOR CLead::GetTextHiliteColor()
{
   OLE_COLOR result;
   GetProperty(dispidTextHiliteColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetTextHiliteColor(OLE_COLOR propval)
{
   SetProperty(dispidTextHiliteColor, VT_I4, propval);
}

float CLead::GetTextLeft()
{
   float result;
   GetProperty(dispidTextLeft, VT_R4, (void *)&result);
   return result;
}

void CLead::SetTextLeft(float propval)
{
   SetProperty(dispidTextLeft, VT_R4, propval);
}

float CLead::GetTextTop()
{
   float result;
   GetProperty(dispidTextTop, VT_R4, (void *)&result);
   return result;
}

void CLead::SetTextTop(float propval)
{
   SetProperty(dispidTextTop, VT_R4, propval);
}

float CLead::GetTextWidth()
{
   float result;
   GetProperty(dispidTextWidth, VT_R4, (void *)&result);
   return result;
}

void CLead::SetTextWidth(float propval)
{
   SetProperty(dispidTextWidth, VT_R4, propval);
}

float CLead::GetTextHeight()
{
   float result;
   GetProperty(dispidTextHeight, VT_R4, (void *)&result);
   return result;
}

void CLead::SetTextHeight(float propval)
{
   SetProperty(dispidTextHeight, VT_R4, propval);
}

short CLead::GetTextAngle()
{
   short result;
   GetProperty(dispidTextAngle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetTextAngle(short propval)
{
   SetProperty(dispidTextAngle, VT_I2, propval);
}

short CLead::GetTextStyle()
{
   short result;
   GetProperty(dispidTextStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetTextStyle(short propval)
{
   SetProperty(dispidTextStyle, VT_I2, propval);
}

short CLead::GetShapeOuterBandStyle()
{
   short result;
   GetProperty(dispidShapeOuterBandStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetShapeOuterBandStyle(short propval)
{
   SetProperty(dispidShapeOuterBandStyle, VT_I2, propval);
}

float CLead::GetShapeOuterBandThickness()
{
   float result;
   GetProperty(dispidShapeOuterBandThickness, VT_R4, (void *)&result);
   return result;
}

void CLead::SetShapeOuterBandThickness(float propval)
{
   SetProperty(dispidShapeOuterBandThickness, VT_R4, propval);
}

OLE_COLOR CLead::GetShapeOuterBandHiliteColor()
{
   OLE_COLOR result;
   GetProperty(dispidShapeOuterBandHiliteColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetShapeOuterBandHiliteColor(OLE_COLOR propval)
{
   SetProperty(dispidShapeOuterBandHiliteColor, VT_I4, propval);
}

OLE_COLOR CLead::GetShapeOuterBandShadowColor()
{
   OLE_COLOR result;
   GetProperty(dispidShapeOuterBandShadowColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetShapeOuterBandShadowColor(OLE_COLOR propval)
{
   SetProperty(dispidShapeOuterBandShadowColor, VT_I4, propval);
}

short CLead::GetShapeBorderStyle()
{
   short result;
   GetProperty(dispidShapeBorderStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetShapeBorderStyle(short propval)
{
   SetProperty(dispidShapeBorderStyle, VT_I2, propval);
}

float CLead::GetFrameThickness()
{
   float result;
   GetProperty(dispidFrameThickness, VT_R4, (void *)&result);
   return result;
}

void CLead::SetFrameThickness(float propval)
{
   SetProperty(dispidFrameThickness, VT_R4, propval);
}

OLE_COLOR CLead::GetFrameColor()
{
   OLE_COLOR result;
   GetProperty(dispidFrameColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetFrameColor(OLE_COLOR propval)
{
   SetProperty(dispidFrameColor, VT_I4, propval);
}

short CLead::GetFrameInnerStyle()
{
   short result;
   GetProperty(dispidFrameInnerStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetFrameInnerStyle(short propval)
{
   SetProperty(dispidFrameInnerStyle, VT_I2, propval);
}

float CLead::GetFrameInnerThickness()
{
   float result;
   GetProperty(dispidFrameInnerThickness, VT_R4, (void *)&result);
   return result;
}

void CLead::SetFrameInnerThickness(float propval)
{
   SetProperty(dispidFrameInnerThickness, VT_R4, propval);
}

OLE_COLOR CLead::GetFrameInnerHiliteColor()
{
   OLE_COLOR result;
   GetProperty(dispidFrameInnerHiliteColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetFrameInnerHiliteColor(OLE_COLOR propval)
{
   SetProperty(dispidFrameInnerHiliteColor, VT_I4, propval);
}

OLE_COLOR CLead::GetFrameInnerShadowColor()
{
   OLE_COLOR result;
   GetProperty(dispidFrameInnerShadowColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetFrameInnerShadowColor(OLE_COLOR propval)
{
   SetProperty(dispidFrameInnerShadowColor, VT_I4, propval);
}

short CLead::GetFrameOuterStyle()
{
   short result;
   GetProperty(dispidFrameOuterStyle, VT_I2, (void *)&result);
   return result;
}

void CLead::SetFrameOuterStyle(short propval)
{
   SetProperty(dispidFrameOuterStyle, VT_I2, propval);
}

float CLead::GetFrameOuterThickness()
{
   float result;
   GetProperty(dispidFrameOuterThickness, VT_R4, (void *)&result);
   return result;
}

void CLead::SetFrameOuterThickness(float propval)
{
   SetProperty(dispidFrameOuterThickness, VT_R4, propval);
}

OLE_COLOR CLead::GetFrameOuterHiliteColor()
{
   OLE_COLOR result;
   GetProperty(dispidFrameOuterHiliteColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetFrameOuterHiliteColor(OLE_COLOR propval)
{
   SetProperty(dispidFrameOuterHiliteColor, VT_I4, propval);
}

OLE_COLOR CLead::GetFrameOuterShadowColor()
{
   OLE_COLOR result;
   GetProperty(dispidFrameOuterShadowColor, VT_I4, (void *)&result);
   return result;
}

void CLead::SetFrameOuterShadowColor(OLE_COLOR propval)
{
   SetProperty(dispidFrameOuterShadowColor, VT_I4, propval);
}

short CLead::GetTextAlign()
{
   short result;
   GetProperty(dispidTextAlign, VT_I2, (void *)&result);
   return result;
}

void CLead::SetTextAlign(short propval)
{
   SetProperty(dispidTextAlign, VT_I2, propval);
}

BOOL CLead::GetTextWordWrap()
{
   BOOL result;
   GetProperty(dispidTextWordWrap, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetTextWordWrap(BOOL propval)
{
   SetProperty(dispidTextWordWrap, VT_BOOL, propval);
}

OLE_HANDLE CLead::GetBitmapAlpha()
{
   OLE_HANDLE result;
   GetProperty(dispidBitmapAlpha, VT_HANDLE, (void *)&result);
   return result;
}

void CLead::SetBitmapAlpha(OLE_HANDLE propval)
{
   SetProperty(dispidBitmapAlpha, VT_HANDLE, propval);
}

short CLead::GetPaintNotificationDelay()
{
   short result;
   GetProperty(dispidPaintNotificationDelay, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPaintNotificationDelay(short propval)
{
   SetProperty(dispidPaintNotificationDelay, VT_I2, propval);
}

BOOL CLead::GetTransparent()
{
   BOOL result;
   GetProperty(dispidTransparent, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetTransparent(BOOL propval)
{
   SetProperty(dispidTransparent, VT_BOOL, propval);
}

OLE_HANDLE CLead::GetAnimationBitmap()
{
   OLE_HANDLE result;
   GetProperty(dispidAnimationBitmap, VT_HANDLE, (void*)&result);
   return result;
}

void CLead::SetAnimationBitmap(OLE_HANDLE propval)
{
   SetProperty(dispidAnimationBitmap, VT_HANDLE, propval);
}



/* methods */
short CLead::Average(short iDim)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidAverage, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iDim);   
      
   return result;
} 

OLE_HANDLE CLead::PrintStart()
{
   OLE_HANDLE   result;

   InvokeHelper(dispidPrintStart, DISPATCH_METHOD, VT_HANDLE, (void*)&result, NULL);
   
   return result;
} 

void CLead::PrintEnd(OLE_HANDLE hDC)
{
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidPrintEnd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      hDC);
} 

short CLead::Median(short iDim)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidMedian, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iDim);   
      
   return result;
} 

short CLead::Contrast(short iChange)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidContrast, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iChange);   
      
   return result;
} 

short CLead::Despeckle()
{
   short   result;

   InvokeHelper(dispidDespeckle, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::Flip()
{
   short   result;

   InvokeHelper(dispidFlip, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::Grayscale(short iBitsPerPixel)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidGrayscale, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iBitsPerPixel);

   return result;
} 

short CLead::HistoEqualize()
{
   short   result;

   InvokeHelper(dispidHistoEqualize, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::AnnFlip(BOOL bUseHorz, float fHorz, BOOL bSelected)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BOOL VTS_R4 VTS_I2 VTS_BOOL  ;

   InvokeHelper(dispidAnnFlip, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      bUseHorz, fHorz, bSelected);   
      
   return result;
}

short CLead::AnnReverse(BOOL bUseVert, float fVert, BOOL bSelected)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BOOL VTS_R4 VTS_I2 VTS_BOOL  ;

   InvokeHelper(dispidAnnReverse, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      bUseVert, fVert, bSelected);   
      
   return result;
}

short CLead::AnnRotate(BOOL bUseCenter, float fCenterX, float fCenterY, float fAngle, BOOL bSelected)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BOOL VTS_R4 VTS_R4 VTS_R4 VTS_BOOL  ;

   InvokeHelper(dispidAnnRotate, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      bUseCenter, fCenterX, fCenterY, fAngle, bSelected);   
      
   return result;
}

short CLead::Hue(short iAngle)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidHue, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iAngle);   
      
   return result;
} 

short CLead::Intensity(short iChange)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidIntensity, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iChange);   
      
   return result;
} 

short CLead::Invert()
{
   short   result;

   InvokeHelper(dispidInvert, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::TwainAcquire(OLE_HANDLE hWnd)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidTwainAcquire, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hWnd);   
      
   return result;
} 

short CLead::TwainSelect(OLE_HANDLE hWnd)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidTwainSelect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hWnd);   
      
   return result;
} 

short CLead::Sharpen(short iChange)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidSharpen, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iChange);   
      
   return result;
} 

short CLead::Saturation(short iChange)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidSaturation, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iChange);   
      
   return result;
} 

short CLead::Reverse()
{
   short   result;

   InvokeHelper(dispidReverse, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::ForceRepaint()
{
   short   result;

   InvokeHelper(dispidForceRepaint, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::Mosaic(short iDim)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidMosaic, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iDim);   
      
   return result;
} 

short CLead::SpatialFilter(short iType)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidSpatialFilter, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iType);   
      
   return result;
} 

short CLead::BinaryFilter(short iType)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidBinaryFilter, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iType);   
      
   return result;
} 

VOID CLead::UnlockSupport(short iType, LPCTSTR pszKey)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_BSTR  ;

   InvokeHelper(dispidUnlockSupport, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, parms,
      iType, pszKey);
      
}

BOOL CLead::IsSupportLocked(short iType)
{
   BOOL   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidIsSupportLocked, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      iType);
      
   return result;
}

short CLead::StartFeedLoad(short iBitsPerPixel, short iPage, short iPages)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2 VTS_I2  ;

   InvokeHelper(dispidStartFeedLoad, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iBitsPerPixel, iPage, iPages);
      
   return result;
}

short CLead::StopFeedLoad()
{
   short   result;

   InvokeHelper(dispidStopFeedLoad, DISPATCH_METHOD, VT_I2, (void*)&result, NULL );
      
   return result;
}

short CLead::SaveMemory(OLE_HANDLE FAR *phMem, short iFormat, short iBitsPerPixel, short iQuality, long FAR *piSize)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_PHANDLE VTS_I2 VTS_I2 VTS_I2 VTS_PI4  ;

   InvokeHelper(dispidSaveMemory, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      phMem, iFormat, iBitsPerPixel, iQuality, piSize);   
      
   return result;
}

short CLead::LoadMemory(OLE_HANDLE hMem, short iBitsPerPixel, short iPage, long iSize)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE VTS_I2 VTS_I2 VTS_I4  ;

   InvokeHelper(dispidLoadMemory, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hMem, iBitsPerPixel, iPage, iSize);
      
   return result;
}

short CLead::GetMemoryInfo(OLE_HANDLE hMem, short iPage, long iSize)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE VTS_I2 VTS_I4  ;

   InvokeHelper(dispidGetMemoryInfo, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hMem, iPage, iSize);
      
   return result;
}

short CLead::FeedLoad(const VARIANT FAR& vBuffer, long iSize)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_VARIANT VTS_I4  ;

   InvokeHelper(dispidFeedLoad, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      &vBuffer, iSize);
      
   return result;
}

short CLead::Save(LPCTSTR pszName, short iFormat, short iBitsPerPixel, short iQuality, BOOL bMultiPage)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_BOOL  ;

   InvokeHelper(dispidSave, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszName, iFormat, iBitsPerPixel, iQuality, bMultiPage);   
      
   return result;
} 

short CLead::Posterize(short iLevels)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidPosterize, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iLevels);   
      
   return result;
} 

LPPICTUREDISP CLead::GetPicture()
{
   LPPICTUREDISP result;

   InvokeHelper(dispidGetPicture, DISPATCH_METHOD, VT_PICTURE, (void*)&result, NULL);
   
   return result;
} 

short CLead::SetPicture(LPPICTUREDISP pPicture)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_PICTURE  ;

   InvokeHelper(dispidSetPicture, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pPicture);   
      
   return result;
} 

short CLead::Rotate(long iAngle, BOOL bResize, OLE_COLOR crFill)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I4 VTS_BOOL VTS_I4  ;

   InvokeHelper(dispidRotate, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iAngle, bResize, crFill);   
      
   return result;
} 

short CLead::Emboss(short iDirection, short iDepth)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2  ;

   InvokeHelper(dispidEmboss, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iDirection, iDepth);   
      
   return result;
} 

short CLead::AddNoise(short iRange, short iChannel)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2  ;

   InvokeHelper(dispidAddNoise, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iRange, iChannel);   
      
   return result;
} 

short CLead::IntensityDetect(short iLow, short iHigh)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2  ;

   InvokeHelper(dispidIntensityDetect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iLow, iHigh);   
      
   return result;
} 

short CLead::Fill(OLE_COLOR crFill)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I4  ;

   InvokeHelper(dispidFill, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      crFill);   
      
   return result;
} 

short CLead::StretchIntensity()
{
   short   result;

   InvokeHelper(dispidStretchIntensity, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::Deskew()
{
   short   result;

   InvokeHelper(dispidDeskew, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::GammaCorrect(short iGamma)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidGammaCorrect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iGamma);   
      
   return result;
} 

short CLead::Shear(short iAngle, BOOL bHorizontal, OLE_COLOR crFill)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_BOOL VTS_I4  ;

   InvokeHelper(dispidShear, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iAngle, bHorizontal, crFill);   
      
   return result;
} 

short CLead::CreateBitmap(float fWidth, float fHeight, short iBitsPerPixel)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_I2  ;

   InvokeHelper(dispidCreateBitmap, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fWidth, fHeight, iBitsPerPixel);   
      
   return result;
} 

short CLead::Render(OLE_HANDLE hDC, float fLeft, float fTop, float fWidth, float fHeight)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidRender, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hDC, fLeft, fTop, fWidth, fHeight);   
      
   return result;
} 

OLE_HANDLE CLead::GetBitmapDC()
{
   OLE_HANDLE   result;

   InvokeHelper(dispidGetBitmapDC, DISPATCH_METHOD, VT_HANDLE, (void*)&result, NULL);
   
   return result;
} 

short CLead::ReleaseBitmapDC()
{
   short   result;

   InvokeHelper(dispidReleaseBitmapDC, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

short CLead::Halftone(short iType, long iAngle)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I4  ;

   InvokeHelper(dispidHalftone, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iType, iAngle);   
      
   return result;
} 

short CLead::Size(float fWidth, float fHeight, short iFlags)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_I2  ;

   InvokeHelper(dispidSize, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fWidth, fHeight, iFlags);   
      
   return result;
} 

short CLead::Copy(short iFlags)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidCopy, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFlags);   
      
   return result;
} 

short CLead::Paste(short iFlags)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidPaste, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFlags);   
      
   return result;
} 

short CLead::Load(LPCTSTR pszName, short iBitsPerPixel, short iPage, short iPages)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BSTR VTS_I2 VTS_I2 VTS_I2 ;

   InvokeHelper(dispidLoad, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszName, iBitsPerPixel, iPage, iPages);   
      
   return result;
} 

short CLead::SetDIB(OLE_HANDLE hDIB)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidSetDIB, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hDIB);   
      
   return result;
} 

OLE_HANDLE CLead::GetDIB()
{
   OLE_HANDLE   result;

   InvokeHelper(dispidGetDIB, DISPATCH_METHOD, VT_HANDLE, (void*)&result, NULL);
   
   return result;
} 

short CLead::SetDDB(OLE_HANDLE hDC, OLE_HANDLE hBitmap, OLE_HANDLE hPalette)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE VTS_HANDLE VTS_HANDLE  ;

   InvokeHelper(dispidSetDDB, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hDC, hBitmap, hPalette);   
      
   return result;
} 

OLE_HANDLE CLead::GetDDB(OLE_HANDLE hDC)
{
   OLE_HANDLE   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidGetDDB, DISPATCH_METHOD, VT_HANDLE, (void*)&result, parms,
      hDC);   
      
   return result;
} 

short CLead::RemapIntensity(short iChannel)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidRemapIntensity, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iChannel);   
      
   return result;
} 

short CLead::GetHistogram(short iChannel)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidGetHistogram, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iChannel);   
      
   return result;
} 

short CLead::ColorRes(short iBitsPerPixel, short iPalette, short iDither, short iColors)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2 VTS_I2 VTS_I2  ;

   InvokeHelper(dispidColorRes, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iBitsPerPixel, iPalette, iDither, iColors);   
      
   return result;
} 

short CLead::Capture(OLE_HANDLE hDC, float fLeft, float fTop, float fWidth, float fHeight)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidCapture, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hDC, fLeft, fTop, fWidth, fHeight);   
      
   return result;
} 

short CLead::ColorSeparate(long iFlags)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I4  ;

   InvokeHelper(dispidColorSeparate, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFlags);   
      
   return result;
} 

short CLead::GetServerBitmap(long pServer)
{
   short result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(dispidGetServerBitmap, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pServer);
   return result;
}

short CLead::SetServerBitmap(long pServer)
{
   short result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(dispidSetServerBitmap, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pServer);
   return result;
}

short CLead::DeleteBitmapListItems(short iIndex, short iCount)
{
   short result;
   static BYTE parms[] =
      VTS_I2 VTS_I2;
   InvokeHelper(dispidDeleteBitmapListItems, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      iIndex, iCount);
   return result;
}

short CLead::AnnLoadMemory(OLE_HANDLE hMem, long iSize)
{
   short result;
   static BYTE parms[] =
      VTS_HANDLE VTS_I4;
   InvokeHelper(dispidAnnLoadMemory, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      hMem, iSize);
   return result;
}

short CLead::AnnSaveMemory(OLE_HANDLE* phMem, short iFormat, BOOL bSelected, long* piSize)
{
   short result;
   static BYTE parms[] =
      VTS_PI4 VTS_I2 VTS_BOOL VTS_PI4;
   InvokeHelper(dispidAnnSaveMemory, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      phMem, iFormat, bSelected, piSize);
   return result;
}

short CLead::DrawEllipse(float fLeft, float fTop, float fWidth, float fHeight)
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_R4 VTS_R4;
   InvokeHelper(dispidDrawEllipse, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      fLeft, fTop, fWidth, fHeight);
   return result;
}

short CLead::DrawRectangle(float fLeft, float fTop, float fWidth, float fHeight)
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_R4 VTS_R4;
   InvokeHelper(dispidDrawRectangle, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      fLeft, fTop, fWidth, fHeight);
   return result;
}

short CLead::DrawLine(float fX1, float fY1, float fX2, float fY2)
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_R4 VTS_R4;
   InvokeHelper(dispidDrawLine, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      fX1, fY1, fX2, fY2);
   return result;
}

short CLead::DrawTextOut(float fX, float fY, LPCSTR pszText)
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_BSTR;
   InvokeHelper(dispidDrawTextOut, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      fX, fY, pszText);
   return result;
}

float CLead::DrawTextWidth(LPCSTR pszText)
{
   float result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(dispidDrawTextWidth, DISPATCH_METHOD, VT_R4, (void *)&result, parms,
      pszText);
   return result;
}

float CLead::DrawTextHeight(LPCSTR pszText)
{
   float result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(dispidDrawTextHeight, DISPATCH_METHOD, VT_R4, (void *)&result, parms,
      pszText);
   return result;
}

short CLead::ZoomToRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_R4 VTS_R4;
   InvokeHelper(dispidZoomToRect, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      fLeft, fTop, fWidth, fHeight);
   return result;
}

short CLead::Solarize(short iThreshold)
{
   short result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidSolarize, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      iThreshold);
   return result;
}

short CLead::AutoTrim(short iThreshold)
{
   short result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidAutoTrim, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      iThreshold);
   return result;
}

short CLead::MinFilter(short iDim)
{
   short result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidMinFilter, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      iDim);
   return result;
}

short CLead::MaxFilter(short iDim)
{
   short result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidMaxFilter, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      iDim);
   return result;
}

short CLead::Oilify(short iDim)
{
   short result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidOilify, DISPATCH_METHOD, VT_I2, (void *)&result, parms,
      iDim);
   return result;
}

short CLead::ColorMerge(long iFlags)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I4  ;

   InvokeHelper(dispidColorMerge, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFlags);   
      
   return result;
} 

short CLead::Underlay(OLE_HANDLE hBitmap, short iFlags)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE VTS_I2  ;

   InvokeHelper(dispidUnderlay, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hBitmap, iFlags);   
      
   return result;
} 

short CLead::Combine(float fLeft, float fTop, float fWidth, float fHeight, OLE_HANDLE hBitmap, float fSrcLeft, float fSrcTop, long iFlags)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_HANDLE VTS_R4 VTS_R4 VTS_I4  ;

   InvokeHelper(dispidCombine, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight, hBitmap, fSrcLeft, fSrcTop, iFlags);   
      
   return result;
} 

short CLead::SetDstRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidSetDstRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight);   
      
   return result;
} 

short CLead::SetDstClipRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidSetDstClipRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight);   
      
   return result;
} 

short CLead::SetSrcRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidSetSrcRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight);   
      
   return result;
} 

short CLead::SetSrcClipRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidSetSrcClipRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight);   
      
   return result;
} 

short CLead::Trim(float fLeft, float fTop, float fWidth, float fHeight)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidTrim, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight);   
      
   return result;
} 

short CLead::HistoContrast(short iChange)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidHistoContrast, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iChange);   
      
   return result;
} 

short CLead::GetFileInfo(LPCTSTR pszName, short iPage)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BSTR VTS_I2  ;

   InvokeHelper(dispidGetFileInfo, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszName, iPage);   
      
   return result;
} 

short CLead::SaveWithStamp(LPCTSTR pszName, short iFormat, short iBitsPerPixel, short iQuality, float fStampWidth, float fStampHeight, short iStampBits)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_R4 VTS_R4 VTS_I2  ;

   InvokeHelper(dispidSaveWithStamp, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszName, iFormat, iBitsPerPixel, iQuality, fStampWidth, fStampHeight, iStampBits);   
      
   return result;
} 

short CLead::LoadStamp(LPCTSTR pszName)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BSTR  ;

   InvokeHelper(dispidLoadStamp, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszName);   
      
   return result;
} 

void CLead::SetRubberBandRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4  ;

   InvokeHelper(dispidSetRubberBandRect, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      fLeft, fTop, fWidth, fHeight);
}

OLE_HANDLE CLead::GetClientDC()
{
   OLE_HANDLE   result;

   InvokeHelper(dispidGetClientDC, DISPATCH_METHOD, VT_HANDLE, (void*)&result, NULL);
   
   return result;
} 

short CLead::ReleaseClientDC()
{
   short   result;

   InvokeHelper(dispidReleaseClientDC, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   
   return result;
} 

OLE_HANDLE CLead::GetPalette(OLE_HANDLE hDC)
{
   OLE_HANDLE   result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidGetPalette, DISPATCH_METHOD, VT_HANDLE, (void*)&result, parms,
      hDC);   
      
   return result;
} 

void CLead::dbAddNew()
{
   InvokeHelper(dispidDbAddNew, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CLead::dbDelete()
{
   InvokeHelper(dispidDbDelete, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void CLead::dbEdit()
{
   InvokeHelper(dispidDbEdit, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void CLead::dbMove(long lRows)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I4  ;

   InvokeHelper(dispidDbMove, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      lRows);
}
void CLead::dbMoveFirst()
{
   InvokeHelper(dispidDbMoveFirst, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void CLead::dbMoveLast()
{
   InvokeHelper(dispidDbMoveLast, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void CLead::dbMoveNext()
{
   InvokeHelper(dispidDbMoveNext, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void CLead::dbMovePrev()
{
   InvokeHelper(dispidDbMovePrev, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void CLead::dbClose()
{
   InvokeHelper(dispidDbClose, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
OLE_COLOR CLead::GetBitmapPalette(short iIndex)
{
   OLE_COLOR   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidBitmapPalette, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetBitmapPalette(short iIndex, OLE_COLOR param1)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I4  ;

   InvokeHelper(dispidBitmapPalette, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, param1);
}

short CLead::GetRemapTable(short iIndex)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidRemapTable, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetRemapTable(short iIndex, short param1)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2  ;

   InvokeHelper(dispidRemapTable, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, param1);
}

long CLead::GetHistogramTable(short iIndex)
{
   long   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidHistogramTable, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetHistogramTable(short iIndex, long param1)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I4  ;

   InvokeHelper(dispidHistogramTable, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, param1);
}

OLE_COLOR CLead::GetUserPalette(short iIndex)
{
   OLE_COLOR   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidUserPalette, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetUserPalette(short iIndex, OLE_COLOR param1)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I4  ;

   InvokeHelper(dispidUserPalette, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, param1);
}

OLE_HANDLE CLead::GetColorPlanes(short iIndex)
{
   OLE_HANDLE   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidColorPlanes, DISPATCH_METHOD, VT_HANDLE, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetColorPlanes(short iIndex, OLE_HANDLE param1)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_HANDLE  ;

   InvokeHelper(dispidColorPlanes, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, param1);
}

BOOL CLead::GetInfoPCDRes(short iIndex)
{
   BOOL   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidInfoPCDRes, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      iIndex);
   return result;
} 

BOOL CLead::dbRequery()
{
   BOOL   result;

   InvokeHelper(dispidDbRequery, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   
   return result;
} 

BOOL CLead::dbOpen(LPCTSTR pszConnect, LPCTSTR pszSQL, LPCTSTR pszField, long iOptions)
{
   BOOL   result;
   static BYTE BASED_CODE parms[] = 
      VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4  ;

   InvokeHelper(dispidDbOpen, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      pszConnect, pszSQL, pszField, iOptions);   
      
   return result;
} 

short CLead::dbUpdate(short iFormat, short iBitsPerPixel, short iQuality)
{
   short   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2 VTS_I2  ;

   InvokeHelper(dispidDbUpdate, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFormat, iBitsPerPixel, iQuality);   
      
   return result;
} 

void CLead::AboutBox()
{
   InvokeHelper(0xFFFFFDD8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short CLead::TwainEnumSources(OLE_HANDLE hWnd)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidTwainEnumSources, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hWnd);
   return result;
} 

short CLead::TwainRealize(OLE_HANDLE hWnd)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidTwainRealize, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hWnd);
   return result;
} 

BOOL CLead::DeleteRgnHandle(OLE_HANDLE hRgn)
{
   BOOL result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE  ;

   InvokeHelper(dispidDeleteRgnHandle, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      hRgn);
   return result;
}

short CLead::FreeRgn()
{
   short result;

   InvokeHelper(dispidFreeRgn, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   return result;
}

long CLead::GetRgnArea()
{
   long result;

   InvokeHelper(dispidGetRgnArea, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
   return result;
}

OLE_HANDLE CLead::GetRgnHandle()
{
   OLE_HANDLE result;

   InvokeHelper(dispidGetRgnHandle, DISPATCH_METHOD, VT_HANDLE, (void*)&result, NULL);
   return result;
}

BOOL CLead::IsPtInRgn(float fx, float fy)
{
   BOOL result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 ;

   InvokeHelper(dispidIsPtInRgn, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      fx, fy);
   return result;
}

short CLead::OffsetRgn(float fDx, float fDy)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 ;

   InvokeHelper(dispidOffsetRgn, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fDx, fDy);
   return result;
}

short CLead::SetRgnColor(OLE_COLOR crMatch, short iCombineMode)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_I4 VTS_I2 ;

   InvokeHelper(dispidSetRgnColor, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      crMatch, iCombineMode);
   return result;
}

short CLead::SetRgnHandle(OLE_HANDLE hRgn, float fDx, float fDy, short iCombineMode)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_HANDLE VTS_R4 VTS_R4 VTS_I2 ;

   InvokeHelper(dispidSetRgnHandle, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hRgn, fDx, fDy, iCombineMode);
   return result;
}

short CLead::SetRgnEllipse(float fLeft, float fTop, float fWidth, float fHeight, short iCombineMode)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I2 ;

   InvokeHelper(dispidSetRgnEllipse, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight, iCombineMode );
   return result;
}

short CLead::SetRgnRect(float fLeft, float fTop, float fWidth, float fHeight, short iCombineMode)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I2 ;

   InvokeHelper(dispidSetRgnRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight, iCombineMode );
   return result;
}

short CLead::SetRgnRoundRect(float fLeft, float fTop, float fWidth, float fHeight, float fWidthEllipse, float fHeightEllipse, short iCombineMode)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I2 ;

   InvokeHelper(dispidSetRgnRoundRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight, iCombineMode );
   return result;
}

BOOL CLead::IsPtInFloater(float fx, float fy)
{
   BOOL result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 ;

   InvokeHelper(dispidIsPtInFloater, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      fx, fy);
   return result;
}

OLE_HANDLE CLead::GetFloaterHandle()
{
   OLE_HANDLE result;

   InvokeHelper(dispidGetFloaterHandle, DISPATCH_METHOD, VT_HANDLE, (void*)&result, NULL);
   return result;
}

short CLead::SetFloaterDstRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4 ;

   InvokeHelper(dispidSetFloaterDstRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
            fLeft, fTop, fWidth, fHeight );
   return result;
}

short CLead::SetFloaterDstClipRect(float fLeft, float fTop, float fWidth, float fHeight)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4 ;

   InvokeHelper(dispidSetFloaterDstClipRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
            fLeft, fTop, fWidth, fHeight );
   return result;
}

float CLead::GetPolygonX(short iIndex)                // CLead Property implemented via Methods
{
   float result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidPolygonX, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetPolygonX(short iIndex, float newValue) // PolygonX Property implemented via Methods
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_R4  ;

   InvokeHelper(dispidPolygonX, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, newValue);
}

float CLead::GetPolygonY(short iIndex)                // CLead Property implemented via Methods
{
   float result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2  ;

   InvokeHelper(dispidPolygonY, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetPolygonY(short iIndex, float newValue) // PolygonX Property implemented via Methods
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_R4  ;

   InvokeHelper(dispidPolygonY, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, newValue);
}

short CLead::SetRgnPolygon(short iFillMode, short iCombineMode)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I2 ;

   InvokeHelper(dispidSetRgnPolygon, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFillMode, iCombineMode);
   return result;
}

short CLead::RepaintRect(float fLeft, float fTop, float fWidth, float fHeight, BOOL bErase)
{
   short result;
   static BYTE BASED_CODE parms[] = 
      VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BOOL;

   InvokeHelper(dispidRepaintRect, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fLeft, fTop, fWidth, fHeight, bErase);
   return result;
}


CString CLead::GetTwainSourceList(short iIndex)
{
   CString result;
   static BYTE BASED_CODE parms[] = 
      VTS_I2 ;

   InvokeHelper(dispidTwainSourceList, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      iIndex);
   return result;
} 

short CLead::AnnSetTag(OLE_HANDLE hObject, long iTag)
{
   short result;
   static BYTE parms[] =
      VTS_HANDLE VTS_I4;
   InvokeHelper(dispidAnnSetTag, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject, iTag);
   return result;
}

long CLead::AnnGetTag(OLE_HANDLE hObject)
{
   long result;
   static BYTE parms[] =
      VTS_HANDLE;
   InvokeHelper(dispidAnnGetTag, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      hObject);
   return result;
}

short CLead::AnnGetType(OLE_HANDLE hObject)
{
   short result;
   static BYTE parms[] =
      VTS_HANDLE;
   InvokeHelper(dispidAnnGetType, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject);
   return result;
}

short CLead::AnnSave(LPCTSTR pszFile, short iFormat, BOOL fSelected)
{
   short result;
   static BYTE parms[] =
      VTS_BSTR VTS_I2 VTS_BOOL;
   InvokeHelper(dispidAnnSave, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszFile, iFormat, fSelected);
   return result;
}

short CLead::AnnLoad(LPCTSTR pszFile)
{
   short result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(dispidAnnLoad, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszFile);
   return result;
}

short CLead::AnnCopy(short iFormat, BOOL fSelected, BOOL fEmpty)
{
   short result;
   static BYTE parms[] =
      VTS_I2 VTS_BOOL VTS_BOOL;
   InvokeHelper(dispidAnnCopy, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFormat, fSelected, fEmpty);
   return result;
}

short CLead::AnnPaste()
{
   short result;
   InvokeHelper(dispidAnnPaste, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   return result;
}

short CLead::AnnRealize(BOOL fRedactOnly)
{
   short result;
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(dispidAnnRealize, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fRedactOnly);
   return result;
}

short CLead::AnnSetAutoText(short iItem, LPCTSTR pszText)
{
   short result;
   static BYTE parms[] =
      VTS_I2 VTS_BSTR;
   InvokeHelper(dispidAnnSetAutoText, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iItem, pszText);
   return result;
}

CString CLead::AnnGetAutoText(short iItem)
{
   CString  result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidAnnGetAutoText, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      iItem);
   return result;
}

short CLead::ColorResList(short iBitsPerPixel, short iPalette, short iDither, short iColors)
{
   short  result;
   static BYTE parms[] =
      VTS_I2 VTS_I2 VTS_I2 VTS_I2 ;
   InvokeHelper(dispidColorResList, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iBitsPerPixel, iPalette, iDither, iColors);
   return result;
}

long CLead::GetColorCount()
{
   long  result;
   InvokeHelper(dispidGetColorCount, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
   return result;
}

short CLead::InsertBitmapListItem(short iIndex, OLE_HANDLE hBitmap)
{
   short  result;
   static BYTE parms[] =
      VTS_I2 VTS_HANDLE;
   InvokeHelper(dispidInsertBitmapListItem, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iIndex, hBitmap);
   return result;
}

short CLead::EnumDimensions(LPCTSTR pszFile)
{
   short  result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(dispidEnumDimensions, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszFile);
   return result;
}

short CLead::SetDimension(short iFormat, float fWidth, float fHeight)
{
   short  result;
   static BYTE parms[] =
      VTS_I2 VTS_R4 VTS_R4;
   InvokeHelper(dispidSetDimension, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFormat, fWidth, fHeight);
   return result;
}

short CLead::GetDimension(short iFormat)
{
   short  result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidGetDimension, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFormat);
   return result;
}

long CLead::AnnGetPointCount(OLE_HANDLE hObject)
{
   long result;
   static BYTE parms[] =
      VTS_HANDLE;
   InvokeHelper(dispidAnnGetPointCount, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      hObject);
   return result;
}

VARIANT CLead::AnnGetPointX(OLE_HANDLE hObject)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_HANDLE;
   InvokeHelper(dispidAnnGetPointX, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      hObject);
   return result;
}

VARIANT CLead::AnnGetPointY(OLE_HANDLE hObject)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_HANDLE;
   InvokeHelper(dispidAnnGetPointY, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      hObject);
   return result;
}

short CLead::AnnGetSelectCount()
{
   short result;
   InvokeHelper(dispidAnnGetSelectCount, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
   return result;
}

VARIANT CLead::AnnGetSelectList()
{
   VARIANT result;
   InvokeHelper(dispidAnnGetSelectList, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
   return result;
}

short CLead::FastRotate(short iAngle)
{
   short result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidFastRotate, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iAngle);
   return result;
}

short CLead::AnnMove(float dxOffset, float dyOffset, BOOL bSelected)
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_BOOL;
   InvokeHelper(dispidAnnMove, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      dxOffset, dyOffset, bSelected);
   return result;
}

short CLead::DrawShape(short uShape, OLE_HANDLE BackgroundImage)
{
   short result;
   static BYTE parms[] =
      VTS_I2 VTS_HANDLE;
   InvokeHelper(dispidDrawShape, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      uShape, BackgroundImage);
   return result;
}

short CLead::DrawText(LPCTSTR szText, OLE_HANDLE BackgroundImage)
{
   short result;
   static BYTE parms[] =
      VTS_BSTR VTS_HANDLE;
   InvokeHelper(dispidDrawText, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      szText, BackgroundImage);
   return result;
}

/* Properties with parameters */

OLE_COLOR CLead::GetAnimationPalette(short iIndex)
{
   OLE_COLOR  result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidAnimationPalette, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iIndex);
   return result;
}

void CLead::SetAnimationPalette(short iIndex, OLE_COLOR nNewValue)
{
   static BYTE parms[] =
      VTS_I2 VTS_I4;
   InvokeHelper(dispidAnimationPalette, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, nNewValue);
}

float CLead::GetDimensionWidth(short iIndex)
{
   float  result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidDimensionWidth, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
      iIndex);
   return result;
}

float CLead::GetDimensionHeight(short iIndex)
{
   float  result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidDimensionHeight, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
      iIndex);
   return result;
}

VARIANT CLead::GetComment(short iIndex)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(dispidComment, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
      iIndex);
   return result;
}

void CLead::SetComment(short iIndex, const VARIANT& newValue)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(dispidComment, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       iIndex, &newValue);
}

OLE_COLOR CLead::GetPixel(float x, float y)
{
   OLE_COLOR result;
   static BYTE parms[] =
      VTS_R4 VTS_R4;
   InvokeHelper(dispidPixel, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
      x, y);
   return result;
}

void CLead::SetPixel(float x, float y, OLE_COLOR nNewValue)
{
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_I4;
   InvokeHelper(dispidPixel, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       x, y, nNewValue);
}

void CLead::Delete(BOOL bAutoDelete)
{

   Release();      

   if (bAutoDelete)
   {
      InternalRelease();  
   }
}

void CLead::Release(OLECLOSE dwCloseOption)
{

   m_scLast = S_OK;

   if (m_pIObject)
   {
      if (m_pIViewObject2)
      {
         DWORD dwAspect;
         VERIFY(m_pIViewObject2->GetAdvise(&dwAspect, NULL, NULL) == NOERROR);
         VERIFY(m_pIViewObject2->SetAdvise(dwAspect, 0, NULL) == NOERROR);
         RELEASE(m_pIViewObject2);
      }

      if (m_dwConnection != 0)
      {
         VERIFY(m_pIObject->Unadvise(m_dwConnection) == NOERROR);
         m_dwConnection = 0;
      }
        
      RELEASE(m_pIOleControl);
      RELEASE(m_pIDispatchControl);
      RELEASE(m_pIOleInPlaceObject);
      RELEASE(m_pIOleInPlaceActiveObject);
         
      if (m_pEventsRedirDispatch)
      {                         
         InterfaceDisconnect(m_pIObject, m_pEventsRedirDispatch->GetAliasIID(), &m_dwEventConnProp);
         delete m_pEventsRedirDispatch;
         m_pEventsRedirDispatch = NULL;
      }
      m_DispatchDriver.ReleaseDispatch();

      if (m_pAmbientsDispatch)
      {
         RELEASE(m_pAmbientsDispatch);
         m_pLeadContainer->RemoveControl(this);
      }

      m_scLast = GetScode(m_pIObject->Close(dwCloseOption));
      RELEASE(m_pIObject);
      RELEASE(m_pIPersistStreamInit);
         
      m_hWnd = NULL;
         
      m_nState = emptyState;      
   }
}

void CLead::Close(OLECLOSE dwCloseOption)
{

   if (m_bInClose)
      return;

   m_bInClose = TRUE;

   m_scLast = GetScode(m_pIObject->Close(dwCloseOption));


   if (m_nState != loadedState)
   {
      if (m_nState == activeUIState)
         OnDeactivateUI(FALSE);

      if (m_nState == activeState)
         OnDeactivate();

      if (m_nState != loadedState)
      {
         OnChange(OLE_CHANGED_STATE, (DWORD)loadedState);
         m_nState = loadedState; 
      }
   }

   m_bInClose = FALSE; 
}

void CLead::Connect(HRESULT hr)
{
#if IS_MFC40
    USES_CONVERSION;
#endif

   TRY 
   {
      CheckGeneral(hr);

      LPADVISESINK lpAdviseSink = (LPADVISESINK)GetInterface(&IID_IAdviseSink);
      CheckGeneral(m_pIObject->Advise(lpAdviseSink, &m_dwConnection));

      CheckGeneral(m_pIObject->QueryInterface(IID_IOleControl, (LPLP)&m_pIOleControl));
      CheckGeneral(m_pIObject->QueryInterface(IID_IOleInPlaceObject, (LPLP)&m_pIOleInPlaceObject));
      CheckGeneral(m_pIObject->QueryInterface(IID_IOleInPlaceActiveObject, (LPLP)&m_pIOleInPlaceActiveObject));
      CheckGeneral(m_pIObject->QueryInterface(IID_IPersistStreamInit, (LPLP)&m_pIPersistStreamInit));
      CheckGeneral(m_pIObject->QueryInterface(IID_IViewObject2, (LPLP)&m_pIViewObject2));
      CheckGeneral(m_pIObject->QueryInterface(IID_IDispatch,(LPLP)&m_pIDispatchControl));

      m_DispatchDriver.AttachDispatch(m_pIDispatchControl);
      m_pIDispatchControl->AddRef();

      CheckGeneral(m_pIViewObject2->SetAdvise(DVASPECT_CONTENT, 0, lpAdviseSink));
         
#if IS_MFC40
      m_pIObject->SetHostNames(T2COLE(AfxGetAppName()), L"");
#else
      m_pIObject->SetHostNames(AfxGetAppName(), _T(""));
#endif

      
      CheckGeneral(m_pIObject->SetClientSite(GetClientSite()));

      ::OleSetContainedObject(m_pIObject, TRUE);
        
      m_nState = loadedState;

      CheckGeneral(m_pIObject->GetMiscStatus(DVASPECT_CONTENT, &m_dwStatus));
      CheckGeneral(m_pIOleControl->GetControlInfo(&m_cntrlInfo));
        
   }
   CATCH_ALL(e)
   {
      Release();
      THROW_LAST();
   }
   END_CATCH_ALL

}

void CLead::ConnectEvents(CCmdTarget *pEventsCmdTarget, AFX_DISPMAP FAR *pDispMap)
{

    HRESULT             hr;
    LPTYPEATTR          pTA;               
    LPTYPEINFO         pTypeInfo;
    IID               eventsIID;

    eventsIID=CLSID_NULL;

    CheckGeneral(ObjectTypeInfoEvents(m_pIObject, &pTypeInfo));

    if (!SUCCEEDED(hr = pTypeInfo->GetTypeAttr(&pTA)))
    {
       pTypeInfo->Release();
      Release();
      AfxThrowOleException(hr);
    }

    eventsIID=pTA->guid;
    m_pEventsRedirDispatch = new COleDispatchRedir( this, pEventsCmdTarget, pDispMap, eventsIID);

    hr = InterfaceConnect(m_pIObject, eventsIID, m_pEventsRedirDispatch, &m_dwEventConnProp);
    pTypeInfo->ReleaseTypeAttr(pTA);
    pTypeInfo->Release();
   
    if (!SUCCEEDED(hr))           
    {
      delete m_pEventsRedirDispatch;
      m_pEventsRedirDispatch = NULL;

      Release();
      AfxThrowOleException(hr);
    }
}


BSTR CLead::GetLicenseKey() const
{
   return SysAllocString(_szLicString);
}


void CLead::CreateInstance(CLeadContainer *pContainer, CWnd *pWnd, int ctrlID)
{
   CWnd   *pWndCtrl;
   WINDOWPLACEMENT   wp;

   if (m_hWnd)
      DestroyWindow();   
   
   pWndCtrl = CWnd::FromHandle(::GetDlgItem(pWnd->m_hWnd, ctrlID));
   
   wp.length = sizeof(wp);
   pWndCtrl->GetWindowPlacement(&wp);
   pWndCtrl->DestroyWindow();
   
   CreateInstance(pContainer, pWnd, wp.rcNormalPosition);

}


void CLead::CreateInstance(CLeadContainer *pContainer, CWnd *pWnd, const RECT &rect)
{

   if (m_hWnd)       
      Release();
   
   m_hWndParent = pWnd->m_hWnd;
   m_rcClient = rect;

   ::SetWindowLong(pWnd->m_hWnd, GWL_STYLE, pWnd->GetStyle()|WS_CLIPCHILDREN);

   if (m_pAmbientsDispatch)
   {
      m_pLeadContainer->RemoveControl(this);
      RELEASE(m_pAmbientsDispatch);               
   }

   m_pAmbientsDispatch = (LPDISPATCH)pContainer->GetInterface(&IID_IDispatch);
    
   m_pAmbientsDispatch->AddRef();
   m_pLeadContainer = pContainer;
   m_pLeadContainer->AddControl(this);
    
   if (m_pIObject)
      CheckGeneral(m_pIObject->SetClientSite(GetClientSite()));

   
   REFCLSID   refClassIID = GetClassID(); 
   BSTR      bstrKey = GetLicenseKey();
      
   if (bstrKey)
   {
      LPCLASSFACTORY2   pIClassFactory2;

      if (SUCCEEDED(::CoGetClassObject(refClassIID, CLSCTX_INPROC_SERVER, NULL, IID_IClassFactory2, (LPLP)&pIClassFactory2)))
      {
         HRESULT hr;
         hr = pIClassFactory2->CreateInstanceLic(NULL, NULL, IID_IOleObject, bstrKey, (LPLP)&m_pIObject);
         RELEASE(pIClassFactory2);
         ::SysFreeString(bstrKey);
         Connect(hr);
      }
      else
         Connect(::CoCreateInstance(refClassIID, NULL, CLSCTX_INPROC_SERVER, IID_IOleObject, (LPLP)&m_pIObject));
   }
   else
      Connect(::CoCreateInstance(refClassIID, NULL, CLSCTX_INPROC_SERVER, IID_IOleObject, (LPLP)&m_pIObject));

   if (m_pIPersistStreamInit)
      m_pIPersistStreamInit->InitNew();

   Activate();
}


BOOL CLead::IsInPlaceActive() const
{
   return (activeUIState == m_nState) || (activeState == m_nState);
}


void CLead::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
}

void CLead::OnDataChange(
   LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
}

void CLead::OnShowItem()
{
}

BOOL CLead::OnChangeItemPosition(const CRect& rectPos)
{
   return TRUE;
}

void CLead::OnActivate()
{
}

void CLead::OnActivateUI()
{
}

void CLead::OnDeactivate()
{

}

void CLead::OnDeactivateUI(BOOL bUndoable)
{
}

void CLead::OnPropertyChanged(DISPID dispid)
{

}

BOOL CLead::OnPropertyRequestEdit(DISPID dispid)
{
   return TRUE;
}

void CLead::DoProperties()
{                      
   m_pIObject->DoVerb(OLEIVERB_PROPERTIES,NULL, GetClientSite(), 0 , m_hWnd, NULL);
}

void CLead::Deactivate()
{
   if (!m_uLockInPlace)
   {
      m_scLast = GetScode(m_pIOleInPlaceObject->InPlaceDeactivate());
      m_nState = loadedState;
   }
   else
   {
      m_bPendingDeactivate = TRUE;
      m_scLast = GetScode(m_pIOleInPlaceObject->UIDeactivate());
      m_nState = activeState; 
   }

}

void CLead::DeactivateUI()
{

   if (m_pIOleInPlaceObject)
      m_scLast = GetScode(m_pIOleInPlaceObject->UIDeactivate());
   m_nState = activeState; 
}                     

void CLead::Activate()
{
   if (m_nState != activeState)
   {
      CRect   activeRect;

      m_pIObject->DoVerb(OLEIVERB_SHOW,NULL, GetClientSite(), 0 , m_hWndParent, m_rcClient);
      SetExtent();
   }
}


void CLead::ActivateUI()
{
   if (m_nState != activeUIState)
   {
      m_pIObject->DoVerb(OLEIVERB_UIACTIVATE,NULL, GetClientSite(), 0 , m_hWndParent, m_rcClient);
      SetExtent();
   }
}

LPOLECLIENTSITE CLead::GetClientSite()
{

   LPOLECLIENTSITE lpClientSite =
      (LPOLECLIENTSITE)GetInterface(&IID_IOleClientSite);
   return lpClientSite;
}


void CLead::CheckGeneral(HRESULT hr)
   
{

   m_scLast = S_OK;    

   if (hr != NOERROR)
   {
      m_scLast = GetScode(hr);
      if (!FAILED(m_scLast))
         return;
      AfxThrowOleException(hr);
   }
}


void CLead::InvokeHelper(DISPID dwDispID, WORD wFlags,
   VARTYPE vtRet, void* pvRet, const BYTE FAR* pbParamInfo, ...)
{

   va_list argList;
   va_start(argList, pbParamInfo);

   m_DispatchDriver.InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, argList);

   va_end(argList);
}

void CLead::GetProperty(DISPID dwDispID, VARTYPE vtProp,
   void* pvProp) const
{

   ((COleDispatchDriver*)&m_DispatchDriver)->InvokeHelper(dwDispID,
      DISPATCH_PROPERTYGET, vtProp, pvProp, NULL);
}

void CLead::SetProperty(DISPID dwDispID, VARTYPE vtProp, ...)
{

   va_list argList;
   va_start(argList, vtProp);

   BYTE rgbparms[2];
   if (vtProp & VT_BYREF)
   {
      vtProp &= ~VT_BYREF;
      vtProp |= VT_MFCBYREF;
   }
   rgbparms[0] = (BYTE)vtProp;
   rgbparms[1] = 0;
   m_DispatchDriver.InvokeHelperV(dwDispID, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL,
      rgbparms, argList);

   va_end(argList);
}

void CLead::OnDestroy() 
{ 
   
   Release();
   CWnd::OnDestroy();
}

void CLead::OnPaint() 
{
   if (!m_pIObject)
      return;   

   CPaintDC dc(this); 

   RECTL rclBounds;
   CRect   clientRect, hiMetricRect;
   CRect   oldViewportRect, oldWindowRect;

   GetClientRect(clientRect);

   SetDCToDrawInHimetricRect(dc.m_hDC, clientRect,
      hiMetricRect, oldWindowRect, oldViewportRect);

   rclBounds.left = hiMetricRect.left;
   rclBounds.top = hiMetricRect.top;
   rclBounds.right = hiMetricRect.right;
   rclBounds.bottom = hiMetricRect.bottom;

   HRESULT hr = m_pIViewObject2->Draw(DVASPECT_CONTENT, -1, NULL,
         NULL, NULL, dc.m_hDC,
         &rclBounds, NULL, NULL, 0);

   if (hr != NOERROR && GetScode(hr) == OLE_E_BLANK)
      return;

   CheckGeneral(hr);
}


void CLead::SetExtent()
{                                                 
   
   SIZEL   pixelsSizel, hiMetricSizel;
   
   pixelsSizel.cx = m_rcClient.Width();
   pixelsSizel.cy = m_rcClient.Height();
   XformSizeInPixelsToHimetric(NULL, &pixelsSizel, &hiMetricSizel); 

   m_pIObject->SetExtent(DVASPECT_CONTENT, &hiMetricSizel);
}


void CLead::MoveControl(LPCRECT lpRect) 
{

   SIZEL   pixelsSizel, hiMetricSizel;

   m_rcClient = lpRect;
   m_pIOleInPlaceObject->SetObjectRects(lpRect, lpRect);
   
   pixelsSizel.cx = m_rcClient.Width();
   pixelsSizel.cy = m_rcClient.Height();
   XformSizeInPixelsToHimetric(NULL, &pixelsSizel, &hiMetricSizel); 
   m_pIObject->SetExtent(DVASPECT_CONTENT, &hiMetricSizel);
   
}


void CLead::DrawMetafile(CDC* pDC, const CRect& rcBounds)
{

   RECTL rclBounds;
   double   dblXFactor, dblYFactor;
   CRect   parentClientRect;
   CWnd   *pParent;

   
   pParent = GetParent();
   pParent->GetClientRect(parentClientRect);
   
   dblXFactor = rcBounds.Width() / parentClientRect.Width();
   dblYFactor = rcBounds.Height() / parentClientRect.Height();
   
   rclBounds.left = (long)(m_rcClient.left * dblXFactor);
   rclBounds.top = (long)(m_rcClient.top  * dblYFactor);
   rclBounds.right = (long)(m_rcClient.right * dblXFactor);
   rclBounds.bottom = (long)(m_rcClient.bottom * dblYFactor);

   RECTL rclWBounds;
   CPoint ptOrg = pDC->GetWindowOrg();
   CSize sizeExt = pDC->GetWindowExt();
   rclWBounds.left = ptOrg.x;
   rclWBounds.top = ptOrg.y;
   rclWBounds.right = sizeExt.cx;
   rclWBounds.bottom = sizeExt.cy;

   HRESULT hr;
   hr = m_pIViewObject2->Draw(DVASPECT_CONTENT, -1, NULL,
      NULL, NULL, pDC->m_hDC,
      &rclBounds, &rclWBounds, NULL, 0);
}

BEGIN_INTERFACE_MAP(CLead, CWnd)
   INTERFACE_PART(CLead, IID_IOleClientSite, OleClientSite)
   INTERFACE_PART(CLead, IID_IAdviseSink, AdviseSink)
   INTERFACE_PART(CLead, IID_IOleWindow, OleIPSite)
   INTERFACE_PART(CLead, IID_IOleInPlaceSite, OleIPSite)
   INTERFACE_PART(CLead, IID_IOleControlSite, OleControlSite)
   INTERFACE_PART(CLead, IID_IPropertyNotifySink, PropertyNotifySink)
   INTERFACE_PART(CLead, IID_IDispatch, Dispatch)
END_INTERFACE_MAP()


STDMETHODIMP_(ULONG) CLead::XOleClientSite::AddRef()
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)
   return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CLead::XOleClientSite::Release()
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)
   return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CLead::XOleClientSite::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)
   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CLead::XOleClientSite::SaveObject()
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)
   return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CLead::XOleClientSite::GetMoniker(
   DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppMoniker)
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)

   return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CLead::XOleClientSite::GetContainer(
   LPOLECONTAINER FAR* ppContainer)
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)

   *ppContainer = NULL;
   return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP CLead::XOleClientSite::ShowObject()
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)

   TRY
   {
      HRESULT hr;
      hr = pThis->m_pIOleInPlaceObject->GetWindow(&pThis->m_hWnd);
   
      pThis->OnShowItem();
   }
   END_TRY


   return NOERROR;
}

STDMETHODIMP CLead::XOleClientSite::OnShowWindow(BOOL fShow)
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)

   if (pThis->IsInPlaceActive())
      return NOERROR;

   TRY
   {
      CLead::State uNewState;
      uNewState = fShow ? CLead::openState :
         CLead::loadedState;
      if (uNewState != pThis->m_nState)
      {
         pThis->OnChange(OLE_CHANGED_STATE, (DWORD)uNewState);
         pThis->m_nState = uNewState;
      }
   }
   END_TRY


   return NOERROR;
}

STDMETHODIMP CLead::XOleClientSite::RequestNewObjectLayout()
{
   METHOD_MANAGE_STATE(CLead, OleClientSite)

   SIZEL   pixelsSizel, hiMetricSizel;
    
   pThis->m_pIObject->GetExtent(DVASPECT_CONTENT, &hiMetricSizel);
    
   XformSizeInHimetricToPixels(NULL, &pixelsSizel, &hiMetricSizel); 

   return NOERROR;
}

STDMETHODIMP_(ULONG) CLead::XAdviseSink::AddRef()
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)

   return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CLead::XAdviseSink::Release()
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)
   return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CLead::XAdviseSink::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)
   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(void) CLead::XAdviseSink::OnDataChange(
   LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)


}

STDMETHODIMP_(void) CLead::XAdviseSink::OnViewChange(
   DWORD aspects, LONG lindex)
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)

   pThis->OnChange(OLE_CHANGED, (DVASPECT)aspects);
}

STDMETHODIMP_(void) CLead::XAdviseSink::OnRename(LPMONIKER lpMoniker)
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)
}

STDMETHODIMP_(void) CLead::XAdviseSink::OnSave()
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)

   pThis->OnChange(OLE_SAVED, (DVASPECT)0);
}

STDMETHODIMP_(void) CLead::XAdviseSink::OnClose()
{
   METHOD_MANAGE_STATE(CLead, AdviseSink)

   pThis->OnChange(OLE_CLOSED, (DVASPECT)0);
}


STDMETHODIMP_(ULONG) CLead::XOleIPSite::AddRef()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)
   return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CLead::XOleIPSite::Release()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)
   return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CLead::XOleIPSite::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)
   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CLead::XOleIPSite::GetWindow(HWND FAR* lphwnd)
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   *lphwnd = pThis->m_hWndParent;
   
   return *lphwnd ? NOERROR : ResultFromScode(E_FAIL);
}

STDMETHODIMP CLead::XOleIPSite::ContextSensitiveHelp(
   BOOL fEnterMode)
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CLead::XOleIPSite::CanInPlaceActivate()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)


   return S_OK;
}

STDMETHODIMP CLead::XOleIPSite::OnInPlaceActivate()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   SCODE sc = E_UNEXPECTED;
   TRY
   {
      pThis->m_bPendingDeactivate = FALSE;

      
      if (pThis->m_nState != activeState)
      {
         pThis->OnChange(OLE_CHANGED_STATE, (DWORD)activeState);
         pThis->m_nState = activeState;
      }
      pThis->OnActivate();    

      sc = S_OK;
   }
   END_TRY

   return ResultFromScode(sc);
}

STDMETHODIMP CLead::XOleIPSite::OnUIActivate()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   SCODE sc = E_UNEXPECTED;
   TRY
   {
      if (pThis->m_nState != activeUIState)
      {
         pThis->OnChange(OLE_CHANGED_STATE, (DWORD)activeUIState);
         pThis->m_nState = activeUIState;
      }
      pThis->OnActivateUI();

      pThis->m_pLeadContainer->OnSetUIActiveObject(pThis);

      sc = S_OK;
   }
   END_TRY

   return ResultFromScode(sc);
}                              


STDMETHODIMP CLead::XOleIPSite::OnUIDeactivate(BOOL fUndoable)
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   SCODE sc = E_UNEXPECTED;
   TRY
   {   
      if (pThis->m_nState != activeState)
      {
         pThis->OnChange(OLE_CHANGED_STATE, (DWORD)activeState);
         pThis->m_nState = activeState;
      }
      pThis->OnDeactivateUI(fUndoable);
      
      sc = S_OK;
   }
   END_TRY

   return ResultFromScode(sc);
}

STDMETHODIMP CLead::XOleIPSite::OnInPlaceDeactivate()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   SCODE sc = E_UNEXPECTED;
   TRY
   {
      pThis->m_hWnd = NULL;
      pThis->m_hWndParent = NULL;
      
      pThis->m_bPendingDeactivate = FALSE;
   
      if (pThis->m_nState != loadedState)
      {
         pThis->OnChange(OLE_CHANGED_STATE, (DWORD)loadedState);
         pThis->m_nState = loadedState;
      }

      pThis->OnDeactivate();

      sc = S_OK;
   }
   END_TRY

   return ResultFromScode(sc);
}


STDMETHODIMP CLead::XOleIPSite::GetWindowContext(
   LPOLEINPLACEFRAME FAR* lplpFrame,
   LPOLEINPLACEUIWINDOW FAR* lplpDoc,
   LPRECT lpPosRect,   LPRECT lpClipRect,
   LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   *lplpFrame = NULL;  
   *lplpDoc = NULL;

   SCODE sc = E_UNEXPECTED;
   TRY
   {
      pThis->m_pLeadContainer->GetWindowContext(pThis, lplpFrame, lplpDoc, lpPosRect, lpClipRect, lpFrameInfo);
      sc = S_OK;
   }
   END_TRY


   return ResultFromScode(sc);
}

STDMETHODIMP CLead::XOleIPSite::Scroll(SIZE scrollExtent)
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CLead::XOleIPSite::DiscardUndoState()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CLead::XOleIPSite::DeactivateAndUndo()
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)

   return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CLead::XOleIPSite::OnPosRectChange(
   LPCRECT lpPosRect)
{
   METHOD_MANAGE_STATE(CLead, OleIPSite)


   SCODE sc = E_UNEXPECTED;
   TRY
   {
      if (!pThis->OnChangeItemPosition(lpPosRect))      
      {                  
         SIZEL   pixelsSizel, hiMetricSizel;
      
         pThis->m_rcClient = lpPosRect;
         
         pThis->m_pIOleInPlaceObject->SetObjectRects(lpPosRect, lpPosRect);
         
         pixelsSizel.cx = pThis->m_rcClient.Width();
         pixelsSizel.cy = pThis->m_rcClient.Height();
         XformSizeInPixelsToHimetric(NULL, &pixelsSizel, &hiMetricSizel); 
         pThis->m_pIObject->SetExtent(DVASPECT_CONTENT, &hiMetricSizel);
      
      }
      sc = S_OK;
   }
   END_TRY



   return ResultFromScode(sc);
}


STDMETHODIMP_(ULONG) CLead::XOleControlSite::AddRef()
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)

   return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CLead::XOleControlSite::Release()
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)

   return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CLead::XOleControlSite::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)

   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP CLead::XOleControlSite::OnControlInfoChanged()
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)

   VERIFY(SUCCEEDED(pThis->m_pIOleControl->GetControlInfo(&pThis->m_cntrlInfo)));

   return NOERROR;
}


STDMETHODIMP CLead::XOleControlSite::LockInPlaceActive(
   BOOL fLock)
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)


   if (fLock)
      pThis->m_uLockInPlace++;
   else                  
   if ( (0 == --pThis->m_uLockInPlace) && pThis->m_bPendingDeactivate )
      pThis->Deactivate();
   
   return NOERROR;
}

STDMETHODIMP CLead::XOleControlSite::GetExtendedControl(
   LPDISPATCH FAR* ppDisp)
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)

   return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CLead::XOleControlSite::TransformCoords(
   POINTL FAR* lpptlHimetric, POINTF FAR* lpptfContainer, DWORD flags) 
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)

    
    if (XFORMCOORDS_HIMETRICTOCONTAINER & flags)
    {
      lpptfContainer->x = (float)XformWidthInHimetricToPixels(NULL, (int)lpptlHimetric->x);
      lpptfContainer->y = (float)XformHeightInHimetricToPixels(NULL, (int)lpptlHimetric->y);
   }
   else
   {
      lpptlHimetric->x = XformWidthInPixelsToHimetric(NULL, (int)lpptfContainer->x);
      lpptlHimetric->y = XformHeightInPixelsToHimetric(NULL, (int)lpptfContainer->y);
   }

   return NOERROR;
}

STDMETHODIMP CLead::XOleControlSite::TranslateAccelerator(
   LPMSG lpMsg, DWORD grfModifiers) 
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)


   if (pThis->m_pLeadContainer)
      return pThis->m_pLeadContainer->OnTranslateAccelerator(lpMsg);

   return ResultFromScode(S_FALSE);
}

STDMETHODIMP CLead::XOleControlSite::OnFocus(
   BOOL fGotFocus)
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)


   if (pThis->m_pLeadContainer)
      pThis->m_pLeadContainer->OnFocus(pThis, fGotFocus);

   return NOERROR;
}

STDMETHODIMP CLead::XOleControlSite::ShowPropertyFrame()
{
   METHOD_MANAGE_STATE(CLead, OleControlSite)

   
   return ResultFromScode(E_NOTIMPL);
}
                
STDMETHODIMP_(ULONG) CLead::XPropertyNotifySink::AddRef()
{
   METHOD_MANAGE_STATE(CLead, PropertyNotifySink)

   return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CLead::XPropertyNotifySink::Release()
{
   METHOD_MANAGE_STATE(CLead, PropertyNotifySink)

   return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CLead::XPropertyNotifySink::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLead, PropertyNotifySink)

   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP CLead::XPropertyNotifySink::OnChanged(
   DISPID dispid)
{
   METHOD_MANAGE_STATE(CLead, PropertyNotifySink)


   pThis->OnPropertyChanged(dispid);

   return NOERROR;
}


STDMETHODIMP CLead::XPropertyNotifySink::OnRequestEdit(
   DISPID dispid)
{
   METHOD_MANAGE_STATE(CLead, PropertyNotifySink)


   if (pThis->OnPropertyRequestEdit(dispid))
      return NOERROR;

   return ResultFromScode(S_FALSE);
}
    
    

STDMETHODIMP_(ULONG) CLead::XDispatch::AddRef()
{
   METHOD_MANAGE_STATE(CLead, Dispatch)
   return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) CLead::XDispatch::Release()
{
   METHOD_MANAGE_STATE(CLead, Dispatch)
   return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP CLead::XDispatch::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLead, Dispatch)
   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP CLead::XDispatch::GetTypeInfoCount(unsigned int FAR* pctinfo)
{
   METHOD_MANAGE_STATE(CLead, Dispatch)

   *pctinfo = 1;
   return NOERROR;
}


STDMETHODIMP CLead::XDispatch::GetTypeInfo(
     unsigned int itinfo,
     LCID lcid,
     ITypeInfo FAR* FAR* pptinfo)
{
   METHOD_MANAGE_STATE(CLead, Dispatch)

    return ResultFromScode(E_NOTIMPL);
}


#ifdef IS_MFC40
STDMETHODIMP CLead::XDispatch::GetIDsOfNames(
     REFIID riid,
     LPOLESTR * rgszNames,
     unsigned int cNames,
     LCID lcid,
     DISPID FAR* rgdispid)
#else
STDMETHODIMP CLead::XDispatch::GetIDsOfNames(
     REFIID riid,
     LPTSTR FAR* rgszNames,
     unsigned int cNames,
     LCID lcid,
     DISPID FAR* rgdispid)
#endif
{
   METHOD_MANAGE_STATE(CLead, Dispatch)

    return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP CLead::XDispatch::Invoke(
     DISPID dispidMember,
     REFIID riid,
     LCID lcid,
     unsigned short wFlags,
     DISPPARAMS FAR* pdispparams,
     VARIANT FAR* pvarResult,
     EXCEPINFO FAR* pexcepinfo,
     unsigned int FAR* puArgErr)
{
   METHOD_MANAGE_STATE(CLead, Dispatch)

   if (DISPID_AMBIENT_DISPLAYASDEFAULT == dispidMember)
   {
      VARIANT varResult;
      
      if (NULL == pvarResult)
         pvarResult=&varResult;


      if (pThis->m_dwStatus && OLEMISC_ACTSLIKEBUTTON)
         V_BOOL(pvarResult) = (BOOL)pThis->m_bDisplayAsDefault;
      else
         V_BOOL(pvarResult) = FALSE;

      return NOERROR;
   }

   if (pThis->m_pAmbientsDispatch)
      return pThis->m_pAmbientsDispatch->Invoke(
            dispidMember,
            riid,
            lcid,
            wFlags,
            pdispparams,
            pvarResult,
            pexcepinfo,
            puArgErr);

    return ResultFromScode(E_NOTIMPL);

}

    
STDMETHODIMP_(ULONG) CLead::XSimpleFrameSite::AddRef()
{
   METHOD_MANAGE_STATE(CLead, SimpleFrameSite)

   return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) CLead::XSimpleFrameSite::Release()
{
   METHOD_MANAGE_STATE(CLead, SimpleFrameSite)
   return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP CLead::XSimpleFrameSite::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLead, SimpleFrameSite)

   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CLead::XSimpleFrameSite::PreMessageFilter(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
         LRESULT FAR* lplResult, DWORD FAR * lpdwCookie)
{
   METHOD_MANAGE_STATE(CLead, SimpleFrameSite)

   return pThis->m_pLeadContainer->PreMessageFilter(hwnd, msg, wp, lp, lplResult, lpdwCookie);
}


STDMETHODIMP CLead::XSimpleFrameSite::PostMessageFilter(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
         LRESULT FAR* lplResult, DWORD dwCookie)
{
   METHOD_MANAGE_STATE(CLead, SimpleFrameSite)

   return pThis->m_pLeadContainer->PostMessageFilter(hwnd, msg, wp, lp, lplResult, dwCookie);
}


class CCmdTargetAccess : public CCmdTarget
{
public:
   friend class COleDispatchRedir;
   
private:
   CCmdTargetAccess ();
};


COleDispatchRedir::COleDispatchRedir()
{
   m_pDispatchMap = NULL;
    m_pCmdTarget = NULL;
    m_pRedirCmdTarget = NULL;
}

COleDispatchRedir::COleDispatchRedir(CCmdTarget FAR *pCmdTarget, CCmdTarget FAR *pRedirCmdTarget, AFX_DISPMAP FAR *pDispatchMap, REFIID aliasIID)
{

   m_pDispatchMap = pDispatchMap;
    m_pCmdTarget = pCmdTarget;
    m_pRedirCmdTarget = pRedirCmdTarget;
    m_AliasIID = aliasIID;

   if (m_pCmdTarget)
      m_pCmdTarget->InternalAddRef();
}

COleDispatchRedir::~COleDispatchRedir()
{
   if (m_pCmdTarget)
      m_pCmdTarget->InternalRelease();
}


STDMETHODIMP COleDispatchRedir::QueryInterface(REFIID riid, LPVOID far* ppvObj)
{
    *ppvObj=NULL;

    if (IID_IUnknown==riid || IID_IDispatch==riid || m_AliasIID==riid)
    {
        *ppvObj=this;
        AddRef();
        return NOERROR;
   }
   else
   if (m_pCmdTarget)
      return (HRESULT)m_pCmdTarget->ExternalQueryInterface(&riid, ppvObj);
   
    return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP_(ULONG) COleDispatchRedir::AddRef(void)
{
   return   m_pCmdTarget->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleDispatchRedir::Release(void)
{
   return m_pCmdTarget->ExternalRelease();
}


STDMETHODIMP COleDispatchRedir::GetTypeInfoCount(UINT *pctInfo)
{
    *pctInfo=NULL;
    return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP COleDispatchRedir::GetTypeInfo(UINT itinfo
    , LCID lcid, ITypeInfo **pptInfo)
{
    *pptInfo=NULL;
    return ResultFromScode(E_NOTIMPL);
}

#if IS_MFC40
STDMETHODIMP COleDispatchRedir::GetIDsOfNames(REFIID riid
    , LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispID)
#else
STDMETHODIMP COleDispatchRedir::GetIDsOfNames(REFIID riid
    , TCHAR **rgszNames, UINT cNames, LCID lcid, DISPID *rgDispID)
#endif
{
    *rgszNames=NULL;
    *rgDispID=NULL;    
    
    
    return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleDispatchRedir::Invoke(DISPID dispIDMember, REFIID riid
    , LCID lcid, unsigned short wFlags, DISPPARAMS * pDispParams
    , VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{                 
   if (!(m_pRedirCmdTarget && m_pDispatchMap))
      return NOERROR;

   if (riid != IID_NULL)
      return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

   if (pDispParams->cNamedArgs != 0)
   {
      if (pDispParams->cNamedArgs != 1 ||
         pDispParams->rgdispidNamedArgs[0] != DISPID_PROPERTYPUT)
      {
         return ResultFromScode(DISP_E_NONAMEDARGS);
      }
   }
   if (wFlags & DISPATCH_PROPERTYPUTREF)
      return ResultFromScode(DISP_E_TYPEMISMATCH);

   AFX_DISPMAP_ENTRY FAR* pEntry = GetDispEntry(dispIDMember);
   if (!pEntry)
      return ResultFromScode(S_OK); 

   if (VT_VOID == pEntry->vt)
      pEntry->vt = VT_EMPTY;

   if ((wFlags & DISPATCH_METHOD) == DISPATCH_METHOD &&
      ((pEntry->pfn == NULL && pEntry->pfnSet == NULL) ||
       (pEntry->pfn != NULL && pEntry->pfnSet != NULL)))
   {
      wFlags &= ~DISPATCH_METHOD;

#ifndef _WIN32
      if (pDispParams->cArgs == (UINT)lstrlen((LPCTSTR)pEntry->pbParams))
         wFlags |= DISPATCH_PROPERTYGET;
      else
         wFlags |= DISPATCH_PROPERTYPUT;
#else
      if (pDispParams->cArgs == (UINT)lstrlen((LPCTSTR)pEntry->lpszParams))
         wFlags |= DISPATCH_PROPERTYGET;
      else
         wFlags |= DISPATCH_PROPERTYPUT;
#endif
   }

   if (wFlags & DISPATCH_PROPERTYGET)
   {
      if (pEntry->pfn == NULL && pDispParams->cArgs != 0)
         return ResultFromScode(DISP_E_BADPARAMCOUNT);
      if (pEntry->vt == VT_EMPTY)
         return ResultFromScode(DISP_E_BADPARAMCOUNT);
      if (!pVarResult)
         return ResultFromScode(DISP_E_PARAMNOTOPTIONAL);
   }
   if (wFlags & DISPATCH_PROPERTYPUT)
   {
      if (pEntry->pfn == NULL && pDispParams->cArgs > 1)
         return ResultFromScode(DISP_E_BADPARAMCOUNT);
   }

   UINT uArgErr = 0;  
   SCODE sc = S_OK;

   TRY
   {
      if (pEntry->pfn == NULL)
      {
         if (pDispParams->cArgs == 0)
            ((CCmdTargetAccess *)m_pRedirCmdTarget)->GetStandardProp(pEntry, pVarResult, &uArgErr);
         else
            ((CCmdTargetAccess *)m_pRedirCmdTarget)->SetStandardProp(pEntry, pDispParams, &uArgErr);
      }
      else
      {
#ifndef _WIN32
         sc = (SCODE)((CCmdTargetAccess *)m_pRedirCmdTarget)->InvokeHelper(pEntry, wFlags,
            pVarResult, pDispParams, &uArgErr);
#else
         sc = ((CCmdTargetAccess *)m_pRedirCmdTarget)->CallMemberFunc(pEntry, wFlags,
            pVarResult, pDispParams, &uArgErr);
#endif
      }
   }
   CATCH_ALL(e)
   {
      if (pExcepInfo)
      {
         COleDispatchException::Process(pExcepInfo, e);
      }
      return ResultFromScode(DISP_E_EXCEPTION);
   }
   END_CATCH_ALL

   if (sc != S_OK && puArgErr && uArgErr != -1)
      *puArgErr = uArgErr;

   return ResultFromScode(sc);

}

void COleDispatchRedir::SetDispatchMap(CCmdTarget FAR *pCmdTarget, CCmdTarget FAR *pRedirCmdTarget, AFX_DISPMAP FAR *pDispMap, REFIID aliasIID)
{                         
   if (m_pCmdTarget)
      m_pCmdTarget->InternalRelease();

    m_pDispatchMap = pDispMap;
    m_pCmdTarget = pCmdTarget;
    m_pRedirCmdTarget = pRedirCmdTarget;
    m_AliasIID = aliasIID;

    if (m_pCmdTarget)
       m_pCmdTarget->InternalAddRef();

}

AFX_DISPMAP_ENTRY FAR* COleDispatchRedir::GetDispEntry(MEMBERID memid)
{
   const AFX_DISPMAP FAR* pDispMap = m_pDispatchMap;
   const AFX_DISPMAP_ENTRY FAR* pEntry;

   while (pDispMap)
   {
      pEntry = pDispMap->lpEntries;
      while ((pEntry->lDispID != memid) && (pEntry->nPropOffset != -1))
         ++pEntry;
      
      if (memid == pEntry->lDispID)
         return (struct AFX_DISPMAP_ENTRY*)pEntry;

#ifndef _WIN32
      pDispMap = pDispMap->lpBaseDispMap;
#else
#ifndef _AFXDLL            
      pDispMap = pDispMap->pBaseMap;
#else
      if (NULL != pDispMap->pfnGetBaseMap)
         pDispMap = (*pDispMap->pfnGetBaseMap)();
      else
         pDispMap = NULL;
#endif
#endif

    }

   return NULL;    
}

CLeadContainer::CLeadContainer()
               : m_fontHolder(&m_xFontNotification)
{
   EnableAutomation();
   InitializeAmbientProperties();
}

CLeadContainer::~CLeadContainer()
{
}

void CLeadContainer::OnFinalRelease()
{
}

void CLeadContainer::InitializeAmbientProperties()
{

   m_backColor = 0x80000000+ COLOR_WINDOW;
   m_foreColor = 0x80000000+ COLOR_WINDOWTEXT;
   m_localeID = 0; 
   m_scaleUnits = "Pixels";
   m_textAlign = 0;
   m_bMessageReflect = FALSE;
   m_bUserMode = TRUE;
   m_bUIDead = FALSE;
   m_bShowGrabHandles = FALSE;
   m_bShowHatching = FALSE;
   m_bSupportsMnemonics = TRUE;
   m_bAutoClip = TRUE;

   m_fontHolder.InitializeFont();

   m_bPropertyChanged = FALSE;
   m_bFontPropertyChanged = FALSE;
   m_pUIActiveItem = NULL;   
   m_pFocusItem = NULL;
   m_pDefaultBttn = NULL;
   m_pEscapeBttn = NULL;
}   

void CLeadContainer::AddControl(CLead *pNewControl)
{

   m_OleControls.AddTail(pNewControl);
}

void CLeadContainer::RemoveControl(CLead *pOldControl)
{

   POSITION   thisPos;
   thisPos = m_OleControls.Find(pOldControl);
         
   if (thisPos)
      m_OleControls.RemoveAt(thisPos);
}

BOOL CLeadContainer::SetDefaultButton(CLead *pDefBttn)
{
                     
   if ((pDefBttn) && !(pDefBttn->m_dwStatus & OLEMISC_ACTSLIKEBUTTON))
      return FALSE;

   if (m_pDefaultBttn && (m_pDefaultBttn != pDefBttn))
   {
      if (TRUE == pDefBttn->m_bDisplayAsDefault)
      {
         m_pDefaultBttn->m_bDisplayAsDefault = FALSE;
         m_pDefaultBttn->m_pIOleControl->OnAmbientPropertyChange(DISPID_AMBIENT_DISPLAYASDEFAULT);
      }
   }
      
   m_pDefaultBttn = pDefBttn;
   
   if (m_pUIActiveItem)
      OnFocus(m_pUIActiveItem, TRUE);
   else
   {
      m_pDefaultBttn->m_bDisplayAsDefault = FALSE;
      m_pDefaultBttn->m_pIOleControl->OnAmbientPropertyChange(DISPID_AMBIENT_DISPLAYASDEFAULT);
   }
      
   return TRUE;
   
}

BOOL CLeadContainer::SetEscapeButton(CLead *pEscBttn)
{
   if ((pEscBttn) && !(pEscBttn->m_dwStatus & OLEMISC_ACTSLIKEBUTTON))
      return FALSE;
   m_pEscapeBttn = pEscBttn;
   return TRUE;
}

void CLeadContainer::OnSetActiveObject(CLead *pActiveControl)
{
}

void CLeadContainer::OnSetUIActiveObject(CLead *pUIActiveControl)
{
   if (pUIActiveControl && m_pUIActiveItem && (pUIActiveControl != m_pUIActiveItem))
      m_pUIActiveItem->DeactivateUI();    

   m_pUIActiveItem = pUIActiveControl;
}

void CLeadContainer::OnFocus(CLead *pFocus, BOOL fGotFocus)
{                                                          
   BOOL bShowDefaultAsDefault = FALSE;
   BOOL bShowCurrentAsDefault = FALSE;
         
   if (pFocus == m_pDefaultBttn)
   {
      bShowDefaultAsDefault = TRUE;
      bShowCurrentAsDefault = TRUE;
   }                               
   else
   if ( fGotFocus && 
       ((pFocus->m_dwStatus & OLEMISC_ACTSLIKEBUTTON) ||
        (pFocus->m_cntrlInfo.dwFlags & CTRLINFO_EATS_RETURN)) )
   {                       
      bShowDefaultAsDefault = FALSE;
      bShowCurrentAsDefault = (BOOL)(pFocus->m_dwStatus & OLEMISC_ACTSLIKEBUTTON);
   }
   else
   {
      bShowDefaultAsDefault = TRUE;
      bShowCurrentAsDefault = FALSE;
   }

   if (pFocus->m_bDisplayAsDefault != bShowCurrentAsDefault)
   {
      pFocus->m_bDisplayAsDefault = bShowCurrentAsDefault;
      pFocus->m_pIOleControl->OnAmbientPropertyChange(DISPID_AMBIENT_DISPLAYASDEFAULT);
   }

   if (m_pDefaultBttn && 
      (m_pDefaultBttn->m_bDisplayAsDefault != bShowDefaultAsDefault))
   {
      m_pDefaultBttn->m_bDisplayAsDefault = bShowDefaultAsDefault;
      m_pDefaultBttn->m_pIOleControl->OnAmbientPropertyChange(DISPID_AMBIENT_DISPLAYASDEFAULT);
   }

   if (fGotFocus)
   {
      m_pFocusItem = pFocus;
      if (m_pUIActiveItem && (m_pUIActiveItem != pFocus))
         m_pUIActiveItem->DeactivateUI();
   }
   else
   if (m_pFocusItem == pFocus)
      m_pFocusItem = NULL;
}

BOOL CLeadContainer::ProcessKey(LPMSG lpmsg)
{

   if ((WM_CHAR == lpmsg->message) && 
      (VK_RETURN == lpmsg->wParam) && m_pDefaultBttn)
   {
      if (m_pUIActiveItem && (m_pUIActiveItem->m_cntrlInfo.dwFlags & CTRLINFO_EATS_RETURN)) 
         return FALSE;
                          
      m_pDefaultBttn->m_pIOleControl->OnMnemonic(lpmsg);
      
      return TRUE;
   }

   if ((WM_CHAR == lpmsg->message) && 
      (VK_ESCAPE == lpmsg->wParam) && m_pEscapeBttn)
   {
      if (m_pUIActiveItem && (m_pUIActiveItem->m_cntrlInfo.dwFlags & CTRLINFO_EATS_ESCAPE)) 
         return FALSE;
                          
      m_pEscapeBttn->m_pIOleControl->OnMnemonic(lpmsg);
      
      return TRUE;
   }                   
   
   if ((WM_KEYDOWN == lpmsg->message) && 
      (VK_TAB == lpmsg->wParam))
   {                            
      if (!m_pUIActiveItem)
         return TRUE;
         
      POSITION      thisPos;
      CLead   *nextControl = NULL;
      BOOL         bShiftState;
      
      bShiftState = GetKeyState(VK_SHIFT) >> 1;
      
      thisPos = m_OleControls.Find(m_pUIActiveItem);
      
      if (bShiftState)
         m_OleControls.GetPrev(thisPos);
      else
         m_OleControls.GetNext(thisPos);
      
      while (nextControl != m_pUIActiveItem)
      {
         if (NULL == thisPos)
         {
            if (bShiftState)
               thisPos = m_OleControls.GetTailPosition();
            else
               thisPos = m_OleControls.GetHeadPosition();
         }

         if (bShiftState)
            nextControl = (CLead *)m_OleControls.GetPrev(thisPos);
         else
            nextControl = (CLead *)m_OleControls.GetNext(thisPos);

         if ( ! ((nextControl->m_dwStatus & OLEMISC_ACTSLIKELABEL) ||
               (nextControl->m_dwStatus & OLEMISC_NOUIACTIVATE)) )
            break;   
      }

      if (m_pUIActiveItem != nextControl)
      {
         nextControl->SetFocus();
         nextControl->ActivateUI();
         return TRUE;
      }
   }                  

   if ((WM_CHAR == lpmsg->message) && 
      (VK_TAB == lpmsg->wParam))
      return TRUE;
   
   return FALSE; 
}

BOOL CLeadContainer::ProcessMnemonics(LPMSG lpmsg)
{
   POSITION      pos;
   CLead   *pOcxItem;
   WORD          wID;
   
   for (pos = m_OleControls.GetHeadPosition(); pos;)
   {
      pOcxItem = (CLead *)m_OleControls.GetNext(pos);
      
      if (pOcxItem->m_cntrlInfo.hAccel)
      {
         for (UINT ai = 0; ai < pOcxItem->m_cntrlInfo.cAccel; ai++)
            if (IsAccelerator(pOcxItem->m_cntrlInfo.hAccel, pOcxItem->m_cntrlInfo.cAccel,
                     lpmsg, &wID))
            {                           
               pOcxItem->m_pIOleControl->OnMnemonic(lpmsg);
               return TRUE;
            }
      }   
   }

   return FALSE;

}

HRESULT CLeadContainer::OnTranslateAccelerator(LPMSG lpmsg)
{ 

   if (ProcessMnemonics(lpmsg))
      ResultFromScode(S_OK);

   return ResultFromScode(S_FALSE);
}

BOOL CLeadContainer::PreTranslateMessage(LPMSG lpmsg)
{
   
   if (lpmsg->message >= WM_KEYFIRST && lpmsg->message <= WM_KEYLAST)
   {
      if (ProcessKey(lpmsg))
         return TRUE;

      if (m_pUIActiveItem && m_pUIActiveItem->m_pIOleInPlaceActiveObject)
      {   
         if (m_pUIActiveItem->m_pIOleInPlaceActiveObject->TranslateAccelerator(lpmsg) == S_OK)
            return TRUE;
      }
   

      if (ProcessMnemonics(lpmsg))
              return TRUE;
   }
        
   return FALSE;
}


HRESULT CLeadContainer::PreMessageFilter(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
         LRESULT FAR* lplResult, DWORD FAR * lpdwCookie)
{
   return S_OK;
}


HRESULT CLeadContainer::PostMessageFilter(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
         LRESULT FAR* lplResult, DWORD dwCookie)
{
   return S_OK;
}

void CLeadContainer::GetWindowContext(
      CLead *pControl, 
      LPOLEINPLACEFRAME FAR* lplpFrame,
      LPOLEINPLACEUIWINDOW FAR* lplpDoc,
      LPRECT lpPosRect,   LPRECT lpClipRect,
      LPOLEINPLACEFRAMEINFO lpFrameInfo)
{

   ::CopyRect(lpPosRect, &pControl->m_rcClient);
   ::CopyRect(lpClipRect, &pControl->m_rcClient);
   lpFrameInfo->hwndFrame = pControl->m_hWndParent;

   *lplpFrame = (LPOLEINPLACEFRAME)GetInterface(&IID_IOleInPlaceFrame);
   *lplpDoc = NULL;

   lpFrameInfo->fMDIApp = FALSE;
   lpFrameInfo->haccel = NULL;
   lpFrameInfo->cAccelEntries = 0;

}

HRESULT CLeadContainer::OnGetWindow(HWND FAR*)
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnContextSensitiveHelp(BOOL)
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnGetBorder(LPRECT)
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnRequestBorderSpace(LPCBORDERWIDTHS)
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnSetBorderSpace(LPCBORDERWIDTHS)
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnInsertMenus(CMenu *, LPOLEMENUGROUPWIDTHS)
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnSetMenu(CMenu *, HOLEMENU, CLead *pActiveOcx)
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnRemoveMenus(CMenu *)
{
   return ResultFromScode(E_NOTIMPL);
}

#if IS_MFC40
/*
    This is for 32-bit code compiled with Visual C++ 4.0 only
*/
HRESULT CLeadContainer::OnSetStatusText(LPCOLESTR)
#else
HRESULT CLeadContainer::OnSetStatusText(LPCTSTR)
#endif
{
   return ResultFromScode(E_NOTIMPL);
}

HRESULT CLeadContainer::OnEnableModeless(BOOL)
{
   return ResultFromScode(E_NOTIMPL);
}

CLead* CLeadContainer::GetControlFromActiveObject(LPOLEINPLACEACTIVEOBJECT lpActiveObject)
{

   CLead   *pControl;
   
   for (POSITION pos = m_OleControls.GetHeadPosition(); pos; )
   {
      pControl = (CLead *)m_OleControls.GetNext(pos);
      if (pControl->m_pIOleInPlaceActiveObject == lpActiveObject)
         return pControl;
   }
      
   return NULL;
}

CLead* CLeadContainer::GetControlFromHwnd(HWND hWnd)
{

   CLead   *pControl;
   
   for (POSITION pos = m_OleControls.GetHeadPosition(); pos; )
   {
      pControl = (CLead *)m_OleControls.GetNext(pos);
      if (pControl->m_hWnd == hWnd)
         return pControl;
   }
      
   return NULL;
}


BEGIN_MESSAGE_MAP(CLeadContainer, CCmdTarget)
   //{{AFX_MSG_MAP(CLeadContainer)
      // NOTE - the ClassWizard will add and remove mapping macros here.
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLeadContainer, CCmdTarget)
   //{{AFX_DISPATCH_MAP(CLeadContainer)
   DISP_PROPERTY_EX_ID(CLeadContainer, "ForeColor",       DISPID_AMBIENT_FORECOLOR,       GetForeColor, NULL, VT_COLOR)
   DISP_PROPERTY_EX_ID(CLeadContainer, "BackColor",       DISPID_AMBIENT_BACKCOLOR,       GetBackColor, NULL, VT_COLOR)
   DISP_PROPERTY_EX_ID(CLeadContainer, "Font",          DISPID_AMBIENT_FONT,          GetFont, NULL, VT_FONT)
   DISP_PROPERTY_EX_ID(CLeadContainer, "ScaleUnits",    DISPID_AMBIENT_SCALEUNITS,       GetScaleUnits, NULL, VT_BSTR)
   DISP_PROPERTY_EX_ID(CLeadContainer, "TextAlign",       DISPID_AMBIENT_TEXTALIGN,       GetTextAlign, NULL, VT_I2)
   DISP_PROPERTY_EX_ID(CLeadContainer, "MessageReflect", DISPID_AMBIENT_MESSAGEREFLECT,    GetMessageReflect, NULL, VT_BOOL)
   DISP_PROPERTY_EX_ID(CLeadContainer, "UserMode",       DISPID_AMBIENT_USERMODE,       GetUserMode, NULL, VT_BOOL)
   DISP_PROPERTY_EX_ID(CLeadContainer, "UIDead",       DISPID_AMBIENT_UIDEAD,          GetUIDead, NULL, VT_BOOL)
   DISP_PROPERTY_EX_ID(CLeadContainer, "ShowGrabHandles",DISPID_AMBIENT_SHOWGRABHANDLES, GetShowGrabHandles, NULL, VT_BOOL)
   DISP_PROPERTY_EX_ID(CLeadContainer, "ShowHatching",    DISPID_AMBIENT_SHOWHATCHING,    GetShowHatching, NULL, VT_BOOL)
   DISP_PROPERTY_EX_ID(CLeadContainer, "SupportsMnemonics",DISPID_AMBIENT_SUPPORTSMNEMONICS,   GetSupportsMnemonics, NULL, VT_BOOL)
   DISP_PROPERTY_EX_ID(CLeadContainer, "AutoClip",       DISPID_AMBIENT_AUTOCLIP,       GetAutoClip, NULL, VT_BOOL)
   //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

OLE_COLOR CLeadContainer::GetForeColor() 
{

   return m_foreColor;
}

void CLeadContainer::SetForeColor(OLE_COLOR nNewValue, BOOL bNotify ) 
{

   m_foreColor = nNewValue;

   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_FORECOLOR);
   else
      m_bPropertyChanged = TRUE;
}

OLE_COLOR CLeadContainer::GetBackColor() 
{

   return m_backColor;
}

void CLeadContainer::SetBackColor(OLE_COLOR nNewValue, BOOL bNotify ) 
{

   m_backColor = nNewValue;

   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_BACKCOLOR);
   else
      m_bPropertyChanged = TRUE;
}

LPFONTDISP CLeadContainer::GetFont()
{
   return m_fontHolder.GetFontDispatch();
}

void CLeadContainer::SetFont(LPFONTDISP pFontDisp, BOOL bNotify) 
{

   m_fontHolder.InitializeFont(NULL, pFontDisp);

   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_FONT);
   else
      m_bPropertyChanged = TRUE;
}


void CLeadContainer::SetFont(LPFONTDESC pFontDesc, BOOL bNotify) 
{

   m_fontHolder.InitializeFont(pFontDesc, NULL);

   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_FONT);
   else
      m_bPropertyChanged = TRUE;
}


long CLeadContainer::GetLocaleID() 
{

   return m_localeID;
}

void CLeadContainer::SetLocaleID(long nNewValue, BOOL bNotify ) 
{

   m_localeID = nNewValue;
   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_LOCALEID);
   else
      m_bPropertyChanged = TRUE;
}

BSTR CLeadContainer::GetScaleUnits() 
{

   return m_scaleUnits.AllocSysString();
}

void CLeadContainer::SetScaleUnits(LPCTSTR lpszNewValue, BOOL bNotify ) 
{

   m_scaleUnits = lpszNewValue;
   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_SCALEUNITS);
   else
      m_bPropertyChanged = TRUE;
}

short CLeadContainer::GetTextAlign() 
{

   return m_textAlign;
}

void CLeadContainer::SetTextAlign(short nNewValue, BOOL bNotify ) 
{

   m_textAlign = nNewValue;
   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_TEXTALIGN);
   else
      m_bPropertyChanged = TRUE;
}

BOOL CLeadContainer::GetMessageReflect() 
{

   return m_bMessageReflect;
}

void CLeadContainer::SetMessageReflect(BOOL bNewValue, BOOL bNotify ) 
{

   m_bMessageReflect = bNewValue;
   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_MESSAGEREFLECT);
   else
      m_bPropertyChanged = TRUE;
}

BOOL CLeadContainer::GetUserMode() 
{

   return m_bUserMode;
}

void CLeadContainer::SetUserMode(BOOL bNewValue, BOOL bNotify ) 
{

   m_bUserMode = bNewValue;

   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_USERMODE);
   else
      m_bPropertyChanged = TRUE;
}

BOOL CLeadContainer::GetUIDead() 
{

   return m_bUIDead;
}

void CLeadContainer::SetUIDead(BOOL bNewValue, BOOL bNotify ) 
{

   m_bUIDead = bNewValue;

   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_UIDEAD);
   else
      m_bPropertyChanged = TRUE;
}

BOOL CLeadContainer::GetShowGrabHandles() 
{
   return m_bShowGrabHandles;
}

void CLeadContainer::SetShowGrabHandles(BOOL bNewValue, BOOL bNotify ) 
{

   m_bShowGrabHandles = bNewValue;

   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_SHOWGRABHANDLES);
   else
      m_bPropertyChanged = TRUE;
}

BOOL CLeadContainer::GetShowHatching() 
{

   return m_bShowHatching;
}

void CLeadContainer::SetShowHatching(BOOL bNewValue, BOOL bNotify) 
{

   m_bShowHatching = bNewValue;
   
   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_SHOWHATCHING);
   else
      m_bPropertyChanged = TRUE;
}

BOOL CLeadContainer::GetSupportsMnemonics() 
{

   return m_bSupportsMnemonics;
}

void CLeadContainer::SetSupportsMnemonics(BOOL bNewValue, BOOL bNotify ) 
{

   m_bSupportsMnemonics = bNewValue;
   
   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_SUPPORTSMNEMONICS);
   else
      m_bPropertyChanged = TRUE;
}

BOOL CLeadContainer::GetAutoClip() 
{

   return m_bAutoClip;
}

void CLeadContainer::SetAutoClip(BOOL bNewValue, BOOL bNotify ) 
{

   m_bAutoClip = bNewValue;
   
   if (bNotify)
      OnPropertyChanged(DISPID_AMBIENT_AUTOCLIP);
   else
      m_bPropertyChanged = TRUE;
}


void CLeadContainer::OnFontChanged()
{

   m_bFontPropertyChanged = TRUE;

}

void CLeadContainer::OnPropertyChanged(DISPID   prop)
{
   
   if (m_bPropertyChanged)
      prop = -1;
   else
   if (m_bFontPropertyChanged && (prop != DISPID_AMBIENT_FONT))
      prop = -1;

   POSITION   pos;
   CLead   *pOcxItem;
   
   for (pos = m_OleControls.GetHeadPosition(); pos;)
   {
      pOcxItem = (CLead *)m_OleControls.GetNext(pos);
      pOcxItem->m_pIOleControl->OnAmbientPropertyChange(prop);
   }

   m_bPropertyChanged = m_bFontPropertyChanged = FALSE;
}

BEGIN_INTERFACE_MAP(CLeadContainer, CCmdTarget)
   INTERFACE_PART(CLeadContainer, IID_IOleInPlaceFrame, OleInPlaceFrame)
   INTERFACE_PART(CLeadContainer, IID_IPropertyNotifySink, FontNotification)
END_INTERFACE_MAP()


STDMETHODIMP_(ULONG) CLeadContainer::XOleInPlaceFrame::AddRef()
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CLeadContainer::XOleInPlaceFrame::Release()
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)
   return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)
   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::GetWindow(
   HWND FAR* lphwnd)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   *lphwnd = NULL;

   return pThis->OnGetWindow(lphwnd);
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::ContextSensitiveHelp(
   BOOL fEnterMode)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return pThis->OnContextSensitiveHelp(fEnterMode);
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::GetBorder(
   LPRECT lpRectBorder)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return pThis->OnGetBorder(lpRectBorder);
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::RequestBorderSpace(
   LPCRECT lpRectWidths)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return pThis->OnRequestBorderSpace(lpRectWidths);
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::SetBorderSpace(
   LPCRECT lpRectWidths)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return pThis->OnSetBorderSpace(lpRectWidths);
}

#if IS_MFC40
STDMETHODIMP CLeadContainer::XOleInPlaceFrame::SetActiveObject(
   LPOLEINPLACEACTIVEOBJECT lpActiveObject, LPCOLESTR pszObjName)
#else
// this is for Visual C++ 2.0
STDMETHODIMP CLeadContainer::XOleInPlaceFrame::SetActiveObject(
   LPOLEINPLACEACTIVEOBJECT lpActiveObject, LPCTSTR pszObjName)
#endif
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)


   if (lpActiveObject)
   {   
      CLead *pControl;
      pControl = pThis->GetControlFromActiveObject(lpActiveObject);
   
      pThis->OnSetActiveObject(pControl);
   }
   else
      pThis->OnSetActiveObject(NULL);
   
   return S_OK;
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::InsertMenus(
   HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return pThis->OnInsertMenus(CMenu::FromHandle(hmenuShared), lpMenuWidths);
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::SetMenu(
   HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)


   CLead *pControl;
   pControl = pThis->GetControlFromHwnd(hwndActiveObject);
   
   return pThis->OnSetMenu(CMenu::FromHandle(hmenuShared), holemenu, pControl);

}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::RemoveMenus(
   HMENU hmenuShared)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return pThis->OnRemoveMenus(CMenu::FromHandle(hmenuShared));
}

// this is for Visual C++ 2.0
#if IS_MFC40
STDMETHODIMP CLeadContainer::XOleInPlaceFrame::SetStatusText(
   LPCOLESTR lpszStatusText)
#else
STDMETHODIMP CLeadContainer::XOleInPlaceFrame::SetStatusText(
   LPCTSTR lpszStatusText)
#endif
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   return pThis->OnSetStatusText(lpszStatusText);

}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::EnableModeless(BOOL fEnable)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)

   
   return pThis->OnEnableModeless(fEnable);
}

STDMETHODIMP CLeadContainer::XOleInPlaceFrame::TranslateAccelerator(
   LPMSG lpmsg, WORD wID)
{
   METHOD_MANAGE_STATE(CLeadContainer, OleInPlaceFrame)
                                    
   return pThis->OnTranslateAccelerator(lpmsg);

}


STDMETHODIMP_(ULONG) CLeadContainer::XFontNotification::AddRef()
{
   METHOD_MANAGE_STATE(CLeadContainer, FontNotification)

   return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) CLeadContainer::XFontNotification::Release()
{
   METHOD_MANAGE_STATE(CLeadContainer, FontNotification)
   return (ULONG)pThis->ExternalRelease();

}


STDMETHODIMP CLeadContainer::XFontNotification::QueryInterface(
   REFIID iid, LPVOID far* ppvObj)
{
   METHOD_MANAGE_STATE(CLeadContainer, FontNotification)

   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP CLeadContainer::XFontNotification::OnChanged(DISPID)
{
   METHOD_MANAGE_STATE(CLeadContainer, FontNotification)

   return NOERROR;
}

STDMETHODIMP CLeadContainer::XFontNotification::OnRequestEdit(DISPID)
{
   METHOD_MANAGE_STATE(CLeadContainer, FontNotification)

   return NOERROR;
}



#ifdef EXPORT
#undef EXPORT
#endif
#define EXPORT

EXPORT CFontHolder::CFontHolder(LPPROPERTYNOTIFYSINK pNotify) :
   m_pFont(NULL),
   m_dwConnectCookie(0),
   m_pNotify(pNotify)
{
}

EXPORT CFontHolder::~CFontHolder()
{
   ReleaseFont();
}

void EXPORT CFontHolder::ReleaseFont()
{
   if ((m_pFont) && (m_pNotify))
   {
      AfxConnectionUnadvise(m_pFont, IID_IPropertyNotifySink, m_pNotify,
         FALSE, m_dwConnectCookie);
   }

   RELEASE(m_pFont);
}




void EXPORT CFontHolder::InitializeFont(const FONTDESC FAR* pFontDesc,
   LPDISPATCH pFontDispAmbient)
{

   ReleaseFont();

   LPFONT pFontAmbient;
   LPFONT pFontNew = NULL;

   if ((pFontDispAmbient) &&
      SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont,
            (LPVOID FAR*)&pFontAmbient)))
   {

      pFontAmbient->Clone(&pFontNew);
      pFontAmbient->Release();
   }
   else
   {
      if (!pFontDesc)
         pFontDesc = &_fdDefault;

      if (FAILED(CreateFontIndirect((LPFONTDESC)pFontDesc, IID_IFont,
            (LPVOID FAR *)&pFontNew)))
         pFontNew = NULL;
   }

   if (pFontNew)
      SetFont(pFontNew);
}


BOOL _AfxIsSameFont(CFontHolder& font, const FONTDESC FAR* pFontDesc,
   LPFONTDISP pFontDispAmbient)
{
   if (!font.m_pFont)
      return FALSE;

   BOOL bSame = FALSE;

   if (pFontDispAmbient)
   {
      LPFONT pFontAmbient;
      if (SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont,
         (LPVOID FAR*)&pFontAmbient)))
      {
         bSame = pFontAmbient->IsEqual(font.m_pFont) == S_OK;
         pFontAmbient->Release();
      }
   }
   else
   {
      if (!pFontDesc)
         pFontDesc = &_fdDefault;

      bSame = TRUE;
      BOOL bFlag;

      font.m_pFont->get_Italic(&bFlag);
      bSame = (bFlag == pFontDesc->fItalic);

      if (bSame)
      {
         font.m_pFont->get_Underline(&bFlag);
         bSame = (bFlag == pFontDesc->fUnderline);
      }

      if (bSame)
      {
         font.m_pFont->get_Strikethrough(&bFlag);
         bSame = (bFlag == pFontDesc->fStrikethrough);
      }

      if (bSame)
      {
         short sCharset;
         font.m_pFont->get_Charset(&sCharset);
         bSame = (sCharset == pFontDesc->sCharset);
      }

      if (bSame)
      {
         short sWeight;
         font.m_pFont->get_Weight(&sWeight);
         bSame = (sWeight == pFontDesc->sWeight);
      }

      if (bSame)
      {
         CURRENCY cy;
         font.m_pFont->get_Size(&cy);
         bSame = (memcmp(&cy, &pFontDesc->cySize, sizeof(CURRENCY)) == 0);
      }

      if (bSame)
      {
         BSTR bstrName;
         font.m_pFont->get_Name(&bstrName);
         CString strName1(bstrName);
         CString strName2(pFontDesc->lpstrName);
         bSame = (strName1 == strName2);
         SysFreeString(bstrName);
      }
   }

   return bSame;
}




HFONT EXPORT CFontHolder::GetFontHandle()
{
   CDC dc;
   dc.CreateCompatibleDC(NULL);
   return GetFontHandle((long)dc.GetDeviceCaps(LOGPIXELSY), HIMETRIC_PER_INCH);
}

HFONT EXPORT CFontHolder::GetFontHandle(long cyLogical, long cyHimetric)
{
   HFONT hFont = NULL;

   if ((m_pFont) &&
      SUCCEEDED(m_pFont->SetRatio(cyLogical, cyHimetric)) &&
      SUCCEEDED(m_pFont->get_hFont(&hFont)))
   {
   }

   return hFont;
}

CFont* EXPORT CFontHolder::Select(CDC* pDC, long cyLogical, long cyHimetric)
{

   HFONT hFont = NULL;

   if (m_pFont)
      hFont = GetFontHandle(cyLogical, cyHimetric);

   if (hFont)
   {
      if ((pDC->m_hAttribDC != pDC->m_hDC) &&
         (pDC->m_hAttribDC))
      {
         ::SelectObject(pDC->m_hAttribDC, hFont);
      }

      return CFont::FromHandle((HFONT)::SelectObject(pDC->m_hDC, hFont));
   }

   return NULL;
}


void EXPORT CFontHolder::QueryTextMetrics(LPTEXTMETRIC lptm)
{

   if (m_pFont)
   {
#if defined(_UNICODE) || defined(OLE2ANSI) || !defined(WIN32)
      m_pFont->QueryTextMetrics(lptm);
#else
      TEXTMETRICW tmw;
      m_pFont->QueryTextMetrics(&tmw);
      AfxTextMetricW2A(lptm, &tmw);
#endif
   }
   else
   {
      memset(lptm, 0, sizeof(TEXTMETRIC));
   }
}


LPFONTDISP EXPORT CFontHolder::GetFontDispatch()
{
   LPFONTDISP pFontDisp = NULL;

   if (m_pFont)
      SUCCEEDED(m_pFont->QueryInterface(IID_IFontDisp, (LPVOID*)&pFontDisp));

   return pFontDisp;
}


void EXPORT CFontHolder::SetFont(LPFONT pFontNew)
{

   if (m_pFont)
      ReleaseFont();

   m_pFont = pFontNew;

   if (m_pNotify)
   {
      AfxConnectionAdvise(m_pFont, IID_IPropertyNotifySink, m_pNotify,
         FALSE, &m_dwConnectCookie);
   }
}


HRESULT CFontHolder::CreateFontIndirect(LPFONTDESC lpFontDesc, REFIID riid, LPVOID FAR* lplpvObj)
{   

    HRESULT hresult;
    LPUNKNOWN punk;
    LPFONT pFont = NULL;

    hresult = CoCreateInstance(CLSID_StdFont, NULL,
         CLSCTX_INPROC_SERVER, IID_IUnknown, (LPLP)&punk);

   if (FAILED(hresult))
      return hresult;

    VERIFY(SUCCEEDED(punk->QueryInterface(IID_IFont, (LPLP) &pFont)));
    punk->Release();


   if (NULL == lpFontDesc)
      lpFontDesc = (LPFONTDESC)&_fdDefault;

   
   pFont->put_Name(lpFontDesc->lpstrName);
   pFont->put_Size(lpFontDesc->cySize);
   pFont->put_Weight(lpFontDesc->sWeight);
   pFont->put_Charset(lpFontDesc->sCharset);
   pFont->put_Italic(lpFontDesc->fItalic);
   pFont->put_Underline(lpFontDesc->fUnderline);
   pFont->put_Strikethrough(lpFontDesc->fStrikethrough);

   if (riid != IID_IFont)
   {
      VERIFY(SUCCEEDED(pFont->QueryInterface(riid, lplpvObj)));
      pFont->Release();
   }
   else
      *lplpvObj = pFont;
      
   return NOERROR;   
}


BOOL AFXAPI AfxConnectionAdvise(LPUNKNOWN pUnkSrc, REFIID iid,
   LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD FAR* pdwCookie)
{

   BOOL bSuccess = FALSE;

   LPCONNECTIONPOINTCONTAINER pCPC;

   if (SUCCEEDED(pUnkSrc->QueryInterface(
               IID_IConnectionPointContainer,
               (LPVOID FAR*)&pCPC)))
   {

      LPCONNECTIONPOINT pCP;

      if (SUCCEEDED(pCPC->FindConnectionPoint(iid, &pCP)))
      {

         if (SUCCEEDED(pCP->Advise(pUnkSink, pdwCookie)))
            bSuccess = TRUE;

         pCP->Release();


         if (bSuccess && !bRefCount)
            pUnkSink->Release();
      }

      pCPC->Release();
   }

   return bSuccess;
}


BOOL AFXAPI AfxConnectionUnadvise(LPUNKNOWN pUnkSrc, REFIID iid,
   LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD dwCookie)
{


   if (!bRefCount)
      pUnkSink->AddRef();

   BOOL bSuccess = FALSE;

   LPCONNECTIONPOINTCONTAINER pCPC;

   if (SUCCEEDED(pUnkSrc->QueryInterface(
               IID_IConnectionPointContainer,
               (LPVOID FAR*)&pCPC)))
   {

      LPCONNECTIONPOINT pCP;

      if (SUCCEEDED(pCPC->FindConnectionPoint(iid, &pCP)))
      {

         if (SUCCEEDED(pCP->Unadvise(dwCookie)))
            bSuccess = TRUE;

         pCP->Release();
      }

      pCPC->Release();
   }


   if (!bRefCount && !bSuccess)
      pUnkSink->Release();

   return bSuccess;
}

static HRESULT InterfaceConnect(LPUNKNOWN pObj, REFIID riid,
    LPUNKNOWN pIUnknownSink, LPDWORD pdwConn)
{
    HRESULT                     hr;
    LPCONNECTIONPOINTCONTAINER  pCPC;
    LPCONNECTIONPOINT           pCP;

    if (NULL==pObj || NULL==pIUnknownSink || NULL==pdwConn)
        return FALSE;

    hr=pObj->QueryInterface(IID_IConnectionPointContainer
        , (LPVOID *)&pCPC);

    if (FAILED(hr))
        return FALSE;

    hr=pCPC->FindConnectionPoint(riid, &pCP);

    if (SUCCEEDED(hr))
   {
        hr=pCP->Advise(pIUnknownSink, pdwConn);
        pCP->Release();
   }

    pCPC->Release();
    return hr;
}


static HRESULT InterfaceDisconnect(LPUNKNOWN pObj, REFIID riid, LPDWORD pdwConn)
{
    HRESULT                     hr;
    LPCONNECTIONPOINTCONTAINER  pCPC;
    LPCONNECTIONPOINT           pCP;

    if (NULL==pObj || NULL==pdwConn)
        return FALSE;

    if (0==*pdwConn)
        return FALSE;

    hr=pObj->QueryInterface(IID_IConnectionPointContainer
        , (LPVOID *)&pCPC);

    if (FAILED(hr))
        return FALSE;

    hr=pCPC->FindConnectionPoint(riid, &pCP);

    if (SUCCEEDED(hr))
        {
        hr=pCP->Unadvise(*pdwConn);

        if (SUCCEEDED(hr))
            *pdwConn=0L;

        pCP->Release();
        }

    pCPC->Release();
    return hr;
}


static HRESULT ObjectTypeInfo(LPUNKNOWN pObj, LPTYPEINFO *ppITypeInfo)
{
    HRESULT             hr;
    LPPROVIDECLASSINFO  pIProvideClassInfo;
    

    *ppITypeInfo=NULL;


    hr=pObj->QueryInterface(IID_IProvideClassInfo
        , (LPVOID *)&pIProvideClassInfo);

    if (FAILED(hr))
        return hr;

    hr=pIProvideClassInfo->GetClassInfo(ppITypeInfo);
    pIProvideClassInfo->Release();

    return hr;
}


static HRESULT ObjectTypeInfoEvents(LPUNKNOWN pObj, LPTYPEINFO *ppITypeInfo)
{
    HRESULT             hr;
    LPTYPEINFO          pITypeInfoAll;
    LPTYPEATTR          pTA;


    if (FAILED(hr = ObjectTypeInfo(pObj, &pITypeInfoAll)))
        return hr;


    *ppITypeInfo=NULL;  

    if (SUCCEEDED(hr = pITypeInfoAll->GetTypeAttr(&pTA)))
        {
        UINT        i;
        int         iFlags;

        for (i=0; i < pTA->cImplTypes; i++)
            {
            hr=pITypeInfoAll->GetImplTypeFlags(i, &iFlags);

            if (FAILED(hr))
                continue;

            if ((iFlags & IMPLTYPEFLAG_FDEFAULT)
                && (iFlags & IMPLTYPEFLAG_FSOURCE))
                {
                HREFTYPE    hRefType=NULL;

                pITypeInfoAll->GetRefTypeOfImplType(i, &hRefType);
                hr=pITypeInfoAll->GetRefTypeInfo(hRefType
                    , ppITypeInfo);

                break;
                }
            }

        pITypeInfoAll->ReleaseTypeAttr(pTA);
        }

    pITypeInfoAll->Release();
    return (hr);
}

static HRESULT ObjectEventsIID(LPUNKNOWN pObj, IID *piid)
{
    HRESULT             hr;
    LPTYPEINFO          pITypeInfo;
    LPTYPEATTR          pTA;

    *piid=CLSID_NULL;

    if (FAILED(hr = ObjectTypeInfoEvents(pObj, &pITypeInfo)))
        return hr;

    hr=pITypeInfo->GetTypeAttr(&pTA);

    if (SUCCEEDED(hr))
        {
        *piid=pTA->guid;
        pITypeInfo->ReleaseTypeAttr(pTA);
        }

    pITypeInfo->Release();
    return hr;
}

static long LongMulDiv(long numer, long denom)
{
   ldiv_t result;
   long quotient;
   long absrem;

   result = ldiv(numer, denom);
   quotient = result.quot;
   absrem = labs(result.rem);

   if (labs(denom) - absrem <= absrem)
   if (quotient >= 0)
      quotient++;
   else
      quotient--;

   return quotient;
}


static int XformWidthInPixelsToHimetric(HDC hDC, int iWidthInPix)
{
   int     iXppli;     
   int     iWidthInHiMetric;
   BOOL    fSystemDC=FALSE;

   if (NULL==hDC)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   iXppli = GetDeviceCaps (hDC, LOGPIXELSX);

   iWidthInHiMetric = MAP_PIX_TO_LOGHIM(iWidthInPix, iXppli);

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return iWidthInHiMetric;
}


static int XformWidthInHimetricToPixels(HDC hDC, int iWidthInHiMetric)
{
   int     iXppli;     
   int     iWidthInPix;
   BOOL    fSystemDC=FALSE;

   if (NULL==hDC)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   iXppli = GetDeviceCaps (hDC, LOGPIXELSX);

   iWidthInPix = MAP_LOGHIM_TO_PIX(iWidthInHiMetric, iXppli);

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return iWidthInPix;
}


static int XformHeightInPixelsToHimetric(HDC hDC, int iHeightInPix)
{
   int     iYppli;     
   int     iHeightInHiMetric;
   BOOL    fSystemDC=FALSE;

   if (NULL==hDC)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

   iHeightInHiMetric = MAP_PIX_TO_LOGHIM(iHeightInPix, iYppli);

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return iHeightInHiMetric;
}


static int XformHeightInHimetricToPixels(HDC hDC, int iHeightInHiMetric)
{
   int     iYppli;     
   int     iHeightInPix;
   BOOL    fSystemDC=FALSE;

   if (NULL==hDC)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

   iHeightInPix = MAP_LOGHIM_TO_PIX(iHeightInHiMetric, iYppli);

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return iHeightInPix;
}


static void XformSizeInPixelsToHimetric(
   HDC hDC, LPSIZEL lpSizeInPix, LPSIZEL lpSizeInHiMetric)
{
   int     iXppli;     
   int     iYppli;     
   BOOL    fSystemDC=FALSE;

   if (NULL==hDC || GetDeviceCaps(hDC, LOGPIXELSX) == 0)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   iXppli = GetDeviceCaps (hDC, LOGPIXELSX);
   iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

   lpSizeInHiMetric->cx = (long)MAP_PIX_TO_LOGHIM((int)lpSizeInPix->cx, iXppli);
   lpSizeInHiMetric->cy = (long)MAP_PIX_TO_LOGHIM((int)lpSizeInPix->cy, iYppli);

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return;
}


static void XformSizeInHimetricToPixels(
   HDC hDC, LPSIZEL lpSizeInHiMetric, LPSIZEL lpSizeInPix)
{
   int     iXppli;     
   int     iYppli;     
   BOOL    fSystemDC=FALSE;

   if (NULL==hDC || GetDeviceCaps(hDC, LOGPIXELSX) == 0)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   iXppli = GetDeviceCaps (hDC, LOGPIXELSX);
   iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

   lpSizeInPix->cx = (long)MAP_LOGHIM_TO_PIX((int)lpSizeInHiMetric->cx, iXppli);
   lpSizeInPix->cy = (long)MAP_LOGHIM_TO_PIX((int)lpSizeInHiMetric->cy, iYppli);

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return;
}

static void XformRectInPixelsToHimetric(
   HDC hDC, LPRECT lprcPix, LPRECT lprcHiMetric)
{
   int     iXppli;
   int     iYppli;
   int     iXextInPix=(lprcPix->right-lprcPix->left);
   int     iYextInPix=(lprcPix->bottom-lprcPix->top);
   BOOL    fSystemDC=FALSE;

   if (NULL==hDC || GetDeviceCaps(hDC, LOGPIXELSX) == 0)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   iXppli = GetDeviceCaps (hDC, LOGPIXELSX);
   iYppli = GetDeviceCaps (hDC, LOGPIXELSY);


   lprcHiMetric->right = (int)LongMulDiv((LONG)iXextInPix * HIMETRIC_PER_INCH, iXppli);
   lprcHiMetric->top = (int)LongMulDiv((LONG)iYextInPix * HIMETRIC_PER_INCH, iYppli);

   lprcHiMetric->left = 0;
   lprcHiMetric->bottom = 0;

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return;
}


static int SetDCToDrawInHimetricRect(HDC hDC, LPRECT lprcPix,
   LPRECT lprcHiMetric, LPRECT lprcWindowOld, LPRECT lprcViewportOld)
{
   int nMapModeOld = SetMapMode(hDC, MM_ANISOTROPIC);
   BOOL fSystemDC = FALSE;

   if (NULL==hDC)
   {
      hDC=GetDC(NULL);
      fSystemDC=TRUE;
   }

   XformRectInPixelsToHimetric(hDC, lprcPix, lprcHiMetric);

   SetWindowOrgEx(hDC, lprcHiMetric->left, lprcHiMetric->bottom,
      (LPPOINT)&lprcWindowOld->left);
   SetWindowExtEx(hDC, (lprcHiMetric->right-lprcHiMetric->left),
      (lprcHiMetric->top-lprcHiMetric->bottom), (LPSIZE)&lprcWindowOld->right);
   SetViewportOrgEx(hDC, lprcPix->left, lprcPix->bottom,
      (LPPOINT)&lprcViewportOld->left);
   SetViewportExtEx(hDC, (lprcPix->right-lprcPix->left),
      (lprcPix->top-lprcPix->bottom), (LPSIZE)&lprcViewportOld->right);

   if (fSystemDC)
      ReleaseDC(NULL, hDC);

   return nMapModeOld;
}


/////////////////////////////////////////////////////////////////////////////
// COleFont copy constructor

COleFont::COleFont(const COleFont& dispatchSrc) : COleDispatchDriver()
{
   ASSERT(this != &dispatchSrc);   // constructing from self?

   m_lpDispatch = dispatchSrc.m_lpDispatch;
   if (m_lpDispatch != NULL)
      m_lpDispatch->AddRef();
   m_bAutoRelease = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COleFont properties

CString COleFont::GetName()
{
   CString result;
   GetProperty(0x0, VT_BSTR, (void*)&result);
   return result;
}

void COleFont::SetName(LPCTSTR propVal)
{
   SetProperty(0x0, VT_BSTR, propVal);
}

CY COleFont::GetSize()
{
   CY result;
   GetProperty(0x2, VT_CY, (void*)&result);
   return result;
}

void COleFont::SetSize(const CY& propVal)
{
   SetProperty(0x2, VT_CY, &propVal);
}

BOOL COleFont::GetBold()
{
   BOOL result;
   GetProperty(0x3, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetBold(BOOL propVal)
{
   SetProperty(0x3, VT_BOOL, propVal);
}

BOOL COleFont::GetItalic()
{
   BOOL result;
   GetProperty(0x4, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetItalic(BOOL propVal)
{
   SetProperty(0x4, VT_BOOL, propVal);
}

BOOL COleFont::GetUnderline()
{
   BOOL result;
   GetProperty(0x5, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetUnderline(BOOL propVal)
{
   SetProperty(0x5, VT_BOOL, propVal);
}

BOOL COleFont::GetStrikethrough()
{
   BOOL result;
   GetProperty(0x6, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetStrikethrough(BOOL propVal)
{
   SetProperty(0x6, VT_BOOL, propVal);
}

short COleFont::GetWeight()
{
   short result;
   GetProperty(0x7, VT_I2, (void*)&result);
   return result;
}

void COleFont::SetWeight(short propVal)
{
   SetProperty(0x7, VT_I2, propVal);
}

short COleFont::GetCharset()
{
   short result;
   GetProperty(0x8, VT_I2, (void*)&result);
   return result;
}

void COleFont::SetCharset(short propVal)
{
   SetProperty(0x8, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// COleFont operations





// new methods/properties


// properties
short CLead::GetLevelLowBit()
{
   short result;
   GetProperty(dispidLevelLowBit, VT_I2, (void *)&result);
   return result;
}

void CLead::SetLevelLowBit(short nNewValue)
{
   SetProperty(dispidLevelLowBit, VT_I2, nNewValue);
}

short CLead::GetLevelHighBit()
{
   short result;
   GetProperty(dispidLevelHighBit, VT_I2, (void *)&result);
   return result;
}

void CLead::SetLevelHighBit(short nNewValue)
{
   SetProperty(dispidLevelHighBit, VT_I2, nNewValue);
}

long CLead::GetLevelLUTLength()
{
   long result;
   GetProperty(dispidLevelLUTLength, VT_I4, (void *)&result);
   return result;
}

void CLead::SetLevelLUTLength(long nNewValue)
{
   SetProperty(dispidLevelLUTLength, VT_I4, nNewValue);
}

long CLead::GetMinVal()
{
   long result;
   GetProperty(dispidMinVal, VT_I4, (void *)&result);
   return result;
}

void CLead::SetMinVal(long nNewValue)
{
   SetProperty(dispidMinVal, VT_I4, nNewValue);
}

long CLead::GetMaxVal()
{
   long result;
   GetProperty(dispidMaxVal, VT_I4, (void *)&result);
   return result;
}

void CLead::SetMaxVal(long nNewValue)
{
   SetProperty(dispidMaxVal, VT_I4, nNewValue);
}

short CLead::GetMinBit()
{
   short result;
   GetProperty(dispidMinBit, VT_I2, (void *)&result);
   return result;
}

void CLead::SetMinBit(short nNewValue)
{
   SetProperty(dispidMinBit, VT_I2, nNewValue);
}

short CLead::GetMaxBit()
{
   short result;
   GetProperty(dispidMaxBit, VT_I2, (void *)&result);
   return result;
}

void CLead::SetMaxBit(short nNewValue)
{
   SetProperty(dispidMaxBit, VT_I2, nNewValue);
}

BOOL CLead::GetAnnShowLockedIcon()
{
   BOOL result;
   GetProperty(dispidAnnShowLockedIcon, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetAnnShowLockedIcon(BOOL bNewValue)
{
   SetProperty(dispidAnnShowLockedIcon, VT_BOOL, bNewValue);
}

BOOL CLead::GetAnnHyperlinkMenuEnable()
{
   BOOL result;
   GetProperty(dispidAnnHyperlinkMenuEnable, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetAnnHyperlinkMenuEnable(BOOL bNewValue)
{
   SetProperty(dispidAnnHyperlinkMenuEnable, VT_BOOL, bNewValue);
}

BOOL CLead::GetEnableZoomInRect()
{
   BOOL result;
   GetProperty(dispidEnableZoomInRect, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetEnableZoomInRect(BOOL bNewValue)
{
   SetProperty(dispidEnableZoomInRect, VT_BOOL, bNewValue);
}

short CLead::GetTwainIntensity()
{
   short result;
   GetProperty(dispidTwainIntensity, VT_I2, (void *)&result);
   return result;
}

void CLead::SetTwainIntensity(short nNewValue)
{
   SetProperty(dispidTwainIntensity, VT_I2, nNewValue);
}

short CLead::GetTwainContrast()
{
   short result;
   GetProperty(dispidTwainContrast, VT_I2, (void *)&result);
   return result;
}

void CLead::SetTwainContrast(short nNewValue)
{
   SetProperty(dispidTwainContrast, VT_I2, nNewValue);
}

short CLead::GetEnableTwainFeeder()
{
   short result;
   GetProperty(dispidEnableTwainFeeder, VT_I2, (void *)&result);
   return result;
}

void CLead::SetEnableTwainFeeder(short nNewValue)
{
   SetProperty(dispidEnableTwainFeeder, VT_I2, nNewValue);
}

short CLead::GetEnableTwainAutoFeed()
{
   short result;
   GetProperty(dispidEnableTwainAutoFeed, VT_I2, (void *)&result);
   return result;
}

void CLead::SetEnableTwainAutoFeed(short nNewValue)
{
   SetProperty(dispidEnableTwainAutoFeed, VT_I2, nNewValue);
}

float CLead::GetPanWinX()
{
   float result;
   GetProperty(dispidPanWinX, VT_R4, (void *)&result);
   return result;
}

void CLead::SetPanWinX(float newValue)
{
   SetProperty(dispidPanWinX, VT_R4, newValue);
}

float CLead::GetPanWinY()
{
   float result;
   GetProperty(dispidPanWinY, VT_R4, (void *)&result);
   return result;
}

void CLead::SetPanWinY(float newValue)
{
   SetProperty(dispidPanWinY, VT_R4, newValue);
}

float CLead::GetPanWinWidth()
{
   float result;
   GetProperty(dispidPanWinWidth, VT_R4, (void *)&result);
   return result;
}

void CLead::SetPanWinWidth(float newValue)
{
   SetProperty(dispidPanWinWidth, VT_R4, newValue);
}

float CLead::GetPanWinHeight()
{
   float result;
   GetProperty(dispidPanWinHeight, VT_R4, (void *)&result);
   return result;
}

void CLead::SetPanWinHeight(float newValue)
{
   SetProperty(dispidPanWinHeight, VT_R4, newValue);
}

OLE_COLOR CLead::GetPanWinRectColor()
{
   OLE_COLOR result;
   GetProperty(dispidPanWinRectColor, VT_COLOR, (void *)&result);
   return result;
}

void CLead::SetPanWinRectColor(OLE_COLOR newValue)
{
   SetProperty(dispidPanWinRectColor, VT_COLOR, newValue);
}

CString CLead::GetPanWinTitle()
{
   CString result;
   GetProperty(dispidPanWinTitle, VT_BSTR, (void *)&result);
   return result;
}

void CLead::SetPanWinTitle(LPCTSTR newValue)
{
   SetProperty(dispidPanWinTitle, VT_BSTR, newValue);
}

BOOL CLead::GetPanWinSysMenu()
{
   BOOL result;
   GetProperty(dispidPanWinSysMenu, VT_BOOL, (void *)&result);
   return result;
}

void CLead::SetPanWinSysMenu(BOOL newValue)
{
   SetProperty(dispidPanWinSysMenu, VT_BOOL, newValue);
}

short CLead::GetPanWinPaintPalette()
{
   short result;
   GetProperty(dispidPanWinPaintPalette, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPanWinPaintPalette(short newValue)
{
   SetProperty(dispidPanWinPaintPalette, VT_I2, newValue);
}

short CLead::GetPanWinBitonalScaling()
{
   short result;
   GetProperty(dispidPanWinBitonalScaling, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPanWinBitonalScaling(short newValue)
{
   SetProperty(dispidPanWinBitonalScaling, VT_I2, newValue);
}

short CLead::GetPanWinPaintDither()
{
   short result;
   GetProperty(dispidPanWinPaintDither, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPanWinPaintDither(short newValue)
{
   SetProperty(dispidPanWinPaintDither, VT_I2, newValue);
}

LPPICTUREDISP CLead::GetPanWinIcon()
{
   LPPICTUREDISP result;
   GetProperty(dispidPanWinIcon, VT_PICTURE, (void *)&result);
   return result;
}

void CLead::SetPanWinIcon(LPPICTUREDISP pPicture)
{
   SetProperty(dispidPanWinIcon, VT_PICTURE, pPicture);
}

short CLead::GetPanWinPointer()
{
   short result;
   GetProperty(dispidPanWinPointer, VT_I2, (void *)&result);
   return result;
}

void CLead::SetPanWinPointer(short newValue)
{
   SetProperty(dispidPanWinPointer, VT_I2, newValue);
}

LPPICTUREDISP CLead::GetPanWinCursor()
{
   LPPICTUREDISP result;
   GetProperty(dispidPanWinCursor, VT_PICTURE, (void *)&result);
   return result;
}

void CLead::SetPanWinCursor(LPPICTUREDISP newValue)
{
   SetProperty(dispidPanWinCursor, VT_PICTURE, newValue);
}

short CLead::GetInfoTotalPages()
{
   short result;
   GetProperty(dispidInfoTotalPages, VT_I2, (void *)&result);
   return result;
}

void CLead::SetInfoTotalPages(short newValue)
{
   SetProperty(dispidInfoTotalPages, VT_I2, newValue);
}

short CLead::GetSavePage()
{
   short result;
   GetProperty(dispidSavePage, VT_I2, (void *)&result);
   return result;
}

void CLead::SetSavePage(short newValue)
{
   SetProperty(dispidSavePage, VT_I2, newValue);
}







// methods
short CLead::WindowLevel()
{
   short result;

   InvokeHelper(dispidWindowLevel, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);

   return result;
}
short CLead::GetMinMaxVal()
{
   short result;

   InvokeHelper(dispidGetMinMaxVal, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);

   return result;
}
short CLead::GetMinMaxBits()
{
   short result;

   InvokeHelper(dispidGetMinMaxBits, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);

   return result;
}

short CLead::AnnLock(OLE_HANDLE hObject, LPCTSTR pszKey, BOOL fSelected)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE VTS_BSTR VTS_BOOL ;

   InvokeHelper(dispidAnnLock, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject, pszKey, fSelected);

   return result;
}

short CLead::AnnUnlock(OLE_HANDLE hObject, LPCTSTR pszKey)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE VTS_BSTR ;

   InvokeHelper(dispidAnnUnlock, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject, pszKey);

   return result;
}

BOOL CLead::AnnGetLocked(OLE_HANDLE hObject)
{
   BOOL result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE;

   InvokeHelper(dispidAnnGetLocked, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      hObject);

   return result;
}

short CLead::AnnUnrealize(OLE_HANDLE hObject, BOOL fSelected)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE VTS_BOOL ;

   InvokeHelper(dispidAnnUnrealize, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject, fSelected);

   return result;
}

short CLead::AnnSaveTag(BOOL fWangMode, BOOL fSelected)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_BOOL VTS_BOOL ;

   InvokeHelper(dispidAnnSaveTag, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      fWangMode, fSelected);

   return result;
}

short CLead::AnnSetHyperlinkString(OLE_HANDLE hObject, short uType, LPCTSTR pszHyperlink)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE VTS_I2 VTS_BSTR ;

   InvokeHelper(dispidAnnSetHyperlinkString, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject, uType, pszHyperlink);

   return result;
}

short CLead::AnnSetHyperlinkMsg(OLE_HANDLE hObject, short uType, long lParam1, long lParam2, long lParam3, long lParam4, long lParam5)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;

   InvokeHelper(dispidAnnSetHyperlinkMsg, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject, uType, lParam1, lParam2, lParam3, lParam4, lParam5);

   return result;
}

short CLead::AnnGetHyperlinkType(OLE_HANDLE hObject)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE;

   InvokeHelper(dispidAnnGetHyperlinkType, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hObject);

   return result;
}

CString CLead::AnnGetHyperlinkString(OLE_HANDLE hObject)
{
   CString result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE;

   InvokeHelper(dispidAnnGetHyperlinkString, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      hObject);

   return result;
}

long CLead::AnnGetHyperlinkParam(OLE_HANDLE hObject, short iParamIndex)
{
   long result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE VTS_I2 ;

   InvokeHelper(dispidAnnGetHyperlinkParam, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      hObject, iParamIndex);

   return result;
}

short CLead::PreLoadFilters(short iFixedFilters, short iCachedFilters, LPCTSTR pszFilters)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_I2 VTS_I2 VTS_BSTR ;

   InvokeHelper(dispidPreLoadFilters, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      iFixedFilters, iCachedFilters, pszFilters);

   return result;
}

short CLead::IgnoreFilters(LPCTSTR pszFilters)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_BSTR;

   InvokeHelper(dispidIgnoreFilters, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszFilters);

   return result;
}

short CLead::ZoomIn()
{
   short result;

   InvokeHelper(dispidZoomIn, DISPATCH_METHOD, VT_I2, (void*)&result, NULL );

   return result;
}

short CLead::TwainCloseSession(OLE_HANDLE hWnd)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_HANDLE;

   InvokeHelper(dispidTwainCloseSession, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      hWnd);

   return result;
}


OLE_COLOR CLead::GetLevelLUT(long iIndex)
{
   OLE_COLOR   result;
   static BYTE BASED_CODE parms[] = 
      VTS_I4  ;

   InvokeHelper(dispidLevelLUT, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      iIndex);
   return result;
} 

void CLead::SetLevelLUT(long iIndex, OLE_COLOR param1)
{
   static BYTE BASED_CODE parms[] = 
      VTS_I2 VTS_I4  ;

   InvokeHelper(dispidLevelLUT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      iIndex, param1);
}

short CLead::ShowPanWin(BOOL bShow)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_BOOL;
   
   InvokeHelper(dispidShowPanWin, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      bShow);
   
   return result;
}

short CLead::DeletePage(LPCTSTR pszName, short iPage)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_BSTR VTS_I2;
   
   InvokeHelper(dispidDeletePage, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszName, iPage);
   
   return result;
}

short CLead::Picturize(LPCTSTR pszDirectory, short iFlags, float fCellWidth, float fCellHeight)
{
   short result;
   static BYTE BASED_CODE parms[] =
      VTS_BSTR VTS_I2 VTS_R4 VTS_R4;

   InvokeHelper(dispidPicturize, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      pszDirectory, iFlags, fCellWidth, fCellHeight);

   return result;
}

