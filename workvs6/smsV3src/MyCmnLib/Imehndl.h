
#ifndef __IMEHNDL_H
#define __IMEHNDL_H               // Prevent multiple includes

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ime.h"

class AFX_CLASS_EXPORT CImeHndl
{
public:
	HANDLE			m_hIME;
	LPIMESTRUCT		m_lpIme;
	long			m_nSeting;

	CImeHndl();
	virtual ~CImeHndl();
	void ImeHndlOn(HWND hWnd);
	void ImeHndlOff(HWND hWnd);
	void ImeHndlKanji(HWND hWnd);
	void ImeHndlKana(HWND hWnd);
	void ImeHndlNon(HWND hWnd);
};

#endif

