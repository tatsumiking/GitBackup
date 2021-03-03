
//
//
// eswin  imehndl.cpp  “ú–{Œê‚©‚ÈŠ¿ŽšŠÖŒWŠÖ”
//
//  Copyright (c) 1999 by THEON

#include "stdafx.h"

#include "imehndl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
}

CImeHndl::CImeHndl()
{
    m_hIME  = GlobalAlloc(GHND, (LONG)sizeof(IMESTRUCT));
	if(m_hIME != 0){
		m_lpIme = (LPIMESTRUCT)GlobalLock(m_hIME);
	}
}

CImeHndl::~CImeHndl()
{
	if(m_lpIme != NULL){
		GlobalUnlock(m_hIME);
	}
	if(m_hIME != 0){
		GlobalFree(m_hIME);
	}
}

void CImeHndl::ImeHndlOn(HWND hWnd)
{
	if(m_lpIme == NULL){
		return;
	}
	m_lpIme->fnc    = IME_SETOPEN;
   	m_lpIme->wParam = 1;
	::SendIMEMessageEx(hWnd, (LPARAM)m_hIME);

	m_lpIme->fnc    = IME_GETCONVERSIONMODE;
   	m_lpIme->wParam = 0;
	::SendIMEMessageEx(hWnd, (LPARAM)m_hIME);
   	m_nSeting = m_lpIme->wParam;
}

void CImeHndl::ImeHndlOff( HWND hWnd)
{
	if(m_lpIme == NULL){
		return;
	}
	m_lpIme->fnc    = IME_SETCONVERSIONMODE;
	m_lpIme->wParam = m_nSeting;
	::SendIMEMessageEx(hWnd, (LPARAM)m_hIME);

	m_lpIme->fnc    = IME_SETOPEN;
   	m_lpIme->wParam = 0;
	::SendIMEMessageEx(hWnd, (LPARAM)m_hIME);
}

void CImeHndl::ImeHndlKanji(HWND hWnd)
{
	if(m_lpIme == NULL){
		return;
	}
	m_lpIme->fnc    = IME_SETCONVERSIONMODE;
	m_lpIme->wParam = (m_nSeting & IME_MODE_ROMAN) | IME_MODE_DBCSCHAR | IME_MODE_HIRAGANA;
	::SendIMEMessageEx(hWnd, (LPARAM)m_hIME);
}

void CImeHndl::ImeHndlKana(HWND hWnd)
{
	long	mode;

	if(m_lpIme == NULL){
		return;
	}
	m_lpIme->fnc    = IME_SETCONVERSIONMODE;
	m_lpIme->wParam = (m_nSeting & IME_MODE_ROMAN) | IME_MODE_SBCSCHAR | IME_MODE_KATAKANA;
	::SendIMEMessageEx(hWnd, (LPARAM)m_hIME);
}

void CImeHndl::ImeHndlNon(HWND hWnd)
{
	long	mode;

	if(m_lpIme == NULL){
		return;
	}
	m_lpIme->fnc    = IME_SETCONVERSIONMODE;
	m_lpIme->wParam = IME_MODE_SBCSCHAR | IME_MODE_ALPHANUMERIC;
	::SendIMEMessageEx(hWnd, (LPARAM)m_hIME);
}
